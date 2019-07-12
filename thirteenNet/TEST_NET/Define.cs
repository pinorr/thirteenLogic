using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.IO;
using System.Xml;
using System.Runtime.Serialization.Formatters.Binary;

namespace TEST_NET
{    
    struct DEF
    {
        public const uint CT_INVALID					= 0	;							// 错误类型
        public const uint CT_SINGLE						= 1	;							// 单牌类型
        public const uint CT_ONE_DOUBLE					= 2	;							// 只有一对
        public const uint CT_TWO_DOUBLE					= 3	;							// 两对牌型
        public const uint CT_THREE						= 4	;							// 三张牌型
        public const uint CT_FIVE_MIXED_FLUSH_NO_A		= 6	;							// 普通顺子
        public const uint CT_FIVE_FLUSH					= 8	;							// 同花
        public const uint CT_FIVE_THREE_DEOUBLE			= 9	;							// 三条一对
        public const uint CT_FIVE_FOUR_ONE				= 10;							// 四带一张
        public const uint CT_FIVE_STRAIGHT_FLUSH		= 12;							// 同花顺牌
        public const uint CT_FIVE_MAX					= 14;							//    
    }

    public static class HelpFun
    {
        //获取数值
        public static byte GetCardValue(byte cbCardData)
        {
            return (byte)(cbCardData & 0x0F);
        }
        //获取花色
        public static byte GetCardColor(byte cbCardData)
        {
            return (byte)((cbCardData & 0xF0) >> 4);
        }
        //逻辑数值
        public static byte GetCardLogicValue(byte cbCardData)
        {
            byte byVal = GetCardValue(cbCardData);
            if (byVal <= 1) byVal = 14;
            return (byte)(byVal - 2);
        }
        public static byte GetCard(byte cor, byte index)
        {
            if (cor > 3 || index > 13) return 0;
            if (index == 12) index = 1;
            else index += 2;
            return (byte)(cor * 0x10 + index);
        }
        public static bool isSameCards(byte[] byCards, stAnswer answer)
        {
            byte[] bySrc = new byte[13];
            byte[] byTemp = new byte[13];
            for (int i = 0; i < 13; ++i)
            {
                bySrc[i] = byCards[i];
            }
            answer.getData(byTemp);
            for (int i = 0; i < 13; ++i)
            {
                bool bFind = false;
                for (int n = 0; n < 13; ++n)
                    if (byTemp[n] == byCards[i])
                    {
                        bFind = true;
                        break;
                    }
                if (bFind == false)
                    return false;
            }
            return true;
        }
        public static bool cmp(ref stAnswer temp1, ref stAnswer temp2)
        {
            // [3,DEF.CT_FIVE_MAX]
            float[,] fPer = new float[3, 14]{
		        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 5, 5, 5,},
		        {0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 8, 10, 10, 10,},
		        {0, 1, 1, 3, 30, 1, 1, 1, 1, 1, 1, 1, 1, 1,},
	        };
            uint[] t1 = new uint[3] { temp1.dwVal[0] >> 22, temp1.dwVal[1] >> 22, temp1.dwVal[2] >> 22 };
            uint[] t2 = new uint[3] { temp2.dwVal[0] >> 22, temp2.dwVal[1] >> 22, temp2.dwVal[2] >> 22 };
            float fFlag1 = 0, fFlag2 = 0;
            for (int i = 0; i < 3; ++i)
            {
                fFlag1 += fPer[i, t1[i]];
                fFlag2 += fPer[i, t2[i]];
            }
            return fFlag1 > fFlag2;
        }
        public static T DeepCopy<T>(T obj)
        {
            object retval;
            using (MemoryStream ms = new MemoryStream())
            {
                BinaryFormatter bf = new BinaryFormatter();
                //序列化成流
                bf.Serialize(ms, obj);
                ms.Seek(0, SeekOrigin.Begin);
                //反序列化成对象
                retval = bf.Deserialize(ms);
                ms.Close();
            }
            return (T)retval;
        }
    }

    [Serializable]
    public class stAnswer
    {
        public byte byNum;
        public byte[,] byCards = new byte[3, 5];
        public uint[] dwVal = new uint[3];
        public bool[] bRepair = new bool[2];

        //public bool	repairNote(ref byte byData[13,5]);
        public bool repairNote(ref byte[,] byData)
        {
            if (byNum != 2)
                return false;

            if (byCards[0, 4] != 0 && byCards[1, 4] != 0)
                return true;

            List<byte> vctCards = new List<byte>();
            //单牌 对子 三张 四张
            for (int i = 12; i >= 0; --i) {
                for (byte n = 0; n < 4; ++n) {
                    if (byData[i, n] == 1)
                        vctCards.Add(HelpFun.GetCard(n, (byte)i));
                }
            }                

            int nDel = 0;
            if (byCards[0, 4] == 0)
            {
                if (vctCards.Count() == 0) return false;
                byte byTemp = vctCards[nDel++];
                byCards[0, 4] = byTemp;

                byte val = (byte)(byTemp % 0x10);
                byte cor = (byte)(byTemp / 0x10);
                val -= 2;
                if (val == 255) val = 12;
                --byData[val, 4];
                --byData[val, cor];
                bRepair[0] = true;
            }
            if (byCards[1, 4] == 0)
            {
                if (vctCards.Count == 0) return false;
                byte byTemp = vctCards[nDel++];
                byCards[1, 4] = byTemp;

                byte val = (byte)(byTemp % 0x10);
                byte cor = (byte)(byTemp / 0x10);
                val -= 2;
                if (val == 255) val = 12;
                --byData[val, 4];
                --byData[val, cor];
                bRepair[1] = true;
            }
            return true;
        }
        public void undo_repair(ref byte[,] byData)
        {
            for (int i = 0; i < 2; ++i)
            {
                if (bRepair[i])
                {
                    byte val = (byte)(byCards[i, 4] % 0x10);
                    byte cor = (byte)(byCards[i, 4] / 0x10);
                    val -= 2;
                    if (val == 255) val = 12;
                    ++byData[val, 4];
                    ++byData[val, cor];
                    bRepair[i] = false;
                    byCards[i, 4] = 0;
                }
            }
        }
        public uint push(List<byte> cards, ref byte[,] byData)
        {
            while (cards.Count < 5) {
                cards.Add(0);
            }
            if (byNum >= 3) return 0;
            int len = (byNum == 2) ? 3 : 5;
            byte[] temp = new byte[5];
            for (int i = 0; i < len; ++i)
	        {
                byCards[byNum, i] = cards[i];
                temp[i] = cards[i];
            }
            uint dwTemp = GameLogic.GetLogicValue(temp, (byte)len);
	        for (int i = 0; i < byNum; ++i) {
		        if (dwVal[i] < dwTemp)
			        return 0;
	        }        
	        for (int i = 0; i < len; ++i)
	        {
		        if (cards[i] > 0) {
                    byte val = (byte)(cards[i] % 0x10);
		            byte cor = (byte)(cards[i] / 0x10);
			        val -= 2;
			        if (val == 255) val = 12;
			        --byData[val,4];
			        --byData[val,cor];
			        Trace.Assert(byData[val,4] <= 4);
		        }
	        }
	        dwVal[byNum] = dwTemp;
	        ++byNum;
	        return dwTemp;
        }
        public void pop(ref byte[,] byData)
        {
            if (byNum == 0) return;
            int len = (byNum == 3) ? 3 : 5;
            --byNum;
            for (int i = 0; i < len; ++i)
            {
                if (byCards[byNum, i] > 0)
                {
                    byte val = (byte)(byCards[byNum, i] % 0x10);
                    byte cor = (byte)(byCards[byNum, i] / 0x10);
                    val -= 2;
                    if (val == 255) val = 12;
                    ++byData[val, 4];
                    ++byData[val, cor];
                }
            }
        }
        public void getData(byte[] cards)
        {
            if (byNum == 3)
            {
                for (int i = 0; i < 13; ++i)
                {
                    if (i < 3) cards[i] = byCards[2, i];
                    else if (i < 8) cards[i] = byCards[2, i - 3];
                    else cards[i] = byCards[2, i - 8];
                }
            }
        }
    };
}

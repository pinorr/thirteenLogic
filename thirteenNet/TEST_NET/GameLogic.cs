using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace TEST_NET
{
    class GameLogic
    {
        //核心函数 （用递归+回溯法 计算所有可能组合） 
        //vctAnswer：    所有结果列表      
        //dwScoreMax：   阀值分      
        //answer：       记录当前答案 
        //byData:        当前剩余牌数量记录         [i, 0-3] 第i张牌的黑红梅方 是否存在           [i, 4] 第i张牌总数量
	    public static void TryCards(ref byte[,] byData, ref stAnswer answer, uint dwScoreMax, ref List<stAnswer> vctAnswer)
        {
            byte lineNum = 0;
            List<byte> vctTemp = new List<byte>();
	        byte nCardsNum = 0;
	        for (byte i = 0; i < 14; ++i)
		        nCardsNum += byData[i,4];

            // 注意：byData[13, 4] 表示当前赖子牌总数量
	        if (answer.byNum < 2)
	        {
		        if (dwScoreMax >= DEF.CT_FIVE_STRAIGHT_FLUSH << 22)	{
			        //同花顺
			        for (byte i = 0; i < 13 - 4; ++i)
			        {
				        for (byte n = 0; n < 4; ++n) {
					        if (byData[i,n] + byData[i + 1,n] + byData[i + 2,n] + byData[i + 3,n] + byData[i + 4,n] + byData[13, 4] >= 5)
					        {				
						        vctTemp.Clear();
						        for (byte id = 0; id < 5; ++id) {
							        if (byData[i + id,n] > 0) {
								        vctTemp.Add(HelpFun.GetCard(n, (byte)(i + id)));
							        }
						        }
						        if (byData[13,0] > 0) vctTemp.Add(HelpFun.GetCard(0, 13));
						        if (byData[13,1] > 0) vctTemp.Add(HelpFun.GetCard(1, 13));
						        uint dwVal = answer.push(vctTemp, ref byData);
						        if (dwVal > 0) {
							        TryCards(ref byData, ref answer, dwVal, ref vctAnswer);
							        answer.pop(ref byData);
						        }
					        }
				        }
			        }
		        }
		        if (dwScoreMax >= DEF.CT_FIVE_FOUR_ONE << 22) {
			        //四张
			        for (byte i = 0; i < 13; ++i)
			        {
				        if (byData[i,4] + byData[13, 4] >= 4) {
					        vctTemp.Clear();
					        for (byte id = 0; id < 4; ++id) {
						        if (byData[i,id] > 0) {
							        vctTemp.Add(HelpFun.GetCard(id, i));
						        }
					        }
					        if (byData[13,0] > 0) vctTemp.Add(HelpFun.GetCard(0, 13));
					        if (byData[13,1] > 0) vctTemp.Add(HelpFun.GetCard(1, 13));
                            if (vctTemp.Count >= 5) {
                                vctTemp[4] = 0;
                            }
                            uint dwVal = answer.push(vctTemp, ref byData);
                            if (dwVal > 0)
                            {
                                TryCards(ref byData, ref answer, dwVal, ref vctAnswer);
                                answer.pop(ref byData);
                            }
				        }
			        }
		        }
		        if (dwScoreMax >= DEF.CT_FIVE_THREE_DEOUBLE << 22) {
			        //三带二
			        for (byte i = 0; i < 13; ++i)
			        {
				        if (byData[13, 4] + byData[i,4] >= 3 && byData[13, 4] <= 1)
				        {
					        byte j = 0;
					        for (; j < 13; ++j) 
                            {
                                if (j != i && (byData[j, 4] == 2 || byData[j, 4] == 3))
                                {
                                    vctTemp.Clear();
                                    // 三张
                                    for (byte id = 0; id < 4; ++id)
                                    {
                                        if (byData[i, id] > 0)
                                            vctTemp.Add(HelpFun.GetCard(id, i));
                                    }
                                    // 这里最多只加一个赖子牌
                                    if (vctTemp.Count == 2)
                                    {
                                        if (byData[13, 0] > 0)
                                            vctTemp.Add(HelpFun.GetCard(0, 13));
                                        else if (byData[13, 1] > 0)
                                            vctTemp.Add(HelpFun.GetCard(1, 13));
                                    }
                                    // 小对
                                    for (byte id = 0; id < 4; ++id)
                                    {
                                        if (byData[j, id] > 0)
                                            vctTemp.Add(HelpFun.GetCard(id, j));
                                    }
                                    uint dwVal = answer.push(vctTemp, ref byData);
                                    if (dwVal > 0)
                                    {
                                        TryCards(ref byData, ref answer, dwVal, ref vctAnswer);
                                        answer.pop(ref byData);
                                    }
                                }
					        }
				        }
			        }
		        }
		        if (dwScoreMax >= DEF.CT_FIVE_FLUSH << 22) {
			        //同花
			        for (byte n = 0; n < 4; ++n)
			        {
				        List<byte> vctCards = new List<byte>();
				        for (byte i = 0; i < 13; ++i) {
					        if (byData[i,n] == 1)
						        vctCards.Add(HelpFun.GetCard(n, i));
				        }
				        if (byData[13,0] > 0) vctCards.Add(HelpFun.GetCard(0, 13));
				        if (byData[13,1] > 0) vctCards.Add(HelpFun.GetCard(1, 13));
				        if (vctCards.Count >= 5)
				        {
                            for (int i1 = 0; i1 < vctCards.Count; ++i1)
                                for (int i2 = i1 + 1; i2 < vctCards.Count; ++i2)
                                    for (int i3 = i2 + 1; i3 < vctCards.Count; ++i3)
                                        for (int i4 = i3 + 1; i4 < vctCards.Count; ++i4)
                                            for (int i5 = i4 + 1; i5 < vctCards.Count; ++i5)
									        {
                                                vctTemp.Clear();
                                                vctTemp.Add(vctCards[i1]);
                                                vctTemp.Add(vctCards[i2]);
                                                vctTemp.Add(vctCards[i3]);
                                                vctTemp.Add(vctCards[i4]);
                                                vctTemp.Add(vctCards[i5]);
                                                uint dwVal = answer.push(vctTemp, ref byData);
                                                if (dwVal > 0)
                                                {
                                                    TryCards(ref byData, ref answer, dwVal, ref vctAnswer);
                                                    answer.pop(ref byData);
                                                }
									        }
				        }
			        }
		        }
		        if (dwScoreMax >= DEF.CT_FIVE_MIXED_FLUSH_NO_A << 22) {
			        //顺子
			        for (byte i = 0; i < 13 - 4; ++i)
			        {
				        lineNum = 0;
				        lineNum += byData[i,4] > 0 ? (byte)1 : (byte)0;
				        lineNum += byData[i + 1,4] > 0 ? (byte)1 : (byte)0;
				        lineNum += byData[i + 2,4] > 0 ? (byte)1 : (byte)0;
				        lineNum += byData[i + 3,4] > 0 ? (byte)1 : (byte)0;
				        lineNum += byData[i + 4,4] > 0 ? (byte)1 : (byte)0;
				        if (lineNum + byData[13, 4] >= 5)
				        {
					        vctTemp.Clear();
					        for (byte x = 0; x < 5; ++x) {
						        for (byte n = 0; n < 4; ++n) {
							        if (byData[i + x,n] == 1) {
								        vctTemp.Add(HelpFun.GetCard(n, (byte)(i + x)));
								        break;
							        }
						        }
					        }
					        if (byData[13,0] > 0) vctTemp.Add(HelpFun.GetCard(0, 13));
					        if (byData[13,1] > 0) vctTemp.Add(HelpFun.GetCard(1, 13));
                            uint dwVal = answer.push(vctTemp, ref byData);
                            if (dwVal > 0)
                            {
                                TryCards(ref byData, ref answer, dwVal, ref vctAnswer);
                                answer.pop(ref byData);
                            }
				        }
			        }
			        // 2 3 4 5 A
			        lineNum = 0;
			        lineNum += byData[0,4] > 0 ? (byte)1 : (byte)0;
			        lineNum += byData[1,4] > 0 ? (byte)1 : (byte)0;
			        lineNum += byData[2,4] > 0 ? (byte)1 : (byte)0;
			        lineNum += byData[3,4] > 0 ? (byte)1 : (byte)0;
			        lineNum += byData[12,4] > 0 ? (byte)1 : (byte)0;
			        if (lineNum + byData[13, 4] >= 5)
			        {
				        vctTemp.Clear();
				        for (byte x = 0; x < 4; ++x) {
					        for (byte n = 0; n < 4; ++n) {
						        if (byData[x,n] == 1) {
							        vctTemp.Add(HelpFun.GetCard(n, x));
							        break;
						        }
					        }
				        }
				        for (byte n = 0; n < 4; ++n) {
					        if (byData[12,n] == 1) {
						        vctTemp.Add(HelpFun.GetCard(n, 12));
						        break;
					        }
				        }
				        if (byData[13,0] > 0) vctTemp.Add(HelpFun.GetCard(0, 13));
				        if (byData[13,1] > 0) vctTemp.Add(HelpFun.GetCard(1, 13));
                        uint dwVal = answer.push(vctTemp, ref byData);
                        if (dwVal > 0)
                        {
                            TryCards(ref byData, ref answer, dwVal, ref vctAnswer);
                            answer.pop(ref byData);
                        }
			        }
		        }
                if (dwScoreMax >= DEF.CT_THREE << 22)
                {
			        //三张
			        for (int i = 12; i >= 0; --i)
			        {
				        if (byData[i,4] <= 3 && byData[i,4] + byData[13, 4] >= 3) {
					        vctTemp.Clear();
					        for (byte n = 0; n < 4; ++n) {
						        if (byData[i,n] == 1)
							        vctTemp.Add(HelpFun.GetCard(n, (byte)i));
					        }
					        if (vctTemp.Count == 2) {
						        if (byData[13,0] > 0)
							        vctTemp.Add(HelpFun.GetCard(0, 13));
						        else if (byData[13,1] > 0)
							        vctTemp.Add(HelpFun.GetCard(1, 13));
					        }
					        else if (vctTemp.Count == 1) {
						        if (byData[13,0] > 0) vctTemp.Add(HelpFun.GetCard(0, 13));
						        if (byData[13,1] > 0) vctTemp.Add(HelpFun.GetCard(1, 13));
					        }
					        Trace.Assert(vctTemp.Count == 3);
					        //再选两个最小单牌
					        for (byte j = 0; j < 13; ++j) {
						        if (byData[j,4] == 1 && i != j) {
							        for (byte n = 0; n < 4; ++n) {
								        if (byData[j,n] == 1)
									        vctTemp.Add(HelpFun.GetCard(n, j));
							        }
						        }
						        if (vctTemp.Count >= 5) break;
					        }
					        if (vctTemp.Count >= 5) {
                                uint dwVal = answer.push(vctTemp, ref byData);
                                if (dwVal > 0)
                                {
                                    TryCards(ref byData, ref answer, dwVal, ref vctAnswer);
                                    answer.pop(ref byData);
                                }
					        }
				        }
			        }
		        }
		        List<byte> vctPairs = new List<byte>();
                List<byte> vctSingle = new List<byte>();
		        for (byte i = 0; i < 13; ++i)
		        {
			        if (byData[i,4] == 2)
				        vctPairs.Add(i);
			        else if (byData[i,4] == 1)
				        vctSingle.Add(i);
		        }

                if (dwScoreMax >= DEF.CT_TWO_DOUBLE << 22)
                {
			        //两对
			        if (byData[13, 4] == 0 && vctPairs.Count >= 2)
			        {
				        //多对子时，最大的对子放到最上面（三张的那组）
				        byte byMax = (byte)(vctPairs.Count - 1);
				        if (answer.byNum == 0) {
					        if (vctPairs.Count >= 5)
						        byMax -= 1;
					        else if (vctPairs.Count >= 4)
						        byMax -= 2;
				        }
				        else if (vctPairs.Count >= 3) {
					        byMax -= 1;
				        }
				        vctTemp.Clear();
				        for (byte n = 0; n < 4; ++n) {
					        if (byData[vctPairs[byMax],n] == 1)
						        vctTemp.Add(HelpFun.GetCard(n, vctPairs[byMax]));
				        }
                        byMax--;
				        for (byte n = 0; n < 4; ++n) {
                            if (byData[vctPairs[byMax], n] == 1)
                                vctTemp.Add(HelpFun.GetCard(n, vctPairs[byMax]));
				        }					

				        //再选一个最小单牌
				        for (byte i = 0; i < 13; ++i)
				        {
					        if (byData[i,4] == 1) {
						        for (byte n = 0; n < 4; ++n) {
							        if (byData[i,n] == 1)
								        vctTemp.Add(HelpFun.GetCard(n, i));
						        }							
					        }
					        if (vctTemp.Count >= 5) break;
				        }
                        uint dwVal = answer.push(vctTemp, ref byData);
                        if (dwVal > 0)
                        {
                            TryCards(ref byData, ref answer, dwVal, ref vctAnswer);
                            answer.pop(ref byData);
                        }
			        }
		        }

                if (dwScoreMax >= DEF.CT_ONE_DOUBLE << 22)
                {
			        //一对			 
			        if ((vctPairs.Count > 0 && byData[13, 4] == 0) || (vctPairs.Count == 0 && vctSingle.Count > 0 && byData[13, 4] == 1))
			        {
				        vctTemp.Clear();
				        if (byData[13, 4] == 0) {
					        byte byMax = (byte)(vctPairs.Count - 1);
					        for (byte n = 0; n < 4; ++n) {
						        if (byData[vctPairs[byMax],n] == 1)
							        vctTemp.Add(HelpFun.GetCard(n, vctPairs[byMax]));
					        }
				        }				
				        else if (byData[13, 4] == 1) {
                            byte byMax = (byte)(vctSingle.Count - 1);
					        for (byte n = 0; n < 4; ++n) {
						        if (byData[vctSingle[byMax],n] == 1) {
							        vctTemp.Add(HelpFun.GetCard(n, vctSingle[byMax]));
							        break;
						        }							
					        }
					        if (vctTemp.Count == 1) {
						        if (byData[13,0] > 0) vctTemp.Add(HelpFun.GetCard(0, 13));
						        else if (byData[13,1] > 0) vctTemp.Add(HelpFun.GetCard(1, 13));
					        }
				        }
				        Trace.Assert(vctTemp.Count == 2);
				        //再选三个最小单牌
				        for (byte i = 0; i < 13; ++i)
				        {
					        if (byData[i,4] == 1) {
						        for (byte n = 0; n < 4; ++n) {
							        if (byData[i,n] == 1)
								        vctTemp.Add(HelpFun.GetCard(n, i));
						        }							
					        }
					        if (vctTemp.Count >= 5) break;
				        }
				        if (vctTemp.Count == 5) {
                            uint dwVal = answer.push(vctTemp, ref byData);
                            if (dwVal > 0)
                            {
                                TryCards(ref byData, ref answer, dwVal, ref vctAnswer);
                                answer.pop(ref byData);
                            }
				        }
			        }
		        }

                if (dwScoreMax >= DEF.CT_SINGLE << 22)
                {
			        //单牌
			        vctTemp.Clear();
			        for (byte i = 12; i > 0; --i)
			        {
				        if (byData[i,4] == 1) {
					        for (byte n = 0; n < 4; ++n) {
						        if (byData[i,n] == 1)
							        vctTemp.Add(HelpFun.GetCard(n, i));
					        }	
				        }
				        if (vctTemp.Count >= 5) break;
			        }
			        if (vctTemp.Count > 5) {
                        uint dwVal = answer.push(vctTemp, ref byData);
                        if (dwVal > 0)
                        {
                            TryCards(ref byData, ref answer, dwVal, ref vctAnswer);
                            answer.pop(ref byData);
                        }
			        }
		        }
	        }
	        else
	        {
                if (dwScoreMax >= DEF.CT_THREE << 22)
                {
			        //三张
			        for (byte i = 0; i < 13; ++i)
			        {
				        if (byData[i,4] + byData[13, 4] >= 3) {
					        vctTemp.Clear();
					        for (byte n = 0; n < 4; ++n) {
						        if (byData[i,n] == 1)
							        vctTemp.Add(HelpFun.GetCard(n, i));
					        }
					        if (byData[13,0] > 0) vctTemp.Add(HelpFun.GetCard(0, 13));
					        if (byData[13,1] > 0) vctTemp.Add(HelpFun.GetCard(1, 13));
                            uint dwVal = answer.push(vctTemp, ref byData);
                            if (dwVal > 0)
                            {
                                answer.repairNote(ref byData);
                                vctAnswer.Add(HelpFun.DeepCopy<stAnswer>(answer));
                                answer.pop(ref byData);
                                answer.undo_repair(ref byData);
                            }
				        }
			        }
		        }

                if (dwScoreMax >= DEF.CT_ONE_DOUBLE << 22)
                {	
			        // 一对
			        for (byte i = 0; i < 13; ++i)
			        {
				        if (byData[i,4] <= 2 && byData[13, 4] < 2 && byData[i,4] + byData[13, 4] >= 2) {
					        vctTemp.Clear();
					        for (byte n = 0; n < 4; ++n) {
						        if (byData[i,n] == 1)
							        vctTemp.Add(HelpFun.GetCard(n, i));
					        }
					        if (vctTemp.Count == 1) {
						        if (byData[13,0] > 0)	
							        vctTemp.Add(HelpFun.GetCard(0, 13));
						        else if (byData[13,1] > 0) 
							        vctTemp.Add(HelpFun.GetCard(1, 13));
					        }	
					        Trace.Assert(vctTemp.Count == 2);
					        //再选一个最大单牌
					        for (int j = 12; j >= 0; --j) {
                                for (byte n = 0; n < 4; ++n) {
                                    if (byData[j, n] == 1 && j != i) {
                                        vctTemp.Add(HelpFun.GetCard(n, (byte)j));
                                        break;
                                    }
                                }						        
						        if (vctTemp.Count >= 3) break;
					        }
                            if (vctTemp.Count >= 3) {
                                uint dwVal = answer.push(vctTemp, ref byData);
                                if (dwVal > 0)
                                {
                                    answer.repairNote(ref byData);
                                    vctAnswer.Add(HelpFun.DeepCopy<stAnswer>(answer));
                                    answer.pop(ref byData);
                                    answer.undo_repair(ref byData);
                                }
                            }
				        }
			        }
		        }

		        if (dwScoreMax >= DEF.CT_SINGLE << 22) {
			        //单牌
			        vctTemp.Clear();
			        for (byte i = 0; i < 13; ++i)
			        {
				        if (byData[i,4] == 1) {
					        for (byte n = 0; n < 4; ++n) {
						        if (byData[i,n] == 1) {
							        vctTemp.Add(HelpFun.GetCard(n, i));
							        break;
						        }
					        }						
				        }
			        }
			        if (vctTemp.Count >= 3) {
				        byte bySize = (byte)vctTemp.Count;
				        byte[] byCards = new byte[5]{ vctTemp[bySize - 1], vctTemp[bySize - 2], vctTemp[bySize - 3], 0, 0 };
                        uint dwVal = answer.push(vctTemp, ref byData);
                        if (dwVal > 0)
                        {
                            answer.repairNote(ref byData);
                            vctAnswer.Add(HelpFun.DeepCopy<stAnswer>(answer));
                            answer.pop(ref byData);
                            answer.undo_repair(ref byData);
                        }
			        }
		        }		        
	        }
        }
	    //
	    public static void JudgeCards(byte[] cbCardData, byte cbCount, ref List<stAnswer> vctOut)
        {
            byte[,] byData = new byte[14, 5];
	        for (byte i = 0; i < cbCount; ++i){
                byte val = byte.Parse((cbCardData[i] % 0x10).ToString());
                byte cor = byte.Parse((cbCardData[i] / 0x10).ToString());
		        val -= 2;
		        if (val == 255) val = 12;

		        ++byData[val,4];
		        ++byData[val,cor];
		        Trace.Assert(byData[val,cor] <= 1);
		        if (byData[val, cor] > 1) {
			        return;         // 输入数据有误！ 牌值重复
		        }
	        }
	        stAnswer answer = new stAnswer();
            uint dwScoreMax = DEF.CT_FIVE_MAX << 22;
	        TryCards(ref byData, ref answer, dwScoreMax, ref vctOut);
        }
	    //逻辑牌值
	    public static uint GetLogicValue(byte[] cbCardData, byte cbCardCount)
        {
            //变量定义
	        uint  nValFlag = 0;
	        uint[] nMaxID = new uint[5];	    //最大牌
	        byte[] byMaxCor = new byte[5];	    //最大牌花色
	        if (cbCardCount <= 5 && cbCardCount >= 3)
	        {
		        byte num = 0;
		        byte[] byCardVal = new byte[14];
		        byte[] byCardCol = new byte[4];
		        for (byte i = 0; i<cbCardCount; ++i)
		        {
			        if (cbCardData[i] > 0) {
                        byte byVal = HelpFun.GetCardLogicValue(cbCardData[i]);
				        ++byCardVal[byVal];
				        ++num;
				        if (byVal < 13) {
					        byte byCol = HelpFun.GetCardColor(cbCardData[i]);
					        ++byCardCol[byCol];
				        }
			        }
		        }
		        cbCardCount = num;
		        byte  nKingNum = byCardVal[13];

		        byte bySameMax = 0;
		        List<byte> vctDouble = new List<byte>();
		        //统计数据
		        for (byte i = 0; i < 13; ++i)
		        {
			        bySameMax = Math.Max(bySameMax, byCardVal[i]);
			        nMaxID[byCardVal[i]] = i;
                    if (byCardVal[i] == 2) {
                        vctDouble.Add(i);
                    }                        
		        }

		        //找到指定值最大颜色
		        for (byte i = 0; i < cbCardCount; ++i)
		        {
                    byte byVal = HelpFun.GetCardLogicValue(cbCardData[i]);
			        for (byte n = 1; n < 5; ++n) {
				        if (byVal == nMaxID[n])
                            byMaxCor[n] = Math.Max(byMaxCor[n], HelpFun.GetCardColor(cbCardData[i]));
			        }
		        }

		        //变量定义
		        bool cbSameColor = false, bLineCard = false;
		        //判断是否是顺子
		        for (int i = 0; i < 13; ++i)
		        {
			        if (i + 4 < 13)
			        {
				        int n = byCardVal[i] + byCardVal[i + 1] + byCardVal[i + 2] + byCardVal[i + 3] + byCardVal[i + 4] + nKingNum;				
				        if (n == 5 && bySameMax == 1)
					        bLineCard = true;
			        }
		        }
		        //特殊 2 3 4 5 A
		        if (byCardVal[0] + byCardVal[1] + byCardVal[2] + byCardVal[3] + byCardVal[12] + nKingNum == 5 && bySameMax == 1)
			        bLineCard = true;

		        //判断是否是同花
		        for (byte i = 0; i < 4; ++i) {
			        if (byCardCol[i] + nKingNum == 5)
				        cbSameColor = true;
		        }

		        if (cbCardCount != 5)
			        cbSameColor = false;

		        //数值比较标志
                int nMaxIndex = -1;
		        for (byte n = 4; n >= 1; --n) {
			        for (int i = (int)nMaxID[n]; i >= 0; --i) {
				        if (byCardVal[i] == n) {
                            if (nMaxIndex < 0) nMaxIndex = i;
					        for (byte m = 0; m < n; ++m)
						        nValFlag = (nValFlag << 4) + (uint)i;
				        }
			        }
		        }
                uint nValFlag1 = nValFlag;
                uint nValFlag2 = nValFlag;
		        if (nKingNum > 0) {
                    nValFlag += (13 << 16);
                    nValFlag1 += (((uint)nMaxIndex) << 16);
                    nValFlag2 += (((uint)nMaxIndex+1) << 16);
                    for (byte i=0; i<5; ++i) {
                        byMaxCor[i] = 0x03;
                    }
		        }

		        //顺金类型
		        if (cbSameColor && bLineCard)
                    return (DEF.CT_FIVE_STRAIGHT_FLUSH << 22) + (nValFlag2 << 2) + ((uint)byMaxCor[1] & 0x03);

		        //顺子类型
		        if (!cbSameColor && bLineCard)
                    return (DEF.CT_FIVE_MIXED_FLUSH_NO_A << 22) + (nValFlag2 << 2) + ((uint)byMaxCor[1] & 0x03);

		        //金花类型
		        if (cbSameColor && !bLineCard)
                    return (DEF.CT_FIVE_FLUSH << 22) + (nValFlag2 << 2) + ((uint)byMaxCor[1] & 0x03);

		        //对子和豹子判断
                if (bySameMax + nKingNum >= 4) {
                    return (DEF.CT_FIVE_FOUR_ONE << 22) + (nValFlag1 << 2) + ((uint)byMaxCor[4] & 0x03);
		        }
		        else if (bySameMax + nKingNum == 3) {
                    if ((nKingNum == 0 && vctDouble.Count == 1) || (nKingNum == 1 && vctDouble.Count == 2))
                        return (DEF.CT_FIVE_THREE_DEOUBLE << 22) + (nValFlag1 << 2) + ((uint)byMaxCor[3] & 0x03);
			        else
                        return (DEF.CT_THREE << 22) + (nValFlag1 << 2) + ((uint)byMaxCor[3] & 0x03);
		        }
		        else if (bySameMax + nKingNum == 2) {
			        if (vctDouble.Count >= 2)
                        return (DEF.CT_TWO_DOUBLE << 22) + (nValFlag1 << 2) + ((uint)byMaxCor[1] & 0x03);
			        else
                        return (DEF.CT_ONE_DOUBLE << 22) + (nValFlag1 << 2) + ((uint)byMaxCor[1] & 0x03);
		        }
	        }
	        return (DEF.CT_SINGLE << 22) + (nValFlag << 2) + ((uint)byMaxCor[1] & 0x03);
        }
	    //逻辑大小
	    public static uint GetCardType(byte[] cbCardData, byte cbCardCount)
        {
            uint dwLogicVal = GetLogicValue(cbCardData, cbCardCount);
            return (dwLogicVal >> 22);
        }
	    //大小比较
        public static bool CompareCard(byte[] cbCardData1, byte cbCount1, byte[] cbCardData2, byte cbCount2)
        {
            return GetLogicValue(cbCardData1, cbCount1) > GetLogicValue(cbCardData2, cbCount2);
        }
        //判断是否是皇家同花顺
        public static bool IsMaxTHS(byte[] cbCardData, int cbCount)
        {
            if (cbCount != 5) return false;

            byte bySameMax = 0;
            byte[] byCardVal = new byte[14];
            byte[] byCardCol = new byte[4];
            for (int i = 0; i < 5; ++i) {
                if (cbCardData[i] > 0) {
                    byte byVal = HelpFun.GetCardLogicValue(cbCardData[i]);
                    ++byCardVal[byVal];                        
                    if (byVal < 13) {
                        bySameMax = Math.Max(bySameMax, byCardVal[byVal]);
                        byte byCol = HelpFun.GetCardColor(cbCardData[i]);
                        ++byCardCol[byCol];
                    }
                }
            }
            
            byte nKingNum = byCardVal[13];
            byte nMaxColNum = 0;
            for (int i = 0; i < 4; ++i) {
                if (nMaxColNum < byCardCol[i]) nMaxColNum = byCardCol[i];
            }
            if (nMaxColNum + nKingNum != 5) 
                return false;

            int n = byCardVal[8] + byCardVal[9] + byCardVal[10] + byCardVal[11] + byCardVal[12] + nKingNum;
            if (n == 5 && bySameMax == 1)
                return true;             
            else
                return false;
        }
    };
}

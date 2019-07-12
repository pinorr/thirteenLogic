#include <iostream>
#include <assert.h>
#include "thirteen_logic.h"

bool	stAnswer::repairNote(BYTE byData[13][5])
{
	if (byNum != 2)
		return false;

	if (byCards[0][4] != 0 && byCards[1][4] != 0)
		return true;

	vector<BYTE> vctCards;
	//单牌 对子 三张 四张
	for (int m = 1; m <= 4; ++m)
		for (int i = 0; i < 13; ++i) {
			if (byData[i][4] == m) {
				for (int n = 0; n < 4; ++n)
					if (byData[i][n] == 1)
						vctCards.push_back(GetCard(n, i));
			}
		}

	int nDel = 0;
	if (byCards[0][4] == 0) {
		if (vctCards.empty()) return false;
		BYTE byTemp = vctCards[nDel++];
		byCards[0][4] = byTemp;
		//dwVal[0] = CGameLogic::GetLogicValue(byCards[0], 5);

		BYTE val = byTemp % 0x10;
		BYTE cor = byTemp / 0x10;
		val -= 2;
		if (val == 255) val = 12;
		--byData[val][4];
		--byData[val][cor];
		bRepair[0] = true;
	}
	if (byCards[1][4] == 0) {
		if (vctCards.empty()) return false;
		BYTE byTemp = vctCards[nDel++];
		byCards[1][4] = byTemp;
		//dwVal[1] = CGameLogic::GetLogicValue(byCards[1], 5);

		BYTE val = byTemp % 0x10;
		BYTE cor = byTemp / 0x10;
		val -= 2;
		if (val == 255) val = 12;
		--byData[val][4];
		--byData[val][cor];
		bRepair[1] = true;
	}
	return true;
}
void	stAnswer::undo_repair(BYTE byData[13][5])
{
	for (int i = 0; i < 2; ++i)
	{
		if (bRepair[i]) {
			BYTE val = byCards[i][4] % 0x10;
			BYTE cor = byCards[i][4] / 0x10;
			val -= 2;
			if (val == 255) val = 12;
			++byData[val][4];
			++byData[val][cor];
			bRepair[i] = false;
			byCards[i][4] = 0;
		}
	}
}
DWORD	stAnswer::push(BYTE cards[5], BYTE byData[14][5])
{
	if (byNum >= 3) return 0;
	int len = (byNum == 2) ? 3 : 5;
	DWORD dwTemp = CGameLogic::GetLogicValue(cards, len);
	for (int i = 0; i < byNum; ++i) {
		if (dwVal[i] < dwTemp)
			return 0;
	}
	memcpy(byCards[byNum], cards, len);
	for (int i = 0; i < len; ++i)
	{
		if (cards[i] > 0) {
			BYTE val = cards[i] % 0x10;
			BYTE cor = cards[i] / 0x10;
			val -= 2;
			if (val == 255) val = 12;
			--byData[val][4];
			--byData[val][cor];
			assert(byData[val][4] <= 4);
		}
	}
	dwVal[byNum] = dwTemp;
	++byNum;
	return dwTemp;
}
void	stAnswer::pop(BYTE byData[14][5])
{
	if (byNum == 0) return;
	int len = (byNum == 3) ? 3 : 5;
	--byNum;
	for (int i = 0; i < len; ++i)
	{
		if (byCards[byNum][i] > 0) {
			BYTE val = byCards[byNum][i] % 0x10;
			BYTE cor = byCards[byNum][i] / 0x10;
			val -= 2;
			if (val == 255) val = 12;
			++byData[val][4];
			++byData[val][cor];
			if (byData[val][cor] > 1)
				int nnn = 0;
		}
	}
}
void    stAnswer::getData(BYTE cards[13])
{
	if (byNum == 3){
		memcpy(cards, byCards[2], 3);
		memcpy(cards + 3, byCards[1], 5);
		memcpy(cards + 8, byCards[0], 5);
	}
}

void	CGameLogic::TryCards(BYTE byData[14][5], stAnswer &answer, DWORD dwScoreMax, vector<stAnswer> &vctAnswer)
{
	vector<BYTE> vctTemp;
	BYTE &nKingNum = byData[13][4];
	int nCardsNum = 0;
	for (int i = 0; i < 14; ++i)
		nCardsNum += byData[i][4];

	if (answer.byNum < 2)
	{
		if (dwScoreMax >= CT_FIVE_STRAIGHT_FLUSH << 22)	{
			//同花顺
			for (int i = 0; i < 13 - 4; ++i)
			{
				for (int n = 0; n < 4; ++n) {
					if (byData[i][n] + byData[i + 1][n] + byData[i + 2][n] + byData[i + 3][n] + byData[i + 4][n] + nKingNum >= 5)
					{				
						vctTemp.clear();
						for (int id = 0; id < 5; ++id) {
							if (byData[i + id][n] > 0) {
								vctTemp.push_back(GetCard(n, i + id));
							}
						}
						if (byData[13][0] > 0) vctTemp.push_back(GetCard(0, 13));
						if (byData[13][1] > 0) vctTemp.push_back(GetCard(1, 13));
						DWORD dwVal = answer.push(&vctTemp[0], byData);
						if (dwVal > 0) {
							TryCards(byData, answer, dwVal, vctAnswer);
							answer.pop(byData);
						}
					}
				}
			}
		}
		if (dwScoreMax >= CT_FIVE_FOUR_ONE << 22) {
			//四张
			for (int i = 0; i < 13; ++i)
			{
				if (byData[i][4] + nKingNum >= 4) {
					vctTemp.clear();
					for (int id = 0; id < 4; ++id) {
						if (byData[i][id] > 0) {
							vctTemp.push_back(GetCard(id, i));
						}
					}
					if (byData[13][0] > 0) vctTemp.push_back(GetCard(0, 13));
					if (byData[13][1] > 0) vctTemp.push_back(GetCard(1, 13));
					vctTemp.resize(5);
					vctTemp[4] = 0;
					DWORD dwVal = answer.push(&vctTemp[0], byData);
					if (dwVal > 0) {
						TryCards(byData, answer, dwVal, vctAnswer);
						answer.pop(byData);
					}
				}
			}
		}
		if (dwScoreMax >= CT_FIVE_THREE_DEOUBLE << 22) {
			//三带二
			for (int i = 0; i < 13; ++i)
			{
				if (nKingNum + byData[i][4] >= 3 && nKingNum <= 1)
				{
					int j = 0;
					for (; j < 13; ++j) {
						if (j != i && byData[j][4] == 2) break;
					}
					if (j >= 13) {
						j = 0;
						for (; j < 13; ++j) {
							if (j != i && byData[j][4] == 3) break;
						}
					}

					if (j < 13) {
						vctTemp.clear();
						// 三张
						for (int id = 0; id < 4; ++id) {
							if (byData[i][id] > 0) {
								vctTemp.push_back(GetCard(id, i));
							}
						}
						// 这里最多只加一个赖子牌
						if (vctTemp.size() == 2) {
							if (byData[13][0] > 0)
								vctTemp.push_back(GetCard(0, 13));
							else if (byData[13][1] > 0)
								vctTemp.push_back(GetCard(1, 13));
						}
						// 小对
						for (int id = 0; id < 4; ++id) {
							if (byData[j][id] > 0) {
								vctTemp.push_back(GetCard(id, j));
							}
						}
						DWORD dwVal = answer.push(&vctTemp[0], byData);
						if (dwVal > 0) {
							TryCards(byData, answer, dwVal, vctAnswer);
							answer.pop(byData);
						}
					}
				}
			}
		}
		if (dwScoreMax >= CT_FIVE_FLUSH << 22) {
			//同花
			for (int n = 0; n < 4; ++n)
			{
				vector<BYTE> vctCards;
				for (int i = 0; i < 13; ++i) {
					if (byData[i][n] == 1)
						vctCards.push_back(GetCard(n, i));
				}
				if (byData[13][0] > 0) vctCards.push_back(GetCard(0, 13));
				if (byData[13][1] > 0) vctCards.push_back(GetCard(1, 13));
				if (vctCards.size() >= 5)
				{
					for (size_t i1 = 0; i1 < vctCards.size(); ++i1)
						for (size_t i2 = i1 + 1; i2 < vctCards.size(); ++i2)
							for (size_t i3 = i2 + 1; i3 < vctCards.size(); ++i3)
								for (size_t i4 = i3 + 1; i4 < vctCards.size(); ++i4)
									for (size_t i5 = i4 + 1; i5 < vctCards.size(); ++i5)
									{
										BYTE byCards[5] = { vctCards[i1], vctCards[i2], vctCards[i3], vctCards[i4], vctCards[i5] };
										DWORD dwVal = answer.push(byCards, byData);
										if (dwVal > 0) {
											TryCards(byData, answer, dwVal, vctAnswer);
											answer.pop(byData);
										}
									}
				}
			}
		}
		if (dwScoreMax >= CT_FIVE_MIXED_FLUSH_NO_A << 22) {
			//顺子
			for (int i = 0; i < 13 - 4; ++i)
			{
				int lineNum = 0;
				lineNum += byData[i][4] > 0;
				lineNum += byData[i + 1][4] > 0;
				lineNum += byData[i + 2][4] > 0;
				lineNum += byData[i + 3][4] > 0;
				lineNum += byData[i + 4][4] > 0;
				if (lineNum + nKingNum >= 5)
				{
					vctTemp.clear();
					for (int x = 0; x < 5; ++x) {
						for (int n = 0; n < 4; ++n) {
							if (byData[i + x][n] == 1) {
								vctTemp.push_back(GetCard(n, i + x));
								break;
							}
						}
					}
					if (byData[13][0] > 0) vctTemp.push_back(GetCard(0, 13));
					if (byData[13][1] > 0) vctTemp.push_back(GetCard(1, 13));
					DWORD dwVal = answer.push(&vctTemp[0], byData);
					if (dwVal > 0) {
						TryCards(byData, answer, dwVal, vctAnswer);
						answer.pop(byData);
					}
				}
			}
			// 2 3 4 5 A
			int lineNum = 0;
			lineNum += byData[0][4] > 0;
			lineNum += byData[1][4] > 0;
			lineNum += byData[2][4] > 0;
			lineNum += byData[3][4] > 0;
			lineNum += byData[12][4] > 0;
			if (lineNum + nKingNum >= 5)
			{
				vctTemp.clear();
				for (int x = 0; x < 4; ++x) {
					for (int n = 0; n < 4; ++n) {
						if (byData[x][n] == 1) {
							vctTemp.push_back(GetCard(n, x));
							break;
						}
					}
				}
				for (int n = 0; n < 4; ++n) {
					if (byData[12][n] == 1) {
						vctTemp.push_back(GetCard(n, 12));
						break;
					}
				}
				if (byData[13][0] > 0) vctTemp.push_back(GetCard(0, 13));
				if (byData[13][1] > 0) vctTemp.push_back(GetCard(1, 13));
				DWORD dwVal = answer.push(&vctTemp[0], byData);
				if (dwVal > 0) {
					TryCards(byData, answer, dwVal, vctAnswer);
					answer.pop(byData);
				}
			}
		}
		if (dwScoreMax >= CT_THREE << 22) {
			//三张
			for (int i = 12; i >= 0; --i)
			{
				if (byData[i][4] + nKingNum >= 3) {
					vctTemp.clear();
					for (int n = 0; n < 4; ++n) {
						if (byData[i][n] == 1)
							vctTemp.push_back(GetCard(n, i));
					}
					if (vctTemp.size() == 2) {
						if (byData[13][0] > 0)
							vctTemp.push_back(GetCard(0, 13));
						else if (byData[13][1] > 0)
							vctTemp.push_back(GetCard(1, 13));
					}
					else if (vctTemp.size() == 1) {
						if (byData[13][0] > 0) vctTemp.push_back(GetCard(0, 13));
						if (byData[13][1] > 0) vctTemp.push_back(GetCard(1, 13));
					}
					assert(vctTemp.size() == 3);
					//再选两个最小单牌
					for (int j = 0; j < 13; ++j) {
						if (byData[j][4] == 1 && i != j) {
							for (int n = 0; n < 4; ++n) {
								if (byData[j][n] == 1)
									vctTemp.push_back(GetCard(n, j));
							}
						}
						if (vctTemp.size() >= 5) break;
					}
					if (vctTemp.size() >= 5) {
						DWORD dwVal = answer.push(&vctTemp[0], byData);
						if (dwVal > 0){
							TryCards(byData, answer, dwVal, vctAnswer);
							answer.pop(byData);
						}
					}
				}
			}
		}
		vector<BYTE> vctPairs, vctSingle;
		for (int i = 0; i < 13; ++i)
		{
			if (byData[i][4] == 2)
				vctPairs.push_back(i);
			else if (byData[i][4] == 1)
				vctSingle.push_back(i);
		}

		if (dwScoreMax >= CT_TWO_DOUBLE << 22) {
			//两对
			if (nKingNum == 0 && vctPairs.size() >= 2)
			{
				//多对子时，最大的对子放到最上面（三张的那组）
				BYTE byMax = vctPairs.size() - 1;
				if (answer.byNum == 0) {
					if (vctPairs.size() >= 5)
						byMax -= 1;
					else if (vctPairs.size() >= 4)
						byMax -= 2;
				}
				else if (vctPairs.size() >= 3) {
					byMax -= 1;
				}
				vctTemp.clear();
				for (int n = 0; n < 4; ++n) {
					if (byData[vctPairs[byMax]][n] == 1)
						vctTemp.push_back(GetCard(n, vctPairs[byMax]));
				}
				for (int n = 0; n < 4; ++n) {
					if (byData[vctPairs[0]][n] == 1)
						vctTemp.push_back(GetCard(n, vctPairs[0]));
				}					

				//再选一个最小单牌
				for (int i = 0; i < 13; ++i)
				{
					if (byData[i][4] == 1) {
						for (int n = 0; n < 4; ++n) {
							if (byData[i][n] == 1)
								vctTemp.push_back(GetCard(n, i));
						}							
					}
					if (vctTemp.size() >= 5) break;
				}
				DWORD dwVal = answer.push(&vctTemp[0], byData);
				if (dwVal > 0){
					TryCards(byData, answer, dwVal, vctAnswer);
					answer.pop(byData);
				}
			}
		}

		if (dwScoreMax >= CT_ONE_DOUBLE << 22) {
			//一对			 
			if ((vctPairs.size() > 0 && nKingNum == 0) || (vctPairs.size() == 0 && vctSingle.size() > 0 && nKingNum == 1))
			{
				vctTemp.clear();
				if (nKingNum == 0) {
					BYTE byMax = vctPairs.size() - 1;
					for (int n = 0; n < 4; ++n) {
						if (byData[vctPairs[byMax]][n] == 1)
							vctTemp.push_back(GetCard(n, vctPairs[byMax]));
					}
				}				
				else if (nKingNum == 1) {
					BYTE byMax = vctSingle.size() - 1;
					for (int n = 0; n < 4; ++n) {
						if (byData[vctSingle[byMax]][n] == 1) {
							vctTemp.push_back(GetCard(n, vctSingle[byMax]));
							break;
						}							
					}
					if (vctTemp.size() == 1) {
						if (byData[13][0] > 0) vctTemp.push_back(GetCard(0, 13));
						else if (byData[13][1] > 0) vctTemp.push_back(GetCard(1, 13));
					}
				}
				assert(vctTemp.size() == 2);
				//再选三个最小单牌
				for (int i = 0; i < 13; ++i)
				{
					if (byData[i][4] == 1) {
						for (int n = 0; n < 4; ++n) {
							if (byData[i][n] == 1)
								vctTemp.push_back(GetCard(n, i));
						}							
					}
					if (vctTemp.size() >= 5) break;
				}
				if (vctTemp.size() == 5) {
					DWORD dwVal = answer.push(&vctTemp[0], byData);
					if (dwVal > 0){
						TryCards(byData, answer, dwVal, vctAnswer);
						answer.pop(byData);
					}
				}
			}
		}

		if (dwScoreMax >= CT_SINGLE << 22) {
			//单牌
			vctTemp.clear();
			for (int i = 12; i > 0; --i)
			{
				if (byData[i][4] == 1) {
					for (int n = 0; n < 4; ++n) {
						if (byData[i][n] == 1)
							vctTemp.push_back(GetCard(n, i));
					}	
				}
				if (vctTemp.size() >= 5) break;
			}
			if (vctTemp.size() > 5) {
				DWORD dwVal = answer.push(&vctTemp[0], byData);
				if (dwVal > 0){
					TryCards(byData, answer, dwVal, vctAnswer);
					answer.pop(byData);
				}
			}
		}
	}
	else
	{
		if (answer.repairNote(byData) == false) return;
		if (dwScoreMax >= CT_THREE << 22) {
			//三张
			for (int i = 0; i < 13; ++i)
			{
				if (byData[i][4] + nKingNum >= 3) {
					vctTemp.clear();
					for (int n = 0; n < 4; ++n) {
						if (byData[i][n] == 1)
							vctTemp.push_back(GetCard(n, i));
					}
					if (byData[13][0] > 0) vctTemp.push_back(GetCard(0, 13));
					if (byData[13][1] > 0) vctTemp.push_back(GetCard(1, 13));
					vctTemp.resize(5);
					DWORD dwVal = answer.push(&vctTemp[0], byData);
					if (dwVal > 0) {
						vctAnswer.push_back(answer);
						answer.pop(byData);
					}
				}
			}
		}

		if (dwScoreMax >= CT_ONE_DOUBLE << 22) {	
			// 一对
			for (int i = 0; i < 13; ++i)
			{
				if (byData[i][4] <= 2 && nKingNum < 2 && byData[i][4] + nKingNum >= 2) {
					vctTemp.clear();
					for (int n = 0; n < 4; ++n) {
						if (byData[i][n] == 1)
							vctTemp.push_back(GetCard(n, i));
					}
					if (vctTemp.size() == 1) {
						if (byData[13][0] > 0)	
							vctTemp.push_back(GetCard(0, 13));
						else if (byData[13][1] > 0) 
							vctTemp.push_back(GetCard(1, 13));
					}	
					assert(vctTemp.size() == 2);
					//再选一个最小单牌
					for (int j = 0; j < 13; ++j)
					{
						if (byData[j][4] == 1 && j != i)
							for (int n = 0; n < 4; ++n) {
								if (byData[j][n] == 1) {
									vctTemp.push_back(GetCard(n, j));
									break;
								}
							}
						if (vctTemp.size() >= 3) break;
					}
					if (vctTemp.size() >= 3) {
						DWORD dwVal = answer.push(&vctTemp[0], byData);
						if (dwVal > 0) {
							vctAnswer.push_back(answer);
							answer.pop(byData);
						}
					}					
				}
			}
		}

		if (dwScoreMax >= CT_SINGLE << 22) {
			//单牌
			vctTemp.clear();
			for (int i = 0; i < 13; ++i)
			{
				if (byData[i][4] == 1) {
					for (int n = 0; n < 4; ++n) {
						if (byData[i][n] == 1) {
							vctTemp.push_back(GetCard(n, i));
							break;
						}
					}						
				}
			}
			if (vctTemp.size() >= 3) {
				BYTE bySize = vctTemp.size();
				BYTE byCards[5] = { vctTemp[bySize - 1], vctTemp[bySize - 2], vctTemp[bySize - 3], 0, 0 };
				DWORD dwVal = answer.push(byCards, byData);
				if (dwVal > 0) {
					vctAnswer.push_back(answer);
					answer.pop(byData);
				}
			}
		}
		answer.undo_repair(byData);
	}
}
//
void	CGameLogic::JudgeCards(const BYTE cbCardData[], BYTE cbCount, vector<stAnswer> &vctOut)
{
	BYTE byData[14][5] = {};
	for (int i = 0; i < cbCount; ++i){
		BYTE val = cbCardData[i] % 0x10;
		BYTE cor = cbCardData[i] / 0x10;
		val -= 2;
		if (val == 255) val = 12;

		++byData[val][4];
		++byData[val][cor];
		assert(byData[val][cor] <= 1);
		if (byData[val][cor] > 1) {
			std::cout << "有牌值重复！请检查配置" << endl;
			return;
		}
	}
	stAnswer answer;
	TryCards(byData, answer, CT_FIVE_MAX << 22, vctOut);
}
//逻辑牌值
DWORD	CGameLogic::GetLogicValue(const BYTE cbCardData[], BYTE cbCardCount)
{
	//变量定义
	int  nValFlag = 0;
	int  nMaxID[5] = {};	//最大牌
	BYTE byMaxCor[5] = {};	//最大牌花色
	if (cbCardCount <= 5 && cbCardCount >= 3)
	{
		BYTE num = 0;
		BYTE byCardVal[14] = {};
		BYTE byCardCol[4] = {};
		for (int i = 0; i<cbCardCount; ++i)
		{
			if (cbCardData[i] > 0) {
				BYTE byVal = GetCardLogicValue(cbCardData[i]);
				++byCardVal[byVal];
				++num;
				if (byVal < 13) {
					BYTE byCol = GetCardColor(cbCardData[i]);
					++byCardCol[byCol];
				}
			}
		}
		cbCardCount = num;
		int  nKingNum = byCardVal[13];

		BYTE bySameMax = 0;
		vector<BYTE> vctDouble;
		//统计数据
		for (int i = 0; i < 13; ++i)
		{
			bySameMax = max(bySameMax, byCardVal[i]);
			nMaxID[byCardVal[i]] = i;
			if (byCardVal[i] == 2)
				vctDouble.push_back(i);
		}

		//找到指定值最大颜色
		for (int i = 0; i < cbCardCount; ++i)
		{
			BYTE byVal = GetCardLogicValue(cbCardData[i]);
			for (int n = 1; n < 5; ++n) {
				if (byVal == nMaxID[n])
					byMaxCor[n] = max(byMaxCor[n], GetCardColor(cbCardData[i]));
			}
		}

		//变量定义
		bool cbSameColor = false, bLineCard = false;
		//判断是否是顺子
		for (int i = 0; i < 13; ++i)
		{
			if (i + 4 < 13)
			{
				int num = byCardVal[i] + byCardVal[i + 1] + byCardVal[i + 2] + byCardVal[i + 3] + byCardVal[i + 4] + nKingNum;				
				if (num == 5 && bySameMax == 1)
					bLineCard = true;
			}
		}
		//特殊 2 3 4 5 A
		if (byCardVal[0] + byCardVal[1] + byCardVal[2] + byCardVal[3] + byCardVal[12] + nKingNum == 5 && bySameMax == 1)
			bLineCard = true;

		//判断是否是同花
		for (int i = 0; i < 4; ++i) {
			if (byCardCol[i] + nKingNum == 5)
				cbSameColor = true;
		}

		if (cbCardCount != 5)
			cbSameColor = false;

		//数值比较标志
		for (int n = 4; n >= 1; --n) {
			for (int i = nMaxID[n]; i >= 0; --i) {
				if (byCardVal[i] == n) {
					for (int m = 0; m < n; ++m)
						nValFlag = (nValFlag << 4) + i;
				}
			}
		}
		if (nKingNum > 0) {
			nValFlag += (13 << 16);
			memset(byMaxCor, 0x03, sizeof(byMaxCor));
		}

		//顺金类型
		if (cbSameColor && bLineCard)
			return ((int)CT_FIVE_STRAIGHT_FLUSH << 22) + (nValFlag << 2) + (byMaxCor[1] & 0x03);

		//顺子类型
		if (!cbSameColor && bLineCard)
			return ((int)CT_FIVE_MIXED_FLUSH_NO_A << 22) + (nValFlag << 2) + (byMaxCor[1] & 0x03);

		//金花类型
		if (cbSameColor && !bLineCard)
			return ((int)CT_FIVE_FLUSH << 22) + (nValFlag << 2) + (byMaxCor[1] & 0x03);

		//对子和豹子判断
		if (bySameMax + nKingNum >= 4) {
			return ((int)CT_FIVE_FOUR_ONE << 22) + (nValFlag << 2) + (byMaxCor[4] & 0x03);
		}
		else if (bySameMax + nKingNum == 3) {
			if ((nKingNum == 0 && vctDouble.size() == 1) || (nKingNum == 1 && vctDouble.size() == 2))
				return ((int)CT_FIVE_THREE_DEOUBLE << 22) + (nValFlag << 2) + (byMaxCor[3] & 0x03);
			else
				return ((int)CT_THREE << 22) + (nValFlag << 2) + (byMaxCor[3] & 0x03);
		}
		else if (bySameMax + nKingNum == 2) {
			if (vctDouble.size() >= 2)
				return ((int)CT_TWO_DOUBLE << 22) + (nValFlag << 2) + (byMaxCor[1] & 0x03);
			else
				return ((int)CT_ONE_DOUBLE << 22) + (nValFlag << 2) + (byMaxCor[1] & 0x03);
		}
	}
	return ((int)CT_SINGLE << 22) + (nValFlag << 2) + (byMaxCor[1] & 0x03);
}
//逻辑大小
DWORD	CGameLogic::GetCardType(const BYTE cbCardData[], BYTE cbCardCount)
{
	DWORD dwLogicVal = GetLogicValue(cbCardData, cbCardCount);
	return DWORD(dwLogicVal >> 22);
}
//大小比较
bool	CGameLogic::CompareCard(const BYTE cbCardData1[], BYTE cbCount1, const BYTE cbCardData2[], BYTE cbCount2)
{
	return GetLogicValue(cbCardData1, cbCount1) > GetLogicValue(cbCardData2, cbCount2);
}



#include <iostream>
#include <vector>
#include <windows.h>
#include <unordered_map>
#include <algorithm>
#include <map>
#include <iomanip>

#include "thirteen_logic.h"

using namespace std;

//帮助函数
void printCards(BYTE *byTemp)
{
	//打印所有牌值
	BYTE byData[13][5] = {};
	for (int i = 0; i < 13; ++i) {
		BYTE val = byTemp[i] % 0x10;
		BYTE cor = byTemp[i] / 0x10;
		val -= 2;
		if (val == 255) val = 12;
		++byData[val][4];
		++byData[val][cor];
	}
	for (int i = 0; i < 13; ++i) {
		if (byData[i][4] > 0) {
			for (int n = 0; n < 4; ++n) {
				if (byData[i][n] > 0)
					cout << hex << "0x" << int(GetCard(n, i)) << ", ";
			}
		}
	}
	cout << endl;
}
//帮助函数
bool isSameCards(BYTE byCards[13], stAnswer answer)
{
	BYTE bySrc[13] = {};
	BYTE byTemp[13] = {};
	memcpy(bySrc, byCards, 13);
	answer.getData(byTemp);
	for (int i = 0; i < 13; ++i) {
		bool bFind = false;
		for (int n = 0; n < 13; ++n)
			if (byTemp[n] == byCards[i]) {
				bFind = true;
				break;
			}
		if (bFind == false) 
			return false;
	}
	return true;
}

void SplitString(const string &s, vector<string> &v, string c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));
		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

int  ReadTestText(string strFile, BYTE byCardsOut[], BYTE byMaxNum)
{
	int index = 0;
	char szPath[MAX_PATH] = { 0 };
	GetCurrentDirectoryA(MAX_PATH, szPath);
	char szFileName[MAX_PATH] = { 0 };
	sprintf(szFileName, "%s/%s", szPath, strFile.c_str());

	FILE *fp;
	fp = fopen(szFileName, "r");
	if (fp != NULL)
	{
		char szTemp[1024] = { 0 };
		char szData[1024] = { 0 };
		// 设置文件起始位置
		fseek(fp, 0, SEEK_SET);
		while (!feof(fp))
		{
			int nSize = 0;
			memset(szTemp, 0, sizeof(szTemp));
			memset(szData, 0, sizeof(szData));
			vector<string> vct;
			// 读取一行
			fgets(szTemp, sizeof(szTemp), fp);
			for (int i = 0; i < 1024; ++i)
			{
				if (szTemp[i] == 0)
					break;
				if (szTemp[i] != 32 && szTemp[i] != 10)
					szData[nSize++] = szTemp[i];
			}
			string strData = szData;
			SplitString(strData, vct, "0x");
			for (size_t i = 0; i < vct.size(); ++i)
			{
				if (vct[i].size() > 0 && index < byMaxNum)
				{
					int byTemp = 0;
					sscanf(vct[i].c_str(), "%x", &byTemp);
					byCardsOut[index++] = byTemp;
				}
			}
		}
		fclose(fp);
	}
	return index;
}
//帮助函数
bool cmp(const stAnswer &temp1, const stAnswer &temp2)
{
	static float fVal[3][CT_FIVE_MAX * 16] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0.01, 0.02, 0.05, 0.09, 0.17, 0.26,
		0.37, 0.49, 0.60, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68,
		0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 
		0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68,
		0.68, 0.68, 0.78, 0.82, 0.88, 0.93, 0.98, 1.03, 1.10, 1.16, 1.26, 1.26, 1.26, 1.26,
		1.26, 1.26, 1.26, 1.26, 1.26, 1.26, 1.26, 1.26, 1.26, 1.26, 1.26, 1.26, 1.26, 1.26, 
		1.26, 1.26, 1.26, 1.26, 1.26, 1.26, 1.26, 1.33, 1.50, 1.82, 2.24, 2.78, 3.39, 4.05,
		4.76, 4.76, 4.76, 4.76, 11.42, 17.58, 23.23, 28.25, 32.63, 36.36, 39.60, 42.41, 44.81, 46.84, 
		48.70, 50.24, 51.57, 51.57, 51.57, 51.57, 55.02, 58.68, 62.40, 66.10, 69.66, 73.21, 76.75, 80.10, 
		83.33, 86.66, 90.06, 93.41, 96.49, 96.49, 96.49, 96.49, 96.49, 96.49, 96.49, 96.49, 96.49, 96.49, 
		96.49, 96.49, 96.49, 96.49, 96.49, 96.49, 96.49, 96.49, 96.49, 96.49, 96.49, 96.49, 96.49, 96.49, 
		96.79, 97.09, 97.46, 97.90, 98.35, 98.79, 99.21, 99.65, 100, 100, 100, 100, 100, 100, 
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,

		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.01, 0.08, 0.34,
		1.19, 1.19, 1.19, 1.19, 1.35, 1.56, 1.77, 2.00, 2.27, 2.59, 2.93, 3.30, 3.71, 4.19, 
		4.73, 5.42, 6.24, 6.24, 6.24, 6.24, 6.24, 6.50, 7.08, 8.02, 9.43, 11.28, 13.58, 16.18,
		19.14, 22.18, 25.34, 28.39, 30.97, 30.97, 30.97, 30.97, 31.03, 31.30, 31.82, 32.63, 33.74, 35.18, 
		36.92, 39.15, 41.50, 44.23, 47.32, 50.62, 53.86, 53.86, 53.86, 53.86, 53.86, 53.86, 53.86, 53.86, 
		53.86, 53.86, 53.86, 53.86, 53.86, 53.86, 53.86, 53.86, 53.86, 53.86, 53.86, 53.86, 53.86, 53.86, 
		53.86, 53.86, 54.10, 54.32, 54.60, 54.93, 55.31, 55.81, 56.42, 57.20, 58.32, 58.32, 58.32, 58.32, 
		58.32, 58.32, 58.32, 58.32, 58.32, 58.32, 58.32, 58.32, 58.32, 58.32, 58.32, 58.32, 58.32, 58.32, 
		58.32, 58.32, 58.32, 58.32, 58.32, 58.32, 58.32, 58.36, 58.46, 58.69, 59.09, 59.83, 60.93, 62.60, 
		64.88, 64.88, 64.88, 64.88, 67.89, 70.87, 73.79, 76.69, 79.52, 82.20, 84.71, 87.10, 89.29, 91.23, 
		92.98, 94.47, 95.75, 95.75, 95.75, 95.75, 96.38, 97.00, 97.58, 98.10, 98.55, 98.92, 99.23, 99.48, 
		99.68, 99.83, 99.92, 99.98, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,

		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0.01, 0.06, 0.19, 0.51, 1.16, 2.36, 4.55, 8.73, 15.70, 27.06,
		45.62, 45.62, 45.62, 45.62, 45.86, 46.30, 46.98, 47.98, 49.31, 51.12, 53.45, 56.39, 60.30, 65.35, 
		71.65, 79.88, 90.65, 90.65, 90.65, 90.65, 90.65, 90.65, 90.65, 90.65, 90.65, 90.65, 90.65, 90.65, 
		90.65, 90.65, 90.65, 90.65, 90.65, 90.65, 90.65, 90.65, 90.71, 90.94, 91.36, 91.97, 92.75, 93.72, 
		94.74, 95.79, 96.79, 97.73, 98.59, 99.37, 100, 100, 100, 100, 100, 100, 100, 100, 
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
	};
	
	static float fPer[3][CT_FIVE_MAX] = {
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 5, 5, 5,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 8, 10, 10, 10,
		0, 1, 1, 3, 30, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	};
	DWORD t1[3] = { temp1.dwVal[0] >> 22, temp1.dwVal[1] >> 22, temp1.dwVal[2] >> 22 };
	DWORD t2[3] = { temp2.dwVal[0] >> 22, temp2.dwVal[1] >> 22, temp2.dwVal[2] >> 22 };
	DWORD type1[3] = { temp1.dwVal[0] >> 18, temp1.dwVal[1] >> 18, temp1.dwVal[2] >> 18 };
	DWORD type2[3] = { temp2.dwVal[0] >> 18, temp2.dwVal[1] >> 18, temp2.dwVal[2] >> 18 };
	float fFlag1 = 0, fFlag2 = 0;
	for (int i = 0; i < 3; ++i)
	{
		fFlag1 += fPer[i][t1[i]] * fVal[i][type1[i]];
		fFlag2 += fPer[i][t2[i]] * fVal[i][type2[i]];
	}
	return fFlag1 > fFlag2;
}

//#define	USE_TEST_INI
void main()
{	
#ifndef USE_TEST_INI
	BYTE byTest[] = { 
		0x02, 0x06, 0x07, 0x0A, 0x0C, 0x0D, 0x14, 0x15, 0x18, 0x19, 0x21, 0x24, 0x33, 0x34, 0x38, 0x39, 0x0F
	};
	BYTE byCardNum = sizeof(byTest);
	//*/
#else
	//配牌 pino
	BYTE byTest[64] = {};
	BYTE byCardNum = ReadTestText("TEST_DATA.txt", byTest, 64);
#endif

	srand(GetTickCount());
	BYTE m_cbCardDataTemp[52] = {};
	random_shuffle(s_cbCardData, s_cbCardData + CARD_COUNT);

	DWORD dwFlag = GetTickCount();
	//CGameLogic::JudgeCards(s_cbCardData, 13, vctOut);
	vector<stAnswer> vctOut;
	vector<stAnswer> vctOut2;
	for (int i = 0; i < 1; ++i) {
		vctOut.clear();
		vctOut2.clear();
		CGameLogic::JudgeCards(byTest, byCardNum, vctOut);
		//vector<stAnswer> vctOut2 = vctOut;

		sort(vctOut.begin(), vctOut.end(), cmp);
		for (size_t i = 0; i < vctOut.size(); ++i)
		{
			int nType[3] = { vctOut[i].dwVal[0] >> 22, vctOut[i].dwVal[1] >> 22, vctOut[i].dwVal[2] >> 22 };
			int nFlag = nType[0] * 10000 + nType[1] * 100 + nType[2];
			size_t j = 0;
			for (; j < vctOut2.size(); ++j)
			{
				int nType2[3] = { vctOut2[j].dwVal[0] >> 22, vctOut2[j].dwVal[1] >> 22, vctOut2[j].dwVal[2] >> 22 };
				int nFlag2 = nType2[0] * 10000 + nType2[1] * 100 + nType2[2];
				if (nFlag == nFlag2) {
					if (cmp(vctOut[i], vctOut2[j])) {
						vctOut2[j] = vctOut[i];
					}
					break;
				}
				if (nType[0] <= nType2[0] && nType[1] <= nType2[1] && nType[2] <= nType2[2]) {
					break;
				}
			}
			if (j >= vctOut2.size())
				vctOut2.push_back(vctOut[i]);
		}
		//*/
		sort(vctOut2.begin(), vctOut2.end(), cmp);
	}	
	cout << GetTickCount() - dwFlag << " ms" << "\t" << vctOut.size() << endl;

	//打印牌组
	for (size_t i = 0; i < vctOut2.size(); ++i) {
		cout << endl;
		stAnswer &answer = vctOut2[i];
		for (int id = 0; id < 3; ++id)
			cout << hex << "type=" << (answer.dwVal[id] >> 22) << "->" << answer.dwVal[id] << '\t' << (int)answer.byCards[id][0] << '\t' << (int)answer.byCards[id][1] <<
			'\t' << (int)answer.byCards[id][2] << '\t' << (int)answer.byCards[id][3] << '\t' << (int)answer.byCards[id][4] << endl;
	}

	Sleep(1000000);
}

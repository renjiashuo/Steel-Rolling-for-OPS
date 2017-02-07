/*************************************************
版权: 任家硕
作者: 任家硕
日期: 2016-12-27
功能: 拟合策略测试_ROLLPASS类
修改历史:

**************************************************/

/*<remark>=========================================================
/// <summary>
/// 拟合策略测试_ROLLPASS类
/// </summary>
===========================================================</remark>*/

#pragma once

#include "ROLLPASS.h"

#pragma region 辅助函数
//////////////////////////////////////////////////////////////////////////

ROLLPASS::ROLLPASS(int NO, string REC_CREATOR, string REC_CREATE_TIME, string REC_REVISOR, string REC_REVISE_TIME, string ARCHIVE_FLAG, string APP_SYS_DIV, string WHOLE_BACKLOG_CODE, string 	WHOLE_BACKLOG_NAME, string 	PASS_SERIES_CODE, string PASS_CODE, string TOTAL_RACK_COUNT, string IN_THICK, string IN_WIDTH, string OUT_THICK, int OUT_WIDTH, string USED_RACK_COUNT, string RACK_CODE1, string RACK_CODE2, string RACK_CODE3, string RACK_CODE4, string RACK_CODE5, string RACK_CODE6, string RACK_CODE7, string RACK_CODE8, string RACK_CODE9, string RACK_CODE10, string RACK_CODE11, string RACK_CODE12, string RACK_CODE13, string RACK_CODE14, string RACK_CODE15, string RACK_CODE16, string RACK_CODE17, string RACK_CODE18, string RACK_CODE19, string RACK_CODE20)
{
	m_NO = NO;
	m_REC_CREATOR = REC_CREATOR;
	m_REC_CREATE_TIME = REC_CREATE_TIME;
	m_REC_REVISOR = REC_REVISOR;
	m_REC_REVISE_TIME = REC_REVISE_TIME;
	m_ARCHIVE_FLAG = ARCHIVE_FLAG;
	m_APP_SYS_DIV = APP_SYS_DIV;
	m_WHOLE_BACKLOG_CODE = WHOLE_BACKLOG_CODE;
	m_WHOLE_BACKLOG_NAME = WHOLE_BACKLOG_NAME;
	m_PASS_SERIES_CODE = PASS_SERIES_CODE;
	m_PASS_CODE = PASS_CODE;
	m_TOTAL_RACK_COUNT = TOTAL_RACK_COUNT;
	m_IN_THICK = IN_THICK;
	m_IN_WIDTH = IN_WIDTH;
	m_OUT_THICK = OUT_THICK;
	m_OUT_WIDTH = OUT_WIDTH;
	m_USED_RACK_COUNT = USED_RACK_COUNT;
	m_RACK_CODE1 = RACK_CODE1;
	m_RACK_CODE2 = RACK_CODE2;
	m_RACK_CODE3 = RACK_CODE3;
	m_RACK_CODE4 = RACK_CODE4;
	m_RACK_CODE5 = RACK_CODE5;
	m_RACK_CODE6 = RACK_CODE6;
	m_RACK_CODE7 = RACK_CODE7;
	m_RACK_CODE8 = RACK_CODE8;
	m_RACK_CODE9 = RACK_CODE9;
	m_RACK_CODE10 = RACK_CODE10;
	m_RACK_CODE11 = RACK_CODE11;
	m_RACK_CODE12 = RACK_CODE12;
	m_RACK_CODE13 = RACK_CODE13;
	m_RACK_CODE14 = RACK_CODE14;
	m_RACK_CODE15 = RACK_CODE15;
	m_RACK_CODE16 = RACK_CODE16;
	m_RACK_CODE17 = RACK_CODE17;
	m_RACK_CODE18 = RACK_CODE18;
	m_RACK_CODE19 = RACK_CODE19;
	m_RACK_CODE20 = RACK_CODE20;
	m_RACK_CODE[0] = RACK_CODE1;
	m_RACK_CODE[1] = RACK_CODE2;
	m_RACK_CODE[2] = RACK_CODE3;
	m_RACK_CODE[3] = RACK_CODE4;
	m_RACK_CODE[4] = RACK_CODE5;
	m_RACK_CODE[5] = RACK_CODE6;
	m_RACK_CODE[6] = RACK_CODE7;
	m_RACK_CODE[7] = RACK_CODE8;
	m_RACK_CODE[8] = RACK_CODE9;
	m_RACK_CODE[9] = RACK_CODE10;
	m_RACK_CODE[10] = RACK_CODE11;
	m_RACK_CODE[11] = RACK_CODE12;
	m_RACK_CODE[12] = RACK_CODE13;
	m_RACK_CODE[13] = RACK_CODE14;
	m_RACK_CODE[14] = RACK_CODE15;
	m_RACK_CODE[15] = RACK_CODE16;
	m_RACK_CODE[16] = RACK_CODE17;
	m_RACK_CODE[17] = RACK_CODE18;
	m_RACK_CODE[18] = RACK_CODE19;
	m_RACK_CODE[19] = RACK_CODE20;
	int count = OUT_THICK.find('-');
	m_OUT_THICK_min = atoi(OUT_THICK.substr(0, count).c_str());
	m_OUT_THICK_max = atoi(OUT_THICK.substr(count+1, OUT_THICK.size()).c_str());
}

void ROLLPASS::readData()
{
	ifstream file("TOPPPROLLPASS.csv");
	while (file.good())
	{
		//定义存储变量
		string value1;
		string value2;
		string value3;
		string value4;
		string value5;
		string value6;
		string value7;
		string value8;
		string value9;
		string value10;
		string value11;
		string value12;
		string value13;
		string value14;
		string value15;
		string value16;
		string value17;
		string value18;
		string value19;
		string value20;
		string value21;
		string value22;
		string value23;
		string value24;
		string value25;
		string value26;
		string value27;
		string value28;
		string value29;
		string value30;
		string value31;
		string value32;
		string value33;
		string value34;
		string value35;
		string value36;
		string value37;
		//读取值
		getline(file, value1, ',');
		getline(file, value2, ',');
		getline(file, value3, ',');
		getline(file, value4, ',');
		getline(file, value5, ',');
		getline(file, value6, ',');
		getline(file, value7, ',');
		getline(file, value8, ',');
		getline(file, value9, ',');
		getline(file, value10, ',');
		getline(file, value11, ',');
		getline(file, value12, ',');
		getline(file, value13, ',');
		getline(file, value14, ',');
		getline(file, value15, ',');
		getline(file, value16, ',');
		getline(file, value17, ',');
		getline(file, value18, ',');
		getline(file, value19, ',');
		getline(file, value20, ',');
		getline(file, value21, ',');
		getline(file, value22, ',');
		getline(file, value23, ',');
		getline(file, value24, ',');
		getline(file, value25, ',');
		getline(file, value26, ',');
		getline(file, value27, ',');
		getline(file, value28, ',');
		getline(file, value29, ',');
		getline(file, value30, ',');
		getline(file, value31, ',');
		getline(file, value32, ',');
		getline(file, value33, ',');
		getline(file, value34, ',');
		getline(file, value35, ',');
		getline(file, value36, ',');
		getline(file, value37);
		if (value1 == "")
			continue;
		//转换并存储
		ROLLPASS *newROLLPASS = new ROLLPASS(atoi(value1.c_str()), value2, value3, value4, value5, value6, value7, value8, value9, value10, value11, value12, value13, value14, value15, atoi(value16.c_str()), value17, value18, value19, value20, value21, value22, value23, value24, value25, value26, value27, value28, value29, value30, value31, value32, value33, value34, value35, value36, value37);
		s_mapSetOfROLLPASS.insert(make_pair(atoi(value1.c_str()), newROLLPASS));
		s_ROLLPASSCount++;
	}
	cout << "读取数据完成，为 " << s_ROLLPASSCount << " 。" << endl;
}

void ROLLPASS::initData()
{
	for (map<int, ROLLPASS *>::iterator iter = s_mapSetOfROLLPASS.begin(); iter != s_mapSetOfROLLPASS.end(); iter++)
	{
		//vector<int> temp;
		ROLLPASS* rollPass1 = iter->second;
		for (map<int, ROLLPASS *>::iterator iter2 = s_mapSetOfROLLPASS.begin(); iter2 != s_mapSetOfROLLPASS.end(); iter2++)
		{
			ROLLPASS* rollPass2 = iter2->second;
			int num = 0;
			for (int i = 0; i < 20; i++)
			{
				if (rollPass1->m_RACK_CODE[i] != rollPass2->m_RACK_CODE[i])
					num++;
			}
			s_distance.insert(make_pair(make_pair(rollPass1, rollPass2), num));
			//temp.push_back(num);
		}
		//s_distance.push_back(temp);
	}
}

//////////////////////////////////////////////////////////////////////////
#pragma endregion

#pragma region 合同集合函数
//////////////////////////////////////////////////////////////////////////
map<int, ROLLPASS *> ROLLPASS::s_mapSetOfROLLPASS = map<int, ROLLPASS *>();
int				ROLLPASS::s_ROLLPASSCount = 0;
map<pair< ROLLPASS*, ROLLPASS* >, int > ROLLPASS::s_distance = map<pair< ROLLPASS*, ROLLPASS* >, int >();
//vector<vector<int> >	ROLLPASS::s_distance = vector<vector<int> >();
//////////////////////////////////////////////////////////////////////////
#pragma endregion
/*************************************************
版权: 任家硕
作者: 任家硕
日期: 2016-12-10
功能: 拟合策略测试_HR类
修改历史:

**************************************************/

/*<remark>=========================================================
/// <summary>
/// 拟合策略测试_HR类
/// </summary>
===========================================================</remark>*/

#pragma once

#include "HR.h"
#include "time.h"

#pragma region 辅助函数
//////////////////////////////////////////////////////////////////////////

HR::HR(int NO, string REC_CREATE_TIME, string REC_CREATOR, string REC_REVISE_TIME, string REC_REVISOR, string ARCHIVE_FLAG, string WORKSHEET_NO, string WHOLE_BACKLOG_CODE, string UNIT_CODE, string SG_SIGN, string SG_STD, double LACK_WT, int OUT_THICK, int OUT_WIDTH, string EPST, string NEST, string BEGIN_TIME, string FINISH_TIME, string NLST, string LPST, string ORDER_NO, string STATUS)
{
	m_NO = NO;
	m_REC_CREATE_TIME = REC_CREATE_TIME;
	m_REC_CREATOR = REC_CREATOR;
	m_REC_REVISE_TIME = REC_REVISE_TIME;
	m_REC_REVISOR = REC_REVISOR;
	m_ARCHIVE_FLAG = ARCHIVE_FLAG;
	m_WORKSHEET_NO = WORKSHEET_NO;
	m_WHOLE_BACKLOG_CODE = WHOLE_BACKLOG_CODE;
	m_UNIT_CODE = UNIT_CODE;
	m_SG_SIGN = SG_SIGN;
	m_SG_STD = SG_STD;
	m_LACK_WT = LACK_WT;
	m_OUT_THICK = OUT_THICK;
	m_OUT_WIDTH = OUT_WIDTH;
	m_BEGIN_TIME = BEGIN_TIME;
	m_FINISH_TIME = FINISH_TIME;
	m_ORDER_NO = ORDER_NO;
	m_STATUS = STATUS;
	m_BEGIN_TIME_t = Time::StringToDatetime(BEGIN_TIME);
	m_FINISH_TIME_t = Time::StringToDatetime(FINISH_TIME);
	// 新增参数
	m_EPST = EPST;
	m_NEST = NEST;
	m_NLST = NLST;
	m_LPST = LPST;
	m_NEST_t = Time::StringToDatetime(m_NEST);
	m_NLST_t = Time::StringToDatetime(m_NLST);
}

void HR::readData()
{
	ifstream file("TOPPPHR01.csv");
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
		getline(file, value22);
		if (value1 == "")
			continue;
		//转换并存储
		HR *newHR = new HR(atoi(value1.c_str()), value2, value3, value4, value5, value6, value7, value8, value9, value10, value11, atof(value12.c_str()), atoi(value13.c_str()), atoi(value14.c_str()), value15, value16, value17, value18, value19, value20, value21, value22);
		s_mapSetOfHR.insert(make_pair(atoi(value1.c_str()), newHR));
		s_HRCount++;
	}
	cout << "读取数据完成，为 " << s_HRCount << " 。" << endl;
}

void HR::initRollPass()
{
	for (map<int, HR *>::iterator iter = HR::s_mapSetOfHR.begin(); iter != HR::s_mapSetOfHR.end(); iter++)
	{
		HR* hr = iter->second;
		//调试用
		{
			if (hr->m_NO == 528)
				bool aaa = false;
		}
		for (map<int, ROLLPASS *>::iterator iter2 = ROLLPASS::s_mapSetOfROLLPASS.begin(); iter2 != ROLLPASS::s_mapSetOfROLLPASS.end(); iter2++)
		{
			ROLLPASS* rollPass = iter2->second;
			if (hr->m_OUT_WIDTH == rollPass->m_OUT_WIDTH && hr->m_OUT_THICK >= rollPass->m_OUT_THICK_min && hr->m_OUT_THICK <= rollPass->m_OUT_THICK_max)
			{
				hr->m_rollPass = rollPass;
				break;
			}
		}
	}
}

void HR::initDataTime()
{
	// 基准时间
	double IA_month = 87000.0;
	double IA_second = IA_month / (31 * 24 * 60 * 60);
	s_standTime = HR::s_mapSetOfHR.begin()->second->m_BEGIN_TIME_t;
	for (map<int, HR *>::iterator iter = HR::s_mapSetOfHR.begin(); iter != HR::s_mapSetOfHR.end(); iter++)
	{
		iter->second->m_BEGIN_TIME_t -= s_standTime;
		iter->second->m_FINISH_TIME_t -= s_standTime;
		iter->second->m_NEST_t -= s_standTime;
		iter->second->m_NLST_t -= s_standTime;
		iter->second->m_MAKING_TIME_t = iter->second->m_LACK_WT / IA_second;
		//iter->second->m_MAKING_TIME_t = iter->second->m_FINISH_TIME_t - iter->second->m_BEGIN_TIME_t;
	}
	s_END_TIME -= s_standTime;
}

void HR::printResult(vector<HR> newSequence)
{
	string result = "newSequence.csv";
	ofstream fout(result);
	fout << "NO,REC_CREATE_TIME,REC_CREATOR,REC_REVISE_TIME,";
	fout << "REC_REVISOR,ARCHIVE_FLAG,WORKSHEET_NO,WHOLE_BACKLOG_CODE,";
	fout << "UNIT_CODE,SG_SIGN,SG_STD,LACK_WT,";
	fout << "OUT_THICK,OUT_WIDTH,BEGIN_TIME,";
	fout << "FINISH_TIME,最早开始时间,ORDER_NO,STATUS" << endl;
	for (vector<HR>::iterator iter = newSequence.begin(); iter != newSequence.end(); iter++)
	{
		fout << iter->m_NO << "," << iter->m_REC_CREATE_TIME << "," << iter->m_REC_CREATOR << "," << iter->m_REC_REVISE_TIME << ",";
		fout << iter->m_REC_REVISOR << "," << iter->m_ARCHIVE_FLAG << "," << iter->m_WORKSHEET_NO << "," << iter->m_WHOLE_BACKLOG_CODE << ",";
		fout << iter->m_UNIT_CODE << "," << iter->m_SG_SIGN << "," << iter->m_SG_STD << "," << iter->m_LACK_WT << ",";
		fout << iter->m_OUT_THICK << "," << iter->m_OUT_WIDTH << "," << Time::DatetimeToString(iter->m_new_BEGIN_TIME_t + s_standTime) << ",";
		fout << Time::DatetimeToString(iter->m_new_FINISH_TIME_t + s_standTime) << "," << iter->m_BEGIN_TIME << "," << iter->m_ORDER_NO << "," << iter->m_STATUS << endl;
	}
}

bool HR::less_second_int(const HR & m1, const HR & m2)
{
	return m1.m_OUT_THICK > m2.m_OUT_THICK;
}

bool HR::more_second_int(const HR & m1, const HR & m2)
{
	return m1.m_OUT_THICK < m2.m_OUT_THICK;
}

vector<HR> HR::heuristic()
{
	// 启发式算法
	vector<HR> newSequence;
	time_t now = HR::s_mapSetOfHR.begin()->second->m_BEGIN_TIME_t;
	int max_OUT_THICK = HR::s_mapSetOfHR.begin()->second->m_OUT_THICK;
	int min_OUT_THICK = HR::s_mapSetOfHR.begin()->second->m_OUT_THICK;
	vector<HR> lessPartSequence;
	vector<HR> morePartSequence;
	int mark = 0;
	for (map<int, HR *>::iterator iter = HR::s_mapSetOfHR.begin(); iter != HR::s_mapSetOfHR.end(); iter++)
	{
		HR *hr = iter->second;
		time_t beginTime = hr->m_BEGIN_TIME_t;
		if (mark == 0)
		{
			max_OUT_THICK = hr->m_OUT_THICK;
			min_OUT_THICK = hr->m_OUT_THICK;
			now = now + 2 * T;
			mark = 1;
		}
		if (beginTime < now + T)
		{
			if (hr->m_OUT_THICK <= max_OUT_THICK)
				lessPartSequence.push_back(*hr);
			else
				morePartSequence.push_back(*hr);
			if (hr->m_OUT_THICK < min_OUT_THICK)
				min_OUT_THICK = hr->m_OUT_THICK;
		}
		else if (beginTime < now + 2 * T)
		{
			morePartSequence.push_back(*hr);
		}
		else
		{
			sort(lessPartSequence.begin(), lessPartSequence.end(), less_second_int);
			sort(morePartSequence.begin(), morePartSequence.end(), more_second_int);
			newSequence.insert(newSequence.end(), lessPartSequence.begin(), lessPartSequence.end());
			newSequence.insert(newSequence.end(), morePartSequence.begin(), morePartSequence.end());
			lessPartSequence.clear();
			morePartSequence.clear();
			mark = 0;
		}
	}
	return newSequence;
}

vector<HR> HR::localSearch()
{
	// 统计时间
	clock_t start, finish;
	start = clock();
	double  duration;
	// 初始解
	vector<HR> sequence = localSearchInit();
	// 初始目标函数值
	int distance = 0;
	int switchFreq = 0;
	localSearchObj(sequence, &distance, &switchFreq);
	cout << "初始目标函数值：" << switchFreq << endl;
	// 邻域搜索
	vector<HR> next;
	int delta;
	bool improved = true;
	while (improved)
	{
		improved = false;
		for (int i = 0; i < sequence.size() - 1; i++)
		{
			for (int j = i + 1; j < sequence.size() && j < i + 100; j++)
			{
				next = sequence;
				if (SwapOperator(&next, i, j))//邻域交换
				{
					int nextDistance = 0;
					int nextSwitchFreq = 0;
					localSearchObj(next, &nextDistance, &nextSwitchFreq);
					delta = nextSwitchFreq - switchFreq;
					if (delta < 0 || (delta == 0 && next.rbegin()->m_new_FINISH_TIME_t < sequence.rbegin()->m_new_FINISH_TIME_t))//如果有改进
					{
						improved = true;
						sequence = next;//更新解
						distance = nextDistance;//更新目标函数值
						switchFreq = nextSwitchFreq;
						cout << switchFreq << endl;//输出当前目标函数值
					}
				}
			}
		}
	}
	cout << "局部最优目标函数值：" << switchFreq << endl;	
	finish = clock();
	duration = finish - start;
	cout << "计算时间：" << duration / 1000 << "秒" << endl;
	return sequence;
}

vector<HR> HR::localSearchInit()
{
	vector<HR> sequence;
	for (map<int, HR *>::iterator iter = HR::s_mapSetOfHR.begin(); iter != HR::s_mapSetOfHR.end(); iter++)
	{
		HR *hr = iter->second;
		sequence.push_back(*hr);
	}
	return sequence;
}

void HR::localSearchObj(vector<HR> sequence, int *distance, int *switchFreq)
{
	int flag = 0;
	for (int i = 0; i < sequence.size() - 1; i++)
	{
		(*distance) += (sequence[i].m_OUT_THICK > sequence[i + 1].m_OUT_THICK ? sequence[i].m_OUT_THICK - sequence[i + 1].m_OUT_THICK : sequence[i + 1].m_OUT_THICK - sequence[i].m_OUT_THICK);
		if (flag == 0)
		{
			if (sequence[i + 1].m_OUT_THICK > sequence[i].m_OUT_THICK)
				flag = 1;
			if (sequence[i + 1].m_OUT_THICK < sequence[i].m_OUT_THICK)
				flag = -1;
		}
		else
		{
			int flag2 = 0;
			if (sequence[i + 1].m_OUT_THICK > sequence[i].m_OUT_THICK)
				flag2 = 1;
			if (sequence[i + 1].m_OUT_THICK < sequence[i].m_OUT_THICK)
				flag2 = -1;
			if (flag * flag2 == -1)
			{
				(*switchFreq)++;
				flag = flag2;
			}
		}
		//if (i != 0 && (sequence[i + 1].m_OUT_THICK - sequence[i].m_OUT_THICK) * (sequence[i].m_OUT_THICK - sequence[i - 1].m_OUT_THICK) < 0)
		//	(*switchFreq)++;
	}
}

bool HR::SwapOperator(vector<HR> *sequence, int a, int b)
{
	HR temp = (*sequence)[a];
	(*sequence)[a] = (*sequence)[b];
	(*sequence)[b] = temp;
	for (int i = 0; i < (*sequence).size(); i++)
	{
		if (i == 0)
			(*sequence)[i].m_new_BEGIN_TIME_t = (*sequence)[i].m_BEGIN_TIME_t;
		else
			(*sequence)[i].m_new_BEGIN_TIME_t = (*sequence)[i - 1].m_new_FINISH_TIME_t >(*sequence)[i].m_BEGIN_TIME_t ? (*sequence)[i - 1].m_new_FINISH_TIME_t : (*sequence)[i].m_BEGIN_TIME_t;
		(*sequence)[i].m_new_FINISH_TIME_t = (*sequence)[i].m_new_BEGIN_TIME_t + (*sequence)[i].m_MAKING_TIME_t;
		if ((*sequence)[i].m_new_FINISH_TIME_t > s_END_TIME)
			return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
#pragma endregion

#pragma region 集合函数
//////////////////////////////////////////////////////////////////////////
map<int, HR *> HR::s_mapSetOfHR = map<int, HR *>();
int				HR::s_HRCount = 0;
time_t			HR::s_standTime = 0;
time_t			HR::s_END_TIME = Time::StringToDatetime("20161125000000");
//////////////////////////////////////////////////////////////////////////
#pragma endregion
/*************************************************
版权: 任家硕
作者: 任家硕
日期: 2016-12-29
功能: 拟合策略测试_GA类
修改历史:

**************************************************/

/*<remark>=========================================================
/// <summary>
/// 拟合策略测试_GA类
/// </summary>
===========================================================</remark>*/

#pragma once

#include "ga.h"
#include "time.h"
#include "ROLLPASS.h"
#include "HR.h"
#include "random.h"

#pragma region 辅助函数
//////////////////////////////////////////////////////////////////////////

vector<int> GA::GenerateRandomTour() 
{
	vector<int> tour(HR::s_HRCount);
	int  numUnVisitedCities = HR::s_HRCount - 1;				//未加入解中城市的计数器
	vector<int> unVisitedCities;				//未加入解中城市列表

	//未加入的解中的城市列表初始化
	for (map<int, HR *>::iterator iter = HR::s_mapSetOfHR.begin(); iter != HR::s_mapSetOfHR.end(); iter++)
	{
		int NO = iter->first;
		unVisitedCities.push_back(NO);
	}

	for (int i = 0; i < HR::s_HRCount; i++)						//作总城市个数次循环
    {
	    int index = randomInteger(0, numUnVisitedCities--);	//随机生成一个城市
      	tour[i] = unVisitedCities[index];					//将该城市添加到当前解的当前位置
      	for(int j = index; j <= numUnVisitedCities; j++)	//在未加入的城市列表中删除该城市
      		unVisitedCities[j] = unVisitedCities[j+1];      	 
    }
	return tour;
};

vector<int> GA::GenerateSequenceTour()
{
	vector<int> unVisitedCities;				//未加入解中城市列表
	//未加入的解中的城市列表初始化
	for (map<int, HR *>::iterator iter = HR::s_mapSetOfHR.begin(); iter != HR::s_mapSetOfHR.end(); iter++)
	{
		int NO = iter->first;
		unVisitedCities.push_back(NO);
	}
	return unVisitedCities;
};

vector<int> GA::GenerateSequenceTour2()// 待完成
{
	vector<int> unVisitedCities;				//未加入解中城市列表
	vector<pair<int, int> > v_unVisitedCities;				//未加入解中城市列表
	//未加入的解中的城市列表初始化
	for (map<int, HR *>::iterator iter = HR::s_mapSetOfHR.begin(); iter != HR::s_mapSetOfHR.end(); iter++)
	{
		int NO = iter->first;
		v_unVisitedCities.push_back(make_pair(NO, iter->second->m_OUT_THICK));
	}
	if (random01() > 0.5)
	{
		for (int i = 0; i < v_unVisitedCities.size() - 1; i++)
		{
			for (int j = i + 1; j < v_unVisitedCities.size(); j++)
			{
				if (v_unVisitedCities[i].second > v_unVisitedCities[j].second)
				{
					pair<int, int> temp = v_unVisitedCities[i];
					v_unVisitedCities[i] = v_unVisitedCities[j];
					v_unVisitedCities[j] = temp;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < v_unVisitedCities.size() - 1; i++)
		{
			for (int j = i + 1; j < v_unVisitedCities.size(); j++)
			{
				if (v_unVisitedCities[i].second < v_unVisitedCities[j].second)
				{
					pair<int, int> temp = v_unVisitedCities[i];
					v_unVisitedCities[i] = v_unVisitedCities[j];
					v_unVisitedCities[j] = temp;
				}
			}
		}
	}
	for (int i = 0; i < v_unVisitedCities.size(); i++)
	{
		unVisitedCities.push_back(v_unVisitedCities[i].first);
	}
	return unVisitedCities;
};

void GA::ComputeFitness()
{
	int max = -1;
	for(int i = 0; i < PopSize; i++)
	{
		fitness[i] = ComputeDistance(individual[i]);			//计算每个个体的旅行总距离
		max = fitness[i] > max ? fitness[i] : max;			//找出当前解中的旅行总距离最大值
		if( fitness[i] < bestValue )						//找出当前解中的最优解
		{
			bestValue = fitness[i];
			CopySolution(&individual[i],&best);
			cout << "最优值已更新：" << bestValue << endl;
		}
	}

	max = max + 1;
	for(int i = 0; i < PopSize; i++)						//令fitness等于旅行总距离最大值与当前解旅行总距离的差值
		fitness[i] = max - fitness[i];
};

void GA::BuildWheel()
{
	double sum = 0;
	for(int i = 0; i < PopSize; i++)						//计算种群每个个体目标函数值之和
		sum += fitness[i];
	prob[0] = (double)fitness[0] / (double)sum;				//第一个个体占总和的分数
	for( int i = 1 ; i < PopSize ; i++ )					//计算每个个体占总和的累计分数
		prob[i] = prob[i-1] + fitness[i] / sum;
};

int  GA::SelectIndividual()
{
	double p = random01();
	if( p < prob[0] )
		return 0;
	if( p >= prob[PopSize - 1] )
		return (PopSize - 1);
	for( int i = 1 ; i < PopSize ; i++ )
		if( (p >= prob[i-1]) && (p < prob[i]) )
			return i;
	return -1;
};

void GA::PMX(int lo, int hi, vector<int> *tour1, vector<int> *tour2, vector<int> *newtour1, vector<int> *newtour2)
{
	int j, pos1, pos2;
	CopySolution(tour1, newtour1);
	CopySolution(tour2, newtour2);

	j = lo;
	while( j <= hi )
	{
		pos1  = FindCity((*tour2)[j], *newtour1);
		pos2  = FindCity((*tour1)[j], *newtour2);
		SwapOperator(j, pos1, newtour1);
		SwapOperator(j, pos2, newtour2);
		j++;
	}
};

void GA::Mutation()
{
	for(int i = 0; i < PopSize; i++)					//对种群中每个个体做循环
	{
		if( random01() < Pm )							//变异概率
		{
			int i1 = randomInteger(0,HR::s_HRCount - 1);	//随机生成两个数
			int i2 = randomInteger(0, HR::s_HRCount - 1);
			while( i1 == i2)
				i2 = randomInteger(0, HR::s_HRCount - 1);

			SwapOperator(i1, i2, &individual[i]);		//对解中这两个位置的数交换
		}
	}
};

void GA::Initialize()
{
	PopSize = 1000;
	Gen = 100;
	Pc = 0.5;
	Pm = 0.5;
	best = vector<int>(HR::s_HRCount);
	bestValue = 9999;
	prob = vector<double>(PopSize);
	for (int i = 0; i < PopSize / 3; i++)		//给初始种群生成随机个体
	{
		individual.push_back(GenerateRandomTour());
		fitness.push_back(0);
	}
	for (int i = PopSize / 3; i < PopSize * 2 / 3; i++)		//给初始种群生成时间顺序个体
	{
		individual.push_back(GenerateSequenceTour());
		fitness.push_back(0);
	}
	for (int i = PopSize * 2 / 3; i < PopSize; i++)		//给初始种群生成孔型顺序个体
	{
		individual.push_back(GenerateSequenceTour2());
		fitness.push_back(0);
	}
	individual.insert(individual.begin(), individual.begin(), individual.end());
	fitness.insert(fitness.begin(), fitness.begin(), fitness.end());
};

void GA::SelectionAndCrossover()
{
	BuildWheel();										//制作轮盘赌的轮盘
	for(int i = PopSize; i < 2*PopSize; i = i + 2)		
	{
		int father = SelectIndividual();				//轮盘赌方式选取一个个体
		int mother = SelectIndividual();				//轮盘赌方式选取另一个个体
		
		if( random01() < Pc )							//如果满足交叉概率
		{
			int lo = randomInteger(1, HR::s_HRCount - 2);	//随机两个数作为交叉段的起始点和终止点
			int hi = randomInteger(1, HR::s_HRCount - 2);
			while( lo == hi)
				hi = randomInteger(1, HR::s_HRCount - 2);
			int min = lo < hi ? lo : hi;
			int max = lo < hi ? hi : lo;
			lo = min;
			hi = max;
			//从lo到hi这段长度对father和mother做交叉然后存到indevidual中
			PMX(lo, hi, &individual[father], &individual[mother], &individual[i], &individual[i+1]);
		}
		else											//如果不满足交叉概率
		{
			CopySolution(&individual[father], &individual[i]  );
			CopySolution(&individual[mother], &individual[i+1]);
		}
	}

	for(int i = 0; i < PopSize; i++)					//更新整个种群
		CopySolution(&individual[PopSize+i], &individual[i]);
}

void GA::GeneticAlgorithm()
{
	srand((unsigned)time(NULL));	//播下一颗时间的种子
	GA ga;
	ga.Initialize();					//初始化
	ga.ComputeFitness();				//计算初始化所得的种群的目标函数值

	for(int g = 1; g <= ga.Gen; g++)	//循环总次数
	{
		if (g % 100 == 0)
			cout << "已计算 " << g << "代" << endl;
		ga.SelectionAndCrossover();		//选择和交叉
		ga.Mutation();				//变异
		ga.ComputeFitness();			//更新新种群对应每个个体的目标函数值
	}
	ga.printResult(ga.best);		// 打印结果
}

int GA::ComputeDistance(vector<int> individual)
{
	int sum = 0;
	int switchFrequency = 0;
	int absTime = 0;
	time_t beginTime = 0, endTime = 0;
	int selectHRnum = 0;
	// 第一个
	int NO1 = individual[0];
	HR* hr1 = HR::s_mapSetOfHR.find(NO1)->second;
	ROLLPASS* rollPass1 = hr1->m_rollPass;
	beginTime = hr1->m_NEST_t > endTime ? hr1->m_NEST_t : endTime;
	for (int i = 1; i < individual.size(); i++)
	{
		// 计算frequency
		int NO2 = individual[i];
		HR* hr2 = HR::s_mapSetOfHR.find(NO2)->second;
		ROLLPASS* rollPass2 = hr2->m_rollPass;
		int frequency = ROLLPASS::s_distance.find(make_pair(rollPass1, rollPass2))->second;
		switchFrequency += frequency;
		endTime = beginTime + hr1->m_MAKING_TIME_t;
		// 计算absTime
		absTime += (beginTime - hr1->m_NEST_t);
		NO1 = NO2;
		hr1 = hr2;
		rollPass1 = rollPass2;
		beginTime = hr1->m_NEST_t > endTime ? hr1->m_NEST_t : endTime;
		if (beginTime < hr1->m_NLST_t)
		{
			selectHRnum++;
		}
	}
	// 思考：孔型如何与时间加权。
	string endTimeStr = Time::DatetimeToString(endTime + HR::s_standTime);
	sum = int((HR::s_HRCount - selectHRnum) * 10000 / HR::s_HRCount) + switchFrequency /*+ endTime / 2500 + absTime / 4000000*/;
	return sum;
}

void GA::CopySolution(vector<int> *from, vector<int> *to)
{
	for (int i = 0; i < HR::s_HRCount; i++)
		(*to)[i] = (*from)[i];
}

int GA::FindCity(int city, vector<int> tour)//根据城市编号在解中寻找城市的位
{
	int j = 0;
	while (tour[j] != city)
		j++;
	return j;
};

void GA::SwapOperator(int city1, int city2, vector<int> *tour)//邻域交换
{
	int temp = (*tour)[city1];
	(*tour)[city1] = (*tour)[city2];
	(*tour)[city2] = temp;
};

void GA::printResult(vector<int> newSequence)
{
	string result = "newSequence.csv";
	ofstream fout(result);
	fout << "NO,REC_CREATE_TIME,REC_CREATOR,REC_REVISE_TIME,";
	fout << "REC_REVISOR,ARCHIVE_FLAG,WORKSHEET_NO,WHOLE_BACKLOG_CODE,";
	fout << "UNIT_CODE,SG_SIGN,SG_STD,LACK_WT,";
	fout << "OUT_THICK,OUT_WIDTH,BEGIN_TIME,";
	fout << "FINISH_TIME,最早开始时间,最晚开始时间,ORDER_NO,STATUS" << endl;
	time_t endTime = 0;
	for (int i = 0; i < newSequence.size(); i++)
	{
		HR* iter = HR::s_mapSetOfHR.find(newSequence[i])->second;
		if (i == 0)
		{
			iter->m_new_BEGIN_TIME_t = iter->m_NEST_t;
			iter->m_new_FINISH_TIME_t = iter->m_new_BEGIN_TIME_t + iter->m_MAKING_TIME_t;
			endTime = iter->m_new_FINISH_TIME_t;
			iter->m_STATUS = "1";
		}
		else
		{
			//HR* lastIter = HR::s_mapSetOfHR.find(newSequence[i - 1])->second;
			iter->m_new_BEGIN_TIME_t = endTime >iter->m_NEST_t ? endTime : iter->m_NEST_t;
			iter->m_new_FINISH_TIME_t = iter->m_new_BEGIN_TIME_t + iter->m_MAKING_TIME_t;
			endTime = iter->m_new_FINISH_TIME_t;
			if (iter->m_new_BEGIN_TIME_t > iter->m_NLST_t)
				iter->m_STATUS = "0";
			else
				iter->m_STATUS = "1";
		}
	}
	//for (int i = newSequence.size() - 2; i >= 0; i--)
	//{
	//	HR* iter = HR::s_mapSetOfHR.find(newSequence[i])->second;
	//	HR* lastIter = HR::s_mapSetOfHR.find(newSequence[i + 1])->second;
	//	iter->m_new_FINISH_TIME_t = lastIter->m_new_BEGIN_TIME_t;
	//	iter->m_new_BEGIN_TIME_t = iter->m_new_FINISH_TIME_t - iter->m_MAKING_TIME_t;
	//}
	for (int i = 0; i < newSequence.size(); i++)
	{
		HR* iter = HR::s_mapSetOfHR.find(newSequence[i])->second;
		fout << iter->m_NO << "," << iter->m_REC_CREATE_TIME << "," << iter->m_REC_CREATOR << "," << iter->m_REC_REVISE_TIME << ",";
		fout << iter->m_REC_REVISOR << "," << iter->m_ARCHIVE_FLAG << "," << iter->m_WORKSHEET_NO << "," << iter->m_WHOLE_BACKLOG_CODE << ",";
		fout << iter->m_UNIT_CODE << "," << iter->m_SG_SIGN << "," << iter->m_SG_STD << "," << iter->m_LACK_WT << ",";
		fout << iter->m_OUT_THICK << "," << iter->m_OUT_WIDTH << "," << Time::DatetimeToString(iter->m_new_BEGIN_TIME_t + HR::s_standTime) << ",";
		fout << Time::DatetimeToString(iter->m_new_FINISH_TIME_t + HR::s_standTime) << "," << iter->m_NEST << "," << iter->m_NLST << "," << iter->m_ORDER_NO << "," << iter->m_STATUS << endl;
	}
}

//////////////////////////////////////////////////////////////////////////
#pragma endregion

#pragma region 集合函数
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
#pragma endregion
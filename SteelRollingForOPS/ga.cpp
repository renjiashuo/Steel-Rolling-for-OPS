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
	long long max = -1;
	for(int i = 0; i < PopSize; i++)
	{
		fitness[i] = ComputeDistance(individual[i]);			//计算每个个体的旅行总距离
		max = fitness[i] > max ? fitness[i] : max;			//找出当前解中的旅行总距离最大值
		//cout << min << endl;
		//cout << bestValue << endl;
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
	bestValue = LLONG_MAX;
	prob = vector<double>(PopSize);
	for (int i = 0; i < 0; i++)		//给初始种群生成随机个体
	{
		individual.push_back(GenerateRandomTour());
		fitness.push_back(0);
	}
	for (int i = 0; i < PopSize / 2; i++)		//给初始种群生成时间顺序个体
	{
		individual.push_back(GenerateSequenceTour());
		fitness.push_back(0);
	}
	for (int i = PopSize / 2; i < PopSize; i++)		//给初始种群生成孔型顺序个体
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

void GA::LocalSearch()
{
	for (int i = 0; i < PopSize; i++)					//对种群中每个个体做循环
	{
		if (random01() < 1)							//变异概率
		{
			int number = randomInteger(1, 6);
			if(number == 1)// Job Move
			{
					   int i1 = randomInteger(0, HR::s_HRCount - 1);	//随机生成两个数
					   int i2 = randomInteger(0, HR::s_HRCount - 1);
					   while (i1 == i2)
						   i2 = randomInteger(0, HR::s_HRCount - 1);
					   if (i1 < i2)
					   {
						   for (int j = i1; j < i2; j++)
						   {
							   SwapOperator(j, j + 1, &individual[i]);		//对解中这两个位置的数交换
						   }
					   }
					   else
					   {
						   for (int j = i1; j > i2; j--)
						   {
							   SwapOperator(j, j - 1, &individual[i]);		//对解中这两个位置的数交换
						   }
					   }
			}
			else if (number == 2)// Job Exchange
			{
					   int i1 = randomInteger(0, HR::s_HRCount - 1);	//随机生成两个数
					   int i2 = randomInteger(0, HR::s_HRCount - 1);
					   while (i1 == i2)
						   i2 = randomInteger(0, HR::s_HRCount - 1);
					   SwapOperator(i1, i2, &individual[i]);		//对解中这两个位置的数交换
			}
			else if (number == 3)// Batch Move
			{
					   int i1 = randomInteger(0, HR::s_HRCount - 1);	//随机生成两个数
					   int i2 = randomInteger(0, HR::s_HRCount - 1);
					   while (i1 == i2)
						   i2 = randomInteger(0, HR::s_HRCount - 1);
					   int i11, i12, i21, i22;
					   int NO1 = individual[i][i1];
					   HR* hr1 = HR::s_mapSetOfHR.find(NO1)->second;
					   int NO2 = individual[i][i2];
					   HR* hr2 = HR::s_mapSetOfHR.find(NO2)->second;
					   for (i11 = i1; i11 >= 0; i11--)
					   {
						   int NO11 = individual[i][i11];
						   HR* hr11 = HR::s_mapSetOfHR.find(NO11)->second;
						   if (hr1->m_OUT_THICK != hr11->m_OUT_THICK)
						   {
							   i11 += 1;
							   break;
						   }
					   }
					   if (i11 == -1)
						   i11 = 0;
					   for (i12 = i1; i12 < HR::s_HRCount; i12++)
					   {
						   int NO12 = individual[i][i12];
						   HR* hr12 = HR::s_mapSetOfHR.find(NO12)->second;
						   if (hr1->m_OUT_THICK != hr12->m_OUT_THICK)
						   {
							   i12 -= 1;
							   break;
						   }
					   }
					   if (i12 == HR::s_HRCount)
						   i12 = HR::s_HRCount - 1;
					   for (i21 = i2; i21 >= 0; i21--)
					   {
						   int NO21 = individual[i][i21];
						   HR* hr21 = HR::s_mapSetOfHR.find(NO21)->second;
						   if (hr2->m_OUT_THICK != hr21->m_OUT_THICK)
						   {
							   i21 += 1;
							   break;
						   }
					   }
					   if (i21 == -1)
						   i21 = 0;
					   for (i22 = i2; i22 < HR::s_HRCount; i22++)
					   {
						   int NO22 = individual[i][i22];
						   HR* hr22 = HR::s_mapSetOfHR.find(NO22)->second;
						   if (hr2->m_OUT_THICK != hr22->m_OUT_THICK)
						   {
							   i22 -= 1;
							   break;
						   }
					   }
					   if (i22 == HR::s_HRCount)
						   i22 = HR::s_HRCount - 1;
					   if (i11 == i21)
						   continue;
					   if (i11 == i12 || i21 == i22)
						   bool aaaaaa = true;
					   vector<int> individual_temp;
					   if (i11 < i21)
					   {
						   for (int j = 0; j < i11; j++)
						   {
							   individual_temp.push_back(individual[i][j]);
						   }
						   for (int j = i12 + 1; j < i21; j++)
						   {
							   individual_temp.push_back(individual[i][j]);
						   }
						   int qh = randomInteger(0, 1);
						   if (qh == 0)
						   {
							   for (int j = i11; j <= i12; j++)
							   {
								   individual_temp.push_back(individual[i][j]);
							   }
							   for (int j = i21; j < HR::s_HRCount; j++)
							   {
								   individual_temp.push_back(individual[i][j]);
							   }
						   }
						   else
						   {
							   for (int j = i21; j <= i22; j++)
							   {
								   individual_temp.push_back(individual[i][j]);
							   }
							   for (int j = i11; j <= i12; j++)
							   {
								   individual_temp.push_back(individual[i][j]);
							   }
							   for (int j = i22 + 1; j < HR::s_HRCount; j++)
							   {
								   individual_temp.push_back(individual[i][j]);
							   }
						   }
					   }
					   else
					   {
						   for (int j = 0; j < i21; j++)
						   {
							   individual_temp.push_back(individual[i][j]);
						   }
						   int qh = randomInteger(0, 1);
						   if (qh == 0)
						   {
							   for (int j = i11; j <= i12; j++)
							   {
								   individual_temp.push_back(individual[i][j]);
							   }
							   for (int j = i21; j < i11; j++)
							   {
								   individual_temp.push_back(individual[i][j]);
							   }
							   for (int j = i12 + 1; j < HR::s_HRCount; j++)
							   {
								   individual_temp.push_back(individual[i][j]);
							   }
						   }
						   else
						   {
							   for (int j = i21; j <= i22; j++)
							   {
								   individual_temp.push_back(individual[i][j]);
							   }
							   for (int j = i11; j <= i12; j++)
							   {
								   individual_temp.push_back(individual[i][j]);
							   }
							   for (int j = i22 + 1; j < i11; j++)
							   {
								   individual_temp.push_back(individual[i][j]);
							   }
							   for (int j = i12 + 1; j < HR::s_HRCount; j++)
							   {
								   individual_temp.push_back(individual[i][j]);
							   }
						   }
					   }
					   if (individual_temp.size() != HR::s_HRCount)
					   {
						   cout << i11 << "  " << i12 << "  " << i21 << "  " << i22 << endl;
					   }
					   individual[i] = individual_temp;
			}
			else if (number == 4)// Batch Exchange
			{
					   int i1 = randomInteger(0, HR::s_HRCount - 1);	//随机生成两个数
					   int i2 = randomInteger(0, HR::s_HRCount - 1);
					   while (i1 == i2)
						   i2 = randomInteger(0, HR::s_HRCount - 1);
					   int i11, i12, i21, i22;
					   int NO1 = individual[i][i1];
					   HR* hr1 = HR::s_mapSetOfHR.find(NO1)->second;
					   int NO2 = individual[i][i2];
					   HR* hr2 = HR::s_mapSetOfHR.find(NO2)->second;
					   for (i11 = i1; i11 >= 0; i11--)
					   {
						   int NO11 = individual[i][i11];
						   HR* hr11 = HR::s_mapSetOfHR.find(NO11)->second;
						   if (hr1->m_OUT_THICK != hr11->m_OUT_THICK)
						   {
							   i11 += 1;
							   break;
						   }
					   }
					   if (i11 == -1)
						   i11 = 0;
					   for (i12 = i1; i12 < HR::s_HRCount; i12++)
					   {
						   int NO12 = individual[i][i12];
						   HR* hr12 = HR::s_mapSetOfHR.find(NO12)->second;
						   if (hr1->m_OUT_THICK != hr12->m_OUT_THICK)
						   {
							   i12 -= 1;
							   break;
						   }
					   }
					   if (i12 == HR::s_HRCount)
						   i12 = HR::s_HRCount - 1;
					   for (i21 = i2; i21 >= 0; i21--)
					   {
						   int NO21 = individual[i][i21];
						   HR* hr21 = HR::s_mapSetOfHR.find(NO21)->second;
						   if (hr2->m_OUT_THICK != hr21->m_OUT_THICK)
						   {
							   i21 += 1;
							   break;
						   }
					   }
					   if (i21 == -1)
						   i21 = 0;
					   for (i22 = i2; i22 < HR::s_HRCount; i22++)
					   {
						   int NO22 = individual[i][i22];
						   HR* hr22 = HR::s_mapSetOfHR.find(NO22)->second;
						   if (hr2->m_OUT_THICK != hr22->m_OUT_THICK)
						   {
							   i22 -= 1;
							   break;
						   }
					   }
					   if (i22 == HR::s_HRCount)
						   i22 = HR::s_HRCount - 1;
					   if (i11 == i21)
						   continue;
					   if (i11 == i12 || i21 == i22)
						   bool aaaaaa = true;
					   vector<int> individual_temp;
					   if (i11 < i21)
					   {
						   for (int j = 0; j < i11; j++)
						   {
							   individual_temp.push_back(individual[i][j]);
						   }
						   for (int j = i21; j <= i22; j++)
						   {
							   individual_temp.push_back(individual[i][j]);
						   }
						   for (int j = i12 + 1; j < i21; j++)
						   {
							   individual_temp.push_back(individual[i][j]);
						   }
						   for (int j = i11; j <= i12; j++)
						   {
							   individual_temp.push_back(individual[i][j]);
						   }
						   for (int j = i22 + 1; j < HR::s_HRCount; j++)
						   {
							   individual_temp.push_back(individual[i][j]);
						   }
					   }
					   else
					   {
						   for (int j = 0; j < i21; j++)
						   {
							   individual_temp.push_back(individual[i][j]);
						   }
						   for (int j = i11; j <= i12; j++)
						   {
							   individual_temp.push_back(individual[i][j]);
						   }
						   for (int j = i22 + 1; j < i11; j++)
						   {
							   individual_temp.push_back(individual[i][j]);
						   }
						   for (int j = i21; j <= i22; j++)
						   {
							   individual_temp.push_back(individual[i][j]);
						   }
						   for (int j = i12 + 1; j < HR::s_HRCount; j++)
						   {
							   individual_temp.push_back(individual[i][j]);
						   }
					   }
					   if (individual_temp.size() != HR::s_HRCount)
					   {
						   cout << i11 << "  " << i12 << "  " << i21 << "  " << i22 << endl;
					   }
					   individual[i] = individual_temp;
			}
			else if (number == 5)// Batch Combine
			{
					   int i1 = randomInteger(0, HR::s_HRCount - 1);	//随机生成两个数
					   int i2 = randomInteger(0, HR::s_HRCount - 1);
					   while (i1 == i2)
						   i2 = randomInteger(0, HR::s_HRCount - 1);
					   int i11, i12, i21, i22;
					   int NO1 = individual[i][i1];
					   HR* hr1 = HR::s_mapSetOfHR.find(NO1)->second;
					   int NO2 = individual[i][i2];
					   HR* hr2 = HR::s_mapSetOfHR.find(NO2)->second;
					   if (hr1->m_OUT_THICK != hr2->m_OUT_THICK)
						   continue;
					   for (i11 = i1; i11 >= 0; i11--)
					   {
						   int NO11 = individual[i][i11];
						   HR* hr11 = HR::s_mapSetOfHR.find(NO11)->second;
						   if (hr1->m_OUT_THICK != hr11->m_OUT_THICK)
						   {
							   i11 += 1;
							   break;
						   }
					   }
					   if (i11 == -1)
						   i11 = 0;
					   for (i12 = i1; i12 < HR::s_HRCount; i12++)
					   {
						   int NO12 = individual[i][i12];
						   HR* hr12 = HR::s_mapSetOfHR.find(NO12)->second;
						   if (hr1->m_OUT_THICK != hr12->m_OUT_THICK)
						   {
							   i12 -= 1;
							   break;
						   }
					   }
					   if (i12 == HR::s_HRCount)
						   i12 = HR::s_HRCount - 1;
					   for (i21 = i2; i21 >= 0; i21--)
					   {
						   int NO21 = individual[i][i21];
						   HR* hr21 = HR::s_mapSetOfHR.find(NO21)->second;
						   if (hr2->m_OUT_THICK != hr21->m_OUT_THICK)
						   {
							   i21 += 1;
							   break;
						   }
					   }
					   if (i21 == -1)
						   i21 = 0;
					   for (i22 = i2; i22 < HR::s_HRCount; i22++)
					   {
						   int NO22 = individual[i][i22];
						   HR* hr22 = HR::s_mapSetOfHR.find(NO22)->second;
						   if (hr2->m_OUT_THICK != hr22->m_OUT_THICK)
						   {
							   i22 -= 1;
							   break;
						   }
					   }
					   if (i22 == HR::s_HRCount)
						   i22 = HR::s_HRCount - 1;
					   if (i11 == i21)
						   continue;
					   if (i11 == i12 || i21 == i22)
						   bool aaaaaa = true;
					   vector<int> individual_temp;
					   if (i11 < i21)
					   {
						   for (int j = 0; j < i11; j++)
						   {
							   individual_temp.push_back(individual[i][j]);
						   }
						   for (int j = i12 + 1; j < i21; j++)
						   {
							   individual_temp.push_back(individual[i][j]);
						   }
						   for (int j = i22 + 1; j < HR::s_HRCount; j++)
						   {
							   individual_temp.push_back(individual[i][j]);
						   }
					   }
					   else
					   {
						   for (int j = 0; j < i21; j++)
						   {
							   individual_temp.push_back(individual[i][j]);
						   }
						   for (int j = i22 + 1; j < i11; j++)
						   {
							   individual_temp.push_back(individual[i][j]);
						   }
						   for (int j = i12 + 1; j < HR::s_HRCount; j++)
						   {
							   individual_temp.push_back(individual[i][j]);
						   }
					   }
					   int i3 = randomInteger(0, individual_temp.size());	//随机生成两个数
					   int qh = randomInteger(0, 1);
					   if (qh == 0)
					   {
						   int NO3 = individual[i][i3];
						   HR* hr3 = HR::s_mapSetOfHR.find(NO3)->second;
						   for (i3; i3 >= 0; i3--)
						   {
							   int NO31 = individual[i][i3];
							   HR* hr31 = HR::s_mapSetOfHR.find(NO31)->second;
							   if (hr3->m_OUT_THICK != hr31->m_OUT_THICK)
							   {
								   i3 += 1;
								   break;
							   }
						   }
						   if (i3 == -1)
							   i3 = 0;
					   }
					   else
					   {
						   int NO3 = individual[i][i3];
						   HR* hr3 = HR::s_mapSetOfHR.find(NO3)->second;
						   for (i3; i3 <HR::s_HRCount; i3++)
						   {
							   int NO32 = individual[i][i3];
							   HR* hr32 = HR::s_mapSetOfHR.find(NO32)->second;
							   if (hr3->m_OUT_THICK != hr32->m_OUT_THICK)
							   {
								   i3 -= 1;
								   break;
							   }
						   }
						   if (i3 == HR::s_HRCount)
							   i3 = HR::s_HRCount - 1;
					   }
					   individual_temp.insert(individual_temp.begin() + i3, individual[i].begin() + i21, individual[i].begin() + i22 + 1);
					   individual_temp.insert(individual_temp.begin() + i3, individual[i].begin() + i11, individual[i].begin() + i12 + 1);
					   individual[i] = individual_temp;
					   if (individual_temp.size() != HR::s_HRCount)
					   {
						   cout << individual_temp.size() << endl;
						   cout << i11 << "  " << i12 << "  " << i21 << "  " << i22 << endl;
					   }
			}
			else if (number == 6)// Batch Break
			{
					   int i1 = randomInteger(0, HR::s_HRCount - 1);
					   int i11, i12;
					   int NO1 = individual[i][i1];
					   HR* hr1 = HR::s_mapSetOfHR.find(NO1)->second;
					   for (i11 = i1; i11 >= 0; i11--)
					   {
						   int NO11 = individual[i][i11];
						   HR* hr11 = HR::s_mapSetOfHR.find(NO11)->second;
						   if (hr1->m_OUT_THICK != hr11->m_OUT_THICK)
						   {
							   i11 += 1;
							   break;
						   }
					   }
					   if (i11 == -1)
						   i11 = 0;
					   for (i12 = i1; i12 < HR::s_HRCount; i12++)
					   {
						   int NO12 = individual[i][i12];
						   HR* hr12 = HR::s_mapSetOfHR.find(NO12)->second;
						   if (hr1->m_OUT_THICK != hr12->m_OUT_THICK)
						   {
							   i12 -= 1;
							   break;
						   }
					   }
					   if (i12 == HR::s_HRCount)
						   i12 = HR::s_HRCount - 1;
					   if (i11 >= i12)
						   continue;
					   vector<int> individual_temp;
					   individual_temp.insert(individual_temp.end(), individual[i].begin(), individual[i].begin() + i11);
					   individual_temp.insert(individual_temp.end(), individual[i].begin() + i12 + 1, individual[i].end());
					   int i2 = 0, i3 = HR::s_HRCount - 1;
					   if (i11 != 0)
						   i2 = randomInteger(0, i11 - 1);
					   if (i12 != HR::s_HRCount - 1)
						   i3 = randomInteger(i12 + 1, HR::s_HRCount - 1);
					   int qh = randomInteger(0, 1);
					   if (qh == 0)
					   {
						   int NO3 = individual[i][i3];
						   HR* hr3 = HR::s_mapSetOfHR.find(NO3)->second;
						   for (i3; i3 >= i11; i3--)
						   {
							   int NO31 = individual[i][i3];
							   HR* hr31 = HR::s_mapSetOfHR.find(NO31)->second;
							   if (hr3->m_OUT_THICK != hr31->m_OUT_THICK)
							   {
								   i3 += 1;
								   break;
							   }
						   }
						   if (i3 == i11 - 1)
							   i3 = i11;
					   }
					   else
					   {
						   int NO3 = individual[i][i3];
						   HR* hr3 = HR::s_mapSetOfHR.find(NO3)->second;
						   for (i3; i3 < individual[i].size(); i3++)
						   {
							   int NO32 = individual[i][i3];
							   HR* hr32 = HR::s_mapSetOfHR.find(NO32)->second;
							   if (hr3->m_OUT_THICK != hr32->m_OUT_THICK)
							   {
								   i3 -= 1;
								   break;
							   }
						   }
						   if (i3 == individual[i].size())
							   i3 = individual[i].size() - 1;
					   }
					   qh = randomInteger(0, 1);
					   if (qh == 0)
					   {
						   int NO2 = individual[i][i2];
						   HR* hr2 = HR::s_mapSetOfHR.find(NO2)->second;
						   for (i2; i2 >= 0; i2--)
						   {
							   int NO21 = individual[i][i2];
							   HR* hr21 = HR::s_mapSetOfHR.find(NO21)->second;
							   if (hr2->m_OUT_THICK != hr21->m_OUT_THICK)
							   {
								   i2 += 1;
								   break;
							   }
						   }
						   if (i2 == -1)
							   i2 = 0;
					   }
					   else
					   {
						   int NO2 = individual[i][i2];
						   HR* hr2 = HR::s_mapSetOfHR.find(NO2)->second;
						   for (i2; i2 < i11; i2++)
						   {
							   int NO22 = individual[i][i2];
							   HR* hr22 = HR::s_mapSetOfHR.find(NO22)->second;
							   if (hr2->m_OUT_THICK != hr22->m_OUT_THICK)
							   {
								   i2 -= 1;
								   break;
							   }
						   }
						   if (i2 == i11)
							   i2 = i11 - 1;
					   }
					   if (i2 == -1)
						   i2 = 0; 
					   //cout << i1 << "\t" << i11 << "\t" << i12 << '\t' << i2 << '\t' << i3 << endl;
					   if (i2 >= i1)
					   {
						   i2 -= (i12 + 1 - i11);
					   }
					   if (i3 >= i1)
					   {
						   i3 -= (i12 + 1 - i11);
					   }
					   //cout << individual_temp.size() << endl;
					   //cout << i1 << "\t" << i11 << "\t" << i12 << '\t' << i2 << '\t' << i3 << endl;
					   individual_temp.insert(individual_temp.begin() + i3, individual[i].begin() + i1, individual[i].begin() + i12 + 1);
					   individual_temp.insert(individual_temp.begin() + i2, individual[i].begin() + i11, individual[i].begin() + i1);
					   //cout << "------------------------------------" << endl;
					   //cout << i << endl;
					   //cout << individual.size() << endl;
					   //cout << individual_temp.size() << endl;
					   //cout << "------------------------------------" << endl;
					   individual[i] = individual_temp;
			}
		}
	}
}

void GA::GeneticAlgorithm()
{
	srand((unsigned)time(NULL));	//播下一颗时间的种子
	GA ga;
	ga.Initialize();					//初始化
	ga.ComputeFitness();				//计算初始化所得的种群的目标函数值
	cout << "初始解生成完毕，开始计算！" << endl;
	for(int g = 1; g <= ga.Gen; g++)	//循环总次数
	{
		if (g % 100 == 0)
			cout << "已计算 " << g << "代" << endl;
		ga.SelectionAndCrossover();		//选择和交叉
		ga.LocalSearch();
		ga.Mutation();				//变异
		ga.ComputeFitness();			//更新新种群对应每个个体的目标函数值
	}
	ga.printResult(ga.best);		// 打印结果
}

long long GA::ComputeDistance(vector<int> individual)
{
	long long sum = 0;
	int switchFrequency = 0;
	int absThick = 0;
	int absTime = 0;
	time_t beginTime = 0, endTime = 0, tardiness = 0;
	// 第一个
	int NO1 = individual[0];
	HR* hr1 = HR::s_mapSetOfHR.find(NO1)->second;
	int OUT_THICK1 = hr1->m_OUT_THICK;
	ROLLPASS* rollPass1 = hr1->m_rollPass;
	beginTime = hr1->m_NEST_t > endTime ? hr1->m_NEST_t : endTime;
	for (int i = 1; i < individual.size(); i++)
	{
		// 计算frequency
		int NO2 = individual[i];
		HR* hr2 = HR::s_mapSetOfHR.find(NO2)->second;
		int OUT_THICK2 = hr2->m_OUT_THICK;
		ROLLPASS* rollPass2 = hr2->m_rollPass;
		int frequency = ROLLPASS::s_distance.find(make_pair(rollPass1, rollPass2))->second;
		switchFrequency += frequency;
		endTime = beginTime + hr1->m_MAKING_TIME_t;
		// 计算tardiness
		int aTardiness = endTime - hr2->m_NLST_t > 0 ? endTime - hr2->m_NLST_t : 0;
		if (aTardiness > 0)
			tardiness += 1;
		// 计算与前一工单出口厚度差的绝对值
		absThick += abs(OUT_THICK1 - OUT_THICK2);
		// 切换到下一个
		NO1 = NO2;
		hr1 = hr2;
		OUT_THICK1 = OUT_THICK2;
		rollPass1 = rollPass2;
		beginTime = hr1->m_NEST_t > endTime ? hr1->m_NEST_t : endTime;
		// 计算停工待料时间
		if (beginTime > endTime)
			absTime += (beginTime - endTime);
	}
	string endTimeStr = Time::DatetimeToString(endTime + HR::s_standTime);
	//sum = switchFrequency + tardiness / 10000;
	sum = switchFrequency + tardiness * 100 ;
	//a0*与前一工单孔型切换时间+a1*与前一工单出口厚度差的绝对值+a2*停工待料时间+a3*NLST
	//a0 = 7, a1 = 1, a2 = 1000, a3 = 0.001
	//sum = switchFrequency + absThick + absTime;
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
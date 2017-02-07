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

#include "basic.h"

#pragma region 新建的类
////////////////////////////////////////////////////////////////////////
class GA;
////////////////////////////////////////////////////////////////////////
#pragma endregion

class GA
{

#pragma region Variables

public:// 参数
	int				PopSize;		// 种群大小
	int				Gen;			//循环次数
	double			Pc;			//交叉概率
	double			Pm;			//变异概率
	int				bestValue;	// 目前最优解

public:// 集合
	vector<vector<int>	 >	individual;	// 种群
	vector<int>		fitness;		// 种群目标函数值
	vector<double>	prob;		// 轮盘赌的盘
	vector<int>		best;		// 目前最优解

#pragma endregion

#pragma region Functions

public:

	#pragma region 函数
	//////////////////////////////////////////////////////////////////////////

	//制作轮盘赌的轮盘
	void BuildWheel();
	//计算目标函数值
	void ComputeFitness();
	//生成随机个体
	vector<int> GenerateRandomTour();
	//生成时间顺序个体
	vector<int> GenerateSequenceTour();
	//生成孔型顺序个体
	vector<int> GenerateSequenceTour2();
	//轮盘赌方式选取一个个体
	int  SelectIndividual();
	//从lo到hi做交叉
	void PMX(int lo, int hi, vector<int> *tour1, vector<int> *tour2, vector<int> *newtour1, vector<int> *newtour2);
	//变异
	void Mutation();
	//初始化
	void Initialize();
	//选择和交叉
	void SelectionAndCrossover();
	//遗传算法主函数
	static void GeneticAlgorithm();
	// 计算每个个体的旅行总距离
	int ComputeDistance(vector<int> individual);
	// copySolution
	void CopySolution(vector<int> *from, vector<int> *to);
	//根据城市编号在解中寻找城市的位置
	int  FindCity(int city, vector<int> tour);
	//邻域交换
	void SwapOperator(int city1, int city2, vector<int> *tour);
	// 打印结果
	void printResult(vector<int> newSequence);
	
	//////////////////////////////////////////////////////////////////////////
	#pragma endregion

#pragma endregion

};
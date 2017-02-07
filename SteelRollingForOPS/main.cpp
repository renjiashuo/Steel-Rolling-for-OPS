#pragma once

#include "HR.h"
#include "time.h"
#include "ROLLPASS.h"
#include "ga.h"


int main()
{
	// 调试用
	HR::s_mapSetOfHR;
	ROLLPASS::s_mapSetOfROLLPASS;
	ROLLPASS::s_distance;

	// 读取数据
	HR::readData();
	ROLLPASS::readData();
	ROLLPASS::initData();

	// 初始化数据（时间）。
	HR::initDataTime();
	// 初始化HR所属ROLLPASS
	HR::initRollPass();

	// 启发式算法
	//vector<HR> newSequence = HR::localSearch();

	// 遗传算法
	GA::GeneticAlgorithm();

	// 打印结果
	//HR::printResult(newSequence);

	system("pause");
	return 0;
}


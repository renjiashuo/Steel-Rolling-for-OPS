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

#include "basic.h"
#include "ROLLPASS.h"

#pragma region 新建的类
////////////////////////////////////////////////////////////////////////
class HR;
class ROLLPASS;
////////////////////////////////////////////////////////////////////////
#pragma endregion

class HR
{

#pragma region Variables

public:// 参数
	int			m_NO;
	string		m_REC_CREATE_TIME;
	string		m_REC_CREATOR;
	string		m_REC_REVISE_TIME;
	string		m_REC_REVISOR;
	string		m_ARCHIVE_FLAG;
	string		m_WORKSHEET_NO;
	string		m_WHOLE_BACKLOG_CODE;
	string		m_UNIT_CODE;
	string		m_SG_SIGN;
	string		m_SG_STD;
	double		m_LACK_WT;
	int			m_OUT_THICK;
	int			m_OUT_WIDTH;
	string		m_BEGIN_TIME;
	string		m_FINISH_TIME;
	time_t		m_BEGIN_TIME_t;
	time_t		m_FINISH_TIME_t;
	time_t		m_MAKING_TIME_t;
	string		m_ORDER_NO;
	string		m_STATUS;
	time_t		m_new_BEGIN_TIME_t;
	time_t		m_new_FINISH_TIME_t;

public:// 新增参数
	string		m_EPST;
	string		m_NEST;
	string		m_NLST;
	string		m_LPST;
	time_t		m_NEST_t;
	time_t		m_NLST_t;

public:// 参数
	ROLLPASS*		m_rollPass;

public:// 集合
	static map<int, HR *>		s_mapSetOfHR;
	static int				s_HRCount;
	static time_t				s_standTime;
	static time_t				s_END_TIME;

#pragma endregion

#pragma region Functions

public:

	#pragma region 辅助函数
	//////////////////////////////////////////////////////////////////////////

	//
	// 摘要:
	//     构造函数。
	HR(int NO, string REC_CREATE_TIME, string REC_CREATOR, string REC_REVISE_TIME, string REC_REVISOR, string ARCHIVE_FLAG, string WORKSHEET_NO, string WHOLE_BACKLOG_CODE, string UNIT_CODE, string SG_SIGN, string SG_STD, double LACK_WT, int OUT_THICK, int OUT_WIDTH, string EPST, string NEST, string BEGIN_TIME, string FINISH_TIME, string NLST, string LPST, string ORDER_NO, string STATUS);
	//
	// 摘要：
	//     读取数据。
	static void readData();
	//
	// 摘要：
	//     初始化数据（所属孔型系列）。
	static void initRollPass();
	//
	// 摘要：
	//     初始化数据（时间）。
	static void initDataTime();
	//
	// 摘要：
	//     打印结果。
	static void printResult(vector<HR> newSequence);
	//
	// 摘要：
	//     孔型拟合策略（启发式）。
	static vector<HR> heuristic();
	//
	// 摘要：
	//     孔型拟合策略（邻域搜索）。
	static vector<HR> localSearch();
	//
	// 摘要：
	//     孔型拟合策略（邻域搜索）初始解。
	static vector<HR> localSearchInit();
	//
	// 摘要：
	//     孔型拟合策略（邻域搜索）计算目标函数值。
	static void localSearchObj(vector<HR> sequence, int *distance, int *switchFreq);
	//
	// 摘要：
	//     孔型拟合策略（邻域搜索）邻域交换。如果不可行，返回false。
	static bool SwapOperator(vector<HR> *sequence, int a, int b);

	// sort辅助函数
	static bool less_second_int(const HR & m1, const HR & m2);
	static bool more_second_int(const HR & m1, const HR & m2);
	
	//////////////////////////////////////////////////////////////////////////
	#pragma endregion

#pragma endregion

};
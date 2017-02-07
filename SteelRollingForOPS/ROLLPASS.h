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

#include "basic.h"

#pragma region 新建的类
////////////////////////////////////////////////////////////////////////
class ROLLPASS;
////////////////////////////////////////////////////////////////////////
#pragma endregion

class ROLLPASS
{
#pragma region Variables

public:// 参数
	int		m_NO;
	string	m_REC_CREATOR;
	string	m_REC_CREATE_TIME;
	string	m_REC_REVISOR;
	string	m_REC_REVISE_TIME;
	string	m_ARCHIVE_FLAG;
	string	m_APP_SYS_DIV;
	string	m_WHOLE_BACKLOG_CODE;
	string	m_WHOLE_BACKLOG_NAME;
	string	m_PASS_SERIES_CODE;
	string	m_PASS_CODE;
	string	m_TOTAL_RACK_COUNT;
	string	m_IN_THICK;
	string	m_IN_WIDTH;
	string	m_OUT_THICK;
	int		m_OUT_THICK_min;
	int		m_OUT_THICK_max;
	int		m_OUT_WIDTH;
	string	m_USED_RACK_COUNT;
	string	m_RACK_CODE1;
	string	m_RACK_CODE2;
	string	m_RACK_CODE3;
	string	m_RACK_CODE4;
	string	m_RACK_CODE5;
	string	m_RACK_CODE6;
	string	m_RACK_CODE7;
	string	m_RACK_CODE8;
	string	m_RACK_CODE9;
	string	m_RACK_CODE10;
	string	m_RACK_CODE11;
	string	m_RACK_CODE12;
	string	m_RACK_CODE13;
	string	m_RACK_CODE14;
	string	m_RACK_CODE15;
	string	m_RACK_CODE16;
	string	m_RACK_CODE17;
	string	m_RACK_CODE18;
	string	m_RACK_CODE19;
	string	m_RACK_CODE20;
	string	m_RACK_CODE[20];

public:// 集合
	static map<int, ROLLPASS *>		s_mapSetOfROLLPASS;
	static int						s_ROLLPASSCount;
	static map<pair< ROLLPASS*, ROLLPASS* >, int > s_distance;
	//static vector<vector<int> >			s_distance;

#pragma endregion

#pragma region Functions

public:

#pragma region 辅助函数
	//////////////////////////////////////////////////////////////////////////

	//
	// 摘要:
	//     构造函数。
	ROLLPASS(int NO, string REC_CREATOR, string REC_CREATE_TIME, string REC_REVISOR, string REC_REVISE_TIME, string ARCHIVE_FLAG, string APP_SYS_DIV, string WHOLE_BACKLOG_CODE, string 	WHOLE_BACKLOG_NAME, string 	PASS_SERIES_CODE, string PASS_CODE, string TOTAL_RACK_COUNT, string IN_THICK, string IN_WIDTH, string OUT_THICK, int OUT_WIDTH, string USED_RACK_COUNT, string RACK_CODE1, string RACK_CODE2, string RACK_CODE3, string RACK_CODE4, string RACK_CODE5, string RACK_CODE6, string RACK_CODE7, string RACK_CODE8, string RACK_CODE9, string RACK_CODE10, string RACK_CODE11, string RACK_CODE12, string RACK_CODE13, string RACK_CODE14, string RACK_CODE15, string RACK_CODE16, string RACK_CODE17, string RACK_CODE18, string RACK_CODE19, string RACK_CODE20);
	//
	// 摘要：
	//     读取数据。
	static void readData();
	//
	// 摘要：
	//     初始化距离矩阵。
	static void initData();

	//////////////////////////////////////////////////////////////////////////
#pragma endregion

#pragma endregion
};
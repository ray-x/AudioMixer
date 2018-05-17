// ConfData.h: interface for the CConfData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONFDATA_H__E3193AB9_9151_4920_9231_5074DC49BF48__INCLUDED_)
#define AFX_CONFDATA_H__E3193AB9_9151_4920_9231_5074DC49BF48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_CHN_NUM	8

#define USB	0
#define UART 1

class CConfData  
{
public:
	CString m_strConfigFileName;
	int m_USBoUART;
	int m_UartDataW;
	int m_UartPortIdx;
	int m_UartBaud;

	void InitConfData();
	
	CConfData();
	virtual ~CConfData();
	int m_numCh;
	int	m_ChannelConf[MAX_CHN_NUM][MAX_CHN_NUM];
	// m_ChannelConf[m][0~7] means input gain from channel 1~7 for channel m;  0 means mute 

	int	m_InputConf[MAX_CHN_NUM];
	int	m_OutputConf[MAX_CHN_NUM];

};
extern CConfData extConfigData;
#endif // !defined(AFX_CONFDATA_H__E3193AB9_9151_4920_9231_5074DC49BF48__INCLUDED_)

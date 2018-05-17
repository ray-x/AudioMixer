// ConfData.cpp: implementation of the CConfData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AudioMixer.h"
#include "ConfData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConfData::CConfData()
{
	memset(m_InputConf, (BYTE)0x0,sizeof(m_InputConf));
	memset(m_OutputConf,(BYTE)0x0,sizeof(m_OutputConf));
	memset(m_ChannelConf,(BYTE)0x0,sizeof(m_ChannelConf));
	m_numCh=0;
}

CConfData::~CConfData()
{

}

void CConfData::InitConfData()
{
	memset(m_InputConf, (int)0x7,sizeof(m_InputConf));
	memset(m_OutputConf,(int)0x7,sizeof(m_OutputConf));
	memset(m_ChannelConf,(int)0x7,sizeof(m_ChannelConf));
	m_numCh=4;
	m_UartBaud=9600;
	m_UartDataW=8;
	m_UartPortIdx=4;
}


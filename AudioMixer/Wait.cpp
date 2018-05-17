// Wait.cpp : implementation file
//

#include "stdafx.h"
#include "AudioMixer.h"
#include "Wait.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWait dialog


CWait::CWait(CWnd* pParent /*=NULL*/)
	: CDialog(CWait::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWait)
	m_strStatusMsg=_T("≈‰÷√÷–£¨«Î…‘∫Ú......");
	
	//}}AFX_DATA_INIT
}
CWait::CWait(int type,CWnd* pParent /*=NULL*/): CDialog(CWait::IDD, pParent)
{
	m_update_type=type;
	
}


void CWait::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWait)
	DDX_Text(pDX, IDC_STR_STATUS, m_strStatusMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWait, CDialog)
	//{{AFX_MSG_MAP(CWait)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWait message handlers

BOOL CWait::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	COMMCONFIG config;
	
    CSerialPort::GetDefaultConfig(extConfigData.m_UartPortIdx, config);
	if(port.IsOpen() == TRUE) port.Close();

    port.Open(extConfigData.m_UartPortIdx, extConfigData.m_UartBaud, CSerialPort::NoParity, extConfigData.m_UartDataW, CSerialPort::OneStopBit, CSerialPort::NoFlowControl);

	//init the serial port
    HANDLE hPort = port.Detach();
    port.Attach(hPort);
	
    DWORD dwMixerStatus;
    port.GetModemStatus(dwMixerStatus);

	DCB dcb;
    port.GetState(dcb);
	
    dcb.BaudRate = 9600;
    port.SetState(dcb);    
	DWORD dwErrors;                      
    port.ClearError(dwErrors);
	
    port.SetBreak();
    port.ClearBreak();
	
    COMSTAT stat;
    port.GetStatus(stat);
	
    COMMTIMEOUTS timeouts;
    port.GetTimeouts(timeouts);
	
    port.Setup(10000, 10000);
	
    port.GetConfig(config);
	
    config.dcb.BaudRate = 9600;
    port.SetConfig(config);
	
    port.Set0WriteTimeout();
    port.Set0ReadTimeout();

	
	//SendData();


	SetTimer (TIMER_ID, 200, NULL);
	InitConfBuf();
	SendData();

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWait::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	

	m_strStatusMsg=_T("≈‰÷√ÕÍ≥…!");
	UpdateData(FALSE);
	//UpdateData();
	
	//MessageBox("timer",NULL,MB_OK);
	
	KillTimer(TIMER_ID);
	
	CDialog::OnTimer(nIDEvent);

}

void CWait::SendData()
{
	switch (m_update_type){
	case 1:
	//input volume update
	{
		BYTE *p=m_pIVol;
		port.Write(p, 12);
		Sleep(120);
		break;
	}
	case 2:
		//out vol update
	{
		BYTE *p=m_pOVol;
		port.Write(p, 12);
		Sleep(120);
		break;
	}
	case 3:
	//mixer update
	{
		BYTE *p=m_pMix;
		for (int i=0; i<MAX_CHN_NUM;i++)
		{
			port.Write(p, 12);
			Sleep(120);
			p+=12;
		}
		break;
			
	}
	default:
	//update all
	{
		BYTE *p=m_pIVol;
		port.Write(p, 12);

		p=m_pOVol;
		port.Write(p, 12);
		
		p=m_pMix;
		for (int i=0; i<MAX_CHN_NUM;i++)
		{
			port.Write(p, 12);
			p+=12;
		}

	}
	}
}

void CWait::InitConfBuf()
{
	int *p,i;
	p=extConfigData.m_InputConf;
	BYTE *pbuf;
	int sum;
	pbuf=m_arrData;
	
	sum=0;
	m_pIVol=pbuf;
	*pbuf++=(BYTE)0x6A;
	//sum+=0x6A;
	*pbuf++=(BYTE)0xEF;	//command for Input
	sum+=0xEF;
	
	for (i=0;i<8;i++)
	{
		*pbuf++=(BYTE)(*p)&0xFF;
		sum+=(BYTE)(*p++)&0xFF;
	}
	sum&=0xFF;
	*pbuf++=(BYTE)sum;
	*pbuf++ = 0x0d;
	//out vol data
	m_pOVol=pbuf;
	p=extConfigData.m_OutputConf;
	sum=0;
	*pbuf++=(BYTE)0x6A;
	//sum+=0x6A;
	*pbuf++=(BYTE)0xDF;	//command for Onput
	sum+=0xDF;
	for (i=0;i<8;i++)
	{
		*pbuf++=(BYTE)(*p)&0xFF;
		sum+=(BYTE)(*p++)&0xFF;
	}
	sum&=0xFF;
	*pbuf++=(BYTE)sum;
	*pbuf++ = 0x0d;
	
	m_pMix=pbuf;
	sum=0;
	p=&extConfigData.m_ChannelConf[0][0];
	for(i=0;i<8;)
	{
		sum=0;
		*pbuf++=0x6A;
		//sum+=*pbuf++;
		*pbuf=0xC1+(BYTE)i;
		sum+=*pbuf++;
		for (int j=0;j<MAX_CHN_NUM;j++)
		{
			*pbuf=(BYTE)(*p++)&0xFF;
			sum+=*pbuf++;
		}
		*pbuf++=(BYTE)(sum&0xFF);	
		*pbuf++ = 0x0d; 		// add	
		i += 2;
	}
}

unsigned char sendbuf[12];
void CWait::UpdateInputVol(int ch, int vol)
{
	memset(sendbuf,0x00,12);
	sendbuf[0]=0x6a;
	sendbuf[1]=0xef;	
	sendbuf[2]=0x1<<(ch-1);
	sendbuf[3]=(unsigned char)(vol&0xff );
	sendbuf[10]=(sendbuf[1]+	sendbuf[2]+	sendbuf[3])&0xff;
	sendbuf[11]=0x0D;
	
	port.Write(sendbuf, 12);



	
}

void CWait::UpdateOutputVol(int ch, int vol)
{

	memset(sendbuf,0x00,12);
	sendbuf[0]=0x6a;
	sendbuf[1]=0xdf;	
	sendbuf[2]=0x1<<(ch-1);
	sendbuf[3]=(unsigned char)(vol&0xff );
	sendbuf[10]=(sendbuf[1]+sendbuf[2]+	sendbuf[3])&0xff;
	sendbuf[11]=0x0D;
	
	port.Write(sendbuf, 12);
	

}

void CWait::UpdateMixConf(int ch)
{
	memset(sendbuf,0x00,12);
	sendbuf[0]=0x6a;
	sendbuf[1]=0xc0+(unsigned char)(ch*2 -1);	
	unsigned char sum = sendbuf[1];
	for (int i=0;i<8;i++)
	{
		sendbuf[i+2]=extConfigData.m_ChannelConf[ch-1][i]&0xFF;
		sum+=sendbuf[i+2];
	}
	sendbuf[10]=(unsigned char)(sum&0xff );
	sendbuf[11]=0x0d;	
	port.Write(sendbuf, 12);
}

void CWait::Init()
{
	COMMCONFIG config;

	port.Close();
    CSerialPort::GetDefaultConfig(extConfigData.m_UartPortIdx, config);
	



    port.Open(extConfigData.m_UartPortIdx, extConfigData.m_UartBaud, CSerialPort::NoParity, extConfigData.m_UartDataW, CSerialPort::OneStopBit, CSerialPort::NoFlowControl);
	
	//init the serial port
    HANDLE hPort = port.Detach();
    port.Attach(hPort);
	
    DWORD dwMixerStatus;
    port.GetModemStatus(dwMixerStatus);
	
	DCB dcb;
    port.GetState(dcb);
	
    dcb.BaudRate = 9600;
    port.SetState(dcb);    
	DWORD dwErrors;                      
    port.ClearError(dwErrors);
	
    port.SetBreak();
    port.ClearBreak();
	
    COMSTAT stat;
    port.GetStatus(stat);
	
    COMMTIMEOUTS timeouts;
    port.GetTimeouts(timeouts);
	
    port.Setup(10000, 10000);
	
    port.GetConfig(config);
	
    config.dcb.BaudRate = 9600;
    port.SetConfig(config);
	
    port.Set0WriteTimeout();
    port.Set0ReadTimeout();
	
	
	//SendData();
	
	
	//SetTimer (TIMER_ID, 200, NULL);
	InitConfBuf();
	//SendData();


}

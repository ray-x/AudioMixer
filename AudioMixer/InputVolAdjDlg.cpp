// InputVolAdjDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AudioMixer.h"
#include "InputVolAdjDlg.h"
#include "ConfData.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "Wait.h"
/////////////////////////////////////////////////////////////////////////////
// CInputVolAdjDlg dialog


CInputVolAdjDlg::CInputVolAdjDlg(CWnd* pParent /*=NULL*/)
: CPropertyPage(CInputVolAdjDlg::IDD)
{
	//{{AFX_DATA_INIT(CInputVolAdjDlg)
	m_inputVol1 = 0;
	m_inputVol2 = 0;
	m_inputVol3 = 0;
	m_inputVol4 = 0;
	m_inputVol5 = 0;
	m_inputVol6 = 0;
	m_inputVol7 = 0;
	m_inputVol8 = 0;
	m_numTotalCh = 0;
	//}}AFX_DATA_INIT
}


void CInputVolAdjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputVolAdjDlg)
	DDX_Control(pDX, IDC_SLIDER8, m_SliderInput8);
	DDX_Control(pDX, IDC_SLIDER7, m_SliderInput7);
	DDX_Control(pDX, IDC_SLIDER6, m_SliderInput6);
	DDX_Control(pDX, IDC_SLIDER5, m_SliderInput5);
	DDX_Control(pDX, IDC_SLIDER4, m_SliderInput4);
	DDX_Control(pDX, IDC_SLIDER3, m_SliderInput3);
	DDX_Control(pDX, IDC_SLIDER2, m_SliderInput2);
	DDX_Control(pDX, IDC_SLIDER1, m_SliderInput1);
	DDX_Slider(pDX, IDC_SLIDER1, m_inputVol1);
	DDX_Slider(pDX, IDC_SLIDER2, m_inputVol2);
	DDX_Slider(pDX, IDC_SLIDER3, m_inputVol3);
	DDX_Slider(pDX, IDC_SLIDER4, m_inputVol4);
	DDX_Slider(pDX, IDC_SLIDER5, m_inputVol5);
	DDX_Slider(pDX, IDC_SLIDER6, m_inputVol6);
	DDX_Slider(pDX, IDC_SLIDER7, m_inputVol7);
	DDX_Slider(pDX, IDC_SLIDER8, m_inputVol8);
	DDX_Radio(pDX, IDC_RADIO_FOURCH, m_numTotalCh);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputVolAdjDlg, CDialog)
	//{{AFX_MSG_MAP(CInputVolAdjDlg)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_IVOLCHANGE, OnButtonApplyInputVolChange)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputVolAdjDlg message handlers

BOOL CInputVolAdjDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_SliderInput1.SetRange(0,15);
	m_SliderInput2.SetRange(0,15);
	m_SliderInput3.SetRange(0,15);
	m_SliderInput4.SetRange(0,15);
	m_SliderInput5.SetRange(0,15);
	m_SliderInput6.SetRange(0,15);
	m_SliderInput7.SetRange(0,15);
	m_SliderInput8.SetRange(0,15);
	m_SliderInput1.SetPos(0xF-extConfigData.m_InputConf[0]&0xF);
	m_SliderInput1.SetRange(0,15,TRUE);
	m_SliderInput2.SetPos(0xF-extConfigData.m_InputConf[1]&0xF);
	m_SliderInput2.SetRange(0,15,TRUE);
	m_SliderInput3.SetPos(0xF-extConfigData.m_InputConf[2]&0xF);
	m_SliderInput3.SetRange(0,15,TRUE);
	m_SliderInput4.SetPos(0xF-extConfigData.m_InputConf[3]&0xF);
	m_SliderInput4.SetRange(0,15,TRUE);
	m_SliderInput5.SetPos(0xF-extConfigData.m_InputConf[4]&0xF);
	m_SliderInput5.SetRange(0,15,TRUE);
	m_SliderInput6.SetPos(0xF-extConfigData.m_InputConf[5]&0xF);
	m_SliderInput6.SetRange(0,15,TRUE);
	m_SliderInput7.SetPos(0xF-extConfigData.m_InputConf[6]&0xF);
	m_SliderInput7.SetRange(0,15,TRUE);
	m_SliderInput8.SetPos(0xF-extConfigData.m_InputConf[7]&0xF);
	m_SliderInput8.SetRange(0,15,TRUE);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputVolAdjDlg::OnButtonApplyInputVolChange() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
 	extConfigData.m_InputConf[0]=0xF-m_inputVol1&0xF;
 	extConfigData.m_InputConf[1]=0xF-m_inputVol2&0xF;
 	extConfigData.m_InputConf[2]=0xF-m_inputVol3&0xF;
 	extConfigData.m_InputConf[3]=0xF-m_inputVol4&0xF;
 	if(m_numTotalCh==1)
 	{
 		extConfigData.m_InputConf[4]=0xF-m_inputVol5&0xF;
 		extConfigData.m_InputConf[5]=0xF-m_inputVol6&0xF;
 		extConfigData.m_InputConf[6]=0xF-m_inputVol7&0xF;
 		extConfigData.m_InputConf[7]=0xF-m_inputVol8&0xF;
 
 	}
	CWait dlg(1,NULL);	//1 means input
	dlg.DoModal();
	//dlg.SendData();
	

}

void CInputVolAdjDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int ch;
	UpdateData(TRUE);

	CWait dlg(1,NULL);	//1 means input

	dlg.Init();
	//dlg.InitConfBuf();

	if(extConfigData.m_InputConf[0]!=(0xF-m_inputVol1&0xF)){
		extConfigData.m_InputConf[0]=0xF-m_inputVol1&0xF;
		ch=1;
		dlg.UpdateInputVol(ch,extConfigData.m_InputConf[ch-1] );
	}
	if(extConfigData.m_InputConf[1]!=(0xF-m_inputVol2&0xF)){
		extConfigData.m_InputConf[1]=0xF-m_inputVol2&0xF;
		ch=2;
		dlg.UpdateInputVol(ch,extConfigData.m_InputConf[ch-1] );
	}

	if(extConfigData.m_InputConf[2]!=(0xF-m_inputVol3&0xF)){
		extConfigData.m_InputConf[2]=0xF-m_inputVol3&0xF;
		ch=3;
		dlg.UpdateInputVol(ch,extConfigData.m_InputConf[ch-1] );
	}

	if(extConfigData.m_InputConf[3]!=(0xF-m_inputVol4&0xF)){
		extConfigData.m_InputConf[3]=0xF-m_inputVol4&0xF;
		ch=4;
		dlg.UpdateInputVol(ch,extConfigData.m_InputConf[ch-1] );

	}
	if(m_numTotalCh==1)
	{
		if(extConfigData.m_InputConf[4]!=(0xF-m_inputVol5&0xF)){
			extConfigData.m_InputConf[4]=0xF-m_inputVol5&0xF;
			ch=5;
			dlg.UpdateInputVol(ch,extConfigData.m_InputConf[ch-1] );

		}
		if (extConfigData.m_InputConf[5]!=(0xF-m_inputVol6&0xF))
		{
			extConfigData.m_InputConf[5]=0xF-m_inputVol6&0xF;
			ch=6;
			dlg.UpdateInputVol(ch,extConfigData.m_InputConf[ch-1] );

		}
		if (extConfigData.m_InputConf[6]!=(0xF-m_inputVol7&0xF))
		{
			extConfigData.m_InputConf[6]=0xF-m_inputVol7&0xF;
			ch=7;
			dlg.UpdateInputVol(ch,extConfigData.m_InputConf[ch-1] );

		}
		if (extConfigData.m_InputConf[7]!=(0xF-m_inputVol8&0xF))
		{
			extConfigData.m_InputConf[7]=0xF-m_inputVol8&0xF;
			ch=8;
			dlg.UpdateInputVol(ch,extConfigData.m_InputConf[ch-1] );

		}
		
	}else
	{
		extConfigData.m_InputConf[4]=extConfigData.m_InputConf[5]=extConfigData.m_InputConf[6]=extConfigData.m_InputConf[7]=0x00;
	}
	if (ch>8||ch<1)
	{
		ch=1;
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

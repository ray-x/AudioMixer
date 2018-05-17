// MixChConf.cpp : implementation file
//

#include "stdafx.h"
#include "AudioMixer.h"
#include "MixChConf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "ConfData.h"
#include "Wait.h"

bool	fVScroll = false;
/////////////////////////////////////////////////////////////////////////////
// CMixChConf dialog


CMixChConf::CMixChConf(CWnd* pParent /*=NULL*/)
	: CDialog(CMixChConf::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMixChConf)
	m_strChannelNumber = _T("");
	m_bChannel1 = TRUE;
	m_bChannel2 = TRUE;
	m_bChannel3 = TRUE;
	m_bChannel4 = TRUE;
	m_bChannel5 = FALSE;
	m_bChannel6 = FALSE;
	m_bChannel7 = FALSE;
	m_bChannel8 = FALSE;
	m_VolInputVol1 = 0;
	m_VolInputVol2 = 0;
	m_VolInputVol3 = 0;
	m_VolInputVol4 = 0;
	m_VolInputVol5 = 0;
	m_VolInputVol6 = 0;
	m_VolInputVol7 = 0;
	m_VolInputVol8 = 0;
	m_TotalChNum = 0;
	//}}AFX_DATA_INIT
}


void CMixChConf::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMixChConf)
	DDX_Control(pDX, IDC_RADIO_4, m_RadioBoxCh);
	DDX_Control(pDX, IDC_SLIDER_INPUT8, m_ctlSliderInput8);
	DDX_Control(pDX, IDC_SLIDER_INPUT7, m_ctlSliderInput7);
	DDX_Control(pDX, IDC_SLIDER_INPUT6, m_ctlSliderInput6);
	DDX_Control(pDX, IDC_SLIDER_INPUT5, m_ctlSliderInput5);
	DDX_Control(pDX, IDC_SLIDER_INPUT4, m_ctlSliderInput4);
	DDX_Control(pDX, IDC_SLIDER_INPUT3, m_ctlSliderInput3);
	DDX_Control(pDX, IDC_SLIDER_INPUT2, m_ctlSliderInput2);
	DDX_Control(pDX, IDC_SLIDER_INPUT1, m_ctlSliderInput1);
	DDX_Text(pDX, IDC_CHNUM, m_strChannelNumber);
	DDX_Check(pDX, IDC_CHECK_MIX1, m_bChannel1);
	DDX_Check(pDX, IDC_CHECK_MIX2, m_bChannel2);
	DDX_Check(pDX, IDC_CHECK_MIX3, m_bChannel3);
	DDX_Check(pDX, IDC_CHECK_MIX4, m_bChannel4);
	DDX_Check(pDX, IDC_CHECK_MIX5, m_bChannel5);
	DDX_Check(pDX, IDC_CHECK_MIX6, m_bChannel6);
	DDX_Check(pDX, IDC_CHECK_MIX7, m_bChannel7);
	DDX_Check(pDX, IDC_CHECK_MIX8, m_bChannel8);
	DDX_Slider(pDX, IDC_SLIDER_INPUT1, m_VolInputVol1);
	DDX_Slider(pDX, IDC_SLIDER_INPUT2, m_VolInputVol2);
	DDX_Slider(pDX, IDC_SLIDER_INPUT3, m_VolInputVol3);
	DDX_Slider(pDX, IDC_SLIDER_INPUT4, m_VolInputVol4);
	DDX_Slider(pDX, IDC_SLIDER_INPUT5, m_VolInputVol5);
	DDX_Slider(pDX, IDC_SLIDER_INPUT6, m_VolInputVol6);
	DDX_Slider(pDX, IDC_SLIDER_INPUT7, m_VolInputVol7);
	DDX_Slider(pDX, IDC_SLIDER_INPUT8, m_VolInputVol8);
	DDX_Radio(pDX, IDC_RADIO_4, m_TotalChNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMixChConf, CDialog)
	//{{AFX_MSG_MAP(CMixChConf)
	ON_BN_CLICKED(IDC_RADIO_4, OnRadio4Ch)
	ON_BN_CLICKED(IDC_RADIO_8, OnRadio8Ch)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMixChConf message handlers

void CMixChConf::OnRadio4Ch() 
{
	// TODO: Add your control notification handler code here
	if (m_TotalChNum==8)
	{
		m_TotalChNum=4;
		//disable the 8 channel tab
		GetDlgItem(IDC_SLIDER_INPUT5)->EnableWindow(false);
		GetDlgItem(IDC_SLIDER_INPUT6)->EnableWindow(false);
		GetDlgItem(IDC_SLIDER_INPUT7)->EnableWindow(false);
		GetDlgItem(IDC_SLIDER_INPUT8)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_MIX5)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_MIX6)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_MIX7)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_MIX8)->EnableWindow(false);
	}



}

void CMixChConf::OnRadio8Ch() 
{
	// TODO: Add your control notification handler code here
	if (m_TotalChNum==4)
	{
		m_TotalChNum=8;
		GetDlgItem(IDC_SLIDER_INPUT5)->EnableWindow(true);
		GetDlgItem(IDC_SLIDER_INPUT6)->EnableWindow(true);
		GetDlgItem(IDC_SLIDER_INPUT7)->EnableWindow(true);
		GetDlgItem(IDC_SLIDER_INPUT8)->EnableWindow(true);
		GetDlgItem(IDC_CHECK_MIX5)->EnableWindow(true);
		GetDlgItem(IDC_CHECK_MIX6)->EnableWindow(true);
		GetDlgItem(IDC_CHECK_MIX7)->EnableWindow(true);
		GetDlgItem(IDC_CHECK_MIX8)->EnableWindow(true);
	}
	
}


void CMixChConf::OnButtonApply() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	ASSERT(m_numCh>=1&&m_numCh<=MAX_CHN_NUM) ;
	int idx=m_numCh-1;

	extConfigData.m_ChannelConf[idx][0] = m_bChannel1? (0xF-m_VolInputVol1&0xF) : 0;

	extConfigData.m_ChannelConf[idx][1] = m_bChannel2? (0xF-m_VolInputVol2&0xF) : 0;

	extConfigData.m_ChannelConf[idx][2]=  m_bChannel3? (0xF-m_VolInputVol3&0xF) : 0;

	extConfigData.m_ChannelConf[idx][3]=  m_bChannel4? (0xF-m_VolInputVol4&0xF) : 0;

	extConfigData.m_ChannelConf[idx][4]=  m_bChannel5? (0xF-m_VolInputVol5&0xF) : 0;

	extConfigData.m_ChannelConf[idx][5]=  m_bChannel6? (0xF-m_VolInputVol6&0xF) : 0;

	extConfigData.m_ChannelConf[idx][6]=  m_bChannel7? (0xF-m_VolInputVol7&0xF) : 0;

	extConfigData.m_ChannelConf[idx][7]=  m_bChannel8? (0xF-m_VolInputVol8&0xF) : 0;
	

	CWait dlg(3,NULL);
	dlg.DoModal();

}

BOOL CMixChConf::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int idx=m_numCh-1;
	// TODO: Add extra initialization here
	m_TotalChNum=4;
	m_ctlSliderInput1.SetRange(0,15);
	m_ctlSliderInput2.SetRange(0,15);
	m_ctlSliderInput3.SetRange(0,15);
	m_ctlSliderInput4.SetRange(0,15);
	m_ctlSliderInput5.SetRange(0,15);
	m_ctlSliderInput6.SetRange(0,15);
	m_ctlSliderInput7.SetRange(0,15);
	m_ctlSliderInput8.SetRange(0,15);
	m_ctlSliderInput1.SetPos(0xF-extConfigData.m_ChannelConf[idx][0]&0xF);
	m_ctlSliderInput1.SetRange(0,15,TRUE);
	m_ctlSliderInput2.SetPos(0xF-extConfigData.m_ChannelConf[idx][1]&0xF);
	m_ctlSliderInput2.SetRange(0,15,TRUE);
	m_ctlSliderInput3.SetPos(0xF-extConfigData.m_ChannelConf[idx][2]&0xF);
	m_ctlSliderInput3.SetRange(0,15,TRUE);
	m_ctlSliderInput4.SetPos(0xF-extConfigData.m_ChannelConf[idx][3]&0xF);
	m_ctlSliderInput4.SetRange(0,15,TRUE);
	m_ctlSliderInput5.SetPos(0xF-extConfigData.m_ChannelConf[idx][4]&0xF);
	m_ctlSliderInput5.SetRange(0,15,TRUE);
	m_ctlSliderInput6.SetPos(0xF-extConfigData.m_ChannelConf[idx][5]&0xF);
	m_ctlSliderInput6.SetRange(0,15,TRUE);
	m_ctlSliderInput7.SetPos(0xF-extConfigData.m_ChannelConf[idx][6]&0xF);
	m_ctlSliderInput7.SetRange(0,15,TRUE);
	m_ctlSliderInput8.SetPos(0xF-extConfigData.m_ChannelConf[idx][7]&0xF);
	m_ctlSliderInput8.SetRange(0,15,TRUE);

	GetDlgItem(IDC_SLIDER_INPUT5)->EnableWindow(false);
	GetDlgItem(IDC_SLIDER_INPUT6)->EnableWindow(false);
	GetDlgItem(IDC_SLIDER_INPUT7)->EnableWindow(false);
	GetDlgItem(IDC_SLIDER_INPUT8)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_MIX5)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_MIX6)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_MIX7)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_MIX8)->EnableWindow(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMixChConf::ApplyDataChange()
{
	OnButtonApply();
}

void CMixChConf::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(fVScroll == true){
		fVScroll = false;

	}
	else{
		fVScroll = true;
		return;
	}
	UpdateData(TRUE);
	int idx=m_numCh-1;
	
	if(m_TotalChNum==1)			// 8·ģʽ
	{

		extConfigData.m_ChannelConf[idx][0]=0xF-m_VolInputVol1&0xF;
		extConfigData.m_ChannelConf[idx][1]=0xF-m_VolInputVol2&0xF;
		extConfigData.m_ChannelConf[idx][2]=0xF-m_VolInputVol3&0xF;
		extConfigData.m_ChannelConf[idx][3]=0xF-m_VolInputVol4&0xF;
		extConfigData.m_ChannelConf[idx][4]=0xF-m_VolInputVol5&0xF;
		extConfigData.m_ChannelConf[idx][5]=0xF-m_VolInputVol6&0xF;
		extConfigData.m_ChannelConf[idx][6]=0xF-m_VolInputVol7&0xF;
		extConfigData.m_ChannelConf[idx][7]=0xF-m_VolInputVol8&0xF;		
	}else						// 4·ģʽ
	{
		extConfigData.m_ChannelConf[idx][0]=0xF-m_VolInputVol1&0xF;
		extConfigData.m_ChannelConf[idx][2]=0xF-m_VolInputVol2&0xF;
		extConfigData.m_ChannelConf[idx][4]=0xF-m_VolInputVol3&0xF;
		extConfigData.m_ChannelConf[idx][6]=0xF-m_VolInputVol4&0xF;

		extConfigData.m_ChannelConf[idx][1]=0x0;
		extConfigData.m_ChannelConf[idx][3]=0x0;
		extConfigData.m_ChannelConf[idx][5]=0x0;
		extConfigData.m_ChannelConf[idx][7]=0x0;	
	}
	CWait dlg(3,NULL);
	dlg.Init();
	dlg.UpdateMixConf(m_numCh);

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

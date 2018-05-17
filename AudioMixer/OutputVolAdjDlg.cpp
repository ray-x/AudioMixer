// OutputVolAdjDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AudioMixer.h"
#include "OutputVolAdjDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "ConfData.h"
#include "Wait.h"
/////////////////////////////////////////////////////////////////////////////
// COutputVolAdjDlg dialog


COutputVolAdjDlg::COutputVolAdjDlg(CWnd* pParent /*=NULL*/)
: CPropertyPage(COutputVolAdjDlg::IDD)
{
	//{{AFX_DATA_INIT(COutputVolAdjDlg)
	m_OutputVol1 = 0;
	m_OutputVol2 = 0;
	m_OutputVol3 = 0;
	m_OutputVol4 = 0;
	m_OutputVol5 = 0;
	m_OutputVol6 = 0;
	m_OutputVol7 = 0;
	m_OutputVol8 = 0;
	m_numTotalCh = 0;
	//}}AFX_DATA_INIT
}


void COutputVolAdjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutputVolAdjDlg)
	DDX_Control(pDX, IDC_SLIDER8, m_SliderOutputVol8);
	DDX_Control(pDX, IDC_SLIDER7, m_SliderOutputVol7);
	DDX_Control(pDX, IDC_SLIDER6, m_SliderOutputVol6);
	DDX_Control(pDX, IDC_SLIDER5, m_SliderOutputVol5);
	DDX_Control(pDX, IDC_SLIDER4, m_SliderOutputVol4);
	DDX_Control(pDX, IDC_SLIDER3, m_SliderOutputVol3);
	DDX_Control(pDX, IDC_SLIDER2, m_SliderOutputVol2);
	DDX_Control(pDX, IDC_SLIDER1, m_SliderOutputVol1);
	DDX_Slider(pDX, IDC_SLIDER1, m_OutputVol1);
	DDX_Slider(pDX, IDC_SLIDER2, m_OutputVol2);
	DDX_Slider(pDX, IDC_SLIDER3, m_OutputVol3);
	DDX_Slider(pDX, IDC_SLIDER4, m_OutputVol4);
	DDX_Slider(pDX, IDC_SLIDER5, m_OutputVol5);
	DDX_Slider(pDX, IDC_SLIDER6, m_OutputVol6);
	DDX_Slider(pDX, IDC_SLIDER7, m_OutputVol7);
	DDX_Slider(pDX, IDC_SLIDER8, m_OutputVol8);
	DDX_Radio(pDX, IDC_RADIO_FOURCH, m_numTotalCh);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COutputVolAdjDlg, CDialog)
	//{{AFX_MSG_MAP(COutputVolAdjDlg)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_OUTVOL, OnButtonApplyOutVol)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputVolAdjDlg message handlers

BOOL COutputVolAdjDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_SliderOutputVol1.SetRange(0,15);
	m_SliderOutputVol2.SetRange(0,15);
	m_SliderOutputVol3.SetRange(0,15);
	m_SliderOutputVol4.SetRange(0,15);
	m_SliderOutputVol5.SetRange(0,15);
	m_SliderOutputVol6.SetRange(0,15);
	m_SliderOutputVol7.SetRange(0,15);
	m_SliderOutputVol8.SetRange(0,15);
	m_SliderOutputVol1.SetPos(0xF-extConfigData.m_OutputConf[0]&0xF);
	m_SliderOutputVol1.SetRange(0,15,TRUE);
	m_SliderOutputVol2.SetPos(0xF-extConfigData.m_OutputConf[1]&0xF);
	m_SliderOutputVol2.SetRange(0,15,TRUE);
	m_SliderOutputVol3.SetPos(0xF-extConfigData.m_OutputConf[2]&0xF);
	m_SliderOutputVol3.SetRange(0,15,TRUE);
	m_SliderOutputVol4.SetPos(0xF-extConfigData.m_OutputConf[3]&0xF);
	m_SliderOutputVol4.SetRange(0,15,TRUE);
	m_SliderOutputVol5.SetPos(0xF-extConfigData.m_OutputConf[4]&0xF);
	m_SliderOutputVol5.SetRange(0,15,TRUE);
	m_SliderOutputVol6.SetPos(0xF-extConfigData.m_OutputConf[5]&0xF);
	m_SliderOutputVol6.SetRange(0,15,TRUE);
	m_SliderOutputVol7.SetPos(0xF-extConfigData.m_OutputConf[6]&0xF);
	m_SliderOutputVol7.SetRange(0,15,TRUE);
	m_SliderOutputVol8.SetPos(0xF-extConfigData.m_OutputConf[7]&0xF);
	m_SliderOutputVol8.SetRange(0,15,TRUE);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COutputVolAdjDlg::OnButtonApplyOutVol() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(FALSE);
 	extConfigData.m_OutputConf[0]=0xF-m_OutputVol1&0xF;
 	extConfigData.m_OutputConf[1]=0xF-m_OutputVol2&0xF;
 	extConfigData.m_OutputConf[2]=0xF-m_OutputVol3&0xF;
 	extConfigData.m_OutputConf[3]=0xF-m_OutputVol4&0xF;
 	if (m_numTotalCh==1)
 	{
 		extConfigData.m_OutputConf[4]=0xF-m_OutputVol5&0xF;
 		extConfigData.m_OutputConf[5]=0xF-m_OutputVol6&0xF;
 		extConfigData.m_OutputConf[6]=0xF-m_OutputVol7&0xF;
 		extConfigData.m_OutputConf[7]=0xF-m_OutputVol8&0xF;
 	}

	CWait dlg(2,NULL);
	dlg.DoModal();

}

void COutputVolAdjDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	UpdateData(TRUE);
	int ch=1;
	CWait dlg(2,NULL);
	dlg.Init();
	//dlg.InitConfBuf();
	//dlg.SendData();

	if (extConfigData.m_OutputConf[0]!=(0xF-m_OutputVol1&0xF))
	{
		extConfigData.m_OutputConf[0]=0xF-m_OutputVol1&0xF;
		ch=1;
		dlg.UpdateOutputVol(ch,extConfigData.m_OutputConf[ch-1]);
	}
	if (extConfigData.m_OutputConf[1]!=(0xF-m_OutputVol2&0xF))
	{
		extConfigData.m_OutputConf[1]=0xF-m_OutputVol2&0xF;
		ch=2;
		dlg.UpdateOutputVol(ch,extConfigData.m_OutputConf[ch-1]);
	}

	if (extConfigData.m_OutputConf[2]!=(0xF-m_OutputVol3&0xF))
	{
		extConfigData.m_OutputConf[2]=0xF-m_OutputVol3&0xF;
		ch=3;
		dlg.UpdateOutputVol(ch,extConfigData.m_OutputConf[ch-1]);
	}
	if (extConfigData.m_OutputConf[3]!=(0xF-m_OutputVol4&0xF))
	{
		extConfigData.m_OutputConf[3]=0xF-m_OutputVol4&0xF;
		ch=4;
		dlg.UpdateOutputVol(ch,extConfigData.m_OutputConf[ch-1]);
	}
	if(m_numTotalCh==1)
	{
		if (extConfigData.m_OutputConf[4]!=(0xF-m_OutputVol5&0xF))
		{
			extConfigData.m_OutputConf[4]=0xF-m_OutputVol5&0xF;
			ch=5;
			dlg.UpdateOutputVol(ch,extConfigData.m_OutputConf[ch-1]);
		}
			if (extConfigData.m_OutputConf[5]!=(0xF-m_OutputVol6&0xF))
		{
			extConfigData.m_OutputConf[5]=0xF-m_OutputVol6&0xF;
			ch=6;
			dlg.UpdateOutputVol(ch,extConfigData.m_OutputConf[ch-1]);
		}
		if (extConfigData.m_OutputConf[6]!=(0xF-m_OutputVol7&0xF))
		{
			extConfigData.m_OutputConf[6]=0xF-m_OutputVol7&0xF;
			ch=7;
			dlg.UpdateOutputVol(ch,extConfigData.m_OutputConf[ch-1]);
		}
		if (extConfigData.m_OutputConf[7]!=(0xF-m_OutputVol8&0xF))
		{
			extConfigData.m_OutputConf[7]=0xF-m_OutputVol8&0xF;
			ch=8;
			dlg.UpdateOutputVol(ch,extConfigData.m_OutputConf[ch-1]);
		}	
	}else
		extConfigData.m_OutputConf[4]=extConfigData.m_OutputConf[5]=extConfigData.m_OutputConf[6]=extConfigData.m_OutputConf[7]=0;

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

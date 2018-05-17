// MixConf.cpp : implementation file
//

#include "stdafx.h"
#include "AudioMixer.h"
#include "MixConf.h"
#include "InputVolAdjDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMixConf dialog


CMixConf::CMixConf(CWnd* pParent /*=NULL*/)
	: CDialog(CMixConf::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMixConf)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMixConf::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMixConf)
	DDX_Control(pDX, IDC_TAB_MIX, m_mixTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMixConf, CDialog)
	//{{AFX_MSG_MAP(CMixConf)
	ON_NOTIFY(NM_CLICK, IDC_TAB_MIX, OnClickTabMix)
	ON_BN_CLICKED(IDOK, OnMixOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMixConf message handlers

BOOL CMixConf::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (m_mixTab.m_nNumOfPages==0)
	{
		m_mixTab.AddPage("ͨ��1", &m_dlgMixCh[0], IDD_PROPPAGEL_MIXCTL);
		m_dlgMixCh[0].m_strChannelNumber=_T("ͨ��1����");
		m_dlgMixCh[0].m_numCh=1;
		m_mixTab.AddPage("ͨ��2", &m_dlgMixCh[1], IDD_PROPPAGEL_MIXCTL);
		m_dlgMixCh[1].m_strChannelNumber=_T("ͨ��2����");
		m_dlgMixCh[1].m_numCh=2;
		m_mixTab.AddPage("ͨ��3", &m_dlgMixCh[2], IDD_PROPPAGEL_MIXCTL);
		m_dlgMixCh[2].m_strChannelNumber=_T("ͨ��3����");
		m_dlgMixCh[2].m_numCh=3;
		m_mixTab.AddPage("ͨ��4", &m_dlgMixCh[3], IDD_PROPPAGEL_MIXCTL);
		m_dlgMixCh[3].m_strChannelNumber=_T("ͨ��4����");
		m_dlgMixCh[3].m_numCh=4;
		m_mixTab.AddPage("ͨ��5", &m_dlgMixCh[4], IDD_PROPPAGEL_MIXCTL);
		m_dlgMixCh[4].m_strChannelNumber=_T("ͨ��5����");
		m_dlgMixCh[4].m_numCh=5;
		m_mixTab.AddPage("ͨ��6", &m_dlgMixCh[5], IDD_PROPPAGEL_MIXCTL);
		m_dlgMixCh[5].m_strChannelNumber=_T("ͨ��6����");
		m_dlgMixCh[5].m_numCh=6;
		m_mixTab.AddPage("ͨ��7", &m_dlgMixCh[6], IDD_PROPPAGEL_MIXCTL);
		m_dlgMixCh[6].m_strChannelNumber=_T("ͨ��7����");
		m_dlgMixCh[6].m_numCh=7;
		m_mixTab.AddPage("ͨ��8", &m_dlgMixCh[7], IDD_PROPPAGEL_MIXCTL);		
		m_dlgMixCh[7].m_strChannelNumber=_T("ͨ��8����");
		m_dlgMixCh[7].m_numCh=8;
	}
	
	m_mixTab.Show();

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMixConf::OnClickTabMix(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here	
	*pResult = 0;
}

void CMixConf::OnMixOK() 
{
	// TODO: Add your control notification handler code here
	for (int i=0;i<8;i++)
		m_dlgMixCh[i].ApplyDataChange();
	OnOK();
}

// VolAdjDlgBox.cpp : implementation file
//

#include "stdafx.h"
#include "AudioMixer.h"
#include "VolAdjDlgBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVolAdjDlgBox dialog


CVolAdjDlgBox::CVolAdjDlgBox(CWnd* pParent /*=NULL*/)
	: CDialog(CVolAdjDlgBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVolAdjDlgBox)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//m_idx=0;

}


void CVolAdjDlgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVolAdjDlgBox)
	DDX_Control(pDX, IDC_VOL_TAB, m_VolCtl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVolAdjDlgBox, CDialog)
	//{{AFX_MSG_MAP(CVolAdjDlgBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVolAdjDlgBox message handlers

BOOL CVolAdjDlgBox::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_VolCtl.m_nNumOfPages==0)
	{
		m_VolCtl.AddPage("输入音量", &m_inputVol, IDD_PROPPAGEL_INPUT_VOLCTL);
		m_VolCtl.AddPage("输出音量", &m_outputVol, IDD_PROPPAGEL_OUTPUT_VOLCTL);
	}

	m_VolCtl.Show();
	if (m_idx==1)
		m_VolCtl.SetCurSel(1);
	else
		m_VolCtl.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVolAdjDlgBox::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

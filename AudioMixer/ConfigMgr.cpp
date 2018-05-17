// ConfigMgr.cpp : implementation file
//

#include "stdafx.h"
#include "AudioMixer.h"
#include "ConfigMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigMgr dialog


CConfigMgr::CConfigMgr(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CConfigMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigMgr, CDialog)
	//{{AFX_MSG_MAP(CConfigMgr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigMgr message handlers

BOOL CConfigMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_sheet.AddPage(&m_dlgConectMgr);
	m_sheet.AddPage(&m_dlgScheMgr);
	m_sheet.Create(this, WS_CHILD | WS_VISIBLE, WS_EX_CONTROLPARENT);

	RECT rect;
	m_sheet.GetWindowRect(&rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	
	m_sheet.SetWindowPos(NULL, 20, 50, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// VolCtl.cpp : implementation file
//

#include "stdafx.h"
#include "AudioMixer.h"
#include "VolCtl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVolCtl dialog


CVolCtl::CVolCtl(CWnd* pParent /*=NULL*/)
	: CDialog(CVolCtl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVolCtl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CVolCtl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVolCtl)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVolCtl, CDialog)
	//{{AFX_MSG_MAP(CVolCtl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVolCtl message handlers

BOOL CVolCtl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if (m_sheet.GetPageCount()==0)
	{
		m_sheet.AddPage(&m_dlgIVol);
		m_sheet.AddPage(&m_dlgOVol);
		
	}


	m_sheet.Create(this, WS_CHILD | WS_VISIBLE, WS_EX_CONTROLPARENT);
	RECT rect;
	m_sheet.GetWindowRect(&rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	
	m_sheet.SetWindowPos(NULL, 10, 20, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	if (m_idx==0)
	{
		m_sheet.SetActivePage(&m_dlgIVol);
	}else
		m_sheet.SetActivePage(&m_dlgOVol);
	

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

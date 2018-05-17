// AudioMixerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AudioMixer.h"
#include "AudioMixerDlg.h"
//#include"ConnectMgr.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ConfData.h"
CConfData extConfigData;
 
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAudioMixerDlg dialog

CAudioMixerDlg::CAudioMixerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAudioMixerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAudioMixerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAudioMixerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAudioMixerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAudioMixerDlg, CDialog)
	//{{AFX_MSG_MAP(CAudioMixerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INPUT_GAIN, OnInputGain)
	ON_BN_CLICKED(IDC_OUTPUT, OnOutputVolAdj)
	ON_BN_CLICKED(IDC_BUTTON3, OnButMixConf)
	ON_BN_CLICKED(IDC_BUTTON4, OnConfigMgr)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAudioMixerDlg message handlers

BOOL CAudioMixerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	extConfigData.InitConfData();

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAudioMixerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAudioMixerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAudioMixerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAudioMixerDlg::OnInputGain() 
{
	// TODO: Add your control notification handler code here
	m_VolBox.m_idx=0;
	//m_VolBox.DoModal();
	m_dlgVolCtl.m_idx=0;
	m_dlgVolCtl.DoModal();

}

void CAudioMixerDlg::OnOutputVolAdj() 
{
	// TODO: Add your control notification handler code here
	//	m_VolBox.m_VolCtl.m_nCurrentPage=1;
	//m_VolBox.m_idx=1;
	m_dlgVolCtl.m_idx=1;
	//m_VolBox.DoModal();	
	m_dlgVolCtl.DoModal();

	//m_VolBox.DoModal();

	
}

void CAudioMixerDlg::OnButMixConf() 
{
	// TODO: Add your control notification handler code here
	m_dlgMixConf.DoModal();
	
}

void CAudioMixerDlg::OnConfigMgr() 
{
	// TODO: Add your control notification handler code here
	//m_dlgConfMgr.DoModal();
	CPropertySheet sheet("≈‰÷√π‹¿Ì");
	CConectMgr dlgConectMgr;
	CSchemeMgr dlgSchMgr;
	sheet.AddPage(&dlgConectMgr);
	sheet.AddPage(&dlgSchMgr);
	sheet.DoModal();	
}

void CAudioMixerDlg::OnButtonApply() 
{
	// TODO: Add your control notification handler code here
	//MessageBox(_T("…Ë÷√±£¥Ê£¨«Î…‘∫Ú"),NULL,MB_OK);
	m_dlgWait.DoModal();
	
}

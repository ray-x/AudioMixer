// ConectMgr.cpp : implementation file
//

#include "stdafx.h"
#include "AudioMixer.h"
#include "ConectMgr.h"
#include "ConfData.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConectMgr property page

IMPLEMENT_DYNCREATE(CConectMgr, CPropertyPage)

CConectMgr::CConectMgr() : CPropertyPage(CConectMgr::IDD)
{
	//{{AFX_DATA_INIT(CConectMgr)
	m_UartOrUSB = 0;
	m_BaudrateIdx = -1;
	m_DataWIdx = -1;
	//}}AFX_DATA_INIT
}

CConectMgr::~CConectMgr()
{
}

void CConectMgr::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConectMgr)
	DDX_Control(pDX, IDC_COMBO_DATAWIDTH, m_DataWidth);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_cboBaudrate);
	DDX_Control(pDX, IDC_COMBO_PORT, m_cboPort);
	DDX_Radio(pDX, IDC_RADIO_SERIAL, m_UartOrUSB);
	DDX_CBIndex(pDX, IDC_COMBO_BAUDRATE, m_BaudrateIdx);
	DDX_CBIndex(pDX, IDC_COMBO_DATAWIDTH, m_DataWIdx);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConectMgr, CPropertyPage)
	//{{AFX_MSG_MAP(CConectMgr)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUDRATE, OnSelchangeComboBaudrate)
	ON_CBN_SELCHANGE(IDC_COMBO_DATAWIDTH, OnSelchangeComboDatawidth)
	ON_CBN_SELCHANGE(IDC_COMBO_PORT, OnSelchangeComboPort)
	ON_BN_CLICKED(IDC_RADIO_SERIAL, OnRadioSerial)
	ON_BN_CLICKED(IDC_RADIO_USB, OnRadioUsb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConectMgr message handlers

BOOL CConectMgr::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_cboBaudrate.SetCurSel(0);
	m_cboPort.SetCurSel(0);
	m_DataWidth.SetCurSel(1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConectMgr::OnSelchangeComboBaudrate() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int i=m_cboBaudrate.GetCurSel();
	switch (i)
	{
	/*9600
	56000
112000*/
	case 0:
		extConfigData.m_UartBaud=9600;
		break;
	case 1:
		extConfigData.m_UartBaud=56000;
		break;
	case 2:
		extConfigData.m_UartBaud=112000;
		break;
	default:
		extConfigData.m_UartBaud=9600;
		break;
		
	}
	
}

void CConectMgr::OnSelchangeComboDatawidth() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int i=m_DataWidth.GetCurSel();
	switch (i)
	{
	/*7 or 8*/
	case 0:
		extConfigData.m_UartDataW=7;
		break;
	case 1:
		extConfigData.m_UartDataW=8;
		break;
	default:
		extConfigData.m_UartDataW=8;	
	}
}

void CConectMgr::OnSelchangeComboPort() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int i=m_cboPort.GetCurSel();
	extConfigData.m_UartPortIdx=i+1;
}

void CConectMgr::OnRadioSerial() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	extConfigData.m_USBoUART=UART;

	
}

void CConectMgr::OnRadioUsb() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	extConfigData.m_USBoUART=USB;
	
}

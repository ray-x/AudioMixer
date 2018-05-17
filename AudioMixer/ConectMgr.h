#if !defined(AFX_CONECTMGR_H__9D09DAA2_2162_4AA3_8702_73617A6B5F2E__INCLUDED_)
#define AFX_CONECTMGR_H__9D09DAA2_2162_4AA3_8702_73617A6B5F2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConectMgr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConectMgr dialog

class CConectMgr : public CPropertyPage
{
	DECLARE_DYNCREATE(CConectMgr)

// Construction
public:
	CConectMgr();
	~CConectMgr();

// Dialog Data
	//{{AFX_DATA(CConectMgr)
	enum { IDD = IDD_DIALOG_SYSMGR };
	CComboBox	m_DataWidth;
	CComboBox	m_cboBaudrate;
	CComboBox	m_cboPort;
	int		m_UartOrUSB;
	int		m_BaudrateIdx;
	int		m_DataWIdx;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConectMgr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConectMgr)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboBaudrate();
	afx_msg void OnSelchangeComboDatawidth();
	afx_msg void OnSelchangeComboPort();
	afx_msg void OnRadioSerial();
	afx_msg void OnRadioUsb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONECTMGR_H__9D09DAA2_2162_4AA3_8702_73617A6B5F2E__INCLUDED_)

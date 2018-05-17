#if !defined(AFX_SCHEMEMGR_H__402C6985_0D2A_4296_AD98_25078F3716E2__INCLUDED_)
#define AFX_SCHEMEMGR_H__402C6985_0D2A_4296_AD98_25078F3716E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "inifile.h"
#include "tstdlibs.h"

// SchemeMgr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSchemeMgr dialog

class CSchemeMgr : public CPropertyPage
{
	DECLARE_DYNCREATE(CSchemeMgr)

// Construction
public:
	CSchemeMgr();
	~CSchemeMgr();
	CIniFileA	m_IniFile;
	CString m_strArrScheName[10];

// Dialog Data
	//{{AFX_DATA(CSchemeMgr)
	enum { IDD = IDD_DIALOG_SCHEME };
	CEdit	m_editSchemeNum;
	CEdit	m_editSchemeName;
	CListCtrl	m_lstScheme;
	CComboBox	m_cboSchemeSel;
	CString	m_strScheName;
	int		m_idxScheme;
	CString	m_SchemeName;
	CString	m_SchemeNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSchemeMgr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSchemeMgr)
	afx_msg void OnButtonExport();
	afx_msg void OnEditchangeComboScheme();
	afx_msg void OnDblclkComboScheme();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDeleteScheme();
	afx_msg void OnClickListScheme(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSave();
	afx_msg void OnButtonApply();
	afx_msg void OnButtonImport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCHEMEMGR_H__402C6985_0D2A_4296_AD98_25078F3716E2__INCLUDED_)

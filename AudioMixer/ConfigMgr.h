#if !defined(AFX_CONFIGMGR_H__C56715CC_FEAD_481F_AEF8_E04EE0B1C419__INCLUDED_)
#define AFX_CONFIGMGR_H__C56715CC_FEAD_481F_AEF8_E04EE0B1C419__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ConectMgr.h"
#include "SchemeMgr.h"
// ConfigMgr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigMgr dialog

class CConfigMgr : public CDialog
{
// Construction
public:
	CConfigMgr(CWnd* pParent = NULL);   // standard constructor
	CConectMgr m_dlgConectMgr;
	CSchemeMgr m_dlgScheMgr;
	CPropertySheet m_sheet;


// Dialog Data
	//{{AFX_DATA(CConfigMgr)
	enum { IDD = IDD_PROPPAGEL_CONFIG_MGR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigMgr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigMgr)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGMGR_H__C56715CC_FEAD_481F_AEF8_E04EE0B1C419__INCLUDED_)

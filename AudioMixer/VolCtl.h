#if !defined(AFX_VOLCTL_H__E0A42601_DB40_49D9_9A7C_06D8FF5C7985__INCLUDED_)
#define AFX_VOLCTL_H__E0A42601_DB40_49D9_9A7C_06D8FF5C7985__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VolCtl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVolCtl dialog
#include "InputVolAdjDlg.h"
#include "OutputVolAdjDlg.h"
class CVolCtl : public CDialog
{
// Construction
public:
	CVolCtl(CWnd* pParent = NULL);   // standard constructor
	CPropertySheet m_sheet;
	CInputVolAdjDlg m_dlgIVol;
	COutputVolAdjDlg m_dlgOVol;
	int m_idx;
// Dialog Data
	//{{AFX_DATA(CVolCtl)
	enum { IDD = IDD_PROPPAGEL_VOLCTL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVolCtl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVolCtl)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VOLCTL_H__E0A42601_DB40_49D9_9A7C_06D8FF5C7985__INCLUDED_)

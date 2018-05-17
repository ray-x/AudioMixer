#if !defined(AFX_MIXCONF_H__1BD45A05_5C94_41D5_A211_307F1C3CD40F__INCLUDED_)
#define AFX_MIXCONF_H__1BD45A05_5C94_41D5_A211_307F1C3CD40F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MixConf.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMixConf dialog
#include "VolTab.h"

#include "MixChConf.h"
#include "InputVolAdjDlg.h"
class CMixConf : public CDialog
{
// Construction
public:
	CMixConf(CWnd* pParent = NULL);   // standard constructor
//child tabs
	CMixChConf m_dlgMixCh[8];
	
// Dialog Data
	//{{AFX_DATA(CMixConf)
	enum { IDD = IDD_MIXCONF };
	CVolTab	m_mixTab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMixConf)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMixConf)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickTabMix(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMixOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIXCONF_H__1BD45A05_5C94_41D5_A211_307F1C3CD40F__INCLUDED_)

#if !defined(AFX_VOLADJDLGBOX_H__C9AC0BC1_3C13_4131_94BD_17B0B7208E9F__INCLUDED_)
#define AFX_VOLADJDLGBOX_H__C9AC0BC1_3C13_4131_94BD_17B0B7208E9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VolAdjDlgBox.h : header file
//
#include "VolTab.h"
#include "InputVolAdjDlg.h"
#include "OutputVolAdjDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CVolAdjDlgBox dialog

class CVolAdjDlgBox : public CDialog
{
// Construction
public:
	CVolAdjDlgBox(CWnd* pParent = NULL);   // standard constructor
	//CVolTab	m_VolTab;
	CInputVolAdjDlg	m_inputVol;
	COutputVolAdjDlg m_outputVol;
	int m_idx;

// Dialog Data
	//{{AFX_DATA(CVolAdjDlgBox)
	enum { IDD = IDD_VOL };
	CVolTab	m_VolCtl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVolAdjDlgBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVolAdjDlgBox)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VOLADJDLGBOX_H__C9AC0BC1_3C13_4131_94BD_17B0B7208E9F__INCLUDED_)

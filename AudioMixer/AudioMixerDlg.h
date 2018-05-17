// AudioMixerDlg.h : header file
//

#if !defined(AFX_AUDIOMIXERDLG_H__D2A17161_641A_4C64_B521_8A214EB48EB9__INCLUDED_)
#define AFX_AUDIOMIXERDLG_H__D2A17161_641A_4C64_B521_8A214EB48EB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "VolAdjDlgBox.h"
#include "MixConf.h"
#include "ConfigMgr.h"
#include "ConectMgr.h"
#include "Wait.h"
#include "ConfData.h"
#include "VolCtl.h"
/////////////////////////////////////////////////////////////////////////////
// CAudioMixerDlg dialog

class CAudioMixerDlg : public CDialog
{
// Construction
public:
	CAudioMixerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAudioMixerDlg)
	enum { IDD = IDD_AUDIOMIXER_DIALOG };
	//enum { IDD = IDD_AUDIOMIXER_MAIN_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAudioMixerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CVolAdjDlgBox	m_VolBox;
	CMixConf m_dlgMixConf;
	CConfigMgr m_dlgConfMgr;
	CWait m_dlgWait;
	CVolCtl m_dlgVolCtl;
	//CConectMgr m_dlg;
	// Generated message map functions
	//{{AFX_MSG(CAudioMixerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnInputGain();
	afx_msg void OnOutputVolAdj();
	afx_msg void OnButMixConf();
	afx_msg void OnConfigMgr();
	afx_msg void OnButtonApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUDIOMIXERDLG_H__D2A17161_641A_4C64_B521_8A214EB48EB9__INCLUDED_)

#if !defined(AFX_MIXCHCONF_H__6AAD098A_E912_4DE8_907C_B3D5F3AB2E5C__INCLUDED_)
#define AFX_MIXCHCONF_H__6AAD098A_E912_4DE8_907C_B3D5F3AB2E5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MixChConf.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMixChConf dialog

class CMixChConf : public CDialog
{
// Construction
public:
	void ApplyDataChange();
	CMixChConf(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMixChConf)
	enum { IDD = IDD_PROPPAGEL_MIXCTL };
	CButton	m_RadioBoxCh;
	CSliderCtrl	m_ctlSliderInput8;
	CSliderCtrl	m_ctlSliderInput7;
	CSliderCtrl	m_ctlSliderInput6;
	CSliderCtrl	m_ctlSliderInput5;
	CSliderCtrl	m_ctlSliderInput4;
	CSliderCtrl	m_ctlSliderInput3;
	CSliderCtrl	m_ctlSliderInput2;
	CSliderCtrl	m_ctlSliderInput1;
	CString	m_strChannelNumber;
	BOOL	m_bChannel1;
	BOOL	m_bChannel2;
	BOOL	m_bChannel3;
	BOOL	m_bChannel4;
	BOOL	m_bChannel5;
	BOOL	m_bChannel6;
	BOOL	m_bChannel7;
	BOOL	m_bChannel8;
	int		m_VolInputVol1;
	int		m_VolInputVol2;
	int		m_VolInputVol3;
	int		m_VolInputVol4;
	int		m_VolInputVol5;
	int		m_VolInputVol6;
	int		m_VolInputVol7;
	int		m_VolInputVol8;
	int		m_TotalChNum;
	//}}AFX_DATA

	int		m_numCh;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMixChConf)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMixChConf)
	afx_msg void OnRadio4Ch();
	afx_msg void OnRadio8Ch();
	afx_msg void OnButtonApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIXCHCONF_H__6AAD098A_E912_4DE8_907C_B3D5F3AB2E5C__INCLUDED_)

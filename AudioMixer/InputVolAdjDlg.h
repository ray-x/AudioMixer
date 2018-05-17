#if !defined(AFX_INPUTVOLADJDLG_H__F8E337A0_CD2C_4748_B978_7DBDBCE8A378__INCLUDED_)
#define AFX_INPUTVOLADJDLG_H__F8E337A0_CD2C_4748_B978_7DBDBCE8A378__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputVolAdjDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputVolAdjDlg dialog

class CInputVolAdjDlg : public CPropertyPage
{
// Construction
public:
	CInputVolAdjDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputVolAdjDlg)
	enum { IDD = IDD_PROPPAGEL_INPUT_VOLCTL };
	CSliderCtrl	m_SliderInput8;
	CSliderCtrl	m_SliderInput7;
	CSliderCtrl	m_SliderInput6;
	CSliderCtrl	m_SliderInput5;
	CSliderCtrl	m_SliderInput4;
	CSliderCtrl	m_SliderInput3;
	CSliderCtrl	m_SliderInput2;
	CSliderCtrl	m_SliderInput1;
	int		m_inputVol1;
	int		m_inputVol2;
	int		m_inputVol3;
	int		m_inputVol4;
	int		m_inputVol5;
	int		m_inputVol6;
	int		m_inputVol7;
	int		m_inputVol8;
	int		m_numTotalCh;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputVolAdjDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputVolAdjDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonApplyInputVolChange();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTVOLADJDLG_H__F8E337A0_CD2C_4748_B978_7DBDBCE8A378__INCLUDED_)

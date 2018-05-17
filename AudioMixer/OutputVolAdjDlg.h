#if !defined(AFX_OUTPUTVOLADJDLG_H__8C1076CF_0D4D_47E8_8787_76F405FE9913__INCLUDED_)
#define AFX_OUTPUTVOLADJDLG_H__8C1076CF_0D4D_47E8_8787_76F405FE9913__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutputVolAdjDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COutputVolAdjDlg dialog

class COutputVolAdjDlg : public CPropertyPage
{
// Construction
public:
	COutputVolAdjDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COutputVolAdjDlg)
	enum { IDD = IDD_PROPPAGEL_OUTPUT_VOLCTL };
	CSliderCtrl	m_SliderOutputVol8;
	CSliderCtrl	m_SliderOutputVol7;
	CSliderCtrl	m_SliderOutputVol6;
	CSliderCtrl	m_SliderOutputVol5;
	CSliderCtrl	m_SliderOutputVol4;
	CSliderCtrl	m_SliderOutputVol3;
	CSliderCtrl	m_SliderOutputVol2;
	CSliderCtrl	m_SliderOutputVol1;
	int		m_OutputVol1;
	int		m_OutputVol2;
	int		m_OutputVol3;
	int		m_OutputVol4;
	int		m_OutputVol5;
	int		m_OutputVol6;
	int		m_OutputVol7;
	int		m_OutputVol8;
	int		m_numTotalCh; 
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputVolAdjDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COutputVolAdjDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonApplyOutVol();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTVOLADJDLG_H__8C1076CF_0D4D_47E8_8787_76F405FE9913__INCLUDED_)

#if !defined(AFX_VOLTAB_H__59EF8D8E_B494_414B_9A70_80FC0B19B885__INCLUDED_)
#define AFX_VOLTAB_H__59EF8D8E_B494_414B_9A70_80FC0B19B885__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VolTab.h : header file
//
#define MAXPAGE 16
/////////////////////////////////////////////////////////////////////////////
// CVolTab window

class CVolTab : public CTabCtrl
{
// Construction
public:
	CVolTab();

// Attributes
public:
	LPCTSTR m_Title[MAXPAGE];
	UINT m_IDD[MAXPAGE];
	CDialog* m_pPages[MAXPAGE];
	int m_nNumOfPages;
	int m_nCurrentPage;

// Operations
public:
	//virtual ~CTabSheet();
	
	int GetCurSel();
	int SetCurSel(int nItem);
	void Show();
	void SetRect();
	BOOL AddPage(LPCTSTR title, CDialog *pDialog, UINT ID);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVolTab)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVolTab();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CVolTab)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VOLTAB_H__59EF8D8E_B494_414B_9A70_80FC0B19B885__INCLUDED_)

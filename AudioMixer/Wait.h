#if !defined(AFX_WAIT_H__1E1021C5_0422_4816_A37C_7A67776A52C4__INCLUDED_)
#define AFX_WAIT_H__1E1021C5_0422_4816_A37C_7A67776A52C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Wait.h : header file
//
#define TIMER_ID	222
/////////////////////////////////////////////////////////////////////////////
// CWait dialog
#include "ConfData.h"
#include <wtypes.h>
#include "serialport.h"
class CWait : public CDialog
{
// Construction
public:
	void Init();
	void UpdateMixConf(int ch);
	void UpdateOutputVol(int ch, int vol);
	void UpdateInputVol(int ch, int vol);
	CWait(int type,CWnd* pParent = NULL);
	void InitConfBuf();
	void SendData();
	CWait(CWnd* pParent = NULL);   // standard constructor
	int m_update_type;
	BYTE *m_pIVol, *m_pOVol, *m_pMix;
	
	CSerialPort port;
// Dialog Data
	//{{AFX_DATA(CWait)
	enum { IDD = IDD_DIALOG_WAIT };
	CString	m_strStatusMsg;
	
	//}}AFX_DATA
	BYTE m_arrData[256]; 
//	COMMCONFIG m_serConf;
    CSerialPort m_Serport;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWait)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWait)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAIT_H__1E1021C5_0422_4816_A37C_7A67776A52C4__INCLUDED_)

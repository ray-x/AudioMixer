#if !defined(AFX_MIXERCONF_H__FE41D5E1_D034_4AEC_A813_A0287BD08A53__INCLUDED_)
#define AFX_MIXERCONF_H__FE41D5E1_D034_4AEC_A813_A0287BD08A53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ConfData.h"

// MixerConf.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMixerConf dialog

class CMixerConf : public CDialog
{
// Construction
public:
	CMixerConf(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMixerConf)
	enum { IDD = IDD_MIXCONF };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMixerConf)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMixerConf)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



#endif // !defined(AFX_MIXERCONF_H__FE41D5E1_D034_4AEC_A813_A0287BD08A53__INCLUDED_)

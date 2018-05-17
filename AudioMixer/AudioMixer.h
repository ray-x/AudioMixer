// AudioMixer.h : main header file for the AUDIOMIXER application
//

#if !defined(AFX_AUDIOMIXER_H__D6C134E3_20C9_424E_9D4E_99FCED711ADC__INCLUDED_)
#define AFX_AUDIOMIXER_H__D6C134E3_20C9_424E_9D4E_99FCED711ADC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAudioMixerApp:
// See AudioMixer.cpp for the implementation of this class
//

class CAudioMixerApp : public CWinApp
{
public:
	CAudioMixerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAudioMixerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAudioMixerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUDIOMIXER_H__D6C134E3_20C9_424E_9D4E_99FCED711ADC__INCLUDED_)

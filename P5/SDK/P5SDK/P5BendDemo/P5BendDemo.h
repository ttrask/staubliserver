// P5BendDemo.h : main header file for the P5BENDDEMO application
//

#if !defined(AFX_P5BENDDEMO_H__00F2C189_71D4_4116_A48B_646FF6EDB956__INCLUDED_)
#define AFX_P5BENDDEMO_H__00F2C189_71D4_4116_A48B_646FF6EDB956__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CP5BendDemoApp:
// See P5BendDemo.cpp for the implementation of this class
//

class CP5BendDemoApp : public CWinApp
{
public:
	CP5BendDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CP5BendDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CP5BendDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_P5BENDDEMO_H__00F2C189_71D4_4116_A48B_646FF6EDB956__INCLUDED_)

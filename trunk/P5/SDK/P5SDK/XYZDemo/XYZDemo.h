// XYZDemo.h : main header file for the XYZDEMO application
//

#if !defined(AFX_XYZDEMO_H__1F7C3872_3EE4_411F_8C48_7B1E1EF2FB2C__INCLUDED_)
#define AFX_XYZDEMO_H__1F7C3872_3EE4_411F_8C48_7B1E1EF2FB2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXYZDemoApp:
// See XYZDemo.cpp for the implementation of this class
//

class CXYZDemoApp : public CWinApp
{
public:
	CXYZDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXYZDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXYZDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XYZDEMO_H__1F7C3872_3EE4_411F_8C48_7B1E1EF2FB2C__INCLUDED_)

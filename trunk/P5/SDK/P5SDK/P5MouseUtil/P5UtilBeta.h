// P5UtilBeta.h : main header file for the P5UTILBETA application
//

#if !defined(AFX_P5UTILBETA_H__5FB79AE9_2CBD_4EC9_BF81_7510D9506B2D__INCLUDED_)
#define AFX_P5UTILBETA_H__5FB79AE9_2CBD_4EC9_BF81_7510D9506B2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CP5UtilBetaApp:
// See P5UtilBeta.cpp for the implementation of this class
//

class CP5UtilBetaApp : public CWinApp
{
public:
	CP5UtilBetaApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CP5UtilBetaApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CP5UtilBetaApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_P5UTILBETA_H__5FB79AE9_2CBD_4EC9_BF81_7510D9506B2D__INCLUDED_)

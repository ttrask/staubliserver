// P5UtilBetaDlg.h : header file
//

#if !defined(AFX_P5UTILBETADLG_H__0FEEAA11_89CD_43CA_B9BB_CC75C3D91E31__INCLUDED_)
#define AFX_P5UTILBETADLG_H__0FEEAA11_89CD_43CA_B9BB_CC75C3D91E31__INCLUDED_

#define FULL

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "resource.h"
#include "BendsPage.h"
#include "MousePage.h"
#include "MouseExPage.h"

#ifdef FULL
	#include "XyzPage.h"
	#include "ButtonsPage.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CP5UtilBetaDlg dialog

class CP5UtilBetaDlg : public CPropertySheet
{
// Construction
public:
//	CP5UtilBetaDlg(CWnd* pParent = NULL);	// standard constructor

	BendsPage   m_bendsPage;
	MousePage   m_mousePage;
	MouseExPage m_mouseExPage;
#ifdef FULL
	XyzPage		m_xyzPage;
	ButtonsPage	m_butPage;
#endif

	CP5UtilBetaDlg() : CPropertySheet( "P5 Utility" )
	{
		m_bendsPage.Construct( IDD_DLG_BENDS );
		m_mousePage.Construct( IDD_DLG_MOUSE );
		m_mouseExPage.Construct( IDD_DLG_MOUSE_EX );
#ifdef FULL
		m_xyzPage.Construct( IDD_DLG_XYZ );
		m_butPage.Construct( IDD_DLG_BUTTONS );
#endif
		AddPage( &m_bendsPage );
		AddPage( &m_mouseExPage );
		AddPage( &m_mousePage );
#ifdef FULL
		AddPage( &m_xyzPage );
		AddPage( &m_butPage );
#endif
	}

	~CP5UtilBetaDlg(){}

	void DisplayPage(int page);

	//{{AFX_DATA(CP5UtilBetaDlg)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CP5UtilBetaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CP5UtilBetaDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_P5UTILBETADLG_H__0FEEAA11_89CD_43CA_B9BB_CC75C3D91E31__INCLUDED_)

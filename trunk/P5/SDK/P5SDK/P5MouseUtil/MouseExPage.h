#if !defined(AFX_MOUSEEXPAGE_H__D174F7B3_912C_495B_BE2D_D1D1789E2A08__INCLUDED_)
#define AFX_MOUSEEXPAGE_H__D174F7B3_912C_495B_BE2D_D1D1789E2A08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MouseExPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MouseExPage dialog

class MouseExPage : public CPropertyPage
{
	DECLARE_DYNCREATE(MouseExPage)

// Construction
public:
	MouseExPage();
	~MouseExPage();

// Dialog Data
	//{{AFX_DATA(MouseExPage)
	enum { IDD = IDD_DLG_MOUSE_EX };
	CStatic	m_txtMouseMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(MouseExPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(MouseExPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRadioMouseOn();
	afx_msg void OnRadioMouseOff();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOUSEEXPAGE_H__D174F7B3_912C_495B_BE2D_D1D1789E2A08__INCLUDED_)

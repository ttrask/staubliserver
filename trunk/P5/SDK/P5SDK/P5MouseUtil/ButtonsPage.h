#if !defined(AFX_BUTTONSPAGE_H__1CD4391F_7745_40F8_8CCA_D803A5EC7663__INCLUDED_)
#define AFX_BUTTONSPAGE_H__1CD4391F_7745_40F8_8CCA_D803A5EC7663__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ButtonsPage.h : header file
//

#include "LedButton.h"

/////////////////////////////////////////////////////////////////////////////
// ButtonsPage dialog

class ButtonsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(ButtonsPage)

// Construction
public:
	ButtonsPage();
	~ButtonsPage();

// Dialog Data
	//{{AFX_DATA(ButtonsPage)
	enum { IDD = IDD_DLG_BUTTONS };
	CLedButton	m_butd;
	CLedButton	m_butc;
	CLedButton	m_butb;
	CLedButton	m_buta;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(ButtonsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(ButtonsPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUTTONSPAGE_H__1CD4391F_7745_40F8_8CCA_D803A5EC7663__INCLUDED_)

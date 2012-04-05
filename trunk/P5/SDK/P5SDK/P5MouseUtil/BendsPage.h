#if !defined(AFX_BENDSPAGE_H__3249DBB6_285E_4971_8D23_DDB07AB87C28__INCLUDED_)
#define AFX_BENDSPAGE_H__3249DBB6_285E_4971_8D23_DDB07AB87C28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BendsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// BendsPage dialog

class BendsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(BendsPage)

// Construction
public:
	BendsPage();
	~BendsPage();

// Dialog Data
	//{{AFX_DATA(BendsPage)
	enum { IDD = IDD_DLG_BENDS };
	CStatic	m_txtBendsMsg;
	CProgressCtrl	m_progThumb;
	CProgressCtrl	m_progRing;
	CProgressCtrl	m_progPinky;
	CProgressCtrl	m_progMiddle;
	CProgressCtrl	m_progIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(BendsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(BendsPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BENDSPAGE_H__3249DBB6_285E_4971_8D23_DDB07AB87C28__INCLUDED_)

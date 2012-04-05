#if !defined(AFX_XYZPAGE_H__93DAD03F_A42E_4751_9CEB_6D9AE591E974__INCLUDED_)
#define AFX_XYZPAGE_H__93DAD03F_A42E_4751_9CEB_6D9AE591E974__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XyzPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// XyzPage dialog

class XyzPage : public CPropertyPage
{
	DECLARE_DYNCREATE(XyzPage)

// Construction
public:
	XyzPage();
	~XyzPage();

// Dialog Data
	//{{AFX_DATA(XyzPage)
	enum { IDD = IDD_DLG_XYZ };
	CStatic	m_txtz;
	CStatic	m_txty;
	CStatic	m_txtx;
	CSliderCtrl	m_sldz;
	CSliderCtrl	m_sldy;
	CSliderCtrl	m_sldx;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(XyzPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(XyzPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnResetPosData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XYZPAGE_H__93DAD03F_A42E_4751_9CEB_6D9AE591E974__INCLUDED_)

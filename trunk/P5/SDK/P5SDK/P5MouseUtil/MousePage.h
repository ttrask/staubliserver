#if !defined(AFX_MOUSEPAGE_H__184F19DD_83AA_4B95_8AEF_86E4126D3470__INCLUDED_)
#define AFX_MOUSEPAGE_H__184F19DD_83AA_4B95_8AEF_86E4126D3470__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MousePage.h : header file
//

class MousePage : public CPropertyPage
{
	DECLARE_DYNCREATE(MousePage)

// Construction
public:
	MousePage();
	~MousePage();

	int nMouseStickTime;
	int nLeftClickFinger;
	int nRightClickFinger;
	int nMiddleClickFinger;


// Dialog Data
	//{{AFX_DATA(MousePage)
	enum { IDD = IDD_DLG_MOUSE };
	CStatic	m_clickTest;
	CStatic	m_txtStickyTime;
	CSliderCtrl	m_sldSticky;
	CComboBox	m_cmbRightClick;
	CComboBox	m_cmbMiddleClick;
	CComboBox	m_cmbLeftClick;
	CSliderCtrl	m_sldMiddleClick;
	CBitmap m_bmp1;
	CBitmap m_bmp2;
	CSliderCtrl	m_sldRightClick;
	CSliderCtrl	m_sldLetfClick;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(MousePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(MousePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButSaveMap();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOUSEPAGE_H__184F19DD_83AA_4B95_8AEF_86E4126D3470__INCLUDED_)

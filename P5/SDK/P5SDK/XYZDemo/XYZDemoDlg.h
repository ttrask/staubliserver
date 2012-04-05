// XYZDemoDlg.h : header file
//

#if !defined(AFX_XYZDEMODLG_H__83158516_C5A8_4DE0_AE2B_88AAD3AEF33D__INCLUDED_)
#define AFX_XYZDEMODLG_H__83158516_C5A8_4DE0_AE2B_88AAD3AEF33D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CXYZDemoDlg dialog

class CXYZDemoDlg : public CDialog
{
// Construction
public:
	CXYZDemoDlg(CWnd* pParent = NULL);	// standard constructor

	BOOL m_XYPlane;
	BOOL m_XZPlane;

// Dialog Data
	//{{AFX_DATA(CXYZDemoDlg)
	enum { IDD = IDD_XYZDEMO_DIALOG };
	CStatic	m_txtPlane;
	CStatic	m_txtMsg;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXYZDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CXYZDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XYZDEMODLG_H__83158516_C5A8_4DE0_AE2B_88AAD3AEF33D__INCLUDED_)

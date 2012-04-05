// P5BendDemoDlg.h : header file
//

#if !defined(AFX_P5BENDDEMODLG_H__6473DE62_F443_49B7_9FDB_487BC8C435CC__INCLUDED_)
#define AFX_P5BENDDEMODLG_H__6473DE62_F443_49B7_9FDB_487BC8C435CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CP5BendDemoDlg dialog

class CP5BendDemoDlg : public CDialog
{
// Construction
public:
	CP5BendDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CP5BendDemoDlg)
	enum { IDD = IDD_P5BENDDEMO_DIALOG };
	CStatic	m_ThumbEdge;
	CStatic	m_RingEdge;
	CStatic	m_PinkyEdge;
	CStatic	m_MiddleEdge;
	CStatic	m_IndexEdge;
	CSliderCtrl	m_ThumbSlider;
	CSliderCtrl	m_RingSlider;
	CSliderCtrl	m_PinkySlider;
	CSliderCtrl	m_MiddleSlider;
	CSliderCtrl	m_IndexSlider;
	CButton	m_ThumbState;
	CButton	m_RingState;
	CButton	m_PinkyState;
	CButton	m_MiddleState;
	CButton	m_IndexState;
	CProgressCtrl	m_Thumb;
	CProgressCtrl	m_Ring;
	CProgressCtrl	m_Pinky;
	CProgressCtrl	m_Middle;
	CProgressCtrl	m_Index;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CP5BendDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CP5BendDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_P5BENDDEMODLG_H__6473DE62_F443_49B7_9FDB_487BC8C435CC__INCLUDED_)

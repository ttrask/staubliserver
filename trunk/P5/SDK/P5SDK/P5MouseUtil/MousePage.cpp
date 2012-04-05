	// MousePage.cpp : implementation file
//

#include "stdafx.h"
#include "P5UtilBeta.h"
#include "MousePage.h"
#include "p5dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CP5DLL P5;
extern BOOL bP5Present;
BOOL bLSngClk = FALSE;
BOOL bRSngClk = FALSE;
BOOL bMSngClk = FALSE;

BOOL bLTblclk = FALSE;
BOOL bMTblclk = FALSE;
BOOL bRTblclk = FALSE;
int nLeftClickSliderValue, nRightClickSliderValue, nMiddleClickSliderValue;


/////////////////////////////////////////////////////////////////////////////
// MousePage property page

IMPLEMENT_DYNCREATE(MousePage, CPropertyPage)

MousePage::MousePage() : CPropertyPage(MousePage::IDD)
{
	//{{AFX_DATA_INIT(MousePage)
	//}}AFX_DATA_INIT
}

MousePage::~MousePage()
{
}

void MousePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MousePage)
	DDX_Control(pDX, IDC_CLICK, m_clickTest);
	DDX_Control(pDX, IDC_TXT_STICKY_TIME, m_txtStickyTime);
	DDX_Control(pDX, IDC_SLD_STICKY, m_sldSticky);
	DDX_Control(pDX, IDC_COMBO_RCLICK, m_cmbRightClick);
	DDX_Control(pDX, IDC_COMBO_MCLICK, m_cmbMiddleClick);
	DDX_Control(pDX, IDC_COMBO_LCLICK, m_cmbLeftClick);
	DDX_Control(pDX, IDC_SLD_M_SENS, m_sldMiddleClick);
	DDX_Control(pDX, IDC_SLD_R_SENS, m_sldRightClick);
	DDX_Control(pDX, IDC_SLD_L_SENS, m_sldLetfClick);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MousePage, CPropertyPage)
	//{{AFX_MSG_MAP(MousePage)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUT_SAVE_MAP, OnButSaveMap)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MousePage message handlers

BOOL MousePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString m_txt;

	GetDlgItem(IDC_SLD_L_SENS)->EnableWindow(bP5Present);
	GetDlgItem(IDC_SLD_R_SENS)->EnableWindow(bP5Present);
	GetDlgItem(IDC_SLD_M_SENS)->EnableWindow(bP5Present);
	GetDlgItem(IDC_SLD_STICKY)->EnableWindow(bP5Present);
	GetDlgItem(IDC_BUT_SAVE_MAP)->EnableWindow(bP5Present);
	GetDlgItem(IDC_COMBO_LCLICK)->EnableWindow(bP5Present);
	GetDlgItem(IDC_COMBO_MCLICK)->EnableWindow(bP5Present);
	GetDlgItem(IDC_COMBO_RCLICK)->EnableWindow(bP5Present);

	SetTimer(1002, 10, NULL);

	m_sldSticky.SetRange(0, 20);
	nMouseStickTime = P5.P5_GetMouseStickTime(0);
	m_sldSticky.SetPos(nMouseStickTime);

	m_cmbLeftClick.AddString("Thumb");
	m_cmbLeftClick.AddString("Index");
	m_cmbLeftClick.AddString("Middle");

	m_cmbRightClick.AddString("Thumb");
	m_cmbRightClick.AddString("Index");
	m_cmbRightClick.AddString("Middle");
	
	m_cmbMiddleClick.AddString("Thumb");
	m_cmbMiddleClick.AddString("Index");
	m_cmbMiddleClick.AddString("Middle");

	P5.P5_GetClickSensitivity(0, &nLeftClickSliderValue, &nRightClickSliderValue, &nMiddleClickSliderValue);

	m_sldLetfClick.SetRange(1, 80);
	m_sldLetfClick.SetPos(nLeftClickSliderValue);
	m_txt.Format("%d", nLeftClickSliderValue);
	GetDlgItem(IDC_TXT_LAREA)->SetWindowText(m_txt);

	m_sldMiddleClick.SetRange(1, 80);
	m_sldMiddleClick.SetPos(nMiddleClickSliderValue);
	m_txt.Format("%d", nMiddleClickSliderValue);
	GetDlgItem(IDC_TXT_MAREA)->SetWindowText(m_txt);

	m_sldRightClick.SetRange(1, 80);
	m_sldRightClick.SetPos(nRightClickSliderValue);
	m_txt.Format("%d", nRightClickSliderValue);
	GetDlgItem(IDC_TXT_RAREA)->SetWindowText(m_txt);

	if(bP5Present)
	{
		P5.P5_GetMouseButtonAllocation(0, &nLeftClickFinger, &nRightClickFinger, &nMiddleClickFinger);
		m_cmbLeftClick.SetCurSel(nLeftClickFinger);
		m_cmbRightClick.SetCurSel(nRightClickFinger);
		m_cmbMiddleClick.SetCurSel(nMiddleClickFinger);

		nMouseStickTime = P5.P5_GetMouseStickTime(0);
		m_sldSticky.SetPos(nMouseStickTime);
		CString s;
		s.Format( "%d", nMouseStickTime );
		m_txtStickyTime.SetWindowText( s );

	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void MousePage::OnTimer(UINT nIDEvent) 
{
	if (P5.m_P5Devices!=NULL)
	{
		CString txt;

		txt.Format( "%d", m_sldLetfClick.GetPos());
		GetDlgItem(IDC_TXT_LAREA)->SetWindowText(txt);

		txt.Format( "%d", m_sldMiddleClick.GetPos());
		GetDlgItem(IDC_TXT_MAREA)->SetWindowText(txt);

		txt.Format( "%d", m_sldRightClick.GetPos());
		GetDlgItem(IDC_TXT_RAREA)->SetWindowText(txt);

		txt.Format( "%d", nMouseStickTime );
		m_txtStickyTime.SetWindowText( txt );
	}
	
	CPropertyPage::OnTimer(nIDEvent);
}



void MousePage::OnButSaveMap() 
{
	if (P5.m_P5Devices!=NULL)
	{
		if ((m_cmbLeftClick.GetCurSel()   != m_cmbRightClick.GetCurSel()) && 
			(m_cmbLeftClick.GetCurSel()   != m_cmbMiddleClick.GetCurSel())&&
			(m_cmbMiddleClick.GetCurSel() != m_cmbRightClick.GetCurSel()) )
		{
			nLeftClickFinger = m_cmbLeftClick.GetCurSel();
			nRightClickFinger = m_cmbRightClick.GetCurSel();
			nMiddleClickFinger = m_cmbMiddleClick.GetCurSel();
			P5.P5_SetMouseButtonAllocation(0, nLeftClickFinger, nRightClickFinger, nMiddleClickFinger);

			P5.P5_GetMouseButtonAllocation(0, &nLeftClickFinger, &nRightClickFinger, &nMiddleClickFinger);
			m_cmbLeftClick.SetCurSel(nLeftClickFinger);
			m_cmbRightClick.SetCurSel(nRightClickFinger);
			m_cmbMiddleClick.SetCurSel(nMiddleClickFinger);
		}
		else
		{
			MessageBox("Duplicate Finger Mappings!");
		}

		//check to see if the click slider has been moved
		if ((nLeftClickSliderValue != m_sldLetfClick.GetPos())   || 
			(nRightClickSliderValue != m_sldRightClick.GetPos()) ||
			(nMiddleClickSliderValue != m_sldMiddleClick.GetPos() ))
		{
			CString txt;
			P5.P5_SetClickSensitivity(0, m_sldLetfClick.GetPos(), m_sldRightClick.GetPos(), m_sldMiddleClick.GetPos() );
			P5.P5_GetClickSensitivity(0, &nLeftClickSliderValue, &nRightClickSliderValue, &nMiddleClickSliderValue);

			m_sldLetfClick.SetPos(nLeftClickSliderValue);
			txt.Format("%d", nLeftClickSliderValue);
			GetDlgItem(IDC_TXT_LAREA)->SetWindowText(txt);

			m_sldMiddleClick.SetPos(nMiddleClickSliderValue);
			txt.Format("%d", nMiddleClickSliderValue);
			GetDlgItem(IDC_TXT_MAREA)->SetWindowText(txt);

			m_sldRightClick.SetPos(nRightClickSliderValue);
			txt.Format("%d", nRightClickSliderValue);
			GetDlgItem(IDC_TXT_RAREA)->SetWindowText(txt);
		}

		//check to see if the mouse stick time has been moved
		if ( m_sldSticky.GetPos() != nMouseStickTime )
		{
			P5.P5_SetMouseStickTime(0, m_sldSticky.GetPos());

			nMouseStickTime = P5.P5_GetMouseStickTime(0);
			m_sldSticky.SetPos(nMouseStickTime);
			CString s;
			s.Format( "%d", nMouseStickTime );
			m_txtStickyTime.SetWindowText( s );
		}

	}
}


void MousePage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (P5.m_P5Devices!=NULL)
	{
		bLSngClk = !bLSngClk;

		if(bLSngClk)
			m_clickTest.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SCLICK_LEFT2)));
		else
			m_clickTest.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SCLICK_LEFT1)));	

	}

	CPropertyPage::OnLButtonDown(nFlags, point);

}


void MousePage::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if (P5.m_P5Devices!=NULL)
	{
		bRSngClk = !bRSngClk;

		if(bRSngClk)
			m_clickTest.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SCLICK_RIGHT2)));
		else
			m_clickTest.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SCLICK_RIGHT1)));	
	}
	
	CPropertyPage::OnRButtonDown(nFlags, point);
}

void MousePage::OnMButtonDown(UINT nFlags, CPoint point) 
{
	if (P5.m_P5Devices!=NULL)
	{
		bMSngClk = !bMSngClk;

		if(bMSngClk)
			m_clickTest.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SCLICK_MID2)));
		else
			m_clickTest.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SCLICK_MID1)));	
	}
	
	CPropertyPage::OnRButtonDown(nFlags, point);
}

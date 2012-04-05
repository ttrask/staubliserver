// ButtonsPage.cpp : implementation file
//

#include "stdafx.h"
#include "P5UtilBeta.h"
#include "ButtonsPage.h"
#include "p5dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CP5DLL P5;
extern BOOL bP5Present;

/////////////////////////////////////////////////////////////////////////////
// ButtonsPage property page

IMPLEMENT_DYNCREATE(ButtonsPage, CPropertyPage)

ButtonsPage::ButtonsPage() : CPropertyPage(ButtonsPage::IDD)
{
	//{{AFX_DATA_INIT(ButtonsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

ButtonsPage::~ButtonsPage()
{
}

void ButtonsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ButtonsPage)
	DDX_Control(pDX, IDC_BUT_D, m_butd);
	DDX_Control(pDX, IDC_BUT_C, m_butc);
	DDX_Control(pDX, IDC_BUT_B, m_butb);
	DDX_Control(pDX, IDC_BUT_A, m_buta);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ButtonsPage, CPropertyPage)
	//{{AFX_MSG_MAP(ButtonsPage)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ButtonsPage message handlers

BOOL ButtonsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	SetTimer(1006, 10, NULL);

	m_buta.SetImage( IDB_LED_RED, 15 );
	m_buta.Depress(false);

	m_butb.SetImage( IDB_LED_RED, 15 );
	m_butb.Depress(false);

	m_butc.SetImage( IDB_LED_RED, 15 );
	m_butc.Depress(false);

	m_butd.SetImage( IDB_LED_RED, 15 );
	m_butd.Depress(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void ButtonsPage::OnTimer(UINT nIDEvent) 
{
	if(bP5Present)
	{
		m_buta.Depress(P5.m_P5Devices[0].m_byButtons[0]);
		m_butb.Depress(P5.m_P5Devices[0].m_byButtons[1]);
		m_butc.Depress(P5.m_P5Devices[0].m_byButtons[2]);
		m_butd.Depress(P5.m_P5Devices[0].m_byButtons[3]);
	}	
	
	CPropertyPage::OnTimer(nIDEvent);
}

// MouseExPage.cpp : implementation file
//

#include "stdafx.h"
#include "P5UtilBeta.h"
#include "MouseExPage.h"
#include "p5dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CP5DLL P5;
extern BOOL bP5Present;

int nResetCtr2 = 0;
BOOL bBut2Pressed = FALSE;

/////////////////////////////////////////////////////////////////////////////
// MouseExPage property page

IMPLEMENT_DYNCREATE(MouseExPage, CPropertyPage)

MouseExPage::MouseExPage() : CPropertyPage(MouseExPage::IDD)
{
	//{{AFX_DATA_INIT(MouseExPage)
	//}}AFX_DATA_INIT
}

MouseExPage::~MouseExPage()
{
}

void MouseExPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MouseExPage)
	DDX_Control(pDX, IDC_TXT_MOUSE_MODE, m_txtMouseMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MouseExPage, CPropertyPage)
	//{{AFX_MSG_MAP(MouseExPage)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO_MOUSE_ON, OnRadioMouseOn)
	ON_BN_CLICKED(IDC_RADIO_MOUSE_OFF, OnRadioMouseOff)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MouseExPage message handlers

BOOL MouseExPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	GetDlgItem(IDC_RADIO_MOUSE_ON)->EnableWindow(bP5Present);
	GetDlgItem(IDC_RADIO_MOUSE_OFF)->EnableWindow(bP5Present);
	SetTimer(1003, 10, NULL);
	
	CButton *mb;

	if(P5.P5_GetMouseState(0))
		mb = (CButton *)(GetDlgItem(IDC_RADIO_MOUSE_ON));
	else
		mb = (CButton *)(GetDlgItem(IDC_RADIO_MOUSE_OFF));

	mb->SetCheck(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void MouseExPage::OnTimer(UINT nIDEvent) 
{
	if( P5.m_P5Devices != NULL )
	{
		if(this->IsWindowVisible() )
		{
			if(P5.m_P5Devices[0].m_byButtons[1] && !bBut2Pressed)
			{
				CButton *mb;

				if(nResetCtr2%2)
				{
					if(P5.m_P5Devices != NULL)
					{
						P5.P5_SetMouseState( 0, TRUE );
						m_txtMouseMode.SetWindowText("Mouse mode is ON");
						mb = (CButton *)(GetDlgItem(IDC_RADIO_MOUSE_ON));
						mb->SetCheck( TRUE );
						mb = (CButton *)(GetDlgItem(IDC_RADIO_MOUSE_OFF));
						mb->SetCheck( FALSE );
					}
				}
				else
				{
					if(P5.m_P5Devices != NULL) 
					{
						P5.P5_SetMouseState( 0, FALSE );
						m_txtMouseMode.SetWindowText("Mouse mode is OFF");
						mb = (CButton *)(GetDlgItem(IDC_RADIO_MOUSE_OFF));
						mb->SetCheck( TRUE );
						mb = (CButton *)(GetDlgItem(IDC_RADIO_MOUSE_ON));
						mb->SetCheck( FALSE );
					}
				}

				nResetCtr2 ++;
				bBut2Pressed = TRUE;
			}

			if(!P5.m_P5Devices[0].m_byButtons[1])
			{
				bBut2Pressed = FALSE;
			}
		}
	}

	CPropertyPage::OnTimer(nIDEvent);
}


void MouseExPage::OnRadioMouseOn() 
{
	P5.P5_SetMouseState(0, TRUE);
}

void MouseExPage::OnRadioMouseOff() 
{
	P5.P5_SetMouseState(0, FALSE);
	
}

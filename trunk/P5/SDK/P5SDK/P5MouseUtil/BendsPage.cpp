// BendsPage.cpp : implementation file
//

#include "stdafx.h"
#include "P5UtilBeta.h"
#include "BendsPage.h"
#include "p5dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL bP5Present;
extern CP5DLL P5;
int nResetCtr = 0;
BOOL bButPressed = FALSE;

/////////////////////////////////////////////////////////////////////////////
// BendsPage property page

IMPLEMENT_DYNCREATE(BendsPage, CPropertyPage)

BendsPage::BendsPage() : CPropertyPage(BendsPage::IDD)
{
	//{{AFX_DATA_INIT(BendsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BendsPage::~BendsPage()
{
}

void BendsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(BendsPage)
	DDX_Control(pDX, IDC_TXT_BENDS_MSG, m_txtBendsMsg);
	DDX_Control(pDX, IDC_THUMB, m_progThumb);
	DDX_Control(pDX, IDC_RING, m_progRing);
	DDX_Control(pDX, IDC_PINKY, m_progPinky);
	DDX_Control(pDX, IDC_MIDDLE, m_progMiddle);
	DDX_Control(pDX, IDC_INDEX, m_progIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(BendsPage, CPropertyPage)
	//{{AFX_MSG_MAP(BendsPage)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// BendsPage message handlers

BOOL BendsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	SetTimer(1234, 10, NULL);

	m_progThumb.SetRange(0,63);
	m_progThumb.SetStep(1);
	m_progIndex.SetRange(0,63);
	m_progIndex.SetStep(1);
	m_progMiddle.SetRange(0,63);
	m_progMiddle.SetStep(1);
	m_progRing.SetRange(0,63);
	m_progRing.SetStep(1);
	m_progPinky.SetRange(0,63);
	m_progPinky.SetStep(1);

	if(bP5Present)
	{
		m_txtBendsMsg.SetWindowText("Status: tracking...");
	}

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void BendsPage::OnTimer(UINT nIDEvent) 
{
	unsigned char value;

	if(P5.m_P5Devices != NULL)
	{
		value=((unsigned char)P5.m_P5Devices[0].m_byBendSensor_Data[P5_INDEX]);
		m_progIndex.SetPos(63-value);
		
		value=((unsigned char)P5.m_P5Devices[0].m_byBendSensor_Data[P5_MIDDLE]);
		m_progMiddle.SetPos(63-value);
		
		value=((unsigned char)P5.m_P5Devices[0].m_byBendSensor_Data[P5_RING]);
		m_progRing.SetPos(63-value);
		
		value=((unsigned char)P5.m_P5Devices[0].m_byBendSensor_Data[P5_PINKY]);
		m_progPinky.SetPos(63-value);

		value=((unsigned char)P5.m_P5Devices[0].m_byBendSensor_Data[P5_THUMB]);
		m_progThumb.SetPos(63-value);

		// saving fingers :: check if we are in the right pane
		if( this->IsWindowVisible() )
		{
			if(P5.m_P5Devices[0].m_byButtons[0] && !bButPressed)
			{
				if(nResetCtr%2)
				{
					if(P5.m_P5Devices != NULL)
					{
						P5.P5_SaveBendSensors(0);
						m_txtBendsMsg.SetWindowText("Fingers has been Saved");
					}
				}
				else
				{
					if(P5.m_P5Devices != NULL) 
					{
						P5.P5_CalibrateBendSensors(0);
						m_txtBendsMsg.SetWindowText("Make a fist and hit 'A' button on P5 Device");
					}
				}

				nResetCtr ++;
				bButPressed = TRUE;
			}

			if(!P5.m_P5Devices[0].m_byButtons[0])
			{
				bButPressed = FALSE;
			}
		}
	}
	
	CPropertyPage::OnTimer(nIDEvent);
}

// XyzPage.cpp : implementation file
//

#include "stdafx.h"
#include "P5UtilBeta.h"
#include "XyzPage.h"
#include "p5dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CP5DLL P5;
extern BOOL bP5Present;

/////////////////////////////////////////////////////////////////////////////
// XyzPage property page

IMPLEMENT_DYNCREATE(XyzPage, CPropertyPage)

XyzPage::XyzPage() : CPropertyPage(XyzPage::IDD)
{
	//{{AFX_DATA_INIT(XyzPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

XyzPage::~XyzPage()
{
}

void XyzPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(XyzPage)
	DDX_Control(pDX, IDC_TXT_Z, m_txtz);
	DDX_Control(pDX, IDC_TXT_Y, m_txty);
	DDX_Control(pDX, IDC_TXT_X, m_txtx);
	DDX_Control(pDX, IDC_SLD_Z, m_sldz);
	DDX_Control(pDX, IDC_SLD_Y, m_sldy);
	DDX_Control(pDX, IDC_SLD_X, m_sldx);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(XyzPage, CPropertyPage)
	//{{AFX_MSG_MAP(XyzPage)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RESET_POS_DATA, OnResetPosData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// XyzPage message handlers

BOOL XyzPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_sldx.SetRange(0,1024);
	m_sldx.SetTicFreq(10);

	m_sldy.SetRange(0,1024);
	m_sldy.SetTicFreq(10);

	m_sldz.SetRange(0,1024);
	m_sldz.SetTicFreq(10);

	SetTimer(1005, 10, NULL);

	GetDlgItem(IDC_SLD_X)->EnableWindow(bP5Present);
	GetDlgItem(IDC_SLD_Y)->EnableWindow(bP5Present);
	GetDlgItem(IDC_SLD_Z)->EnableWindow(bP5Present);
	GetDlgItem(IDC_RESET_POS_DATA)->EnableWindow(bP5Present);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void XyzPage::OnTimer(UINT nIDEvent) 
{
	float signedvalue;
	CString tag;

	if(bP5Present)
	{
		signedvalue=P5.m_P5Devices[0].m_fx;
		
		tag.Format("X:%2.2f", (float)(signedvalue)/P5HEAD_SEPARATION);
		m_txtx.SetWindowText(tag);
		signedvalue=(signedvalue+512);
		m_sldx.SetPos((int)signedvalue);

		signedvalue=P5.m_P5Devices[0].m_fy;
		signedvalue*=-1;
		tag.Format("Y:%2.2f", (float)(signedvalue)/P5HEAD_SEPARATION);
		m_txty.SetWindowText(tag);
		signedvalue=(signedvalue+512);
		m_sldy.SetPos((int)signedvalue);

		signedvalue=P5.m_P5Devices[0].m_fz;
		
		tag.Format("Z:%2.2f", (float)(signedvalue)/P5HEAD_SEPARATION);
		m_txtz.SetWindowText(tag);
		signedvalue=(signedvalue+512);
		m_sldz.SetPos((int)signedvalue);
	}
	
	CPropertyPage::OnTimer(nIDEvent);
}

void XyzPage::OnResetPosData() 
{
	if(bP5Present)
		P5.P5_CalibratePositionData(0);
	
}

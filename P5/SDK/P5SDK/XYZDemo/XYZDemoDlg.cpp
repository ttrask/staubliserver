// XYZDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XYZDemo.h"
#include "XYZDemoDlg.h"
#include "p5dll.h"
#include "p5motion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CP5DLL P5;
BOOL bP5Present  = FALSE;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXYZDemoDlg dialog

CXYZDemoDlg::CXYZDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXYZDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXYZDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXYZDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXYZDemoDlg)
	DDX_Control(pDX, IDC_TXT_PLANE, m_txtPlane);
	DDX_Control(pDX, IDC_TXT_MSG, m_txtMsg);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CXYZDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CXYZDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXYZDemoDlg message handlers

BOOL CXYZDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	//Set up refresh timer
	SetTimer(3100, 10, NULL);

	m_XYPlane = TRUE;
	m_XZPlane = FALSE;


	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	if ((bP5Present=P5.P5_Init()) != TRUE)
	{
//		sprintf(msgStatus, "No P5 detected...\r\n");
//		GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(msgStatus);
//		GetDlgItem(IDC_CHECK_YPR)->EnableWindow(bP5Present);
		m_txtMsg.SetWindowText("P5 Not Found");
	}
	else
	{
//		InitState = TRUE;
//		strcat(msgStatus, "P5 Found...\r\n");
//		GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(msgStatus);
//		GetDlgItem(IDC_CHECK_YPR)->EnableWindow(bP5Present);

//		revisionNumber.Format("Firmware Rev: %d.%d",P5.m_P5Devices[0].m_nMajorRevisionNumber,P5.m_P5Devices[0].m_nMinorRevisionNumber);
//		GetDlgItem(IDC_REV_NUMBER)->SetWindowText(revisionNumber);

		RECT region;

		P5Motion_Init(&P5, 0);

		P5Motion_InvertMouse(P5MOTION_INVERTAXIS, P5MOTION_NORMALAXIS, P5MOTION_NORMALAXIS);
		GetClipCursor(&region);
		P5Motion_SetClipRegion(region.left, region.right, region.top, region.bottom, region.top, region.bottom);

		P5.P5_SetMouseState(0, FALSE);

		m_txtMsg.SetWindowText("P5 Initialized");
		m_txtPlane.SetWindowText("XY Plane");
	}

	


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CXYZDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CXYZDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CXYZDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CXYZDemoDlg::OnTimer(UINT nIDEvent) 
{
	static int lock=0;
	CString tag;
	CString fastreads;
	CString textstr;

	if (lock)
		return;
	lock=1;

	if (P5.m_P5Devices!=NULL)
	{

		P5Motion_Process();

		if(P5.m_P5Devices[0].m_byButtons[0]==1)
		{
			m_XYPlane = TRUE;
			m_XZPlane = FALSE;
			m_txtPlane.SetWindowText("XY Plane");
		}

		if(P5.m_P5Devices[0].m_byButtons[1]==1)
		{
			m_XYPlane = FALSE;
			m_XZPlane = TRUE;
			m_txtPlane.SetWindowText("XZ Plane");
		}


		if(m_XYPlane == TRUE)
			SetCursorPos(nXPos, nYPos);
		if(m_XZPlane == TRUE)
			SetCursorPos(nXPos, nZPos);

	}
	lock=0;
	CDialog::OnTimer(nIDEvent);
}


LRESULT CXYZDemoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WM_DEVICECHANGE)
	{
		if ( wParam == 0x0007 )
		{
 			if( bP5Present = P5.P5_Init() )
				m_txtMsg.SetWindowText("P5 Initialized");
			else
				m_txtMsg.SetWindowText("P5 Not Found");
		}
	}
	
	return CDialog::WindowProc(message, wParam, lParam);
}

void CXYZDemoDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	P5.P5_SetMouseState(0, TRUE);	
	
}

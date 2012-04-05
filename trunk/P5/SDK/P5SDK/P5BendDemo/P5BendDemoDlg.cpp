// P5BendDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "P5BendDemo.h"
#include "P5BendDemoDlg.h"
#include "p5dll.h"
#include "p5bend.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char msgStatus[1024];
CP5DLL P5;

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
// CP5BendDemoDlg dialog

CP5BendDemoDlg::CP5BendDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CP5BendDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CP5BendDemoDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CP5BendDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CP5BendDemoDlg)
	DDX_Control(pDX, IDC_THUMBEDGE, m_ThumbEdge);
	DDX_Control(pDX, IDC_RINGEDGE, m_RingEdge);
	DDX_Control(pDX, IDC_PINKYEDGE, m_PinkyEdge);
	DDX_Control(pDX, IDC_MIDDLEEDGE, m_MiddleEdge);
	DDX_Control(pDX, IDC_INDEXEDGE, m_IndexEdge);
	DDX_Control(pDX, IDC_THUMBSLIDER, m_ThumbSlider);
	DDX_Control(pDX, IDC_RINGSLIDER, m_RingSlider);
	DDX_Control(pDX, IDC_PINKYSLIDER, m_PinkySlider);
	DDX_Control(pDX, IDC_MIDDLESLIDER, m_MiddleSlider);
	DDX_Control(pDX, IDC_INDEXSLIDER, m_IndexSlider);
	DDX_Control(pDX, IDC_THUMBCLICKSTATE, m_ThumbState);
	DDX_Control(pDX, IDC_RINGCLICKSTATE, m_RingState);
	DDX_Control(pDX, IDC_PINKYCLICKSTATE, m_PinkyState);
	DDX_Control(pDX, IDC_MIDDLECLICKSTATE, m_MiddleState);
	DDX_Control(pDX, IDC_INDEXCLICKSTATE, m_IndexState);
	DDX_Control(pDX, IDC_THUMB, m_Thumb);
	DDX_Control(pDX, IDC_RING, m_Ring);
	DDX_Control(pDX, IDC_PINKY, m_Pinky);
	DDX_Control(pDX, IDC_MIDDLE, m_Middle);
	DDX_Control(pDX, IDC_INDEX, m_Index);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CP5BendDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CP5BendDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CP5BendDemoDlg message handlers

BOOL CP5BendDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

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

	if (P5.P5_Init() != TRUE)
	{
		sprintf(msgStatus, "No P5 detected...\r\n");
		GetDlgItem(IDC_STATUSMSG)->SetWindowText(msgStatus);
	}
	else
	{
		strcat(msgStatus, "P5 Found...\r\n");
		GetDlgItem(IDC_STATUSMSG)->SetWindowText(msgStatus);

		P5Bend_Init(&P5, 0);
		P5.P5_SetMouseState(0, false);
	}

	SetTimer(3100, 10, NULL);

	m_Index.SetRange(0,63);
	m_Index.SetStep(1);

	m_Middle.SetRange(0,63);
	m_Middle.SetStep(1);
	
	m_Ring.SetRange(0,63);
	m_Ring.SetStep(1);
	
	m_Pinky.SetRange(0,63);
	m_Pinky.SetStep(1);
	
	m_Thumb.SetRange(0,63);
	m_Thumb.SetStep(1);

	m_ThumbSlider.SetRange(0, 30);
	m_ThumbSlider.SetPos(nBendSensitivity[P5_THUMB]);
	m_ThumbSlider.SetTicFreq(5);

	m_IndexSlider.SetRange(0, 30);
	m_IndexSlider.SetPos(nBendSensitivity[P5_INDEX]);
	m_IndexSlider.SetTicFreq(5);

	m_MiddleSlider.SetRange(0, 30);
	m_MiddleSlider.SetPos(nBendSensitivity[P5_MIDDLE]);
	m_MiddleSlider.SetTicFreq(5);

	m_RingSlider.SetRange(0, 30);
	m_RingSlider.SetPos(nBendSensitivity[P5_RING]);
	m_RingSlider.SetTicFreq(5);

	m_PinkySlider.SetRange(0, 30);
	m_PinkySlider.SetPos(nBendSensitivity[P5_PINKY]);
	m_PinkySlider.SetTicFreq(5);

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CP5BendDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CP5BendDemoDlg::OnPaint() 
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
HCURSOR CP5BendDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CP5BendDemoDlg::OnTimer(UINT nIDEvent) 
{
	if (P5.m_P5Devices!=NULL)
	{
		int value;

		value=((unsigned char)P5.m_P5Devices[0].m_byBendSensor_Data[P5_INDEX]);
		m_Index.SetPos(63-value);
		
		value=((unsigned char)P5.m_P5Devices[0].m_byBendSensor_Data[P5_MIDDLE]);
		m_Middle.SetPos(63-value);
		
		value=((unsigned char)P5.m_P5Devices[0].m_byBendSensor_Data[P5_RING]);
		m_Ring.SetPos(63-value);
		
		value=((unsigned char)P5.m_P5Devices[0].m_byBendSensor_Data[P5_PINKY]);
		m_Pinky.SetPos(63-value);

		value=((unsigned char)P5.m_P5Devices[0].m_byBendSensor_Data[P5_THUMB]);
		m_Thumb.SetPos(63-value);

		P5Bend_SetClickSensitivity(P5_THUMB, m_ThumbSlider.GetPos());
		P5Bend_SetClickSensitivity(P5_INDEX, m_IndexSlider.GetPos());
		P5Bend_SetClickSensitivity(P5_MIDDLE, m_MiddleSlider.GetPos());
		P5Bend_SetClickSensitivity(P5_RING, m_RingSlider.GetPos());
		P5Bend_SetClickSensitivity(P5_PINKY, m_PinkySlider.GetPos());

		P5Bend_Process();

		m_IndexState.SetCheck(bP5ClickLevel[P5_INDEX]);
		m_MiddleState.SetCheck(bP5ClickLevel[P5_MIDDLE]);
		m_RingState.SetCheck(bP5ClickLevel[P5_RING]);
		m_PinkyState.SetCheck(bP5ClickLevel[P5_PINKY]);
		m_ThumbState.SetCheck(bP5ClickLevel[P5_THUMB]);

		sprintf (msgStatus, "%d", nP5ClickEdge[P5_INDEX]);
		m_IndexEdge.SetWindowText(msgStatus);

		sprintf (msgStatus, "%d", nP5ClickEdge[P5_MIDDLE]);
		m_MiddleEdge.SetWindowText(msgStatus);

		sprintf (msgStatus, "%d", nP5ClickEdge[P5_RING]);
		m_RingEdge.SetWindowText(msgStatus);

		sprintf (msgStatus, "%d", nP5ClickEdge[P5_PINKY]);
		m_PinkyEdge.SetWindowText(msgStatus);

		sprintf (msgStatus, "%d", nP5ClickEdge[P5_THUMB]);
		m_ThumbEdge.SetWindowText(msgStatus);


	}
	
	CDialog::OnTimer(nIDEvent);
}

void CP5BendDemoDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	P5.P5_SetMouseState(0, true);
	
}

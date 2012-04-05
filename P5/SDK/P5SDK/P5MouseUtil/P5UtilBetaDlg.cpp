// P5UtilBetaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Dbt.h"
#include "P5UtilBeta.h"
#include "P5UtilBetaDlg.h"
#include "P5dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CP5DLL P5;
BOOL bP5Present;

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
// CP5UtilBetaDlg dialog



void CP5UtilBetaDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertySheet::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CP5UtilBetaDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CP5UtilBetaDlg, CPropertySheet)
	//{{AFX_MSG_MAP(CP5UtilBetaDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND_RANGE( IDD_DLG_BENDS, IDD_DLG_MOUSE, DisplayPage )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CP5UtilBetaDlg message handlers

BOOL CP5UtilBetaDlg::OnInitDialog()
{
	CPropertySheet::OnInitDialog();

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
	
	// TODO: Add extra initialization here


	GetDlgItem( IDOK )->ShowWindow( SW_HIDE );
	GetDlgItem( IDCANCEL )->ShowWindow( SW_HIDE );
	GetDlgItem( IDHELP )->ShowWindow( SW_HIDE );
	GetDlgItem( ID_APPLY_NOW )->ShowWindow( SW_HIDE );

	


	if(bP5Present = P5.P5_Init())
	{
		SetWindowText("P5 Utility [P5 Initialized]");
	}else
	{
		SetWindowText("P5 Utility [P5 not found]");
	}

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CP5UtilBetaDlg::DisplayPage(int page)
{
		switch( page )
		{
			case IDD_DLG_BENDS :
				SetActivePage( &m_bendsPage );
				break;
			case IDD_DLG_MOUSE :
				SetActivePage( &m_mousePage );
				break;
			case IDD_DLG_MOUSE_EX :
				SetActivePage( &m_mouseExPage );
				break;
#ifdef FULL
			case IDD_DLG_XYZ :
				SetActivePage( &m_xyzPage );
				break;

			case IDD_DLG_BUTTONS :
				SetActivePage( &m_butPage );
				break;
#endif
		}
}


void CP5UtilBetaDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CPropertySheet::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CP5UtilBetaDlg::OnPaint() 
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
		CPropertySheet::OnPaint();
	}

}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CP5UtilBetaDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


LRESULT CP5UtilBetaDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WM_DEVICECHANGE)
	{
		switch(wParam)
		{
		case DBT_DEVICEARRIVAL:
			MessageBox("DBT_DEVICEARRIVAL");
			break;

		case DBT_DEVICEREMOVECOMPLETE:
			MessageBox("DBT_DEVICEREMOVECOMPLETE");
			break;
		
		default:
			break;
		}
	}
	
	return CPropertySheet::WindowProc(message, wParam, lParam);
}

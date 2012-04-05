/******************************************************************************
//	File:	YPRModeTest.cpp
//	
//	Authors:	Igor Borysov
//	
//	Revision History:
//
//	May 8/02: Created file for sample code
//
//
//	Copyright (c) 2001 Essential Reality LLC
******************************************************************************/
#define STRICT
#include "stdafx.h"
#include "Dbt.h"
#include "YPRModeTest.h"
#include "p5dll.h"
#include "P5Motion.h"

#define VAL		30  //threshold for the YPR stuff
#define BENDT	20


//-----------------------------------------------------------------------------
// Application globals
//-----------------------------------------------------------------------------
TCHAR*          g_strAppTitle       = _T( "YPR Mode Test" );
CApp            g_App;
HINSTANCE       g_hInst = NULL;
CAppForm*       g_AppFormView = NULL;

CP5DLL P5;
BOOL bP5Present = FALSE;
RECT region;
D3DXMATRIX matRot;

//-----------------------------------------------------------------------------
// The MFC macros are all listed here
//-----------------------------------------------------------------------------
IMPLEMENT_DYNCREATE( CAppDoc,      CDocument )
IMPLEMENT_DYNCREATE( CAppFrameWnd, CFrameWnd )
IMPLEMENT_DYNCREATE( CAppForm,     CFormView )




BEGIN_MESSAGE_MAP( CApp, CWinApp )
    //{{AFX_MSG_MAP(CApp)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()




BEGIN_MESSAGE_MAP( CAppForm, CFormView )
    //{{AFX_MSG_MAP(CAppForm)
    ON_COMMAND(    IDC_VIEWFULLSCREEN, OnToggleFullScreen )
    ON_BN_CLICKED(IDC_CHANGEDEVICE, OnChangeDevice)
	ON_BN_CLICKED(IDC_MODE_ABS, OnModeAbs)
	ON_BN_CLICKED(IDC_MODE_RC, OnModeRc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BEGIN_MESSAGE_MAP(CAppDoc, CDocument)
    //{{AFX_MSG_MAP(CAppDoc)
            // NOTE - the ClassWizard will add and remove mapping macros here.
            //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()




BEGIN_MESSAGE_MAP(CAppFrameWnd, CFrameWnd)
    //{{AFX_MSG_MAP(CAppFrameWnd)
    ON_COMMAND(IDM_CHANGEDEVICE, OnChangeDevice)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()




//-----------------------------------------------------------------------------
// Name: CAppForm()
// Desc: Constructor for the dialog resource form
//-----------------------------------------------------------------------------
CAppForm::CAppForm()
         :CFormView( IDD_FORMVIEW )
{
    //{{AFX_DATA_INIT(CAppForm)
	//}}AFX_DATA_INIT

    g_AppFormView          = this;
    m_hwndRenderWindow     = NULL;
    m_hwndRenderFullScreen = NULL;
    m_hWndTopLevelParent   = NULL;

    // Override some CD3DApplication defaults:
    m_dwCreationWidth           = 800;
    m_dwCreationHeight          = 600;
    m_strWindowTitle            = TEXT( "YPR Mode Test" );
    m_bUseDepthBuffer           = TRUE;

    m_pD3DXFont                 = NULL;
    m_bLoadingApp               = TRUE;
    m_pD3DXMesh                 = NULL;

    ZeroMemory( &m_UserInput, sizeof(m_UserInput) );
    m_fWorldRotY                = 0.0f;
    m_fWorldRotP                = 0.0f;
	m_fWorldRotR				= 0.0f;
	m_fInz						= 0.0f;

	m_nThreshold				= VAL;
	m_bDemoing					= FALSE;

    // Read settings from registry
    ReadSettings();
}




//-----------------------------------------------------------------------------
// Name: CAppForm::OneTimeSceneInit()
// Desc: Called during initial app startup, this function performs all the
//       permanent initialization.
//-----------------------------------------------------------------------------
HRESULT CAppForm::OneTimeSceneInit()
{
	m_sldYaw.SetRange( -179, 180, FALSE );
	m_sldYaw.SetPos( 0 );
	m_sldYaw.SetZero( 180 );
	m_sldYaw.SetInverted();
	m_sldYaw.SetKnobColor( RGB(0, 0, 255) );

	m_sldPitch.SetRange( -179, 180, FALSE );
	m_sldPitch.SetPos( 0 );
	m_sldPitch.SetZero( 180 );
	m_sldPitch.SetInverted();
	m_sldPitch.SetKnobColor( RGB(0, 0, 255) );

	m_sldRoll.SetRange( -179, 180, FALSE );
	m_sldRoll.SetPos( 0 );
	m_sldRoll.SetZero( 180 );
	m_sldRoll.SetInverted();
	m_sldRoll.SetKnobColor( RGB(0, 0, 255) );

	if( bP5Present = P5.P5_Init() )
	{
		CButton *mb;
		CString t;

		mb = (CButton*)(GetDlgItem( IDC_MODE_RC ));
		mb->SetCheck( TRUE );

		P5.P5_SetMouseState( 0, FALSE );
		m_txtMsg.SetWindowText( "P5 Initialized" );

		P5Motion_Init( &P5, 0 );
		P5Motion_InvertMouse(P5MOTION_INVERTAXIS, P5MOTION_NORMALAXIS, P5MOTION_NORMALAXIS);
		GetClipCursor(&region);
		P5Motion_SetClipRegion(region.left, region.right, region.top, region.bottom, 0, 1000);

		mb = (CButton*)(GetDlgItem( IDC_CHECK_ENABLE_YAW ));
		mb->SetCheck( TRUE );
		mb = (CButton*)(GetDlgItem( IDC_CHECK_ENABLE_PITCH ));
		mb->SetCheck( TRUE );
		mb = (CButton*)(GetDlgItem( IDC_CHECK_ENABLE_ROLL ));
		mb->SetCheck( TRUE );
	}
	else
	{
		m_txtMsg.SetWindowText( "P5 is not initialized" );
		GetDlgItem( IDC_MODE_ABS )->EnableWindow( FALSE );
		GetDlgItem( IDC_MODE_RC )->EnableWindow( FALSE );

		m_sldYaw.EnableWindow( FALSE );
		m_sldPitch.EnableWindow( FALSE );
		m_sldRoll.EnableWindow( FALSE );
	}


	D3DXMatrixIdentity( &matRot);
	m_bLoadingApp = FALSE;
	m_bYprMode = RT_CTRL;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ReadSettings()
// Desc: Read the app settings from the registry
//-----------------------------------------------------------------------------
VOID CAppForm::ReadSettings()
{
    HKEY hkey;
    if( ERROR_SUCCESS == RegCreateKeyEx( HKEY_CURRENT_USER, DXAPP_KEY, 
        0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL ) )
    {
        // TODO: change as needed

        // Read the stored window width/height.  This is just an example,
        // of how to use DXUtil_Read*() functions.
        DXUtil_ReadIntRegKey( hkey, TEXT("Width"), &m_dwCreationWidth, m_dwCreationWidth );
        DXUtil_ReadIntRegKey( hkey, TEXT("Height"), &m_dwCreationHeight, m_dwCreationHeight );

        RegCloseKey( hkey );
    }
}




//-----------------------------------------------------------------------------
// Name: WriteSettings()
// Desc: Write the app settings to the registry
//-----------------------------------------------------------------------------
VOID CAppForm::WriteSettings()
{
    HKEY hkey;
    DWORD dwType = REG_DWORD;
    DWORD dwLength = sizeof(DWORD);

    if( ERROR_SUCCESS == RegCreateKeyEx( HKEY_CURRENT_USER, DXAPP_KEY, 
        0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL ) )
    {
        // TODO: change as needed

        // Write the window width/height.  This is just an example,
        // of how to use DXUtil_Write*() functions.
        DXUtil_WriteIntRegKey( hkey, TEXT("Width"), m_rcWindowClient.right );
        DXUtil_WriteIntRegKey( hkey, TEXT("Height"), m_rcWindowClient.bottom );

        RegCloseKey( hkey );
    }
}





//-----------------------------------------------------------------------------
// Name: ConfirmDevice()
// Desc: Called during device initialization, this code checks the display device
//       for some minimum set of capabilities
//-----------------------------------------------------------------------------
HRESULT CAppForm::ConfirmDevice( D3DCAPS8* pCaps, DWORD dwBehavior,
                                          D3DFORMAT Format )
{
    BOOL bCapsAcceptable;

    // TODO: Perform checks to see if these display caps are acceptable.
    bCapsAcceptable = TRUE;

    if( bCapsAcceptable )         
        return S_OK;
    else
        return E_FAIL;
}




//-----------------------------------------------------------------------------
// Name: CAppForm::InitDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT CAppForm::InitDeviceObjects()
{
    // TODO: create device objects

    HRESULT hr;

    // Create a teapot mesh using D3DX
	if( FAILED( hr = D3DXCreateTeapot( m_pd3dDevice, &m_pD3DXMesh, NULL ) ) )
		return DXTRACE_ERR_NOMSGBOX( "D3DXCreateTeapot", hr );

//	if( FAILED( hr = D3DXCreateBox( m_pd3dDevice, 1.f, 1.f, 1.f, &m_pD3DXMesh, NULL ) ) )
//		return DXTRACE_ERR_NOMSGBOX( "D3DXCreateTeapot", hr );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CAppForm::RestoreDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT CAppForm::RestoreDeviceObjects()
{
    // TODO: setup render states
    HRESULT hr;

    // Setup a material
    D3DMATERIAL8 mtrl;
    D3DUtil_InitMaterial( mtrl, 1.0f, 0.0f, 0.0f );
    m_pd3dDevice->SetMaterial( &mtrl );

    // Set up the textures
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );

    // Set miscellaneous render states
    m_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE,   FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,        TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_AMBIENT,        0x000F0F0F );

    // Set the world matrix
    D3DXMATRIX matIdentity;
    D3DXMatrixIdentity( &matIdentity );
    m_pd3dDevice->SetTransform( D3DTS_WORLD,  &matIdentity );

    // Set up our view matrix. A view matrix can be defined given an eye point,
    // a point to lookat, and a direction for which way is up. Here, we set the
    // eye five units back along the z-axis and up three units, look at the
    // origin, and define "up" to be in the y-direction.
    D3DXMATRIX matView;
    D3DXVECTOR3 vFromPt   = D3DXVECTOR3( 0.0f, 0.0f, -5.0f );
    D3DXVECTOR3 vLookatPt = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
    D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
    m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

    // Set the projection matrix
    D3DXMATRIX matProj;
    FLOAT fAspect = ((FLOAT)m_d3dsdBackBuffer.Width) / m_d3dsdBackBuffer.Height;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, fAspect, 1.0f, 100.0f );
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

    // Set up lighting states
    D3DLIGHT8 light;
    D3DUtil_InitLight( light, D3DLIGHT_DIRECTIONAL, -1.0f, -1.0f, 2.0f );
    m_pd3dDevice->SetLight( 0, &light );
    m_pd3dDevice->LightEnable( 0, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

    // Create a D3D font using D3DX
    HFONT hFont = CreateFont( 14, 0, 0, 0, 0, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                              ANTIALIASED_QUALITY, FF_DONTCARE, "Arial" );
	
    if( FAILED( hr = D3DXCreateFont( m_pd3dDevice, hFont, &m_pD3DXFont ) ) )
        return DXTRACE_ERR_NOMSGBOX( "D3DXCreateFont", hr );

    return S_OK;
}


VOID CAppForm::AreWeDoingSomething()
{
	(P5.m_P5Devices[0].m_byBendSensor_Data[P5_RING] > BENDT && 
		P5.m_P5Devices[0].m_byBendSensor_Data[P5_PINKY] > BENDT )? m_bDemoing = TRUE : m_bDemoing = FALSE;
}


//-----------------------------------------------------------------------------
// Name: CAppForm::FrameMove()
// Desc: Called once per frame, the call is the entry point for animating
//       the scene.
//-----------------------------------------------------------------------------

HRESULT CAppForm::FrameMove()
{
    // Update user input state
    UpdateInput( &m_UserInput );

	//check for the P5
	if( P5.m_P5Devices != NULL )
	{
		CString t;
		D3DXMATRIX matTemp;
		static float fOldY, fOldP, fOldR;
		static int firsttime = 1;

		AreWeDoingSomething();

		P5Motion_Process();

		if( m_UserInput.bInzUp && !m_UserInput.bInzDown )
			m_fInz += 0.01f;
		else if( m_UserInput.bInzDown && !m_UserInput.bInzUp )
			m_fInz -= 0.01f;

		if( m_fInz < -3.f )
			m_fInz = -3.f;

		if( m_fInz > 0.f )
			m_fInz = 0.f;

		if (firsttime==1)
		{
			firsttime = 0;

			fOldY = fRelYawPos;
			fOldP = fRelPitchPos;
			fOldR = fRelRollPos;
		}
		else
		{
			if( m_bDemoing )
			{
				m_sldYaw.SetPosition( (int)(fAbsYawPos) );
				m_sldPitch.SetPosition( (int)(fAbsPitchPos) );
				m_sldRoll.SetPosition( (int)(fAbsRollPos) );
			}

			if( m_bYprMode == RT_CTRL )
			{
				m_fWorldRotP =  (fOldY - fRelYawPos)*3.1415f/180.0f;
				m_fWorldRotY = (fOldP - fRelPitchPos)*3.1415f/180.0f;
				m_fWorldRotR = (fOldR - fRelRollPos)*3.1415f/180.0f;

				fOldY = fRelYawPos;
				fOldP = fRelPitchPos;
				fOldR = fRelRollPos;
			}
			else
			{

				D3DXMatrixIdentity( &matRot);
				m_fWorldRotP =  fAbsYawPos*3.1415f/180.0f;
				m_fWorldRotY = -fAbsPitchPos*3.1415f/180.0f;
				m_fWorldRotR = -fAbsRollPos*3.1415f/180.0f;
			}

			D3DXMatrixTranslation( &matWorld, 0.f, 0.f, m_fInz );

			if( ((CButton*)GetDlgItem( IDC_CHECK_ENABLE_PITCH ))->GetCheck() )
			{
				D3DXMatrixRotationX( &matTemp, m_fWorldRotY );
				D3DXMatrixMultiply( &matRot,  &matRot, &matTemp  );
			}

			if( ((CButton*)GetDlgItem( IDC_CHECK_ENABLE_YAW ))->GetCheck() )
			{
				D3DXMatrixRotationY( &matTemp, m_fWorldRotP );
				D3DXMatrixMultiply( &matRot,  &matRot, &matTemp  );
			}

			if( ((CButton*)GetDlgItem( IDC_CHECK_ENABLE_ROLL ))->GetCheck() )
			{
				D3DXMatrixRotationZ( &matTemp, m_fWorldRotR );
				D3DXMatrixMultiply( &matRot,  &matRot, &matTemp  );
			}

			D3DXMatrixMultiply( &matWorld, &matRot, &matWorld );

			m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		}
	}

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: UpdateInput()
// Desc: Update the user input.  Called once per frame 
//-----------------------------------------------------------------------------
void CAppForm::UpdateInput( UserInput* pUserInput )
{
    m_bHasFocus = ( m_hWndTopLevelParent == GetForegroundWindow()->GetSafeHwnd() );

	if( P5.m_P5Devices != NULL )
	{
		if( m_bYprMode == RT_CTRL)
		{
			pUserInput->bRotateUp    = ( m_bHasFocus && (P5.m_P5Devices[0].m_fpitch >  VAL));
			pUserInput->bRotateDown  = ( m_bHasFocus && (P5.m_P5Devices[0].m_fpitch < -VAL));
			pUserInput->bRotateLeft  = ( m_bHasFocus && (P5.m_P5Devices[0].m_fyaw   >  VAL));
			pUserInput->bRotateRight = ( m_bHasFocus && (P5.m_P5Devices[0].m_fyaw   < -VAL));
			pUserInput->bRotateCCW   = ( m_bHasFocus && (P5.m_P5Devices[0].m_froll  >  VAL));
			pUserInput->bRotateCW    = ( m_bHasFocus && (P5.m_P5Devices[0].m_froll  < -VAL));
		}
	}

	pUserInput->bInzUp    = ( m_bHasFocus && (GetAsyncKeyState( VK_PRIOR )    & 0x8000) == 0x8000 );
	pUserInput->bInzDown  = ( m_bHasFocus && (GetAsyncKeyState( VK_NEXT )  & 0x8000) == 0x8000 );
}




//-----------------------------------------------------------------------------
// Name: CAppForm::Render()
// Desc: Called once per frame, the call is the entry point for 3d
//       rendering. This function sets up render states, clears the
//       viewport, and renders the scene.
//-----------------------------------------------------------------------------
HRESULT CAppForm::Render()
{
    // Clear the viewport
    m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
                         0x000000ff, 1.0f, 0L );

    // Begin the scene
    if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
    {
        // TODO: render world
        
        // Render the teapot mesh
        m_pD3DXMesh->DrawSubset(0);

        // Render stats and help text  
        RenderText();

        // End the scene.
        m_pd3dDevice->EndScene();
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: RenderText()
// Desc: Renders stats and help text to the scene.
//-----------------------------------------------------------------------------
HRESULT CAppForm::RenderText()
{
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DoDataExchange()
// Desc: DDX/DDV support
//-----------------------------------------------------------------------------
void CAppForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAppForm)
	DDX_Control(pDX, IDC_TXT_MSG, m_txtMsg);
	DDX_Control(pDX, IDC_SLD_ROLL, m_sldRoll);
	DDX_Control(pDX, IDC_SLD_PITCH, m_sldPitch);
	DDX_Control(pDX, IDC_SLD_YAW, m_sldYaw);
	//}}AFX_DATA_MAP
}




//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: 
//-----------------------------------------------------------------------------
LRESULT CAppForm::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WM_DEVICECHANGE)
	{
		if (wParam == 0x0b7 )
		{
 			if( bP5Present = P5.P5_Init() )
				m_txtMsg.SetWindowText( "P5 Found" );
			else
				m_txtMsg.SetWindowText( "P5 is not here" );
		}
	}

    return CFormView ::WindowProc(message, wParam, lParam);
}




//-----------------------------------------------------------------------------
// Name: OnChangeDevice()
// Desc: Needed to enable dlg menu item 
//-----------------------------------------------------------------------------
void CAppFrameWnd::OnChangeDevice() 
{
    g_AppFormView->OnChangeDevice();
}




//-----------------------------------------------------------------------------
// Name: CAppForm::InvalidateDeviceObjects()
// Desc: Called when the device-dependent objects are about to be lost.
//-----------------------------------------------------------------------------
HRESULT CAppForm::InvalidateDeviceObjects()
{
    // TODO: Cleanup any objects created in RestoreDeviceObjects()
    SAFE_RELEASE( m_pD3DXFont );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CAppForm::DeleteDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//       this function deletes any device dependent objects.
//-----------------------------------------------------------------------------
HRESULT CAppForm::DeleteDeviceObjects()
{
    // TODO: Cleanup any objects created in InitDeviceObjects()
    SAFE_RELEASE( m_pD3DXMesh );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CAppForm::FinalCleanup()
// Desc: Called before the app exits, this function gives the app the chance
//       to cleanup after itself.
//-----------------------------------------------------------------------------
HRESULT CAppForm::FinalCleanup()
{
    // TODO: Perform any final cleanup needed
    // Write the settings to the registry
    WriteSettings();

	P5.P5_SetMouseState( 0, TRUE );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InitInstance()
// Desc: This is the main entry point for the application. The MFC window stuff
//       is initialized here. See also the main initialization routine for the
//       CAppForm class, which is called indirectly from here.
//-----------------------------------------------------------------------------
BOOL CApp::InitInstance()
{
    // Asscociate the MFC app with the frame window and doc/view classes
    AddDocTemplate( new CSingleDocTemplate( IDR_MAINFRAME,
                                            RUNTIME_CLASS(CAppDoc),
                                            RUNTIME_CLASS(CAppFrameWnd),
                                            RUNTIME_CLASS(CAppForm) ) );

    // Dispatch commands specified on the command line (req'd by MFC). This
    // also initializes the the CAppDoc, CAppFrameWnd, and CAppForm classes.
    CCommandLineInfo cmdInfo;
    ParseCommandLine( cmdInfo );
    if( !ProcessShellCommand( cmdInfo ) )
        return FALSE;

    if( !g_AppFormView->IsReady() )
        return FALSE;

    g_AppFormView->GetParentFrame()->RecalcLayout();
    g_AppFormView->ResizeParentToFit( FALSE ); 
    
    m_pMainWnd->SetWindowText( g_strAppTitle );
    m_pMainWnd->UpdateWindow();

    return TRUE;
}




//-----------------------------------------------------------------------------
// Name: LoadFrame()
// Desc: Uses idle time to render the 3D scene.
//-----------------------------------------------------------------------------
BOOL CAppFrameWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
    BOOL bResult = CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);

    LoadAccelTable( MAKEINTRESOURCE(IDR_MAIN_ACCEL) );

    return bResult;
}




//-----------------------------------------------------------------------------
// Name: OnIdle()
// Desc: Uses idle time to render the 3D scene.
//-----------------------------------------------------------------------------
BOOL CApp::OnIdle( LONG )
{
    // Do not render if the app is minimized
    if( m_pMainWnd->IsIconic() )
        return FALSE;

    TCHAR strStatsPrev[200];

    lstrcpy(strStatsPrev, g_AppFormView->PstrFrameStats());

    // Update and render a frame
    if( g_AppFormView->IsReady() )
    {
        g_AppFormView->CheckForLostFullscreen();
        g_AppFormView->RenderScene();
    }

    // Keep requesting more idle time
    return TRUE;
}




//-----------------------------------------------------------------------------
// Name: PreCreateWindow()
// Desc: Change the window style (so it cannot maximize or be sized) before
//       the main frame window is created.
//-----------------------------------------------------------------------------
BOOL CAppFrameWnd::PreCreateWindow( CREATESTRUCT& cs )
{
    cs.style = WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX;

    return CFrameWnd::PreCreateWindow( cs );
}




//-----------------------------------------------------------------------------
// Name: ~CAppForm()
// Desc: Destructor for the dialog resource form. Shuts down the app
//-----------------------------------------------------------------------------
CAppForm::~CAppForm()
{
    Cleanup3DEnvironment();
}




//-----------------------------------------------------------------------------
// Name: OnToggleFullScreen()
// Desc: Called when user toggles the fullscreen mode
//-----------------------------------------------------------------------------
void CAppForm::OnToggleFullScreen()
{
    ToggleFullscreen();
}




//-----------------------------------------------------------------------------
// Name: OnChangeDevice()
// Desc: Use hit the "Change Device.." button. Display the dialog for the user
//       to select a new device/mode, and call Change3DEnvironment to
//       use the new device/mode.
//-----------------------------------------------------------------------------
VOID CAppForm::OnChangeDevice()
{
    Pause(TRUE);

    UserSelectNewDevice();

    // Update UI
    UpdateUIForDeviceCapabilites();

    Pause(FALSE);
}




//-----------------------------------------------------------------------------
// Name: AdjustWindowForChange()
// Desc: Adjusts the window properties for windowed or fullscreen mode
//-----------------------------------------------------------------------------
HRESULT CAppForm::AdjustWindowForChange()
{
    if( m_bWindowed )
    {
        ::ShowWindow( m_hwndRenderFullScreen, SW_HIDE );
        CD3DApplication::m_hWnd = m_hwndRenderWindow;
    }
    else
    {
        if( ::IsIconic( m_hwndRenderFullScreen ) )
            ::ShowWindow( m_hwndRenderFullScreen, SW_RESTORE );
        ::ShowWindow( m_hwndRenderFullScreen, SW_SHOW );
        CD3DApplication::m_hWnd = m_hwndRenderFullScreen;
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: FullScreenWndProc()
// Desc: The WndProc funtion used when the app is in fullscreen mode. This is
//       needed simply to trap the ESC key.
//-----------------------------------------------------------------------------
LRESULT CALLBACK FullScreenWndProc( HWND hWnd, UINT msg, WPARAM wParam,
                                    LPARAM lParam )
{
    if( msg == WM_CLOSE )
    {
        // User wants to exit, so go back to windowed mode and exit for real
        g_AppFormView->OnToggleFullScreen();
        g_App.GetMainWnd()->PostMessage( WM_CLOSE, 0, 0 );
    }
    else if( msg == WM_SETCURSOR )
    {
        SetCursor( NULL );
    }
    else if( msg == WM_KEYUP && wParam == VK_ESCAPE )
    {
        // User wants to leave fullscreen mode
        g_AppFormView->OnToggleFullScreen();
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}




//-----------------------------------------------------------------------------
// Name: CheckForLostFullscreen()
// Desc: If fullscreen and device was lost (probably due to alt-tab), 
//       automatically switch to windowed mode
//-----------------------------------------------------------------------------
HRESULT CAppForm::CheckForLostFullscreen()
{
    HRESULT hr;

    if( m_bWindowed )
        return S_OK;

    if( FAILED( hr = m_pd3dDevice->TestCooperativeLevel() ) )
        ForceWindowed();

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: UpdateUIForDeviceCapabilites()
// Desc: Whenever we get a new device, call this function to enable/disable the
//       appropiate UI controls to match the device's capabilities.
//-----------------------------------------------------------------------------
VOID CAppForm::UpdateUIForDeviceCapabilites()
{
    // TODO: Check the capabilities of the device and update the UI as needed
    DWORD dwCaps = m_d3dCaps.RasterCaps;
}




//-----------------------------------------------------------------------------
// Name: OnInitialUpdate()
// Desc: When the AppForm object is created, this function is called to
//       initialize it. Here we getting access ptrs to some of the controls,
//       and setting the initial state of some of them as well.
//-----------------------------------------------------------------------------
VOID CAppForm::OnInitialUpdate()
{
    // Update the UI
    CFormView::OnInitialUpdate();

    // Get the top level parent hwnd
    m_hWndTopLevelParent = GetTopLevelParent()->GetSafeHwnd();

    // Save static reference to the render window
    m_hwndRenderWindow = GetDlgItem(IDC_RENDERVIEW)->GetSafeHwnd();

    // Register a class for a fullscreen window
    WNDCLASS wndClass = { CS_HREDRAW | CS_VREDRAW, FullScreenWndProc, 0, 0, NULL,
                          NULL, NULL, (HBRUSH)GetStockObject(WHITE_BRUSH), NULL,
                          _T("Fullscreen Window") };
    RegisterClass( &wndClass );

    // We create the fullscreen window (not visible) at startup, so it can
    // be the focus window.  The focus window can only be set at CreateDevice
    // time, not in a Reset, so ToggleFullscreen wouldn't work unless we have
    // already set up the fullscreen focus window.
    m_hwndRenderFullScreen = CreateWindow( _T("Fullscreen Window"), NULL,
                                           WS_POPUP, CW_USEDEFAULT,
                                           CW_USEDEFAULT, 100, 100,
                                           m_hWndTopLevelParent, 0L, NULL, 0L );

    // Note that for the MFC samples, the device window and focus window
    // are not the same.
    CD3DApplication::m_hWnd = m_hwndRenderWindow;
    CD3DApplication::m_hWndFocus = m_hwndRenderFullScreen;
    CD3DApplication::Create( AfxGetInstanceHandle() );

    // TODO: Update the UI as needed
}


void CAppForm::OnModeAbs() 
{
	m_bYprMode = ABS_CTRL;
}

void CAppForm::OnModeRc() 
{
	m_bYprMode = RT_CTRL;
}

LRESULT CAppFrameWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WM_DEVICECHANGE)
	{
		if (wParam == 0x0007 )
		{
 			bP5Present = P5.P5_Init();
		}
	}

	
	return CFrameWnd::WindowProc(message, wParam, lParam);
}

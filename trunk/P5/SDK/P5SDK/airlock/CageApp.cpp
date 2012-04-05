// CageApp.cpp: implementation of the CCageApp class.
//
//////////////////////////////////////////////////////////////////////
#define STRICT
#include <vector>
#include <iterator>
#include <basetsd.h>
#include <stdio.h>
#include <math.h>
#include <D3DX8.h>
#include <dmusici.h>
#include "dxutil.h"
#include "d3dutil.h"
#include "d3dres.h"
#include "d3dfont.h"
#include "dinput.h"
#include <tchar.h>
#include <time.h>

#include "CageObject.h"
#include "CageApp.h"
#include "Star.h"
#include "Cage.h"
#include "Shadow.h"
#include "Donut.h"
#include "Credits.h"
#include "Starfield.h"
#include "ScoringStar.h"
#include "Explosion.h"
#include "D3DInputWrapper.h"
#include "P5dll.h"
#include "P5motion.h"
#include "P5bend.h"

using std::iterator;
using std::vector;


extern LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

CCageApp g_d3dApp;

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{

    if( FAILED( g_d3dApp.Create( hInst ) ) )
        return 0;

    return g_d3dApp.Run();
}



const int CCageApp::m_numEventSounds =6;
const int CCageApp::m_EVENT_MISS = 0;
const int CCageApp::m_EVENT_CATCH = 1;
const int CCageApp::m_EVENT_WIN = 2;
const int CCageApp::m_EVENT_BEGIN = 3;
const int CCageApp::m_EVENT_DEATH = 4;
const int CCageApp::m_EVENT_THROW = 5;

WCHAR* CCageApp::szEventFiles[] = {L"miss.sgt",L"catch.sgt",L"win_game.sgt",L"begin_game.sgt",L"back_wall.sgt",L"throw.sgt"};



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCageApp::CCageApp() :
	m_nNumCaught(0),
	m_pDMLoader(0),
	m_pDMPerformance(0),
	m_pDMSegment(0),
	m_fRoomXScaleFactor(1.0),
	m_pDMEventSegments(0),
	m_bEnableSound(true),
	m_dCageCatchRadius(10.0),
	m_dCameraSensitivity(1.0),
	m_dGloveSensitivity(1.0),
	m_dGloveAccelerationSensitivity(1.0),
	m_dMouseSensitivity(1.0),
	m_dKeyboardSensitivity(1.0),
	m_nNumNonStarObjects(0),
	m_nShadowMode(0),
	m_pP5(0),
	m_nNumToWin(10),
	m_CameraPosition(0,0,0),
	m_bLevelOver(false),
	m_dElapsedTimeSinceLevelOver(0),
	m_dLevelTransitionSeconds(4),
	m_bDebug(false),
	m_dZAxisMultiplier(1.0),
	m_dStarEntrySpeedMulitplier(5.0),
	m_dStarExitSpeedMulitplier(10.0),
	m_bUseGloveOrientation(false),
	m_bGameOver(false)
{

	m_nCurrentLevel =0;
	m_nNumLevels = 3;



	m_szAmbientLight[0]=0;

    m_dwCreationWidth=800;
    m_dwCreationHeight=600;

	m_fRoomXScaleFactor = 1.3f;
	m_dZAxisMultiplier=2.0;

	m_MaxPosition = D3DXVECTOR3(50*m_fRoomXScaleFactor,50,50*m_dZAxisMultiplier);
	m_MinPosition = D3DXVECTOR3(-50*m_fRoomXScaleFactor,-50,-75);


	m_ShadowMaxPosition = D3DXVECTOR3((m_MaxPosition.x-0.1)*1.3,(m_MaxPosition.y-0.1)*1.3,m_MaxPosition.z-0.1);
	m_ShadowMinPosition = D3DXVECTOR3((m_MinPosition.x+0.1)*1.3,(m_MinPosition.y+0.1)*1.3,m_MinPosition.z+0.1);

	// this will only work if field centered at 0,0,0...
	m_MaxLookAtPosition = D3DXVECTOR3(m_MaxPosition.x*0.5,m_MaxPosition.y*0.5,0);
	m_MinLookAtPosition = D3DXVECTOR3(m_MinPosition.x*0.5,m_MinPosition.y*0.5,0);
	m_LookAtPosition = D3DXVECTOR3(0,0,0);
	m_CameraPosition = D3DXVECTOR3(0,0,-190);

    m_bUseDepthBuffer    = TRUE;

	srand( (unsigned)time( NULL ) );

	rand();

    m_pFont           = new CD3DFont( _T("Arial"), 12, D3DFONT_BOLD );

}

CCageApp::~CCageApp()
{
	if (m_pDMEventSegments)
		delete [] m_pDMEventSegments;

}


//-----------------------------------------------------------------------------
// Name: Create()
// Desc:	Copied from d3dapp.cpp and modified to support a parameterized
//			requested first video mode			
//-----------------------------------------------------------------------------
HRESULT CCageApp::Create( HINSTANCE hInstance )
{
    HRESULT hr;

    // Create the Direct3D object
    m_pD3D = Direct3DCreate8( D3D_SDK_VERSION );
    if( m_pD3D == NULL )
        return DisplayErrorMsg( D3DAPPERR_NODIRECT3D, MSGERR_APPMUSTEXIT );

    // Build a list of Direct3D adapters, modes and devices. The
    // ConfirmDevice() callback is used to confirm that only devices that
    // meet the app's requirements are considered.
    if( FAILED( hr = BuildDeviceList() ) )
    {
        SAFE_RELEASE( m_pD3D );
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    }


	AttemptToSetRequestMode();

    // Unless a substitute hWnd has been specified, create a window to
    // render into
    if( m_hWnd == NULL)
    {
        // Register the windows class
        WNDCLASS wndClass = { 0, WndProc, 0, 0, hInstance,
                              LoadIcon( hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON) ),
                              LoadCursor( NULL, IDC_ARROW ),
                              (HBRUSH)GetStockObject(WHITE_BRUSH),
                              NULL, _T("D3D Window") };
        RegisterClass( &wndClass );

        // Set the window's initial style
        m_dwWindowStyle = WS_POPUP|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|
                          WS_MINIMIZEBOX|WS_VISIBLE;

        // Set the window's initial width
        RECT rc;
        SetRect( &rc, 0, 0, m_dwCreationWidth, m_dwCreationHeight );
        AdjustWindowRect( &rc, m_dwWindowStyle, TRUE );

        // Create the render window
        m_hWnd = CreateWindow( _T("D3D Window"), m_strWindowTitle, m_dwWindowStyle,
                               CW_USEDEFAULT, CW_USEDEFAULT,
                               (rc.right-rc.left), (rc.bottom-rc.top), 0L,
                               LoadMenu( hInstance, MAKEINTRESOURCE(IDR_MENU) ),
                               hInstance, 0L );
    }

    // The focus window can be a specified to be a different window than the
    // device window.  If not, use the device window as the focus window.
    if( m_hWndFocus == NULL )
        m_hWndFocus = m_hWnd;

    // Save window properties
    m_dwWindowStyle = GetWindowLong( m_hWnd, GWL_STYLE );
    GetWindowRect( m_hWnd, &m_rcWindowBounds );
    GetClientRect( m_hWnd, &m_rcWindowClient );

    // Initialize the app's custom scene stuff
    if( FAILED( hr = OneTimeSceneInit() ) )
    {
        SAFE_RELEASE( m_pD3D );
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    }

    // Initialize the 3D environment for the app
    if( FAILED( hr = Initialize3DEnvironment() ) )
    {
        SAFE_RELEASE( m_pD3D );
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    }

    // Setup the app so it can support single-stepping
    DXUtil_Timer( TIMER_START );

    // The app is ready to go
    m_bReady = TRUE;

    return S_OK;
}

bool CCageApp::AttemptToSetRequestMode()
{

	char requestedMode[128];

	::GetPrivateProfileString("Cage","VideoMode","800x600x16",requestedMode,sizeof(requestedMode),".\\cage.ini");

	int h,w,b;
	sscanf(requestedMode,"%dx%dx%d",&w,&h,&b);
	
	m_dwRequestedWidth=w;
	m_dwRequestedHeight=h;
	m_dwRequestedBitDepth=b;


    // Working variables
    //D3DAdapterInfo* pAdapter;
    D3DDeviceInfo*  pDevice;



    // Pick a default device that can render into a window
    // (This code assumes that the HAL device comes before the REF
    // device in the device array).
    for( DWORD a=0; a<m_dwNumAdapters; a++ )
    {
        for( DWORD d=0; d < m_Adapters[a].dwNumDevices; d++ )
        {
			pDevice = &(m_Adapters[a].devices[d]);

			//spin through modes to find correct size, color depth
			for( DWORD m=0; m < pDevice->dwNumModes; m++ )
			{
				DWORD BitDepth = 16;
				if( pDevice->modes[m].Format == D3DFMT_X8R8G8B8 ||
					pDevice->modes[m].Format == D3DFMT_A8R8G8B8 ||
					pDevice->modes[m].Format == D3DFMT_R8G8B8 )
				{
					BitDepth = 32;
				}


				if (pDevice->modes[m].Height==m_dwRequestedHeight &&
					pDevice->modes[m].Width==m_dwRequestedWidth &&
					BitDepth == m_dwRequestedBitDepth)
				{
					pDevice->bWindowed=false;
					pDevice->dwCurrentMode=m;
					m_Adapters[a].dwCurrentDevice = d;
					m_dwAdapter = a;
					m_bWindowed = FALSE;
					return true;
				}
			}
        }
    }

	return false;

}


HRESULT CCageApp::OneTimeSceneInit()                         
{ 
	// read .ini file settings...
	m_nNumStars						= GetProfileInt("NumStars","5");
	m_dInitStarSpeed				= GetProfileDouble("StarSpeed","0.5");
	m_dStarSpeedLevelIncrement		= GetProfileDouble("StarSpeedLevelIncrement","0.025");	
	m_dStarScale					= GetProfileDouble("StarScale","0.125");
	m_dCageScale					= GetProfileDouble("CageScale","0.2");
	m_dClosedCageScale				= GetProfileDouble("ClosedCageScale","0.1");
	m_dGloveSensitivity				= GetProfileDouble("GloveSensitivity","1.0");
	m_dGloveAccelerationSensitivity	= GetProfileDouble("GloveAccelerationSensitivity","1.0");
	m_dMouseSensitivity				= GetProfileDouble("MouseSensitivity","1.0");
	m_dCameraSensitivity			= GetProfileDouble("CameraSensitivity","1.0");	
	m_dKeyboardSensitivity			= GetProfileDouble("KeyboardSensitivity","1.0");
	::GetPrivateProfileString("Cage","AmbientLightColor","0x66666666",m_szAmbientLight,sizeof(m_szAmbientLight),".\\cage.ini");
	m_bEnableSound					= GetProfileInt("EnableSound","1")!=0;
	m_dCageCatchRadius				= GetProfileDouble("CageCatchRadius","10.0");
	m_nShadowMode					= GetProfileInt("ShadowMode","0");
	m_bDebug		  				= GetProfileInt("Debug","1")!=0;
	m_dStarEntrySpeedMulitplier		= GetProfileDouble("StarEntrySpeedMulitplier","5.0");
	m_dStarExitSpeedMulitplier		= GetProfileDouble("StarExitSpeedMulitplier","15.0");
	m_bUseGloveOrientation			= GetProfileInt("UseGloveOrientation","1")!=0;

	
	if (m_bEnableSound)
		InitMusic();

	m_pP5 = new CP5DLL();

	if(m_pP5->P5_Init())
	{
//		RECT region;

		P5Motion_Init(m_pP5, 0);
		P5Bend_Init(m_pP5, 0);

		P5Motion_InvertMouse(P5MOTION_INVERTAXIS, P5MOTION_INVERTAXIS, P5MOTION_INVERTAXIS);
//		GetClipCursor(&region);
//		P5Motion_SetClipRegion(region.left, region.right, region.top, region.bottom, region.top, region.bottom);
		P5Motion_SetClipRegion(-512, 512, -512, 512, -512, 512);


		m_pP5->P5_SetMouseState(0, FALSE);
	}
	else
	{
		m_pP5->P5_Close();
		delete m_pP5;
		m_pP5=0;
	}

	return S_OK; 
}


HRESULT CCageApp::InitMusic()
{
    CoInitialize(NULL);
    
    HRESULT r;
		
	r = CoCreateInstance(CLSID_DirectMusicLoader, NULL, 
                     CLSCTX_INPROC, IID_IDirectMusicLoader8,
                     (void**)&m_pDMLoader);

	if (FAILED(r))
	{
		DisplayDebugMsg("Unable create DirectMuisc Loader");
		return E_FAIL;
	}

	r = CoCreateInstance(CLSID_DirectMusicPerformance, NULL,
                     CLSCTX_INPROC, IID_IDirectMusicPerformance8,
                     (void**)&m_pDMPerformance );

	if (FAILED(r))
	{
		m_pDMLoader->Release(); 
		m_pDMLoader = 0;
		DisplayDebugMsg("Unable create DirectMuisc performance");
		return E_FAIL;
	}


    m_pDMPerformance->InitAudio( 
        NULL,                  // IDirectMusic interface not needed.
        NULL,                  // IDirectSound interface not needed.
        m_hWnd,                  // Window handle.
        //DMUS_APATH_SHARED_STEREOPLUSREVERB,  // Default audiopath type.
        DMUS_APATH_DYNAMIC_STEREO,  // Default audiopath type.
        64,                    // Number of performance channels.
        DMUS_AUDIOF_ALL,       // Features on synthesizer.
        NULL                   // Audio parameters; use defaults.
    );

 
   // Convert to Unicode. 
    WCHAR wstrSearchPath[MAX_PATH];
    MultiByteToWideChar( CP_ACP, 0, ".\\media\\sound", -1, 
                         wstrSearchPath, MAX_PATH );
 
    // Set the search directory.
 
    m_pDMLoader->SetSearchDirectory( 
        GUID_DirectMusicAllTypes,   // Types of files sought.
        wstrSearchPath,             // Where to look.
        FALSE                       // Don't clear object data.
    );


    //WCHAR wstrFileName[MAX_PATH] = L"The Microsoft Sound.wav";
    WCHAR wstrFileName[MAX_PATH] = L"Segment1.sgt";
 
    if (FAILED(m_pDMLoader->LoadObjectFromFile(
        CLSID_DirectMusicSegment,   // Class identifier.
        IID_IDirectMusicSegment8,   // ID of desired interface.
        wstrFileName,               // Filename.
        (LPVOID*) &m_pDMSegment       // Pointer that receives interface.
    )))
    {
		DisplayDebugMsg("Unable create find DirectMuisc file!");
		return E_FAIL;
    }
	if (FAILED(m_pDMSegment->Download( m_pDMPerformance )))
	{
		DisplayDebugMsg("Unable to download DirectMuisc file!");
		return E_FAIL;
	}

    
	m_pDMEventSegments = new IDirectMusicSegment8*[m_numEventSounds];
	for (int i=0; i<m_numEventSounds; i++)
	{
		//WCHAR wstrFileName3[MAX_PATH] = L(szEventFiles[i]);
		if (FAILED(m_pDMLoader->LoadObjectFromFile(
			CLSID_DirectMusicSegment,   // Class identifier.
			IID_IDirectMusicSegment8,   // ID of desired interface.
			szEventFiles[i],               // Filename.
			(LPVOID*) &(m_pDMEventSegments[i])
		)))
		{
			DisplayDebugMsg("Unable to create DirectMuisc file!");
			return E_FAIL;
		}
		if (FAILED(m_pDMEventSegments[i]->Download( m_pDMPerformance )))
		{
			DisplayDebugMsg("Unable to download DirectMuisc file!");
			return E_FAIL;
		}
	}


    m_pDMPerformance->PlaySegment(m_pDMEventSegments[m_EVENT_BEGIN],0,0,NULL);
    m_pDMPerformance->PlaySegment(m_pDMSegment,DMUS_SEGF_QUEUE,0,NULL);

	return S_OK;
}



void CCageApp::PlayEventSound(int index)
{
	if (index<0||index>m_numEventSounds-1)
		return;

	if (m_pDMPerformance && m_pDMEventSegments[index])
	{
		HRESULT r = m_pDMPerformance->PlaySegment(m_pDMEventSegments[index],DMUS_SEGF_SECONDARY,0,NULL);

		if (FAILED(r))
		{
			char msg[256];
			wsprintf(msg,"HRSULT=%d\n",r);
			DisplayDebugMsg(msg);
		}
	}
}



HRESULT CCageApp::FrameMove()                                
{ 

	if( m_pInput->IsKeyDown( DIK_ESCAPE ) )
	{
		if (m_pP5)
		{
			m_pP5->P5_SetMouseState(0,TRUE);
			m_pP5->P5_Close();
			delete m_pP5;
			m_pP5 = 0;
		}
		PostQuitMessage( 0 );
	}

	if( m_pInput->IsKeyDown( DIK_F ) )
		PostMessage(m_hWnd,WM_COMMAND,IDM_TOGGLEFULLSCREEN,0);

	if( m_pInput->IsKeyDown( DIK_D ) )
	{
		PostMessage(m_hWnd,WM_COMMAND,IDM_CHANGEDEVICE,0);
	}




	m_pInput->PollMouse();

	for (std::vector<CCageObject*>::iterator iter = m_CageObjects.begin();
			iter != m_CageObjects.end(); ++iter)
	{
		(*iter)->Update(m_fElapsedTime);
	}

	
	// move camera position if necessary

	D3DXVECTOR3 cagePosition = GetCage()->GetPosition();
	
	if (cagePosition.x>m_MaxPosition.x-1)
	{
		m_LookAtPosition.x += m_dCameraSensitivity*m_fElapsedTime;
	}
	else if (cagePosition.x<m_MinPosition.x+1)
	{
		m_LookAtPosition.x -= m_dCameraSensitivity*m_fElapsedTime;
	}
	if (cagePosition.y>m_MaxPosition.y-1)
	{
		m_LookAtPosition.y += m_dCameraSensitivity*m_fElapsedTime;
	}
	else if (cagePosition.y<m_MinPosition.y+1)
	{
		m_LookAtPosition.y -= m_dCameraSensitivity*m_fElapsedTime;
	}

	CheckLookAtPosition();

	D3DXMatrixLookAtLH( &m_MatView, &m_CameraPosition,
								  &D3DXVECTOR3( m_LookAtPosition.x, m_LookAtPosition.y, m_LookAtPosition.z),
								  &D3DXVECTOR3( 0, 1, 0 ) );
	m_pd3dDevice->SetTransform( D3DTS_VIEW,  &m_MatView );


	LevelCheck();

	return S_OK; 
}

HRESULT CCageApp::Render()                                   
{ 
    // Clear the scene
    m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
                         0x00000000, 1.0f, 0x00000000 );

	D3DXMATRIX matWorld;
	CCageObject *pCageObject;
    if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
    {
		for (std::vector<CCageObject*>::iterator iter = m_CageObjects.begin();
				iter != m_CageObjects.end(); ++iter)
		{

			D3DXMatrixIdentity(&matWorld);
			pCageObject = (*iter);
			pCageObject->GetCurrentMatrix(&matWorld);
		    m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
			pCageObject->RenderCageObject(m_pd3dDevice);
		}

		
		if (m_bDebug)
		{
			char score[512];
			sprintf(score,"Score:%d\n",m_nNumCaught);

//			m_pFont->DrawText(   2,  0, D3DCOLOR_ARGB(255,255,255,0), m_strFrameStats );
			//m_pFont->DrawText(   2, 20, D3DCOLOR_ARGB(255,255,255,0), m_strDeviceStats );
			//m_pFont->DrawText(   2, 40, D3DCOLOR_ARGB(255,255,255,0), score );
		}


        // End the scene.
        m_pd3dDevice->EndScene();

	}




	return S_OK; 
}

void CCageApp::LevelCheck()
{
	if (!m_bLevelOver && m_nNumCaught>=m_nNumToWin && m_nCurrentLevel==(m_nNumLevels-1))
	{
		m_bLevelOver=true;
		m_bGameOver=true;
		GetCredits()->StartCreditSequence();
	}
	if (!m_bGameOver && !m_bLevelOver && m_nNumCaught>=m_nNumToWin)
	{
		// start transition to new level
		m_dElapsedTimeSinceLevelOver=0;
		m_bLevelOver=true;
	}
	else if(!m_bGameOver && m_bLevelOver && m_dElapsedTimeSinceLevelOver>m_dLevelTransitionSeconds)
	{
		// start next level
		m_nCurrentLevel++;
		InitNewLevel();
	}
	else if (!m_bGameOver && m_bLevelOver)
	{
		// continue with level transition
		m_dElapsedTimeSinceLevelOver += m_fElapsedTime;
	}
}

void CCageApp::InitNewLevel()
{
	m_bLevelOver=false;
	m_nNumCaught=0;
	m_dElapsedTimeSinceLevelOver=0.0;
	GetCredits()->TurnOffCreditSequence();

	for (std::vector<CCageObject*>::iterator iter = m_CageObjects.begin();
			iter != m_CageObjects.end(); ++iter)
	{
		(*iter)->InitForNewLevel();
	}
}

void CCageApp::ResetGame()
{
	m_nCurrentLevel=0;
	m_bGameOver=false;
	InitNewLevel();
}



// order in which objects are loaded is important!

const int CCageApp::m_INDEX_CAGE = 0;
const int CCageApp::m_INDEX_SHADOW = 1;
const int CCageApp::m_INDEX_DARKSTAR = 2;
const int CCageApp::m_INDEX_LITESTAR = 3;
const int CCageApp::m_INDEX_STARFIELD= 4;
const int CCageApp::m_INDEX_EXPLOSION=5;
const int CCageApp::m_INDEX_CREDITS=6;


HRESULT CCageApp::InitDeviceObjects()                        
{ 

	m_pInput = new CD3DInputWrapper(m_hWnd);
	m_pInput->InitializeInput();


    CCageObject *pTempCageObject;
	
	// Load the cage
	pTempCageObject = new CCage(this,"cage1",m_dCageScale,m_dClosedCageScale);
	if (FAILED(pTempCageObject->Create(m_pd3dDevice,"media/art/cage05.x")))
	{
		DisplayDebugMsg("Unable to load cage1");
		return E_FAIL;
	}
	m_CageObjects.push_back(pTempCageObject);


	// Load the shadow
	pTempCageObject = new CShadow(this,"shadow",1.00f);
	if (FAILED(pTempCageObject->Create(m_pd3dDevice,"media/art/placement-disk.x")))
		
	{
		DisplayDebugMsg("Unable to load shadow");
		return E_FAIL;
	}
	m_CageObjects.push_back(pTempCageObject);

	// Load the dark star
	pTempCageObject = new CScoringStar(this,"star-lite",0.02f,false);
	if (FAILED(pTempCageObject->Create(m_pd3dDevice,"media/art/star06-lite.x")))
		
	{
		DisplayDebugMsg("Unable to load star-lite");
		return E_FAIL;
	}
	m_CageObjects.push_back(pTempCageObject);

	// Load the lite star
	pTempCageObject = new CScoringStar(this,"star-dark",0.02f,true);
	if (FAILED(pTempCageObject->Create(m_pd3dDevice,"media/art/star06-dark.x")))
		
	{
		DisplayDebugMsg("Unable to load star-dark");
		return E_FAIL;
	}
	m_CageObjects.push_back(pTempCageObject);

	// Load the starfield
	pTempCageObject = new CStarfield(this,"starfield",1);
	if (FAILED(pTempCageObject->Create(m_pd3dDevice,"media/art/disc-starfield3.x")))
		
	{
		DisplayDebugMsg("Unable to load starfield");
		return E_FAIL;
	}
	pTempCageObject->SetPosition(D3DXVECTOR3(0,0,900));
	m_CageObjects.push_back(pTempCageObject);

	// Load the explosion
	pTempCageObject = new CExplosion(this,"explosion",0.1f);
	if (FAILED(pTempCageObject->Create(m_pd3dDevice,"media/art/disc-explosion.x")))
		
	{
		DisplayDebugMsg("Unable to load explosion");
		return E_FAIL;
	}
	pTempCageObject->SetPosition(D3DXVECTOR3(0,0,900));
	m_CageObjects.push_back(pTempCageObject);

	// Load the credits box
	pTempCageObject = new CCredits(this,"credits",0.5f);
	if (FAILED(pTempCageObject->Create(m_pd3dDevice,"media/art/credits_box.x")))
		
	{
		DisplayDebugMsg("Unable to load explosion");
		return E_FAIL;
	}
	pTempCageObject->SetPosition(D3DXVECTOR3(0,0,900));
	m_CageObjects.push_back(pTempCageObject);
	

	// Load the donut

	//////////
	pTempCageObject = new CDonut(this,"donut1",.5,0.025f);
	if (FAILED(pTempCageObject->Create(m_pd3dDevice,"media/art/donut01.x")))		
	{
		DisplayDebugMsg("Unable to load donut1");
		return E_FAIL;
	}
	pTempCageObject->SetPosition(D3DXVECTOR3(0,0,-100));
	//pTempCageObject->SetPosition(D3DXVECTOR3(0,0,250));
	m_CageObjects.push_back(pTempCageObject);


	//////////
	pTempCageObject = new CDonut(this,"donut2",0.7f,-0.05f);
	if (FAILED(pTempCageObject->Create(m_pd3dDevice,"media/art/donut02.x")))		
	{
		DisplayDebugMsg("Unable to load donut2");
		return E_FAIL;
	}
	pTempCageObject->SetPosition(D3DXVECTOR3(0,0,60));
	m_CageObjects.push_back(pTempCageObject);
	
	//////////
	pTempCageObject = new CDonut(this,"donut3",0.7f,0.02f);
	if (FAILED(pTempCageObject->Create(m_pd3dDevice,"media/art/donut03.x")))		
	{
		DisplayDebugMsg("Unable to load donut2");
		return E_FAIL;
	}
	pTempCageObject->SetPosition(D3DXVECTOR3(0,0,150));
	m_CageObjects.push_back(pTempCageObject);

	//////////
	pTempCageObject = new CDonut(this,"donut4",0.7f,-0.0225f);
	if (FAILED(pTempCageObject->Create(m_pd3dDevice,"media/art/donut04.x")))		
	{
		DisplayDebugMsg("Unable to load donut4");
		return E_FAIL;
	}
	pTempCageObject->SetPosition(D3DXVECTOR3(0,0,350));
	m_CageObjects.push_back(pTempCageObject);

	//////////
	pTempCageObject = new CDonut(this,"donut5",0.7f,-0.0275f);
	if (FAILED(pTempCageObject->Create(m_pd3dDevice,"media/art/donut05.x")))		
	{
		DisplayDebugMsg("Unable to load donut5");
		return E_FAIL;
	}
	pTempCageObject->SetPosition(D3DXVECTOR3(10,10,750));
	m_CageObjects.push_back(pTempCageObject);

	//////////
	
	pTempCageObject = new CDonut(this,"donut6",0.5f,-0.0325f);
	if (FAILED(pTempCageObject->Create(m_pd3dDevice,"media/art/donut06.x")))		
	{
		DisplayDebugMsg("Unable to load donut6");
		return E_FAIL;
	}
	pTempCageObject->SetPosition(D3DXVECTOR3(30,30,800));
	m_CageObjects.push_back(pTempCageObject);
		
	
	m_nNumNonStarObjects = m_CageObjects.size();

	for (int i=0; i<m_nNumStars; i++)
	{
	
		// Load the sphere(s)
		pTempCageObject = new CStar(this,"sphere1",m_dStarScale);
		if (FAILED(pTempCageObject->Create(m_pd3dDevice,"media/art/star06-ingame.x")))
		{
			DisplayDebugMsg("Unable to load sphere1");
			return E_FAIL;
		}
		m_CageObjects.push_back(pTempCageObject);
	}
	
    m_pFont->InitDeviceObjects( m_pd3dDevice );


	return S_OK; 
}

HRESULT CCageApp::RestoreDeviceObjects()                     
{ 

	for (std::vector<CCageObject*>::iterator iter = m_CageObjects.begin();
			iter != m_CageObjects.end(); ++iter)
	{
		(*iter)->RestoreDeviceObjects( m_pd3dDevice );
	}


    // Setup render state
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING,     TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,      TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );

    // enable alpha blending
	
	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,  true);
	m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA);
	

	          
	char* stopstring = "\n ";
    m_pd3dDevice->SetRenderState( D3DRS_AMBIENT,     (DWORD) strtoul(m_szAmbientLight,&stopstring,16 ));
    
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );

    // Setup the light
    D3DLIGHT8 light;
    D3DUtil_InitLight( light, D3DLIGHT_DIRECTIONAL, -1.0f,-1.0f, 1.0f );
    m_pd3dDevice->SetLight(0, &light );
    m_pd3dDevice->LightEnable(0, TRUE );

    D3DXMATRIX matProj;
    FLOAT      fAspect = (FLOAT)m_d3dsdBackBuffer.Width / (FLOAT)m_d3dsdBackBuffer.Height;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, fAspect, 
                                1.0,1500.0 );
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );


    m_pFont->RestoreDeviceObjects();

	m_pInput = new CD3DInputWrapper(m_hWnd);
	m_pInput->InitializeInput();



	return S_OK; 
}


HRESULT CCageApp::InvalidateDeviceObjects()                 
{ 

	for (std::vector<CCageObject*>::iterator iter = m_CageObjects.begin();
			iter != m_CageObjects.end(); ++iter)
	{
		(*iter)->InvalidateDeviceObjects();
	}	

	if (m_pInput)
	{
		m_pInput->ShutdownInput();
		delete m_pInput;
		m_pInput=0;
	}



    if (m_pFont)
		m_pFont->InvalidateDeviceObjects();

	return S_OK; 
}

HRESULT CCageApp::DeleteDeviceObjects()                      
{ 

	for (std::vector<CCageObject*>::iterator iter = m_CageObjects.begin();
			iter != m_CageObjects.end(); ++iter)
	{
		delete (*iter);
	}
	m_CageObjects.clear();
			
    if (m_pFont)
	{
		m_pFont->DeleteDeviceObjects();
	}

	if (m_pInput)
	{
		m_pInput->ShutdownInput();
		delete m_pInput;
		m_pInput=0;
	}

	return S_OK; 
}

HRESULT CCageApp::FinalCleanup()                             
{ 

    if (m_pFont)
	{
		delete m_pFont;
		m_pFont = 0;
	}

	// Direct Music Stuff

    if (m_pDMPerformance)
	{
		m_pDMPerformance->Stop(NULL,NULL,0,0);
	    m_pDMPerformance->CloseDown();
	}
 
 
    if (m_pDMLoader)
		m_pDMLoader->Release(); 
    if (m_pDMPerformance)
	    m_pDMPerformance->Release();
    if (m_pDMSegment)
		m_pDMSegment->Release();

	if (m_pDMEventSegments)
	{
		for (int i=0; i<m_numEventSounds; i++)
		{
			if (m_pDMEventSegments[i])
				m_pDMEventSegments[i]->Release();
		}
	}
 
    CoUninitialize();

	if (m_pP5)
	{
		m_pP5->P5_SetMouseState(0,TRUE);
		m_pP5->P5_Close();
		delete m_pP5;
		m_pP5 = 0;
	}
	return S_OK; 
}

CD3DInputWrapper* CCageApp::GetInput()
{
	return m_pInput;
}


void CCageApp::CheckCatches()
{

	bool caughtSome = false;
	CCage *pCage = GetCage();

	if (pCage->IsOpen())
		return;

	std::vector<CCageObject*>::iterator iter = m_CageObjects.begin();

	// skip non-stars at beginning of vector
	for (int i=0;i<m_nNumNonStarObjects; i++)
	{
		iter++;
	}
	
	for ( ; iter != m_CageObjects.end(); ++iter)
	{			
		CStar* pTemp = (CStar*)(*iter);
		if (abs(Distance(&pTemp->GetPosition(),&pCage->GetPosition()))<GetCageCatchRadius() && pTemp->IsInPlay())
		{
			pTemp->SetCaught(true);
			caughtSome=true;
			m_nNumCaught++;
			PlayEventSound(m_EVENT_CATCH);
		}
	}

	if (!caughtSome)
	{
		PlayEventSound(m_EVENT_MISS);
	}
	else if (m_nNumStars==m_nNumCaught)
	{
		PlayEventSound(m_EVENT_WIN);
	}
		


}

CCage* CCageApp::GetCage()
{
	if (m_CageObjects.size()==0)
	{
		DisplayDebugMsg("ERROR: GetCage() called when m_CageObjects.size()==0");
		return NULL;
	}

	return (CCage*)m_CageObjects[m_INDEX_CAGE];
}

CStarfield* CCageApp::GetStarfield()
{
	if (m_CageObjects.size()==0)
	{
		DisplayDebugMsg("ERROR: GetStarfield() called when m_CageObjects.size()==0");
		return NULL;
	}

	return (CStarfield*)m_CageObjects[m_INDEX_STARFIELD];
}

CExplosion* CCageApp::GetExplosion()
{
	if (m_CageObjects.size()==0)
	{
		DisplayDebugMsg("ERROR: GetExplosion() called when m_CageObjects.size()==0");
		return NULL;
	}

	return (CExplosion*)m_CageObjects[m_INDEX_EXPLOSION];
}

CCredits* CCageApp::GetCredits()
{
	if (m_CageObjects.size()==0)
	{
		DisplayDebugMsg("ERROR: GetCredits() called when m_CageObjects.size()==0");
		return NULL;
	}

	return (CCredits*)m_CageObjects[m_INDEX_CREDITS];
}


CShadow* CCageApp::GetShadow()
{
	if (m_CageObjects.size()==0)
	{
		DisplayDebugMsg("ERROR: GetCage() called when m_CageObjects.size()==0");
		return NULL;
	}

	return (CShadow*)m_CageObjects[m_INDEX_SHADOW];
}



void CCageApp::CheckLookAtPosition()
{
	if (m_LookAtPosition.x > m_MaxLookAtPosition.x)
		m_LookAtPosition.x = m_MaxLookAtPosition.x;
	if (m_LookAtPosition.x < m_MinLookAtPosition.x)
		m_LookAtPosition.x = m_MinLookAtPosition.x;

	if (m_LookAtPosition.y > m_MaxLookAtPosition.y)
		m_LookAtPosition.y = m_MaxLookAtPosition.y;
	if (m_LookAtPosition.y < m_MinLookAtPosition.y)
		m_LookAtPosition.y = m_MinLookAtPosition.y;

	if (m_LookAtPosition.z > m_MaxLookAtPosition.z)
		m_LookAtPosition.z = m_MaxLookAtPosition.z;
	if (m_LookAtPosition.z < m_MinLookAtPosition.z)
		m_LookAtPosition.z = m_MinLookAtPosition.z;

}






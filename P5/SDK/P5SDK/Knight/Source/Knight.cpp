//////////////////////////////////////////////////////////////////
//
//  Knight.cpp
//
//  Copyright (c) 2001 Ninjaneering
//	All rights reserved.
//
//
//////////////////////////////////////////////////////////////////


#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <crtdbg.h>

#include "resource.h"
#include "C3DEngine.h"
#include "C3DFrameObject.h"
#include "C3DCamera.h"
#include "KeyboardControl.h"
#include "Glove.h"
#include "p5dll.h"
#include "P5Motion.h"


#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

#define ASSERT _ASSERT

// thought this was defined in winuser.h?
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#endif

#define ZOOM_DELTA		30

extern CP5DLL P5Device;
extern BOOL P5Present;
extern float preset_yaw; 

//////////////////////////////////////////////////////////////////
//
// Global variables

HINSTANCE   g_hInstance;
char        g_szAppName[]			= "Virtual Glove";
char        g_szClassName[]		= "Virtual Glove";
char        g_szTitle[]				= "Virtual Glove";
char				g_szDefaultIni[]	= "glove_knight.ini";
HWND        g_hWnd;
HDC         g_hDC;
HFONT       g_hFont;
HMENU       g_hMenu;
BOOL        g_bActive = FALSE;

// model data
char				g_szKnightFileName[MAX_PATH] = "";
char				g_szRoomFileName[MAX_PATH] = "";
float				g_fKnightScale;
float				g_fRoomScale;

// scene data
float				g_fCameraX, g_fCameraY, g_fCameraZ;
float				g_fCameraLookatX, g_fCameraLookatY, g_fCameraLookatZ;
float				g_fNearPlane, g_fFarPlane;
float				g_fCameraFOV;
float				g_fZoom;

char* sError;

C3DEngine*  g_pEngine = NULL;
C3DObject*  g_pRoomObject = NULL;
C3DFrameObject*  g_pKnightObject = NULL;
KeyboardControl* g_pKeyboardControl = NULL;

char **g_szMenuModelNames;
char **g_szModelFileNames;
int g_iNumModels = 0;

// ini file keys
const char g_szPhysicsControllerType[] = "Phys";
const char g_szAnimationControllerType[] = "Anim";
const char g_sz6DOFControllerType[] = "6DOF";
const char g_szPitchControllerType[] = "Pitch";

//////////////////////////////////////////////////////////////////
//
// Prototypes

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
HRESULT InitInstance(HINSTANCE hInstance, LPSTR lpCmdLine, long nCmdShow, DWORD dwWidth, DWORD dwHeight);
HRESULT InitControllers();
void FreeControllers();
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);


//////////////////////////////////////////////////////////////////

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;


  switch(uMsg) 
  {
  case WM_COMMAND:
	  {
		  // load new model here
		  unsigned int id = LOWORD(wParam);
			if( id > 500 && (int)(id - 500) <= g_iNumModels ) 
			{
			  int index = id - 501;
			  strcpy(g_szKnightFileName,g_szModelFileNames[index]);

				FreeControllers();
				HRESULT rval = InitControllers();
			  if (FAILED(rval))
			  {
					if( ERR_FILENOTFOUND != rval )	// annunciated by InitControllers()
				    DisplayErrorCode(rval);
					exit(rval);
			  }

//				g_pEngine->Resize3DEnvironment();
			}
	  }
	  break;
    case WM_ACTIVATE:
      break;

    case WM_ACTIVATEAPP:
      g_bActive = wParam;
      break;

    case WM_PAINT:
      BeginPaint(hWnd, &ps);
      EndPaint(hWnd, &ps);
      break;

    case WM_CLOSE:
			PostQuitMessage( 0 );
			return 0;

    case WM_KEYDOWN:
      switch( wParam )
      {
        case VK_ESCAPE:
          PostQuitMessage( 0 );
        break;

      }
      break;


    default:
      break;
  }

	// grab keypresses for the keyboardcontrol
	if (WM_CHAR == uMsg)
  {
    TCHAR chCharCode = (TCHAR) wParam;
	if (TCHAR('r') == chCharCode)
	{
	  P5Device.P5_CalibratePositionData( 0 );
	}
	else if (TCHAR('y') == chCharCode)
	{
		preset_yaw+=0.05f;
		if (preset_yaw>1.0f)
			preset_yaw=0.0f;
	}
	else if (TCHAR('u') == chCharCode)
	{
		preset_yaw-=0.05f;
		if (preset_yaw<0.0f)
			preset_yaw=1.0f;
	}

    if (!g_pKeyboardControl->HandleKey(chCharCode))
    {
      if (TCHAR(' ') == chCharCode)
      {
				//toggle full screen mode
				uMsg = WM_COMMAND;
				wParam = IDM_TOGGLEFULLSCREEN;
      }
      else if (TCHAR('z') == chCharCode)
      {
				// zoom in
				uMsg = WM_MOUSEWHEEL;
				wParam = MAKEWPARAM( 0, ZOOM_DELTA );
      }
      else if (TCHAR('x') == chCharCode)
      {
				// zoom out
				uMsg = WM_MOUSEWHEEL;
				wParam = MAKEWPARAM( 0, -ZOOM_DELTA );
      }
      else if (TCHAR('?') == chCharCode)
      {
				//toggle stats
				uMsg = WM_COMMAND;
				wParam = IDM_TOGGLE_STATS;
      }
    }
  }


  // then allow the engine its chance to handle messages (like WM_SIZE)
  if (g_pEngine)
  {
    g_pEngine->HandleMessage(hWnd, uMsg, wParam, lParam);
  }
  
  return DefWindowProc(hWnd, uMsg, wParam, lParam); 

}

//////////////////////////////////////////////////////////////////

HRESULT InitInstance(HINSTANCE hInstance, LPSTR lpCmdLine, long nCmdShow, DWORD dwWidth, DWORD dwHeight)
{
	// grab the ini file name from the command line
	char buf[256] = ".\\";
	if( !lpCmdLine || !strlen(lpCmdLine) )
		strcat( buf, g_szDefaultIni );
	else
		strcat( buf, lpCmdLine );
	
	// load main .ini file and create keyboard control
	const char szGroupName[] = "MODELS";
	g_pKeyboardControl = new KeyboardControl( buf, szGroupName );
	g_iNumModels = g_pKeyboardControl->GetInt( szGroupName, "NumModels", 0 );
	if( g_iNumModels )
	{
		g_szMenuModelNames = new LPSTR[g_iNumModels];
		g_szModelFileNames = new LPSTR[g_iNumModels];
		char buffer[256],mName[256], fName[256];
		for(int i=1; i <= g_iNumModels; i++)
		{
			wsprintf(buffer,"Model%d",i);
			strcpy(mName, g_pKeyboardControl->GetString( buffer, "Name", "?" ) );
			strcpy( fName, ".\\" );
			strcat(fName, g_pKeyboardControl->GetString( buffer, "File", "?" ) );
			g_szMenuModelNames[i-1] = new char[strlen(mName)+1];
			g_szModelFileNames[i-1] = new char[strlen(fName)+1];
			strcpy(g_szMenuModelNames[i-1],mName);
			strcpy(g_szModelFileNames[i-1],fName);
		}

		// select the first in the list
		strcat( g_szKnightFileName, g_szModelFileNames[0] );
	}

	delete g_pKeyboardControl;
	g_pKeyboardControl = NULL;

	// create window
  WNDCLASS wc;
  wc.style         = CS_HREDRAW | CS_VREDRAW; // Class style(s).
  wc.lpfnWndProc   = (WNDPROC)WndProc;        // Window Procedure
  wc.cbClsExtra    = 0;                       // No per-class extra data.
  wc.cbWndExtra    = 0;                       // No per-window extra data.
  wc.hInstance     = hInstance;               // Owner of this class
  wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HAND_ICON)); // Icon name from .RC
  wc.hCursor       = LoadCursor(NULL, IDC_ARROW);// Cursor
  wc.hbrBackground = (HBRUSH)(BLACK_BRUSH); // Default color
  wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MAIN_MENU); // menu name from .RC
  wc.lpszClassName = g_szClassName;            // Name to register as

  if(!RegisterClass(&wc))
		return ERR_INIT;


  // Create a main window for this application instance.
  // adjust the size of the window so that the client size is the requested size
  RECT rcRect;
  SetRect(&rcRect, 0,0,dwWidth, dwHeight);

  DWORD dwStyle = WS_CAPTION | WS_BORDER | WS_THICKFRAME | WS_SYSMENU;
  AdjustWindowRectEx(&rcRect, dwStyle, TRUE,0);

  dwWidth = rcRect.right - rcRect.left;
  dwHeight = rcRect.bottom - rcRect.top;

  // create a window of this new size
  g_hWnd = CreateWindowEx( 0, g_szClassName, g_szTitle, dwStyle, 30, 30,
		dwWidth, dwHeight, NULL, NULL, hInstance, NULL );

  // If window could not be created, return "failure"
  if (! g_hWnd)
  {
	  return(ERR_INIT);
  }


 	if( P5Device.P5_Init()==false )
	{
		sError = "P5 Not found";
		P5Present=false;
	}
	else
	{
		P5Motion_Init( &P5Device, 0 );
		P5Motion_InvertMouse(P5MOTION_INVERTAXIS, P5MOTION_NORMALAXIS, P5MOTION_NORMALAXIS);
		P5Motion_SetClipRegion( (0 - dwWidth), dwWidth, (0 - dwHeight), dwHeight, 0, 1000);

		P5Device.P5_SetMouseState( 0 , FALSE );

		P5Present=true;
	}


  // Make the window visible; update its client area; and return "success"
  ShowWindow (g_hWnd, SW_SHOW); // Show the window
  UpdateWindow(g_hWnd);

  g_hInstance = hInstance;
  g_hMenu = GetMenu(g_hWnd);

	if (g_hMenu)
	{
		if (g_iNumModels > 0)
		{
			HMENU hModelMenu = CreateMenu();
			for(int i=1; i <= g_iNumModels; i++)
			{
				AppendMenu(hModelMenu,MF_STRING,500+i,g_szMenuModelNames[i-1]);
			}
			AppendMenu(g_hMenu,MF_POPUP|MF_STRING,(unsigned int)hModelMenu,"Model");
			DrawMenuBar(g_hWnd);
		}
	}

  return S_OK;              // We succeeded...
}

//////////////////////////////////////////////////////////////////

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  MSG    msg;
  HRESULT rval;

  rval = InitInstance(hInstance, lpCmdLine, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);
  if (rval != S_OK)
  {
		if( ERR_FILENOTFOUND != rval )	// annunciated by InitInstance
	    DisplayErrorCode(rval);
    exit(rval);
  }

  // create the core engine
  g_pEngine = new (_NORMAL_BLOCK, __FILE__, __LINE__)C3DEngine;
  rval = g_pEngine->Init(g_hWnd, g_hInstance, g_szAppName);
  if (rval)
  {
    DisplayErrorCode(rval);
    exit(rval);
  }  

	// initialize controllers
	rval = InitControllers();
  if (rval)
  {
    DisplayErrorCode(rval);
    exit(rval);
  }

  // create a room object
  g_pRoomObject = new C3DFrameObject;
  rval = g_pRoomObject->Init(g_pEngine, g_szRoomFileName );
  if (rval)
  {
    DisplayErrorCode(rval);
    exit(rval);
  }

	// scale it
	D3DXMATRIX mScale, mIdentity;
	D3DXMatrixIdentity( &mIdentity );
	D3DXMatrixScaling( &mScale, g_fRoomScale, g_fRoomScale, g_fRoomScale );
//	g_pRoomObject->SetMatrices( &mIdentity, &mScale );

  //* Orient correctly
  D3DXMATRIX mInitialRot;
  D3DXMatrixRotationYawPitchRoll( &mInitialRot, 3.14159f, -3.14159f/2.0f, 0.0f );
  g_pRoomObject->SetMatrices( &mInitialRot, &mScale );

  // add it to our world
  rval = g_pEngine->AddChild(g_pRoomObject);
  if (rval)
  {
    DisplayErrorCode(rval);
    exit(rval);
  }

	// message pump
  do
  {
	if(P5Present)
	{
		if(P5Device.m_P5Devices[0].m_byButtons[0])
			P5Device.P5_CalibratePositionData( 0 );

		P5Motion_Process();
	}

    if (PeekMessage(&msg, g_hWnd, 0, 0,PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    else
    {

      // idle tasks here...

			// This is where the glove can change values
		  // Read the current values
		  float x = g_pKeyboardControl->GetRawValue("HandX");
		  float y = g_pKeyboardControl->GetRawValue("HandY");
		  float z = g_pKeyboardControl->GetRawValue("HandZ");
		  float yaw = g_pKeyboardControl->GetRawValue("HandYaw");
		  float pitch = g_pKeyboardControl->GetRawValue("HandPitch");
		  float roll = g_pKeyboardControl->GetRawValue("HandRoll");
		  float thumb = g_pKeyboardControl->GetRawValue("ThumbCurl");
		  float index = g_pKeyboardControl->GetRawValue("IndexCurl1");
		  float middle = g_pKeyboardControl->GetRawValue("MiddleCurl1");
		  float ring = g_pKeyboardControl->GetRawValue("RingCurl1");
		  float pinky = g_pKeyboardControl->GetRawValue("PinkyCurl1");
		 
		  // Allow Glove to change values
		  ReadGloveValues(x,y,z,yaw,pitch,roll,thumb,index,middle,ring,pinky);
		 
		  // Write the changes back to the keyboard control
		  g_pKeyboardControl->SetRawValue("HandX",x);
		  g_pKeyboardControl->SetRawValue("HandY",y);
		  g_pKeyboardControl->SetRawValue("HandZ",z);
		  g_pKeyboardControl->SetRawValue("HandYaw",yaw);
		  g_pKeyboardControl->SetRawValue("HandPitch",pitch);
		  g_pKeyboardControl->SetRawValue("HandRoll",roll);
		  g_pKeyboardControl->SetRawValue("ThumbCurl",thumb);
		  g_pKeyboardControl->SetRawValue("ThumbCurl2",thumb);
		  g_pKeyboardControl->SetRawValue("IndexCurl1",index);
		  g_pKeyboardControl->SetRawValue("IndexCurl2",index);
		  g_pKeyboardControl->SetRawValue("IndexCurl3",index);
		  g_pKeyboardControl->SetRawValue("MiddleCurl1",middle);
		  g_pKeyboardControl->SetRawValue("MiddleCurl2",middle);
		  g_pKeyboardControl->SetRawValue("MiddleCurl3",middle);
		  g_pKeyboardControl->SetRawValue("RingCurl1",ring);
		  g_pKeyboardControl->SetRawValue("RingCurl2",ring);
		  g_pKeyboardControl->SetRawValue("RingCurl3",ring);
		  g_pKeyboardControl->SetRawValue("PinkyCurl1",pinky);
		  g_pKeyboardControl->SetRawValue("PinkyCurl2",pinky);
		  g_pKeyboardControl->SetRawValue("PinkyCurl3",pinky);

			// render the scene
			g_pEngine->Render();

      // show the back surface
      g_pEngine->Flip();
    }

  } while (msg.message != WM_QUIT);

	// clean up
	delete g_pKeyboardControl;
	
  // destroy and delete the engine
  g_pEngine->Destroy();
  delete g_pEngine;
  g_pEngine = NULL;

	if(P5Present)
		P5Device.P5_SetMouseState( 0, TRUE );

	for(int n=0; n<g_iNumModels; n++)
	{
		delete g_szMenuModelNames[n];
		delete g_szModelFileNames[n];
	}
  return 0;
}

HRESULT InitControllers()
{
	_ASSERT( !g_pKeyboardControl );
	
	// load model file
	g_pKeyboardControl = new KeyboardControl( g_szKnightFileName, "PUPPET" );

	if( !g_pKeyboardControl->GetNumObjects() )
	{
		char msg[256];
		sprintf( msg, "Could not load %s.", g_szKnightFileName );
		int ok = MessageBox( g_hWnd, msg, "", MB_OKCANCEL );
//		if( IDOK != ok )
			return ERR_FILENOTFOUND;
	}

	// load model data
	strcpy( g_szKnightFileName, g_pKeyboardControl->GetString( "MODEL", "Puppet", "media\\knighttxt.x" ) );
	strcpy( g_szRoomFileName, g_pKeyboardControl->GetString( "MODEL", "Room", "media\\stagetxt.x" ) );
	g_fKnightScale = g_pKeyboardControl->GetFloat( "MODEL", "PuppetScale", 1.0f );
	g_fRoomScale = g_pKeyboardControl->GetFloat( "MODEL", "RoomScale", 1.0f );

	// load scene data
	g_fCameraX = g_pKeyboardControl->GetFloat( "SCENE", "CameraX", 0.0f );
	g_fCameraY = g_pKeyboardControl->GetFloat( "SCENE", "CameraY", 0.0f );
	g_fCameraZ = g_pKeyboardControl->GetFloat( "SCENE", "CameraZ", 1.0f );

	g_fCameraLookatX = g_pKeyboardControl->GetFloat( "SCENE", "LookatX", 0.0f );
	g_fCameraLookatY = g_pKeyboardControl->GetFloat( "SCENE", "LookatY", 0.0f );
	g_fCameraLookatZ = g_pKeyboardControl->GetFloat( "SCENE", "LookatZ", 0.0f );

	g_fNearPlane = g_pKeyboardControl->GetFloat( "SCENE", "NearPlane", 1.0f );
	g_fFarPlane = g_pKeyboardControl->GetFloat( "SCENE", "FarPlane", 30.0f );
	g_fZoom = g_pKeyboardControl->GetFloat( "SCENE", "Zoom", 20.0f );

	g_fCameraFOV = g_pKeyboardControl->GetFloat( "SCENE", "FOVoverPi", 0.25f ) * 3.14159265f;

	// set camera values
	C3DCamera *pCamera = g_pEngine->GetCamera();
	pCamera->SetInitialEyePt( g_fCameraX, g_fCameraY, g_fCameraZ );
	pCamera->SetInitialLookatPt( g_fCameraLookatX, g_fCameraLookatY, g_fCameraLookatZ );

	pCamera->SetFieldOfView( g_fCameraFOV );
	pCamera->SetNearAndFarPlanes( g_fNearPlane, g_fFarPlane );

	g_pEngine->SetZoom( g_fZoom );

	// reset arcball
	g_pEngine->ResetArcBall();

  // create a Knight object
  g_pKnightObject = new C3DFrameObject();
  HRESULT rval = g_pKnightObject->Init(g_pEngine, g_szKnightFileName );
  if (rval)
  {
    DisplayErrorCode(rval);
    exit(rval);
  }

	// scale it
	D3DXMATRIX mScale;
	D3DXMatrixScaling( &mScale, g_fKnightScale, g_fKnightScale, g_fKnightScale );
	D3DXMATRIX mIdentity;
  D3DXMatrixIdentity( &mIdentity );

  //* Orient Knight correctly
  D3DXMATRIX mInitialRot;
  D3DXMatrixRotationYawPitchRoll( &mInitialRot, 3.14159f, -3.14159f/2.0f, 0.0f );
  mInitialRot._42 += 55.0f;

  g_pKnightObject->SetMatrices( &mInitialRot, &mScale );

  // add it to our world
  rval = g_pEngine->AddChild(g_pKnightObject);
  if (rval)
  {
    DisplayErrorCode(rval);
    exit(rval);
  }

	// create text and controllers
	const float fFirstTextLine = UI_FIRST_LINE_Y;
	const float fTextIndent = UI_OFFSET_X;
	const float fTextLineHeight = UI_GROUP_OFFSET_Y;

	float fTextLine = fFirstTextLine;
	C3DFont *pFont = g_pEngine->GetFont();

	int iNumParts = g_pKeyboardControl->GetNumObjects();
	for( int i = 0; i < iNumParts; ++i )
	{
		KeyboardObject *pKBO = g_pKeyboardControl->GetObjectPtr(i);
		ASSERT( pKBO );

		const char *szBoneName = pKBO->GetName();
		ASSERT(szBoneName);

		int iNumValues = pKBO->GetNumValues();
		ASSERT( iNumValues );

		const char *szAnimFileName = pKBO->GetAnimation();

		const char *szControlType = pKBO->GetType();
		if( !strlen( szControlType ) )
		{
			// default to pitch controller if 1 control value
			if( 1 == iNumValues )
				szControlType = g_szPitchControllerType;
			// else default to 6-dof controller if 6 control values
			else if( 6 == iNumValues )
				szControlType = g_sz6DOFControllerType;
			// else default to animation frame controller if anim file name present
//			else if( strlen( szAnimFileName )
//				szControlType = g_szAnimationControllerType;
			else
				return E_FAIL;	// unrecognized controller type
		}

		Controller *pController = NULL;
		C3DText *pText = NULL;

		SIZE size;
		pFont->GetTextExtent("1",&size);

		//
		// create controller of type specified
		//
		// animation frame controller
		if( 0 == stricmp( g_szAnimationControllerType, szControlType ) )
		{
			ASSERT( 1 == iNumValues );
			float *fInput = pKBO->GetValuePtr(0);

			pController = g_pKnightObject->AttachAnimController( szBoneName, fInput );
			if( !pController )
				return E_FAIL;

			// attach text display
			if( strlen( pKBO->GetDisplayText() ) )
			{
				pText = new C3DText(2);
				pText->SetOffset( fTextIndent, fTextLine );
				pText->SetLine( 0, pKBO->GetDisplayText() );
				pText->SetColor(UI_HAND_RED,UI_HAND_GREEN,UI_HAND_BLUE,UI_HAND_ALPHA);
				pController->SetText( pText );
				g_pEngine->AddText( pText );
				fTextLine += pText->GetTextBlockHeight(pFont) + fTextLineHeight;
			}
		}

		// 6-dof controller
		else if( 0 == stricmp( g_sz6DOFControllerType, szControlType ) )
		{
			ASSERT( 6 == iNumValues );
			float *fX = pKBO->GetValuePtr(0);
			float *fY = pKBO->GetValuePtr(1);
			float *fZ = pKBO->GetValuePtr(2);
			float *fRoll = pKBO->GetValuePtr(3);
			float *fPitch = pKBO->GetValuePtr(4);
			float *fYaw = pKBO->GetValuePtr(5);

			pController = g_pKnightObject->Attach6DOFController( szBoneName, fX, fY, fZ, fRoll, fPitch, fYaw );
			if( !pController )
				return E_FAIL;

			// attach text display
			if( strlen( pKBO->GetDisplayText() ) )
			{
				pText = new C3DText(4);
				pText->SetOffset( fTextIndent, fTextLine );
				pText->SetLine( 0, pKBO->GetDisplayText() );
				pText->SetColor(UI_HAND_RED,UI_HAND_GREEN,UI_HAND_BLUE,UI_HAND_ALPHA);
				pController->SetText( pText );
				g_pEngine->AddText( pText );
				fTextLine += pText->GetTextBlockHeight(pFont) + fTextLineHeight;
			}
		}

		// pitch-only controller
		else if( 0 == stricmp( g_szPitchControllerType, szControlType ) )
		{
			ASSERT( 1 == iNumValues );
			float *fPitch = pKBO->GetValuePtr(0);

			pController = g_pKnightObject->Attach6DOFController( szBoneName, NULL, NULL, NULL, NULL, fPitch, NULL );
			if( !pController )
				return E_FAIL;

			// attach text display
			if( strlen( pKBO->GetDisplayText() ) )
			{
				pText = new C3DText(2);
				pText->SetOffset( fTextIndent, fTextLine );
				pText->SetLine( 0, pKBO->GetDisplayText() );
				pText->SetColor(UI_HAND_RED,UI_HAND_GREEN,UI_HAND_BLUE,UI_HAND_ALPHA);
				pController->SetText( pText );
				g_pEngine->AddText( pText );
				fTextLine += pText->GetTextBlockHeight(pFont) + fTextLineHeight;
			}
		}

		// rigid body physics controller
		else if( 0 == stricmp( g_szPhysicsControllerType, szControlType ) )
		{
			// input currently used for debugging
			ASSERT( 1 == iNumValues );
			float *fInput = pKBO->GetValuePtr(0);

			pController = g_pKnightObject->AttachPhysicsController( szBoneName, fInput );
			if( !pController )
				return E_FAIL;
		}
	}
	return S_OK;
}

void FreeControllers()
{
	// free Knight model
	g_pEngine->RemoveChild(g_pKnightObject);
	g_pKnightObject->InvalidateDeviceObjects();
	g_pKnightObject->DeleteDeviceObjects();
	delete g_pKnightObject;
	g_pKnightObject = NULL;

	// free screen text
	g_pEngine->RemoveText();

	// free keyboard control
	delete g_pKeyboardControl;
	g_pKeyboardControl = NULL;
}
//////////////////////////////////////////////////////////////////


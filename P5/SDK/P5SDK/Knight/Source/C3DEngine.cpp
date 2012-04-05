//-----------------------------------------------------------------------------
// File: C3DEngine.cpp
//
// Desc: Application class for the Engine framework library.
//
//-----------------------------------------------------------------------------
#define STRICT
#include "C3DEngine.h"
#include "C3DUtil.h"
#include "C3DFrameObject.h"
#include "Glove.h"
#include "p5dll.h"
#include "P5Motion.h"
#include "resource.h"

// should come with winuser.h?
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL                   0x020A
#endif

//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------
char g_szApplicationName[256];


//-----------------------------------------------------------------------------
// Name: C3DEngine()
// Desc: Constructor
//-----------------------------------------------------------------------------
C3DEngine::C3DEngine() :
  m_dwNumAdapters( 0 ),
  m_dwAdapter( 0L ),
  m_pD3D( NULL ),
  m_pd3dDevice( NULL ),
  m_hWnd( NULL ),
  m_hWndFocus( NULL ),
  m_bActive( FALSE ),
  m_bReady( FALSE ),
  m_dwCreateFlags( 0L ),

  m_bFrameMoving( TRUE ),
  m_bSingleStep( FALSE ),
	m_bShowFrameRate( FALSE ),
  m_fFPS( 0.0f ),

  m_dwCreationWidth( 500 ),
  m_dwCreationHeight( 300 ),
  m_bUseDepthBuffer( FALSE ),
  m_dwMinDepthBits( 16 ),
  m_dwMinStencilBits( 0 ),
  m_bShowCursorWhenFullscreen( FALSE ),

  m_pFont( NULL ),
  m_pCamera( NULL ),
  m_fCameraDistance( 18.0f ),
  m_pCurrentObject( NULL ),
  m_pLastObject( NULL ),
	m_pTextHead( NULL ),
  m_bControlCamera( TRUE ),
  m_pStringVB(0)
{
  m_strDeviceStats[0] = _T('\0');
  m_strFrameStats[0]  = _T('\0');

  m_strLastMeshFile[0] = _T('\0');
  m_strLastMeshPath[0] = _T('\0');

  // Overrides, move these somewhere where the app can change them
  m_bUseDepthBuffer   = TRUE;
  m_bShowCursorWhenFullscreen = TRUE;

  m_dwFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX1;

  m_method = D3DNONINDEXED;
}

//-----------------------------------------------------------------------------
// Name: Init()
// Desc:
//-----------------------------------------------------------------------------
HRESULT C3DEngine::Init( HWND hWnd, HINSTANCE hInstance, char* szApplicationName)
{
  HRESULT hr;

  // save away the application name
  strcpy(g_szApplicationName, szApplicationName);

	if (!hWnd)
	{
		return DisplayErrorMsg( C3DENGINE_ERR_NOWINDOW, MSGERR_APPMUSTEXIT );
	}

  
	// set our window and instance
	m_hWnd = hWnd;
	m_hInstance = hInstance;
    
	// set the display size to the size of the client area of the passed in window
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	m_dwCreationWidth = rc.right - rc.left;
	m_dwCreationHeight = rc.bottom - rc.top;

  // create the child list
  m_pChildList = new List;
	if (!m_pChildList)
	{
		return E_FAIL;
	}



  // Create the Direct3D object
  m_pD3D = Direct3DCreate8( D3D_SDK_VERSION );
  if( m_pD3D == NULL )
    return DisplayErrorMsg( C3DENGINE_ERR_NODIRECT3D, MSGERR_APPMUSTEXIT );

  // Build a list of Direct3D adapters, modes and devices. The
  // ConfirmDevice() callback is used to confirm that only devices that
  // meet the app's requirements are considered.
  if( FAILED( hr = BuildDeviceList() ) )
  {
    SAFE_RELEASE( m_pD3D );
    return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
  }


/*
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
*/
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
  C3DUtil_Timer( TIMER_START );

  // The app is ready to go
  m_bReady = TRUE;

//	if(IDYES == MessageBox(NULL, "Full screen?", "Options", MB_ICONQUESTION | MB_YESNO ))
	ToggleFullscreen();

  return S_OK;
}




//-----------------------------------------------------------------------------
// Name: SortModesCallback()
// Desc: Callback function for sorting display modes (used by BuildDeviceList).
//-----------------------------------------------------------------------------
int SortModesCallback( const VOID* arg1, const VOID* arg2 )
{
  D3DDISPLAYMODE* p1 = (D3DDISPLAYMODE*)arg1;
  D3DDISPLAYMODE* p2 = (D3DDISPLAYMODE*)arg2;

  if( p1->Format > p2->Format )   return -1;
  if( p1->Format < p2->Format )   return +1;
  if( p1->Width  < p2->Width )    return -1;
  if( p1->Width  > p2->Width )    return +1;
  if( p1->Height < p2->Height )   return -1;
  if( p1->Height > p2->Height )   return +1;

  return 0;
}




//-----------------------------------------------------------------------------
// Name: BuildDeviceList()
// Desc:
//-----------------------------------------------------------------------------
HRESULT C3DEngine::BuildDeviceList()
{
    const DWORD dwNumDeviceTypes = 2;
    const TCHAR* strDeviceDescs[] = { _T("HAL"), _T("REF") };
    const D3DDEVTYPE DeviceTypes[] = { D3DDEVTYPE_HAL, D3DDEVTYPE_REF };

    BOOL bHALExists = FALSE;
    BOOL bHALIsWindowedCompatible = FALSE;
    BOOL bHALIsDesktopCompatible = FALSE;
    BOOL bHALIsSampleCompatible = FALSE;

    // Loop through all the adapters on the system (usually, there's just one
    // unless more than one graphics card is present).
    for( UINT iAdapter = 0; iAdapter < m_pD3D->GetAdapterCount(); iAdapter++ )
    {
        // Fill in adapter info
        D3DAdapterInfo* pAdapter  = &m_Adapters[m_dwNumAdapters];
        m_pD3D->GetAdapterIdentifier( iAdapter, 0, &pAdapter->d3dAdapterIdentifier );
        m_pD3D->GetAdapterDisplayMode( iAdapter, &pAdapter->d3ddmDesktop );
        pAdapter->dwNumDevices    = 0;
        pAdapter->dwCurrentDevice = 0;

        // Enumerate all display modes on this adapter
        D3DDISPLAYMODE modes[100];
        D3DFORMAT      formats[20];
        DWORD dwNumFormats      = 0;
        DWORD dwNumModes        = 0;
        DWORD dwNumAdapterModes = m_pD3D->GetAdapterModeCount( iAdapter );

        // Add the adapter's current desktop format to the list of formats
        formats[dwNumFormats++] = pAdapter->d3ddmDesktop.Format;

        for( UINT iMode = 0; iMode < dwNumAdapterModes; iMode++ )
        {
            // Get the display mode attributes
            D3DDISPLAYMODE DisplayMode;
            m_pD3D->EnumAdapterModes( iAdapter, iMode, &DisplayMode );

            // Filter out low-resolution modes
            if( DisplayMode.Width  < 640 || DisplayMode.Height < 400 )
                continue;

            // Check if the mode already exists (to filter out refresh rates)
            for( DWORD m=0L; m<dwNumModes; m++ )
            {
                if( ( modes[m].Width  == DisplayMode.Width  ) &&
                    ( modes[m].Height == DisplayMode.Height ) &&
                    ( modes[m].Format == DisplayMode.Format ) )
                    break;
            }

            // If we found a new mode, add it to the list of modes
            if( m == dwNumModes )
            {
                modes[dwNumModes].Width       = DisplayMode.Width;
                modes[dwNumModes].Height      = DisplayMode.Height;
                modes[dwNumModes].Format      = DisplayMode.Format;
                modes[dwNumModes].RefreshRate = 0;
                dwNumModes++;

                // Check if the mode's format already exists
                for( DWORD f=0; f<dwNumFormats; f++ )
                {
                    if( DisplayMode.Format == formats[f] )
                        break;
                }

                // If the format is new, add it to the list
                if( f== dwNumFormats )
                    formats[dwNumFormats++] = DisplayMode.Format;
            }
        }

        // Sort the list of display modes (by format, then width, then height)
        qsort( modes, dwNumModes, sizeof(D3DDISPLAYMODE), SortModesCallback );

        // Add devices to adapter
        for( UINT iDevice = 0; iDevice < dwNumDeviceTypes; iDevice++ )
        {
            // Fill in device info
            D3DDeviceInfo* pDevice;
            pDevice                 = &pAdapter->devices[pAdapter->dwNumDevices];
            pDevice->DeviceType     = DeviceTypes[iDevice];
            m_pD3D->GetDeviceCaps( iAdapter, DeviceTypes[iDevice], &pDevice->d3dCaps );
            pDevice->strDesc        = strDeviceDescs[iDevice];
            pDevice->dwNumModes     = 0;
            pDevice->dwCurrentMode  = 0;
            pDevice->bCanDoWindowed = FALSE;
            pDevice->bWindowed      = FALSE;
            pDevice->MultiSampleType = D3DMULTISAMPLE_NONE;

            // Examine each format supported by the adapter to see if it will
            // work with this device and meets the needs of the application.
            BOOL  bFormatConfirmed[20];
            DWORD dwBehavior[20];
            D3DFORMAT fmtDepthStencil[20];

            for( DWORD f=0; f<dwNumFormats; f++ )
            {
                bFormatConfirmed[f] = FALSE;
                fmtDepthStencil[f] = D3DFMT_UNKNOWN;

                // Skip formats that cannot be used as render targets on this device
                if( FAILED( m_pD3D->CheckDeviceType( iAdapter, pDevice->DeviceType,
                                                     formats[f], formats[f], FALSE ) ) )
                    continue;

                if( pDevice->DeviceType == D3DDEVTYPE_HAL )
                {
                    // This system has a HAL device
                    bHALExists = TRUE;

                    if( pDevice->d3dCaps.Caps2 & D3DCAPS2_CANRENDERWINDOWED )
                    {
                        // HAL can run in a window for some mode
                        bHALIsWindowedCompatible = TRUE;

                        if( f == 0 )
                        {
                            // HAL can run in a window for the current desktop mode
                            bHALIsDesktopCompatible = TRUE;
                        }
                    }
                }

                // Confirm the device/format for HW vertex processing
                if( pDevice->d3dCaps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT )
                {
                    if( pDevice->d3dCaps.DevCaps&D3DDEVCAPS_PUREDEVICE )
                    {
                        dwBehavior[f] = D3DCREATE_HARDWARE_VERTEXPROCESSING |
                                        D3DCREATE_PUREDEVICE;

                        if( SUCCEEDED( ConfirmDevice( &pDevice->d3dCaps, dwBehavior[f],
                                                      formats[f] ) ) )
                            bFormatConfirmed[f] = TRUE;
                    }

                    if ( FALSE == bFormatConfirmed[f] )
                    {
                        dwBehavior[f] = D3DCREATE_HARDWARE_VERTEXPROCESSING;

                        if( SUCCEEDED( ConfirmDevice( &pDevice->d3dCaps, dwBehavior[f],
                                                      formats[f] ) ) )
                            bFormatConfirmed[f] = TRUE;
                    }

                    if ( FALSE == bFormatConfirmed[f] )
                    {
                        dwBehavior[f] = D3DCREATE_MIXED_VERTEXPROCESSING;

                        if( SUCCEEDED( ConfirmDevice( &pDevice->d3dCaps, dwBehavior[f],
                                                      formats[f] ) ) )
                            bFormatConfirmed[f] = TRUE;
                    }
                }

                // Confirm the device/format for SW vertex processing
                if( FALSE == bFormatConfirmed[f] )
                {
                    dwBehavior[f] = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

                    if( SUCCEEDED( ConfirmDevice( &pDevice->d3dCaps, dwBehavior[f],
                                                  formats[f] ) ) )
                        bFormatConfirmed[f] = TRUE;
                }

                // Find a suitable depth/stencil buffer format for this device/format
                if( bFormatConfirmed[f] && m_bUseDepthBuffer )
                {
                    if( !FindDepthStencilFormat( iAdapter, pDevice->DeviceType,
                        formats[f], &fmtDepthStencil[f] ) )
                    {
                        bFormatConfirmed[f] = FALSE;
                    }
                }
            }

            // Add all enumerated display modes with confirmed formats to the
            // device's list of valid modes
            for( DWORD m=0L; m<dwNumModes; m++ )
            {
                for( DWORD f=0; f<dwNumFormats; f++ )
                {
                    if( modes[m].Format == formats[f] )
                    {
                        if( bFormatConfirmed[f] == TRUE )
                        {
                            // Add this mode to the device's list of valid modes
                            pDevice->modes[pDevice->dwNumModes].Width      = modes[m].Width;
                            pDevice->modes[pDevice->dwNumModes].Height     = modes[m].Height;
                            pDevice->modes[pDevice->dwNumModes].Format     = modes[m].Format;
                            pDevice->modes[pDevice->dwNumModes].dwBehavior = dwBehavior[f];
                            pDevice->modes[pDevice->dwNumModes].DepthStencilFormat = fmtDepthStencil[f];
                            pDevice->dwNumModes++;

                            if( pDevice->DeviceType == D3DDEVTYPE_HAL )
                                bHALIsSampleCompatible = TRUE;
                        }
                    }
                }
            }

            // Select any 800x600 mode for default (but prefer a 16-bit mode)
            for( m=0; m<pDevice->dwNumModes; m++ )
            {
                if( pDevice->modes[m].Width==800 && pDevice->modes[m].Height==600 )
                {
                    pDevice->dwCurrentMode = m;
                    if( pDevice->modes[m].Format == D3DFMT_R5G6B5 ||
                        pDevice->modes[m].Format == D3DFMT_X1R5G5B5 ||
                        pDevice->modes[m].Format == D3DFMT_A1R5G5B5 )
                    {
                        break;
                    }
                }
            }

            // Check if the device is compatible with the desktop display mode
            // (which was added initially as formats[0])
            if( bFormatConfirmed[0] && (pDevice->d3dCaps.Caps2 & D3DCAPS2_CANRENDERWINDOWED) )
            {
                pDevice->bCanDoWindowed = TRUE;
                pDevice->bWindowed      = TRUE;
            }

            // If valid modes were found, keep this device
            if( pDevice->dwNumModes > 0 )
                pAdapter->dwNumDevices++;
        }

        // If valid devices were found, keep this adapter
        if( pAdapter->dwNumDevices > 0 )
            m_dwNumAdapters++;
    }

    // Return an error if no compatible devices were found
    if( 0L == m_dwNumAdapters )
        return C3DENGINE_ERR_NOCOMPATIBLEDEVICES;

    // Pick a default device that can render into a window
    // (This code assumes that the HAL device comes before the REF
    // device in the device array).
    for( DWORD a=0; a<m_dwNumAdapters; a++ )
    {
        for( DWORD d=0; d < m_Adapters[a].dwNumDevices; d++ )
        {
            if( m_Adapters[a].devices[d].bWindowed )
            {
                m_Adapters[a].dwCurrentDevice = d;
                m_dwAdapter = a;
                m_bWindowed = TRUE;

                // Display a warning message
                if( m_Adapters[a].devices[d].DeviceType == D3DDEVTYPE_REF )
                {
                    if( !bHALExists )
                        DisplayErrorMsg( C3DENGINE_ERR_NOHARDWAREDEVICE, MSGWARN_SWITCHEDTOREF );
                    else if( !bHALIsSampleCompatible )
                        DisplayErrorMsg( C3DENGINE_ERR_HALNOTCOMPATIBLE, MSGWARN_SWITCHEDTOREF );
                    else if( !bHALIsWindowedCompatible )
                        DisplayErrorMsg( C3DENGINE_ERR_NOWINDOWEDHAL, MSGWARN_SWITCHEDTOREF );
                    else if( !bHALIsDesktopCompatible )
                        DisplayErrorMsg( C3DENGINE_ERR_NODESKTOPHAL, MSGWARN_SWITCHEDTOREF );
                    else // HAL is desktop compatible, but not sample compatible
                        DisplayErrorMsg( C3DENGINE_ERR_NOHALTHISMODE, MSGWARN_SWITCHEDTOREF );
                }

                return S_OK;
            }
        }
    }

    return C3DENGINE_ERR_NOWINDOWABLEDEVICES;
}




//-----------------------------------------------------------------------------
// Name: FindDepthStencilFormat()
// Desc: Finds a depth/stencil format for the given device that is compatible
//       with the render target format and meets the needs of the app.
//-----------------------------------------------------------------------------
BOOL C3DEngine::FindDepthStencilFormat( UINT iAdapter, D3DDEVTYPE DeviceType,
    D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat )
{
    if( m_dwMinDepthBits <= 16 && m_dwMinStencilBits == 0 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D16 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D16;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 15 && m_dwMinStencilBits <= 1 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D15S1 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D15S1 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D15S1;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits == 0 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D24X8 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D24X8;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits <= 8 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D24S8 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D24S8;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits <= 4 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X4S4 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D24X4S4 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D24X4S4;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 32 && m_dwMinStencilBits == 0 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D32 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D32;
                return TRUE;
            }
        }
    }

    return FALSE;
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Message handling function.
//-----------------------------------------------------------------------------
LRESULT C3DEngine::HandleMessage( HWND hWnd, UINT uMsg, WPARAM wParam,
                                  LPARAM lParam )
{
  HRESULT hr;
  // control camera
  if( m_bControlCamera )
  {
    // Pass mouse messages to the camera ArcBall so it can build internal matrices
    m_ArcBall.HandleMouseMessages( hWnd, uMsg, wParam, lParam );
  }
  else
  {
    // Pass mouse messages to the object ArcBall so it can build internal matrices
    m_ObjectArcBall.HandleMouseMessages( hWnd, uMsg, wParam, lParam );
  }


  switch( uMsg )
  {
    case WM_PAINT:
      // Handle paint messages when the app is not ready
      if( m_pd3dDevice && !m_bReady )
      {
        if( m_bWindowed )
          Flip();
      }
      break;

    case WM_GETMINMAXINFO:
      ((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
      ((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
      break;

    case WM_ENTERSIZEMOVE:
      // Halt frame movement while the app is sizing or moving
      if( m_bFrameMoving )
        C3DUtil_Timer( TIMER_STOP );
      break;

    case WM_SIZE:
      // Check to see if we are losing our window...
      if( SIZE_MAXHIDE==wParam || SIZE_MINIMIZED==wParam )
        m_bActive = FALSE;
      else
        m_bActive = TRUE;
      break;

    case WM_EXITSIZEMOVE:
      if( m_bFrameMoving )
        C3DUtil_Timer( TIMER_START );

      if( m_bActive && m_bWindowed )
      {
        RECT rcClientOld;
        rcClientOld = m_rcWindowClient;

        // Update window properties
        GetWindowRect( m_hWnd, &m_rcWindowBounds );
        GetClientRect( m_hWnd, &m_rcWindowClient );

        if( ( rcClientOld.right - rcClientOld.left != m_rcWindowClient.right - m_rcWindowClient.left ) ||
            ( rcClientOld.bottom - rcClientOld.top != m_rcWindowClient.bottom - m_rcWindowClient.top ) )
        {
          // A new window size will require a new backbuffer
          // size, so the 3D structures must be changed accordingly.
          m_bReady = FALSE;

          m_d3dpp.BackBufferWidth  = m_rcWindowClient.right - m_rcWindowClient.left;
          m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;

          // Resize the 3D environment
          if( FAILED( hr = Resize3DEnvironment() ) )
          {
            DisplayErrorMsg( C3DENGINE_ERR_RESIZEFAILED, MSGERR_APPMUSTEXIT );
            return 0;
          }

          m_bReady = TRUE;
        }
      }
      break;

    case WM_SETCURSOR:
      // Turn off Windows cursor in fullscreen mode
      if( m_bActive && m_bReady && !m_bWindowed )
      {
        SetCursor( NULL );
        if( m_bShowCursorWhenFullscreen )
          m_pd3dDevice->ShowCursor( TRUE );
        return TRUE; // prevent Windows from setting cursor to window class cursor
      }
      break;

    case WM_MOUSEMOVE:
      if( m_bActive && m_bReady && m_pd3dDevice != NULL )
      {
        POINT ptCursor;
        GetCursorPos( &ptCursor );
        ScreenToClient( m_hWnd, &ptCursor );
        m_pd3dDevice->SetCursorPosition( ptCursor.x, ptCursor.y, 0L );
      }
      break;

      
    case WM_MOUSEWHEEL:
      if( m_bActive && m_bReady && m_pd3dDevice && m_pCamera )
      {
        short zDelta = (short) HIWORD(wParam);    // wheel rotation
        if( abs( zDelta ) < 1000 )
          m_fCameraDistance -= ( 0.01f * zDelta ); // positive is forward
        else
          m_fCameraDistance = 18.0f;
      }
      break;

    case WM_ENTERMENULOOP:
      // Pause the app when menus are displayed
      Pause(TRUE);
      break;

    case WM_EXITMENULOOP:
      Pause(FALSE);
      break;

    case WM_CONTEXTMENU:
      // No context menus allowed in fullscreen mode
      if( m_bWindowed == FALSE )
        break;

      // Handle the app's context menu (via right mouse click)
//      TrackPopupMenuEx( GetSubMenu( LoadMenu( 0, MAKEINTRESOURCE(IDR_POPUP) ), 0 ),
//      TPM_VERTICAL, LOWORD(lParam), HIWORD(lParam), hWnd, NULL );
      break;

      case WM_NCHITTEST:
      // Prevent the user from selecting the menu in fullscreen mode
      if( !m_bWindowed )
        return HTCLIENT;

      break;

    case WM_POWERBROADCAST:
      switch( wParam )
      {
#ifndef PBT_APMQUERYSUSPEND
#define PBT_APMQUERYSUSPEND 0x0000
#endif
        case PBT_APMQUERYSUSPEND:
          // At this point, the app should save any data for open
          // network connections, files, etc., and prepare to go into
          // a suspended mode.
          return TRUE;

#ifndef PBT_APMRESUMESUSPEND
#define PBT_APMRESUMESUSPEND 0x0007
#endif
        case PBT_APMRESUMESUSPEND:
          // At this point, the app should recover any data, network
          // connections, files, etc., and resume running from when
          // the app was suspended.
          return TRUE;
      }
      break;

    case WM_SYSCOMMAND:
      // Prevent moving/sizing and power loss in fullscreen mode
      switch( wParam )
      {
        case SC_MOVE:
        case SC_SIZE:
        case SC_MAXIMIZE:
        case SC_KEYMENU:
        case SC_MONITORPOWER:
        if( FALSE == m_bWindowed )
          return 1;
        break;
      }
      break;

    case WM_COMMAND:
      switch( LOWORD(wParam) )
      {
/*
				case IDM_CONTROL_CAMERA:
          // Toggle camera movement
          m_bControlCamera = TRUE;
          CheckMenuItem(GetMenu(hWnd), IDM_CONTROL_CAMERA, MF_CHECKED);
          CheckMenuItem(GetMenu(hWnd), IDM_CONTROL_OBJECT, MF_UNCHECKED);
          break;

        case IDM_CONTROL_OBJECT:
          // Toggle object movement
          m_bControlCamera = FALSE;
          CheckMenuItem(GetMenu(hWnd), IDM_CONTROL_OBJECT, MF_CHECKED);
          CheckMenuItem(GetMenu(hWnd), IDM_CONTROL_CAMERA, MF_UNCHECKED);
          break;

        case IDM_TOGGLESTART:
          // Toggle frame movement
          m_bFrameMoving = !m_bFrameMoving;
          C3DUtil_Timer( m_bFrameMoving ? TIMER_START : TIMER_STOP );
          break;

        case IDM_SINGLESTEP:
          // Single-step frame movement
          if( FALSE == m_bFrameMoving )
            C3DUtil_Timer( TIMER_ADVANCE );
          else
            C3DUtil_Timer( TIMER_STOP );
          m_bFrameMoving = FALSE;
          m_bSingleStep  = TRUE;
          break;
*/
        case IDM_CHANGEDEVICE:
          // Prompt the user to select a new device or mode
          if( m_bActive && m_bReady )
          {
            Pause(TRUE);
            if( FAILED( hr = UserSelectNewDevice() ) )
              return 0;
            Pause(FALSE);
          }
          return 0;
/*
        case ID_FILE_OPENMESHHEIRARCHY:
          // Open a .x file
          if( m_bActive && m_bReady )
          {
            Pause(TRUE);

            hr = UserSelectMesh();

            Pause(FALSE);
          }
          return 0;
*/
        case IDM_TOGGLEFULLSCREEN:
          // Toggle the fullscreen/window mode
          if( m_bActive && m_bReady )
          {
            if( FAILED( ToggleFullscreen() ) )
            {
              DisplayErrorMsg( C3DENGINE_ERR_RESIZEFAILED, MSGERR_APPMUSTEXIT );
              return 0;
            }
          }
          return 0;

        case IDM_TOGGLE_STATS:
          // Toggle the frame rate and device stats
					ToggleFrameRate();
          return 0;
/*
        case ID_OPTIONS_D3DINDEXED:
          CheckMenuItem(GetMenu(hWnd), ID_OPTIONS_D3DINDEXED, MF_CHECKED);
          CheckMenuItem(GetMenu(hWnd), ID_OPTIONS_D3DNONINDEXED, MF_UNCHECKED);
          CheckMenuItem(GetMenu(hWnd), ID_OPTIONS_SOFTWARESKINNING, MF_UNCHECKED);
          m_method = D3DINDEXED;
          break;

        case ID_OPTIONS_D3DNONINDEXED:

          CheckMenuItem(GetMenu(hWnd), ID_OPTIONS_D3DINDEXED, MF_UNCHECKED);
          CheckMenuItem(GetMenu(hWnd), ID_OPTIONS_D3DNONINDEXED, MF_CHECKED);
          CheckMenuItem(GetMenu(hWnd), ID_OPTIONS_SOFTWARESKINNING, MF_UNCHECKED);
          m_method = D3DNONINDEXED;
          break;

        case ID_OPTIONS_SOFTWARESKINNING:

          CheckMenuItem(GetMenu(hWnd), ID_OPTIONS_D3DINDEXED, MF_UNCHECKED);
          CheckMenuItem(GetMenu(hWnd), ID_OPTIONS_D3DNONINDEXED, MF_UNCHECKED);
          CheckMenuItem(GetMenu(hWnd), ID_OPTIONS_SOFTWARESKINNING, MF_CHECKED);
          m_method = SOFTWARE;
          break;
*/
        case IDM_FILE_EXIT:
          // Recieved key/menu command to exit app
          PostQuitMessage(0);
          return 0;
      }
      break;
    }
	return 1;
    //return DefWindowProc( hWnd, uMsg, wParam, lParam );
}




//-----------------------------------------------------------------------------
// Name: Initialize3DEnvironment()
// Desc:
//-----------------------------------------------------------------------------
HRESULT C3DEngine::Initialize3DEnvironment()
{
    HRESULT hr;

    D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
    D3DDeviceInfo*  pDeviceInfo  = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
    D3DModeInfo*    pModeInfo    = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];

    // Prepare window for possible windowed/fullscreen change
    AdjustWindowForChange();

    // Set up the presentation parameters
    ZeroMemory( &m_d3dpp, sizeof(m_d3dpp) );
    m_d3dpp.Windowed               = pDeviceInfo->bWindowed;
    m_d3dpp.BackBufferCount        = 1;
    m_d3dpp.MultiSampleType        = pDeviceInfo->MultiSampleType;
    m_d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    m_d3dpp.EnableAutoDepthStencil = m_bUseDepthBuffer;
    m_d3dpp.AutoDepthStencilFormat = pModeInfo->DepthStencilFormat;
    m_d3dpp.hDeviceWindow          = m_hWnd;
    if( m_bWindowed )
    {
        m_d3dpp.BackBufferWidth  = m_rcWindowClient.right - m_rcWindowClient.left;
        m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;
        m_d3dpp.BackBufferFormat = pAdapterInfo->d3ddmDesktop.Format;
    }
    else
    {
        m_d3dpp.BackBufferWidth  = pModeInfo->Width;
        m_d3dpp.BackBufferHeight = pModeInfo->Height;
        m_d3dpp.BackBufferFormat = pModeInfo->Format;
    }

    // Create the device
    hr = m_pD3D->CreateDevice( m_dwAdapter, pDeviceInfo->DeviceType,
                               m_hWndFocus, pModeInfo->dwBehavior, &m_d3dpp,
                               &m_pd3dDevice );
    if( SUCCEEDED(hr) )
    {
        // When moving from fullscreen to windowed mode, it is important to
        // adjust the window size after recreating the device rather than
        // beforehand to ensure that you get the window size you want.  For
        // example, when switching from 640x480 fullscreen to windowed with
        // a 1000x600 window on a 1024x768 desktop, it is impossible to set
        // the window size to 1000x600 until after the display mode has
        // changed to 1024x768, because windows cannot be larger than the
        // desktop.
        if( m_bWindowed )
        {
            SetWindowPos( m_hWnd, HWND_NOTOPMOST,
                          m_rcWindowBounds.left, m_rcWindowBounds.top,
                          ( m_rcWindowBounds.right - m_rcWindowBounds.left ),
                          ( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
                          SWP_SHOWWINDOW );
        }

        // Store device Caps
        m_pd3dDevice->GetDeviceCaps( &m_d3dCaps );
        m_dwCreateFlags = pModeInfo->dwBehavior;

        // Store device description
        if( pDeviceInfo->DeviceType == D3DDEVTYPE_REF )
            lstrcpy( m_strDeviceStats, TEXT("REF") );
        else if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
            lstrcpy( m_strDeviceStats, TEXT("HAL") );
        else if( pDeviceInfo->DeviceType == D3DDEVTYPE_SW )
            lstrcpy( m_strDeviceStats, TEXT("SW") );

        if( pModeInfo->dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING &&
            pModeInfo->dwBehavior & D3DCREATE_PUREDEVICE )
        {
            if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
                lstrcat( m_strDeviceStats, TEXT(" (pure hw vp)") );
            else
                lstrcat( m_strDeviceStats, TEXT(" (simulated pure hw vp)") );
        }
        else if( pModeInfo->dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING )
        {
            if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
                lstrcat( m_strDeviceStats, TEXT(" (hw vp)") );
            else
                lstrcat( m_strDeviceStats, TEXT(" (simulated hw vp)") );
        }
        else if( pModeInfo->dwBehavior & D3DCREATE_MIXED_VERTEXPROCESSING )
        {
            if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
                lstrcat( m_strDeviceStats, TEXT(" (mixed vp)") );
            else
                lstrcat( m_strDeviceStats, TEXT(" (simulated mixed vp)") );
        }
        else if( pModeInfo->dwBehavior & D3DCREATE_SOFTWARE_VERTEXPROCESSING )
        {
            lstrcat( m_strDeviceStats, TEXT(" (sw vp)") );
        }

        if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
        {
            lstrcat( m_strDeviceStats, TEXT(": ") );
            lstrcat( m_strDeviceStats, pAdapterInfo->d3dAdapterIdentifier.Description );
        }

        // Store render target surface desc
        LPDIRECT3DSURFACE8 pBackBuffer;
        m_pd3dDevice->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
        pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
        pBackBuffer->Release();

        // Set up the fullscreen cursor
        if( m_bShowCursorWhenFullscreen && !m_bWindowed )
        {
            HCURSOR hCursor;
#ifdef _WIN64
            hCursor = (HCURSOR)GetClassLongPtr( m_hWnd, GCLP_HCURSOR );
#else
            hCursor = (HCURSOR)GetClassLong( m_hWnd, GCL_HCURSOR );
#endif
            C3DUtil_SetDeviceCursor( m_pd3dDevice, hCursor );
            m_pd3dDevice->ShowCursor( TRUE );
        }

        // Initialize the app's device-dependent objects
        hr = InitDeviceObjects();
        if( SUCCEEDED(hr) )
        {
            hr = RestoreDeviceObjects();
            if( SUCCEEDED(hr) )
            {
              m_bActive = TRUE;
              return S_OK;
            }
        }

        // Cleanup before we try again
        InvalidateDeviceObjects();
        DeleteDeviceObjects();
        SAFE_RELEASE( m_pd3dDevice );
    }

    // If that failed, fall back to the reference rasterizer
    if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
    {
        // Let the user know we are switching from HAL to the reference rasterizer
        DisplayErrorMsg( hr, MSGWARN_SWITCHEDTOREF );

        // Select the default adapter
        m_dwAdapter = 0L;
        pAdapterInfo = &m_Adapters[m_dwAdapter];

        // Look for a software device
        for( UINT i=0L; i<pAdapterInfo->dwNumDevices; i++ )
        {
            if( pAdapterInfo->devices[i].DeviceType == D3DDEVTYPE_REF )
            {
                pAdapterInfo->dwCurrentDevice = i;
                pDeviceInfo = &pAdapterInfo->devices[i];
                m_bWindowed = pDeviceInfo->bWindowed;
                break;
            }
        }

        // Try again, this time with the reference rasterizer
        if( pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice].DeviceType ==
            D3DDEVTYPE_REF )
        {
            hr = Initialize3DEnvironment();
        }
    }
    return hr;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT C3DEngine::Resize3DEnvironment()
{
    HRESULT hr;

    // Release all vidmem objects
    if( FAILED( hr = InvalidateDeviceObjects() ) )
        return hr;

    // Reset the device
    if( FAILED( hr = m_pd3dDevice->Reset( &m_d3dpp ) ) )
        return hr;

    // Store render target surface desc
    LPDIRECT3DSURFACE8 pBackBuffer;
    m_pd3dDevice->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
    pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
    pBackBuffer->Release();

    // Set up the fullscreen cursor
    if( m_bShowCursorWhenFullscreen && !m_bWindowed )
    {
        HCURSOR hCursor;
#ifdef _WIN64
        hCursor = (HCURSOR)GetClassLongPtr( m_hWnd, GCLP_HCURSOR );
#else
        hCursor = (HCURSOR)GetClassLong( m_hWnd, GCL_HCURSOR );
#endif
        C3DUtil_SetDeviceCursor( m_pd3dDevice, hCursor );
        m_pd3dDevice->ShowCursor( TRUE );
    }

    // Initialize the app's device-dependent objects
    hr = RestoreDeviceObjects();
    if( FAILED(hr) )
        return hr;

    // If the app is paused, trigger the rendering of the current frame
    if( FALSE == m_bFrameMoving )
    {
        m_bSingleStep = TRUE;
        C3DUtil_Timer( TIMER_START );
        C3DUtil_Timer( TIMER_STOP );
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ToggleFullScreen()
// Desc: Called when user toggles between fullscreen mode and windowed mode
//-----------------------------------------------------------------------------
HRESULT C3DEngine::ToggleFullscreen()
{
    // Get access to current adapter, device, and mode
    D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
    D3DDeviceInfo*  pDeviceInfo  = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
    D3DModeInfo*    pModeInfo    = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];

    // Need device change if going windowed and the current device
    // can only be fullscreen
    if( !m_bWindowed && !pDeviceInfo->bCanDoWindowed )
        return ForceWindowed();

    m_bReady = FALSE;

    // Toggle the windowed state
    m_bWindowed = !m_bWindowed;
    pDeviceInfo->bWindowed = m_bWindowed;

    // Prepare window for windowed/fullscreen change
    AdjustWindowForChange();

    // Set up the presentation parameters
    m_d3dpp.Windowed               = pDeviceInfo->bWindowed;
    m_d3dpp.MultiSampleType        = pDeviceInfo->MultiSampleType;
    m_d3dpp.AutoDepthStencilFormat = pModeInfo->DepthStencilFormat;
    m_d3dpp.hDeviceWindow          = m_hWnd;
    if( m_bWindowed )
    {
        m_d3dpp.BackBufferWidth  = m_rcWindowClient.right - m_rcWindowClient.left;
        m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;
        m_d3dpp.BackBufferFormat = pAdapterInfo->d3ddmDesktop.Format;
    }
    else
    {
        m_d3dpp.BackBufferWidth  = pModeInfo->Width;
        m_d3dpp.BackBufferHeight = pModeInfo->Height;
        m_d3dpp.BackBufferFormat = pModeInfo->Format;
    }

    // Resize the 3D device
    if( FAILED( Resize3DEnvironment() ) )
    {
        if( m_bWindowed )
            return ForceWindowed();
        else
            return E_FAIL;
    }

    // When moving from fullscreen to windowed mode, it is important to
    // adjust the window size after resetting the device rather than
    // beforehand to ensure that you get the window size you want.  For
    // example, when switching from 640x480 fullscreen to windowed with
    // a 1000x600 window on a 1024x768 desktop, it is impossible to set
    // the window size to 1000x600 until after the display mode has
    // changed to 1024x768, because windows cannot be larger than the
    // desktop.
    if( m_bWindowed )
    {
        SetWindowPos( m_hWnd, HWND_NOTOPMOST,
                      m_rcWindowBounds.left, m_rcWindowBounds.top,
                      ( m_rcWindowBounds.right - m_rcWindowBounds.left ),
                      ( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
                      SWP_SHOWWINDOW );
    }

    m_bReady = TRUE;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ForceWindowed()
// Desc: Switch to a windowed mode, even if that means picking a new device
//       and/or adapter
//-----------------------------------------------------------------------------
HRESULT C3DEngine::ForceWindowed()
{
    HRESULT hr;
    D3DAdapterInfo* pAdapterInfoCur = &m_Adapters[m_dwAdapter];
    D3DDeviceInfo*  pDeviceInfoCur  = &pAdapterInfoCur->devices[pAdapterInfoCur->dwCurrentDevice];
    BOOL bFoundDevice = FALSE;

    if( pDeviceInfoCur->bCanDoWindowed )
    {
        bFoundDevice = TRUE;
    }
    else
    {
        // Look for a windowable device on any adapter
        D3DAdapterInfo* pAdapterInfo;
        DWORD dwAdapter;
        D3DDeviceInfo* pDeviceInfo;
        DWORD dwDevice;
        for( dwAdapter = 0; dwAdapter < m_dwNumAdapters; dwAdapter++ )
        {
            pAdapterInfo = &m_Adapters[dwAdapter];
            for( dwDevice = 0; dwDevice < pAdapterInfo->dwNumDevices; dwDevice++ )
            {
                pDeviceInfo = &pAdapterInfo->devices[dwDevice];
                if( pDeviceInfo->bCanDoWindowed )
                {
                    m_dwAdapter = dwAdapter;
                    pDeviceInfoCur = pDeviceInfo;
                    pAdapterInfo->dwCurrentDevice = dwDevice;
                    bFoundDevice = TRUE;
                    break;
                }
            }
            if( bFoundDevice )
                break;
        }
    }

    if( !bFoundDevice )
        return E_FAIL;

    pDeviceInfoCur->bWindowed = TRUE;
    m_bWindowed = TRUE;

    // Now destroy the current 3D device objects, then reinitialize

    m_bReady = FALSE;

    // Release all scene objects that will be re-created for the new device
    InvalidateDeviceObjects();
    DeleteDeviceObjects();

    // Release display objects, so a new device can be created
    if( m_pd3dDevice->Release() > 0L )
        return DisplayErrorMsg( C3DENGINE_ERR_NONZEROREFCOUNT, MSGERR_APPMUSTEXIT );

    // Create the new device
    if( FAILED( hr = Initialize3DEnvironment() ) )
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    m_bReady = TRUE;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: AdjustWindowForChange()
// Desc: Prepare the window for a possible change between windowed mode and
//       fullscreen mode.  This function is virtual and thus can be overridden
//       to provide different behavior, such as switching to an entirely
//       different window for fullscreen mode (as in the MFC sample apps).
//-----------------------------------------------------------------------------
HRESULT C3DEngine::AdjustWindowForChange()
{
    if( m_bWindowed )
    {
        // Set windowed-mode style
        SetWindowLong( m_hWnd, GWL_STYLE, m_dwWindowStyle );
    }
    else
    {
        // Set fullscreen-mode style
        SetWindowLong( m_hWnd, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE );
    }
    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: UserSelectMesh()
// Desc: Displays a dialog so the user can select a mesh to load from a .X file.
//-----------------------------------------------------------------------------
HRESULT C3DEngine::UserSelectMesh()
{
  HRESULT hr;

  // Can't display dialogs in fullscreen mode
  if( m_bWindowed == FALSE )
  {
    if( FAILED( ToggleFullscreen() ) )
    {
      DisplayErrorMsg( C3DENGINE_ERR_RESIZEFAILED, MSGERR_APPMUSTEXIT );
      return E_FAIL;
    }
  }

  // Prompt the user to select the mesh
  TCHAR strFilename[512];
  TCHAR strFullFilename[512] = _T("");

  _tcscpy( strFilename, _T("a") );
  _tcscpy( strFullFilename, m_strLastMeshFile );

  // Display the OpenFileName dialog. Then, try to load the specified file
  OPENFILENAME ofn = { sizeof(OPENFILENAME), NULL, NULL, _T(".X Files (.x)\0*.x\0\0"), 
    NULL, 0, 1, strFullFilename, 512, strFilename, 512, m_strLastMeshPath, _T("Open Mesh File"), 
    OFN_FILEMUSTEXIST, 0, 1, NULL, 0, NULL, NULL };

  if( TRUE == GetOpenFileName( &ofn ) )
  {
    _tcscpy( m_strLastMeshPath, strFullFilename );
    _tcscpy( m_strLastMeshFile, strFilename );
    TCHAR* pLastSlash =  _tcsrchr( m_strLastMeshPath, _T('\\') );
    if( pLastSlash )
      *pLastSlash = 0;

    _tcscpy( strFullFilename, strFilename );

    C3DFrameObject *pFrameObject = new C3DFrameObject();
    hr = pFrameObject->Init(this, strFullFilename);
    if( FAILED( hr ) )
    {
      delete pFrameObject;
      Pause(FALSE);
      return E_FAIL;
    }

    // add it to our world
    hr = AddChild(pFrameObject);
    if( FAILED( hr ) )
    {
      delete pFrameObject;
      Pause(FALSE);
      return E_FAIL;
    }

    // select it (temporary)
    m_pCurrentObject = pFrameObject;

  }
  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: UserSelectNewDevice()
// Desc: Displays a dialog so the user can select a new adapter, device, or
//       display mode, and then recreates the 3D environment if needed
//-----------------------------------------------------------------------------
HRESULT C3DEngine::UserSelectNewDevice()
{
    HRESULT hr;

    // Can't display dialogs in fullscreen mode
    if( m_bWindowed == FALSE )
    {
        if( FAILED( ToggleFullscreen() ) )
        {
            DisplayErrorMsg( C3DENGINE_ERR_RESIZEFAILED, MSGERR_APPMUSTEXIT );
            return E_FAIL;
        }
    }

    // Prompt the user to change the mode
    if( IDOK != DialogBoxParam( (HINSTANCE)GetModuleHandle(NULL),
                                MAKEINTRESOURCE(IDD_SELECTDEVICE), m_hWnd,
                                SelectDeviceProc, (LPARAM)this ) )
        return S_OK;

    // Get access to the newly selected adapter, device, and mode
    DWORD dwDevice;
    dwDevice  = m_Adapters[m_dwAdapter].dwCurrentDevice;
    m_bWindowed = m_Adapters[m_dwAdapter].devices[dwDevice].bWindowed;

    // Release all scene objects that will be re-created for the new device
    InvalidateDeviceObjects();
    DeleteDeviceObjects();

    // Release display objects, so a new device can be created
    if( m_pd3dDevice->Release() > 0L )
        return DisplayErrorMsg( C3DENGINE_ERR_NONZEROREFCOUNT, MSGERR_APPMUSTEXIT );

    // Inform the display class of the change. It will internally
    // re-create valid surfaces, a d3ddevice, etc.
    if( FAILED( hr = Initialize3DEnvironment() ) )
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );

    // If the app is paused, trigger the rendering of the current frame
    if( FALSE == m_bFrameMoving )
    {
        m_bSingleStep = TRUE;
        C3DUtil_Timer( TIMER_START );
        C3DUtil_Timer( TIMER_STOP );
    }
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: SelectDeviceProc()
// Desc: Windows message handling function for the device select dialog
//-----------------------------------------------------------------------------
INT_PTR CALLBACK C3DEngine::SelectDeviceProc( HWND hDlg, UINT msg,
                                                    WPARAM wParam, LPARAM lParam )
{
    // Get access to the UI controls
    HWND hwndAdapterList        = GetDlgItem( hDlg, IDC_ADAPTER_COMBO );
    HWND hwndDeviceList         = GetDlgItem( hDlg, IDC_DEVICE_COMBO );
    HWND hwndFullscreenModeList = GetDlgItem( hDlg, IDC_FULLSCREENMODES_COMBO );
    HWND hwndWindowedRadio      = GetDlgItem( hDlg, IDC_WINDOW );
    HWND hwndFullscreenRadio    = GetDlgItem( hDlg, IDC_FULLSCREEN );
    HWND hwndMultiSampleList    = GetDlgItem( hDlg, IDC_MULTISAMPLE_COMBO );
    BOOL bUpdateDlgControls     = FALSE;

    // Static state for adapter/device/mode selection
    static C3DEngine* pC3DEngine;
    static DWORD  dwOldAdapter, dwNewAdapter;
    static DWORD  dwOldDevice,  dwNewDevice;
    static DWORD  dwOldMode,    dwNewMode;
    static BOOL   bOldWindowed, bNewWindowed;
    static D3DMULTISAMPLE_TYPE OldMultiSampleType, NewMultiSampleType;

    // Working variables
    D3DAdapterInfo* pAdapter;
    D3DDeviceInfo*  pDevice;

    // Handle the initialization message
    if( WM_INITDIALOG == msg )
    {
        // Old state
        pC3DEngine      = (C3DEngine*)lParam;
        dwOldAdapter = pC3DEngine->m_dwAdapter;
        pAdapter     = &pC3DEngine->m_Adapters[dwOldAdapter];

        dwOldDevice  = pAdapter->dwCurrentDevice;
        pDevice      = &pAdapter->devices[dwOldDevice];

        dwOldMode    = pDevice->dwCurrentMode;
        bOldWindowed = pDevice->bWindowed;
        OldMultiSampleType = pDevice->MultiSampleType;

        // New state is initially the same as the old state
        dwNewAdapter = dwOldAdapter;
        dwNewDevice  = dwOldDevice;
        dwNewMode    = dwOldMode;
        bNewWindowed = bOldWindowed;
        NewMultiSampleType = OldMultiSampleType;

        // Set flag to update dialog controls below
        bUpdateDlgControls = TRUE;
    }

    if( WM_COMMAND == msg )
    {
        // Get current UI state
        bNewWindowed  = Button_GetCheck( hwndWindowedRadio );

        if( IDOK == LOWORD(wParam) )
        {
            // Handle the case when the user hits the OK button. Check if any
            // of the options were changed
            if( dwNewAdapter != dwOldAdapter || dwNewDevice  != dwOldDevice  ||
                dwNewMode    != dwOldMode    || bNewWindowed != bOldWindowed ||
                NewMultiSampleType != OldMultiSampleType )
            {
                pC3DEngine->m_dwAdapter = dwNewAdapter;

                pAdapter = &pC3DEngine->m_Adapters[dwNewAdapter];
                pAdapter->dwCurrentDevice = dwNewDevice;

                pAdapter->devices[dwNewDevice].dwCurrentMode = dwNewMode;
                pAdapter->devices[dwNewDevice].bWindowed     = bNewWindowed;
                pAdapter->devices[dwNewDevice].MultiSampleType = NewMultiSampleType;

                EndDialog( hDlg, IDOK );
            }
            else
                EndDialog( hDlg, IDCANCEL );

            return TRUE;
        }
        else if( IDCANCEL == LOWORD(wParam) )
        {
            // Handle the case when the user hits the Cancel button
            EndDialog( hDlg, IDCANCEL );
            return TRUE;
        }
        else if( CBN_SELENDOK == HIWORD(wParam) )
        {
            if( LOWORD(wParam) == IDC_ADAPTER_COMBO )
            {
                dwNewAdapter = ComboBox_GetCurSel( hwndAdapterList );
                pAdapter     = &pC3DEngine->m_Adapters[dwNewAdapter];

                dwNewDevice  = pAdapter->dwCurrentDevice;
                dwNewMode    = pAdapter->devices[dwNewDevice].dwCurrentMode;
                bNewWindowed = pAdapter->devices[dwNewDevice].bWindowed;
            }
            else if( LOWORD(wParam) == IDC_DEVICE_COMBO )
            {
                pAdapter     = &pC3DEngine->m_Adapters[dwNewAdapter];

                dwNewDevice  = ComboBox_GetCurSel( hwndDeviceList );
                dwNewMode    = pAdapter->devices[dwNewDevice].dwCurrentMode;
                bNewWindowed = pAdapter->devices[dwNewDevice].bWindowed;
            }
            else if( LOWORD(wParam) == IDC_FULLSCREENMODES_COMBO )
            {
                dwNewMode = ComboBox_GetCurSel( hwndFullscreenModeList );
            }
            else if( LOWORD(wParam) == IDC_MULTISAMPLE_COMBO )
            {
                DWORD dwItem = ComboBox_GetCurSel( hwndMultiSampleList );
                NewMultiSampleType = (D3DMULTISAMPLE_TYPE)ComboBox_GetItemData( hwndMultiSampleList, dwItem );
            }
        }
        // Keep the UI current
        bUpdateDlgControls = TRUE;
    }

    // Update the dialog controls
    if( bUpdateDlgControls )
    {
        // Reset the content in each of the combo boxes
        ComboBox_ResetContent( hwndAdapterList );
        ComboBox_ResetContent( hwndDeviceList );
        ComboBox_ResetContent( hwndFullscreenModeList );
        ComboBox_ResetContent( hwndMultiSampleList );

        pAdapter = &pC3DEngine->m_Adapters[dwNewAdapter];
        pDevice  = &pAdapter->devices[dwNewDevice];

        // Add a list of adapters to the adapter combo box
        for( DWORD a=0; a < pC3DEngine->m_dwNumAdapters; a++ )
        {
            // Add device name to the combo box
            DWORD dwItem = ComboBox_AddString( hwndAdapterList,
                             pC3DEngine->m_Adapters[a].d3dAdapterIdentifier.Description );

            // Set the item data to identify this adapter
            ComboBox_SetItemData( hwndAdapterList, dwItem, a );

            // Set the combobox selection on the current adapater
            if( a == dwNewAdapter )
                ComboBox_SetCurSel( hwndAdapterList, dwItem );
        }

        // Add a list of devices to the device combo box
        for( DWORD d=0; d < pAdapter->dwNumDevices; d++ )
        {
            // Add device name to the combo box
            DWORD dwItem = ComboBox_AddString( hwndDeviceList,
                                               pAdapter->devices[d].strDesc );

            // Set the item data to identify this device
            ComboBox_SetItemData( hwndDeviceList, dwItem, d );

            // Set the combobox selection on the current device
            if( d == dwNewDevice )
                ComboBox_SetCurSel( hwndDeviceList, dwItem );
        }

        // Add a list of modes to the mode combo box
        for( DWORD m=0; m < pDevice->dwNumModes; m++ )
        {
            DWORD BitDepth = 16;
            if( pDevice->modes[m].Format == D3DFMT_X8R8G8B8 ||
                pDevice->modes[m].Format == D3DFMT_A8R8G8B8 ||
                pDevice->modes[m].Format == D3DFMT_R8G8B8 )
            {
                BitDepth = 32;
            }

            // Add mode desc to the combo box
            TCHAR strMode[80];
            _stprintf( strMode, _T("%ld x %ld x %ld"), pDevice->modes[m].Width,
                                                       pDevice->modes[m].Height,
                                                       BitDepth );
            DWORD dwItem = ComboBox_AddString( hwndFullscreenModeList, strMode );

            // Set the item data to identify this mode
            ComboBox_SetItemData( hwndFullscreenModeList, dwItem, m );

            // Set the combobox selection on the current mode
            if( m == dwNewMode )
                ComboBox_SetCurSel( hwndFullscreenModeList, dwItem );
        }

        // Add a list of multisample modes to the multisample combo box
        for( m=0; m <= 16; m++ )
        {
            TCHAR strDesc[50];

            D3DFORMAT fmt;
            if( bNewWindowed )
                fmt = pC3DEngine->m_Adapters[dwNewAdapter].d3ddmDesktop.Format;
            else
                fmt = pDevice->modes[dwNewMode].Format;

            if ( m == 1 ) // 1 is not a valid multisample type
                continue;

            if( SUCCEEDED( pC3DEngine->m_pD3D->CheckDeviceMultiSampleType( dwNewAdapter,
                pDevice->DeviceType, fmt, bNewWindowed, (D3DMULTISAMPLE_TYPE)m ) ) )
            {
                if( m == 0 )
                    lstrcpy( strDesc, _T("none") );
                else
                    wsprintf( strDesc, _T("%d samples"), m );

                // Add device name to the combo box
                DWORD dwItem = ComboBox_AddString( hwndMultiSampleList, strDesc );

                // Set the item data to identify this multisample type
                ComboBox_SetItemData( hwndMultiSampleList, dwItem, m );

                // Set the combobox selection on the current multisample type
                if( (D3DMULTISAMPLE_TYPE)m == NewMultiSampleType || m == 0 )
                    ComboBox_SetCurSel( hwndMultiSampleList, dwItem );
            }
        }
        DWORD dwItem = ComboBox_GetCurSel( hwndMultiSampleList );
        NewMultiSampleType = (D3DMULTISAMPLE_TYPE)ComboBox_GetItemData( hwndMultiSampleList, dwItem );
        EnableWindow( hwndMultiSampleList, ComboBox_GetCount( hwndMultiSampleList ) > 1);
        EnableWindow( hwndWindowedRadio, pDevice->bCanDoWindowed );

        if( bNewWindowed )
        {
            Button_SetCheck( hwndWindowedRadio,   TRUE );
            Button_SetCheck( hwndFullscreenRadio, FALSE );
            EnableWindow( hwndFullscreenModeList, FALSE );
        }
        else
        {
            Button_SetCheck( hwndWindowedRadio,   FALSE );
            Button_SetCheck( hwndFullscreenRadio, TRUE );
            EnableWindow( hwndFullscreenModeList, TRUE );
        }
        return TRUE;
    }

    return FALSE;
}

LPDIRECT3DDEVICE8 C3DEngine::Device()
{
  return m_pd3dDevice;
}


//-----------------------------------------------------------------------------
// Name: Render()
// Desc:
//-----------------------------------------------------------------------------
HRESULT C3DEngine::Render()
{
	HRESULT hval;

  // Render a frame
  if( m_bActive && m_bReady )
  {
    if( m_pCurrentObject )
    {
      if( m_pCurrentObject != m_pLastObject )
      {
        // set object arcball matrices from object
        m_ObjectArcBall.SetMatrices( *m_pCurrentObject->GetRotationMatrix(), *m_pCurrentObject->GetTranslationMatrix() );
      }
      
      // set object matrices from object arcball
      hval = m_pCurrentObject->SetMatrices( m_ObjectArcBall.GetRotationMatrix(), m_ObjectArcBall.GetTranslationMatrix() );
    }
    

		// set camera vector from arcball
		D3DXVECTOR3 vLookatPt = m_pCamera->GetInitialLookatPt();

		 m_ArcBall.ClearDeltaMatrices();

		// set eye point by rotating initial eye direction by arcball rotation
		D3DXVECTOR3 vEyeDir = m_pCamera->GetInitialLookatPt();
		vEyeDir -= m_pCamera->GetInitialEyePt();
		D3DXVECTOR4 vEyePt4;
		D3DXVec3Transform( &vEyePt4, &vEyeDir, m_ArcBall.GetRotationMatrix() );
		D3DXVECTOR3 vEyePt( vEyePt4.x, vEyePt4.y, vEyePt4.z );

		vEyePt *= m_fCameraDistance;
		vEyePt += vLookatPt;
		m_ArcBall.SetRadius( m_fCameraDistance );

		// set up vector by rotating initial up vector by arcball rotation
		const D3DXVECTOR3 vUpDir = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		D3DXVECTOR4 vUpDir4;
		D3DXVec3Transform( &vUpDir4, &vUpDir, m_ArcBall.GetRotationMatrix() );
		D3DXVECTOR3 vUpVec( vUpDir4.x, vUpDir4.y, vUpDir4.z );

		// recreate camera view matrix
		m_pCamera->SetViewParams( vEyePt, vLookatPt, vUpVec );

		m_pd3dDevice->SetTransform( D3DTS_PROJECTION, (D3DMATRIX*)&m_pCamera->GetProjMatrix() );
		m_pd3dDevice->SetTransform(D3DTS_VIEW, (D3DMATRIX*)&m_pCamera->GetViewMatrix());

    hval = Render3DEnvironment();
	  if( FAILED(hval) )
	  {
      SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
		  return hval;
	  }
  }

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Render3DEnvironment()
// Desc: Draws the scene.
//-----------------------------------------------------------------------------
HRESULT C3DEngine::Render3DEnvironment()
{
  HRESULT hr;

  // Test the cooperative level to see if it's okay to render
  if( FAILED( hr = m_pd3dDevice->TestCooperativeLevel() ) )
  {
    // If the device was lost, do not render until we get it back
    if( D3DERR_DEVICELOST == hr )
      return S_OK;

    // Check if the device needs to be resized.
    if( D3DERR_DEVICENOTRESET == hr )
    {
      // If we are windowed, read the desktop mode and use the same format for
      // the back buffer
      if( m_bWindowed )
      {
        D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
        m_pD3D->GetAdapterDisplayMode( m_dwAdapter, &pAdapterInfo->d3ddmDesktop );
        m_d3dpp.BackBufferFormat = pAdapterInfo->d3ddmDesktop.Format;
      }

      if( FAILED( hr = Resize3DEnvironment() ) )
        return hr;
    }
    return hr;
  }

  // Get the app's time, in seconds. Skip rendering if no time elapsed
  FLOAT fAppTime        = C3DUtil_Timer( TIMER_GETAPPTIME );
  FLOAT fElapsedAppTime = C3DUtil_Timer( TIMER_GETELAPSEDTIME );
  if( ( 0.0f == fElapsedAppTime ) && m_bFrameMoving )
    return S_OK;

  // animate the scene
  if( m_bFrameMoving || m_bSingleStep )
  {
    // Store the time for the app
    m_fTime        = fAppTime;
    m_fElapsedTime = fElapsedAppTime;

    // Frame move the scene
//    if( FAILED( hr = FrameMove() ) )
//      return hr;

		if( FAILED( hr = TickControllers() ) )
			return hr;

    m_bSingleStep = FALSE;
  }
  
  // Render the objects in the scene
  // Clear the viewport
  m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(89,135,179), 1.0f, 0 );
  
  // Begin the scene 
  if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
  {
    UINT cTriangles = 0;
    D3DXMATRIX matD3D;
    D3DXMATRIX mOut;
    D3DXMATRIX mCur;
    D3DXVECTOR3 vTemp;
    D3DXMATRIX mObject;
    D3DXMATRIX mTrans;
    LPDIRECT3DDEVICE8 m_pDevice = m_pd3dDevice;
    
    // set world transform
    D3DXMatrixIdentity(&matD3D);
    hr = m_pd3dDevice->SetTransform(D3DTS_WORLD, &matD3D);
    
    bool bStringsDrawnThisFrame = false;
    int iLineCount = 0;
    
	  //* draw all the heirarchy
	  m_pChildList->IteratorRewind();
	  C3DObject* pChild = m_pChildList->IteratorNext();
	  while (pChild)
	  {
		  hr = pChild->Render(this);
		  if (hr != S_OK)
		  {
        // End the scene.
        m_pd3dDevice->EndScene();
			  return hr;
		  }

      //* *Hack*
      //* If this is the Knight Object Draw the strings
      C3DFrameObject* pKnight = static_cast<C3DFrameObject*>(pChild);
			char * foo = strstr(pKnight->GetPathname(), "stage");
      if ( !bStringsDrawnThisFrame && (m_pStringVB != NULL) && (pKnight->GetFileName() != NULL) && (strstr(pKnight->GetFileName(), "stage") == NULL))
      {
        bStringsDrawnThisFrame = true;
        struct Vertex
        {
          float x,y,z;
          DWORD c;
        };
        Vertex* pVertex;
        hr = m_pStringVB->Lock(0,0,(BYTE**)&pVertex,D3DLOCK_DISCARD);
        if( SUCCEEDED( hr ) )
        {
          //* Start adding the string vectors to the line list
          const DWORD color = 0x30FFFFFF;

          SFrame* pHeadFrame = pKnight->GetHeadDrawElement()->FindFrame("Box34");
          if ( pHeadFrame )
          {
            pVertex->x = pHeadFrame->matCombined._41;
            pVertex->y = pHeadFrame->matCombined._42;
            pVertex->z = pHeadFrame->matCombined._43+0.2f;
            pVertex->c = color;
            ++pVertex;
            pVertex->x = pHeadFrame->matCombined._41;
            pVertex->y = pHeadFrame->matCombined._42+4.1f;
            pVertex->z = pHeadFrame->matCombined._43-0.4f;
            pVertex->c = color;
            ++pVertex;
            
            ++iLineCount;
          }

          SFrame* pLSFrame = pKnight->GetHeadDrawElement()->FindFrame("left_shin");
          if ( pLSFrame )
          {
            pVertex->x = pLSFrame->matCombined._41;
            pVertex->y = pLSFrame->matCombined._42;
            pVertex->z = pLSFrame->matCombined._43-0.2f;
            pVertex->c = color;
            ++pVertex;
            pVertex->x = pLSFrame->matCombined._41-1.0f;
            pVertex->y = pLSFrame->matCombined._42+6.5f;
            pVertex->z = pLSFrame->matCombined._43-0.4f;
            pVertex->c = color;
            ++pVertex;
            
            ++iLineCount;
          }
          
          SFrame* pRSFrame = pKnight->GetHeadDrawElement()->FindFrame("right_shin");
          if ( pRSFrame )
          {
            pVertex->x = pRSFrame->matCombined._41;
            pVertex->y = pRSFrame->matCombined._42;
            pVertex->z = pRSFrame->matCombined._43-0.2f;
            pVertex->c = color;
            ++pVertex;
            pVertex->x = pRSFrame->matCombined._41+1.0f;
            pVertex->y = pRSFrame->matCombined._42+6.5f;
            pVertex->z = pRSFrame->matCombined._43-0.4f;
            pVertex->c = color;
            ++pVertex;
            
            ++iLineCount;
          }
          
          SFrame* pLHFrame = pKnight->GetHeadDrawElement()->FindFrame("left_hand");
          if ( pLHFrame )
          {
            pVertex->x = pLHFrame->matCombined._41;
            pVertex->y = pLHFrame->matCombined._42;
            pVertex->z = pLHFrame->matCombined._43;
            pVertex->c = color;
            ++pVertex;
            pVertex->x = pLHFrame->matCombined._41-0.5f;
            pVertex->y = pLHFrame->matCombined._42+5.5f;
            pVertex->z = pLHFrame->matCombined._43+0.2f;
            pVertex->c = color;
            ++pVertex;
            
            ++iLineCount;
          }
          
          SFrame* pRHFrame = pKnight->GetHeadDrawElement()->FindFrame("right_hand");
          if ( pRHFrame )
          {
            pVertex->x = pRHFrame->matCombined._41;
            pVertex->y = pRHFrame->matCombined._42;
            pVertex->z = pRHFrame->matCombined._43;
            pVertex->c = color;
            ++pVertex;
            pVertex->x = pRHFrame->matCombined._41+0.5f;
            pVertex->y = pRHFrame->matCombined._42+5.5f;
            pVertex->z = pRHFrame->matCombined._43+0.2f;
            pVertex->c = color;
            ++pVertex;
            
            ++iLineCount;
          }
          
          SFrame* pHipFrame = pKnight->GetHeadDrawElement()->FindFrame("hips");
          if ( pHipFrame )
          {
            pVertex->x = pHipFrame->matCombined._41;
            pVertex->y = pHipFrame->matCombined._42;
            pVertex->z = pHipFrame->matCombined._43+0.2f;
            pVertex->c = color;
            ++pVertex;
            pVertex->x = pHipFrame->matCombined._41;
            pVertex->y = pHipFrame->matCombined._42+6.0f;
            pVertex->z = pHipFrame->matCombined._43+0.5f;
            pVertex->c = color;
            ++pVertex;
            
            ++iLineCount;
          }

          //* Draw the linelist
          m_pStringVB->Unlock();
        }
          
     }
      
		  pChild = m_pChildList->IteratorNext();
	  }

    if ( iLineCount > 0)
    {
      m_pd3dDevice->SetTransform(D3DTS_WORLD, &matD3D);
      m_pd3dDevice->SetRenderState(D3DRS_COLORVERTEX, true  );
      
      m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );
      m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

      m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
      m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
      m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA  );
            
      m_pd3dDevice->SetStreamSource(0, m_pStringVB, 16);
      m_pd3dDevice->SetVertexShader(D3DFVF_XYZ | D3DFVF_DIFFUSE);
      m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, iLineCount );
    }

    

/*

    D3DXMatrixTranslation(&mTrans, 0, 0, -m_pdeSelected->fRadius * 2.8f);

    hr = m_pDevice->SetTransform(D3DTS_VIEW, (D3DMATRIX*)&mTrans);
    if(FAILED(hr))
        return hr;

    pdeCur = m_pdeHead;
    while (pdeCur != NULL)
    {
        D3DXMatrixTranslation(&mTrans, -pdeCur->vCenter.x, -pdeCur->vCenter.y, -pdeCur->vCenter.z);
        D3DXMatrixIdentity(&mCur);

        hr = UpdateFrames(pdeCur->pframeRoot, mCur);
        if (FAILED(hr))
            return hr;
        hr = DrawFrames(pdeCur->pframeRoot, cTriangles);
        if (FAILED(hr))
            return hr;

        pdeCur = pdeCur->pdeNext;
    }
*/

    // Show frame rate and device stats
		if( m_bShowFrameRate )
		{

	    // Draw text objects
			C3DText *pText = m_pTextHead;
			while( pText != NULL )
			{
				hr = pText->Render( m_pFont );
				if( FAILED(hr) )
					return hr;
				pText = pText->m_pNext;
			}

//			TCHAR strBuf[132];
//			_stprintf( strBuf, _T("%s : %s"), m_strFrameStats, m_strDeviceStats );
//			m_pFont->DrawText( 2,  0, D3DCOLOR_ARGB(255,255,255,0), strBuf );
//			sprintf( strBuf, "%d %d %d", nXPos, nYPos ,nZPos );
//			m_pFont->DrawText( 2, 20, D3DCOLOR_ARGB(255,255,255,0), strBuf );

//			m_pFont->DrawText( 2,  0, D3DCOLOR_ARGB(255,255,255,0), m_strFrameStats );
//			m_pFont->DrawText( 2, 20, D3DCOLOR_ARGB(255,255,255,0), m_strDeviceStats );
		}


    // End the scene.
    m_pd3dDevice->EndScene();
  }


  // Keep track of the frame count
  {
    static FLOAT fLastTime = 0.0f;
    static DWORD dwFrames  = 0L;
    FLOAT fTime = C3DUtil_Timer( TIMER_GETABSOLUTETIME );
    ++dwFrames;

    // Update the scene stats once per second
    if( fTime - fLastTime > 1.0f )
    {
      m_fFPS    = dwFrames / (fTime - fLastTime);
      fLastTime = fTime;
      dwFrames  = 0L;

      // Get adapter's current mode so we can report
      // bit depth (back buffer depth may be unknown)
      D3DDISPLAYMODE mode;
      m_pD3D->GetAdapterDisplayMode(m_dwAdapter, &mode);

      _stprintf( m_strFrameStats, _T("%.02f fps (%dx%dx%d)"), m_fFPS,
				m_d3dsdBackBuffer.Width, m_d3dsdBackBuffer.Height,
				mode.Format==D3DFMT_X8R8G8B8?32:16 );
      if( m_bUseDepthBuffer )
      {
        D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
        D3DDeviceInfo*  pDeviceInfo  = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
        D3DModeInfo*    pModeInfo    = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];

        switch( pModeInfo->DepthStencilFormat )
        {
        case D3DFMT_D16:
          lstrcat( m_strFrameStats, _T(" (D16)") );
          break;
        case D3DFMT_D15S1:
          lstrcat( m_strFrameStats, _T(" (D15S1)") );
          break;
        case D3DFMT_D24X8:
          lstrcat( m_strFrameStats, _T(" (D24X8)") );
          break;
        case D3DFMT_D24S8:
          lstrcat( m_strFrameStats, _T(" (D24S8)") );
          break;
        case D3DFMT_D24X4S4:
          lstrcat( m_strFrameStats, _T(" (D24X4S4)") );
          break;
        case D3DFMT_D32:
          lstrcat( m_strFrameStats, _T(" (D32)") );
          break;
        }
      }
    }
  }

  return S_OK;
}


//-----------------------------------------------------------------------------
// Name: Flip()
// Desc: Called in to toggle the pause state of the app.
//-----------------------------------------------------------------------------
HRESULT C3DEngine::Flip()
{
    if (m_pd3dDevice)
    {
      // Show the frame on the primary surface.
      m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Pause()
// Desc: Called in to toggle the pause state of the app.
//-----------------------------------------------------------------------------
VOID C3DEngine::Pause( BOOL bPause )
{
    static DWORD dwAppPausedCount = 0L;

    dwAppPausedCount += ( bPause ? +1 : -1 );
    m_bReady          = ( dwAppPausedCount ? FALSE : TRUE );

    // Handle the first pause request (of many, nestable pause requests)
    if( bPause && ( 1 == dwAppPausedCount ) )
    {
        // Stop the scene from animating
        if( m_bFrameMoving )
            C3DUtil_Timer( TIMER_STOP );
    }

    if( 0 == dwAppPausedCount )
    {
        // Restart the timers
        if( m_bFrameMoving )
            C3DUtil_Timer( TIMER_START );
    }
}




//-----------------------------------------------------------------------------
// Name: Destroy()
// Desc: Cleanup scene objects
//-----------------------------------------------------------------------------
VOID C3DEngine::Destroy()
{
    m_bActive = FALSE;
    m_bReady  = FALSE;

    if( m_pd3dDevice )
    {
        InvalidateDeviceObjects();
        DeleteDeviceObjects();

        m_pd3dDevice->Release();
        m_pD3D->Release();

        m_pd3dDevice = NULL;
        m_pD3D       = NULL;
    }

    // remove and destroy any children
    if( m_pChildList )
    {
		  m_pChildList->IteratorRewind();
		  C3DObject* pCurrent = m_pChildList->IteratorNext();
		  while (pCurrent)
		  {
			  // if there are any children still connected, destroy them
			  RemoveChild(pCurrent);
			  pCurrent->Destroy();
			  delete pCurrent;
			  pCurrent = m_pChildList->IteratorNext();
		  }
    
      delete m_pChildList;
		  m_pChildList = NULL;
  	}

		// delete text objects
		RemoveText();
    
    FinalCleanup();

}




//-----------------------------------------------------------------------------
// Name: DisplayErrorMsg()
// Desc: Displays error messages in a message box
//-----------------------------------------------------------------------------
HRESULT C3DEngine::DisplayErrorMsg( HRESULT hr, DWORD dwType )
{
    TCHAR strMsg[512];

    switch( hr )
    {
        case C3DENGINE_ERR_NODIRECT3D:
            _tcscpy( strMsg, _T("Could not initialize Direct3D. You may\n")
                             _T("want to check that the latest version of\n")
                             _T("DirectX is correctly installed on your\n")
                             _T("system.  Also make sure that this program\n")
                             _T("was compiled with header files that match\n")
                             _T("the installed DirectX DLLs.") );
            break;

        case C3DENGINE_ERR_NOCOMPATIBLEDEVICES:
            _tcscpy( strMsg, _T("Could not find any compatible Direct3D\n")
                             _T("devices.") );
            break;

        case C3DENGINE_ERR_NOWINDOWABLEDEVICES:
            _tcscpy( strMsg, _T("This sample cannot run in a desktop\n")
                             _T("window with the current display settings.\n")
                             _T("Please change your desktop settings to a\n")
                             _T("16- or 32-bit display mode and re-run this\n")
                             _T("sample.") );
            break;

        case C3DENGINE_ERR_NOHARDWAREDEVICE:
            _tcscpy( strMsg, _T("No hardware-accelerated Direct3D devices\n")
                             _T("were found.") );
            break;

        case C3DENGINE_ERR_HALNOTCOMPATIBLE:
            _tcscpy( strMsg, _T("This sample requires functionality that is\n")
                             _T("not available on your Direct3D hardware\n")
                             _T("accelerator.") );
            break;

        case C3DENGINE_ERR_NOWINDOWEDHAL:
            _tcscpy( strMsg, _T("Your Direct3D hardware accelerator cannot\n")
                             _T("render into a window.\n")
                             _T("Press F2 while the app is running to see a\n")
                             _T("list of available devices and modes.") );
            break;

        case C3DENGINE_ERR_NODESKTOPHAL:
            _tcscpy( strMsg, _T("Your Direct3D hardware accelerator cannot\n")
                             _T("render into a window with the current\n")
                             _T("desktop display settings.\n")
                             _T("Press F2 while the app is running to see a\n")
                             _T("list of available devices and modes.") );
            break;

        case C3DENGINE_ERR_NOHALTHISMODE:
            _tcscpy( strMsg, _T("This sample requires functionality that is\n")
                             _T("not available on your Direct3D hardware\n")
                             _T("accelerator with the current desktop display\n")
                             _T("settings.\n")
                             _T("Press F2 while the app is running to see a\n")
                             _T("list of available devices and modes.") );
            break;

        case C3DENGINE_ERR_MEDIANOTFOUND:
            _tcscpy( strMsg, _T("Could not load required media." ) );
            break;

        case C3DENGINE_ERR_RESIZEFAILED:
            _tcscpy( strMsg, _T("Could not reset the Direct3D device." ) );
            break;

        case C3DENGINE_ERR_NONZEROREFCOUNT:
            _tcscpy( strMsg, _T("A D3D object has a non-zero reference\n")
                             _T("count (meaning things were not properly\n")
                             _T("cleaned up).") );
            break;

        case E_OUTOFMEMORY:
            _tcscpy( strMsg, _T("Not enough memory.") );
            break;

        case D3DERR_OUTOFVIDEOMEMORY:
            _tcscpy( strMsg, _T("Not enough video memory.") );
            break;

        default:
            _tcscpy( strMsg, _T("Generic application error. Enable\n")
                             _T("debug output for detailed information.") );
    }

    if( MSGERR_APPMUSTEXIT == dwType )
    {
        _tcscat( strMsg, _T("\n\nExiting.") );
        MessageBox( NULL, strMsg, "Notice", MB_ICONERROR|MB_OK );

        // Close the window, which shuts down the app
        if( m_hWnd )
            SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
    }
    else
    {
        if( MSGWARN_SWITCHEDTOREF == dwType )
            _tcscat( strMsg, _T("\n\nSwitching to the reference rasterizer,\n")
                             _T("a software device that implements the entire\n")
                             _T("Direct3D feature set, but runs very slowly.") );
        MessageBox( NULL, strMsg, "Notice", MB_ICONWARNING|MB_OK );
    }

    return hr;
}

//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT C3DEngine::InitDeviceObjects()
{
    HRESULT hr = S_OK;

    // Restore the fonts
    m_pFont->InitDeviceObjects( m_pd3dDevice );

	  // restore all the heirarchy of children
	  m_pChildList->IteratorRewind();
	  C3DObject* pChild = m_pChildList->IteratorNext();
	  while (pChild)
	  {

		  hr = pChild->Init(this, "Reload");
		  if (hr != S_OK)
		  {
			  return hr;
		  }
		  pChild = m_pChildList->IteratorNext();
	  }

    m_ArcBall.SetRadius( 1.0f );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT C3DEngine::RestoreDeviceObjects()
{
    // Restore the fonts
    m_pFont->RestoreDeviceObjects();

    if( m_pChildList )
    {
		  m_pChildList->IteratorRewind();
		  C3DObject* pCurrent = m_pChildList->IteratorNext();
		  while (pCurrent)
		  {
        pCurrent->RestoreDeviceObjects(m_pd3dDevice);

        pCurrent = m_pChildList->IteratorNext();
		  }
    }

    //* init the string VB
    HRESULT hr = S_OK;
		hr = m_pd3dDevice->CreateVertexBuffer(12*16, D3DUSAGE_DYNAMIC|D3DUSAGE_SOFTWAREPROCESSING , D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &m_pStringVB);
    if (FAILED(hr))
      return E_FAIL;

    D3DLIGHT8 light;

    m_ArcBall.SetWindow( m_d3dsdBackBuffer.Width, m_d3dsdBackBuffer.Height, 2.0f );

    m_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );

    m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR  );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR  );

    m_pd3dDevice->SetRenderState( D3DRS_COLORVERTEX, FALSE );

    //m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
    //m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

    ZeroMemory( &light, sizeof(light) );
    light.Type = D3DLIGHT_DIRECTIONAL;

    light.Diffuse.r = 1.0;
    light.Diffuse.g = 1.0;
    light.Diffuse.b = 1.0;
    light.Specular.r = 0;
    light.Specular.g = 0;
    light.Specular.b = 0;
//    light.Ambient.r = 0.85;
//    light.Ambient.g = 0.85;
//    light.Ambient.b = 0.85;
    light.Ambient.r = 0.25;
    light.Ambient.g = 0.25;
    light.Ambient.b = 0.25;

    light.Direction = D3DXVECTOR3( 0.0f, 0.0f, 1.0f);

    hr = m_pd3dDevice->SetLight(0, &light );
    if (FAILED(hr))
        return E_FAIL;

    hr = m_pd3dDevice->LightEnable(0, TRUE);
    if (FAILED(hr))
        return E_FAIL;

    return S_OK;
}






//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//       this function deletes any device dependent objects.
//-----------------------------------------------------------------------------
HRESULT C3DEngine::InvalidateDeviceObjects()
{
    m_pFont->InvalidateDeviceObjects();
    if( m_pChildList )
    {
		  m_pChildList->IteratorRewind();
		  C3DObject* pCurrent = m_pChildList->IteratorNext();
		  while (pCurrent)
		  {
        pCurrent->InvalidateDeviceObjects();
        pCurrent = m_pChildList->IteratorNext();
		  }
    }
     if ( m_pStringVB )
      m_pStringVB->Release();
    m_pStringVB = 0;
    
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//       this function deletes any device dependent objects.
//-----------------------------------------------------------------------------
HRESULT C3DEngine::DeleteDeviceObjects()
{
  m_pFont->DeleteDeviceObjects();

  if( m_pChildList )
  {
		m_pChildList->IteratorRewind();
		C3DObject* pCurrent = m_pChildList->IteratorNext();
		while (pCurrent)
		{
      pCurrent->DeleteDeviceObjects();
      pCurrent = m_pChildList->IteratorNext();
		}
  }
  
  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: FinalCleanup()
// Desc: Called before the app exits, this function gives the app the chance
//       to cleanup after itself.
//-----------------------------------------------------------------------------
HRESULT C3DEngine::FinalCleanup()
{
  SAFE_DELETE( m_pCamera );
  SAFE_DELETE( m_pFont );

  return S_OK;
}


//-----------------------------------------------------------------------------
// Name: SetProjectionMatrix()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C3DEngine::SetProjectionMatrix()
{
  D3DXMATRIX mat;

  FLOAT fAspect = m_d3dsdBackBuffer.Width / (FLOAT)m_d3dsdBackBuffer.Height;
  D3DXMatrixPerspectiveFovRH(&mat, 0.25f*3.141592654f, fAspect, 1.0f, 20.0f);
  return m_pd3dDevice->SetTransform( D3DTS_PROJECTION, (D3DMATRIX*)&mat );
}

 

//-----------------------------------------------------------------------------
// Name: OneTimeSceneInit()
// Desc: Called during initial app startup, this function performs all the
//       permanent initialization.
//-----------------------------------------------------------------------------
HRESULT C3DEngine::OneTimeSceneInit()
{
  // create font
  m_pFont = new C3DFont( UI_FONT_NAME, UI_FONT_SIZE, UI_FONT_STYLE );	//C3DFONT_BOLD );

  // create camera
  m_pCamera = new (_NORMAL_BLOCK, __FILE__, __LINE__)C3DCamera;
//  m_pCamera->SetViewParams( D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(0.0f,4.2f, -1.0f),	D3DXVECTOR3(0.0f, 1.0f, 0.0f) );
  FLOAT fAspect = (float)Width() / (float)Height();
  m_pCamera->SetProjParams( 0.25f*D3DX_PI, fAspect, 1.0f, 1000.0f);

  // Set cursor to indicate that user can move the object with the mouse
#ifdef _WIN64
  SetClassLongPtr( m_hWnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor( NULL, IDC_SIZEALL ) );
#else
  SetClassLong( m_hWnd, GCL_HCURSOR, (LONG)LoadCursor( NULL, IDC_SIZEALL ) );
#endif
  return S_OK;
}




//-----------------------------------------------------------------------------
// Name: FrameMove()
// Desc: Called once per frame, the call is the entry point for animating
//       the scene.
//-----------------------------------------------------------------------------
HRESULT C3DEngine::FrameMove()
{
	//tick all the heirarchy
	m_pChildList->IteratorRewind();
	C3DObject* pChild = m_pChildList->IteratorNext();
	while (pChild)
	{
		pChild->FrameMove(m_fElapsedTime);
		pChild = m_pChildList->IteratorNext();
  }

  return S_OK;
}


//-----------------------------------------------------------------------------
// Name: TickControllers()
// Desc: Called once per frame to give each object's controllers a chance to update
//-----------------------------------------------------------------------------
HRESULT C3DEngine::TickControllers()
{
	//tick all the heirarchy
	m_pChildList->IteratorRewind();
	C3DObject* pChild = m_pChildList->IteratorNext();
	while (pChild)
	{
		HRESULT hr = pChild->TickControllers( m_fElapsedTime );
		if( FAILED( hr ) )
			return hr;
		pChild = m_pChildList->IteratorNext();
  }

  return S_OK;
}


//-----------------------------------------------------------------------------
// Name: ConfirmDevice()
// Desc: Called during device intialization, this code checks the device
//       for some minimum set of capabilities
//-----------------------------------------------------------------------------
HRESULT C3DEngine::ConfirmDevice( D3DCAPS8* pCaps, DWORD dwBehavior,
                                          D3DFORMAT Format )
{
    // This sample wants mixed vertex processing rather than hardware
    // vertex processing so it can fallback to sw processing if the 
    // device supports fewer than three matrices when skinning.
    if( dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING )
        return E_FAIL;

    return S_OK;
}

////////////////////////////////////////////////////////////////////////////

HRESULT C3DEngine::AddChild(C3DObject* pChild)
{
	m_pChildList->Append(pChild);
  pChild->RestoreDeviceObjects(m_pd3dDevice);

	return S_OK;
}

////////////////////////////////////////////////////////////////////////////

HRESULT C3DEngine::RemoveChild( C3DObject* pChild )
{
	m_pChildList->Remove(pChild);

	return S_OK;
}

////////////////////////////////////////////////////////////////////////////

HRESULT C3DEngine::AddText(C3DText* pText)
{
	pText->m_pNext = m_pTextHead;
	m_pTextHead = pText;
	return S_OK;
}

////////////////////////////////////////////////////////////////////////////

HRESULT C3DEngine::RemoveText()
{
	C3DText *pText = m_pTextHead;
	while( pText != NULL )
	{
		C3DText *pNext = pText->m_pNext;
		delete pText;
		pText = pNext;
	}
	m_pTextHead = NULL;
	return S_OK;
}
	
////////////////////////////////////////////////////////////////////////////

DWORD C3DEngine::Width()
{
  return m_d3dpp.BackBufferWidth;
}

////////////////////////////////////////////////////////////////////////////

DWORD C3DEngine::Height()
{
  return m_d3dpp.BackBufferHeight;
}

void C3DEngine::ResetArcBall()
{
	D3DXMATRIX mI;
	D3DXMatrixIdentity( &mI );
	m_ArcBall.SetMatrices( mI, mI );
}

////////////////////////////////////////////////////////////////////////////
// Generic node, can be added to a list
// Node Implementations...
Node::Node(C3DObject* pObject)
{
	m_pObject = pObject;
	m_pNext = NULL;
}

//////////////////////////////////////////////////////////////////

Node::~Node()
{
	m_pObject = NULL;
}

//////////////////////////////////////////////////////////////////

Node * Node::Next()
{
    return m_pNext;
}

//////////////////////////////////////////////////////////////////

C3DObject* Node::Object()
{
    return m_pObject;
}

//////////////////////////////////////////////////////////////////
// Generic list template
// Implementations for Lists...

List::List()
{
	m_pHead = NULL;
	m_dwNumberOfNodes = 0;
	m_pCurrent = NULL;
}

//////////////////////////////////////////////////////////////////

List::~List()
{
	Node * pCurrent = m_pHead;
	Node * pNext;

	while (pCurrent)
	{
		pNext = pCurrent->m_pNext;
		delete pCurrent;
		pCurrent = pNext;
	}
}

//////////////////////////////////////////////////////////////////

C3DObject* List::Head()
{
	if (m_pHead)
	{
		return m_pHead->m_pObject;
	}
	else
	{
		return NULL;
	}
}

//////////////////////////////////////////////////////////////////

void List::Append(C3DObject* pObject)
{
	Node * pNode = new Node(pObject);
	Node * pCurrent = m_pHead;
	Node * pNext = NULL;

	m_dwNumberOfNodes++;

	if (!m_pHead)
	{
		m_pHead = pNode;
		m_pCurrent = m_pHead;
		return;
	}

// find the end of the list and append this object
	while (pCurrent->m_pNext)
	{
		pCurrent = pCurrent->m_pNext;
	}

	pCurrent->m_pNext = pNode;
	pNode->m_pNext = NULL;
	return;
}

//////////////////////////////////////////////////////////////////

void List::Remove(C3DObject* pObject)
{
	Node * pCurrent = m_pHead;
	Node * pPrevious = NULL;

	if (!pObject)
		return;


// find the item in the list and remove it
	while (pCurrent)
	{
		if (pCurrent->m_pObject == pObject)
		{
			if (pPrevious)
			{
				pPrevious->m_pNext = pCurrent->m_pNext;
			}
			// move the head pointer if we remove the head
			if (pCurrent == m_pHead)
			{
				m_pHead = pCurrent->m_pNext;
			}

			// move the iterator pointer ahead if we removed the object it points to
			if (m_pCurrent == pCurrent)
			{
				m_pCurrent = pCurrent->m_pNext;
			}

			delete (pCurrent);
			m_dwNumberOfNodes--;
			return;
		}
		pPrevious = pCurrent;
		pCurrent = pCurrent->m_pNext;
		
	}

	return;
}

//////////////////////////////////////////////////////////////////

void List::IteratorRewind()
{
	m_pCurrent = m_pHead;
}

//////////////////////////////////////////////////////////////////

C3DObject* List::IteratorNext()
{
	Node * pCurrent = m_pCurrent;

	if (m_pCurrent)
	{
		m_pCurrent = m_pCurrent->m_pNext;
	}

	if (pCurrent)
	{
		return pCurrent->m_pObject;
	}

	return NULL;
}

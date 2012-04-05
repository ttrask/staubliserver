//-----------------------------------------------------------------------------
// File: C3DUtil.cpp
//
// Desc: Shortcut macros and functions for using DX objects
//
//
//-----------------------------------------------------------------------------
#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <stdio.h>
#include "C3DUtil.h"
#include <D3DX8.h>
#include <DDraw.h>
#include <d3d8.h>




//-----------------------------------------------------------------------------
// Name: C3DUtil_InitMaterial()
// Desc: Initializes a D3DMATERIAL8 structure, setting the diffuse and ambient
//       colors. It does not set emissive or specular colors.
//-----------------------------------------------------------------------------
VOID C3DUtil_InitMaterial( D3DMATERIAL8& mtrl, FLOAT r, FLOAT g, FLOAT b,
                           FLOAT a )
{
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL8) );
    mtrl.Diffuse.r = mtrl.Ambient.r = r;
    mtrl.Diffuse.g = mtrl.Ambient.g = g;
    mtrl.Diffuse.b = mtrl.Ambient.b = b;
    mtrl.Diffuse.a = mtrl.Ambient.a = a;
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_InitLight()
// Desc: Initializes a D3DLIGHT structure, setting the light position. The
//       diffuse color is set to white; specular and ambient are left as black.
//-----------------------------------------------------------------------------
VOID C3DUtil_InitLight( D3DLIGHT8& light, D3DLIGHTTYPE ltType,
                        FLOAT x, FLOAT y, FLOAT z )
{
    ZeroMemory( &light, sizeof(D3DLIGHT8) );
    light.Type        = ltType;
    light.Diffuse.r   = 1.0f;
    light.Diffuse.g   = 1.0f;
    light.Diffuse.b   = 1.0f;
    D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &D3DXVECTOR3(x, y, z) );
    light.Position.x   = x;
    light.Position.y   = y;
    light.Position.z   = z;
    light.Range        = 1000.0f;
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_CreateTexture()
// Desc: Helper function to create a texture. It checks the root path first,
//       then tries the DXSDK media path (as specified in the system registry).
//-----------------------------------------------------------------------------
HRESULT C3DUtil_CreateTexture( LPDIRECT3DDEVICE8 pd3dDevice, TCHAR* strTexture,
                               LPDIRECT3DTEXTURE8* ppTexture, D3DFORMAT d3dFormat )
{
    // Get the path to the texture
    TCHAR strPath[MAX_PATH];
    C3DUtil_FindMediaFile( strPath, strTexture );

    // Create the texture using D3DX
    return D3DXCreateTextureFromFileEx( pd3dDevice, strPath, 
                D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, d3dFormat, 
                D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
                D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 0, NULL, NULL, ppTexture );
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_SetColorKey()
// Desc: Changes all texels matching the colorkey to transparent, black.
//-----------------------------------------------------------------------------
HRESULT C3DUtil_SetColorKey( LPDIRECT3DTEXTURE8 pTexture, DWORD dwColorKey )
{
    // Get colorkey's red, green, and blue components
    DWORD r = ((dwColorKey&0x00ff0000)>>16);
    DWORD g = ((dwColorKey&0x0000ff00)>>8);
    DWORD b = ((dwColorKey&0x000000ff)>>0);

    // Put the colorkey in the texture's native format
    D3DSURFACE_DESC d3dsd;
    pTexture->GetLevelDesc( 0, &d3dsd );
    if( d3dsd.Format == D3DFMT_A4R4G4B4 )
        dwColorKey = 0xf000 + ((r>>4)<<8) + ((g>>4)<<4) + (b>>4);
    else if( d3dsd.Format == D3DFMT_A1R5G5B5 )
        dwColorKey = 0x8000 + ((r>>3)<<10) + ((g>>3)<<5) + (b>>3);
    else if( d3dsd.Format != D3DFMT_A8R8G8B8 )
        return E_FAIL;

    // Lock the texture
    D3DLOCKED_RECT  d3dlr;
    if( FAILED( pTexture->LockRect( 0, &d3dlr, 0, 0 ) ) )
        return E_FAIL;

    // Scan through each pixel, looking for the colorkey to replace
    for( DWORD y=0; y<d3dsd.Height; y++ )
    {
        for( DWORD x=0; x<d3dsd.Width; x++ )
        {
            if( d3dsd.Format==D3DFMT_A8R8G8B8 )
            {
                // Handle 32-bit formats
                if( ((DWORD*)d3dlr.pBits)[d3dsd.Width*y+x] == dwColorKey )
                    ((DWORD*)d3dlr.pBits)[d3dsd.Width*y+x] = 0x00000000;
            }
            else
            {
                // Handle 16-bit formats
                if( ((WORD*)d3dlr.pBits)[d3dsd.Width*y+x] == dwColorKey )
                    ((WORD*)d3dlr.pBits)[d3dsd.Width*y+x] = 0x0000;
            }
        }
    }

    // Unlock the texture and return OK.
    pTexture->UnlockRect(0);
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_CreateVertexShader()
// Desc: Assembles and creates a file-based vertex shader
//-----------------------------------------------------------------------------
HRESULT C3DUtil_CreateVertexShader( LPDIRECT3DDEVICE8 pd3dDevice, 
                                    TCHAR* strFilename, DWORD* pdwVertexDecl,
                                    DWORD* pdwVertexShader )
{
    LPD3DXBUFFER pCode;
    TCHAR        strPath[MAX_PATH];
    HRESULT      hr;

    // Get the path to the vertex shader file
    C3DUtil_FindMediaFile( strPath, strFilename );

    // Assemble the vertex shader file
    if( FAILED( hr = D3DXAssembleShaderFromFile( strPath, 0, NULL, &pCode, NULL ) ) )
        return hr;

    // Create the vertex shader
    hr = pd3dDevice->CreateVertexShader( pdwVertexDecl, 
                                         (DWORD*)pCode->GetBufferPointer(),
                                         pdwVertexShader, 0 );
    pCode->Release();
    return hr;
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_GetCubeMapViewMatrix()
// Desc: Returns a view matrix for rendering to a face of a cubemap.
//-----------------------------------------------------------------------------
D3DXMATRIX C3DUtil_GetCubeMapViewMatrix( DWORD dwFace )
{
    D3DXVECTOR3 vEyePt   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vLookDir;
    D3DXVECTOR3 vUpDir;

    switch( dwFace )
    {
        case D3DCUBEMAP_FACE_POSITIVE_X:
            vLookDir = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
            break;
        case D3DCUBEMAP_FACE_NEGATIVE_X:
            vLookDir = D3DXVECTOR3(-1.0f, 0.0f, 0.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
            break;
        case D3DCUBEMAP_FACE_POSITIVE_Y:
            vLookDir = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 0.0f,-1.0f );
            break;
        case D3DCUBEMAP_FACE_NEGATIVE_Y:
            vLookDir = D3DXVECTOR3( 0.0f,-1.0f, 0.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
            break;
        case D3DCUBEMAP_FACE_POSITIVE_Z:
            vLookDir = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
            break;
        case D3DCUBEMAP_FACE_NEGATIVE_Z:
            vLookDir = D3DXVECTOR3( 0.0f, 0.0f,-1.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
            break;
    }

    // Set the view transform for this cubemap surface
    D3DXMATRIX matView;
    D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookDir, &vUpDir );
    return matView;
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_GetRotationFromCursor()
// Desc: Returns a quaternion for the rotation implied by the window's cursor
//       position.
//-----------------------------------------------------------------------------
D3DXQUATERNION C3DUtil_GetRotationFromCursor( HWND hWnd,
                                              FLOAT fTrackBallRadius )
{
    POINT pt;
    RECT  rc;
    GetCursorPos( &pt );
    GetClientRect( hWnd, &rc );
    ScreenToClient( hWnd, &pt );
    FLOAT sx = ( ( ( 2.0f * pt.x ) / (rc.right-rc.left) ) - 1 );
    FLOAT sy = ( ( ( 2.0f * pt.y ) / (rc.bottom-rc.top) ) - 1 );
    FLOAT sz;

    if( sx == 0.0f && sy == 0.0f )
        return D3DXQUATERNION( 0.0f, 0.0f, 0.0f, 1.0f );

    FLOAT d1 = 0.0f;
    FLOAT d2 = sqrtf( sx*sx + sy*sy );

    if( d2 < fTrackBallRadius * 0.70710678118654752440 ) // Inside sphere
        sz = sqrtf( fTrackBallRadius*fTrackBallRadius - d2*d2 );
    else                                                 // On hyperbola
        sz = (fTrackBallRadius*fTrackBallRadius) / (2.0f*d2);

    // Get two points on trackball's sphere
    D3DXVECTOR3 p1( sx, sy, sz );
    D3DXVECTOR3 p2( 0.0f, 0.0f, fTrackBallRadius );

    // Get axis of rotation, which is cross product of p1 and p2
    D3DXVECTOR3 vAxis;
    D3DXVec3Cross( &vAxis, &p1, &p2);

    // Calculate angle for the rotation about that axis
    FLOAT t = D3DXVec3Length( &(p2-p1) ) / ( 2.0f*fTrackBallRadius );
    if( t > +1.0f) t = +1.0f;
    if( t < -1.0f) t = -1.0f;
    FLOAT fAngle = 2.0f * asinf( t );

    // Convert axis to quaternion
    D3DXQUATERNION quat;
    D3DXQuaternionRotationAxis( &quat, &vAxis, fAngle );
    return quat;
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_SetDeviceCursor
// Desc: Gives the D3D device a cursor with image and hotspot from hCursor.
//-----------------------------------------------------------------------------
HRESULT C3DUtil_SetDeviceCursor( LPDIRECT3DDEVICE8 pd3dDevice, HCURSOR hCursor )
{
    HRESULT hr = E_FAIL;
    ICONINFO iconinfo;
    BOOL bBWCursor;
    LPDIRECT3DSURFACE8 pCursorBitmap = NULL;
    HDC hdcColor = NULL;
    HDC hdcMask = NULL;
    HDC hdcScreen = NULL;
    BITMAP bm;
    DWORD dwWidth;
    DWORD dwHeightSrc;
    DWORD dwHeightDest;
    COLORREF crColor;
    COLORREF crMask;
    UINT x;
    UINT y;
    BITMAPINFO bmi;
    COLORREF* pcrArrayColor = NULL;
    COLORREF* pcrArrayMask = NULL;
    DWORD* pBitmap;
    HGDIOBJ hgdiobjOld;

    ZeroMemory( &iconinfo, sizeof(iconinfo) );
    if( !GetIconInfo( hCursor, &iconinfo ) )
        goto End;

    if (0 == GetObject((HGDIOBJ)iconinfo.hbmMask, sizeof(BITMAP), (LPVOID)&bm))
        goto End;
    dwWidth = bm.bmWidth;
    dwHeightSrc = bm.bmHeight;

    if( iconinfo.hbmColor == NULL )
    {
        bBWCursor = TRUE;
        dwHeightDest = dwHeightSrc / 2;
    }
    else 
    {
        bBWCursor = FALSE;
        dwHeightDest = dwHeightSrc;
    }

    // Create a surface for the fullscreen cursor
    if( FAILED( hr = pd3dDevice->CreateImageSurface( dwWidth, dwHeightDest, 
        D3DFMT_A8R8G8B8, &pCursorBitmap ) ) )
    {
        goto End;
    }

    pcrArrayMask = new DWORD[dwWidth * dwHeightSrc];

    ZeroMemory(&bmi, sizeof(bmi));
    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth = dwWidth;
    bmi.bmiHeader.biHeight = dwHeightSrc;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    hdcScreen = GetDC( NULL );
    hdcMask = CreateCompatibleDC( hdcScreen );
    if( hdcMask == NULL )
    {
        hr = E_FAIL;
        goto End;
    }
    hgdiobjOld = SelectObject(hdcMask, iconinfo.hbmMask);
    GetDIBits(hdcMask, iconinfo.hbmMask, 0, dwHeightSrc, 
        pcrArrayMask, &bmi, DIB_RGB_COLORS);
    SelectObject(hdcMask, hgdiobjOld);

    if (!bBWCursor)
    {
        pcrArrayColor = new DWORD[dwWidth * dwHeightDest];
        hdcColor = CreateCompatibleDC( GetDC( NULL ) );
        if( hdcColor == NULL )
        {
            hr = E_FAIL;
            goto End;
        }
        SelectObject(hdcColor, iconinfo.hbmColor);
        GetDIBits(hdcColor, iconinfo.hbmColor, 0, dwHeightDest, 
            pcrArrayColor, &bmi, DIB_RGB_COLORS);
    }

    // Transfer cursor image into the surface
    D3DLOCKED_RECT lr;
    pCursorBitmap->LockRect( &lr, NULL, 0 );
    pBitmap = (DWORD*)lr.pBits;
    for( y = 0; y < dwHeightDest; y++ )
    {
        for( x = 0; x < dwWidth; x++ )
        {
            if (bBWCursor)
            {
                crColor = pcrArrayMask[dwWidth*(dwHeightDest-1-y) + x];
                crMask = pcrArrayMask[dwWidth*(dwHeightSrc-1-y) + x];
            }
            else
            {
                crColor = pcrArrayColor[dwWidth*(dwHeightDest-1-y) + x];
                crMask = pcrArrayMask[dwWidth*(dwHeightDest-1-y) + x];
            }
            if (crMask == 0)
                pBitmap[dwWidth*y + x] = 0xff000000 | crColor;
            else
                pBitmap[dwWidth*y + x] = 0x00000000;
        }
    }
    pCursorBitmap->UnlockRect();

    // Set the device cursor
    if( FAILED( hr = pd3dDevice->SetCursorProperties( iconinfo.xHotspot, 
        iconinfo.yHotspot, pCursorBitmap ) ) )
    {
        goto End;
    }

    hr = S_OK;

End:
    if( iconinfo.hbmMask != NULL )
        DeleteObject( iconinfo.hbmMask );
    if( iconinfo.hbmColor != NULL )
        DeleteObject( iconinfo.hbmColor );
    if( hdcScreen != NULL )
        ReleaseDC( NULL, hdcScreen );
    if( hdcColor != NULL )
        DeleteDC( hdcColor );
    if( hdcMask != NULL )
        DeleteDC( hdcMask );
    SAFE_DELETE_ARRAY( pcrArrayColor );
    SAFE_DELETE_ARRAY( pcrArrayMask );
    SAFE_RELEASE( pCursorBitmap );
    return hr;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
C3DArcBall::C3DArcBall()
{
    D3DXQuaternionIdentity( &m_qDown );
    D3DXQuaternionIdentity( &m_qNow );
    D3DXMatrixIdentity( &m_matRotation );
    D3DXMatrixIdentity( &m_matRotationDelta );
    D3DXMatrixIdentity( &m_matTranslation );
    D3DXMatrixIdentity( &m_matTranslationDelta );
    m_bDrag = FALSE;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
VOID C3DArcBall::SetWindow( int iWidth, int iHeight, float fRadius )
{
    // Set ArcBall info
    m_iWidth  = iWidth;
    m_iHeight = iHeight;
    m_fRadius = fRadius;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
D3DXVECTOR3 C3DArcBall::ScreenToVector( int sx, int sy )
{
    // Scale to screen
    FLOAT x   =  (sx - m_iWidth/2)  / (m_fRadius*m_iWidth/2);
    FLOAT y   = -(sy - m_iHeight/2) / (m_fRadius*m_iHeight/2);
    FLOAT z   = 0.0f;
    FLOAT mag = x*x + y*y;

    if( mag > 1.0f )
    {
        FLOAT scale = 1.0f/sqrtf(mag);
        x *= scale;
        y *= scale;
    }
    else
        z = sqrtf( 1.0f - mag );

    // Return vector
    return D3DXVECTOR3( x, y, z );
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
VOID C3DArcBall::SetRadius( FLOAT fRadius )
{
    m_fRadius2 = fRadius;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
LRESULT C3DArcBall::HandleMouseMessages( HWND hWnd, UINT uMsg, WPARAM wParam,
                                          LPARAM lParam )
{
    static int         iCurMouseX;      // Saved mouse position
    static int         iCurMouseY;
    static D3DXVECTOR3 m_vDown;         // Button down quaternion
    static D3DXVECTOR3 m_vCur;          // Current quaternion

    // Current mouse position
    int iMouseX = LOWORD(lParam);
    int iMouseY = HIWORD(lParam);

    switch( uMsg )
    {
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
            // Store off the position of the cursor when the button is pressed
            iCurMouseX = iMouseX;
            iCurMouseY = iMouseY;
            return TRUE;

        case WM_LBUTTONDOWN:
            // Start drag mode
            m_bDrag = TRUE;
            m_vDown = ScreenToVector( iMouseX, iMouseY );
            return TRUE;

        case WM_LBUTTONUP:
            // End drag mode
            m_bDrag = FALSE;
            m_qDown = m_qNow;
            return TRUE;

        case WM_MOUSEMOVE:
            // Drag object
            if( MK_LBUTTON&wParam )
            {
                if( m_bDrag )
                {
                    D3DXVECTOR3 vPart;
                    D3DXVECTOR3 vCur = ScreenToVector( iMouseX, iMouseY );
                    D3DXVec3Cross( &vPart, &m_vDown, &vCur );
                    m_qNow = m_qDown * D3DXQUATERNION( vPart.x, vPart.y, vPart.z,
                                                       D3DXVec3Dot( &m_vDown, &vCur ) );
                }

                D3DXQUATERNION qConj;
                D3DXQuaternionConjugate( &qConj, &m_qNow );

                D3DXMatrixRotationQuaternion( &m_matRotationDelta, &qConj );
                D3DXMatrixTranspose( &m_matRotationDelta, &m_matRotationDelta );
                D3DXMatrixMultiply( &m_matRotation, &m_matRotation, &m_matRotationDelta );

                D3DXQuaternionIdentity( &m_qDown );
                D3DXQuaternionIdentity( &m_qNow );
                m_vDown = ScreenToVector( iMouseX, iMouseY );
                m_bDrag = TRUE;
            }
            else if( (MK_RBUTTON&wParam) || (MK_MBUTTON&wParam) )
            {
                // Normalize based on size of window and bounding sphere radius
                FLOAT fDeltaX = ( iCurMouseX-iMouseX ) * m_fRadius2 / m_iWidth;
                FLOAT fDeltaY = ( iCurMouseY-iMouseY ) * m_fRadius2 / m_iHeight;

                if( wParam & MK_RBUTTON )
                {
                    D3DXMatrixTranslation( &m_matTranslationDelta, -2*fDeltaX, 2*fDeltaY, 0.0f );
                    D3DXMatrixMultiply( &m_matTranslation, &m_matTranslation, &m_matTranslationDelta );
                }
                else  // wParam & MK_MBUTTON
                {
                    D3DXMatrixTranslation( &m_matTranslationDelta, 0.0f, 0.0f, 5*fDeltaY );
                    D3DXMatrixMultiply( &m_matTranslation, &m_matTranslation, &m_matTranslationDelta );
                }

                // Store mouse coordinate
                iCurMouseX = iMouseX;
                iCurMouseY = iMouseY;
            }
            return TRUE;
    }

    return FALSE;
}




//-----------------------------------------------------------------------------
// Name: C3DDisplay()
// Desc:
//-----------------------------------------------------------------------------
C3DDisplay::C3DDisplay()
{
    m_pDD                = NULL;
    m_pddsFrontBuffer    = NULL;
    m_pddsBackBuffer     = NULL;
    m_pddsBackBufferLeft = NULL;
}




//-----------------------------------------------------------------------------
// Name: ~C3DDisplay()
// Desc:
//-----------------------------------------------------------------------------
C3DDisplay::~C3DDisplay()
{
    DestroyObjects();
}




//-----------------------------------------------------------------------------
// Name: DestroyObjects()
// Desc:
//-----------------------------------------------------------------------------
HRESULT C3DDisplay::DestroyObjects()
{
    SAFE_RELEASE( m_pddsBackBufferLeft );
    SAFE_RELEASE( m_pddsBackBuffer );
    SAFE_RELEASE( m_pddsFrontBuffer );

    if( m_pDD )
        m_pDD->SetCooperativeLevel( m_hWnd, DDSCL_NORMAL );

    SAFE_RELEASE( m_pDD );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CreateFullScreenDisplay()
// Desc:
//-----------------------------------------------------------------------------
HRESULT C3DDisplay::CreateFullScreenDisplay( HWND hWnd, DWORD dwWidth,
                                           DWORD dwHeight, DWORD dwBPP )
{
    HRESULT hr;

    // Cleanup anything from a previous call
    DestroyObjects();

    // DDraw stuff begins here
    if( FAILED( hr = DirectDrawCreateEx( NULL, (VOID**)&m_pDD,
                                         IID_IDirectDraw7, NULL ) ) )
        return E_FAIL;

    // Set cooperative level
    hr = m_pDD->SetCooperativeLevel( hWnd, DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN );
    if( FAILED(hr) )
        return E_FAIL;

    // Set the display mode
    if( FAILED( m_pDD->SetDisplayMode( dwWidth, dwHeight, dwBPP, 0, 0 ) ) )
        return E_FAIL;

    // Create primary surface (with backbuffer attached)
    DDSURFACEDESC2 ddsd;
    ZeroMemory( &ddsd, sizeof( ddsd ) );
    ddsd.dwSize            = sizeof( ddsd );
    ddsd.dwFlags           = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP |
                             DDSCAPS_COMPLEX | DDSCAPS_3DDEVICE;
    ddsd.dwBackBufferCount = 1;

    if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsFrontBuffer,
                                           NULL ) ) )
        return E_FAIL;

    // Get a pointer to the back buffer
    DDSCAPS2 ddscaps;
    ZeroMemory( &ddscaps, sizeof( ddscaps ) );
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER;

    if( FAILED( hr = m_pddsFrontBuffer->GetAttachedSurface( &ddscaps,
                                                            &m_pddsBackBuffer ) ) )
        return E_FAIL;

    m_pddsBackBuffer->AddRef();

    m_hWnd      = hWnd;
    m_bWindowed = FALSE;
    UpdateBounds();

    return S_OK;
}
    



//-----------------------------------------------------------------------------
// Name: CreateWindowedDisplay()
// Desc:
//-----------------------------------------------------------------------------
HRESULT C3DDisplay::CreateWindowedDisplay( HWND hWnd, DWORD dwWidth, DWORD dwHeight )
{
    HRESULT hr;

    // Cleanup anything from a previous call
    DestroyObjects();

    // DDraw stuff begins here
    if( FAILED( hr = DirectDrawCreateEx( NULL, (VOID**)&m_pDD,
                                         IID_IDirectDraw7, NULL ) ) )
        return E_FAIL;

    // Set cooperative level
    hr = m_pDD->SetCooperativeLevel( hWnd, DDSCL_NORMAL );
    if( FAILED(hr) )
        return E_FAIL;

    RECT  rcWork;
    RECT  rc;
    DWORD dwStyle;

    // If we are still a WS_POPUP window we should convert to a normal app
    // window so we look like a windows app.
    dwStyle  = GetWindowStyle( hWnd );
    dwStyle &= ~WS_POPUP;
    dwStyle |= WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX;
    SetWindowLong( hWnd, GWL_STYLE, dwStyle );

    // Aet window size
    SetRect( &rc, 0, 0, dwWidth, dwHeight );

    AdjustWindowRectEx( &rc, GetWindowStyle(hWnd), GetMenu(hWnd) != NULL,
                        GetWindowExStyle(hWnd) );

    SetWindowPos( hWnd, NULL, 0, 0, rc.right-rc.left, rc.bottom-rc.top,
                  SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE );

    SetWindowPos( hWnd, HWND_NOTOPMOST, 0, 0, 0, 0,
                  SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE );

    //  Make sure our window does not hang outside of the work area
    SystemParametersInfo( SPI_GETWORKAREA, 0, &rcWork, 0 );
    GetWindowRect( hWnd, &rc );
    if( rc.left < rcWork.left ) rc.left = rcWork.left;
    if( rc.top  < rcWork.top )  rc.top  = rcWork.top;
    SetWindowPos( hWnd, NULL, rc.left, rc.top, 0, 0,
                  SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );

    LPDIRECTDRAWCLIPPER pcClipper;
    
    // Create the primary surface
    DDSURFACEDESC2 ddsd;
    ZeroMemory( &ddsd, sizeof( ddsd ) );
    ddsd.dwSize         = sizeof( ddsd );
    ddsd.dwFlags        = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    if( FAILED( m_pDD->CreateSurface( &ddsd, &m_pddsFrontBuffer, NULL ) ) )
        return E_FAIL;

    // Create the backbuffer surface
    ddsd.dwFlags        = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;    
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
    ddsd.dwWidth        = dwWidth;
    ddsd.dwHeight       = dwHeight;

    if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsBackBuffer, NULL ) ) )
        return E_FAIL;

    if( FAILED( hr = m_pDD->CreateClipper( 0, &pcClipper, NULL ) ) )
        return E_FAIL;

    if( FAILED( hr = pcClipper->SetHWnd( 0, hWnd ) ) )
    {
        pcClipper->Release();
        return E_FAIL;
    }

    if( FAILED( hr = m_pddsFrontBuffer->SetClipper( pcClipper ) ) )
    {
        pcClipper->Release();
        return E_FAIL;
    }

    // Done with clipper
    pcClipper->Release();

    m_hWnd      = hWnd;
    m_bWindowed = TRUE;
    UpdateBounds();

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C3DDisplay::CreateSurface( C3DSurface** ppSurface,
                                 DWORD dwWidth, DWORD dwHeight )
{
    if( NULL == m_pDD )
        return E_POINTER;
    if( NULL == ppSurface )
        return E_INVALIDARG;

    HRESULT        hr;
    DDSURFACEDESC2 ddsd;
    ZeroMemory( &ddsd, sizeof( ddsd ) );
    ddsd.dwSize         = sizeof( ddsd );
    ddsd.dwFlags        = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT; 
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth        = dwWidth;
    ddsd.dwHeight       = dwHeight;

    (*ppSurface) = new C3DSurface();
    if( FAILED( hr = (*ppSurface)->Create( m_pDD, &ddsd ) ) )
    {
        delete (*ppSurface);
        return hr;
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: C3DDisplay::CreateSurfaceFromBitmap()
// Desc: Create a DirectDrawSurface from a bitmap resource or bitmap file.
//       Use MAKEINTRESOURCE() to pass a constant into strBMP.
//-----------------------------------------------------------------------------
HRESULT C3DDisplay::CreateSurfaceFromBitmap( C3DSurface** ppSurface,
                                           TCHAR* strBMP,                                            
                                           DWORD dwDesiredWidth, 
                                           DWORD dwDesiredHeight )
{
    HRESULT        hr;
    HBITMAP        hBMP = NULL;
    BITMAP         bmp;
    DDSURFACEDESC2 ddsd;

    if( m_pDD == NULL || strBMP == NULL || ppSurface == NULL ) 
        return E_INVALIDARG;

    *ppSurface = NULL;

    //  Try to load the bitmap as a resource, if that fails, try it as a file
    hBMP = (HBITMAP) LoadImage( GetModuleHandle(NULL), strBMP, 
                                IMAGE_BITMAP, dwDesiredWidth, dwDesiredHeight, 
                                LR_CREATEDIBSECTION );
    if( hBMP == NULL )
    {
        hBMP = (HBITMAP) LoadImage( NULL, strBMP, 
                                    IMAGE_BITMAP, dwDesiredWidth, dwDesiredHeight, 
                                    LR_LOADFROMFILE | LR_CREATEDIBSECTION );
        if( hBMP == NULL )
            return E_FAIL;
    }

    // Get size of the bitmap
    GetObject( hBMP, sizeof(bmp), &bmp );

    // Create a DirectDrawSurface for this bitmap
    ZeroMemory( &ddsd, sizeof(ddsd) );
    ddsd.dwSize         = sizeof(ddsd);
    ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth        = bmp.bmWidth;
    ddsd.dwHeight       = bmp.bmHeight;

    (*ppSurface) = new C3DSurface();
    if( FAILED( hr = (*ppSurface)->Create( m_pDD, &ddsd ) ) )
    {
        delete (*ppSurface);
        return hr;
    }

    // Draw the bitmap on this surface
    if( FAILED( hr = (*ppSurface)->DrawBitmap( hBMP, 0, 0, 0, 0 ) ) )
    {
        DeleteObject( hBMP );
        return hr;
    }

    DeleteObject( hBMP );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: C3DDisplay::CreateSurfaceFromText()
// Desc: Creates a DirectDrawSurface from a text string using hFont or the default 
//       GDI font if hFont is NULL.
//-----------------------------------------------------------------------------
HRESULT C3DDisplay::CreateSurfaceFromText( C3DSurface** ppSurface,
                                         HFONT hFont, TCHAR* strText, 
                                         COLORREF crBackground, COLORREF crForeground )
{
    HDC                  hDC  = NULL;
    LPDIRECTDRAWSURFACE7 pDDS = NULL;
    HRESULT              hr;
    DDSURFACEDESC2       ddsd;
    SIZE                 sizeText;

    if( m_pDD == NULL || strText == NULL || ppSurface == NULL )
        return E_INVALIDARG;

    *ppSurface = NULL;

    hDC = GetDC( NULL );

    if( hFont )
        SelectObject( hDC, hFont );

    GetTextExtentPoint32( hDC, strText, _tcslen(strText), &sizeText );
    ReleaseDC( NULL, hDC );

    // Create a DirectDrawSurface for this bitmap
    ZeroMemory( &ddsd, sizeof(ddsd) );
    ddsd.dwSize         = sizeof(ddsd);
    ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth        = sizeText.cx;
    ddsd.dwHeight       = sizeText.cy;

    (*ppSurface) = new C3DSurface();
    if( FAILED( hr = (*ppSurface)->Create( m_pDD, &ddsd ) ) )
    {
        delete (*ppSurface);
        return hr;
    }

    if( FAILED( hr = (*ppSurface)->DrawText( hFont, strText, 0, 0, 
                                             crBackground, crForeground ) ) )
        return hr;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C3DDisplay::Present()
{
    HRESULT hr;

    if( NULL == m_pddsFrontBuffer && NULL == m_pddsBackBuffer )
        return E_POINTER;

    while( 1 )
    {
        if( m_bWindowed )
            hr = m_pddsFrontBuffer->Blt( &m_rcWindow, m_pddsBackBuffer,
                                         NULL, DDBLT_WAIT, NULL );
        else
            hr = m_pddsFrontBuffer->Flip( NULL, 0 );

        if( hr == DDERR_SURFACELOST )
        {
            m_pddsFrontBuffer->Restore();
            m_pddsBackBuffer->Restore();
        }

        if( hr != DDERR_WASSTILLDRAWING )
            return hr;
    }
}




//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C3DDisplay::ShowBitmap( HBITMAP hbm, LPDIRECTDRAWPALETTE pPalette )
{
    if( NULL == m_pddsFrontBuffer ||  NULL == m_pddsBackBuffer )
        return E_POINTER;

    // Set the palette before loading the bitmap
    if( pPalette )
        m_pddsFrontBuffer->SetPalette( pPalette );

    C3DSurface backBuffer;
    backBuffer.Create( m_pddsBackBuffer );

    if( FAILED( backBuffer.DrawBitmap( hbm, 0, 0, 0, 0 ) ) )
        return E_FAIL;

    return Present();
}




//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C3DDisplay::ColorKeyBlt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 pdds,
                               RECT* prc )
{
    if( NULL == m_pddsBackBuffer )
        return E_POINTER;

    return m_pddsBackBuffer->BltFast( x, y, pdds, prc, DDBLTFAST_SRCCOLORKEY );
}




//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C3DDisplay::Blt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 pdds, RECT* prc,
                       DWORD dwFlags )
{
    if( NULL == m_pddsBackBuffer )
        return E_POINTER;

    return m_pddsBackBuffer->BltFast( x, y, pdds, prc, dwFlags );
}




//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C3DDisplay::Blt( DWORD x, DWORD y, C3DSurface* pSurface, RECT* prc )
{
    if( NULL == pSurface )
        return E_INVALIDARG;

    if( pSurface->IsColorKeyed() )
        return Blt( x, y, pSurface->GetDDrawSurface(), prc, DDBLTFAST_SRCCOLORKEY );
    else
        return Blt( x, y, pSurface->GetDDrawSurface(), prc, 0L );
}




//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C3DDisplay::Clear( DWORD dwColor )
{
    if( NULL == m_pddsBackBuffer )
        return E_POINTER;

    // Erase the background
    DDBLTFX ddbltfx;
    ZeroMemory( &ddbltfx, sizeof(ddbltfx) );
    ddbltfx.dwSize      = sizeof(ddbltfx);
    ddbltfx.dwFillColor = dwColor;

    return m_pddsBackBuffer->Blt( NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );
}




//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C3DDisplay::SetPalette( LPDIRECTDRAWPALETTE pPalette )
{
    if( NULL == m_pddsFrontBuffer )
        return E_POINTER;

    return m_pddsFrontBuffer->SetPalette( pPalette );
}




//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C3DDisplay::CreatePaletteFromBitmap( LPDIRECTDRAWPALETTE* ppPalette,
                                           const TCHAR* strBMP )
{
    HRSRC             hResource      = NULL;
    RGBQUAD*          pRGB           = NULL;
    BITMAPINFOHEADER* pbi = NULL;
    PALETTEENTRY      aPalette[256];
    HANDLE            hFile = NULL;
    DWORD             iColor;
    DWORD             dwColors;
    BITMAPFILEHEADER  bf;
    BITMAPINFOHEADER  bi;
    DWORD             dwBytesRead;

    if( m_pDD == NULL || strBMP == NULL || ppPalette == NULL )
        return E_INVALIDARG;

    *ppPalette = NULL;

    //  Try to load the bitmap as a resource, if that fails, try it as a file
    hResource = FindResource( NULL, strBMP, RT_BITMAP );
    if( hResource )
    {
        pbi = (LPBITMAPINFOHEADER) LockResource( LoadResource( NULL, hResource ) );       
        if( NULL == pbi )
            return E_FAIL;

        pRGB = (RGBQUAD*) ( (BYTE*) pbi + pbi->biSize );

        // Figure out how many colors there are
        if( pbi == NULL || pbi->biSize < sizeof(BITMAPINFOHEADER) )
            dwColors = 0;
        else if( pbi->biBitCount > 8 )
            dwColors = 0;
        else if( pbi->biClrUsed == 0 )
            dwColors = 1 << pbi->biBitCount;
        else
            dwColors = pbi->biClrUsed;

        //  A DIB color table has its colors stored BGR not RGB
        //  so flip them around.
        for( iColor = 0; iColor < dwColors; iColor++ )
        {
            aPalette[iColor].peRed   = pRGB[iColor].rgbRed;
            aPalette[iColor].peGreen = pRGB[iColor].rgbGreen;
            aPalette[iColor].peBlue  = pRGB[iColor].rgbBlue;
            aPalette[iColor].peFlags = 0;
        }

        return m_pDD->CreatePalette( DDPCAPS_8BIT, aPalette, ppPalette, NULL );
    }

    // Attempt to load bitmap as a file
    hFile = CreateFile( strBMP, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL );
    if( NULL == hFile )
        return E_FAIL;

    // Read the BITMAPFILEHEADER
    ReadFile( hFile, &bf, sizeof(bf), &dwBytesRead, NULL );
    if( dwBytesRead != sizeof(bf) )
    {
        CloseHandle( hFile );
        return E_FAIL;
    }

    // Read the BITMAPINFOHEADER
    ReadFile( hFile, &bi, sizeof(bi), &dwBytesRead, NULL );
    if( dwBytesRead != sizeof(bi) )
    {
        CloseHandle( hFile );
        return E_FAIL;
    }

    // Read the PALETTEENTRY 
    ReadFile( hFile, aPalette, sizeof(aPalette), &dwBytesRead, NULL );
    if( dwBytesRead != sizeof(aPalette) )
    {
        CloseHandle( hFile );
        return E_FAIL;
    }

    CloseHandle( hFile );

    // Figure out how many colors there are
    if( bi.biSize != sizeof(BITMAPINFOHEADER) )
        dwColors = 0;
    else if (bi.biBitCount > 8)
        dwColors = 0;
    else if (bi.biClrUsed == 0)
        dwColors = 1 << bi.biBitCount;
    else
        dwColors = bi.biClrUsed;

    //  A DIB color table has its colors stored BGR not RGB
    //  so flip them around since DirectDraw uses RGB
    for( iColor = 0; iColor < dwColors; iColor++ )
    {
        BYTE r = aPalette[iColor].peRed;
        aPalette[iColor].peRed  = aPalette[iColor].peBlue;
        aPalette[iColor].peBlue = r;
    }

    return m_pDD->CreatePalette( DDPCAPS_8BIT, aPalette, ppPalette, NULL );
}




//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C3DDisplay::UpdateBounds()
{
    if( m_bWindowed )
    {
        GetClientRect( m_hWnd, &m_rcWindow );
        ClientToScreen( m_hWnd, (POINT*)&m_rcWindow );
        ClientToScreen( m_hWnd, (POINT*)&m_rcWindow+1 );
    }
    else
    {
        SetRect( &m_rcWindow, 0, 0, GetSystemMetrics(SM_CXSCREEN),
                 GetSystemMetrics(SM_CYSCREEN) );
    }

    return S_OK;
}





//-----------------------------------------------------------------------------
// Name: C3DDisplay::InitClipper
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C3DDisplay::InitClipper()
{
    LPDIRECTDRAWCLIPPER pClipper;
    HRESULT hr;

    // Create a clipper when using GDI to draw on the primary surface 
    if( FAILED( hr = m_pDD->CreateClipper( 0, &pClipper, NULL ) ) )
        return hr;

    pClipper->SetHWnd( 0, m_hWnd );

    if( FAILED( hr = m_pddsFrontBuffer->SetClipper( pClipper ) ) )
        return hr;

    // We can release the clipper now since g_pDDSPrimary 
    // now maintains a ref count on the clipper
    SAFE_RELEASE( pClipper );

    return S_OK;
}





//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
C3DSurface::C3DSurface()
{
    m_pdds = NULL;
    m_bColorKeyed = NULL;
}




//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
C3DSurface::~C3DSurface()
{
    SAFE_RELEASE( m_pdds );
}




//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C3DSurface::Create( LPDIRECTDRAWSURFACE7 pdds )
{
    m_pdds = pdds;

    if( m_pdds )
    {
        m_pdds->AddRef();

        // Get the DDSURFACEDESC structure for this surface
        m_ddsd.dwSize = sizeof(m_ddsd);
        m_pdds->GetSurfaceDesc( &m_ddsd );
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C3DSurface::Create( LPDIRECTDRAW7 pDD, DDSURFACEDESC2* pddsd )
{
    HRESULT hr;

    // Create the DDraw surface
    if( FAILED( hr = pDD->CreateSurface( pddsd, &m_pdds, NULL ) ) )
        return hr;

    // Prepare the DDSURFACEDESC structure
    m_ddsd.dwSize = sizeof(m_ddsd);

    // Get the DDSURFACEDESC structure for this surface
    m_pdds->GetSurfaceDesc( &m_ddsd );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C3DSurface::Destroy()
{
    SAFE_RELEASE( m_pdds );
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: C3DSurface::DrawBitmap()
// Desc: Draws a bitmap over an entire DirectDrawSurface, stretching the 
//       bitmap if nessasary
//-----------------------------------------------------------------------------
HRESULT C3DSurface::DrawBitmap( HBITMAP hBMP, 
                              DWORD dwBMPOriginX, DWORD dwBMPOriginY, 
                              DWORD dwBMPWidth, DWORD dwBMPHeight )
{
    HDC            hDCImage;
    HDC            hDC;
    BITMAP         bmp;
    DDSURFACEDESC2 ddsd;
    HRESULT        hr;

    if( hBMP == NULL || m_pdds == NULL )
        return E_INVALIDARG;

    // Make sure this surface is restored.
    if( FAILED( hr = m_pdds->Restore() ) )
        return hr;

    // Get the surface.description
    ddsd.dwSize  = sizeof(ddsd);
    m_pdds->GetSurfaceDesc( &ddsd );

    if( ddsd.ddpfPixelFormat.dwFlags == DDPF_FOURCC )
        return E_NOTIMPL;

    // Select bitmap into a memoryDC so we can use it.
    hDCImage = CreateCompatibleDC( NULL );
    if( NULL == hDCImage )
        return E_FAIL;

    SelectObject( hDCImage, hBMP );

    // Get size of the bitmap
    GetObject( hBMP, sizeof(bmp), &bmp );

    // Use the passed size, unless zero
    dwBMPWidth  = ( dwBMPWidth  == 0 ) ? bmp.bmWidth  : dwBMPWidth;     
    dwBMPHeight = ( dwBMPHeight == 0 ) ? bmp.bmHeight : dwBMPHeight;

    // Stretch the bitmap to cover this surface
    if( FAILED( hr = m_pdds->GetDC( &hDC ) ) )
        return hr;

    StretchBlt( hDC, 0, 0, 
                ddsd.dwWidth, ddsd.dwHeight, 
                hDCImage, dwBMPOriginX, dwBMPOriginY,
                dwBMPWidth, dwBMPHeight, SRCCOPY );

    if( FAILED( hr = m_pdds->ReleaseDC( hDC ) ) )
        return hr;

    DeleteDC( hDCImage );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: C3DSurface::DrawText()
// Desc: Draws a text string on a DirectDraw surface using hFont or the default
//       GDI font if hFont is NULL.  
//-----------------------------------------------------------------------------
HRESULT C3DSurface::DrawText( HFONT hFont, TCHAR* strText, 
                            DWORD dwOriginX, DWORD dwOriginY,
                            COLORREF crBackground, COLORREF crForeground )
{
    HDC     hDC = NULL;
    HRESULT hr;

    if( m_pdds == NULL || strText == NULL )
        return E_INVALIDARG;

    // Make sure this surface is restored.
    if( FAILED( hr = m_pdds->Restore() ) )
        return hr;

    if( FAILED( hr = m_pdds->GetDC( &hDC ) ) )
        return hr;

    // Set the background and foreground color
    SetBkColor( hDC, crBackground );
    SetTextColor( hDC, crForeground );

    if( hFont )
        SelectObject( hDC, hFont );

    // Use GDI to draw the text on the surface
    TextOut( hDC, dwOriginX, dwOriginY, strText, _tcslen(strText) );

    if( FAILED( hr = m_pdds->ReleaseDC( hDC ) ) )
        return hr;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: C3DSurface::ReDrawBitmapOnSurface()
// Desc: Load a bitmap from a file or resource into a DirectDraw surface.
//       normaly used to re-load a surface after a restore.
//-----------------------------------------------------------------------------
HRESULT C3DSurface::DrawBitmap( TCHAR* strBMP, 
                              DWORD dwDesiredWidth, DWORD dwDesiredHeight  )
{
    HBITMAP hBMP;
    HRESULT hr;

    if( m_pdds == NULL || strBMP == NULL )
        return E_INVALIDARG;

    //  Try to load the bitmap as a resource, if that fails, try it as a file
    hBMP = (HBITMAP) LoadImage( GetModuleHandle(NULL), strBMP, 
                                IMAGE_BITMAP, dwDesiredWidth, dwDesiredHeight, 
                                LR_CREATEDIBSECTION );
    if( hBMP == NULL )
    {
        hBMP = (HBITMAP) LoadImage( NULL, strBMP, IMAGE_BITMAP, 
                                    dwDesiredWidth, dwDesiredHeight, 
                                    LR_LOADFROMFILE | LR_CREATEDIBSECTION );
        if( hBMP == NULL )
            return E_FAIL;
    }

    // Draw the bitmap on this surface
    if( FAILED( hr = DrawBitmap( hBMP, 0, 0, 0, 0 ) ) )
    {
        DeleteObject( hBMP );
        return hr;
    }

    DeleteObject( hBMP );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT C3DSurface::SetColorKey( DWORD dwColorKey )
{
    if( NULL == m_pdds )
        return E_POINTER;

    m_bColorKeyed = TRUE;

    DDCOLORKEY ddck;
    ddck.dwColorSpaceLowValue  = ConvertGDIColor( dwColorKey );
    ddck.dwColorSpaceHighValue = ConvertGDIColor( dwColorKey );
    
    return m_pdds->SetColorKey( DDCKEY_SRCBLT, &ddck );
}





//-----------------------------------------------------------------------------
// Name: C3DSurface::ConvertGDIColor()
// Desc: Converts a GDI color (0x00bbggrr) into the equivalent color on a 
//       DirectDrawSurface using its pixel format.  
//-----------------------------------------------------------------------------
DWORD C3DSurface::ConvertGDIColor( COLORREF dwGDIColor )
{
    if( m_pdds == NULL )
	    return 0x00000000;

    COLORREF       rgbT;
    HDC            hdc;
    DWORD          dw = CLR_INVALID;
    DDSURFACEDESC2 ddsd;
    HRESULT        hr;

    //  Use GDI SetPixel to color match for us
    if( dwGDIColor != CLR_INVALID && m_pdds->GetDC(&hdc) == DD_OK)
    {
        rgbT = GetPixel(hdc, 0, 0);     // Save current pixel value
        SetPixel(hdc, 0, 0, dwGDIColor);       // Set our value
        m_pdds->ReleaseDC(hdc);
    }

    // Now lock the surface so we can read back the converted color
    ddsd.dwSize = sizeof(ddsd);
    hr = m_pdds->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL );
    if( hr == DD_OK)
    {
        dw = *(DWORD *) ddsd.lpSurface; 
        if( ddsd.ddpfPixelFormat.dwRGBBitCount < 32 ) // Mask it to bpp
            dw &= ( 1 << ddsd.ddpfPixelFormat.dwRGBBitCount ) - 1;  
        m_pdds->Unlock(NULL);
    }

    //  Now put the color that was there back.
    if( dwGDIColor != CLR_INVALID && m_pdds->GetDC(&hdc) == DD_OK )
    {
        SetPixel( hdc, 0, 0, rgbT );
        m_pdds->ReleaseDC(hdc);
    }
    
    return dw;    
}




//-----------------------------------------------------------------------------
// Name: C3DSurface::GetBitMaskInfo()
// Desc: Returns the number of bits and the shift in the bit mask
//-----------------------------------------------------------------------------
HRESULT C3DSurface::GetBitMaskInfo( DWORD dwBitMask, DWORD* pdwShift, DWORD* pdwBits )
{
    DWORD dwShift = 0;
    DWORD dwBits  = 0; 

    if( pdwShift == NULL || pdwBits == NULL )
        return E_INVALIDARG;

    if( dwBitMask )
    {
        while( (dwBitMask & 1) == 0 )
        {
            dwShift++;
            dwBitMask >>= 1;
        }
    }

    while( (dwBitMask & 1) != 0 )
    {
        dwBits++;
        dwBitMask >>= 1;
    }

    *pdwShift = dwShift;
    *pdwBits  = dwBits;

    return S_OK;
}


extern char g_szApplicationName[256];


//-----------------------------------------------------------------------------
// Name: C3DUtil_GetInstalledMediaPath()
// Desc: Returns the media path from the install directory
//
// NOTE: This has been modified to use the install directory's media path
// (rjk)
//-----------------------------------------------------------------------------
const TCHAR* C3DUtil_GetFT2MediaPath()
{
    static TCHAR strNull[2] = _T("");
    static TCHAR strPath[MAX_PATH];
    DWORD dwType;
    DWORD dwSize = MAX_PATH;
    HKEY  hKey;
	char  szRegistryPath[256];

    // Open the appropriate registry key
	sprintf(szRegistryPath,"%s\\%s\\%s", "Software", "FT2", g_szApplicationName); 
  LONG lResult = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                              _T(szRegistryPath),
                              0, KEY_READ, &hKey );
  if( ERROR_SUCCESS != lResult )
      return strNull;

  lResult = RegQueryValueEx( hKey, _T("Media"), NULL,
                            &dwType, (BYTE*)strPath, &dwSize );
  RegCloseKey( hKey );

  if( ERROR_SUCCESS != lResult )
      return strNull;

  return strPath;
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_FindMediaFile()
// Desc: Returns a valid path to a media file
//-----------------------------------------------------------------------------
HRESULT C3DUtil_FindMediaFile( TCHAR* strPath, TCHAR* strFilename )
{
    HANDLE file;

    if( NULL==strFilename || NULL==strPath )
        return E_INVALIDARG;

    // Check if the file exists in the current directory
    _tcscpy( strPath, strFilename );

    file = CreateFile( strPath, GENERIC_READ, FILE_SHARE_READ, NULL, 
                       OPEN_EXISTING, 0, NULL );
    if( INVALID_HANDLE_VALUE != file )
    {
        CloseHandle( file );
        return S_OK;
    }
    
    // Check if the file exists in the current directory
    _stprintf( strPath, _T("%s%s"), C3DUtil_GetFT2MediaPath(), strFilename );

    file = CreateFile( strPath, GENERIC_READ, FILE_SHARE_READ, NULL, 
                       OPEN_EXISTING, 0, NULL );
    if( INVALID_HANDLE_VALUE != file )
    {
        CloseHandle( file );
        return S_OK;
    }

    // On failure, just return the file as the path
    _tcscpy( strPath, strFilename );
    return E_FAIL;
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_ReadStringRegKey()
// Desc: Helper function to read a registry key string
//-----------------------------------------------------------------------------
HRESULT C3DUtil_ReadStringRegKey( HKEY hKey, TCHAR* strRegName, TCHAR* strValue, 
                                 DWORD dwLength, TCHAR* strDefault )
{
    DWORD dwType;

    if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
                                          (BYTE*)strValue, &dwLength ) )
    {
        _tcscpy( strValue, strDefault );
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_WriteStringRegKey()
// Desc: Helper function to write a registry key string
//-----------------------------------------------------------------------------
HRESULT C3DUtil_WriteStringRegKey( HKEY hKey, TCHAR* strRegName,
                                  TCHAR* strValue )
{
    if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_SZ, 
                                        (BYTE*)strValue, 
                                        (_tcslen(strValue)+1)*sizeof(TCHAR) ) )
        return E_FAIL;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_ReadIntRegKey()
// Desc: Helper function to read a registry key int
//-----------------------------------------------------------------------------
HRESULT C3DUtil_ReadIntRegKey( HKEY hKey, TCHAR* strRegName, DWORD* pdwValue, 
                              DWORD dwDefault )
{
    DWORD dwType;
    DWORD dwLength = sizeof(DWORD);

    if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
                                          (BYTE*)pdwValue, &dwLength ) )
    {
        *pdwValue = dwDefault;
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_WriteIntRegKey()
// Desc: Helper function to write a registry key int
//-----------------------------------------------------------------------------
HRESULT C3DUtil_WriteIntRegKey( HKEY hKey, TCHAR* strRegName, DWORD dwValue )
{
    if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_DWORD, 
                                        (BYTE*)&dwValue, sizeof(DWORD) ) )
        return E_FAIL;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_ReadBoolRegKey()
// Desc: Helper function to read a registry key BOOL
//-----------------------------------------------------------------------------
HRESULT C3DUtil_ReadBoolRegKey( HKEY hKey, TCHAR* strRegName, BOOL* pbValue, 
                              BOOL bDefault )
{
    DWORD dwType;
    DWORD dwLength = sizeof(BOOL);

    if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
                                          (BYTE*)pbValue, &dwLength ) )
    {
        *pbValue = bDefault;
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_WriteBoolRegKey()
// Desc: Helper function to write a registry key BOOL
//-----------------------------------------------------------------------------
HRESULT C3DUtil_WriteBoolRegKey( HKEY hKey, TCHAR* strRegName, BOOL bValue )
{
    if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_DWORD, 
                                        (BYTE*)&bValue, sizeof(BOOL) ) )
        return E_FAIL;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_ReadGuidRegKey()
// Desc: Helper function to read a registry key guid
//-----------------------------------------------------------------------------
HRESULT C3DUtil_ReadGuidRegKey( HKEY hKey, TCHAR* strRegName, GUID* pGuidValue, 
                               GUID& guidDefault )
{
    DWORD dwType;
    DWORD dwLength = sizeof(GUID);

    if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
                                          (LPBYTE) pGuidValue, &dwLength ) )
    {
        *pGuidValue = guidDefault;
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_WriteGuidRegKey()
// Desc: Helper function to write a registry key guid
//-----------------------------------------------------------------------------
HRESULT C3DUtil_WriteGuidRegKey( HKEY hKey, TCHAR* strRegName, GUID guidValue )
{
    if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_BINARY, 
                                        (BYTE*)&guidValue, sizeof(GUID) ) )
        return E_FAIL;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_Timer()
// Desc: Performs timer opertations. Use the following commands:
//          TIMER_RESET           - to reset the timer
//          TIMER_START           - to start the timer
//          TIMER_STOP            - to stop (or pause) the timer
//          TIMER_ADVANCE         - to advance the timer by 0.1 seconds
//          TIMER_GETABSOLUTETIME - to get the absolute system time
//          TIMER_GETAPPTIME      - to get the current time
//          TIMER_GETELAPSEDTIME  - to get the time that elapsed between 
//                                  TIMER_GETELAPSEDTIME calls
//-----------------------------------------------------------------------------
FLOAT __stdcall C3DUtil_Timer( TIMER_COMMAND command )
{
    static BOOL     m_bTimerInitialized = FALSE;
    static BOOL     m_bUsingQPF         = FALSE;
    static LONGLONG m_llQPFTicksPerSec  = 0;

    // Initialize the timer
    if( FALSE == m_bTimerInitialized )
    {
        m_bTimerInitialized = TRUE;

        // Use QueryPerformanceFrequency() to get frequency of timer.  If QPF is
        // not supported, we will timeGetTime() which returns milliseconds.
        LARGE_INTEGER qwTicksPerSec;
        m_bUsingQPF = QueryPerformanceFrequency( &qwTicksPerSec );
        if( m_bUsingQPF )
            m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
    }

    if( m_bUsingQPF )
    {
        static LONGLONG m_llStopTime        = 0;
        static LONGLONG m_llLastElapsedTime = 0;
        static LONGLONG m_llBaseTime        = 0;
        double fTime;
        double fElapsedTime;
        LARGE_INTEGER qwTime;
        
        // Get either the current time or the stop time, depending
        // on whether we're stopped and what command was sent
        if( m_llStopTime != 0 && command != TIMER_START && command != TIMER_GETABSOLUTETIME)
            qwTime.QuadPart = m_llStopTime;
        else
            QueryPerformanceCounter( &qwTime );

        // Return the elapsed time
        if( command == TIMER_GETELAPSEDTIME )
        {
            fElapsedTime = (double) ( qwTime.QuadPart - m_llLastElapsedTime ) / (double) m_llQPFTicksPerSec;
            m_llLastElapsedTime = qwTime.QuadPart;
            return (FLOAT) fElapsedTime;
        }
    
        // Return the current time
        if( command == TIMER_GETAPPTIME )
        {
            double fAppTime = (double) ( qwTime.QuadPart - m_llBaseTime ) / (double) m_llQPFTicksPerSec;
            return (FLOAT) fAppTime;
        }
    
        // Reset the timer
        if( command == TIMER_RESET )
        {
            m_llBaseTime        = qwTime.QuadPart;
            m_llLastElapsedTime = qwTime.QuadPart;
            return 0.0f;
        }
    
        // Start the timer
        if( command == TIMER_START )
        {
            m_llBaseTime += qwTime.QuadPart - m_llStopTime;
            m_llStopTime = 0;
            m_llLastElapsedTime = qwTime.QuadPart;
            return 0.0f;
        }
    
        // Stop the timer
        if( command == TIMER_STOP )
        {
            m_llStopTime = qwTime.QuadPart;
            m_llLastElapsedTime = qwTime.QuadPart;
            return 0.0f;
        }
    
        // Advance the timer by 1/10th second
        if( command == TIMER_ADVANCE )
        {
            m_llStopTime += m_llQPFTicksPerSec/10;
            return 0.0f;
        }

        if( command == TIMER_GETABSOLUTETIME )
        {
            fTime = qwTime.QuadPart / (double) m_llQPFTicksPerSec;
            return (FLOAT) fTime;
        }

        return -1.0f; // Invalid command specified
    }
    else
    {
        // Get the time using timeGetTime()
        static double m_fLastElapsedTime  = 0.0;
        static double m_fBaseTime         = 0.0;
        static double m_fStopTime         = 0.0;
        double fTime;
        double fElapsedTime;
        
        // Get either the current time or the stop time, depending
        // on whether we're stopped and what command was sent
        if( m_fStopTime != 0.0 && command != TIMER_START && command != TIMER_GETABSOLUTETIME)
            fTime = m_fStopTime;
        else
            fTime = timeGetTime() * 0.001;
    
        // Return the elapsed time
        if( command == TIMER_GETELAPSEDTIME )
        {   
            fElapsedTime = (double) (fTime - m_fLastElapsedTime);
            m_fLastElapsedTime = fTime;
            return (FLOAT) fElapsedTime;
        }
    
        // Return the current time
        if( command == TIMER_GETAPPTIME )
        {
            return (FLOAT) (fTime - m_fBaseTime);
        }
    
        // Reset the timer
        if( command == TIMER_RESET )
        {
            m_fBaseTime         = fTime;
            m_fLastElapsedTime  = fTime;
            return 0.0f;
        }
    
        // Start the timer
        if( command == TIMER_START )
        {
            m_fBaseTime += fTime - m_fStopTime;
            m_fStopTime = 0.0f;
            m_fLastElapsedTime  = fTime;
            return 0.0f;
        }
    
        // Stop the timer
        if( command == TIMER_STOP )
        {
            m_fStopTime = fTime;
            return 0.0f;
        }
    
        // Advance the timer by 1/10th second
        if( command == TIMER_ADVANCE )
        {
            m_fStopTime += 0.1f;
            return 0.0f;
        }

        if( command == TIMER_GETABSOLUTETIME )
        {
            return (FLOAT) fTime;
        }

        return -1.0f; // Invalid command specified
    }
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_ConvertAnsiStringToWide()
// Desc: This is a UNICODE conversion utility to convert a CHAR string into a
//       WCHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
VOID C3DUtil_ConvertAnsiStringToWide( WCHAR* wstrDestination, const CHAR* strSource, 
                                     int cchDestChar )
{
    if( wstrDestination==NULL || strSource==NULL )
        return;

    if( cchDestChar == -1 )
        cchDestChar = strlen(strSource)+1;

    MultiByteToWideChar( CP_ACP, 0, strSource, -1, 
                         wstrDestination, cchDestChar-1 );

    wstrDestination[cchDestChar-1] = 0;
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_ConvertWideStringToAnsi()
// Desc: This is a UNICODE conversion utility to convert a WCHAR string into a
//       CHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
VOID C3DUtil_ConvertWideStringToAnsi( CHAR* strDestination, const WCHAR* wstrSource, 
                                     int cchDestChar )
{
    if( strDestination==NULL || wstrSource==NULL )
        return;

    if( cchDestChar == -1 )
        cchDestChar = wcslen(wstrSource)+1;

    WideCharToMultiByte( CP_ACP, 0, wstrSource, -1, strDestination, 
                         cchDestChar-1, NULL, NULL );

    strDestination[cchDestChar-1] = 0;
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_ConvertGenericStringToAnsi()
// Desc: This is a UNICODE conversion utility to convert a TCHAR string into a
//       CHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
VOID C3DUtil_ConvertGenericStringToAnsi( CHAR* strDestination, const TCHAR* tstrSource, 
                                        int cchDestChar )
{
    if( strDestination==NULL || tstrSource==NULL )
        return;

#ifdef _UNICODE
    C3DUtil_ConvertWideStringToAnsi( strDestination, tstrSource, cchDestChar );
#else
    if( cchDestChar == -1 )
    	strcpy( strDestination, tstrSource );
    else
    	strncpy( strDestination, tstrSource, cchDestChar );
#endif
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_ConvertGenericStringToWide()
// Desc: This is a UNICODE conversion utility to convert a TCHAR string into a
//       WCHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
VOID C3DUtil_ConvertGenericStringToWide( WCHAR* wstrDestination, const TCHAR* tstrSource, 
                                        int cchDestChar )
{
    if( wstrDestination==NULL || tstrSource==NULL )
        return;

#ifdef _UNICODE
    if( cchDestChar == -1 )
	    wcscpy( wstrDestination, tstrSource );
    else
	    wcsncpy( wstrDestination, tstrSource, cchDestChar );
#else
    C3DUtil_ConvertAnsiStringToWide( wstrDestination, tstrSource, cchDestChar );
#endif
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_ConvertAnsiStringToGeneric()
// Desc: This is a UNICODE conversion utility to convert a CHAR string into a
//       TCHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
VOID C3DUtil_ConvertAnsiStringToGeneric( TCHAR* tstrDestination, const CHAR* strSource, 
                                        int cchDestChar )
{
    if( tstrDestination==NULL || strSource==NULL )
        return;
        
#ifdef _UNICODE
    C3DUtil_ConvertAnsiStringToWide( tstrDestination, strSource, cchDestChar );
#else
    if( cchDestChar == -1 )
    	strcpy( tstrDestination, strSource );
    else
    	strncpy( tstrDestination, strSource, cchDestChar );
#endif
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_ConvertAnsiStringToGeneric()
// Desc: This is a UNICODE conversion utility to convert a WCHAR string into a
//       TCHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
VOID C3DUtil_ConvertWideStringToGeneric( TCHAR* tstrDestination, const WCHAR* wstrSource, 
                                        int cchDestChar )
{
    if( tstrDestination==NULL || wstrSource==NULL )
        return;

#ifdef _UNICODE
    if( cchDestChar == -1 )
	    wcscpy( tstrDestination, wstrSource );
    else
	    wcsncpy( tstrDestination, wstrSource, cchDestChar );
#else
    C3DUtil_ConvertWideStringToAnsi( tstrDestination, wstrSource, cchDestChar );
#endif
}




//-----------------------------------------------------------------------------
// Name: _DbgOut()
// Desc: Outputs a message to the debug stream
//-----------------------------------------------------------------------------
HRESULT _DbgOut( TCHAR* strFile, DWORD dwLine, HRESULT hr, TCHAR* strMsg )
{
    TCHAR buffer[256];
    wsprintf( buffer, _T("%s(%ld): "), strFile, dwLine );
    OutputDebugString( buffer );
    OutputDebugString( strMsg );

    if( hr )
    {
        wsprintf( buffer, _T("(hr=%08lx)\n"), hr );
        OutputDebugString( buffer );
    }

    OutputDebugString( _T("\n") );

    return hr;
}




//-----------------------------------------------------------------------------
// Name: C3DUtil_Trace()
// Desc: Outputs to the debug stream a formatted string with a variable-
//       argument list.
//-----------------------------------------------------------------------------
VOID C3DUtil_Trace( TCHAR* strMsg, ... )
{
#if defined(DEBUG) | defined(_DEBUG)
    TCHAR strBuffer[512];
    
    va_list args;
    va_start(args, strMsg);
    _vsntprintf( strBuffer, 512, strMsg, args );
    va_end(args);

    OutputDebugString( strBuffer );
#endif
}


void DisplayErrorMessage(char* szErrorMessage)
{

	// Display the string.
	MessageBox( NULL, szErrorMessage, "Notice", MB_OK|MB_ICONINFORMATION );

}

void DisplayErrorCode(long err)
{
	char* lpMsgBuf;

	// check to see if it's a dx error code
	lpMsgBuf = ErrorToString(err);
	if (!lpMsgBuf)
	{
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			err,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
		);
	}

	// Display the string.
	MessageBox( NULL, lpMsgBuf, "Notice", MB_OK|MB_ICONINFORMATION );

	// Free the buffer.
	LocalFree( lpMsgBuf );
}

char* ErrorToString(HRESULT error)
{
  switch(error)
  {
    case DD_OK:
  // Also includes D3D_OK and D3DRM_OK and S_OK and ERR_NONE:
      return "No error.\0";

	// local error messages
	case ERR_MEMORY:
		return "Unable to allocate or deallocate a memory block.\0";
	case ERR_NETWORK:
		return "A network error has occured.\0";
	case ERR_INIT:
		return "An error has occured during initialization.\0";
	case ERR_GENERAL:
		return "An unspecified error has occured.\0";
	case ERR_QUEUEFULL:
		return "An internal queue has reached maximum limits.\0";
	case ERR_QUEUEEMPTY:
		return "Attempted to access an empty internal queue..\0";
	case ERR_FILENOTFOUND:
		return "The specified file was not found.\0";
	case ERR_BUSY:
		return "An attempt was made to access an already active process.\0";
	case ERR_FILENOTOPEN:
		return "The specified file was not open for reading/writing.\0";
	case ERR_PATHNOTFOUND:
		return "The specified path was not found.\0";

	// Direct3D error messages
  case D3DERR_WRONGTEXTUREFORMAT:
      return "D3D Error: wrong texture format.\0";
  case D3DERR_UNSUPPORTEDCOLOROPERATION:
      return "D3D Error: unsupported color operation.\0";
  case D3DERR_UNSUPPORTEDCOLORARG:
      return "D3D Error: unsupported color argument.\0";
  case D3DERR_UNSUPPORTEDALPHAOPERATION:
      return "D3D Error: unsupported alpha operation.\0";
  case D3DERR_UNSUPPORTEDALPHAARG:
      return "D3D Error: unsupported alpha argument.\0";
  case D3DERR_TOOMANYOPERATIONS:
      return "D3D Error: too many operations.\0";
  case D3DERR_CONFLICTINGTEXTUREFILTER:
      return "TD3D Error: conflicting texture filter.\0";
  case D3DERR_UNSUPPORTEDFACTORVALUE:
      return "TD3D Error: unsupported factor value.\0";
  case D3DERR_CONFLICTINGRENDERSTATE:
      return "D3D Error: conflicting renderstate.\0";
  case D3DERR_UNSUPPORTEDTEXTUREFILTER:
      return "D3D Error: unsupported texture filter.\0";
  case D3DERR_CONFLICTINGTEXTUREPALETTE:
      return "D3D Error: conflicting texture palette.\0";
  case D3DERR_DRIVERINTERNALERROR:
      return "D3D Error: driver internal error.\0";
  case D3DERR_NOTFOUND:
      return "D3D Error: not found.\0";
  case D3DERR_MOREDATA:
      return "D3D Error: more data.\0";
  case D3DERR_DEVICELOST:
      return "D3D Error: device lost.\0";
  case D3DERR_DEVICENOTRESET:
      return "D3D Error: device not reset.\0";
  case D3DERR_NOTAVAILABLE:
      return "D3D Error: not available.\0";
  // also includes DDERR_OUTOFVIDEOMEMORY
  case D3DERR_OUTOFVIDEOMEMORY:
      return "D3D Error: out of video memory.\0";
  case D3DERR_INVALIDDEVICE:
      return "D3D Error: invalid device.\0";
  case D3DERR_INVALIDCALL:
      return "D3D Error: invalid call.\0";
  case D3DERR_DRIVERINVALIDCALL:
      return "D3D Error: driver invalid call.\0";

  // DirectDraw error messages
  case DDERR_ALREADYINITIALIZED:
    return "This object is already initialized.\0";
  case DDERR_BLTFASTCANTCLIP:
    return "Return if a clipper object is attached to the source surface passed into a BltFast call.\0";
  case DDERR_CANNOTATTACHSURFACE:
    return "This surface can not be attached to the requested surface.\0";
  case DDERR_CANNOTDETACHSURFACE:
    return "This surface can not be detached from the requested surface.\0";
  case DDERR_CANTCREATEDC:
    return "Windows can not create any more DCs.\0";
  case DDERR_CANTDUPLICATE:
    return "Can't duplicate primary & 3D surfaces, or surfaces that are implicitly created.\0";
  case DDERR_CLIPPERISUSINGHWND:
    return "An attempt was made to set a cliplist for a clipper object that is already monitoring an hwnd.\0";
  case DDERR_COLORKEYNOTSET:
    return "No src color key specified for this operation.\0";
  case DDERR_CURRENTLYNOTAVAIL:
    return "Support is currently not available.\0";
  case DDERR_DIRECTDRAWALREADYCREATED:
    return "A DirectDraw object representing this driver has already been created for this process.\0";
  case DDERR_EXCEPTION:
    return "An exception was encountered while performing the requested operation.\0";
  case DDERR_EXCLUSIVEMODEALREADYSET:
    return "An attempt was made to set the cooperative level when it was already set to exclusive.\0";
  case DDERR_GENERIC:
    return "Generic failure.\0";
  case DDERR_HEIGHTALIGN:
    return "Height of rectangle provided is not a multiple of reqd alignment.\0";
  case DDERR_HWNDALREADYSET:
    return "The CooperativeLevel HWND has already been set. It can not be reset while the process has surfaces or palettes created.\0";
  case DDERR_HWNDSUBCLASSED:
    return "HWND used by DirectDraw CooperativeLevel has been subclassed, this prevents DirectDraw from restoring state.\0";
  case DDERR_IMPLICITLYCREATED:
    return "This surface can not be restored because it is an implicitly created surface.\0";
  case DDERR_INCOMPATIBLEPRIMARY:
    return "Unable to match primary surface creation request with existing primary surface.\0";
  case DDERR_INVALIDCAPS:
    return "One or more of the caps bits passed to the callback are incorrect.\0";
  case DDERR_INVALIDCLIPLIST:
    return "DirectDraw does not support the provided cliplist.\0";
  case DDERR_INVALIDDIRECTDRAWGUID:
    return "The GUID passed to DirectDrawCreate is not a valid DirectDraw driver identifier.\0";
  case DDERR_INVALIDMODE:
    return "DirectDraw does not support the requested mode.\0";
  case DDERR_INVALIDOBJECT:
    return "DirectDraw received a pointer that was an invalid DIRECTDRAW object.\0";
  case DDERR_INVALIDPARAMS:
    return "One or more of the parameters passed to the function are incorrect.\0";
  case DDERR_INVALIDPIXELFORMAT:
    return "The pixel format was invalid as specified.\0";
  case DDERR_INVALIDPOSITION:
    return "Returned when the position of the overlay on the destination is no longer legal for that destination.\0";
  case DDERR_INVALIDRECT:
    return "Rectangle provided was invalid.\0";
  case DDERR_LOCKEDSURFACES:
    return "Operation could not be carried out because one or more surfaces are locked.\0";
  case DDERR_NO3D:
    return "There is no 3D present.\0";
  case DDERR_NOALPHAHW:
    return "Operation could not be carried out because there is no alpha accleration hardware present or available.\0";
  case DDERR_NOBLTHW:
    return "No blitter hardware present.\0";
  case DDERR_NOCLIPLIST:
    return "No cliplist available.\0";
  case DDERR_NOCLIPPERATTACHED:
    return "No clipper object attached to surface object.\0";
  case DDERR_NOCOLORCONVHW:
    return "Operation could not be carried out because there is no color conversion hardware present or available.\0";
  case DDERR_NOCOLORKEY:
    return "Surface doesn't currently have a color key\0";
  case DDERR_NOCOLORKEYHW:
    return "Operation could not be carried out because there is no hardware support of the destination color key.\0";
  case DDERR_NOCOOPERATIVELEVELSET:
    return "Create function called without DirectDraw object method SetCooperativeLevel being called.\0";
  case DDERR_NODC:
    return "No DC was ever created for this surface.\0";
  case DDERR_NODDROPSHW:
    return "No DirectDraw ROP hardware.\0";
  case DDERR_NODIRECTDRAWHW:
    return "A hardware-only DirectDraw object creation was attempted but the driver did not support any hardware.\0";
  case DDERR_NOEMULATION:
    return "Software emulation not available.\0";
  case DDERR_NOEXCLUSIVEMODE:
    return "Operation requires the application to have exclusive mode but the application does not have exclusive mode.\0";
  case DDERR_NOFLIPHW:
    return "Flipping visible surfaces is not supported.\0";
  case DDERR_NOGDI:
    return "There is no GDI present.\0";
  case DDERR_NOHWND:
    return "Clipper notification requires an HWND or no HWND has previously been set as the CooperativeLevel HWND.\0";
  case DDERR_NOMIRRORHW:
    return "Operation could not be carried out because there is no hardware present or available.\0";
  case DDERR_NOOVERLAYDEST:
    return "Returned when GetOverlayPosition is called on an overlay that UpdateOverlay has never been called on to establish a destination.\0";
  case DDERR_NOOVERLAYHW:
    return "Operation could not be carried out because there is no overlay hardware present or available.\0";
  case DDERR_NOPALETTEATTACHED:
    return "No palette object attached to this surface.\0";
  case DDERR_NOPALETTEHW:
    return "No hardware support for 16 or 256 color palettes.\0";
  case DDERR_NORASTEROPHW:
    return "Operation could not be carried out because there is no appropriate raster op hardware present or available.\0";
  case DDERR_NOROTATIONHW:
    return "Operation could not be carried out because there is no rotation hardware present or available.\0";
  case DDERR_NOSTRETCHHW:
    return "Operation could not be carried out because there is no hardware support for stretching.\0";
  case DDERR_NOT4BITCOLOR:
    return "DirectDrawSurface is not in 4 bit color palette and the requested operation requires 4 bit color palette.\0";
  case DDERR_NOT4BITCOLORINDEX:
    return "DirectDrawSurface is not in 4 bit color index palette and the requested operation requires 4 bit color index palette.\0";
  case DDERR_NOT8BITCOLOR:
    return "DirectDrawSurface is not in 8 bit color mode and the requested operation requires 8 bit color.\0";
  case DDERR_NOTAOVERLAYSURFACE:
    return "Returned when an overlay member is called for a non-overlay surface.\0";
  case DDERR_NOTEXTUREHW:
    return "Operation could not be carried out because there is no texture mapping hardware present or available.\0";
  case DDERR_NOTFLIPPABLE:
    return "An attempt has been made to flip a surface that is not flippable.\0";
  case DDERR_NOTFOUND:
    return "Requested item was not found.\0";
  case DDERR_NOTLOCKED:
    return "Surface was not locked.  An attempt to unlock a surface that was not locked at all, or by this process, has been attempted.\0";
  case DDERR_NOTPALETTIZED:
    return "The surface being used is not a palette-based surface.\0";
  case DDERR_NOVSYNCHW:
    return "Operation could not be carried out because there is no hardware support for vertical blank synchronized operations.\0";
  case DDERR_NOZBUFFERHW:
    return "Operation could not be carried out because there is no hardware support for zbuffer blitting.\0";
  case DDERR_NOZOVERLAYHW:
    return "Overlay surfaces could not be z layered based on their BltOrder because the hardware does not support z layering of overlays.\0";
  case DDERR_OUTOFCAPS:
    return "The hardware needed for the requested operation has already been allocated.\0";
  case DDERR_OUTOFMEMORY:
    return "DirectDraw does not have enough memory to perform the operation.\0";
  case DDERR_OVERLAYCANTCLIP:
    return "The hardware does not support clipped overlays.\0";
  case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:
    return "Can only have ony color key active at one time for overlays.\0";
  case DDERR_OVERLAYNOTVISIBLE:
    return "Returned when GetOverlayPosition is called on a hidden overlay.\0";
  case DDERR_PALETTEBUSY:
    return "Access to this palette is being refused because the palette is already locked by another thread.\0";
  case DDERR_PRIMARYSURFACEALREADYEXISTS:
    return "This process already has created a primary surface.\0";
  case DDERR_REGIONTOOSMALL:
    return "Region passed to Clipper::GetClipList is too small.\0";
  case DDERR_SURFACEALREADYATTACHED:
    return "This surface is already attached to the surface it is being attached to.\0";
  case DDERR_SURFACEALREADYDEPENDENT:
    return "This surface is already a dependency of the surface it is being made a dependency of.\0";
  case DDERR_SURFACEBUSY:
    return "Access to this surface is being refused because the surface is already locked by another thread.\0";
  case DDERR_SURFACEISOBSCURED:
    return "Access to surface refused because the surface is obscured.\0";
  case DDERR_SURFACELOST:
    return "Access to this surface is being refused because the surface memory is gone. The DirectDrawSurface object representing this surface should have Restore called on it.\0";
  case DDERR_SURFACENOTATTACHED:
    return "The requested surface is not attached.\0";
  case DDERR_TOOBIGHEIGHT:
    return "Height requested by DirectDraw is too large.\0";
  case DDERR_TOOBIGSIZE:
    return "Size requested by DirectDraw is too large, but the individual height and width are OK.\0";
  case DDERR_TOOBIGWIDTH:
    return "Width requested by DirectDraw is too large.\0";
  case DDERR_UNSUPPORTED:
    return "Action not supported.\0";
  case DDERR_UNSUPPORTEDFORMAT:
    return "FOURCC format requested is unsupported by DirectDraw.\0";
  case DDERR_UNSUPPORTEDMASK:
    return "Bitmask in the pixel format requested is unsupported by DirectDraw.\0";
  case DDERR_VERTICALBLANKINPROGRESS:
    return "Vertical blank is in progress.\0";
  case DDERR_WASSTILLDRAWING:
    return "Informs DirectDraw that the previous Blt which is transfering information to or from this Surface is incomplete.\0";
  case DDERR_WRONGMODE:
    return "This surface can not be restored because it was created in a different mode.\0";
  case DDERR_XALIGN:
    return "Rectangle provided was not horizontally aligned on required boundary.\0";

  // DirectX file error messages
  case DXFILEERR_BADOBJECT:
    return "DXFile error: Object is invalid.\0";
  case DXFILEERR_BADVALUE:
    return "DXFile error: Parameter is invalid.\0";
  case DXFILEERR_BADTYPE:
    return "DXFile error: Object type is invalid.\0";
  case DXFILEERR_BADSTREAMHANDLE:
    return "DXFile error: Stream handle is invalid.\0";
  case DXFILEERR_BADALLOC:
    return "DXFile error: Memory allocation failed.\0";
  case DXFILEERR_NOTFOUND:
    return "DXFile error: Object could not be found.\0";
  case DXFILEERR_NOTDONEYET:
    return "DXFile error: Operation has not completed.\0";
  case DXFILEERR_FILENOTFOUND:
    return "DXFile error: File could not be found.\0";
  case DXFILEERR_RESOURCENOTFOUND:
    return "DXFile error: Resource could not be found.\0";
  case DXFILEERR_URLNOTFOUND:
    return "DXFile error: URL could not be found.\0";
  case DXFILEERR_BADRESOURCE:
    return "DXFile error: Resource is invalid.\0";
  case DXFILEERR_BADFILETYPE:
    return "DXFile error: File is not a DirectX (.x) file.\0";
  case DXFILEERR_BADFILEVERSION:
    return "DXFile error: File version is not valid.\0";
  case DXFILEERR_BADFILEFLOATSIZE:
    return "DXFile error: Floating-point size is invalid.\0";
  case DXFILEERR_BADFILECOMPRESSIONTYPE:
    return "DXFile error: File compression type is invalid.\0";
  case DXFILEERR_BADFILE:
    return "DXFile error: File is invalid.\0";
  case DXFILEERR_PARSEERROR:
    return "DXFile error: File could not be parsed.\0";
  case DXFILEERR_NOTEMPLATE:
    return "DXFile error: No template available.\0";
  case DXFILEERR_BADARRAYSIZE:
    return "DXFile error: Array size is invalid.\0";
  case DXFILEERR_BADDATAREFERENCE:
    return "DXFile error: Data reference is invalid.\0";
  case DXFILEERR_INTERNALERROR:
    return "DXFile error: Internal error occurred.\0";
  case DXFILEERR_NOMOREOBJECTS:
    return "DXFile error: All objects have been enumerated.\0";
  case DXFILEERR_BADINTRINSICS:
    return "DXFile error: Intrinsics are invalid.\0";
  case DXFILEERR_NOMORESTREAMHANDLES:
    return "DXFile error: No stream handles are available.\0";
  case DXFILEERR_NOMOREDATA:
    return "DXFile error: No further data is available.\0";
  case DXFILEERR_BADCACHEFILE:
    return "DXFile error: The cache file containing the .x file data is invalid.\0";
  case DXFILEERR_NOINTERNET:
    return "DXFile error: Internet connection not found.\0";

  default:
		{
			static char buf[256];
			sprintf( buf, "An unknown error has occured: %x.", error);
	    return &buf[0];
		}
  }
}



//-----------------------------------------------------------------------------
// File: C3DUtil.h
//
// Desc: Helper functions and typing shortcuts for Direct3D programming.
//
//-----------------------------------------------------------------------------
#ifndef C3DUTIL_H
#define C3DUTIL_H
#include <D3D8.h>
#include <D3DX8Math.h>

//-----------------------------------------------------------------------------
// Classes defined in this header file 
//-----------------------------------------------------------------------------
class C3DDisplay;
class C3DSurface;
class C3DArcBall;

#include <DDraw.h>

//-----------------------------------------------------------------------------
// Miscellaneous helper functions
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }


//-----------------------------------------------------------------------------
// Flags for the C3DDisplay and C3DSurface methods
//-----------------------------------------------------------------------------
#define DSURFACELOCK_READ
#define DSURFACELOCK_WRITE

//////////////////////////////////////////////////////////////////
// error codes
enum ERROR_CODES
{
	ERR_NONE = 0,
	ERR_MEMORY,
	ERR_NETWORK,
	ERR_INIT,
	ERR_GENERAL,
	ERR_QUEUEFULL,
	ERR_QUEUEEMPTY,
	ERR_FILENOTFOUND,
	ERR_BUSY,
	ERR_FILENOTOPEN,
	ERR_PATHNOTFOUND,
};

//-----------------------------------------------------------------------------
// display a win32 error message from the GetLastError function
//-----------------------------------------------------------------------------
void DisplayErrorCode(long err);
void DisplayErrorMessage(char* szErrorMessage);
char* ErrorToString(HRESULT error);


//-----------------------------------------------------------------------------
// Name: C3DUtil_InitMaterial()
// Desc: Initializes a D3DMATERIAL8 structure, setting the diffuse and ambient
//       colors. It does not set emissive or specular colors.
//-----------------------------------------------------------------------------
VOID C3DUtil_InitMaterial( D3DMATERIAL8& mtrl, FLOAT r=0.0f, FLOAT g=0.0f,
                                               FLOAT b=0.0f, FLOAT a=1.0f );




//-----------------------------------------------------------------------------
// Name: C3DUtil_InitLight()
// Desc: Initializes a D3DLIGHT structure, setting the light position. The
//       diffuse color is set to white, specular and ambient left as black.
//-----------------------------------------------------------------------------
VOID C3DUtil_InitLight( D3DLIGHT8& light, D3DLIGHTTYPE ltType,
                        FLOAT x=0.0f, FLOAT y=0.0f, FLOAT z=0.0f );




//-----------------------------------------------------------------------------
// Name: C3DUtil_CreateTexture()
// Desc: Helper function to create a texture. It checks the root path first,
//       then tries the DXSDK media path (as specified in the system registry).
//-----------------------------------------------------------------------------
HRESULT C3DUtil_CreateTexture( LPDIRECT3DDEVICE8 pd3dDevice, TCHAR* strTexture,
                               LPDIRECT3DTEXTURE8* ppTexture,
                               D3DFORMAT d3dFormat = D3DFMT_UNKNOWN );




//-----------------------------------------------------------------------------
// Name: C3DUtil_SetColorKey()
// Desc: Changes all texels matching the colorkey to transparent, black.
//-----------------------------------------------------------------------------
HRESULT C3DUtil_SetColorKey( LPDIRECT3DTEXTURE8 pTexture, DWORD dwColorKey );




//-----------------------------------------------------------------------------
// Name: C3DUtil_CreateVertexShader()
// Desc: Assembles and creates a file-based vertex shader
//-----------------------------------------------------------------------------
HRESULT C3DUtil_CreateVertexShader( LPDIRECT3DDEVICE8 pd3dDevice, 
								    TCHAR* strFilename, DWORD* pdwVertexDecl,
									DWORD* pdwVertexShader );

									
									
									
//-----------------------------------------------------------------------------
// Name: C3DUtil_GetCubeMapViewMatrix()
// Desc: Returns a view matrix for rendering to a face of a cubemap.
//-----------------------------------------------------------------------------
D3DXMATRIX C3DUtil_GetCubeMapViewMatrix( DWORD dwFace );




//-----------------------------------------------------------------------------
// Name: C3DUtil_GetRotationFromCursor()
// Desc: Returns a quaternion for the rotation implied by the window's cursor
//       position.
//-----------------------------------------------------------------------------
D3DXQUATERNION C3DUtil_GetRotationFromCursor( HWND hWnd,
                                              FLOAT fTrackBallRadius=1.0f );




//-----------------------------------------------------------------------------
// Name: C3DUtil_SetDeviceCursor
// Desc: Builds and sets a cursor for the D3D device based on hCursor.
//-----------------------------------------------------------------------------
HRESULT C3DUtil_SetDeviceCursor( LPDIRECT3DDEVICE8 pd3dDevice, HCURSOR hCursor );




//-----------------------------------------------------------------------------
// Name: class C3DArcBall
// Desc:
//-----------------------------------------------------------------------------
class C3DArcBall
{
	INT            m_iWidth;   // ArcBall's window width
	INT            m_iHeight;  // ArcBall's window height
	FLOAT          m_fRadius;  // ArcBall's radius in screen coords
	FLOAT          m_fRadius2; // ArcBall's radius in screen coords

	D3DXQUATERNION m_qDown;               // Quaternion before button down
	D3DXQUATERNION m_qNow;                // Composite quaternion for current drag
	D3DXMATRIX     m_matRotation;         // Matrix for arcball's orientation
	D3DXMATRIX     m_matRotationDelta;    // Matrix for arcball's orientation
	D3DXMATRIX     m_matTranslation;      // Matrix for arcball's position
	D3DXMATRIX     m_matTranslationDelta; // Matrix for arcball's position
	BOOL           m_bDrag;               // Whether user is dragging arcball

	D3DXVECTOR3 ScreenToVector( int sx, int sy );

public:
	LRESULT     HandleMouseMessages( HWND, UINT, WPARAM, LPARAM );

	D3DXMATRIX* GetRotationMatrix()         { return &m_matRotation; }
	D3DXMATRIX* GetRotationDeltaMatrix()    { return &m_matRotationDelta; }
	D3DXMATRIX* GetTranslationMatrix()      { return &m_matTranslation; }
	D3DXMATRIX* GetTranslationDeltaMatrix() { return &m_matTranslationDelta; }
	BOOL        IsBeingDragged()            { return m_bDrag; }

	VOID        SetRadius( FLOAT fRadius );
	VOID        SetWindow( INT w, INT h, FLOAT r=0.9 );
  VOID        SetMatrices( const D3DXMATRIX &matRot, const D3DXMATRIX &matTrans )
    { m_matRotation = matRot; m_matTranslation = matTrans; }
	VOID        ClearDeltaMatrices()
    { D3DXMatrixIdentity( &m_matRotationDelta ); D3DXMatrixIdentity( &m_matTranslationDelta ); }

	C3DArcBall();
};




//-----------------------------------------------------------------------------
// Helper macros for pixel shader instructions
//-----------------------------------------------------------------------------

// Parameter writemasks
#define D3DPSP_WRITEMASK_B   D3DSP_WRITEMASK_0
#define D3DPSP_WRITEMASK_G   D3DSP_WRITEMASK_1
#define D3DPSP_WRITEMASK_R   D3DSP_WRITEMASK_2
#define D3DPSP_WRITEMASK_A   D3DSP_WRITEMASK_3
#define D3DPSP_WRITEMASK_C   (D3DPSP_WRITEMASK_B|D3DPSP_WRITEMASK_G|D3DPSP_WRITEMASK_R)
#define D3DPSP_WRITEMASK_ALL (D3DSP_WRITEMASK_0|D3DSP_WRITEMASK_1|D3DSP_WRITEMASK_2|D3DSP_WRITEMASK_3)
#define D3DPSP_WRITEMASK_10  (D3DSP_WRITEMASK_0|D3DSP_WRITEMASK_1)
#define D3DPSP_WRITEMASK_32  (D3DSP_WRITEMASK_2|D3DSP_WRITEMASK_3)

// Source and destination parameter token
#define D3DPS_REGNUM_MASK(_Num)   ( (1L<<31) | ((_Num)&D3DSP_REGNUM_MASK) )
#define D3DPS_DST(_Num)           ( D3DPS_REGNUM_MASK(_Num) | D3DSPR_TEMP | D3DPSP_WRITEMASK_ALL )
#define D3DPS_SRC_TEMP(_Num)      ( D3DPS_REGNUM_MASK(_Num) | D3DSP_NOSWIZZLE | D3DSPR_TEMP )
#define D3DPS_SRC_INPUT(_Num)     ( D3DPS_REGNUM_MASK(_Num) | D3DSP_NOSWIZZLE | D3DSPR_INPUT )
#define D3DPS_SRC_CONST(_Num)     ( D3DPS_REGNUM_MASK(_Num) | D3DSP_NOSWIZZLE | D3DSPR_CONST )
#define D3DPS_SRC_TEXTURE(_Num)   ( D3DPS_REGNUM_MASK(_Num) | D3DSP_NOSWIZZLE | D3DSPR_TEXTURE )
#define D3DVS_SRC_ADDR(_Num)      ( D3DPS_REGNUM_MASK(_Num) | D3DSP_NOSWIZZLE | D3DSPR_ADDR )
#define D3DVS_SRC_RASTOUT(_Num)   ( D3DPS_REGNUM_MASK(_Num) | D3DSP_NOSWIZZLE | D3DSPR_RASTOUT )
#define D3DVS_SRC_ATTROUT(_Num)   ( D3DPS_REGNUM_MASK(_Num) | D3DSP_NOSWIZZLE | D3DSPR_ATTROUT )
#define D3DVS_SRC_TEXCRDOUT(_Num) ( D3DPS_REGNUM_MASK(_Num) | D3DSP_NOSWIZZLE | D3DSPR_TEXCRDOUT )

// Temp destination registers
#define D3DS_DR0   D3DPS_DST(0)
#define D3DS_DR1   D3DPS_DST(1)
#define D3DS_DR2   D3DPS_DST(2)
#define D3DS_DR3   D3DPS_DST(3)
#define D3DS_DR4   D3DPS_DST(4)
#define D3DS_DR5   D3DPS_DST(5)
#define D3DS_DR6   D3DPS_DST(6)
#define D3DS_DR7   D3DPS_DST(7)

// Temp source registers
#define D3DS_SR0   D3DPS_SRC_TEMP(0)
#define D3DS_SR1   D3DPS_SRC_TEMP(1)
#define D3DS_SR2   D3DPS_SRC_TEMP(2)
#define D3DS_SR3   D3DPS_SRC_TEMP(3)
#define D3DS_SR4   D3DPS_SRC_TEMP(4)
#define D3DS_SR5   D3DPS_SRC_TEMP(5)
#define D3DS_SR6   D3DPS_SRC_TEMP(6)
#define D3DS_SR7   D3DPS_SRC_TEMP(7)

// Texture parameters
#define D3DS_T0   D3DPS_SRC_TEXTURE(0)
#define D3DS_T1   D3DPS_SRC_TEXTURE(1)
#define D3DS_T2   D3DPS_SRC_TEXTURE(2)
#define D3DS_T3   D3DPS_SRC_TEXTURE(3)
#define D3DS_T4   D3DPS_SRC_TEXTURE(4)
#define D3DS_T5   D3DPS_SRC_TEXTURE(5)
#define D3DS_T6   D3DPS_SRC_TEXTURE(6)
#define D3DS_T7   D3DPS_SRC_TEXTURE(7)

// Constant (factor) source parameters
#define D3DS_C0     D3DPS_SRC_CONST(0)
#define D3DS_C1     D3DPS_SRC_CONST(1)
#define D3DS_C2     D3DPS_SRC_CONST(2)
#define D3DS_C3     D3DPS_SRC_CONST(3)
#define D3DS_C4     D3DPS_SRC_CONST(4)
#define D3DS_C5     D3DPS_SRC_CONST(5)
#define D3DS_C6     D3DPS_SRC_CONST(6)
#define D3DS_C7     D3DPS_SRC_CONST(7)

// Iterated source parameters (0==Diffuse, 1==specular)
#define D3DS_V0     D3DPS_SRC_INPUT(0)
#define D3DS_V1     D3DPS_SRC_INPUT(1)


//-----------------------------------------------------------------------------
// Name: class C3DDisplay
// Desc: Class to handle all DDraw aspects of a display, including creation of
//       front and back buffers, creating offscreen surfaces and palettes,
//       and blitting surface and displaying bitmaps.
//-----------------------------------------------------------------------------
class C3DDisplay
{
protected:
    LPDIRECTDRAW7        m_pDD;
    LPDIRECTDRAWSURFACE7 m_pddsFrontBuffer;
    LPDIRECTDRAWSURFACE7 m_pddsBackBuffer;
    LPDIRECTDRAWSURFACE7 m_pddsBackBufferLeft; // For stereo modes

    HWND                 m_hWnd;
    RECT                 m_rcWindow;
    BOOL                 m_bWindowed;
    BOOL                 m_bStereo;

public:
    C3DDisplay();
    ~C3DDisplay();

    // Access functions
    HWND                 GetHWnd()           { return m_hWnd; }
    LPDIRECTDRAW7        GetDirectDraw()     { return m_pDD; }
    LPDIRECTDRAWSURFACE7 GetFrontBuffer()    { return m_pddsFrontBuffer; }
    LPDIRECTDRAWSURFACE7 GetBackBuffer()     { return m_pddsBackBuffer; }
    LPDIRECTDRAWSURFACE7 GetBackBufferLEft() { return m_pddsBackBufferLeft; }

    // Status functions
    BOOL    IsWindowed()                     { return m_bWindowed; }
    BOOL    IsStereo()                       { return m_bStereo; }

    // Creation/destruction methods
    HRESULT CreateFullScreenDisplay( HWND hWnd, DWORD dwWidth, DWORD dwHeight,
		                             DWORD dwBPP );
    HRESULT CreateWindowedDisplay( HWND hWnd, DWORD dwWidth, DWORD dwHeight );
    HRESULT InitClipper();
    HRESULT UpdateBounds();
    virtual HRESULT DestroyObjects();

    // Methods to create child objects
    HRESULT CreateSurface( C3DSurface** ppSurface, DWORD dwWidth,
		                   DWORD dwHeight );
    HRESULT CreateSurfaceFromBitmap( C3DSurface** ppSurface, TCHAR* strBMP,
		                             DWORD dwDesiredWidth,
									 DWORD dwDesiredHeight );
    HRESULT CreateSurfaceFromText( C3DSurface** ppSurface, HFONT hFont,
		                           TCHAR* strText, 
								   COLORREF crBackground,
								   COLORREF crForeground );
    HRESULT CreatePaletteFromBitmap( LPDIRECTDRAWPALETTE* ppPalette, const TCHAR* strBMP );

    // Display methods
    HRESULT Clear( DWORD dwColor = 0L );
    HRESULT ColorKeyBlt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 pdds,
                         RECT* prc = NULL );
    HRESULT Blt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 pdds,
		         RECT* prc=NULL, DWORD dwFlags=0 );
    HRESULT Blt( DWORD x, DWORD y, C3DSurface* pSurface, RECT* prc = NULL );
    HRESULT ShowBitmap( HBITMAP hbm, LPDIRECTDRAWPALETTE pPalette=NULL );
    HRESULT SetPalette( LPDIRECTDRAWPALETTE pPalette );
    HRESULT Present();
};




//-----------------------------------------------------------------------------
// Name: class C3DSurface
// Desc: Class to handle aspects of a DirectDrawSurface.
//-----------------------------------------------------------------------------
class C3DSurface
{
    LPDIRECTDRAWSURFACE7 m_pdds;
    DDSURFACEDESC2       m_ddsd;
    BOOL                 m_bColorKeyed;

public:
    LPDIRECTDRAWSURFACE7 GetDDrawSurface() { return m_pdds; }
    BOOL                 IsColorKeyed()    { return m_bColorKeyed; }

    HRESULT DrawBitmap( HBITMAP hBMP, DWORD dwBMPOriginX = 0, DWORD dwBMPOriginY = 0, 
		                DWORD dwBMPWidth = 0, DWORD dwBMPHeight = 0 );
    HRESULT DrawBitmap( TCHAR* strBMP, DWORD dwDesiredWidth, DWORD dwDesiredHeight );
    HRESULT DrawText( HFONT hFont, TCHAR* strText, DWORD dwOriginX, DWORD dwOriginY,
		              COLORREF crBackground, COLORREF crForeground );

    HRESULT SetColorKey( DWORD dwColorKey );
    DWORD   ConvertGDIColor( COLORREF dwGDIColor );
    static HRESULT GetBitMaskInfo( DWORD dwBitMask, DWORD* pdwShift, DWORD* pdwBits );

    HRESULT Create( LPDIRECTDRAW7 pDD, DDSURFACEDESC2* pddsd );
    HRESULT Create( LPDIRECTDRAWSURFACE7 pdds );
    HRESULT Destroy();

    C3DSurface();
    ~C3DSurface();
};






//-----------------------------------------------------------------------------
// Name: C3DUtil_GetDXSDKMediaPath() and C3DUtil_FindMediaFile() 
// Desc: Returns the DirectX SDK path, as stored in the system registry
//       during the SDK install.
//-----------------------------------------------------------------------------
const TCHAR* C3DUtil_GetDXSDKMediaPath();
HRESULT      C3DUtil_FindMediaFile( TCHAR* strPath, TCHAR* strFilename );




//-----------------------------------------------------------------------------
// Name: C3DUtil_Read*RegKey() and C3DUtil_Write*RegKey()
// Desc: Helper functions to read/write a string registry key 
//-----------------------------------------------------------------------------
HRESULT C3DUtil_WriteStringRegKey( HKEY hKey, TCHAR* strRegName, TCHAR* strValue );
HRESULT C3DUtil_WriteIntRegKey( HKEY hKey, TCHAR* strRegName, DWORD dwValue );
HRESULT C3DUtil_WriteGuidRegKey( HKEY hKey, TCHAR* strRegName, GUID guidValue );
HRESULT C3DUtil_WriteBoolRegKey( HKEY hKey, TCHAR* strRegName, BOOL bValue );

HRESULT C3DUtil_ReadStringRegKey( HKEY hKey, TCHAR* strRegName, TCHAR* strValue, DWORD dwLength, TCHAR* strDefault );
HRESULT C3DUtil_ReadIntRegKey( HKEY hKey, TCHAR* strRegName, DWORD* pdwValue, DWORD dwDefault );
HRESULT C3DUtil_ReadGuidRegKey( HKEY hKey, TCHAR* strRegName, GUID* pGuidValue, GUID& guidDefault );
HRESULT C3DUtil_ReadBoolRegKey( HKEY hKey, TCHAR* strRegName, BOOL* pbValue, BOOL bDefault );




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
enum TIMER_COMMAND { TIMER_RESET, TIMER_START, TIMER_STOP, TIMER_ADVANCE,
                     TIMER_GETABSOLUTETIME, TIMER_GETAPPTIME, TIMER_GETELAPSEDTIME };
FLOAT __stdcall C3DUtil_Timer( TIMER_COMMAND command );




//-----------------------------------------------------------------------------
// UNICODE support for converting between CHAR, TCHAR, and WCHAR strings
//-----------------------------------------------------------------------------
VOID C3DUtil_ConvertAnsiStringToWide( WCHAR* wstrDestination, const CHAR* strSource, int cchDestChar = -1 );
VOID C3DUtil_ConvertWideStringToAnsi( CHAR* strDestination, const WCHAR* wstrSource, int cchDestChar = -1 );
VOID C3DUtil_ConvertGenericStringToAnsi( CHAR* strDestination, const TCHAR* tstrSource, int cchDestChar = -1 );
VOID C3DUtil_ConvertGenericStringToWide( WCHAR* wstrDestination, const TCHAR* tstrSource, int cchDestChar = -1 );
VOID C3DUtil_ConvertAnsiStringToGeneric( TCHAR* tstrDestination, const CHAR* strSource, int cchDestChar = -1 );
VOID C3DUtil_ConvertWideStringToGeneric( TCHAR* tstrDestination, const WCHAR* wstrSource, int cchDestChar = -1 );




//-----------------------------------------------------------------------------
// Debug printing support
//-----------------------------------------------------------------------------
VOID    C3DUtil_Trace( TCHAR* strMsg, ... );
HRESULT _DbgOut( TCHAR*, DWORD, HRESULT, TCHAR* );

#if defined(DEBUG) | defined(_DEBUG)
    #define DXTRACE           C3DUtil_Trace
#else
    #define DXTRACE           sizeof
#endif

#if defined(DEBUG) | defined(_DEBUG)
    #define DEBUG_MSG(str)    _DbgOut( __FILE__, (DWORD)__LINE__, 0, str )
#else
    #define DEBUG_MSG(str)    (0L)
#endif




#endif // C3DUTIL_H

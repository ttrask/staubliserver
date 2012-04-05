//-----------------------------------------------------------------------------
// File: C3DEngine.h
//
// Desc: Application class for the Direct3D engine
//
//-----------------------------------------------------------------------------
#ifndef C3DENGINE_H
#define C3DENGINE_H


#define GXRELEASE(_p) do { if ((_p) != NULL) {(_p)->Release(); (_p) = NULL;} } while (0)

#include <windows.h>
#include <windowsx.h>
#include <basetsd.h>
#include <mmsystem.h>
#include <stdio.h>
#include <tchar.h>
#include <crtdbg.h>
#include <d3d8.h>
#include <d3dx8.h>

enum METHOD
{
  D3DNONINDEXED,
  D3DINDEXED,
  SOFTWARE,
  NONE
};


#include "C3DFont.h"
#include "C3DObject.h"
#include "C3DText.h"
#include "C3DUtil.h"
#include "C3DCamera.h"

//-----------------------------------------------------------------------------
// Error codes
//-----------------------------------------------------------------------------
enum APPMSGTYPE { MSG_NONE, MSGERR_APPMUSTEXIT, MSGWARN_SWITCHEDTOREF };

#define C3DENGINE_ERR_NODIRECT3D          0x82000001
#define C3DENGINE_ERR_NOWINDOW            0x82000002
#define C3DENGINE_ERR_NOCOMPATIBLEDEVICES 0x82000003
#define C3DENGINE_ERR_NOWINDOWABLEDEVICES 0x82000004
#define C3DENGINE_ERR_NOHARDWAREDEVICE    0x82000005
#define C3DENGINE_ERR_HALNOTCOMPATIBLE    0x82000006
#define C3DENGINE_ERR_NOWINDOWEDHAL       0x82000007
#define C3DENGINE_ERR_NODESKTOPHAL        0x82000008
#define C3DENGINE_ERR_NOHALTHISMODE       0x82000009
#define C3DENGINE_ERR_NONZEROREFCOUNT     0x8200000a
#define C3DENGINE_ERR_MEDIANOTFOUND       0x8200000b
#define C3DENGINE_ERR_RESIZEFAILED        0x8200000c

// linked list class
class Node;
class List;


//-----------------------------------------------------------------------------
// Name: class C3DEngine
// Desc: D3D8 rendering class
//          OneTimeSceneInit()    - To initialize app data (alloc mem, etc.)
//          InitDeviceObjects()   - To initialize the 3D scene objects
//          FrameMove()           - To animate the scene
//          Render()              - To render the scene
//          DeleteDeviceObjects() - To cleanup the 3D scene objects
//          FinalCleanup()        - To cleanup app data (for exitting the app)
//          MsgProc()             - To handle Windows messages
//-----------------------------------------------------------------------------
class C3DEngine
{
public:
  C3DEngine();
  ~C3DEngine() {};

  // Functions to create, render, pause, and clean up the application
  virtual HRESULT Init( HWND hWnd, HINSTANCE hInstance, char* szApplicationName );
  virtual HRESULT Render();
  virtual HRESULT Flip();
  virtual LRESULT HandleMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
  virtual VOID    Pause( BOOL bPause );
  virtual VOID    Destroy();
	virtual HRESULT	AddChild(C3DObject* pChild);
	virtual HRESULT	RemoveChild(C3DObject* pChild);
	virtual HRESULT	AddText(C3DText* pText);
	virtual HRESULT	RemoveText();
  virtual LPDIRECT3DDEVICE8 Device();

  virtual DWORD   Width();
  virtual DWORD   Height();
  C3DFont		*GetFont()		{ return m_pFont; }
	C3DCamera *GetCamera()	{ return m_pCamera; }
	void			 SetZoom( FLOAT fZoom )	{ m_fCameraDistance = fZoom; }
	void			 ResetArcBall();
  HRESULT		 Resize3DEnvironment();
 
  // Methods for objects to use
  LPDIRECT3DDEVICE8 GetD3DDevice()  { return m_pd3dDevice; }

protected:
  //-----------------------------------------------------------------------------
  // Name: struct D3DModeInfo
  // Desc: Structure for holding information about a display mode
  //-----------------------------------------------------------------------------
  struct D3DModeInfo
  {
    DWORD      Width;      // Screen width in this mode
    DWORD      Height;     // Screen height in this mode
    D3DFORMAT  Format;     // Pixel format in this mode
    DWORD      dwBehavior; // Hardware / Software / Mixed vertex processing
    D3DFORMAT  DepthStencilFormat; // Which depth/stencil format to use with this mode
  };

  //-----------------------------------------------------------------------------
  // Name: struct D3DDeviceInfo
  // Desc: Structure for holding information about a Direct3D device, including
  //       a list of modes compatible with this device
  //-----------------------------------------------------------------------------
  struct D3DDeviceInfo
  {
    // Device data
    D3DDEVTYPE   DeviceType;      // Reference, HAL, etc.
    D3DCAPS8     d3dCaps;         // Capabilities of this device
    const TCHAR* strDesc;         // Name of this device
    BOOL         bCanDoWindowed;  // Whether this device can work in windowed mode

    // Modes for this device
    DWORD        dwNumModes;
    D3DModeInfo  modes[150];

    // Current state
    DWORD        dwCurrentMode;
    BOOL         bWindowed;
    D3DMULTISAMPLE_TYPE MultiSampleType;
  };


  //-----------------------------------------------------------------------------
  // Name: struct D3DAdapterInfo
  // Desc: Structure for holding information about an adapter, including a list
  //       of devices available on this adapter
  //-----------------------------------------------------------------------------
  struct D3DAdapterInfo
  {
    // Adapter data
    D3DADAPTER_IDENTIFIER8 d3dAdapterIdentifier;
    D3DDISPLAYMODE d3ddmDesktop;      // Desktop display mode for this adapter

    // Devices for this adapter
    DWORD          dwNumDevices;
    D3DDeviceInfo  devices[5];

    // Current state
    DWORD          dwCurrentDevice;
  };

  // Internal variables for the state of the app
  D3DAdapterInfo    m_Adapters[10];
  DWORD             m_dwNumAdapters;
  DWORD             m_dwAdapter;
  BOOL              m_bWindowed;
  BOOL              m_bActive;
  BOOL              m_bReady;

  // Internal variables used for timing
  BOOL              m_bFrameMoving;
  BOOL              m_bSingleStep;

  // Last file and path
  TCHAR             m_strLastMeshFile[512];
  TCHAR             m_strLastMeshPath[MAX_PATH];

  // Internal error handling function
  HRESULT DisplayErrorMsg( HRESULT hr, DWORD dwType );

  // Internal functions to manage and render the 3D scene
  HRESULT BuildDeviceList();
  BOOL    FindDepthStencilFormat( UINT iAdapter, D3DDEVTYPE DeviceType,
              D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat );
  HRESULT Initialize3DEnvironment();
	void ToggleFrameRate()	{ m_bShowFrameRate = !m_bShowFrameRate; }
	HRESULT ToggleFullscreen();
  HRESULT ForceWindowed();
  HRESULT UserSelectNewDevice();
  HRESULT UserSelectMesh();
  HRESULT Render3DEnvironment();
  virtual HRESULT AdjustWindowForChange();
  static INT_PTR CALLBACK SelectDeviceProc( HWND hDlg, UINT msg, 
              WPARAM wParam, LPARAM lParam );

  // Overridable functions for the 3D scene created by the app
  virtual HRESULT ConfirmDevice(D3DCAPS8*,DWORD,D3DFORMAT); 
  virtual HRESULT OneTimeSceneInit();                       
  virtual HRESULT InitDeviceObjects();                      
  virtual HRESULT RestoreDeviceObjects();                   
  virtual HRESULT FrameMove();
	virtual HRESULT TickControllers();

  virtual HRESULT InvalidateDeviceObjects();                
  virtual HRESULT DeleteDeviceObjects();
  virtual HRESULT FinalCleanup();     
  virtual HRESULT SetProjectionMatrix();


protected:
  // Main objects used for creating and rendering the 3D scene
  D3DPRESENT_PARAMETERS m_d3dpp;         // Parameters for CreateDevice/Reset
  HWND              m_hWnd;              // The main app window
  HINSTANCE         m_hInstance;         // The current instance
  HWND              m_hWndFocus;         // The D3D focus window (usually same as m_hWnd)
  LPDIRECT3D8       m_pD3D;              // The main D3D object
  LPDIRECT3DDEVICE8 m_pd3dDevice;        // The D3D rendering device
  D3DCAPS8          m_d3dCaps;           // Caps for the device
  D3DSURFACE_DESC   m_d3dsdBackBuffer;   // Surface desc of the backbuffer
  DWORD             m_dwCreateFlags;     // Indicate sw or hw vertex processing
  DWORD             m_dwWindowStyle;     // Saved window style for mode switches
  RECT              m_rcWindowBounds;    // Saved window bounds for mode switches
  RECT              m_rcWindowClient;    // Saved client area size for mode switches

  // Variables for timing
  FLOAT             m_fTime;             // Current time in seconds
  FLOAT             m_fElapsedTime;      // Time elapsed since last frame
  FLOAT             m_fFPS;              // Instanteous frame rate
	BOOL							m_bShowFrameRate;		 // Display frame rate and device stats
  TCHAR             m_strDeviceStats[90];// String to hold D3D device stats
  TCHAR             m_strFrameStats[40]; // String to hold frame stats

  // Settings
  BOOL              m_bUseDepthBuffer;   // Whether to autocreate depthbuffer
  DWORD             m_dwMinDepthBits;    // Minimum number of bits needed in depth buffer
  DWORD             m_dwMinStencilBits;  // Minimum number of bits needed in stencil buffer
  DWORD             m_dwCreationWidth;   // Width used to create window
  DWORD             m_dwCreationHeight;  // Height used to create window
  BOOL              m_bShowCursorWhenFullscreen; // Whether to show cursor when fullscreen

  // Font
  C3DFont          *m_pFont;

  // Objects
	List             *m_pChildList;
  C3DObject        *m_pCurrentObject;
  C3DObject        *m_pLastObject;
	C3DText					 *m_pTextHead;
  C3DArcBall        m_ObjectArcBall;

  DWORD             m_dwFVF;  // FIXME: this is per-object info
  METHOD            m_method; // FIXME: this is skin-specific

  // Camera
  BOOL              m_bControlCamera;
  C3DCamera        *m_pCamera;
  C3DArcBall        m_ArcBall;
  FLOAT             m_fCameraDistance;
  
  LPDIRECT3DVERTEXBUFFER8 m_pStringVB;

};

// Node for linked list class
class Node
{
	public:
		Node(C3DObject* pObject);
		~Node();

		Node*		Next();
		C3DObject*	Object();

	private:
		C3DObject*	m_pObject;
		Node*		m_pNext;

	friend class List;
};

// Linked List Class
class List
{
	public:
		List();
		~List();

		C3DObject*	Head();
		void		Append(C3DObject* pObject);
		void		Remove(C3DObject* pObject);
		int			NumberOfNodes() { return m_dwNumberOfNodes; }
		void		IteratorRewind();
		C3DObject*	IteratorNext();
	private:
		Node*	m_pHead;
		DWORD  m_dwNumberOfNodes;
		Node*	m_pCurrent;

};

#endif




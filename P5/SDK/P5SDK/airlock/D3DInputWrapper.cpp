// D3DInputWrapper.cpp: implementation of the CD3DInputWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include <D3DX8.h>
#include <dinput.h>
#include "dxutil.h"
#include "d3dutil.h"
#include "d3dfont.h"
#include "cageapp.h"
#include "D3DInputWrapper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CD3DInputWrapper::CD3DInputWrapper(HWND hWnd) :
	m_pDI(0),
	m_hWnd(hWnd)
{
	m_pMouseDev = 0;

	ZeroMemory( &m_KeyBuffer, sizeof( m_KeyBuffer ) );
	m_pKeyDev = 0;

	m_bInitialized = FALSE;
}

CD3DInputWrapper::~CD3DInputWrapper()
{
	// Unacquire and release the mouse device
	if( m_pMouseDev )
	{
		m_pMouseDev->Unacquire();
		m_pMouseDev->Release();
	}

	if( m_pKeyDev )
	{
		m_pKeyDev->Unacquire();		
		m_pKeyDev->Release();
	}

}

HRESULT CD3DInputWrapper::InitializeInput()
{
	if( m_pDI )
		m_pDI->Release();

	HRESULT r = 0;

	// Create the IDirectInput8 object
	r = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL );
	if( FAILED( r ) )
	{
		CCageApp::DisplayDebugMsg( "Failed to create DirectInput" );
		return E_FAIL;
	}

	// Initialize the keyboard
	r = InitializeKeyboard();
	if( FAILED( r ) )
	{	
		CCageApp::DisplayDebugMsg( "Failed to initialize keyboard" );
		return E_FAIL;
	}	

	// Initialize the mouse
	r = InitializeMouse();
	if( FAILED( r ) )
	{
		CCageApp::DisplayDebugMsg( "Unable to initialize mouse" );
		return E_FAIL;
	}

	// Set the initialization flag to true
	m_bInitialized = TRUE;


	return S_OK;
}

HRESULT CD3DInputWrapper::ShutdownInput()
{
	// Get rid of the IDirectInput8 object
	if( m_pDI )
	{
		m_pDI->Release();
		m_pDI = 0;
	}
	
	return E_FAIL;
}


POINT CD3DInputWrapper::GetMousePos()
{
	// Holds mouse data
	POINT MousePos;

	// Get the data from the buffer
	MousePos.x = m_MouseData.lX;
	MousePos.y = m_MouseData.lY;
	
	// Return the position
	return MousePos;
}

BOOL CD3DInputWrapper::IsButtonDown( int Button )
{
	// Return the button status from the buffer	
	if( m_MouseData.rgbButtons[Button] & 0x80 )
		return TRUE;
	else
		return FALSE;
}

HRESULT CD3DInputWrapper::PollMouse()
{
	HRESULT r = 0;

	// Return if the object has not been initialized
	if( !m_bInitialized )
		return E_FAIL;

	// Get the state of the mouse
	r = m_pMouseDev->GetDeviceState( sizeof( DIMOUSESTATE ), &m_MouseData );
	if( FAILED( r ) )
	{
		// If the mouse has moved focus
		if( r == DIERR_INPUTLOST )
		{
			// Reacquire the mouse
			while( r == DIERR_INPUTLOST )
				r = m_pMouseDev->Acquire();

			// Try to test the state again
			if( SUCCEEDED( r ) )
				m_pMouseDev->GetDeviceState( sizeof( DIMOUSESTATE ), &m_MouseData );
			else
				return FALSE;
		}
		else
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CD3DInputWrapper::InitializeMouse()
{
	HRESULT r = 0;

	// Return if the DirectInput object does not exist
	if( !m_pDI )
		return E_FAIL;

	// Release the mouse device if it has already been created
	if( m_pMouseDev )
		m_pMouseDev->Release();

	// Create the mouse device
	r = m_pDI->CreateDevice( GUID_SysMouse, &m_pMouseDev, NULL );
	if( FAILED( r ) )
	{
		CCageApp::DisplayDebugMsg( "Unable to create mouse device" );
		return E_FAIL;
	}

	// Set the data format for the mouse
	r = m_pMouseDev->SetDataFormat( &c_dfDIMouse );
	if( FAILED( r ) )
	{
		CCageApp::DisplayDebugMsg( "Unable to set the mouse data format" );
		return E_FAIL;
	}

	// Set the cooperative level for the mouse
	r = m_pMouseDev->SetCooperativeLevel( m_hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND );
	if( FAILED( r ) )
	{
		CCageApp::DisplayDebugMsg( "Unable to set the cooperative level for the mouse" );
		return E_FAIL;
	}

	// Acquire the physical mouse into the device
	r = m_pMouseDev->Acquire();
	if( FAILED( r ) )
	{
		CCageApp::DisplayDebugMsg( "Unable to acquire mouse" );
		return E_FAIL;
	}

	

	return S_OK;

}
	
HRESULT CD3DInputWrapper::InitializeKeyboard()
{
	HRESULT r = 0;

	if( !m_pDI )
		return E_FAIL;

	// Release the device if it has already been created
	if( m_pKeyDev )
	{
		m_pKeyDev->Unacquire();
		m_pKeyDev->Release();
	}
	
	// Create the device for the keyboard
	r = m_pDI->CreateDevice( GUID_SysKeyboard, &m_pKeyDev, NULL );
	if( FAILED( r ) )
	{
		CCageApp::DisplayDebugMsg( "Failed to create key device" );
		return E_FAIL;
	}

	// Set the data format for the device
	r = m_pKeyDev->SetDataFormat( &c_dfDIKeyboard );
	if( FAILED( r ) )
	{
		CCageApp::DisplayDebugMsg( "Unable to set the keyboard data format" );
		return E_FAIL;
	}

	// Set the cooperative level
	r = m_pKeyDev->SetCooperativeLevel( m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	if( FAILED( r ) )
	{
		CCageApp::DisplayDebugMsg( "Unable to set keyboard cooperative level" );
		return E_FAIL;
	}
	
	// Acquire the device
	r = m_pKeyDev->Acquire();
	if( FAILED( r ) )
	{
		CCageApp::DisplayDebugMsg( "Unable to acquire the keyboard" );
		return E_FAIL;
	}


	return S_OK;
}

BOOL CD3DInputWrapper::IsKeyDown( int Key )
{
	HRESULT r = 0;

	// Make sure the keyboard has been initialized
	if( !m_bInitialized )
		return FALSE;

	// Get the state of the keyboard into the key buffer
	r = m_pKeyDev->GetDeviceState( sizeof( m_KeyBuffer ), &m_KeyBuffer );
	if( FAILED( r ) )
	{
		// If the device is not acquired...
		if( r == DIERR_INPUTLOST )
		{
			// ...then reacquire the device
			while( r == DIERR_INPUTLOST )
				r = m_pKeyDev->Acquire();

			if( SUCCEEDED( r ) )
				m_pKeyDev->GetDeviceState( sizeof( m_KeyBuffer ), &m_KeyBuffer );
			else
				return FALSE;
		}
		else
			// ...Otherwise it was some other error
			return FALSE;
	}

	// Check if the key was set
	if( m_KeyBuffer[Key] & 0x80 )
		return TRUE;
	else
		return FALSE;
}

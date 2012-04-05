// D3DInputWrapper.h: interface for the CD3DInputWrapper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_D3DINPUTWRAPPER_H__6E760552_2805_4E41_B4D9_4C1B82FC51B1__INCLUDED_)
#define AFX_D3DINPUTWRAPPER_H__6E760552_2805_4E41_B4D9_4C1B82FC51B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CD3DApplication;

class CD3DInputWrapper
{
public:
	CD3DInputWrapper(HWND hWnd);
	~CD3DInputWrapper();

// Variables
public:
	
protected:
	LPDIRECTINPUT8 m_pDI;
	LPDIRECTINPUTDEVICE8 m_pMouseDev;
	LPDIRECTINPUTDEVICE8 m_pKeyDev;
	
	DIMOUSESTATE m_MouseData;
	char m_KeyBuffer[256];

	BOOL m_bInitialized;

// Functions
public:
	HRESULT CD3DInputWrapper::InitializeInput();
	HRESULT CD3DInputWrapper::ShutdownInput();
	
	HRESULT PollMouse();
	BOOL IsKeyDown( int Key );
	POINT GetMousePos();
	BOOL IsButtonDown( int Button );

private:
	HRESULT InitializeMouse();
	HRESULT InitializeKeyboard();

	HWND m_hWnd;

};

#endif // !defined(AFX_D3DINPUTWRAPPER_H__6E760552_2805_4E41_B4D9_4C1B82FC51B1__INCLUDED_)

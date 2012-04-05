// Donut.cpp: implementation of the CDonut class.
//
//////////////////////////////////////////////////////////////////////

#include "Donut.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDonut::CDonut(CCageApp* pCageApp,  TCHAR* strName, float scaleFactor, float fRotationsPerSecond ) :
	CCageObject(pCageApp,strName,scaleFactor),
	m_fRotationsPerSecond(fRotationsPerSecond)
{
}

CDonut::~CDonut()
{

}





HRESULT CDonut::Create( LPDIRECT3DDEVICE8 pd3dDevice, TCHAR* strFilename)
{
	HRESULT hr = CCageObject::Create(pd3dDevice,strFilename);

	if (FAILED(hr))
		return hr;

	return S_OK;
}



void CDonut::Update(FLOAT fElapsedAppTime)
{	
	static float fullRotPerMilli = 2*D3DX_PI;

	m_fRoll += (float)(fullRotPerMilli*fElapsedAppTime*m_fRotationsPerSecond);

	m_fRoll = (float)fmod(m_fRoll,2*D3DX_PI);
}

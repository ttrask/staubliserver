// Starfield.cpp: implementation of the CStarfield class.
//
//////////////////////////////////////////////////////////////////////

#include <D3D8.h>
#include "dxutil.h"
#include "Cage.h"
#include "CageApp.h"
#include "Starfield.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CStarfield::CStarfield(CCageApp* pCageApp,  TCHAR* strName, float scaleFactor ) :
	CCageObject(pCageApp,strName,scaleFactor)
{
}

CStarfield::~CStarfield()
{

}



HRESULT CStarfield::Create( LPDIRECT3DDEVICE8 pd3dDevice, TCHAR* strFilename )
{
	HRESULT hr = CCageObject::Create(pd3dDevice,strFilename);

	if (FAILED(hr))
		return hr;
	return S_OK;

}

HRESULT CStarfield::RenderCageObject( LPDIRECT3DDEVICE8 pd3dDevice, BOOL bDrawOpaqueSubsets /*= TRUE*/, BOOL bDrawAlphaSubsets /*= TRUE*/ )
{
	return CCageObject::RenderCageObject(pd3dDevice,bDrawOpaqueSubsets,bDrawAlphaSubsets);
}

void CStarfield::Update(FLOAT fElapsedAppTime)
{
}

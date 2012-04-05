// ScoringStar.cpp: implementation of the CScoringStar class.
//
//////////////////////////////////////////////////////////////////////

#include "ScoringStar.h"
#include "CageApp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScoringStar::CScoringStar(CCageApp* pCageApp,  TCHAR* strName, float scaleFactor, bool bIsDark ) :
	CCageObject(pCageApp,strName,scaleFactor),
	m_bIsDark(bIsDark)

{
	m_Position.x = 0;
	m_Position.y = 0;
	m_Position.z = 0;
}

CScoringStar::~CScoringStar()
{

}


HRESULT CScoringStar::Create( LPDIRECT3DDEVICE8 pd3dDevice, TCHAR* strFilename )
{
	m_vScaleFactor.x = m_vScaleFactor.x;

	HRESULT hr = CCageObject::Create(pd3dDevice,strFilename);

	if (FAILED(hr))
		return hr;


	for (int i=0; i<m_dwNumMaterials; i++)
	{
		m_pMaterials[i].Ambient.r = 0.0f;
		m_pMaterials[i].Ambient.g = 0.0f;
		m_pMaterials[i].Ambient.b = 0.0f;

	}



	return S_OK;
}

void CScoringStar::Update(FLOAT fElapsedAppTime)
{
	static bool bInTransition= false;
	static bool blinkOn=false;
	static float fElapsedSinceBlinkStateChange=0;
	static float blinkPeriod = 0.5;




	if (m_pCageApp->IsLevelOver() && !m_pCageApp->GetCredits()->IsGameOverTransitionOver())
	{
		if (!bInTransition)
		{
			bInTransition=true;
			fElapsedSinceBlinkStateChange=0;
			blinkOn=true;
		}
		else
		{
			fElapsedSinceBlinkStateChange+=fElapsedAppTime;
			if (fElapsedSinceBlinkStateChange>=blinkPeriod)
			{
				fElapsedSinceBlinkStateChange=0;
				blinkOn=!blinkOn;
			}

		}
		
		if (blinkOn)
		{
			for (int i=0; i<m_dwNumMaterials; i++)
			{
				m_pMaterials[i].Emissive.r = m_pMaterials[i].Diffuse.r;
				m_pMaterials[i].Emissive.g = m_pMaterials[i].Diffuse.g;
				m_pMaterials[i].Emissive.b = 0.8f;

			}
		}
		else
		{
			for (int i=0; i<m_dwNumMaterials; i++)
			{
				m_pMaterials[i].Emissive.r = 0.0f;
				m_pMaterials[i].Emissive.g = 0.0f;
				m_pMaterials[i].Emissive.b = 0.0f;

			}
		}

	}
	else if (bInTransition)
	{
		bInTransition=false;

		for (int i=0; i<m_dwNumMaterials; i++)
		{
			m_pMaterials[i].Emissive.r = 0.0f;
			m_pMaterials[i].Emissive.g = 0.0f;
			m_pMaterials[i].Emissive.b = 0.0f;

		}
	}
}

HRESULT CScoringStar::RenderCageObject( LPDIRECT3DDEVICE8 pd3dDevice, BOOL bDrawOpaqueSubsets /*= TRUE*/, BOOL bDrawAlphaSubsets /*= TRUE*/ )
{
	static D3DXVECTOR3 firstScoringStar(-14,-15,-150);
	static int spacingX = 3;
	D3DXMATRIX matWorld,tempMatView;

	// set view transform such that stars are positioned releative to camera
	D3DXMatrixLookAtLH( &tempMatView, m_pCageApp->GetCameraPosition(),&D3DXVECTOR3( 0, 0, 0),&D3DXVECTOR3( 0, 1, 0 ) );
	pd3dDevice->SetTransform( D3DTS_VIEW,  &tempMatView);

	if (m_bIsDark)
	{
		for (int i=m_pCageApp->GetNumCaught(); i<m_pCageApp->GetNumToWin(); i++)
		{
			m_Position = firstScoringStar;
			m_Position.x += (spacingX*i);
			GetCurrentMatrix(&matWorld);
		    pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );			
			CCageObject::Render(pd3dDevice);			
		}
	}
	else
	{
		for (int i=0; i<m_pCageApp->GetNumCaught(); i++)
		{
			m_Position = firstScoringStar;
			m_Position.x += (spacingX*i);
			GetCurrentMatrix(&matWorld);
		    pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
			CCageObject::Render(pd3dDevice);
		}
	}

	// restore original view transform
	pd3dDevice->SetTransform( D3DTS_VIEW,  m_pCageApp->GetViewMatrix());

	return S_OK;
}


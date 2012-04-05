// Explosion.cpp: implementation of the CExplosion class.
//
//////////////////////////////////////////////////////////////////////

#include "Explosion.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExplosion::CExplosion(CCageApp* pCageApp,  TCHAR* strName, float scaleFactor ) :
	CCageObject(pCageApp,strName,scaleFactor),
	m_bIsExploding(false)
{
	m_dElapsedExplosionTime=0.0;
	m_dTargetExplosionTime=0.75;
	m_dTargetScaleMultiplier = 2.5;
}

CExplosion::~CExplosion()
{

}

void CExplosion::Update(FLOAT fElapsedAppTime)
{
	
	static D3DXVECTOR3 scaleMultiplierPerSecond = D3DXVECTOR3((float)(m_dTargetScaleMultiplier/m_dTargetExplosionTime),
																(float)(m_dTargetScaleMultiplier/m_dTargetExplosionTime),
																(float)(m_dTargetScaleMultiplier/m_dTargetExplosionTime));
	static D3DXVECTOR3 m_vOriginalScale = m_vScaleFactor;

	if (m_bIsExploding)
	{
		if (m_dElapsedExplosionTime>=m_dTargetExplosionTime)
		{
			m_bIsExploding=false;
			m_vScaleFactor = m_vOriginalScale;
			RecalcWorldMatrix1();
		}
		else
		{
			m_dElapsedExplosionTime += fElapsedAppTime;

			m_vCurrentScaleMultiplier += (fElapsedAppTime*scaleMultiplierPerSecond);

			m_vScaleFactor.x = m_vCurrentScaleMultiplier.x*m_vOriginalScale.x;
			m_vScaleFactor.y = m_vCurrentScaleMultiplier.y*m_vOriginalScale.y;
			m_vScaleFactor.z = m_vCurrentScaleMultiplier.z*m_vOriginalScale.z;
			RecalcWorldMatrix1();


			for (unsigned int i=0; i<m_dwNumMaterials; i++)
			{
				m_pMaterials[i].Diffuse.a = (float)(m_dElapsedExplosionTime/m_dTargetExplosionTime);
				m_pMaterials[i].Ambient.a = (float)(m_dElapsedExplosionTime/m_dTargetExplosionTime);
				m_pMaterials[i].Specular.a = (float)(m_dElapsedExplosionTime/m_dTargetExplosionTime);
				m_pMaterials[i].Emissive.a = (float)(m_dElapsedExplosionTime/m_dTargetExplosionTime);
			}

		}
	}
}


HRESULT CExplosion::RenderCageObject( LPDIRECT3DDEVICE8 pd3dDevice, BOOL bDrawOpaqueSubsets /*= TRUE*/, BOOL bDrawAlphaSubsets /*= TRUE*/ )
{
	if (m_bIsExploding)
		return CCageObject::RenderCageObject(pd3dDevice,bDrawOpaqueSubsets,bDrawAlphaSubsets);
	else
		return S_OK;
}

void CExplosion::StartExplosion(D3DXVECTOR3 pos)
{
	m_Position = pos;
	m_vCurrentScaleMultiplier = D3DXVECTOR3(0.0,0.0,0.0);


	m_bIsExploding=true;
	m_dElapsedExplosionTime=0;
}



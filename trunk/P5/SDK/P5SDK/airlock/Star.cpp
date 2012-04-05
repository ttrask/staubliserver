// Star.cpp: implementation of the CStar class.
//
//////////////////////////////////////////////////////////////////////

#include <D3D8.h>
#include "dxutil.h"
#include "Explosion.h"
#include "CageApp.h"
#include "Star.h"
#include "Cage.h"
#include "Shadow.h"
#include "Starfield.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStar::CStar(CCageApp* pCageApp,  TCHAR* strName, float scaleFactor ) :
	CCageObject(pCageApp,strName,scaleFactor)
{
	InitForNewLevel();
}

CStar::~CStar()
{

}

HRESULT CStar::Create( LPDIRECT3DDEVICE8 pd3dDevice, TCHAR* strFilename )
{

	HRESULT hr = CCageObject::Create(pd3dDevice,strFilename);

	if (FAILED(hr))
		return hr;


	for (int i=0; i<m_dwNumMaterials; i++)
	{
		m_pMaterials[i].Ambient.r = m_pMaterials[i].Diffuse.r*0.5f;
		m_pMaterials[i].Ambient.g = m_pMaterials[i].Diffuse.g*0.5f;
		m_pMaterials[i].Ambient.b = m_pMaterials[i].Diffuse.b*0.5f;

	}


	return S_OK;
}

void CStar::Update(FLOAT fElapsedAppTime)
{
	if (IsDead())
		return;

	CCage* pCage = m_pCageApp->GetCage();

	float distThisMove = (float)(m_fVelocity*(fElapsedAppTime*1000));
	
	if (IsCaught() && pCage->IsClosed())
	{
		m_Position = pCage->GetPosition();
	}
	else if (IsCaught() && pCage->IsOpen())
	{
		m_Position = pCage->GetPosition();
		m_NextWaypoint = m_DeathWaypoint;
		m_fVelocity *= m_pCageApp->GetStarExitSpeedMulitplier();
		m_bCaught=false;
		m_bDying=true;
		m_pCageApp->PlayEventSound(CCageApp::m_EVENT_THROW);

	}
	else if (IsDying())
	{
		if (DistanceToWayPoint()<distThisMove)
		{
			m_Position=m_DeathWaypoint;
			m_bDead = true;
			m_pCageApp->GetExplosion()->StartExplosion(m_Position);
			m_pCageApp->PlayEventSound(CCageApp::m_EVENT_DEATH);
		}
		else
		{
			D3DXVECTOR3 move = m_NextWaypoint - m_Position;
			D3DXVec3Normalize(&move, &move );
			move *= distThisMove;
			m_Position += move;
		}
	}
	else
	{
		if (DistanceToWayPoint()<distThisMove)
		{
			if (IsBeingBorn())
			{
				m_fVelocity =m_pCageApp->GetCurrentStarSpeed();
				m_bBeingBorn=false;
			}
			GenerateRandomWaypoint();
		}

		D3DXVECTOR3 move = m_NextWaypoint - m_Position;
		D3DXVec3Normalize(&move, &move );
		move *= distThisMove;

		m_Position += move;
	}


	static float rotationsPerSecond=0.5f;
	static float fullRotPerMilli = 2*D3DX_PI;
	m_fYaw += (float)(fullRotPerMilli*fElapsedAppTime*rotationsPerSecond);
	m_fYaw = fmod(m_fYaw,2*D3DX_PI);

	
	DoLighting(fElapsedAppTime);

}

void CStar::DoLighting(double fElapsedAppTime)
{
	static D3DMATERIAL8* pOrigMaterials=NULL;

	CCage* pCage = m_pCageApp->GetCage();


	if (pOrigMaterials==NULL)
	{
		pOrigMaterials = new D3DMATERIAL8[m_dwNumMaterials];
		for (int i=0; i<m_dwNumMaterials; i++)
		{
			pOrigMaterials[i] = m_pMaterials[i];
		}
	}
	
	
	// if the star is catchable at this momemnt, make it glow a little

	bool bCatchable = abs(m_pCageApp->Distance(&GetPosition(),&pCage->GetPosition()))<m_pCageApp->GetCageCatchRadius();
		
	if (bCatchable)
		pCage->SetIsStarCatchable(true);

	if ( bCatchable || IsDying())
	{
		for (int i=0; i<m_dwNumMaterials; i++)
		{
			m_pMaterials[i].Emissive.r = m_pMaterials[i].Diffuse.r;
			m_pMaterials[i].Emissive.g = m_pMaterials[i].Diffuse.g;
			m_pMaterials[i].Emissive.b = m_pMaterials[i].Diffuse.b;
			//m_pMaterials[i].Emissive.b = 0.8f;
			

		}
	}
	else
	{
		double zEmissiveFactor = 1.0 - (m_Position.z-m_pCageApp->GetMinPosition()->z)
			/(m_pCageApp->GetMaxPosition()->z - m_pCageApp->GetMinPosition()->z);

		if (zEmissiveFactor<0.0)
			zEmissiveFactor=0.0;
		else if (zEmissiveFactor>1.0)
			zEmissiveFactor=1.0;

		
		zEmissiveFactor = 0.1 + zEmissiveFactor*2.0;

		for (int i=0; i<m_dwNumMaterials; i++)
		{
			m_pMaterials[i].Diffuse.r = pOrigMaterials[i].Diffuse.r*zEmissiveFactor;
			m_pMaterials[i].Diffuse.g = pOrigMaterials[i].Diffuse.g*zEmissiveFactor;
			m_pMaterials[i].Diffuse.b = pOrigMaterials[i].Diffuse.b*zEmissiveFactor;
			
			m_pMaterials[i].Emissive.r = 0.0f;
			m_pMaterials[i].Emissive.g = 0.0f;
			m_pMaterials[i].Emissive.b = 0.0f;
			
		}
		
	}

}


double CStar::DistanceToWayPoint()
{
	return sqrt(pow(m_Position.x-m_NextWaypoint.x,2)+
				pow(m_Position.y-m_NextWaypoint.y,2)+
				pow(m_Position.z-m_NextWaypoint.z,2));
}



void CStar::GetCurrentMatrix(D3DXMATRIX* pMat)
{ 
	// assumes matWorld1 is already set up with model to world origin and scaling matricies

	D3DXMATRIX trans_to_position,rotYaw,rotPitch,rotRoll,rot;	

	D3DXMatrixTranslation( &trans_to_position,	m_Position.x, m_Position.y, m_Position.z );
   
	D3DXMatrixRotationX(&rotPitch,m_fPitch);
	D3DXMatrixRotationY(&rotYaw,m_fYaw);
	D3DXMatrixRotationZ(&rotRoll,m_fRoll);
	
	D3DXMatrixIdentity(&rot);
	D3DXMatrixMultiply(&rot,&rot,&rotYaw);
	D3DXMatrixMultiply(&rot,&rot,&rotRoll);
	D3DXMatrixMultiply(&rot,&rot,&rotPitch);
	
	D3DXMatrixMultiply(pMat,&matWorld1,&rot);
	D3DXMatrixMultiply(pMat,pMat,&trans_to_position);
}



HRESULT CStar::RenderCageObject( LPDIRECT3DDEVICE8 pd3dDevice, BOOL bDrawOpaqueSubsets /*= TRUE*/, BOOL bDrawAlphaSubsets /*= TRUE*/ )
{
	if (IsDead())
		return S_OK;

	// render actual star.  matrix has already been set up in CCageApp::Render()
	CCageObject::Render(pd3dDevice);

	CShadow* pShadow = m_pCageApp->GetShadow();
	pShadow->RenderShadows(pd3dDevice,GetPosition());

	return S_OK;
}


void CStar::InitForNewLevel()
{
	m_DeathWaypoint = D3DXVECTOR3(20,20,850);

	m_bBeingBorn=true;
	m_bDead=false;
	m_bCaught=false;
	m_bDying=false;

	SetVelocity(m_pCageApp->GetCurrentStarSpeed()*m_pCageApp->GetStarEntrySpeedMulitplier());
	
	m_Position = m_DeathWaypoint;
	GenerateRandomWaypoint();

}

void CStar::GenerateRandomWaypoint()
{
	m_NextWaypoint.x = (float)(rand() % ((int)(m_pCageApp->GetMaxPosition()->x-m_pCageApp->GetMinPosition()->x)) + m_pCageApp->GetMinPosition()->x);
	m_NextWaypoint.y = (float)(rand() % ((int)(m_pCageApp->GetMaxPosition()->y-m_pCageApp->GetMinPosition()->y)) + m_pCageApp->GetMinPosition()->y);
	m_NextWaypoint.z = (float)(rand() % ((int)(m_pCageApp->GetMaxPosition()->z-m_pCageApp->GetMinPosition()->z)) + m_pCageApp->GetMinPosition()->z);
}

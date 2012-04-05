// Cage.cpp: implementation of the CCage class.
//
//////////////////////////////////////////////////////////////////////

#include "dinput.h"
#include "Cage.h"
#include "CageApp.h"
#include "D3DInputWrapper.h"
#include "P5Motion.h"
#include "P5Bend.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCage::CCage(CCageApp* pCageApp,  TCHAR* strName, float scaleFactor, float closedScaleFactor ) :
	CCageObject(pCageApp,strName,scaleFactor),
	m_bIsOpen(true),
	m_vClosedScaleFactor(closedScaleFactor,closedScaleFactor,closedScaleFactor),
	m_bClosedByMouse(true),
	m_bCatchableStar(false),
	m_fBendClickThreshold(15.0)
{
}

CCage::~CCage()
{

}


HRESULT CCage::Create( LPDIRECT3DDEVICE8 pd3dDevice, TCHAR* strFilename )
{

	HRESULT hr = CCageObject::Create(pd3dDevice,strFilename);

	if (FAILED(hr))
		return hr;

	m_vCurrentScaleFactor = m_vScaleFactor;


	return S_OK;
}

void CCage::Update(FLOAT fElapsedAppTime)
{

	if (m_pCageApp->GetP5())
	{
		P5Bend_Process();
	}

	if (m_pCageApp->IsGameOver() && m_pCageApp->GetCredits()->IsGameOverTransitionOver())
	{
		if (m_pCageApp->GetInput()->IsButtonDown(0))
		{
			m_pCageApp->ResetGame();
		}
//		if (m_pCageApp->GetInput()->IsButtonDown(0) ||
//			(m_pCageApp->GetP5() && m_pCageApp->GetP5()->m_P5Devices[0].m_byBendSensor_Data[1]>m_fBendClickThreshold))

		if (m_pCageApp->GetInput()->IsButtonDown(0) || bP5ClickLevel[P5_INDEX])
		{
			m_pCageApp->ResetGame();
		}
  }


	DoMovement(fElapsedAppTime);
	DoAnimations(fElapsedAppTime);
	DoLighting(fElapsedAppTime);

 
	// reset... actually CStar::Update() is called after CCage::Update each frame, so we just updated
	// the state of the cage according to the last frame, not this frame.  this shouldn't be noticable
	m_bCatchableStar=false;
}

void CCage::DoMovement(double fElapsedAppTime)
{
	// get mouse movement data

	if (!m_pCageApp->GetP5())
	{
		POINT relPos = m_pCageApp->GetInput()->GetMousePos();
		
		m_Position.x += (relPos.x*m_pCageApp->GetMouseSensitivity()*fElapsedAppTime);
		m_Position.y -= (relPos.y*m_pCageApp->GetMouseSensitivity()*fElapsedAppTime);

		if (m_pCageApp->GetInput()->IsKeyDown(DIK_UP))
		{
			m_Position.z+=m_pCageApp->GetKeyboardSensitivity()*fElapsedAppTime;
		}
		else if (m_pCageApp->GetInput()->IsKeyDown(DIK_DOWN))
		{
			m_Position.z-=m_pCageApp->GetKeyboardSensitivity()*fElapsedAppTime;
		}
	}


	// now get P5 data if available

	if (m_pCageApp->GetP5())
	{
		P5Motion_Process();

		static float maxGlovePosition = 512.0*m_pCageApp->GetGloveSensitivity();
		static float minGlovePosition = -512.0*m_pCageApp->GetGloveSensitivity();

/*
		float gloveX = m_pCageApp->GetP5()->m_P5Devices[0].m_fx;
		float gloveY = m_pCageApp->GetP5()->m_P5Devices[0].m_fy;
		float gloveZ = m_pCageApp->GetP5()->m_P5Devices[0].m_fz;

		m_Position.x = (gloveX/maxGlovePosition)*m_pCageApp->GetMaxPosition()->x;
		m_Position.y = (gloveY/maxGlovePosition)*m_pCageApp->GetMaxPosition()->y;
		
		if (gloveZ>=0)
			m_Position.z = (-gloveZ/maxGlovePosition)*m_pCageApp->GetMaxPosition()->z;
		else
			m_Position.z = (gloveZ/maxGlovePosition)*m_pCageApp->GetMinPosition()->z;
*/

		m_Position.x = (nXPos/maxGlovePosition)*m_pCageApp->GetMaxPosition()->x;
		m_Position.y = (nYPos/maxGlovePosition)*m_pCageApp->GetMaxPosition()->y;
		m_Position.z = (nZPos/maxGlovePosition)*m_pCageApp->GetMaxPosition()->z;

	}

	CheckPosition();

}

void CCage::DoAnimations(double fElapsedAppTime)
{

	static float rotation = 2.0*D3DX_PI;
	static float openRotationsPerSecond=0.1875f*rotation;
	static float closedRotationsPerSecond=openRotationsPerSecond*-4.0f;
	
	static float animationLengthSeconds = 0.125;
	static float animationPitchRotationPercent = 0.3f;
	static float animationRollRotationPercent = 0.05f;

	static float animationPitchRotationsPerSecond= (animationPitchRotationPercent/animationLengthSeconds)*rotation;
	static float animationClosedPitchTarget= animationPitchRotationPercent*rotation;

	static float animationRollRotationsPerSecond= (animationRollRotationPercent/animationLengthSeconds)*rotation;
	static float animationClosedRollTarget= animationRollRotationPercent*rotation;

	static D3DXVECTOR3 animaitonScaleFactorPerSecond = (m_vScaleFactor - m_vClosedScaleFactor)/animationLengthSeconds;



	if (m_pCageApp->GetInput()->IsButtonDown(0) && IsOpen())
	{
		StartCloseSequence(true);
		m_pCageApp->CheckCatches();
	}
//	else if (m_pCageApp->GetP5() && m_pCageApp->GetP5()->m_P5Devices[0].m_byBendSensor_Data[1]>m_fBendClickThreshold && IsOpen())
	else if (m_pCageApp->GetP5() && bP5ClickLevel[P5_INDEX] && IsOpen())
	{
		StartCloseSequence(false);
		m_pCageApp->CheckCatches();
	}
	else if (!m_pCageApp->GetInput()->IsButtonDown(0) && m_bClosedByMouse && IsClosed())
	{
		StartOpenSequence();
	}
//	else if (m_pCageApp->GetP5() && m_pCageApp->GetP5()->m_P5Devices[0].m_byBendSensor_Data[1]<=m_fBendClickThreshold && !m_bClosedByMouse && IsClosed())
	else if (m_pCageApp->GetP5() && !bP5ClickLevel[P5_INDEX] && !m_bClosedByMouse && IsClosed())
	{
		StartOpenSequence();
	}

	if (m_pCageApp->UseGloveOrientation())
	{
		// THIS ASSUMES P5 REPORTS YAW, PITCH AND ROLL IN RADIANS
		// THIS IS NOT YET TESTED BECAUSE THE P5 DOES NOT YET SUPPORT PITCH, YAW AND ROLL
		m_fYaw = m_pCageApp->GetP5()->m_P5Devices[0].m_fyaw;
		m_fPitch = m_pCageApp->GetP5()->m_P5Devices[0].m_fpitch;
		m_fRoll = m_pCageApp->GetP5()->m_P5Devices[0].m_froll;
	}

	if (!m_pCageApp->UseGloveOrientation())
	{
		if (IsOpen())
		{
			m_fYaw += (float)(fElapsedAppTime*openRotationsPerSecond);
		}
		else
		{
			m_fYaw += (float)(fElapsedAppTime*closedRotationsPerSecond);
		}

		m_fYaw = fmod(m_fYaw,rotation);
	}


	if (IsClosing())
	{
		m_vCurrentScaleFactor -= (fElapsedAppTime*animaitonScaleFactorPerSecond);

		if (!m_pCageApp->UseGloveOrientation())
		{
			m_fPitch += (float)(fElapsedAppTime*animationPitchRotationsPerSecond);
			m_fRoll += (float)(fElapsedAppTime*animationRollRotationsPerSecond);
		}
		
		if (m_vCurrentScaleFactor.x <= m_vClosedScaleFactor.x)
		{
			m_bIsClosing = false;
		}

	}
	else if (IsOpening())
	{
		m_vCurrentScaleFactor +=(fElapsedAppTime*animaitonScaleFactorPerSecond);

		if (!m_pCageApp->UseGloveOrientation())
		{
			m_fPitch -= (float)(fElapsedAppTime*animationPitchRotationsPerSecond);
			m_fRoll -= (float)(fElapsedAppTime*animationRollRotationsPerSecond);
		}

		if (m_vCurrentScaleFactor.x >= m_vScaleFactor.x)
		{
			m_bIsOpening = false;
		}
	}
	else if (IsClosed())
	{
		m_vCurrentScaleFactor = m_vClosedScaleFactor;
		if (!m_pCageApp->UseGloveOrientation())
		{
			m_fPitch = animationClosedPitchTarget;
			m_fRoll = animationClosedRollTarget;
		}
	}
	else
	{
		m_vCurrentScaleFactor = m_vScaleFactor;
		if (!m_pCageApp->UseGloveOrientation())
		{
			m_fPitch = 0.0f;
			m_fRoll = 0.0f;
		}
	}

	
	// recalculate matWorld one because scale may have changed due to animation
	D3DXMATRIX trans_to_origin,scale;
	D3DXMatrixTranslation( &trans_to_origin,	-m_vObjectCenter.x, -m_vObjectCenter.y, -m_vObjectCenter.z);
	D3DXMatrixScaling( &scale, m_vCurrentScaleFactor.x,m_vCurrentScaleFactor.y,m_vCurrentScaleFactor.z);

	D3DXMatrixIdentity(&matWorld1);
	D3DXMatrixMultiply(&matWorld1,&trans_to_origin,&scale);

}

void CCage::DoLighting(double fElapsedAppTime)
{
		static D3DMATERIAL8* pOrigMaterials=NULL;

	if (pOrigMaterials==NULL)
	{
		pOrigMaterials = new D3DMATERIAL8[m_dwNumMaterials];
		for (int i=0; i<m_dwNumMaterials; i++)
		{
			pOrigMaterials[i] = m_pMaterials[i];
		}
	}


	// if a star is catchable at this momemnt, make it glow a little

	if (m_bCatchableStar)
	{
		for (int i=0; i<m_dwNumMaterials; i++)
		{
			m_pMaterials[i].Emissive.r = m_pMaterials[i].Diffuse.r*2;
			m_pMaterials[i].Emissive.g = m_pMaterials[i].Diffuse.g*2;
			m_pMaterials[i].Emissive.b = m_pMaterials[i].Diffuse.b*2;
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

void CCage::StartCloseSequence(bool bClosedByMouse)
{
	m_bIsOpen = false;
	m_bIsClosing = true;
	m_bIsOpening = false;
	m_bClosedByMouse = bClosedByMouse;
}

void CCage::StartOpenSequence()
{
	m_bIsOpen = true;
	m_bIsClosing = false;
	m_bIsOpening = true;

}



void CCage::CheckPosition()
{
	if (m_Position.x > m_pCageApp->GetMaxPosition()->x)
		m_Position.x = m_pCageApp->GetMaxPosition()->x;
	if (m_Position.x < m_pCageApp->GetMinPosition()->x)
		m_Position.x = m_pCageApp->GetMinPosition()->x;

	if (m_Position.y > m_pCageApp->GetMaxPosition()->y)
		m_Position.y = m_pCageApp->GetMaxPosition()->y;
	if (m_Position.y < m_pCageApp->GetMinPosition()->y)
		m_Position.y = m_pCageApp->GetMinPosition()->y;

	if (m_Position.z > m_pCageApp->GetMaxPosition()->z)
		m_Position.z = m_pCageApp->GetMaxPosition()->z;
	if (m_Position.z < m_pCageApp->GetMinPosition()->z)
		m_Position.z = m_pCageApp->GetMinPosition()->z;


}


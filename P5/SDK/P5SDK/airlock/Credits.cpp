// Credits.cpp: implementation of the CCredits class.
//
//////////////////////////////////////////////////////////////////////

#include "CageApp.h"
#include "Credits.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCredits::CCredits(CCageApp* pCageApp,  TCHAR* strName, float scaleFactor ) :
	CCageObject(pCageApp,strName,scaleFactor)
{

	m_vStartPosition	= D3DXVECTOR3(25,25,3000);
	m_vEndPosition		= D3DXVECTOR3(0,0,10);
	m_fVelocity = pCageApp->GetInitStarSpeed()*10.0;

	InitForNewLevel();

	m_bCreditSequenceOn=false;
}

CCredits::~CCredits()
{

}

void CCredits::Update(FLOAT fElapsedAppTime)
{	
	if (GetCreditSequenceOn())
	{

		float distThisMove = (float)(m_fVelocity*(fElapsedAppTime*1000));

		if (CCageApp::Distance(&m_Position,&m_vEndPosition)<distThisMove)
			m_bGameOverTransitionOver=true;

		if (m_bGameOverTransitionOver)
		{
			m_Position=m_vEndPosition;
		}
		else
		{
			D3DXVECTOR3 move = m_vEndPosition - m_Position;
			D3DXVec3Normalize(&move, &move );
			move *= distThisMove;

			m_Position += move;
		}


		static float rotationsPerSecond=0.5f;
		static float fullRotPerMilli = 2*D3DX_PI;

		if (!m_bGameOverTransitionOver || fabs(m_fYaw)>0.1)
		{

			m_fYaw += (float)(fullRotPerMilli*fElapsedAppTime*rotationsPerSecond);
			m_fYaw = fmod(m_fYaw,2*D3DX_PI);
		}
		else if (CCageApp::Distance(&m_Position,&m_vEndPosition)<distThisMove )
		{
			m_fYaw=0.0;
		}


	}



}


HRESULT CCredits::RenderCageObject( LPDIRECT3DDEVICE8 pd3dDevice, BOOL bDrawOpaqueSubsets /*= TRUE*/, BOOL bDrawAlphaSubsets /*= TRUE*/ )
{
	if (!GetCreditSequenceOn())
		return S_OK;

	CCageObject::Render(pd3dDevice);

	return S_OK;
}


void CCredits::StartCreditSequence()
{
	m_bCreditSequenceOn=true;
	m_bGameOverTransitionOver=false;
	m_dElapsedSequenceTime=0.0;
	m_Position =  m_vStartPosition;
	m_fYaw = 0.0;
}

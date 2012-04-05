// Shadow.cpp: implementation of the CShadow class.
//
//////////////////////////////////////////////////////////////////////


#include <D3D8.h>
#include "dxutil.h"
#include "Shadow.h"
#include "CageApp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CShadow::CShadow(CCageApp* pCageApp,  TCHAR* strName, float scaleFactor ) :
	CCageObject(pCageApp,strName,scaleFactor)
{
}

CShadow::~CShadow()
{

}

HRESULT CShadow::Create( LPDIRECT3DDEVICE8 pd3dDevice, TCHAR* strFilename )
{
	HRESULT hr = CCageObject::Create(pd3dDevice,strFilename);

	if (FAILED(hr))
		return hr;

	return S_OK;
}


HRESULT CShadow::RenderShadows( LPDIRECT3DDEVICE8 pd3dDevice, D3DXVECTOR3 m_starPosition,
		            BOOL bDrawOpaqueSubsets,
		            BOOL bDrawAlphaSubsets)
{
	static float nintyDegrees = 0.5*D3DX_PI;


	if (m_pCageApp->GetShadowMode()==1 || m_pCageApp->GetShadowMode()==2)
	{

		m_Position = m_starPosition;

		// now render any shadows for this star

		D3DXMATRIX matWorld;
		D3DXMATRIX trans_to_position,rot;	
		
    
		// floor first
		
		D3DXMatrixTranslation( &trans_to_position,	m_Position.x,m_pCageApp->GetShadowMinPosition()->y,m_Position.z );
		D3DXMatrixMultiply(&matWorld,&matWorld1,&trans_to_position);

		pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		Render(pd3dDevice);


		if (m_pCageApp->GetShadowMode()==2)
		{


			// now left wall

			D3DXMatrixRotationZ( &rot, -nintyDegrees);
			D3DXMatrixTranslation( &trans_to_position,	m_pCageApp->GetShadowMinPosition()->x,m_Position.y,m_Position.z );
				
			D3DXMatrixMultiply(&matWorld,&matWorld1,&rot);
			D3DXMatrixMultiply(&matWorld,&matWorld,&trans_to_position);


			pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
			Render(pd3dDevice);


			// now right wall

			D3DXMatrixRotationZ( &rot, nintyDegrees);
			D3DXMatrixTranslation( &trans_to_position,	m_pCageApp->GetShadowMaxPosition()->x,m_Position.y,m_Position.z );
				
			D3DXMatrixMultiply(&matWorld,&matWorld1,&rot);
			D3DXMatrixMultiply(&matWorld,&matWorld,&trans_to_position);


			pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
			Render(pd3dDevice);
		}
	}



	return S_OK;
}

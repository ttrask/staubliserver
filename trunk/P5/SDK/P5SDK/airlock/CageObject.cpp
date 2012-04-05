// CageObject.cpp: implementation of the CCageObject class.
//
//////////////////////////////////////////////////////////////////////
#define STRICT
#include <basetsd.h>
#include <stdio.h>
#include <math.h>
#include <D3DX8.h>
#include "dxutil.h"
#include "CageObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCageObject::CCageObject(CCageApp* pCageApp,  TCHAR* strName, float scaleFactor ) :
	CD3DMesh(strName),
	m_pCageApp(pCageApp),
	m_vScaleFactor(scaleFactor,scaleFactor,scaleFactor)
{

	m_fYaw = 0.0f;
	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_Position = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_fVelocity = 0.0f;
}

CCageObject::~CCageObject()
{

}



///////////////////
// Public Interface
///////////////////

HRESULT CCageObject::Create( LPDIRECT3DDEVICE8 pd3dDevice, TCHAR* strFilename )
{
	HRESULT hr = CD3DMesh::Create(pd3dDevice,strFilename);

	if (FAILED(hr))
		return hr;

	LPDIRECT3DVERTEXBUFFER8 pVB = NULL;
	BYTE*      pVertices = NULL;

	// Lock the vertex buffer, to generate a simple bounding sphere
	hr = m_pSysMemMesh->GetVertexBuffer( &pVB );
	if( FAILED(hr) )
		return hr;

	hr = pVB->Lock( 0, 0, &pVertices, 0 );
	if( FAILED(hr) )
	{
		SAFE_RELEASE( pVB );
		return hr;
	}

	hr = D3DXComputeBoundingSphere( pVertices, m_pSysMemMesh->GetNumVertices(), 
									m_pSysMemMesh->GetFVF(), &m_vObjectCenter, 
									&m_fObjectRadius );


    // Make sure there are normals
    if( !(m_pSysMemMesh->GetFVF() & D3DFVF_NORMAL) )
    {
	    LPD3DXMESH pTempMesh;

        hr = m_pSysMemMesh->CloneMeshFVF( m_pSysMemMesh->GetOptions(), 
                                          m_pSysMemMesh->GetFVF() | D3DFVF_NORMAL, 
                                          pd3dDevice, &pTempMesh );
        if( FAILED(hr) )
            return hr;

        D3DXComputeNormals( pTempMesh , NULL);

        SAFE_RELEASE( m_pSysMemMesh );
        m_pSysMemMesh = pTempMesh;
    }
	
	pVB->Unlock();
	SAFE_RELEASE( pVB );

	RecalcWorldMatrix1();

	return hr;
}

void CCageObject::Update(FLOAT fElapsedAppTime)
{
	
}

void CCageObject::GetCurrentMatrix(D3DXMATRIX* pMat)
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


void CCageObject::RecalcWorldMatrix1()
{ 
	// rebuild matWorld1 from current m_vObjectCenter and m_vScaleFactor
	D3DXMATRIX trans_to_origin,scale;

	D3DXMatrixTranslation( &trans_to_origin,	-m_vObjectCenter.x, -m_vObjectCenter.y, -m_vObjectCenter.z);
	D3DXMatrixScaling( &scale, m_vScaleFactor.x,m_vScaleFactor.y,m_vScaleFactor.z);

	D3DXMatrixIdentity(&matWorld1);
	D3DXMatrixMultiply(&matWorld1,&trans_to_origin,&scale);
}
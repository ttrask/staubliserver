//-----------------------------------------------------------------------------
// File: C3DMesh.cpp
//
// Desc: Support code for loading DirectX .X files.
//
//-----------------------------------------------------------------------------
//#define STRICT
//#include <tchar.h>
//#include <stdio.h>
//#include <d3d8.h>
//#include <d3dx8.h>
//#include <dxfile.h>

//#include <rmxfguid.h>
//#include <rmxftmpl.h>
//#include <malloc.h> // _alloca
#include "C3DEngine.h"
//#include "C3DUtil.h"
#include "C3DMesh.h"


C3DMesh::C3DMesh() :
//  pMesh(NULL),
//  pMeshHW(NULL),
//  pMeshSW(NULL),
  m_pMaterials( NULL ),
  m_pTextures( NULL ),
//  cpattr(0),
//  pmcNext(NULL),
  szName( NULL )
{
}

C3DMesh::~C3DMesh()
{
  delete [] m_pMaterials;

  if( m_pTextures )
  {
    for( DWORD i = 0; i < m_dwNumMaterials; ++i )
      GXRELEASE( m_pTextures[i] );
    delete [] m_pTextures;
  }

//	GXRELEASE(pMesh);
//  GXRELEASE(pMeshHW);
//  GXRELEASE(pMeshSW);
//  delete [] szName;
//  delete pmcNext;
}

/* // re-implement
HRESULT CalculateSum(SFrame *pframe, D3DXMATRIX *pmatCur, D3DXVECTOR3 *pvCenter, 
                     UINT *pcVertices)
{
    HRESULT hr = S_OK;
    PBYTE pbPoints = NULL;
    UINT cVerticesLocal = 0;
    PBYTE pbCur;
    D3DXVECTOR3 *pvCur;
    D3DXVECTOR3 vTransformedCur;
    UINT iPoint;
    SMeshContainer *pmcCur;
    SFrame *pframeCur;
    UINT cVertices;
    D3DXMATRIX matLocal;
    
    D3DXMatrixMultiply(&matLocal, &pframe->matRot, pmatCur);
    
    pmcCur = pframe->pmcMesh;
    while (pmcCur != NULL)
    {
        DWORD fvfsize = D3DXGetFVFVertexSize(pmcCur->pMesh->GetFVF());
        
        cVertices = pmcCur->pMesh->GetNumVertices();
        
        hr = pmcCur->pMesh->LockVertexBuffer(0, &pbPoints);
        if (FAILED(hr))
            goto e_Exit;
        
        for( iPoint=0, pbCur = pbPoints; iPoint < cVertices; iPoint++, pbCur += fvfsize)
        {
            pvCur = (D3DXVECTOR3*)pbCur;
            
            if ((pvCur->x != 0.0) || (pvCur->y != 0.0) || (pvCur->z != 0.0))
            {
                cVerticesLocal++;
                
                D3DXVec3TransformCoord(&vTransformedCur, pvCur, &matLocal);
                
                pvCenter->x += vTransformedCur.x;
                pvCenter->y += vTransformedCur.y;
                pvCenter->z += vTransformedCur.z;
            }
        }
        
        
        pmcCur->pMesh->UnlockVertexBuffer();
        pbPoints = NULL;
        
        pmcCur = pmcCur->pmcNext;
    }
    
    *pcVertices += cVerticesLocal;
    
    pframeCur = pframe->pframeFirstChild;
    while (pframeCur != NULL)
    {
        hr = CalculateSum(pframeCur, &matLocal, pvCenter, pcVertices);
        if (FAILED(hr))
            goto e_Exit;
        
        pframeCur = pframeCur->pframeSibling;
    }
    
e_Exit:
    if (pbPoints != NULL)
    {
        pmcCur->pMesh->UnlockVertexBuffer();
    }
    
    return hr;
}




HRESULT CalculateRadius(SFrame *pframe, D3DXMATRIX *pmatCur, D3DXVECTOR3 *pvCenter, 
                        float *pfRadiusSq)
{
    HRESULT hr = S_OK;
    PBYTE pbPoints = NULL;
    PBYTE pbCur;
    D3DXVECTOR3 *pvCur;
    D3DXVECTOR3 vDist;;
    UINT iPoint;
    UINT cVertices;
    SMeshContainer *pmcCur;
    SFrame *pframeCur;
    float fRadiusLocalSq;
    float fDistSq;
    D3DXMATRIX matLocal;
    
    D3DXMatrixMultiply(&matLocal, &pframe->matRot, pmatCur);
    
    pmcCur = pframe->pmcMesh;
    fRadiusLocalSq = *pfRadiusSq;
    while (pmcCur != NULL)
    {
        DWORD fvfsize = D3DXGetFVFVertexSize(pmcCur->pMesh->GetFVF());
        
        cVertices = pmcCur->pMesh->GetNumVertices();
        
        hr = pmcCur->pMesh->LockVertexBuffer(0, &pbPoints);
        if (FAILED(hr))
            goto e_Exit;
        
        for( iPoint=0, pbCur = pbPoints; iPoint < cVertices; iPoint++, pbCur += fvfsize )
        {
            pvCur = (D3DXVECTOR3*)pbCur;
            
            if ((pvCur->x == 0.0) && (pvCur->y == 0.0) && (pvCur->z == 0.0))
                continue;
            
            D3DXVec3TransformCoord(&vDist, pvCur, &matLocal);
            
            vDist -= *pvCenter;
            
            fDistSq = D3DXVec3LengthSq(&vDist);
            
            if( fDistSq > fRadiusLocalSq )
                fRadiusLocalSq = fDistSq;
        }
        
        
        pmcCur->pMesh->UnlockVertexBuffer();
        pbPoints = NULL;
        
        pmcCur = pmcCur->pmcNext;
    }
    
    *pfRadiusSq = fRadiusLocalSq;
    
    pframeCur = pframe->pframeFirstChild;
    while (pframeCur != NULL)
    {
        hr = CalculateRadius(pframeCur, &matLocal, pvCenter, pfRadiusSq);
        if (FAILED(hr))
            goto e_Exit;
        
        pframeCur = pframeCur->pframeSibling;
    }
    
e_Exit:
    if (pbPoints != NULL)
    {
        pmcCur->pMesh->UnlockVertexBuffer();
    }
    
    return hr;
}




HRESULT CalculateBoundingSphere(SDrawElement *pdeCur)
{
    HRESULT hr = S_OK;
    D3DXVECTOR3 vCenter(0,0,0);
    UINT cVertices = 0;
    float fRadiusSq = 0;
    D3DXMATRIX matCur;
    
    D3DXMatrixIdentity(&matCur);
    hr = CalculateSum(pdeCur->pframeRoot, &matCur, &vCenter, &cVertices);
    if (FAILED(hr))
        goto e_Exit;
    
    if (cVertices > 0)
    {
        vCenter /= (float)cVertices;
        
        D3DXMatrixIdentity(&matCur);
        hr = CalculateRadius(pdeCur->pframeRoot, &matCur, &vCenter, &fRadiusSq);
        if (FAILED(hr))
            goto e_Exit;
    }
    
    pdeCur->fRadius = (float)sqrt((double)fRadiusSq);;
    pdeCur->vCenter = vCenter;
e_Exit:
    return hr;
}

*/

HRESULT C3DMesh::Load( const char *szFilename, LPDIRECT3DDEVICE8 pDevice8 )
{
  HRESULT hr = S_OK;
  
  // Load the mesh from the specified file
  LPD3DXMESH pMeshSysMem = NULL;
  LPD3DXBUFFER pbufMaterials = NULL;
  D3DXMATERIAL *pd3dxMaterials;

  hr = D3DXLoadMeshFromX( const_cast<char *>(szFilename), D3DXMESH_SYSTEMMEM, pDevice8,
    &m_pbufAdjacency, &pbufMaterials, &m_dwNumMaterials, &pMeshSysMem );

  // Load the materials and textures
  if( m_dwNumMaterials )
  {
    // Get the array of materials out of the returned buffer, and allocate a texture array
    pd3dxMaterials = static_cast<D3DXMATERIAL *>( pbufMaterials->GetBufferPointer() );
    m_pMaterials = new D3DMATERIAL8[m_dwNumMaterials];
    m_pTextures = new LPDIRECT3DTEXTURE8[m_dwNumMaterials];

    for( DWORD i = 0; i < m_dwNumMaterials; i++ )
    {
      m_pMaterials[i] = pd3dxMaterials[i].MatD3D;
      m_pMaterials[i].Ambient = m_pMaterials[i].Diffuse;
      m_pTextures[i]  = NULL;

      // Get a path to the texture
      TCHAR strPath[512];
      C3DUtil_FindMediaFile( strPath, pd3dxMaterials[i].pTextureFilename );

      // Load the texture
      D3DXCreateTextureFromFile( pDevice8, strPath, &m_pTextures[i] );
    }

    // Done with the material buffer
    SAFE_RELEASE( pbufMaterials );
  }
  return hr;
}

//-----------------------------------------------------------------------------
// Name: ReleaseDeviceDependentMeshes()
// Desc: 
//-----------------------------------------------------------------------------
void C3DMesh::ReleaseDeviceDependentMeshes()
{
//  GXRELEASE(pMesh);
//  GXRELEASE(pMeshHW);
//  GXRELEASE(pMeshSW);
}

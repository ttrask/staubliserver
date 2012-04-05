//-----------------------------------------------------------------------------
// File: C3DObject.cpp
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
#include "C3DObject.h"
#include "C3DMesh.h"

C3DObject::C3DObject() :
  m_pMesh( NULL ),
  m_pEffect( NULL ),
  m_pTechnique( NULL ),
  m_pEngine( NULL ),
  m_bSelected( false )
{
  strcpy(m_szFileName, "");
}


HRESULT C3DObject::Init(C3DEngine* pEngine, char* szFileName)
{
  m_pEngine = pEngine;
  return S_OK;
}


//-----------------------------------------------------------------------------
// Name: Destroy()
// Desc: Called before the app exits, this function gives the app the chance
//       to cleanup after itself.
//-----------------------------------------------------------------------------
HRESULT C3DObject::Destroy()
{
  return S_OK;
}


HRESULT C3DObject::LoadMesh( const char *szFileName )
{
  if( szFileName )
    strcpy( m_szFileName, szFileName );

  if( !m_szFileName || strlen( m_szFileName ) )
    return D3DERR_NOTFOUND;
  
  // Find the path to the file, and convert it to ANSI (for the D3DXOF API)
  TCHAR strPath[MAX_PATH];
  CHAR  strPathANSI[MAX_PATH];
  C3DUtil_FindMediaFile( strPath, m_szFileName );

  C3DUtil_ConvertGenericStringToAnsi( strPathANSI, strPath );

  // save away our mesh name for reuse
  sprintf(m_szFileName, strPathANSI);

  // create a mesh object
  C3DMesh *pMesh = new C3DMesh();

  // Load the mesh from the specified file
  HRESULT hr = pMesh->Load( m_szFileName, m_pEngine->GetD3DDevice() );
  if( FAILED( hr ) )
  {
    delete pMesh;
    return hr;
  }

  m_pMesh = pMesh;
  return S_OK;
}



HRESULT C3DObject::DrawMesh()
{
/*
    UINT ipattr;
    HRESULT hr = S_OK;
    LPD3DXBONECOMBINATION pBoneComb;

    if (pmcMesh->m_pSkinMesh)
    {
        D3DCAPS8    caps;

        m_pDevice->GetDeviceCaps(&caps);

        if (m_method != pmcMesh->m_Method)
        {
            GenerateMesh(pmcMesh);
        }

//        if (m_method == D3DNONINDEXED)
        {
            if (caps.MaxVertexBlendMatrices >= pmcMesh->m_maxFaceInfl || (pmcMesh->pMeshSW == NULL && pmcMesh->pMeshHW == NULL))
            {
                m_pDevice->SetRenderState(D3DRS_VERTEXBLEND, pmcMesh->m_maxFaceInfl - 1);
                pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pmcMesh->m_pBoneCombinationBuf->GetBufferPointer());
                for (ipattr = 0; ipattr < pmcMesh->cpattr; ipattr++)
                {
                    for (DWORD i = 0; i < pmcMesh->m_maxFaceInfl; ++i)
                    {
                        DWORD matid = pBoneComb[ipattr].BoneId[i];
                        if (matid != UINT_MAX && (ipattr == 0 || matid != pBoneComb[ipattr - 1].BoneId[i]))
                        {
                            m_pDevice->SetTransform(D3DTS_WORLDMATRIX(i), pmcMesh->m_pBoneMatrix[matid]);
                            m_pDevice->MultiplyTransform(D3DTS_WORLDMATRIX(i), &pmcMesh->m_pBoneOffsetMat[matid]);
                        }
                    }

                    if (ipattr == 0 || (pBoneComb[ipattr].AttribId != pBoneComb[ipattr - 1].AttribId))
                    {
                        m_pDevice->SetMaterial(&(pmcMesh->rgMaterials[pBoneComb[ipattr].AttribId]));
                        m_pDevice->SetTexture(0, pmcMesh->pTextures[pBoneComb[ipattr].AttribId]);
                    }

		            hr = pmcMesh->pMesh->DrawSubset( ipattr );
                    if(FAILED(hr))
                        return hr;
                }
                m_pDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);
            }
            else
            {
                if (pmcMesh->pMeshHW != NULL)
                {
                    m_pDevice->SetRenderState(D3DRS_VERTEXBLEND, 1);        // Two matrix blending
                    pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pmcMesh->m_pBoneCombinationBuf->GetBufferPointer());
                    for (ipattr = 0; ipattr < pmcMesh->iAttrSplit; ipattr++)
                    {
                        for (DWORD i = 0; i < 2; ++i)       
                        {
                            DWORD matid = pBoneComb[ipattr].BoneId[i];
                            if (matid != UINT_MAX && (ipattr == 0 || matid != pBoneComb[ipattr - 1].BoneId[i]))
                            {
                                m_pDevice->SetTransform(D3DTS_WORLDMATRIX(i), pmcMesh->m_pBoneMatrix[matid]);
                                m_pDevice->MultiplyTransform(D3DTS_WORLDMATRIX(i), &pmcMesh->m_pBoneOffsetMat[matid]);
                            }
                        }

                        if (ipattr == 0 || (pBoneComb[ipattr].AttribId != pBoneComb[ipattr - 1].AttribId))
                        {
                            m_pDevice->SetMaterial(&(pmcMesh->rgMaterials[pBoneComb[ipattr].AttribId]));
                            m_pDevice->SetTexture(0, pmcMesh->pTextures[pBoneComb[ipattr].AttribId]);
                        }

		                hr = pmcMesh->pMeshHW->DrawSubset( ipattr );
                        if(FAILED(hr))
                            return hr;
                    }
                    m_pDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);
                }

                if (pmcMesh->pMeshSW != NULL)
                {
                    m_pDevice->SetRenderState(D3DRS_SOFTWAREVERTEXPROCESSING, TRUE);
                    m_pDevice->SetRenderState(D3DRS_VERTEXBLEND, pmcMesh->m_maxFaceInfl - 1);
                    pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pmcMesh->m_pBoneCombinationBuf->GetBufferPointer());
                    for (ipattr = pmcMesh->iAttrSplit; ipattr < pmcMesh->cpattr; ipattr++)
                    {
                        for (DWORD i = 0; i < pmcMesh->m_maxFaceInfl; ++i)
                        {
                            DWORD matid = pBoneComb[ipattr].BoneId[i];
                            if (matid != UINT_MAX && (ipattr == pmcMesh->iAttrSplit || matid != pBoneComb[ipattr - 1].BoneId[i]))
                            {
                                m_pDevice->SetTransform(D3DTS_WORLDMATRIX(i), pmcMesh->m_pBoneMatrix[matid]);
                                m_pDevice->MultiplyTransform(D3DTS_WORLDMATRIX(i), &pmcMesh->m_pBoneOffsetMat[matid]);
                            }
                        }

                        if (ipattr == pmcMesh->iAttrSplit || (pBoneComb[ipattr].AttribId != pBoneComb[ipattr - 1].AttribId))
                        {
                            m_pDevice->SetMaterial(&(pmcMesh->rgMaterials[pBoneComb[ipattr].AttribId]));
                            m_pDevice->SetTexture(0, pmcMesh->pTextures[pBoneComb[ipattr].AttribId]);
                        }

		                hr = pmcMesh->pMeshSW->DrawSubset( ipattr );
                        if(FAILED(hr))
                            return hr;
                    }
                    m_pDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);
                    m_pDevice->SetRenderState(D3DRS_SOFTWAREVERTEXPROCESSING, FALSE);
                }
            }
        }
        else if (m_method == D3DINDEXED)
        {
            if (caps.MaxVertexBlendMatrixIndex < pmcMesh->m_pSkinMesh->GetNumBones())
            {
                m_pDevice->SetRenderState(D3DRS_SOFTWAREVERTEXPROCESSING, TRUE);
            }

            m_pDevice->SetRenderState(D3DRS_VERTEXBLEND, pmcMesh->m_maxFaceInfl - 1);
            if (pmcMesh->m_maxFaceInfl - 1)
                m_pDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);
            pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pmcMesh->m_pBoneCombinationBuf->GetBufferPointer());
            for (ipattr = 0; ipattr < pmcMesh->cpattr; ipattr++)
            {
                for (DWORD i = 0; i < 255; ++i) // paletteSize
                {
                    DWORD matid = pBoneComb[ipattr].BoneId[i];
                    if (matid != UINT_MAX)
                    {
                        m_pDevice->SetTransform(D3DTS_WORLDMATRIX(i), pmcMesh->m_pBoneMatrix[matid]);
                        m_pDevice->MultiplyTransform(D3DTS_WORLDMATRIX(i), &pmcMesh->m_pBoneOffsetMat[matid]);
                    }
                }
                
                m_pDevice->SetMaterial(&(pmcMesh->rgMaterials[pBoneComb[ipattr].AttribId]));
                m_pDevice->SetTexture(0, pmcMesh->pTextures[pBoneComb[ipattr].AttribId]);

                hr = pmcMesh->pMesh->DrawSubset( ipattr );
                if(FAILED(hr))
                    return hr;
            }
            m_pDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
            m_pDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);

            if (caps.MaxVertexBlendMatrixIndex < pmcMesh->m_pSkinMesh->GetNumBones())
            {
                m_pDevice->SetRenderState(D3DRS_SOFTWAREVERTEXPROCESSING, FALSE);
            }
        }
        else if (m_method == SOFTWARE)
        {
            D3DXMATRIX  Identity;

            DWORD       cBones  = pmcMesh->m_pSkinMesh->GetNumBones();

            // set up bone transforms

            D3DXMATRIX* rgBoneMatrices  = NULL;
            
            rgBoneMatrices  = new D3DXMATRIX[cBones];

            if (rgBoneMatrices == NULL)
            {
                hr = E_OUTOFMEMORY;

                goto e_ExitSOFTWARE;
            }

            {
            for (DWORD iBone = 0; iBone < cBones; ++iBone)
            {
                D3DXMatrixMultiply
                (
                    &rgBoneMatrices[iBone],                 // output
                    &pmcMesh->m_pBoneOffsetMat[iBone], 
                    pmcMesh->m_pBoneMatrix[iBone]
                );
            }
            }

            // set world transform
            D3DXMatrixIdentity(&Identity);

            hr = m_pDevice->SetTransform(D3DTS_WORLD, &Identity);

            if (FAILED(hr))
                goto e_ExitSOFTWARE;


            // generate skinned mesh
            hr = pmcMesh->m_pSkinMesh->UpdateSkinnedMesh(rgBoneMatrices, pmcMesh->pMesh);

            if (FAILED(hr))
                goto e_ExitSOFTWARE;


            for (ipattr = 0; ipattr < pmcMesh->cpattr; ipattr++)
            {
                m_pDevice->SetMaterial(&(pmcMesh->rgMaterials[ipattr]));
                m_pDevice->SetTexture(0, pmcMesh->pTextures[ipattr]);

		        hr  = pmcMesh->pMesh->DrawSubset(ipattr);

                if (FAILED(hr))
                    goto e_ExitSOFTWARE;
            }

e_ExitSOFTWARE:

            delete[] rgBoneMatrices;

            return hr;
        }
        
    }
    else
    {
        for (ipattr = 0; ipattr < pmcMesh->cpattr; ipattr++)
        {
            m_pDevice->SetMaterial(&(pmcMesh->rgMaterials[ipattr]));
            m_pDevice->SetTexture(0, pmcMesh->pTextures[ipattr]);
		    hr = pmcMesh->pMesh->DrawSubset( ipattr );
            if(FAILED(hr))
                return hr;
        }
    }
*/
  return S_OK;
}


//-----------------------------------------------------------------------------
// Name: Render()
// Desc: This function sets up object-specific render states
//       and renders the object.
//-----------------------------------------------------------------------------
HRESULT C3DObject::Render(C3DEngine* pEngine)
{
/*
    SDrawElement *pdeCur;
    pdeCur = m_pdeHead;
    UINT cTriangles = 0;
    HRESULT hr;
    D3DXMATRIX mCur;
    D3DXMATRIX mTrans;
    LPDIRECT3DDEVICE8 m_pDevice = pEngine->Device();

    if (pdeCur == NULL)
    {
        return S_OK;
    }

    D3DXMatrixTranslation(&mTrans, 0, 0, 50.0f);

    hr = m_pDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&mTrans);
    if(FAILED(hr))
       return hr;

    D3DXMatrixIdentity(&mCur);

//    FLOAT fScale = 0.01f;
//    D3DXMatrixScaling(&mCur, fScale, fScale, fScale);

    D3DXMatrixTranslation(&mTrans, -pdeCur->vCenter.x, -pdeCur->vCenter.y, -pdeCur->vCenter.z);

    while (pdeCur != NULL)
    {


        hr = UpdateFrames(pdeCur->pframeRoot, mCur);
        if (FAILED(hr))
            return hr;
        hr = DrawFrames(pdeCur->pframeRoot, cTriangles, m_pDevice);
        if (FAILED(hr))
            return hr;

        pdeCur = pdeCur->pdeNext;
    }

*/
  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: SetMatrices()
// Desc: Set object rotation and translation matrices.
//-----------------------------------------------------------------------------
HRESULT C3DObject::SetMatrices( const D3DXMATRIX *pmRot, const D3DXMATRIX *pmTrans )
{
  // not yet implemented
  return E_FAIL;
}

D3DMATRIX *C3DObject::GetRotationMatrix()
{
  // not yet implemented
  return NULL;
}

D3DMATRIX *C3DObject::GetTranslationMatrix()
{
  // not yet implemented
  return NULL;
}

//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT C3DObject::InitDeviceObjects()
{
  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT C3DObject::RestoreDeviceObjects(LPDIRECT3DDEVICE8 m_pd3dDevice)
{
    HRESULT hr = S_OK;
/*
    D3DLIGHT8 light;

 

    m_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );

    m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR  );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR  );

    m_pd3dDevice->SetRenderState( D3DRS_COLORVERTEX, FALSE );

    ZeroMemory( &light, sizeof(light) );
    light.Type = D3DLIGHT_DIRECTIONAL;

    light.Diffuse.r = 1.0;
    light.Diffuse.g = 1.0;
    light.Diffuse.b = 1.0;
    light.Specular.r = 0;
    light.Specular.g = 0;
    light.Specular.b = 0;
    light.Ambient.r = 0.95;
    light.Ambient.g = 0.95;
    light.Ambient.b = 0.95;

    light.Direction = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);

    hr = m_pd3dDevice->SetLight(0, &light );
    if (FAILED(hr))
        return E_FAIL;

    hr = m_pd3dDevice->LightEnable(0, TRUE);
    if (FAILED(hr))
        return E_FAIL;
*/
    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//       this function deletes any device dependent objects.
//-----------------------------------------------------------------------------
HRESULT C3DObject::InvalidateDeviceObjects()
{
  if( m_pMesh )
    m_pMesh->ReleaseDeviceDependentMeshes();
  return S_OK;
}


//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//       this function deletes any device dependent objects.
//-----------------------------------------------------------------------------
HRESULT C3DObject::DeleteDeviceObjects()
{
  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: FrameMove()
// Desc: Called once per frame, the call is the entry point for animating
//       the object.
//-----------------------------------------------------------------------------
HRESULT C3DObject::FrameMove(float fElapsedTime)
{
  return S_OK;
}

HRESULT C3DObject::CalculateBoundingSphere()
{
  return S_OK;
}


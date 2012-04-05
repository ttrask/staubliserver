//-----------------------------------------------------------------------------
// File: C3DFrameObject.cpp
//
// Desc: Support code for loading DirectX .X files.
//
//-----------------------------------------------------------------------------
#define STRICT
#include <tchar.h>
#include <stdio.h>
#include <d3d8.h>
#include <d3dx8.h>
#include <dxfile.h>

#include <rmxfguid.h>
#include <rmxftmpl.h>
#include <malloc.h> // _alloca
#include "C3DEngine.h"
#include "C3DFrameObject.h"
#include "C3DUtil.h"



HRESULT CalculateSum(SFrame *pFrame, D3DXMATRIX *pmatCur, D3DXVECTOR3 *pvCenter, 
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
  SFrame *pFrameCur;
  UINT cVertices;
  D3DXMATRIX matLocal;
  
  D3DXMatrixMultiply(&matLocal, &pFrame->matRot, pmatCur);
  
  pmcCur = pFrame->pmcMesh;
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
  
  pFrameCur = pFrame->pframeFirstChild;
  while (pFrameCur != NULL)
  {
    hr = CalculateSum(pFrameCur, &matLocal, pvCenter, pcVertices);
    if (FAILED(hr))
      goto e_Exit;
    
    pFrameCur = pFrameCur->pframeSibling;
  }
    
e_Exit:
  if (pbPoints != NULL)
  {
    pmcCur->pMesh->UnlockVertexBuffer();
  }
  
  return hr;
}




HRESULT CalculateRadius(SFrame *pFrame, D3DXMATRIX *pmatCur, D3DXVECTOR3 *pvCenter, 
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
  SFrame *pFrameCur;
  float fRadiusLocalSq;
  float fDistSq;
  D3DXMATRIX matLocal;
    
  D3DXMatrixMultiply(&matLocal, &pFrame->matRot, pmatCur);
  
  pmcCur = pFrame->pmcMesh;
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
  
  pFrameCur = pFrame->pframeFirstChild;
  while (pFrameCur != NULL)
  {
    hr = CalculateRadius(pFrameCur, &matLocal, pvCenter, pfRadiusSq);
    if (FAILED(hr))
      goto e_Exit;
    
    pFrameCur = pFrameCur->pframeSibling;
  }
    
e_Exit:
  if (pbPoints != NULL)
  {
    pmcCur->pMesh->UnlockVertexBuffer();
  }
  
  return hr;
}

HRESULT C3DFrameObject::CalculateBoundingSphere(SDrawElement *pdeCur)
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




HRESULT C3DFrameObject::FindBones(SFrame *pFrameCur, SDrawElement *pde)
{
    HRESULT hr = S_OK;
    SMeshContainer *pmcMesh;
    SFrame *pframeChild;
    
    pmcMesh = pFrameCur->pmcMesh;
    while (pmcMesh != NULL)
    {
        if (pmcMesh->m_pSkinMesh)
        {
            char** pBoneName = static_cast<char**>(pmcMesh->m_pBoneNamesBuf->GetBufferPointer());
            for (DWORD i = 0; i < pmcMesh->m_pSkinMesh->GetNumBones(); ++i)
            {
                SFrame* pFrame = pde->FindFrame(pBoneName[i]);
                pmcMesh->m_pBoneMatrix[i] = &(pFrame->matCombined);
            }
        }
        pmcMesh = pmcMesh->pmcNext;
    }
    
    pframeChild = pFrameCur->pframeFirstChild;
    while (pframeChild != NULL)
    {
        hr = FindBones(pframeChild, pde);
        if (FAILED(hr))
            return hr;
        
        pframeChild = pframeChild->pframeSibling;
    }
    
    return S_OK;
}


C3DFrameObject::C3DFrameObject()
{
	m_pd3dDevice = NULL;
  m_pdeHead = NULL;
  strcpy(m_szFileName, "");
}


HRESULT C3DFrameObject::Init(C3DEngine* pEngine, char* szFileName)
{
  TCHAR* pszFile;
  SDrawElement *pdeMesh = NULL;
  HRESULT hr = S_OK;
  LPDIRECTXFILE pxofapi = NULL;
  LPDIRECTXFILEENUMOBJECT pxofenum = NULL;
  LPDIRECTXFILEDATA pxofobjCur = NULL;
  DWORD dwOptions;
  int cchFileName;
  LPDIRECT3DDEVICE8 m_pd3dDevice = pEngine->Device();

// move this
  DWORD m_dwFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX1;

  if (!strlen(m_szFileName))
  {
    // Find the path to the file, and convert it to ANSI (for the D3DXOF API)
    TCHAR strPath[MAX_PATH];
    CHAR  strPathANSI[MAX_PATH];
    C3DUtil_FindMediaFile( strPath, szFileName );

    C3DUtil_ConvertGenericStringToAnsi( strPathANSI, strPath );

    // save away our mesh name for reuse
    sprintf(m_szFileName, strPathANSI);
  }    

  pszFile = m_szFileName;

  if (pszFile == NULL)
    return E_INVALIDARG;
  
  pdeMesh = new SDrawElement();
  
  delete pdeMesh->pframeRoot;
  pdeMesh->pframeAnimHead = NULL;
  
  pdeMesh->pframeRoot = new SFrame();
  if (pdeMesh->pframeRoot == NULL)
  {
    hr = E_OUTOFMEMORY;
    goto e_Exit;
  }
  
  
  dwOptions = 0;
  
  cchFileName = strlen(pszFile);
  if (cchFileName < 2)
  {
    hr = E_FAIL;
    goto e_Exit;
  }
  
  hr = DirectXFileCreate(&pxofapi);
  if (FAILED(hr))
    goto e_Exit;
  
  // Register templates for d3drm.
  
	hr = pxofapi->RegisterTemplates((LPVOID)D3DRM_XTEMPLATES, D3DRM_XTEMPLATE_BYTES);
  if (FAILED(hr))
    goto e_Exit;
  
  // Create enum object.
  hr = pxofapi->CreateEnumObject((LPVOID)pszFile, DXFILELOAD_FROMFILE, &pxofenum);
  if (FAILED(hr))
    goto e_Exit;
  
  
  // Enumerate top level objects.
  // Top level objects are always data object.
  while (SUCCEEDED(pxofenum->GetNextDataObject(&pxofobjCur)))
  {
    hr = LoadFrames(pxofobjCur, pdeMesh, dwOptions, m_dwFVF,
			m_pd3dDevice, pdeMesh->pframeRoot);
    GXRELEASE(pxofobjCur);
    
    if (FAILED(hr))
      goto e_Exit;
  }
  
  hr = FindBones(pdeMesh->pframeRoot, pdeMesh);
  if (FAILED(hr))
    goto e_Exit;
  
  
  delete []pdeMesh->szName;
  pdeMesh->szName = new char[cchFileName+1];
  if (pdeMesh->szName == NULL)
  {
    hr = E_OUTOFMEMORY;
    goto e_Exit;
  }
  memcpy(pdeMesh->szName, pszFile, cchFileName+1);

  // link into the draw list
  pdeMesh->pdeNext = m_pdeHead;
  m_pdeHead = pdeMesh;
  
  hr = CalculateBoundingSphere(pdeMesh);
  if (FAILED(hr))
    goto e_Exit;
  
  
e_Exit:
  GXRELEASE(pxofobjCur);
  GXRELEASE(pxofenum);
  GXRELEASE(pxofapi);
  
  if (FAILED(hr))
  {
    delete pdeMesh;
  }
  
  return hr;
}


//-----------------------------------------------------------------------------
// Name: Destroy()
// Desc: Called before the app exits, this function gives the app the chance
//       to cleanup after itself.
//-----------------------------------------------------------------------------
HRESULT C3DFrameObject::Destroy()
{
    return S_OK;
}


HRESULT C3DFrameObject::LoadAnimation(LPDIRECTXFILEDATA pxofobjCur, SDrawElement *pde,
                                         DWORD options, DWORD fvf, LPDIRECT3DDEVICE8 pD3DDevice,
                                         SFrame *pframeParent)
{
    HRESULT hr = S_OK;
    SRotateKeyXFile *pFileRotateKey;
    SScaleKeyXFile *pFileScaleKey;
    SPositionKeyXFile *pFilePosKey;
    SMatrixKeyXFile *pFileMatrixKey;
    SFrame *pFrameCur;
    LPDIRECTXFILEDATA pxofobjChild = NULL;
    LPDIRECTXFILEOBJECT pxofChild = NULL;
    LPDIRECTXFILEDATAREFERENCE pxofobjChildRef = NULL;
    const GUID *type;
    DWORD dwSize;
    PBYTE pData;
    DWORD dwKeyType;
    DWORD cKeys;
    DWORD iKey;
    DWORD cchName;
    char *szFrameName;
    
    pFrameCur = new SFrame();
    if (pFrameCur == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }
    pFrameCur->bAnimationFrame = true;
    
    pframeParent->AddFrame(pFrameCur);
    pde->AddAnimationFrame(pFrameCur);
    
    // Enumerate child objects.
    // Child object can be data, data reference or binary.
    // Use QueryInterface() to find what type of object a child is.
    while (SUCCEEDED(pxofobjCur->GetNextObject(&pxofChild)))
    {
        // Query the child for it's FileDataReference
        hr = pxofChild->QueryInterface(IID_IDirectXFileDataReference,
            (LPVOID *)&pxofobjChildRef);
        if (SUCCEEDED(hr))
        {
            hr = pxofobjChildRef->Resolve(&pxofobjChild);
            if (SUCCEEDED(hr))
            {
                hr = pxofobjChild->GetType(&type);
                if (FAILED(hr))
                    goto e_Exit;
                
                if( TID_D3DRMFrame == *type )
                {
                    if (pFrameCur->pframeToAnimate != NULL)
                    {
                        hr = E_INVALIDARG;
                        goto e_Exit;
                    }
                    
                    hr = pxofobjChild->GetName(NULL, &cchName);
                    if (FAILED(hr))
                        goto e_Exit;
                    
                    if (cchName == 0)
                    {
                        hr = E_INVALIDARG;
                        goto e_Exit;
                        
                    }
                    
                    szFrameName = (char*)_alloca(cchName);
                    if (szFrameName == NULL)
                    {
                        hr = E_OUTOFMEMORY;
                        goto e_Exit;
                    }
                    
                    hr = pxofobjChild->GetName(szFrameName, &cchName);
                    if (FAILED(hr))
                        goto e_Exit;
                    
                    pFrameCur->pframeToAnimate = pde->FindFrame(szFrameName);
                    if (pFrameCur->pframeToAnimate == NULL)
                    {
                        hr = E_INVALIDARG;
                        goto e_Exit;
                    }
                }
                
                GXRELEASE(pxofobjChild);
            }
            
            GXRELEASE(pxofobjChildRef);
        }
        else
        {
            
            // Query the child for it's FileData
            hr = pxofChild->QueryInterface(IID_IDirectXFileData,
                (LPVOID *)&pxofobjChild);
            if (SUCCEEDED(hr))
            {
                hr = pxofobjChild->GetType(&type);
                if (FAILED(hr))
                    goto e_Exit;
                
                if ( TID_D3DRMFrame == *type )
                {
                    hr = LoadFrames(pxofobjChild, pde, options, fvf, pD3DDevice, pFrameCur);
                    if (FAILED(hr))
                        goto e_Exit;
                }
                else if ( TID_D3DRMAnimationOptions == *type )
                {
                    //ParseAnimOptions(pChildData,pParentFrame);
                    //i=2;
                }
                else if ( TID_D3DRMAnimationKey == *type )
                {
                    hr = pxofobjChild->GetData( NULL, &dwSize, (PVOID*)&pData );
                    if (FAILED(hr))
                        goto e_Exit;
                    
                    dwKeyType = ((DWORD*)pData)[0];
                    cKeys = ((DWORD*)pData)[1];
                    
                    if (dwKeyType == 0)
                    {
                        if (pFrameCur->m_pRotateKeys != NULL)
                        {
                            hr = E_INVALIDARG;
                            goto e_Exit;
                        }
                        
                        pFrameCur->m_pRotateKeys = new SRotateKey[cKeys];
                        if (pFrameCur->m_pRotateKeys == NULL)
                        {
                            hr = E_OUTOFMEMORY;
                            goto e_Exit;
                        }
                        
                        pFrameCur->m_cRotateKeys = cKeys;
                        //NOTE x files are w x y z and QUATERNIONS are x y z w
                        
                        pFileRotateKey =  (SRotateKeyXFile*)(pData + (sizeof(DWORD) * 2));
                        for (iKey = 0;iKey < cKeys; iKey++)
                        {
                            pFrameCur->m_pRotateKeys[iKey].dwTime = pFileRotateKey->dwTime;
                            pFrameCur->m_pRotateKeys[iKey].quatRotate.x = pFileRotateKey->x;
                            pFrameCur->m_pRotateKeys[iKey].quatRotate.y = pFileRotateKey->y;
                            pFrameCur->m_pRotateKeys[iKey].quatRotate.z = pFileRotateKey->z;
                            pFrameCur->m_pRotateKeys[iKey].quatRotate.w = pFileRotateKey->w;
                            
                            pFileRotateKey += 1;
                        }
                    }
                    else if (dwKeyType == 1)
                    {
                        if (pFrameCur->m_pScaleKeys != NULL)
                        {
                            hr = E_INVALIDARG;
                            goto e_Exit;
                        }
                        
                        pFrameCur->m_pScaleKeys = new SScaleKey[cKeys];
                        if (pFrameCur->m_pScaleKeys == NULL)
                        {
                            hr = E_OUTOFMEMORY;
                            goto e_Exit;
                        }
                        
                        pFrameCur->m_cScaleKeys = cKeys;
                        
                        pFileScaleKey =  (SScaleKeyXFile*)(pData + (sizeof(DWORD) * 2));
                        for (iKey = 0;iKey < cKeys; iKey++)
                        {
                            pFrameCur->m_pScaleKeys[iKey].dwTime = pFileScaleKey->dwTime;
                            pFrameCur->m_pScaleKeys[iKey].vScale = pFileScaleKey->vScale;
                            
                            pFileScaleKey += 1;
                        }
                    }
                    else if (dwKeyType == 2)
                    {
                        if (pFrameCur->m_pPositionKeys != NULL)
                        {
                            hr = E_INVALIDARG;
                            goto e_Exit;
                        }
                        
                        pFrameCur->m_pPositionKeys = new SPositionKey[cKeys];
                        if (pFrameCur->m_pPositionKeys == NULL)
                        {
                            hr = E_OUTOFMEMORY;
                            goto e_Exit;
                        }
                        
                        pFrameCur->m_cPositionKeys = cKeys;
                        
                        pFilePosKey =  (SPositionKeyXFile*)(pData + (sizeof(DWORD) * 2));
                        for (iKey = 0;iKey < cKeys; iKey++)
                        {
                            pFrameCur->m_pPositionKeys[iKey].dwTime = pFilePosKey->dwTime;
                            pFrameCur->m_pPositionKeys[iKey].vPos = pFilePosKey->vPos;
                            
                            pFilePosKey += 1;
                        }
                    }
                    else if (dwKeyType == 4)
                    {
                        if (pFrameCur->m_pMatrixKeys != NULL)
                        {
                            hr = E_INVALIDARG;
                            goto e_Exit;
                        }
                        
                        pFrameCur->m_pMatrixKeys = new SMatrixKey[cKeys];
                        if (pFrameCur->m_pMatrixKeys == NULL)
                        {
                            hr = E_OUTOFMEMORY;
                            goto e_Exit;
                        }
                        
                        pFrameCur->m_cMatrixKeys = cKeys;
                        
                        pFileMatrixKey =  (SMatrixKeyXFile*)(pData + (sizeof(DWORD) * 2));
                        for (iKey = 0;iKey < cKeys; iKey++)
                        {
                            pFrameCur->m_pMatrixKeys[iKey].dwTime = pFileMatrixKey->dwTime;
                            pFrameCur->m_pMatrixKeys[iKey].mat = pFileMatrixKey->mat;
                            
                            pFileMatrixKey += 1;
                        }
                    }
                    else
                    {
                        hr = E_INVALIDARG;
                        goto e_Exit;
                    }
                }
                
                GXRELEASE(pxofobjChild);
            }
        }
        
        GXRELEASE(pxofChild);
    }
    
e_Exit:
    GXRELEASE(pxofobjChild);
    GXRELEASE(pxofChild);
    GXRELEASE(pxofobjChildRef);
    return hr;
}


HRESULT C3DFrameObject::LoadAnimationSet(LPDIRECTXFILEDATA pxofobjCur, SDrawElement *pde,
DWORD options, DWORD fvf, LPDIRECT3DDEVICE8 pD3DDevice, SFrame *pframeParent)
{
	SFrame *pFrameCur;
	const GUID *type;
	HRESULT hr = S_OK;
	LPDIRECTXFILEDATA pxofobjChild = NULL;
	LPDIRECTXFILEOBJECT pxofChild = NULL;
	DWORD cchName;

	pFrameCur = new SFrame();
	if (pFrameCur == NULL)
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}
	pFrameCur->bAnimationFrame = true;

	pframeParent->AddFrame(pFrameCur);

	hr = pxofobjCur->GetName(NULL, &cchName);
	if (FAILED(hr))
		goto e_Exit;

	if (cchName > 0)
	{
		pFrameCur->szName = new char[cchName];
		if (pFrameCur->szName == NULL)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		hr = pxofobjCur->GetName(pFrameCur->szName, &cchName);
		if (FAILED(hr))
			goto e_Exit;
	}


	// Enumerate child objects.
	// Child object can be data, data reference or binary.
	// Use QueryInterface() to find what type of object a child is.
	while (SUCCEEDED(pxofobjCur->GetNextObject(&pxofChild)))
	{
		// Query the child for it's FileData
		hr = pxofChild->QueryInterface(IID_IDirectXFileData,
		(LPVOID *)&pxofobjChild);
		if (SUCCEEDED(hr))
		{
			hr = pxofobjChild->GetType(&type);
			if (FAILED(hr))
				goto e_Exit;

			if( TID_D3DRMAnimation == *type )
			{
				hr = LoadAnimation(pxofobjChild, pde, options, fvf, pD3DDevice, pFrameCur);
				if (FAILED(hr))
					goto e_Exit;
			}

			GXRELEASE(pxofobjChild);
		}

		GXRELEASE(pxofChild);
	}

	e_Exit:
	GXRELEASE(pxofobjChild);
	GXRELEASE(pxofChild);
	return hr;
}




HRESULT SplitMesh
        (
            LPD3DXMESH  pMesh,              // ASSUMPTION:  *pMesh is attribute sorted & has a valid attribute table
            DWORD       iAttrSplit,         // **ppMeshB gets the mesh comprising of this attribute range onward
            DWORD*      rgiAdjacency, 
            DWORD       optionsA, 
            DWORD       optionsB, 
            LPD3DXMESH* ppMeshA, 
            LPD3DXMESH* ppMeshB
        )
{
    *ppMeshA = NULL;

    *ppMeshB = NULL;

    
    HRESULT hr  = S_OK;

    PBYTE   pbVerticesIn    = NULL;
    PBYTE   pbIndicesIn     = NULL;
    DWORD*  piAttribsIn     = NULL;

    LPD3DXMESH pMeshA   = NULL;
    LPD3DXMESH pMeshB   = NULL;
    
    LPD3DXBUFFER pVertexRemapA  = NULL;
    LPD3DXBUFFER pVertexRemapB  = NULL;
        
    DWORD*  rgiAdjacencyA   = NULL;
    DWORD*  rgiAdjacencyB   = NULL;




    LPDIRECT3DDEVICE8   pDevice     = NULL;


    D3DXATTRIBUTERANGE* rgAttrTable = NULL;

    DWORD               cAttrTable  = 0;



    DWORD   cVerticesA;
    DWORD   cVerticesB;

    DWORD   cFacesA;
    DWORD   cFacesB;

    DWORD   cbVertexSize;

    DWORD   dw32bit;


    dw32bit         = pMesh->GetOptions() & D3DXMESH_32BIT;

    cbVertexSize    = D3DXGetFVFVertexSize(pMesh->GetFVF());

    hr  = pMesh->GetDevice(&pDevice);

    if (FAILED(hr))
        goto e_Exit;


    hr  = pMesh->GetAttributeTable(NULL, &cAttrTable);

    if (FAILED(hr))
        goto e_Exit;

    
    rgAttrTable = new D3DXATTRIBUTERANGE[cAttrTable];

    if (rgAttrTable == NULL)
    {
        hr  = E_OUTOFMEMORY;

        goto e_Exit;
    }


    hr  = pMesh->GetAttributeTable(rgAttrTable, NULL);

    if (FAILED(hr))
        goto e_Exit;

    if (iAttrSplit == 0)
    {
        cVerticesA  = 0;

        cFacesA     = 0;
    }
    else if (iAttrSplit >= cAttrTable)
    {
        cVerticesA  = pMesh->GetNumVertices();

        cFacesA     = pMesh->GetNumFaces();
    }
    else
    {
        cVerticesA  = rgAttrTable[iAttrSplit].VertexStart;

        cFacesA     = rgAttrTable[iAttrSplit].FaceStart;
    }

    cVerticesB  = pMesh->GetNumVertices() - cVerticesA;

    cFacesB     = pMesh->GetNumFaces() - cFacesA;


    hr  = pMesh->LockVertexBuffer(D3DLOCK_READONLY, &pbVerticesIn);

    if (FAILED(hr))
        goto e_Exit;


    hr  = pMesh->LockIndexBuffer(D3DLOCK_READONLY, &pbIndicesIn);

    if (FAILED(hr))
        goto e_Exit;


    hr  = pMesh->LockAttributeBuffer(D3DLOCK_READONLY, &piAttribsIn);

    if (FAILED(hr))
        goto e_Exit;


    if (cFacesA && cVerticesA)
    {
        PBYTE   pbVerticesOut   = NULL;
        PBYTE   pbIndicesOut    = NULL;
        DWORD*  piAttribsOut    = NULL;
        DWORD i;

        hr  = D3DXCreateMeshFVF(cFacesA, cVerticesA, optionsA | dw32bit, pMesh->GetFVF(), pDevice, &pMeshA);

        if (FAILED(hr))
            goto e_ExitA;


        hr  = pMeshA->LockVertexBuffer(0, &pbVerticesOut);

        if (FAILED(hr))
            goto e_ExitA;


        hr  = pMeshA->LockIndexBuffer(0, (LPBYTE*)&pbIndicesOut);

        if (FAILED(hr))
            goto e_ExitA;


        hr  = pMeshA->LockAttributeBuffer(0, &piAttribsOut);

        if (FAILED(hr))
            goto e_ExitA;


        memcpy(pbVerticesOut, pbVerticesIn, cVerticesA * cbVertexSize * sizeof(BYTE));

        if (dw32bit)
        {
            memcpy(pbIndicesOut, pbIndicesIn, cFacesA * 3 * sizeof(DWORD));
        }
        else
        {
            memcpy(pbIndicesOut, pbIndicesIn, cFacesA * 3 * sizeof(WORD));
        }

      
        memcpy(piAttribsOut, piAttribsIn, cFacesA * sizeof(DWORD));


        rgiAdjacencyA   = new DWORD[cFacesA * 3];

        if (rgiAdjacencyA == NULL)
        {
            hr  = E_OUTOFMEMORY;

            goto e_ExitA;
        }


        for (i = 0; i <  cFacesA * 3; i++)
        {
            rgiAdjacencyA[i]    = (rgiAdjacency[i] < cFacesA) ? rgiAdjacency[i] : 0xFFFFFFFF;
        }


e_ExitA:

        if (pbVerticesOut != NULL)
        {
            pMeshA->UnlockVertexBuffer();
        }


        if (pbIndicesOut != NULL)
        {
            pMeshA->UnlockIndexBuffer();
        }


        if (piAttribsOut != NULL)
        {
            pMeshA->UnlockAttributeBuffer();
        }


        if (FAILED(hr))
            goto e_Exit;
    }


    // calculate Mesh A's attribute table

    if (pMeshA != NULL)
    {
        hr  = pMeshA->OptimizeInplace
                      (
                          D3DXMESHOPT_VERTEXCACHE,
                          rgiAdjacencyA,
                          NULL,
                          NULL,
                          &pVertexRemapA
                      );

        if (FAILED(hr))
            goto e_Exit;
    }


    if (cFacesB && cVerticesB)
    {
        PBYTE   pbVerticesOut   = NULL;
        PBYTE   pbIndicesOut    = NULL;
        DWORD*  piAttribsOut    = NULL;
        DWORD i;

        hr  = D3DXCreateMeshFVF(cFacesB, cVerticesB, optionsB | dw32bit, pMesh->GetFVF(), pDevice, &pMeshB);

        if (FAILED(hr))
            goto e_ExitB;


        hr  = pMeshB->LockVertexBuffer(0, &pbVerticesOut);

        if (FAILED(hr))
            goto e_ExitB;


        hr  = pMeshB->LockIndexBuffer(0, &pbIndicesOut);

        if (FAILED(hr))
            goto e_ExitB;


        hr  = pMeshB->LockAttributeBuffer(0, &piAttribsOut);

        if (FAILED(hr))
            goto e_ExitB;


        memcpy(pbVerticesOut, pbVerticesIn + (cVerticesA * cbVertexSize), cVerticesB * cbVertexSize * sizeof(BYTE));


        // copy & renumber indices

        if (dw32bit)
        {
            for (DWORD i = 0; i < cFacesB * 3; i++)
            {
                ((DWORD*)pbIndicesOut)[i]    = ((DWORD*)pbIndicesIn)[(cFacesA * 3) + i]  - (DWORD)cVerticesA;
            }
        }
        else
        {
            for (DWORD i = 0; i < cFacesB * 3; i++)
            {
                ((WORD*)pbIndicesOut)[i]    = ((WORD*)pbIndicesIn)[(cFacesA * 3) + i]  - (WORD)cVerticesA;
            }
        }

        memcpy(piAttribsOut, piAttribsIn + cFacesA, cFacesB * sizeof(DWORD));


        rgiAdjacencyB   = new DWORD[cFacesB * 3];

        if (rgiAdjacencyB == NULL)
        {
            hr  = E_OUTOFMEMORY;

            goto e_ExitB;
        }


        // copy & renumber adjacency

        for (i = 0; i < cFacesB * 3; i++)
        {
            rgiAdjacencyB[i]    = (rgiAdjacency[(cFacesA * 3) + i] >= cFacesA && rgiAdjacency[(cFacesA * 3) + i] != 0xFFFFFFFF) ? rgiAdjacency[(cFacesA * 3) + i] - cFacesA : 0xFFFFFFFF;
        }


e_ExitB:

        if (pbVerticesOut != NULL)
        {
            pMeshB->UnlockVertexBuffer();
        }


        if (pbIndicesOut != NULL)
        {
            pMeshB->UnlockIndexBuffer();
        }


        if (piAttribsOut != NULL)
        {
            pMeshB->UnlockAttributeBuffer();
        }


        if (FAILED(hr))
            goto e_Exit;
    }


    // calculate Mesh B's attribute table

    if (pMeshB != NULL)
    {
        hr  = pMeshB->OptimizeInplace
                      (
                          D3DXMESHOPT_ATTRSORT,
                          rgiAdjacencyB,
                          NULL,
                          NULL,
                          &pVertexRemapB
                      );

        if (FAILED(hr))
            goto e_Exit;
    }


e_Exit:
    
    if (rgAttrTable != NULL)
    {
        delete[] rgAttrTable;
    }

    if (pbVerticesIn != NULL)
    {
        pMesh->UnlockVertexBuffer();
    }

    if (pbIndicesIn != NULL)
    {
        pMesh->UnlockIndexBuffer();
    }

    if (piAttribsIn != NULL)
    {
        pMesh->UnlockAttributeBuffer();
    }
    
    if (rgiAdjacencyA != NULL)
    {
        delete[] rgiAdjacencyA;
    }

    if (rgiAdjacencyB != NULL)
    {
        delete[] rgiAdjacencyB;
    }

    GXRELEASE(pDevice);

    GXRELEASE(pVertexRemapA);

    GXRELEASE(pVertexRemapB);

    if (FAILED(hr))
    {
        GXRELEASE(pMeshA);

        GXRELEASE(pMeshB);

        pMeshA  = NULL;

        pMeshB  = NULL;
    }

    *ppMeshA    = pMeshA;

    *ppMeshB    = pMeshB;

    return hr;
}


HRESULT C3DFrameObject::GenerateMesh(SMeshContainer *pmcMesh)
{
    // ASSUMPTION:  pmcMesh->m_rgiAdjacency contains the current adjacency

    HRESULT hr  = S_OK;

    DWORD*  pAdjacencyIn    = NULL;

    DWORD   cFaces  = pmcMesh->m_pSkinMesh->GetNumFaces();

    pAdjacencyIn    = new DWORD[pmcMesh->m_pSkinMesh->GetNumFaces() * 3];

		// select method
		METHOD m_method = SOFTWARE;	//D3DINDEXED;
		pmcMesh->m_Method = m_method;

    if (pAdjacencyIn == NULL)
    {
        hr = E_OUTOFMEMORY;

        goto e_Exit;
    }
    
    memcpy(pAdjacencyIn, pmcMesh->m_rgiAdjacency, cFaces * 3 * sizeof(DWORD));


    GXRELEASE(pmcMesh->pMesh);
    GXRELEASE(pmcMesh->pMeshHW);
    GXRELEASE(pmcMesh->pMeshSW);

    pmcMesh->pMesh      = NULL;
    pmcMesh->pMeshHW    = NULL;
    pmcMesh->pMeshSW    = NULL;

    if (m_method == D3DNONINDEXED)
    {
        DWORD*  rgiAdjacency    = NULL;
        
        LPDIRECT3DDEVICE8       pDevice = NULL;

        
        LPD3DXBONECOMBINATION   rgBoneCombinations;

        D3DCAPS8                caps;


        rgiAdjacency    = new DWORD[cFaces * 3];

        if (rgiAdjacency == NULL)
        {
            hr  = E_OUTOFMEMORY;

            goto e_ExitNONINDEXED;
        }


        hr = pmcMesh->m_pSkinMesh->ConvertToBlendedMesh
                                   (
                                       0, 
                                       pAdjacencyIn, 
                                       rgiAdjacency, 
                                       &pmcMesh->cpattr, 
                                       &pmcMesh->m_pBoneCombinationBuf, 
                                       &pmcMesh->pMesh
                                   );
        if (FAILED(hr))
            goto e_ExitNONINDEXED;



        // calculate the max face influence count

        if ((pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) != D3DFVF_XYZ)
        {
            pmcMesh->m_maxFaceInfl = 1 + ((pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) - D3DFVF_XYZRHW) / 2;
        }
        else
        {
            pmcMesh->m_maxFaceInfl = 1;
        }



        hr  = pmcMesh->pMesh->GetDevice(&pDevice);

        if (FAILED(hr))
            goto e_ExitNONINDEXED;


        hr  = pDevice->GetDeviceCaps(&caps);

        if (FAILED(hr))
            goto e_ExitNONINDEXED;

        /* If the device can only do 2 matrix blends, ConvertToBlendedMesh cannot approximate all meshes to it
           Thus we split the mesh in two parts: The part that uses at most 2 matrices and the rest. The first is
           drawn using the device's HW vertex processing and the rest is drawn using SW vertex processing. */
        if (caps.MaxVertexBlendMatrices == 2)       
        {
      
            // calculate the index of the attribute table to split on

            rgBoneCombinations  = reinterpret_cast<LPD3DXBONECOMBINATION>(pmcMesh->m_pBoneCombinationBuf->GetBufferPointer());

            for (pmcMesh->iAttrSplit = 0; pmcMesh->iAttrSplit < pmcMesh->cpattr; pmcMesh->iAttrSplit++)
            {
                DWORD   cInfl   = 0;

                for (DWORD iInfl = 0; iInfl < pmcMesh->m_maxFaceInfl; iInfl++)
                {
                    if (rgBoneCombinations[pmcMesh->iAttrSplit].BoneId[iInfl] != UINT_MAX)
                    {
                        ++cInfl;
                    }
                }

                if (cInfl > 2)
                {
                    break;
                }
            }

            // split the mesh

            hr  = SplitMesh(pmcMesh->pMesh, pmcMesh->iAttrSplit, rgiAdjacency, D3DXMESH_WRITEONLY,
              pmcMesh->pMesh->GetOptions() | D3DXMESH_SYSTEMMEM, &pmcMesh->pMeshHW, &pmcMesh->pMeshSW);

            if (FAILED(hr))
                goto e_ExitNONINDEXED;
        }
		else
		{
            // Vertex cache optimize the mesh
            LPD3DXMESH pMeshOpt;
            hr = pmcMesh->pMesh->Optimize(D3DXMESHOPT_VERTEXCACHE, rgiAdjacency, NULL, NULL, NULL, &pMeshOpt);
            if (!FAILED(hr))
            {
				pmcMesh->pMesh->Release();
				pmcMesh->pMesh = pMeshOpt;
				pMeshOpt = NULL;
            }

            // Need to clone the mesh to be WRITEONLY since we will not now read back from it.
 			LPD3DXMESH pMeshVid;
			hr = pmcMesh->pMesh->CloneMeshFVF(pmcMesh->pMesh->GetOptions() | D3DXMESH_WRITEONLY, pmcMesh->pMesh->GetFVF(),
										      pDevice, &pMeshVid);
			if (!FAILED(hr))
			{
				pmcMesh->pMesh->Release();
				pmcMesh->pMesh = pMeshVid;
				pMeshVid = NULL;
			}
		}


e_ExitNONINDEXED:

        GXRELEASE(pDevice);

        if (rgiAdjacency != NULL)
        {
            delete[] rgiAdjacency;
        }

        if (FAILED(hr))
            goto e_Exit;

    }
    else if (m_method == D3DINDEXED)
    {
        hr = pmcMesh->m_pSkinMesh->ConvertToIndexedBlendedMesh(D3DXMESH_SYSTEMMEM, pAdjacencyIn, 255, NULL,
            &pmcMesh->cpattr, &pmcMesh->m_pBoneCombinationBuf, &pmcMesh->pMesh);
        if (FAILED(hr))
            goto e_Exit;

        // Here we are talking of max vertex influence which we determine from 
        // the FVF of the returned mesh
        if ((pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) != D3DFVF_XYZ)
        {
            pmcMesh->m_maxFaceInfl = ((pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) - D3DFVF_XYZRHW) / 2;
        }
        else
        {
            pmcMesh->m_maxFaceInfl = 1;
        }
    }
    else if (m_method == SOFTWARE)
    {
        hr = pmcMesh->m_pSkinMesh->GenerateSkinnedMesh
                                   (
                                       0,	//D3DXMESH_WRITEONLY,          // options
                                       0.0f,                        // minimumm bone weight allowed
                                       pAdjacencyIn,                // adjacency of in-mesh
                                       pmcMesh->m_rgiAdjacency,     // adjacency of out-mesh
                                       &pmcMesh->pMesh              // out-mesh
                                   );
        if (FAILED(hr))
            goto e_Exit;


        hr = pmcMesh->pMesh->GetAttributeTable(NULL, &pmcMesh->cpattr);

        if (FAILED(hr))
            goto e_Exit;


        delete[] pmcMesh->m_pAttrTable;

        pmcMesh->m_pAttrTable  = new D3DXATTRIBUTERANGE[pmcMesh->cpattr];

        if (pmcMesh->m_pAttrTable == NULL)
        {
            hr = E_OUTOFMEMORY;

            goto e_Exit;
        }


        hr = pmcMesh->pMesh->GetAttributeTable(pmcMesh->m_pAttrTable, NULL);

        if (FAILED(hr))
            goto e_Exit;


        hr = pmcMesh->m_pSkinMesh->GetMaxFaceInfluences(&pmcMesh->m_maxFaceInfl);

        if (FAILED(hr))
            goto e_Exit;
    }

e_Exit:

    delete[] pAdjacencyIn;

    return hr;
}


HRESULT C3DFrameObject::LoadMesh(LPDIRECTXFILEDATA pxofobjCur, DWORD options, DWORD fvf,
  LPDIRECT3DDEVICE8 pD3DDevice, SFrame *pframeParent)
{
  HRESULT hr = S_OK;
  SMeshContainer *pmcMesh = NULL;
  LPD3DXBUFFER pbufMaterials = NULL;
  LPD3DXBUFFER pbufAdjacency = NULL;
  DWORD cchName;
  UINT cFaces;
  UINT iMaterial;
  LPDIRECT3DDEVICE8 m_pDevice = pD3DDevice;

#define REVERSE_WINDING_ORDER 1
#if REVERSE_WINDING_ORDER	// reverse winding order
	BYTE *pData = NULL;
	WORD *pIndex = NULL;
	unsigned int face = 0;
#endif // reverse winding order

  pmcMesh = new SMeshContainer();
  if (pmcMesh == NULL)
  {
    hr = E_OUTOFMEMORY;
    goto e_Exit;
  }

  hr = pxofobjCur->GetName(NULL, &cchName);
  if (FAILED(hr))
    goto e_Exit;

  if (cchName > 0)
  {
    pmcMesh->szName = new char[cchName];
    if (pmcMesh->szName == NULL)
    {
    hr = E_OUTOFMEMORY;
      goto e_Exit;
    }

    hr = pxofobjCur->GetName(pmcMesh->szName, &cchName);
    if (FAILED(hr))
      goto e_Exit;
  }

  hr = D3DXLoadSkinMeshFromXof(pxofobjCur, options, pD3DDevice, &pbufAdjacency, &pbufMaterials, &pmcMesh->cMaterials, 
  &pmcMesh->m_pBoneNamesBuf, &pmcMesh->m_pBoneOffsetBuf, &pmcMesh->m_pSkinMesh);
  if (FAILED(hr))
    goto e_Exit;

  cFaces = pmcMesh->m_pSkinMesh->GetNumFaces();

#if REVERSE_WINDING_ORDER	// reverse winding order

	hr = pmcMesh->m_pSkinMesh->LockIndexBuffer( 0, &pData );
  if (FAILED(hr))
    goto e_Exit;

	pIndex = (WORD *)&pData[0];
	for( face = 0; face < cFaces; ++face )
	{
		WORD foo = pIndex[0];
		pIndex[0] = pIndex[1];
		pIndex[1] = foo;
		pIndex += 3;
	}

	hr = pmcMesh->m_pSkinMesh->UnlockIndexBuffer();
  if (FAILED(hr))
    goto e_Exit;
#endif	// reverse winding order

  // Process skinning data
  if (pmcMesh->m_pSkinMesh->GetNumBones())
  {
    pmcMesh->m_pBoneMatrix = new D3DXMATRIX*[pmcMesh->m_pSkinMesh->GetNumBones()];
    if (pmcMesh->m_pBoneMatrix == NULL)
      goto e_Exit;
    pmcMesh->m_pBoneOffsetMat = reinterpret_cast<D3DXMATRIX*>(pmcMesh->m_pBoneOffsetBuf->GetBufferPointer());
    LPDWORD pAdjacencyIn = static_cast<LPDWORD>(pbufAdjacency->GetBufferPointer());

    pmcMesh->m_rgiAdjacency = new DWORD[cFaces * 3];

    if (pmcMesh->m_rgiAdjacency == NULL)
    {
      hr = E_OUTOFMEMORY;
      goto e_Exit;
    }

    memcpy(pmcMesh->m_rgiAdjacency, pAdjacencyIn, cFaces * 3 * sizeof(DWORD));

    hr = GenerateMesh(pmcMesh);
    if (FAILED(hr))
      goto e_Exit;
  }
  else
  {
    pmcMesh->m_pSkinMesh->GetOriginalMesh(&(pmcMesh->pMesh));
    pmcMesh->m_pSkinMesh->Release();
    pmcMesh->m_pSkinMesh = NULL;
    pmcMesh->cpattr = pmcMesh->cMaterials;
  }

  if ((pbufMaterials == NULL) || (pmcMesh->cMaterials == 0))
  {
    pmcMesh->rgMaterials = new D3DMATERIAL8[1];
    pmcMesh->pTextures = new LPDIRECT3DTEXTURE8[1];
    if (pmcMesh->rgMaterials == NULL || pmcMesh->pTextures == NULL)
    {
      hr = E_OUTOFMEMORY;
      goto e_Exit;
    }

    memset(pmcMesh->rgMaterials, 0, sizeof(D3DXMATERIAL));
    pmcMesh->rgMaterials[0].Diffuse.r = 0.5f;
    pmcMesh->rgMaterials[0].Diffuse.g = 0.5f;
    pmcMesh->rgMaterials[0].Diffuse.b = 0.5f;
    pmcMesh->rgMaterials[0].Specular = pmcMesh->rgMaterials[0].Diffuse;
    pmcMesh->pTextures[0] = NULL;
  }
  else
  {
    pmcMesh->rgMaterials = new D3DMATERIAL8[pmcMesh->cMaterials];
    pmcMesh->pTextures = new LPDIRECT3DTEXTURE8[pmcMesh->cMaterials];
    if (pmcMesh->rgMaterials == NULL || pmcMesh->pTextures == NULL)
    {
      hr = E_OUTOFMEMORY;
      goto e_Exit;
    }

    LPD3DXMATERIAL pMaterials = (LPD3DXMATERIAL)pbufMaterials->GetBufferPointer();

    for (iMaterial = 0; iMaterial < pmcMesh->cMaterials; iMaterial++)
    {
      pmcMesh->rgMaterials[iMaterial] = pMaterials[iMaterial].MatD3D;

      pmcMesh->pTextures[iMaterial] = NULL;
      if (pMaterials[iMaterial].pTextureFilename != NULL)
      {
        TCHAR szPath[MAX_PATH];
        C3DUtil_FindMediaFile(szPath, pMaterials[iMaterial].pTextureFilename);

        hr = D3DXCreateTextureFromFile(m_pDevice, szPath, &(pmcMesh->pTextures[iMaterial]));
        if (FAILED(hr))
          pmcMesh->pTextures[iMaterial] = NULL;
      }
    }
  }

  pmcMesh->rgMaterials[0].Ambient.r = 1.0f;
  pmcMesh->rgMaterials[0].Ambient.g = 1.0f;
  pmcMesh->rgMaterials[0].Ambient.b = 1.0f;
  
  // add the mesh to the parent frame
  pframeParent->AddMesh(pmcMesh);
  pmcMesh = NULL;

  e_Exit:
    delete pmcMesh;

  GXRELEASE(pbufAdjacency);
  GXRELEASE(pbufMaterials);

  return hr;
}

HRESULT C3DFrameObject::LoadFrames(LPDIRECTXFILEDATA pxofobjCur, SDrawElement *pde,
  DWORD options, DWORD fvf, LPDIRECT3DDEVICE8 pD3DDevice, SFrame *pframeParent)
{
  HRESULT hr = S_OK;
  LPDIRECTXFILEDATA pxofobjChild = NULL;
  LPDIRECTXFILEOBJECT pxofChild = NULL;
  const GUID *type;
  DWORD cbSize;
  D3DXMATRIX *pmatNew;
  SFrame *pFrameCur;
  DWORD cchName;
  
  // Get the type of the object
  hr = pxofobjCur->GetType(&type);
  if (FAILED(hr))
    goto e_Exit;
    
  if (*type == TID_D3DRMMesh)
  {
    hr = LoadMesh(pxofobjCur, options, fvf, pD3DDevice, pframeParent);
    if (FAILED(hr))
      goto e_Exit;
  }
  else if (*type == TID_D3DRMFrameTransformMatrix)
  {
    hr = pxofobjCur->GetData(NULL, &cbSize, (PVOID*)&pmatNew);
    if (FAILED(hr))
      goto e_Exit;

    // update the parents matrix with the new one
    pframeParent->matRot = *pmatNew;
    pframeParent->matRotOrig = *pmatNew;
  }
  else if (*type == TID_D3DRMAnimationSet)
  {
    LoadAnimationSet(pxofobjCur, pde, options, fvf, pD3DDevice, pframeParent);
  }
  else if (*type == TID_D3DRMAnimation)
  {
    LoadAnimation(pxofobjCur, pde, options, fvf, pD3DDevice, pframeParent);
  }
  else if (*type == TID_D3DRMFrame)
  {
    pFrameCur = new SFrame();
    if (pFrameCur == NULL)
    {
      hr = E_OUTOFMEMORY;
      goto e_Exit;
    }

    hr = pxofobjCur->GetName(NULL, &cchName);
    if (FAILED(hr))
      goto e_Exit;

    if (cchName > 0)
    {
      pFrameCur->szName = new char[cchName];
      if (pFrameCur->szName == NULL)
      {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
      }

      hr = pxofobjCur->GetName(pFrameCur->szName, &cchName);
      if (FAILED(hr))
        goto e_Exit;
    }

    pframeParent->AddFrame(pFrameCur);

    // Enumerate child objects.
    // Child object can be data, data reference or binary.
    // Use QueryInterface() to find what type of object a child is.
    while (SUCCEEDED(pxofobjCur->GetNextObject(&pxofChild)))
    {
      // Query the child for it's FileData
      hr = pxofChild->QueryInterface(IID_IDirectXFileData, (LPVOID *)&pxofobjChild);
      if (SUCCEEDED(hr))
      {
        hr = LoadFrames(pxofobjChild, pde, options, fvf, pD3DDevice, pFrameCur);
        if (FAILED(hr))
          goto e_Exit;

        GXRELEASE(pxofobjChild);
      }

      GXRELEASE(pxofChild);
    }
  }
    
e_Exit:
  GXRELEASE(pxofobjChild);
  GXRELEASE(pxofChild);
  return hr;
}

HRESULT C3DFrameObject::DrawMeshContainer(SMeshContainer *pmcMesh, LPDIRECT3DDEVICE8 m_pDevice)
{
  UINT ipattr;
  HRESULT hr = S_OK;
  LPD3DXBONECOMBINATION pBoneComb;

	// select method
	METHOD m_method = SOFTWARE;	//D3DINDEXED;

  if (pmcMesh->m_pSkinMesh)
  {
    D3DCAPS8    caps;
    m_pDevice->GetDeviceCaps(&caps);

		// FIXME: this doesn't need to be called each frame [AJM]
		//	NOTE: must be called after fullscreen toggle, though!
    if (m_method != pmcMesh->m_Method)
    {
      GenerateMesh(pmcMesh);
    }

    if (m_method == D3DNONINDEXED)
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

			// transform bones
			{
				DWORD iBone;
				for (iBone = 0; iBone < cBones; ++iBone)
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

  return S_OK;
}

// sets matCombined in self and all children, from matRot and matTrans
HRESULT C3DFrameObject::UpdateFrames(SFrame *pFrameCur, D3DXMATRIX &matCur)
{
  HRESULT hr = S_OK;
  pFrameCur->matCombined = matCur;
  D3DXMatrixMultiply(&pFrameCur->matCombined, &pFrameCur->matRot, &matCur);
  D3DXMatrixMultiply(&pFrameCur->matCombined, &pFrameCur->matCombined, &pFrameCur->matTrans );
  SFrame *pframeChild = pFrameCur->pframeFirstChild;
  while (pframeChild != NULL)
  {
    hr = UpdateFrames(pframeChild, pFrameCur->matCombined);
    if (FAILED(hr))
      return hr;

    pframeChild = pframeChild->pframeSibling;
  }
  return S_OK;
}

HRESULT C3DFrameObject::DrawFrames(SFrame *pFrameCur, UINT &cTriangles, LPDIRECT3DDEVICE8 m_pDevice)
{
  HRESULT hr = S_OK;
  SMeshContainer *pmcMesh;
  SFrame *pframeChild;


  if (pFrameCur->pmcMesh != NULL)
  {
		D3DXMATRIX mWorld;
    hr = m_pDevice->SetTransform(D3DTS_WORLD, &pFrameCur->matCombined);
    if(FAILED(hr))
      return hr;
  }

  pmcMesh = pFrameCur->pmcMesh;
  while (pmcMesh != NULL)
  {
    hr = DrawMeshContainer(pmcMesh, m_pDevice);
    if (FAILED(hr))
      return hr;

    cTriangles += pmcMesh->pMesh->GetNumFaces();

    pmcMesh = pmcMesh->pmcNext;
  }

  pframeChild = pFrameCur->pframeFirstChild;
  while (pframeChild != NULL)
  {
    hr = DrawFrames(pframeChild, cTriangles, m_pDevice);
    if (FAILED(hr))
      return hr;

    pframeChild = pframeChild->pframeSibling;
  }

  return S_OK;
}

// sets pframeToAnimate->matRot
void SFrame::SetTime(float fGlobalTime)
{
	if( !bAnimationFrame )
		return;
	
  UINT iKey;
  UINT dwp2;
  UINT dwp3;
  D3DXMATRIX matResult;
  D3DXMATRIX matTemp;
  float fTime1;
  float fTime2;
  float fLerpValue;
  D3DXVECTOR3 vScale;
  D3DXVECTOR3 vPos;
  D3DXQUATERNION quat;
  BOOL bAnimate = false;
  float fTime;

  if (m_pMatrixKeys )
  {
    fTime = (float)fmod(fGlobalTime, m_pMatrixKeys[m_cMatrixKeys-1].dwTime);

    for (iKey = 0 ;iKey < m_cMatrixKeys ; iKey++)
    {
      if ((float)m_pMatrixKeys[iKey].dwTime > fTime)
      {
        dwp3 = iKey;

        if (iKey > 0)
        {
          dwp2= iKey - 1;
        }
        else  // when iKey == 0, then dwp2 == 0
        {
          dwp2 = iKey;
        }

        break;
      }
    }
    fTime1 = (float)m_pMatrixKeys[dwp2].dwTime;
    fTime2 = (float)m_pMatrixKeys[dwp3].dwTime;

    if ((fTime2 - fTime1) ==0)
      fLerpValue = 0;
    else
      fLerpValue =  (fTime - fTime1)  / (fTime2 - fTime1);

#if 0	// without tweening

		if (fLerpValue > 0.5)
    {
      iKey = dwp3;
    }
    else
    {
      iKey = dwp2;
    }
    pframeToAnimate->matRot = m_pMatrixKeys[iKey].mat;

#else	// with tweening

	  D3DXMatrixIdentity(&matResult);
    D3DXQUATERNION q1,q2;
		D3DXQuaternionRotationMatrix( &q1, &m_pMatrixKeys[dwp2].mat );
		D3DXQuaternionRotationMatrix( &q2, &m_pMatrixKeys[dwp3].mat );

    D3DXQuaternionSlerp(&quat, &q1, &q2, fLerpValue);

    D3DXMatrixRotationQuaternion(&matTemp, &quat);

    D3DXMatrixMultiply(&matResult, &matResult, &matTemp);

		D3DXVECTOR3 v1,v2;
		v1.x = m_pMatrixKeys[dwp2].mat._41;
		v1.y = m_pMatrixKeys[dwp2].mat._42;
		v1.z = m_pMatrixKeys[dwp2].mat._43;

		v2.x = m_pMatrixKeys[dwp3].mat._41;
		v2.y = m_pMatrixKeys[dwp3].mat._42;
		v2.z = m_pMatrixKeys[dwp3].mat._43;
		D3DXVec3Lerp( &vPos, &v1, &v2, fLerpValue);

    D3DXMatrixTranslation(&matTemp, vPos.x, vPos.y, vPos.z);

    D3DXMatrixMultiply(&matResult, &matResult, &matTemp);

    pframeToAnimate->matRot = matResult;

#endif // tweening
  }
  else
  {
    D3DXMatrixIdentity(&matResult);

    if (m_pScaleKeys)
    {
      dwp2 = dwp3 = 0;

      fTime = (float)fmod(fGlobalTime, m_pScaleKeys[m_cScaleKeys-1].dwTime);

      for (iKey = 0 ;iKey < m_cScaleKeys ; iKey++)
      {
        if ((float)m_pScaleKeys[iKey].dwTime > fTime)
        {
          dwp3 = iKey;

          if (iKey > 0)
          {
            dwp2= iKey - 1;
          }
          else  // when iKey == 0, then dwp2 == 0
          {
            dwp2 = iKey;
          }

          break;
        }
      }
      fTime1 = (float)m_pScaleKeys[dwp2].dwTime;
      fTime2 = (float)m_pScaleKeys[dwp3].dwTime;

      if ((fTime2 - fTime1) ==0)
        fLerpValue = 0;
      else
        fLerpValue =  (fTime - fTime1)  / (fTime2 - fTime1);

      D3DXVec3Lerp(&vScale, &m_pScaleKeys[dwp2].vScale,
				&m_pScaleKeys[dwp3].vScale, fLerpValue);


      D3DXMatrixScaling(&matTemp, vScale.x, vScale.y, vScale.z);

      D3DXMatrixMultiply(&matResult, &matResult, &matTemp);

      bAnimate = true;
    }

    //check rot keys
    if (m_pRotateKeys )
    {
      dwp2 = dwp3 = 0;

      fTime = (float)fmod(fGlobalTime, m_pRotateKeys[m_cRotateKeys-1].dwTime);

      for (iKey = 0 ;iKey < m_cRotateKeys ; iKey++)
      {
        if ((float)m_pRotateKeys[iKey].dwTime > fTime)
        {
          dwp3 = iKey;

          if (iKey > 0)
          {
            dwp2= iKey - 1;
          }
          else  // when iKey == 0, then dwp2 == 0
          {
            dwp2 = iKey;
          }

          break;
        }
      }
      fTime1 = (float)m_pRotateKeys[dwp2].dwTime;
      fTime2 = (float)m_pRotateKeys[dwp3].dwTime;

      if ((fTime2 - fTime1) ==0)
        fLerpValue = 0;
      else
        fLerpValue =  (fTime - fTime1)  / (fTime2 - fTime1);

			// This is lame. Flip these on load, or fix slerp. [AJM]
      //s=0;
      D3DXQUATERNION q1,q2;
      q1.x =-m_pRotateKeys[dwp2].quatRotate.x;
      q1.y =-m_pRotateKeys[dwp2].quatRotate.y;
      q1.z =-m_pRotateKeys[dwp2].quatRotate.z;
      q1.w =m_pRotateKeys[dwp2].quatRotate.w;

      q2.x =-m_pRotateKeys[dwp3].quatRotate.x;
      q2.y =-m_pRotateKeys[dwp3].quatRotate.y;
      q2.z =-m_pRotateKeys[dwp3].quatRotate.z;
      q2.w =m_pRotateKeys[dwp3].quatRotate.w;

      D3DXQuaternionSlerp(&quat, &q1, &q2, fLerpValue);

      D3DXMatrixRotationQuaternion(&matTemp, &quat);

      D3DXMatrixMultiply(&matResult, &matResult, &matTemp);
      bAnimate = true;
    }

    if (m_pPositionKeys)
    {
      dwp2=dwp3=0;

      fTime = (float)fmod(fGlobalTime, m_pPositionKeys[m_cRotateKeys-1].dwTime);

      for (iKey = 0 ;iKey < m_cPositionKeys ; iKey++)
      {
        if ((float)m_pPositionKeys[iKey].dwTime > fTime)
        {
          dwp3 = iKey;

          if (iKey > 0)
          {
            dwp2= iKey - 1;
          }
          else  // when iKey == 0, then dwp2 == 0
          {
            dwp2 = iKey;
          }

          break;
        }
      }
      fTime1 = (float)m_pPositionKeys[dwp2].dwTime;
      fTime2 = (float)m_pPositionKeys[dwp3].dwTime;

      if ((fTime2 - fTime1) ==0)
        fLerpValue = 0;
      else
        fLerpValue =  (fTime - fTime1)  / (fTime2 - fTime1);


      D3DXVec3Lerp((D3DXVECTOR3*)&vPos, &m_pPositionKeys[dwp2].vPos,
				&m_pPositionKeys[dwp3].vPos, fLerpValue);

      D3DXMatrixTranslation(&matTemp, vPos.x, vPos.y, vPos.z);

      D3DXMatrixMultiply(&matResult, &matResult, &matTemp);
      bAnimate = true;
    }
    else
    {
      D3DXMatrixTranslation(&matTemp, pframeToAnimate->matRotOrig._41, pframeToAnimate->matRotOrig._42, pframeToAnimate->matRotOrig._43);

      D3DXMatrixMultiply(&matResult, &matResult, &matTemp);
			bAnimate = true;
    }

    if (bAnimate)
    {
      pframeToAnimate->matRot = matResult;
    }
  }
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: This function sets up object-specific render states
//       and renders the object.
//-----------------------------------------------------------------------------
HRESULT C3DFrameObject::Render(C3DEngine* pEngine)
{
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

//  D3DXMatrixTranslation(&mTrans, 0.0f, 0.0f, 0.0f);
  D3DXMatrixIdentity(&mTrans);
  D3DXMatrixIdentity(&mCur);

//    FLOAT fScale = 0.01f;
//    D3DXMatrixScaling(&mCur, fScale, fScale, fScale);

  D3DXMatrixTranslation(&mTrans, -pdeCur->vCenter.x, -pdeCur->vCenter.y, -pdeCur->vCenter.z);

  hr = m_pDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&mTrans);
  if(FAILED(hr))
    return hr;

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

  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: SetMatrices()
// Desc: Set object rotation and translation matrices.
//-----------------------------------------------------------------------------
HRESULT C3DFrameObject::SetMatrices( const D3DXMATRIX *pmRot, const D3DXMATRIX *pmTrans )
{
  SDrawElement *pdeCur = m_pdeHead;
  while( pdeCur != NULL )
  {
    pdeCur->pframeRoot->matRot = *pmRot;
    pdeCur->pframeRoot->matTrans = *pmTrans;
    pdeCur = pdeCur->pdeNext;
  }
  return S_OK;
}

D3DMATRIX *C3DFrameObject::GetRotationMatrix()
{
  if( m_pdeHead != NULL )
  {
    return &m_pdeHead->pframeRoot->matRot;
  }
  else
    return NULL;
}

D3DMATRIX *C3DFrameObject::GetTranslationMatrix()
{
  if( m_pdeHead != NULL )
  {
    return &m_pdeHead->pframeRoot->matTrans;
  }
  else
    return NULL;
}

//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT C3DFrameObject::InitDeviceObjects()
{
    HRESULT hr = S_OK;


    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT C3DFrameObject::RestoreDeviceObjects(LPDIRECT3DDEVICE8 m_pd3dDevice)
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
// Name: ReleaseDeviceDependentMeshes()
// Desc: 
//-----------------------------------------------------------------------------
void ReleaseDeviceDependentMeshes(SFrame* pFrame)
{
    if (pFrame->pmcMesh != NULL)
    {
        for (SMeshContainer* pmcCurr = pFrame->pmcMesh; pmcCurr != NULL; pmcCurr = pmcCurr->pmcNext)
        {
            if (pmcCurr->m_pSkinMesh != NULL)
            {
                GXRELEASE(pmcCurr->pMesh);
                GXRELEASE(pmcCurr->pMeshHW);
                GXRELEASE(pmcCurr->pMeshSW);

                pmcCurr->m_Method = NONE;
            }
        }
    }

    if (pFrame->pframeFirstChild != NULL)
        ReleaseDeviceDependentMeshes(pFrame->pframeFirstChild);

    if (pFrame->pframeSibling != NULL)
        ReleaseDeviceDependentMeshes(pFrame->pframeSibling);
}

//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//       this function releases any device dependent objects.
//-----------------------------------------------------------------------------
HRESULT C3DFrameObject::InvalidateDeviceObjects()
{
  for (SDrawElement* pdeCurr = m_pdeHead; pdeCurr != NULL; pdeCurr = pdeCurr->pdeNext)
  {
    ReleaseDeviceDependentMeshes(pdeCurr->pframeRoot);
  }

  return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//       this function deletes any device dependent objects.
//-----------------------------------------------------------------------------
HRESULT C3DFrameObject::DeleteDeviceObjects()
{
  if (m_pdeHead)
  {
    delete m_pdeHead;
    m_pdeHead = NULL;
  }
  
  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: FrameMove()
// Desc: Called once per frame, the call is the entry point for animating the object.
//	Walks draw element list at m_pdeHead
//	Calls SetTime() on each frame in list at pFrameAnimHead
//-----------------------------------------------------------------------------
HRESULT C3DFrameObject::FrameMove(float fElapsedTime)
{
	SDrawElement *pdeCur;
	SFrame *pFrameCur;

	pdeCur = m_pdeHead;
	while (pdeCur != NULL)
	{
		pdeCur->fCurTime += fElapsedTime * 4800;
		if (pdeCur->fCurTime > 1.0e15f)
			pdeCur->fCurTime = 0;

		pFrameCur = pdeCur->pframeAnimHead;
		while (pFrameCur != NULL)
		{
			pFrameCur->SetTime(pdeCur->fCurTime);
			pFrameCur = pFrameCur->pframeAnimNext;
		}

		pdeCur = pdeCur->pdeNext;
	}

	return S_OK;
}

// attach 6-DOF controller 
Controller *C3DFrameObject::Attach6DOFController( const char *szBoneName,
	float *pfX, float *pfY, float *pfZ,
	float *pfRoll, float *pfPitch, float *pfYaw )
{
	// find frame by bone name
	SFrame *pFrame = NULL;
  SDrawElement *pdeCur = m_pdeHead;
  while( pdeCur != NULL )
  {
		pFrame = m_pdeHead->FindFrame( szBoneName );
		if( pFrame )
			break;
    pdeCur = pdeCur->pdeNext;
  }

	if( !pFrame )
		return NULL;

	// create controller
	Controller6DOF *pController = new Controller6DOF();
	pController->SetFrame( pFrame );

	// set initial values from frame
	pController->SetX( pFrame->matRot._41 ); 
	pController->SetY( pFrame->matRot._42 ); 
	pController->SetZ( pFrame->matRot._43 ); 
	

	// set inputs
	pController->SetInputX( pfX );
	pController->SetInputY( pfY );
	pController->SetInputZ( pfZ );
	pController->SetInputRoll( pfRoll );
	pController->SetInputPitch( pfPitch );
	pController->SetInputYaw( pfYaw );

	// add to controller list
	pController->m_pNext = pdeCur->pControllerHead;
	pdeCur->pControllerHead = pController;

	return pController;
}

// attach animation controller 
Controller *C3DFrameObject::AttachAnimController( const char *szBoneName, float *fInput )
{
	// find animation frame linked to bone with this name
	SDrawElement *pdeCur;
	SFrame *pFrameCur;
	SFrame *pFrameFound = NULL;

	pdeCur = m_pdeHead;
	while( pdeCur )
	{
		pFrameCur = pdeCur->pframeAnimHead;
		while( pFrameCur )
		{
			const char *szName = pFrameCur->pframeToAnimate->szName;
			if( szName && ( 0 == stricmp( szBoneName, szName ) ) )
			{
				pFrameFound = pFrameCur;
				break;
			}
			pFrameCur = pFrameCur->pframeAnimNext;
		}
		if( pFrameFound )
			break;
		pdeCur = pdeCur->pdeNext;
	}

	if( !pFrameFound )
		return NULL;

	// create controller
	AnimController *pController = new AnimController();

	// attach animation frame to controller
	pController->SetFrame( pFrameFound );

	// set input
	pController->SetInput( fInput );

	// set max time from animation length
	DWORD dwLastFrame = pFrameFound->m_cMatrixKeys - 1;

	// Exporter last frame is same as first, so skip it
#define SKIP_LAST_FRAME 1
#if SKIP_LAST_FRAME
	// skip last frame
	if( dwLastFrame > 0 )
		--dwLastFrame;
#endif
	DWORD dwLastFrameTime = pFrameFound->m_pMatrixKeys[dwLastFrame].dwTime;
	if( !dwLastFrameTime )
		return NULL;
	pController->SetMaxTime( (float)dwLastFrameTime );

	// add to controller list
	pController->m_pNext = m_pdeHead->pControllerHead;
	m_pdeHead->pControllerHead = pController;

	return pController;
}

// attach physics controller 
Controller *C3DFrameObject::AttachPhysicsController( const char *szBoneName, float *fInput )
{
	// find frame by bone name
	SFrame *pFrame = NULL;
  SDrawElement *pdeCur = m_pdeHead;
  while( pdeCur != NULL )
  {
		pFrame = m_pdeHead->FindFrame( szBoneName );
		if( pFrame )
			break;
    pdeCur = pdeCur->pdeNext;
  }

	if( !pFrame )
		return NULL;

	// create controller
	PhysicsController *pController = new PhysicsController();
	pController->SetFrame( pFrame );

	// set input
	pController->SetInput( fInput );

	// set initial orientation and position from frame
	pController->SetMatrix( pFrame->matRot ); 
	
	// add to controller list
	pController->m_pNext = pdeCur->pControllerHead;
	pdeCur->pControllerHead = pController;

	return pController;
}


HRESULT C3DFrameObject::TickControllers( float fElapsedTime )
{
	SDrawElement *pdeCur = m_pdeHead;
  while( pdeCur != NULL )
  {
		Controller *pController = pdeCur->pControllerHead;
	  while( pController != NULL )
	  {
			pController->Tick( fElapsedTime );
			pController = pController->m_pNext;
		}
    pdeCur = pdeCur->pdeNext;
  }
	return S_OK;
}

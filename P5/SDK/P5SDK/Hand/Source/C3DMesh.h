//-----------------------------------------------------------------------------
// File: C3DMesh.h
//
// Desc: Support code for loading DirectX .X files.
//
//-----------------------------------------------------------------------------
#ifndef C3DMESH_H
#define C3DMESH_H
//#include <tchar.h>
#include <d3d8.h>
#include <d3dx8.h>
//#include "C3DEngine.h"

//class C3DEngine;

class C3DMesh
{
public:
  C3DMesh();
	~C3DMesh();

  HRESULT Load( const char *szFilename, LPDIRECT3DDEVICE8 pDevice8 );
  void ReleaseDeviceDependentMeshes();

protected:
//  HRESULT CalculateBoundingSphere();

private:
//  ID3DXMesh *pMesh;
//  ID3DXMesh *pMeshHW;
//  ID3DXMesh *pMeshSW;

  D3DMATERIAL8 *m_pMaterials;
  LPDIRECT3DTEXTURE8 *m_pTextures;
//  DWORD cpattr;
  DWORD m_dwNumMaterials;

//  C3DMesh *pmcNext;
  char *szName;

  LPD3DXEFFECT m_pEffect;
  LPD3DXTECHNIQUE m_pTechnique;

  // for optimizing
  LPD3DXBUFFER m_pbufAdjacency;

};

#endif




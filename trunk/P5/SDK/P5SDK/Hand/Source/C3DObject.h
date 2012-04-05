//-----------------------------------------------------------------------------
// File: C3DObject.h
//
// Desc: Support code for loading DirectX .X files.
//
//-----------------------------------------------------------------------------
#ifndef C3DOBJECT_H
#define C3DOBJECT_H
#include <tchar.h>
#include <d3d8.h>
#include <d3dx8.h>
#include "C3DEngine.h"

class C3DEngine;
class C3DMesh;

extern DWORD      g_dwNumDeviceTypes;
//-----------------------------------------------------------------------------
// Name: class C3DObject
// Desc: Application class. The base class (C3DApplication) provides the 
//       generic functionality needed in all Direct3D samples. C3DObject 
//       adds functionality specific to this sample program.
//-----------------------------------------------------------------------------
class C3DObject
{
public:
  C3DObject();
  virtual ~C3DObject() {}
  virtual HRESULT Init(C3DEngine* pEngine, char* szFileName);
  virtual HRESULT Destroy();

  virtual HRESULT InitDeviceObjects();
  virtual HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICE8 m_pd3dDevice);
  virtual HRESULT InvalidateDeviceObjects();
  virtual HRESULT DeleteDeviceObjects();
  virtual HRESULT Render(C3DEngine* pEngine);
  virtual HRESULT FrameMove(float /*fElapsedTime*/);
	virtual HRESULT TickControllers() { return S_OK; }

  virtual HRESULT SetMatrices( const D3DXMATRIX *pmRot, const D3DXMATRIX *pmTrans );
  virtual D3DMATRIX *GetRotationMatrix();
  virtual D3DMATRIX *GetTranslationMatrix();

protected:
  HRESULT LoadMesh( const char *szFileName );
  HRESULT DrawMesh();
  HRESULT CalculateBoundingSphere();

  // member variables
  bool m_bSelected;
  char m_szFileName[256];
	C3DMesh *m_pMesh;
  C3DEngine* m_pEngine;

  LPD3DXEFFECT m_pEffect;
  LPD3DXTECHNIQUE m_pTechnique;
};


#endif




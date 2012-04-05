//-----------------------------------------------------------------------------
// File: C3DFrameObject.h
//
// Desc: Support code for loading DirectX .X files.
//
//-----------------------------------------------------------------------------
#ifndef C3DFrameObject_H
#define C3DFrameObject_H
#include <tchar.h>
#include <d3d8.h>
#include <d3dx8.h>
#include "C3DEngine.h"
#include "C3DText.h"

class C3DEngine;

struct SMeshContainer
{
  ID3DXMesh *pMesh;
  ID3DXMesh* pMeshHW;
  ID3DXMesh* pMeshSW;
  D3DMATERIAL8 *rgMaterials;
  LPDIRECT3DTEXTURE8 *pTextures;
  DWORD cpattr;
  DWORD cMaterials;
  DWORD iAttrSplit;

  SMeshContainer *pmcNext;

  char *szName;

  // Skin info
  LPD3DXSKINMESH m_pSkinMesh;
  D3DXATTRIBUTERANGE *m_pAttrTable;
  D3DXMATRIX** m_pBoneMatrix;
  LPD3DXBUFFER m_pBoneNamesBuf;
  LPD3DXBUFFER m_pBoneOffsetBuf;
  D3DXMATRIX* m_pBoneOffsetMat;
  DWORD* m_rgiAdjacency;
  DWORD m_numBoneComb;
  DWORD m_maxFaceInfl;
  LPD3DXBUFFER m_pBoneCombinationBuf;
  METHOD  m_Method;

  SMeshContainer() :
    pMesh(NULL),
    pMeshHW(NULL),
    pMeshSW(NULL),
    rgMaterials(NULL),
    pTextures(NULL),
    cpattr(0),
    iAttrSplit(0),
    cMaterials(0),
    pmcNext(NULL),
    szName(NULL),
    m_pSkinMesh(NULL),
    m_pAttrTable(NULL),
    m_pBoneMatrix(NULL),
    m_pBoneNamesBuf(NULL),
    m_pBoneOffsetBuf(NULL),
    m_pBoneOffsetMat(NULL),
    m_rgiAdjacency(NULL),
    m_numBoneComb(0),
    m_maxFaceInfl(0),
    m_pBoneCombinationBuf(NULL),
    m_Method(NONE)
	{
	}

	~SMeshContainer()
	{
    delete [] rgMaterials;

    if (pTextures)
    {
      for (DWORD i = 0; i < cMaterials; ++i)
        GXRELEASE(pTextures[i]);
      delete [] pTextures;
    }

		GXRELEASE(pMesh);
    GXRELEASE(pMeshHW);
    GXRELEASE(pMeshSW);
    GXRELEASE(m_pSkinMesh);
    GXRELEASE(m_pBoneNamesBuf);
    GXRELEASE(m_pBoneOffsetBuf);
    GXRELEASE(m_pBoneCombinationBuf);

    delete [] m_pBoneMatrix;
    delete [] m_pAttrTable;
    delete [] szName;
    delete [] m_rgiAdjacency;
    delete pmcNext;
	}
};

// X File formation rotate key
struct SRotateKeyXFile
{
	DWORD			dwTime;
	DWORD			dwFloats;	
	float			w;
	float			x;
	float			y;
	float			z;
};

struct SScaleKeyXFile
{
	DWORD	dwTime;
	DWORD	dwFloats;	
	D3DXVECTOR3	vScale;	
};


struct SPositionKeyXFile
{
	DWORD	dwTime;
	DWORD	dwFloats;	
	D3DXVECTOR3	vPos;	
};

struct SMatrixKeyXFile
{
	DWORD	dwTime;
	DWORD	dwFloats;	
	D3DXMATRIX	mat;	
};

// in memory versions

struct SRotateKey
{
	DWORD			dwTime;
	D3DXQUATERNION	quatRotate;	
};

struct SPositionKey
{
	DWORD	dwTime;
	D3DXVECTOR3	vPos;	
};

struct SScaleKey
{
	DWORD	dwTime;
	D3DXVECTOR3	vScale;	
};

struct SMatrixKey
{
	DWORD	dwTime;
	D3DXMATRIX	mat;	
};

struct SFrame
{
  SMeshContainer *pmcMesh;
  D3DXMATRIX matRot;
  D3DXMATRIX matTrans;
  D3DXMATRIX matRotOrig;
  D3DXMATRIX matCombined;

  // animation information
  SPositionKey *m_pPositionKeys;
  UINT m_cPositionKeys;
  SRotateKey *m_pRotateKeys;
  UINT m_cRotateKeys;
  SScaleKey *m_pScaleKeys;
  UINT m_cScaleKeys;
  SMatrixKey *m_pMatrixKeys;
  UINT m_cMatrixKeys;

  SFrame *pframeAnimNext;
  SFrame *pframeToAnimate;

  SFrame *pframeSibling;
  SFrame *pframeFirstChild;

  bool bAnimationFrame;
  char *szName;

  SFrame() :
    pmcMesh(NULL),
    m_pPositionKeys(NULL),
    m_cPositionKeys(0),
    m_pScaleKeys(NULL),
    m_cScaleKeys(0),
    m_pRotateKeys(NULL),
    m_cRotateKeys(0),
    m_pMatrixKeys(NULL),
    m_cMatrixKeys(0),
    pframeAnimNext(NULL),
    pframeToAnimate(NULL),
    pframeSibling(NULL),
    pframeFirstChild(NULL),
    bAnimationFrame(false),
    szName(NULL)
  {
    D3DXMatrixIdentity(&matRot);
    D3DXMatrixIdentity(&matRotOrig);
    D3DXMatrixIdentity(&matTrans);
  }

  ~SFrame()
  {
    delete []szName;
    delete pmcMesh;        
    delete pframeFirstChild;
    delete pframeSibling;

    delete []m_pPositionKeys;
    delete []m_pRotateKeys;
    delete []m_pScaleKeys;
    delete []m_pMatrixKeys;

    // do NOT delete pframeAnimNext
    // do NOT delete pframeToAnimate
  }

  void SetTime(float fTime);

  SFrame *FindFrame(const char *szFrame)
  {
      SFrame *pframe;

      if ((szName != NULL) && (strcmp(szName, szFrame) == 0))
          return this;

      if (pframeFirstChild != NULL)
      {
          pframe = pframeFirstChild->FindFrame(szFrame);
          if (pframe != NULL)
              return pframe;
      }

      if (pframeSibling != NULL)
      {
          pframe = pframeSibling->FindFrame(szFrame);
          if (pframe != NULL)
              return pframe;
      }

      return NULL;
  }

  void ResetMatrix()
  {
    matRot = matRotOrig;
    D3DXMatrixIdentity(&matTrans);        

    if (pframeFirstChild != NULL)
      pframeFirstChild->ResetMatrix();

    if (pframeSibling != NULL)
      pframeSibling->ResetMatrix();
  }

  void AddFrame(SFrame *pframe)
  {
    if (pframeFirstChild == NULL)
      pframeFirstChild = pframe;
    else
    {
      pframe->pframeSibling = pframeFirstChild->pframeSibling;
      pframeFirstChild->pframeSibling = pframe;
    }
  }

  void AddMesh(SMeshContainer *pmc)
  {
    pmc->pmcNext = pmcMesh;
    pmcMesh = pmc;
  }
};

class Controller
{
public:
	Controller() :
		m_pText(NULL),
		m_pNext(NULL)
	{
	}
		
	virtual void Tick() = 0;
	void SetText( C3DText *pText )	{ m_pText = pText; }

	Controller *m_pNext;
	C3DText *m_pText;
};

class AnimController : public Controller
{
public:	
	AnimController() :
		m_pfInput(NULL),
		m_fMaxTime(1.0f),
		m_pFrameAnim(NULL)
	{
	}

	void SetFrame( SFrame *pFrame )	{ m_pFrameAnim = pFrame; }
	void SetInput( float *pfInput )	{ m_pfInput = pfInput; }
	void SetMaxTime( float fTime )	{ m_fMaxTime = fTime; }
	void Tick()
	{
		if( !m_pfInput )
			return;
		m_pFrameAnim->SetTime( (*m_pfInput) * m_fMaxTime );

		// update text
		if( m_pText )
		{
			int iLine = 1;
			char buf[MAX_LINE_LENGTH];
			sprintf( buf, "%.4f", *m_pfInput );
			m_pText->SetLine( iLine++, buf );
		}
	}

private:	
	SFrame *m_pFrameAnim;
	float		m_fMaxTime;
	float	 *m_pfInput;

};

class Controller6DOF : public Controller
{
public:	
	Controller6DOF() :
		m_pFrame(NULL),
		m_pfX(NULL),
		m_pfY(NULL),
		m_pfZ(NULL),
		m_pfRoll(NULL),
		m_pfPitch(NULL),
		m_pfYaw(NULL),
		m_fX(0.0f),
		m_fY(0.0f),
		m_fZ(0.0f),
		m_fRoll(0.0f),
		m_fPitch(0.0f),
		m_fYaw(0.0f)
	{
	}

	void SetFrame( SFrame *pFrame )	{ m_pFrame = pFrame; }

	void SetInputX( float *pfX )	{ m_pfX = pfX; }
	void SetInputY( float *pfY )	{ m_pfY = pfY; }
	void SetInputZ( float *pfZ )	{ m_pfZ = pfZ; }
	void SetInputRoll( float *pfRoll )	{ m_pfRoll = pfRoll; }
	void SetInputPitch( float *pfPitch )	{ m_pfPitch = pfPitch; }
	void SetInputYaw( float *pfYaw )	{ m_pfYaw = pfYaw; }

	void SetX( float fX )	{ m_fX = fX; }
	void SetY( float fY )	{ m_fY = fY; }
	void SetZ( float fZ )	{ m_fZ = fZ; }
	void SetRoll( float fRoll )	{ m_fRoll = fRoll; }
	void SetPitch( float fPitch )	{ m_fPitch = fPitch; }
	void SetYaw( float fYaw )	{ m_fYaw = fYaw; }

	void Tick()
	{
		if( !m_pFrame )
			return;

		// sample rotational inputs
		if( m_pfRoll )
			m_fRoll = *m_pfRoll * 2.0f * D3DX_PI;
		if( m_pfPitch )
			m_fPitch = *m_pfPitch * 2.0f * D3DX_PI;
		if( m_pfYaw )
			m_fYaw = *m_pfYaw * 2.0f * D3DX_PI;

		D3DXMATRIX matRot;
		
		// build rotation matrix
		D3DXMatrixRotationYawPitchRoll( &matRot, m_fYaw, m_fPitch, m_fRoll );

		// sample translational inputs
		if( m_pfX )
			m_fX = *m_pfX;
		if( m_pfY )
			m_fY = *m_pfY;
		if( m_pfZ )
			m_fZ = *m_pfZ;

		// build translation matrix
		// flip sign of X (artifact of conversion from LH to RH coordinate system)
		D3DXMATRIX matTrans;
		D3DXMatrixTranslation( &matTrans, m_fX, m_fY, m_fZ );

		D3DXMATRIX matResult;
    D3DXMatrixMultiply(&matResult, &matRot, &matTrans);

		// store result into frame
		m_pFrame->matRot = matResult;

		// update text
		if( m_pText )
		{
			int iLine = 1;
			char buf[MAX_LINE_LENGTH];

			if( m_pfX || m_pfY || m_pfZ )
			{
				sprintf( buf, "X:  %.1f    Y:  %.1f    Z:  %.1f", -m_fX, m_fY, m_fZ );
				m_pText->SetLine( iLine++, buf );
			}

			// pitch only
			if( !m_pfRoll && m_pfPitch && !m_pfYaw )
			{
				sprintf( buf, "%.1f", D3DXToDegree(m_fPitch) );
				m_pText->SetLine( iLine++, buf );
			}
//			else if( m_pfRoll && m_pfPitch && m_pfYaw )
//			{
//				sprintf( buf, "YAW:  %.1f    PITCH:  %.1f    ROLL:  %.1f",
//					D3DXToDegree(m_fYaw), D3DXToDegree(m_fPitch), D3DXToDegree(m_fRoll) );
//				m_pText->SetLine( iLine++, buf );
//			}
		}
	}

private:	
	float						*m_pfInput;

	float m_fX, m_fY, m_fZ, m_fRoll, m_fPitch, m_fYaw;
	
	float						*m_pfX;
	float						*m_pfY;
	float						*m_pfZ;
	float						*m_pfRoll;
	float						*m_pfPitch;
	float						*m_pfYaw;

	SFrame					*m_pFrame;
};

struct SDrawElement
{
  SFrame *pframeRoot;

  D3DXVECTOR3 vCenter;
  float fRadius;

	// name of element for selection purposes
	char *szName;

  // animation list
  SFrame *pframeAnimHead;

	// controller list
  Controller *pControllerHead;

	// next element in list
	SDrawElement *pdeNext;

  float fCurTime;
  float fMaxTime;

  SDrawElement() :
    vCenter(0.0,0.0,0.0),
		fRadius(1.0),
		szName(NULL),
    pframeRoot(NULL),
    pframeAnimHead(NULL),
		pControllerHead(NULL),
  	pdeNext(NULL),
		fCurTime(0.0f)
	{
	}

	~SDrawElement()
	{
    delete pframeRoot;
		delete pdeNext;
    delete [] szName;

    // do NOT delete pframeAnimHead;
	}

  void AddAnimationFrame(SFrame *pframeAnim)
  {
    pframeAnim->pframeAnimNext = pframeAnimHead;
    pframeAnimHead = pframeAnim;
  }

  SFrame *FindFrame(const char *szName)
  {
    if (pframeRoot == NULL)
      return NULL;
    else
      return pframeRoot->FindFrame(szName);
  }
};

extern DWORD      g_dwNumDeviceTypes;
//-----------------------------------------------------------------------------
// Name: class C3DFrameObject
// Desc: A C3DObject using the D3DX frames for animation
//-----------------------------------------------------------------------------
class C3DFrameObject : public C3DObject
{
public:
  C3DFrameObject();
  ~C3DFrameObject() {}
  HRESULT Init(C3DEngine* pEngine, char* szFileName);
  HRESULT Destroy();

  HRESULT InitDeviceObjects();
  HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICE8 m_pd3dDevice);
  HRESULT InvalidateDeviceObjects();
  HRESULT DeleteDeviceObjects();
  HRESULT Render(C3DEngine* pEngine);
  HRESULT FrameMove(float fElapsedTime);
  HRESULT TickControllers();

  HRESULT SetMatrices( const D3DXMATRIX *pmRot, const D3DXMATRIX *pmTrans );
  D3DMATRIX *GetRotationMatrix();
  D3DMATRIX *GetTranslationMatrix();

	Controller *Attach6DOFController( const char *szBoneName, float *fX, float *fY, float *fZ,
		float *fRoll, float *fPitch, float *fYaw );

	Controller *AttachAnimController( const char *szBoneName, float *fInput );

protected:
  HRESULT LoadAnimationSet(LPDIRECTXFILEDATA pxofobjCur, SDrawElement *pde,
    DWORD options, DWORD fvf, LPDIRECT3DDEVICE8 pD3DDevice, SFrame *pframeParent);
  HRESULT LoadAnimation(LPDIRECTXFILEDATA pxofobjCur, SDrawElement *pde,
    DWORD options, DWORD fvf, LPDIRECT3DDEVICE8 pD3DDevice, SFrame *pframeParent);
  HRESULT LoadFrames(LPDIRECTXFILEDATA pxofobjCur, SDrawElement *pde,
    DWORD options, DWORD fvf, LPDIRECT3DDEVICE8 pD3DDevice, SFrame *pframeParent);
  HRESULT LoadMesh(LPDIRECTXFILEDATA pxofobjCur,
    DWORD options, DWORD fvf, LPDIRECT3DDEVICE8 pD3DDevice, SFrame *pframeParent);
  HRESULT DrawMeshContainer(SMeshContainer *pmcMesh, LPDIRECT3DDEVICE8 m_pDevice);
  HRESULT UpdateFrames(SFrame *pframeCur, D3DXMATRIX &matCur);
  HRESULT GenerateMesh(SMeshContainer* pmcMesh);
  HRESULT DrawFrames(SFrame *pframeCur, UINT &cTriangles, LPDIRECT3DDEVICE8 m_pDevice);
  HRESULT FindBones(SFrame *pframeCur, SDrawElement *pde);
  HRESULT CalculateBoundingSphere(SDrawElement *pdeCur);

  // member variables
	LPDIRECT3DDEVICE8 m_pd3dDevice;
  char m_szFileName[MAX_PATH];
	SDrawElement *m_pdeHead;

  LPD3DXEFFECT m_pEffect;
  LPD3DXTECHNIQUE m_pTechnique;
};


#endif




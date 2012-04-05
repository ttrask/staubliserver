//-----------------------------------------------------------------------------
// File: C3DCamera.h
//
// Desc: 
//
//-----------------------------------------------------------------------------
#ifndef C3DCAMERA_H
#define C3DCAMERA_H
#include <D3D8.h>
#include <D3DX8Math.h>


//-----------------------------------------------------------------------------
// Name: class C3DCamera
// Desc:
//-----------------------------------------------------------------------------
class C3DCamera
{
	D3DXVECTOR3 m_vEyePt;       // Attributes for view matrix
	D3DXVECTOR3 m_vLookatPt;
	D3DXVECTOR3 m_vUpVec;

	D3DXVECTOR3 m_vView;
	D3DXVECTOR3 m_vCross;

	D3DXMATRIX  m_matView;
	D3DXMATRIX  m_matBillboard; // Special matrix for billboarding effects

	FLOAT       m_fFOV;         // Attributes for projection matrix
	FLOAT       m_fAspect;
	FLOAT       m_fNearPlane;
	FLOAT       m_fFarPlane;
	D3DXMATRIX  m_matProj;

public:
	// Access functions
	D3DXVECTOR3 GetEyePt()           { return m_vEyePt; }
	D3DXVECTOR3 GetLookatPt()        { return m_vLookatPt; }
	D3DXVECTOR3 GetUpVec()           { return m_vUpVec; }
	D3DXVECTOR3 GetViewDir()         { return m_vView; }
	D3DXVECTOR3 GetCross()           { return m_vCross; }

	D3DXMATRIX  GetViewMatrix()      { return m_matView; }
	D3DXMATRIX  GetBillboardMatrix() { return m_matBillboard; }
	D3DXMATRIX  GetProjMatrix()      { return m_matProj; }

	VOID SetViewParams( D3DXVECTOR3 &vEyePt, D3DXVECTOR3& vLookatPt,
						D3DXVECTOR3& vUpVec );
	VOID SetProjParams( FLOAT fFOV, FLOAT fAspect, FLOAT fNearPlane,
						FLOAT fFarPlane );

	C3DCamera();
};


#endif // C3DCAMERA_H

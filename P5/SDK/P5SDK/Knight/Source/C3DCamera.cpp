//-----------------------------------------------------------------------------
// File: C3DCamera.cpp
//
// Desc: 
//
//
//-----------------------------------------------------------------------------

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <stdio.h>
#include "C3DCamera.h"
#include "C3DUtil.h"
#include <D3DX8.h>
#include <DDraw.h>
#include <d3d8.h>

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
C3DCamera::C3DCamera()
{
	m_vInitialEyePt = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vInitialLookatPt = D3DXVECTOR3(0.0f,0.0f,1.0f);
	m_vInitialUpVec = D3DXVECTOR3(0.0f,1.0f,0.0f);

  // Set attributes for the view matrix
  SetViewParams( D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,1.0f),
                 D3DXVECTOR3(0.0f,1.0f,0.0f) );

  // Set attributes for the projection matrix
  SetProjParams( D3DX_PI/4, 1.0f, 1.0f, 30.0f );
}

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
VOID C3DCamera::SetViewParams( D3DXVECTOR3 &vEyePt, D3DXVECTOR3& vLookatPt,
                                D3DXVECTOR3& vUpVec )
{
  // Set attributes for the view matrix
  m_vEyePt    = vEyePt;
  m_vLookatPt = vLookatPt;
  m_vUpVec    = vUpVec;
  D3DXVec3Normalize( &m_vView, &(m_vLookatPt - m_vEyePt) );
  D3DXVec3Cross( &m_vCross, &m_vView, &m_vUpVec );

  D3DXMatrixLookAtRH( &m_matView, &m_vEyePt, &m_vLookatPt, &m_vUpVec );
  D3DXMatrixInverse( &m_matBillboard, NULL, &m_matView );
  m_matBillboard._41 = 0.0f;
  m_matBillboard._42 = 0.0f;
  m_matBillboard._43 = 0.0f;
}

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
VOID C3DCamera::SetEyePt( FLOAT fX, FLOAT fY, FLOAT fZ )
{
	m_vEyePt = D3DXVECTOR3( fX, fY, fZ );
	SetViewParams( m_vEyePt, m_vLookatPt, m_vUpVec );
}


//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
VOID C3DCamera::SetLookatPt( FLOAT fX, FLOAT fY, FLOAT fZ )
{
	m_vLookatPt = D3DXVECTOR3( fX, fY, fZ );
	SetViewParams( m_vEyePt, m_vLookatPt, m_vUpVec );
}

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
VOID C3DCamera::SetUpVec( FLOAT fX, FLOAT fY, FLOAT fZ )
{
	m_vUpVec = D3DXVECTOR3( fX, fY, fZ );
	SetViewParams( m_vEyePt, m_vLookatPt, m_vUpVec );
}

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
VOID C3DCamera::SetProjParams( FLOAT fFOV, FLOAT fAspect, FLOAT fNearPlane,
                                FLOAT fFarPlane )
{
  // Set attributes for the projection matrix
  m_fFOV        = fFOV;
  m_fAspect     = fAspect;
  m_fNearPlane  = fNearPlane;
  m_fFarPlane   = fFarPlane;

  D3DXMatrixPerspectiveFovRH( &m_matProj, fFOV, fAspect, fNearPlane, fFarPlane );
}

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
VOID C3DCamera::SetFieldOfView( FLOAT fFOV )
{
	m_fFOV = fFOV;
	SetProjParams( fFOV, m_fAspect, m_fNearPlane, m_fFarPlane );
}

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
VOID C3DCamera::SetNearAndFarPlanes( FLOAT fNear, FLOAT fFar )
{
	m_fNearPlane = fNear;
	m_fFarPlane = fFar;
	SetProjParams( m_fFOV, m_fAspect, fNear, fFar );
}

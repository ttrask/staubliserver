//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2001 Ninjaneering.  All Rights Reserved.
//
//  File:       d3dx8math2.inl
//  Content:    Extensions of D3DX math inline functions
//
//////////////////////////////////////////////////////////////////////////////

#ifndef D3DX8MATH2_INL
#define D3DX8MATH2_INL

#include <d3d8.h>

//===========================================================================
//
// Inline functions
//
//===========================================================================

//--------------------------
// 4D Matrix
//--------------------------

D3DXINLINE D3DXMATRIX* D3DXMatrixSkewSymmetric
    ( D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pVector )
{
  pOut->m[0][0] = 0.0f;					pOut->m[0][1] = pVector->z;		pOut->m[0][2] = -pVector->y;
  pOut->m[1][0] = -pVector->z;	pOut->m[1][1] = 0.0f;					pOut->m[1][2] = pVector->x;
  pOut->m[2][0] = pVector->y;		pOut->m[2][1] = -pVector->x;	pOut->m[2][2] = 0.0f;

	pOut->m[0][3] = pOut->m[1][3] = pOut->m[2][3] =
	pOut->m[3][0] = pOut->m[3][1] = pOut->m[3][2] =
	pOut->m[3][3]	= 0.0f;
  return pOut;
}

D3DXINLINE D3DXMATRIX* D3DXMatrixOrthonormalize
    ( D3DXMATRIX *pOut, CONST D3DXMATRIX *pM )
{
	D3DXVECTOR3 x( pM->m[0][0], pM->m[1][0], pM->m[2][0] );
	D3DXVECTOR3 y( pM->m[0][1], pM->m[1][1], pM->m[2][1] );
	D3DXVECTOR3 z;

	D3DXVec3Normalize( &x, &x );
	D3DXVec3Cross( &z, &x, &y );
	D3DXVec3Normalize( &z, &z );
	D3DXVec3Cross( &y, &z, &x );
	D3DXVec3Normalize( &y, &y );

	pOut->m[0][0] = x.x;	pOut->m[0][1] = y.x;	pOut->m[0][2] = z.x;	
	pOut->m[1][0] = x.y;	pOut->m[1][1] = y.y;	pOut->m[1][2] = z.y;	
	pOut->m[2][0] = x.z;	pOut->m[2][1] = y.z;	pOut->m[2][2] = z.z;	
	return pOut;
}

#endif // __D3DX8MATH_INL__

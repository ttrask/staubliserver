// Shadow.h: interface for the CShadow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHADOW_H__ACE6CFB8_D408_4368_A1C7_D1340B4A5EA5__INCLUDED_)
#define AFX_SHADOW_H__ACE6CFB8_D408_4368_A1C7_D1340B4A5EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CageObject.h"

class CShadow : public CCageObject  
{
public:
	CShadow(CCageApp* pCageApp,  TCHAR* strName, float scaleFactor );
	virtual ~CShadow();

	/////////////////////////////////////////////////////
	// interface

	virtual	HRESULT Create( LPDIRECT3DDEVICE8 pd3dDevice, TCHAR* strFilename );
    HRESULT RenderShadows( LPDIRECT3DDEVICE8 pd3dDevice, D3DXVECTOR3 starPosition,
		            BOOL bDrawOpaqueSubsets = TRUE,
		            BOOL bDrawAlphaSubsets = TRUE );

private:

};

#endif // !defined(AFX_SHADOW_H__ACE6CFB8_D408_4368_A1C7_D1340B4A5EA5__INCLUDED_)

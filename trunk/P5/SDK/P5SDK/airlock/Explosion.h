// Explosion.h: interface for the CExplosion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPLOSION_H__46EF5436_EEE7_4C1E_876F_982129DEECF6__INCLUDED_)
#define AFX_EXPLOSION_H__46EF5436_EEE7_4C1E_876F_982129DEECF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "CageObject.h"

class CExplosion : public CCageObject  
{
public:
	CExplosion(CCageApp* pCageApp,  TCHAR* strName, float scaleFactor );
	virtual ~CExplosion();

	/////////////////////////////////////////////////////
	// interface

	virtual void	Update(FLOAT fElapsedAppTime=0);	
    virtual HRESULT RenderCageObject( LPDIRECT3DDEVICE8 pd3dDevice, 
		            BOOL bDrawOpaqueSubsets = TRUE,
		            BOOL bDrawAlphaSubsets = TRUE );
	void StartExplosion(D3DXVECTOR3 pos);

private:

	bool	m_bIsExploding;
	double	m_dElapsedExplosionTime;
	double	m_dTargetExplosionTime;
	double	m_dTargetScaleMultiplier;

	D3DXVECTOR3	m_vCurrentScaleMultiplier;
};


#endif // !defined(AFX_EXPLOSION_H__46EF5436_EEE7_4C1E_876F_982129DEECF6__INCLUDED_)

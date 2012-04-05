// ScoringStar.h: interface for the CScoringStar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCORINGSTAR_H__35172DDD_A1A2_414C_836D_F219501EF633__INCLUDED_)
#define AFX_SCORINGSTAR_H__35172DDD_A1A2_414C_836D_F219501EF633__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CageObject.h"

class CScoringStar : public CCageObject  
{
public:
	CScoringStar(CCageApp* pCageApp,  TCHAR* strName, float scaleFactor, bool bIsDark);
	virtual ~CScoringStar();

	/////////////////////////////////////////////////////
	// interface

	virtual	HRESULT Create( LPDIRECT3DDEVICE8 pd3dDevice, TCHAR* strFilename );	
	virtual void	Update(FLOAT fElapsedAppTime=0);
    HRESULT RenderCageObject( LPDIRECT3DDEVICE8 pd3dDevice, 
		            BOOL bDrawOpaqueSubsets = TRUE,
		            BOOL bDrawAlphaSubsets = TRUE );

private:
	bool	m_bIsDark;
};

#endif // !defined(AFX_SCORINGSTAR_H__35172DDD_A1A2_414C_836D_F219501EF633__INCLUDED_)

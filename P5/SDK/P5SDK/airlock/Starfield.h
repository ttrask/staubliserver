// Starfield.h: interface for the CStarfield class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STARFIELD_H__3F87A733_2B1F_40A3_9363_879A403234CB__INCLUDED_)
#define AFX_STARFIELD_H__3F87A733_2B1F_40A3_9363_879A403234CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CageObject.h"

class CStarfield : public CCageObject  
{
public:
	CStarfield(CCageApp* pCageApp,  TCHAR* strName, float scaleFactor );
	virtual ~CStarfield();

	/////////////////////////////////////////////////////
	// interface

	virtual	HRESULT Create( LPDIRECT3DDEVICE8 pd3dDevice, TCHAR* strFilename );	
	virtual void	Update(FLOAT fElapsedAppTime=0);
    virtual HRESULT RenderCageObject( LPDIRECT3DDEVICE8 pd3dDevice, 
		            BOOL bDrawOpaqueSubsets = TRUE,
		            BOOL bDrawAlphaSubsets = TRUE );

private:

	
};

#endif // !defined(AFX_STARFIELD_H__3F87A733_2B1F_40A3_9363_879A403234CB__INCLUDED_)

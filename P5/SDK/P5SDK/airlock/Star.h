// Star.h: interface for the CStar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STAR_H__C7C6E0F7_D904_4374_BB12_67F8B05BEF25__INCLUDED_)
#define AFX_STAR_H__C7C6E0F7_D904_4374_BB12_67F8B05BEF25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CageObject.h"

class CStar : public CCageObject  
{
public:
	CStar(CCageApp* pCageApp,  TCHAR* strName, float scaleFactor );
	virtual ~CStar();

	/////////////////////////////////////////////////////
	// interface

	virtual	HRESULT Create( LPDIRECT3DDEVICE8 pd3dDevice, TCHAR* strFilename );	
	virtual void	Update(FLOAT fElapsedAppTime=0);

    HRESULT RenderCageObject( LPDIRECT3DDEVICE8 pd3dDevice, 
		            BOOL bDrawOpaqueSubsets = TRUE,
		            BOOL bDrawAlphaSubsets = TRUE );

	virtual void InitForNewLevel();


	/////////////////////////////////////////////////////
	// accessors

	bool IsInPlay()				{ return !m_bCaught && !m_bDying && !m_bDead && !m_bBeingBorn; }
	void SetCaught(bool bCaught){ m_bCaught = bCaught;	}
	bool IsBeingBorn()			{ return m_bBeingBorn;	}
	bool IsCaught()				{ return m_bCaught;		}
	bool IsDead()				{ return m_bDead;		}
	bool IsDying()				{ return m_bDying;		}



private:
	void	DoLighting(double fElapsedAppTime);
	void	GenerateRandomWaypoint();
	double	DistanceToWayPoint();
	void	GetCurrentMatrix(D3DXMATRIX* pMat);



	D3DXVECTOR3 m_NextWaypoint;

	// star lifecycle:
	// star is being born when approaching playing field at beginning of level
	// star is in play
	// star is caught (when it's in cage)
	// star is dying (when it's headed into vortex)
	// star dead (not visible)
	bool m_bBeingBorn;
	bool m_bCaught;
	bool m_bDying;
	bool m_bDead;



	D3DXVECTOR3 m_DeathWaypoint;

};

#endif // !defined(AFX_STAR_H__C7C6E0F7_D904_4374_BB12_67F8B05BEF25__INCLUDED_)

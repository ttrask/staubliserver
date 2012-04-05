// CageObject.h: interface for the CCageObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAGEOBJECT_H__45E558EF_42E4_4C10_9AB6_94BC08BC6C46__INCLUDED_)
#define AFX_CAGEOBJECT_H__45E558EF_42E4_4C10_9AB6_94BC08BC6C46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "d3dfile.h"

class CCageApp;

class CCageObject  : public CD3DMesh
{
public:
	CCageObject(CCageApp* pCageApp,  TCHAR* strName, float scaleFactor );
	virtual ~CCageObject();

	/////////////////////////////////////////////////////
	// interface

	virtual	HRESULT Create( LPDIRECT3DDEVICE8 pd3dDevice, TCHAR* strFilename );
	virtual void	Update(FLOAT fElapsedAppTime);
	virtual void	InitForNewLevel()	{};
	virtual void	GetCurrentMatrix(D3DXMATRIX*);

    virtual HRESULT RenderCageObject( LPDIRECT3DDEVICE8 pd3dDevice, BOOL bDrawOpaqueSubsets = TRUE, BOOL bDrawAlphaSubsets = TRUE )
	{
		return CD3DMesh::Render(pd3dDevice,bDrawOpaqueSubsets,bDrawAlphaSubsets);
	}


	/////////////////////////////////////////////////////
	// accessors

	D3DXVECTOR3& GetPosition()						{ return m_Position;	}
	void		SetPosition(D3DXVECTOR3 Position)	{ m_Position=Position;	}
	float		GetVelocity()						{ return m_fVelocity;	}
	void		SetVelocity(float Velocity)			{ m_fVelocity = Velocity;}

	float		GetYaw()							{ return m_fYaw;			}
	float		GetPitch()							{ return m_fPitch;		}
	float		GetRoll()							{ return m_fRoll;		}

    D3DXVECTOR3	GetObjectCenter()					{ return m_vObjectCenter;	}
	float		GetObjectRadius()					{ return m_fObjectRadius;	}


	D3DXVECTOR3& GetScaleFactor()					{ return m_vScaleFactor;	}



protected:
	
	void RecalcWorldMatrix1();
	
	CCageApp*	m_pCageApp;

	// position, speed and orientation of this object
	D3DXVECTOR3 m_Position;
	float m_fVelocity;
	float m_fYaw, m_fPitch, m_fRoll;

    D3DXVECTOR3         m_vObjectCenter;    // Center of bounding sphere of object
    float               m_fObjectRadius;    // Radius of bounding sphere of object


	D3DXVECTOR3 m_vScaleFactor;

	// stores prebuild translation to origin and scale matrix
	D3DXMATRIX matWorld1;


};

#endif // !defined(AFX_CAGEOBJECT_H__45E558EF_42E4_4C10_9AB6_94BC08BC6C46__INCLUDED_)

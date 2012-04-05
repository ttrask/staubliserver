// Donut.h: interface for the CDonut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DONUT_H__67456010_8953_4D31_9284_9BFB3D5C4F01__INCLUDED_)
#define AFX_DONUT_H__67456010_8953_4D31_9284_9BFB3D5C4F01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CageObject.h"

class CDonut : public CCageObject  
{
public:

	CDonut(CCageApp* pCageApp,  TCHAR* strName, float scaleFactor, float fRotationsPerSecond );
	virtual ~CDonut();

	/////////////////////////////////////////////////////
	// interface


	virtual	HRESULT Create( LPDIRECT3DDEVICE8 pd3dDevice, TCHAR* strFilename );	
	void Update(FLOAT fElapsedAppTime);

private:
	float m_fRotationsPerSecond;

};

#endif // !defined(AFX_DONUT_H__67456010_8953_4D31_9284_9BFB3D5C4F01__INCLUDED_)

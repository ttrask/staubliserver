// Credits.h: interface for the CCredits class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CREDITS_H__C33AB94C_1C2C_48CE_9BAB_AF4E2E5C9E7C__INCLUDED_)
#define AFX_CREDITS_H__C33AB94C_1C2C_48CE_9BAB_AF4E2E5C9E7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CageObject.h"


class CCredits : public CCageObject  
{
public:
	CCredits(CCageApp* pCageApp,  TCHAR* strName, float scaleFactor );
	virtual ~CCredits();

	/////////////////////////////////////////////////////
	// interface

	virtual void	Update(FLOAT fElapsedAppTime=0);

    HRESULT RenderCageObject( LPDIRECT3DDEVICE8 pd3dDevice, 
		            BOOL bDrawOpaqueSubsets = TRUE,
		            BOOL bDrawAlphaSubsets = TRUE );

	void	StartCreditSequence();
	bool	GetCreditSequenceOn()	{ return m_bCreditSequenceOn; }
	void	TurnOffCreditSequence(){ m_bCreditSequenceOn=false; }
	bool	IsGameOverTransitionOver()	{ return m_bGameOverTransitionOver; }

private:

	bool	m_bCreditSequenceOn;
	double	m_dElapsedSequenceTime;
	bool	m_bGameOverTransitionOver;

	D3DXVECTOR3	m_vStartPosition;
	D3DXVECTOR3	m_vEndPosition;

};

#endif // !defined(AFX_CREDITS_H__C33AB94C_1C2C_48CE_9BAB_AF4E2E5C9E7C__INCLUDED_)

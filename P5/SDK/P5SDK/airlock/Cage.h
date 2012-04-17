// Cage.h: interface for the CCage class. // 
//////////////////////////////////////////////////////////////////////
#ifndef AFX_CAGE_H__BE301F3C_4477_47E6_83BF_5AF954550561__INCLUDED_
#define AFX_CAGE_H__BE301F3C_4477_47E6_83BF_5AF954550561__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CageObject.h"
class CCage : public CCageObject  
{
public:
	CCage::CCage(CCageApp* pCageApp,  TCHAR* strName, float scaleFactor, float closedScaleFactor );
	virtual ~CCage();
	/////////////////////////////////////////////////////
	// interface
	virtual HRESULT Create( LPDIRECT3DDEVICE8 pd3dDevice, TCHAR* strFilename );	
	virtual void	Update(FLOAT fElapsedAppTime);
	
	bool			IsOpen()	{ return m_bIsOpen; }	
	bool			IsClosed()	{ return !m_bIsOpen; }	
	bool			IsOpening()	{ return m_bIsOpening; }	
	bool			IsClosing()	{ return m_bIsClosing; }
	bool			IsStarCatchable()			{ return m_bCatchableStar;	}
	void			SetIsStarCatchable(bool b)	{ m_bCatchableStar=b;		}

private:

	void		DoMovement(double fElapsedAppTime);
	void		DoAnimations(double fElapsedAppTime);	
	void		DoLighting(double fElapsedAppTime);
	void		CheckPosition();
	void		StartCloseSequence(bool bClosedByMouse);	
	void		StartOpenSequence();	
	
	D3DXVECTOR3	m_vClosedScaleFactor;
	bool		m_bIsOpen;

	// state to hold closing cage animation

	// true if cage is currently in the act of closing
	bool	m_bIsClosing;	
	// true if cage is currently in the act of opening
	bool	m_bIsOpening;	
	// true if cage was last closed by a mouse (not the P5)
	bool	m_bClosedByMouse;
	// length close(open) animation should take, in milliseconds
	FLOAT	m_fAnimationTargetLength;		
	// time current animaiton started
	FLOAT	m_fAnimationStart;
	// current scale factor (should be between m_vScaleFactor and m_vClosedScaleFactor depending
	// on where we are in the animation)
	D3DXVECTOR3	m_vCurrentScaleFactor;
	// true when at least one star is catachable (set by CStar::Update())
	bool m_bCatchableStar;
	// amount of finger bend to trigger "mouse down"
	FLOAT	m_fBendClickThreshold;
};

#endif // !defined(AFX_CAGE_H__BE301F3C_4477_47E6_83BF_5AF954550561__INCLUDED_)
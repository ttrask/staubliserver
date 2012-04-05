// CageApp.h: interface for the CCageApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAGEAPP_H__5239054F_DC82_4F29_A3E8_2852CFD642F1__INCLUDED_)
#define AFX_CAGEAPP_H__5239054F_DC82_4F29_A3E8_2852CFD642F1__INCLUDED_

#include "Credits.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "P5dll.h"
#include "d3dapp.h"
#include <dmusici.h>
#include <vector>

class CCageObject;
class CCage;
class CStarfield;
class CExplosion;
class CShadow;
class CD3DFont;
class CD3DInputWrapper;
class CP5DLL;


class CCageApp : public CD3DApplication  
{
public:
	CD3DInputWrapper* GetInput();
	CCageApp();
	virtual ~CCageApp();


	/////////////////////////////////////////////////////
	// interface

    virtual HRESULT Create( HINSTANCE hInstance );
    virtual HRESULT OneTimeSceneInit();
    virtual HRESULT InitDeviceObjects();
    virtual HRESULT RestoreDeviceObjects();
    virtual HRESULT FrameMove();
    virtual HRESULT Render();
    virtual HRESULT InvalidateDeviceObjects();
    virtual HRESULT DeleteDeviceObjects();
    virtual HRESULT FinalCleanup();


	void PlayEventSound(int index);
	void CheckCatches();
	CCage* GetCage();
	CShadow* GetShadow();
	CStarfield* GetStarfield();
	CExplosion* GetExplosion();
	CCredits* GetCredits();
	void ResetGame();
	void InitNewLevel();







	/////////////////////////////////////////////////////
	// utility

	static double Distance(D3DXVECTOR3* pPoint1,D3DXVECTOR3* pPoint2)
	{
		return sqrt(pow(pPoint1->x-pPoint2->x,2)+
					pow(pPoint1->y-pPoint2->y,2)+
					pow(pPoint1->z-pPoint2->z,2));
	}


	inline D3DVALUE
	static Magnitude (const _D3DVECTOR& v)
	{
	   return (D3DVALUE) sqrt(SquareMagnitude(v));
	}

	inline D3DVALUE
	static SquareMagnitude (const _D3DVECTOR& v)
	{
	   return v.x*v.x + v.y*v.y + v.z*v.z;
	}


	static void DisplayDebugMsg(char* msg )
	{
		OutputDebugString( "DEBUG: " );
		OutputDebugString( msg );
		OutputDebugString( "\n" );

	}

	static double GetProfileDouble(LPSTR szEntry,LPSTR szDefault="0.0")
	{
		char value[512];
		::GetPrivateProfileString("Cage",szEntry,szDefault,value,sizeof(value),".\\cage.ini");
		return atof(value);		
	}

	static int GetProfileInt(LPSTR szEntry,LPSTR szDefault="0")
	{
		char value[512];
		::GetPrivateProfileString("Cage",szEntry,szDefault,value,sizeof(value),".\\cage.ini");
		return atoi(value);		
	}


	/////////////////////////////////////////////////////
	// accessors


	double GetGloveSensitivity()				{ return m_dGloveSensitivity;	}
	double GetGloveAccelerationSensitivity()	{ return m_dGloveAccelerationSensitivity;	}
	double GetMouseSensitivity()				{ return m_dMouseSensitivity;	}
	double GetKeyboardSensitivity()				{ return m_dKeyboardSensitivity; }

	const D3DXVECTOR3* GetMaxPosition()			{ return &m_MaxPosition;}
	const D3DXVECTOR3* GetMinPosition()			{ return &m_MinPosition;}
	
	const D3DXVECTOR3* GetLookAtPosition()		{ return &m_LookAtPosition;}
	const D3DXVECTOR3* GetCameraPosition()		{ return &m_CameraPosition;}
	const D3DXMATRIX*  GetViewMatrix()			{ return &m_MatView; }
	
	const 
	const D3DXVECTOR3* GetShadowMaxPosition()	{ return &m_ShadowMaxPosition;}
	const D3DXVECTOR3* GetShadowMinPosition()	{ return &m_ShadowMinPosition;}

	double GetCageCatchRadius()					{ return m_dCageCatchRadius; }
	int		GetShadowMode()						{ return m_nShadowMode; }

	CP5DLL*	GetP5()								{ return m_pP5; }	

	int		GetNumCaught()						{ return m_nNumCaught;	}
	int		GetNumToWin()						{ return m_nNumToWin;	}
	int		GetCurrentLevel()					{ return m_nCurrentLevel; }
	bool	IsLevelOver()						{ return m_bLevelOver; }
	bool	IsGameOver()						{ return m_bGameOver; }
	
	double	GetInitStarSpeed()					{ return m_dInitStarSpeed; }
	double	GetCurrentStarSpeed()				{ return m_dInitStarSpeed + (m_dStarSpeedLevelIncrement*m_nCurrentLevel); }

	double	GetStarEntrySpeedMulitplier()		{ return m_dStarEntrySpeedMulitplier; }
	double	GetStarExitSpeedMulitplier()		{ return m_dStarExitSpeedMulitplier; }

	bool	UseGloveOrientation()				{ return m_bUseGloveOrientation&&m_pP5!=NULL; }
	


protected:

	bool AttemptToSetRequestMode();
	// enforces max and min positions for camera lookat
	void CheckLookAtPosition();
	// checks for level win and starts transition if
	void LevelCheck();



	// P5 stuff...
	CP5DLL* m_pP5;

	std::vector<CCageObject*> m_CageObjects;
	
	// number of cage objects in m_CageObjects that aren't stars.  non-stars always come first by convention
	// so the first star is always at m_CageObjects[m_NumNonStarObjects];
	int m_nNumNonStarObjects;

	// indexes of non-star objects
	static const int CCageApp::m_INDEX_CAGE ;
	static const int CCageApp::m_INDEX_SHADOW;
	static const int CCageApp::m_INDEX_DARKSTAR;
	static const int CCageApp::m_INDEX_LITESTAR;
	static const int CCageApp::m_INDEX_STARFIELD;
	static const int CCageApp::m_INDEX_EXPLOSION;
	static const int CCageApp::m_INDEX_CREDITS;

	// game state
	int	m_nNumCaught;
	int m_nNumToWin;
	int m_nCurrentLevel;		// first level is 0
	int m_nNumLevels;
	bool m_bLevelOver;		// true during transition between levels...
	bool m_bGameOver;		// true while credits are showing...
	double m_dElapsedTimeSinceLevelOver;
	double m_dLevelTransitionSeconds;
		    
	// implementation objects
    CD3DFont*           m_pFont;
	CD3DInputWrapper*	m_pInput;


	// DirectMusic stuff
	bool	m_bEnableSound;
	HRESULT	InitMusic();
	IDirectMusicLoader8*      m_pDMLoader;
	IDirectMusicPerformance8* m_pDMPerformance;
	// background score
	IDirectMusicSegment8*     m_pDMSegment;

	// Event Sounds...
	static const int m_numEventSounds;
public:
	static const int m_EVENT_MISS;
	static const int m_EVENT_CATCH;
	static const int m_EVENT_BEGIN;
	static const int m_EVENT_WIN;
	static const int m_EVENT_DEATH;
	static const int m_EVENT_THROW;
protected:
	static WCHAR* szEventFiles[];
	IDirectMusicSegment8** m_pDMEventSegments;

	// .ini file settings
	int		m_nNumStars;
	double	m_dInitStarSpeed;
	double	m_dStarSpeedLevelIncrement;
	double	m_dStarScale;
	double	m_dCageScale;
	double	m_dClosedCageScale;
	double  m_dGloveSensitivity;
	double  m_dGloveAccelerationSensitivity;
	double  m_dMouseSensitivity;
	double  m_dKeyboardSensitivity;
	char    m_szAmbientLight[512];
	double	m_dCageCatchRadius;
	double  m_dCameraSensitivity;
	int		m_nShadowMode;
	DWORD	m_dwRequestedWidth;
	DWORD	m_dwRequestedHeight;
	DWORD	m_dwRequestedBitDepth;
	bool	m_bDebug;
	double	m_dZAxisMultiplier;
	double	m_dStarEntrySpeedMulitplier;
	double	m_dStarExitSpeedMulitplier;
	bool	m_bUseGloveOrientation;



	// working start variables

	float	m_fRoomXScaleFactor;
	// world coordinates of bounding box of playing area 
	// max=upper,right,back corner (positive coords), min= lower,left,front corner (neg coords)
	D3DXVECTOR3	m_MaxPosition;
	D3DXVECTOR3	m_MinPosition;

	D3DXVECTOR3	m_ShadowMaxPosition;
	D3DXVECTOR3	m_ShadowMinPosition;

	// current camera look at position
	D3DXVECTOR3	m_MaxLookAtPosition;
	D3DXVECTOR3	m_MinLookAtPosition;
	D3DXVECTOR3	m_LookAtPosition;
	D3DXVECTOR3	m_CameraPosition;
	D3DXMATRIX	m_MatView;
	




};

#endif // !defined(AFX_CAGEAPP_H__5239054F_DC82_4F29_A3E8_2852CFD642F1__INCLUDED_)

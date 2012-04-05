//-----------------------------------------------------------------------------
// File: C3DText.h
//
// Desc: 
//
//-----------------------------------------------------------------------------
#ifndef C3DText_H
#define C3DText_H

#define MAX_LINE_LENGTH 512

#include "C3DFont.h"

//-----------------------------------------------------------------------------
// Name: class C3DText
// Desc:
//-----------------------------------------------------------------------------
class C3DText
{
public:
	int GetTextBlockHeight(C3DFont *pFont);
	void SetLineHeight(int height);
	int m_LineHeight;
	void SetColor(int red, int green, int blue, int alpha);
	C3DText( int iNumLines );
	~C3DText();

	void SetLine( int i, const char *szText );
	void SetOffset( float fX, float fY )	{ m_fX = fX; m_fY = fY; }
  HRESULT Render(C3DFont *pFont);

	C3DText *m_pNext;
	
private:
	int m_iNumLines;
	char **m_pszLines;
	DWORD *m_dwColors;
	float m_fX, m_fY;
};


#endif // C3DText_H

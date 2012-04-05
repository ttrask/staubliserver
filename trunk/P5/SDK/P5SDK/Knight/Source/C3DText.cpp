//-----------------------------------------------------------------------------
// File: C3DText.cpp
//
// Desc: A block of text lines to display
//
//
//-----------------------------------------------------------------------------

#include "C3DText.h"

#include <crtdbg.h>

#define LINE_HEIGHT	10	// ick

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
C3DText::C3DText( int iNumLines ) :
	m_pNext(NULL),
	m_pszLines(NULL),
	m_dwColors(NULL),
	m_fX(0.0f),
	m_fY(0.0f)
{
	_ASSERT( iNumLines > 0 );
	m_iNumLines = iNumLines;
	m_LineHeight = LINE_HEIGHT;
	m_pszLines = new char *[iNumLines];
	m_dwColors = new DWORD[iNumLines];
	for( int i = 0; i < iNumLines; ++i )
	{
		m_pszLines[i] = new char[MAX_LINE_LENGTH];
		m_pszLines[i][0] = 0;
		m_dwColors[i] = D3DCOLOR_ARGB(255,255,255,0);
	}

}

C3DText::~C3DText()
{
	for( int i = 0; i < m_iNumLines; ++i )
	{
		delete [] m_pszLines[i];
	}

	delete [] m_pszLines;
	m_pszLines = NULL;
	delete [] m_dwColors;
	m_dwColors = NULL;

	m_iNumLines = 0;
}

void C3DText::SetLine( int i, const char *szText )
{
	if( (i >= 0) && (i < m_iNumLines) )
		strcpy( m_pszLines[i], szText );
}

HRESULT C3DText::Render(C3DFont *pFont)
{
	_ASSERT( pFont );
	HRESULT hr;
	float fY = m_fY;
	for( int i = 0; i < m_iNumLines; ++i )
	{
		hr = pFont->DrawText( m_fX, fY, m_dwColors[i], m_pszLines[i] );
		if( FAILED( hr ) )
			return hr;
		SIZE size;
		pFont->GetTextExtent(m_pszLines[i],&size);
		fY += size.cy;
	}
	return S_OK;
}

void C3DText::SetColor(int red, int green, int blue, int alpha)
{
	for( int i = 0; i < m_iNumLines; ++i )
	{
		m_dwColors[i] = D3DCOLOR_ARGB(alpha,red,green,blue);
	}
}

void C3DText::SetLineHeight(int height)
{
	m_LineHeight = height;
}

int C3DText::GetTextBlockHeight(C3DFont *pFont)
{
	SIZE size;
	pFont->GetTextExtent("1",&size);
	return m_iNumLines * size.cy;
}

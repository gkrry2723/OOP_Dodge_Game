#pragma once

#include "d3dUtility.h"
#include "globalVar.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>


// -----------------------------------------------------------------------------
// CText class definition
// -----------------------------------------------------------------------------
class CText {

private:
	CText * TextInst;
	ID3DXFont* m_pFont;
	int m_nMax_x;
	int m_nMax_y;


public:
	CText(void)
	{

	}
	~CText(void)
	{
	}
	void Init(IDirect3DDevice9* pDevice, int height, int width, int weight, LPCSTR Face)   //초기설정, create와 비슷한 역할
	{
		D3DXFONT_DESC Font;
		ZeroMemory(&Font, sizeof(D3DXFONT_DESC));

		Font.Height = height;                            // 전체 글자 높이
		Font.Width = width;                             // 전체 글자 넓이
		Font.Weight = FW_BOLD;                     // 긁자 굵기 ( FW_BOLD 하면 굵음 )
		Font.MipLevels = D3DX_DEFAULT;
		Font.Italic = false;                         // 이텔릭
		Font.CharSet = DEFAULT_CHARSET;
		Font.OutputPrecision = OUT_DEFAULT_PRECIS;
		Font.Quality = DEFAULT_QUALITY;
		Font.PitchAndFamily = DEFAULT_PITCH;
		Font.FaceName, TEXT("맑은고딕");              // 글씨체
		m_nMax_x = 1024;                           // 윈도우 최대 X
		m_nMax_y = 768;                                // 윈도우 최대 Y

		D3DXCreateFontIndirect(pDevice, &Font, &m_pFont);
	}

	void Print(LPCSTR cSTR, int nX/*문자열의 왼쪽좌표*/, int nY/*문자열의 윗쪽좌표*/, D3DXCOLOR rgb = D3DCOLOR_XRGB(0, 0, 0)/*문자 색: 검정*/)   //그리기. print와 비슷한 역할
	{
		RECT rt = { nX, nY, m_nMax_x, m_nMax_y };

		m_pFont->DrawTextA(0, cSTR, -1, &rt, DT_TOP | DT_LEFT, rgb);
	}

	CText* GetInst(void)
	{
		if (!TextInst)
		{
			TextInst = new CText;
		}
		return TextInst;
	}

	void FreeInst(void)
	{
		if (TextInst)
		{
			m_pFont->Release();
			delete TextInst;
			TextInst = NULL;
		}
	}

};

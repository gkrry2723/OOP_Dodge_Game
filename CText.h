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
	void Init(IDirect3DDevice9* pDevice, int height, int width, int weight, LPCSTR Face)   //�ʱ⼳��, create�� ����� ����
	{
		D3DXFONT_DESC Font;
		ZeroMemory(&Font, sizeof(D3DXFONT_DESC));

		Font.Height = height;                            // ��ü ���� ����
		Font.Width = width;                             // ��ü ���� ����
		Font.Weight = FW_BOLD;                     // ���� ���� ( FW_BOLD �ϸ� ���� )
		Font.MipLevels = D3DX_DEFAULT;
		Font.Italic = false;                         // ���ڸ�
		Font.CharSet = DEFAULT_CHARSET;
		Font.OutputPrecision = OUT_DEFAULT_PRECIS;
		Font.Quality = DEFAULT_QUALITY;
		Font.PitchAndFamily = DEFAULT_PITCH;
		Font.FaceName, TEXT("�������");              // �۾�ü
		m_nMax_x = 1024;                           // ������ �ִ� X
		m_nMax_y = 768;                                // ������ �ִ� Y

		D3DXCreateFontIndirect(pDevice, &Font, &m_pFont);
	}

	void Print(LPCSTR cSTR, int nX/*���ڿ��� ������ǥ*/, int nY/*���ڿ��� ������ǥ*/, D3DXCOLOR rgb = D3DCOLOR_XRGB(0, 0, 0)/*���� ��: ����*/)   //�׸���. print�� ����� ����
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

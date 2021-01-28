#pragma once
#include "d3dUtility.h"
#include "globalVar.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>
#include "CSphere.h"

// -----------------------------------------------------------------------------
// CWall class definition
// -----------------------------------------------------------------------------

class CWall {

private:

	float               m_x;
	float               m_z;
	float                   m_width;
	float                   m_depth;
	float               m_height;

public:
	CWall(void)
	{
		D3DXMatrixIdentity(&m_mLocal);
		ZeroMemory(&m_mtrl, sizeof(m_mtrl));
		m_width = 0;
		m_depth = 0;
		m_pBoundMesh = NULL;
	}
	~CWall(void) {}
public:
	bool create(IDirect3DDevice9* pDevice, float ix, float iz, float iwidth, float iheight, float idepth, D3DXCOLOR color = d3d::WHITE)
	{
		if (NULL == pDevice)
			return false;

		m_mtrl.Ambient = color;
		m_mtrl.Diffuse = color;
		m_mtrl.Specular = color;
		m_mtrl.Emissive = d3d::BLACK;
		m_mtrl.Power = 5.0f;

		m_width = iwidth;
		m_depth = idepth;

		if (FAILED(D3DXCreateBox(pDevice, iwidth, iheight, idepth, &m_pBoundMesh, NULL)))
			return false;
		return true;
	}
	void destroy(void)
	{
		if (m_pBoundMesh != NULL) {
			m_pBoundMesh->Release();
			m_pBoundMesh = NULL;
		}
	}
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
	{
		if (NULL == pDevice)
			return;
		pDevice->SetTransform(D3DTS_WORLD, &mWorld);
		pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
		pDevice->SetMaterial(&m_mtrl);
		m_pBoundMesh->DrawSubset(0);
	}

	bool hasIntersected(CSphere& ball)
	{
		if (ball.getCenter().x - ball.getRadius() > this->m_x - (this->m_width / 2) &&
			ball.getCenter().x + ball.getRadius() < this->m_x + (this->m_width / 2) &&
			ball.getCenter().z - ball.getRadius() > this->m_z - (this->m_depth / 2) &&
			ball.getCenter().z + ball.getRadius() < this->m_z + (this->m_depth / 2))
		{
			return false;
		}

		else {
			return true;
		}
	}

	void hitBy(CSphere& ball)
	{
		if (hasIntersected(ball) == true) {
			float wallPxb, wallPxt, wallPzb, wallPzt = 0;

			wallPxb = this->m_x - (this->m_width / 2);
			wallPxt = this->m_x + (this->m_width / 2);
			wallPzb = this->m_z - (this->m_depth / 2);
			wallPzt = this->m_z + (this->m_depth / 2);

			if (ball.getCenter().x - ball.getRadius() >= wallPxb || ball.getCenter().x + ball.getRadius() <= wallPxt) {
				ball.setPower(-ball.getVelocity_X(), ball.getVelocity_Z());
			}

			if (ball.getCenter().z - ball.getRadius() > wallPzb || ball.getCenter().z + ball.getRadius() < wallPzt) {
				ball.setPower(ball.getVelocity_X(), -ball.getVelocity_Z());
			}
		}
	}

	void setPosition(float x, float y, float z)
	{
		D3DXMATRIX m;
		this->m_x = x;
		this->m_z = z;

		D3DXMatrixTranslation(&m, x, y, z);
		setLocalTransform(m);
	}

	float getHeight(void) const { return M_HEIGHT; }



private:
	void setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }

	D3DXMATRIX              m_mLocal;
	D3DMATERIAL9            m_mtrl;
	ID3DXMesh* m_pBoundMesh;
};
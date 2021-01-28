#pragma once
#include "d3dUtility.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>
#include "globalVar.h"

// -----------------------------------------------------------------------------
// CSphere class definition
// -----------------------------------------------------------------------------

class CSphere {
private:
	float               center_x, center_y, center_z;
	float                   m_radius;
	float               m_velocity_x;
	float               m_velocity_z;
	
	
public:
	CSphere(void)
	{
		D3DXMatrixIdentity(&m_mLocal);
		ZeroMemory(&m_mtrl, sizeof(m_mtrl));
		m_radius = 0;
		m_velocity_x = 0;
		m_velocity_z = 0;
		m_pSphereMesh = NULL;
	}
	~CSphere(void) {}

public:
	bool create(IDirect3DDevice9* pDevice, D3DXCOLOR color = d3d::WHITE)
	{
		if (NULL == pDevice)
			return false;

		m_mtrl.Ambient = color;
		m_mtrl.Diffuse = color;
		m_mtrl.Specular = color;
		m_mtrl.Emissive = d3d::BLACK;
		m_mtrl.Power = 5.0f;

		if (FAILED(D3DXCreateSphere(pDevice, getRadius(), 50, 50, &m_pSphereMesh, NULL)))
			return false;
		return true;
	}

	void destroy(void)
	{
		if (m_pSphereMesh != NULL) {
			m_pSphereMesh->Release();
			m_pSphereMesh = NULL;
		}
	}

	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
	{
		if (NULL == pDevice)
			return;
		pDevice->SetTransform(D3DTS_WORLD, &mWorld);
		pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
		pDevice->SetMaterial(&m_mtrl);
		m_pSphereMesh->DrawSubset(0);
	}

	bool hasIntersected(CSphere& ball)
	{
		float radiusDist = 2 * M_RADIUS;
		float xdist = this->center_x - ball.center_x;
		float ydist = this->center_z - ball.center_z;
		float realDist = xdist * xdist + ydist * ydist;

		if (radiusDist * radiusDist >= realDist) {
			
			return true;
		}
		return false;
	}

	//hit by 함수를 사용하지 않음. 아래 display에서 부딪혔는지에 대한 여부가 필요하므로 bool형식인 hasIntersected를 사용.
	/*
	void hitBy(CSphere& ball)
	{
	//float angle;
	//float distx = ball.center_x - this->center_x;
	//float distz = ball.center_z - this->center_z;

	//float abdist = abs(sqrt((double)(distx*distx-distz*distz)));
	float tempax, tempay, tempbx, tempby;
	float newax, neway, newbx, newby;

	float vax = ball.getVelocity_X();
	float vay = ball.getVelocity_Z();
	float vbx = this->getVelocity_X();
	float vby = this->getVelocity_Z();

	bool isHit = this->hasIntersected(ball);

	if (isHit == true) {
	angle = atan2(vax, vay);
	tempax = vbx * cos(angle) + vby * sin(angle);
	tempbx = vax * cos(angle) + vay * sin(angle);
	tempay = vay * cos(angle) - vax * sin(angle);
	tempby = vby * cos(angle) - vbx * sin(angle);
	newax = tempax * cos(angle) - tempay * sin(angle);
	neway = tempax * sin(angle) + tempay * cos(angle);
	newbx = tempbx * cos(angle) - tempby * sin(angle);
	newby = tempbx * sin(angle) + tempby * cos(angle);

	this->setPower(newbx, newby);
	ball.setPower(newax, neway);
	}
	}
	*/

	void ballUpdate(float timeDiff)
	{
		const float TIME_SCALE = 3.3;
		D3DXVECTOR3 cord = this->getCenter();
		double vx = abs(this->getVelocity_X());
		double vz = abs(this->getVelocity_Z());

		if (vx > 0.01 || vz > 0.01)
		{
			float tX = cord.x + TIME_SCALE * timeDiff * m_velocity_x;
			float tZ = cord.z + TIME_SCALE * timeDiff * m_velocity_z;

			//correction of position of ball
			// Please uncomment this part because this correction of ball position is necessary when a ball collides with a wall
			/*if(tX >= (4.5 - M_RADIUS))
			tX = 4.5 - M_RADIUS;
			else if(tX <=(-4.5 + M_RADIUS))
			tX = -4.5 + M_RADIUS;
			else if(tZ <= (-3 + M_RADIUS))
			tZ = -3 + M_RADIUS;
			else if(tZ >= (3 - M_RADIUS))
			tZ = 3 - M_RADIUS;
			*/

			this->setCenter(tX, cord.y, tZ);
		}
		else { this->setPower(0, 0); }
		//this->setPower(this->getVelocity_X() * DECREASE_RATE, this->getVelocity_Z() * DECREASE_RATE);
		double rate = 1 - (1 - DECREASE_RATE) * timeDiff * 400;
		if (rate < 0)
			rate = 0;
		this->setPower(getVelocity_X() * rate, getVelocity_Z() * rate);
	}

	double getVelocity_X() { return this->m_velocity_x; }
	double getVelocity_Z() { return this->m_velocity_z; }

	void setPower(double vx, double vz)
	{
		this->m_velocity_x = vx;
		this->m_velocity_z = vz;
	}

	void setCenter(float x, float y, float z)
	{
		D3DXMATRIX m;
		center_x = x;   center_y = y;   center_z = z;
		D3DXMatrixTranslation(&m, x, y, z);
		setLocalTransform(m);
	}

	float getRadius(void)  const { return (float)(M_RADIUS); }
	const D3DXMATRIX& getLocalTransform(void) const { return m_mLocal; }
	void setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }
	D3DXVECTOR3 getCenter(void) const
	{
		D3DXVECTOR3 org(center_x, center_y, center_z);
		return org;
	}

private:
	D3DXMATRIX              m_mLocal;
	D3DMATERIAL9            m_mtrl;
	ID3DXMesh* m_pSphereMesh;

};
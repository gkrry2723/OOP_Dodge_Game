////////////////////////////////////////////////////////////////////////////////
//
// File: virtualLego.cpp
//
// Original Author: 박창현 Chang-hyeon Park, 
// Modified by Bong-Soo Sohn and Dong-Jun Kim
// 
// Originally programmed for Virtual LEGO. 
// Modified later to program for Virtual Billiard.
//        
////////////////////////////////////////////////////////////////////////////////

#pragma comment(lib, "dsound.lib")
#include <Windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include "d3dUtility.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>
#include "CSphere.h"
#include "globalVar.h"
#include "CText.h"
#include "CWall.h"
#include "CLight.h"
#include "CSound.h"

IDirect3DDevice9* Device = NULL;

// window size
const int Width = 1024;
const int Height = 768;

// initialize the position (coordinate) of each ball (별모양 되게!)
const float spherePos[20][2] = { { 0, 2.0f } ,{ -0.225f, 1.3f } ,{ -0.45f, 0.6f } ,{ -1.175f, 0.6f } ,{ -1.9f, 0.6f }
,{ -1.31f, 0.12f } ,{ -0.72f, -0.24f } ,{ -0.96f, -0.92f } ,{ -1.2f, -1.6f } ,{ -0.6f, -1.175f }
,{ 0, -0.75f } ,{ 0.6f, -1.175f } ,{ 1.2f, -1.6f } ,{ 0.96f, -0.92f } ,{ 0.72f, -0.24f }
,{ 1.31f, 0.12f } ,{ 1.9f, 0.6f } ,{ 1.175f, 0.6f } ,{ 0.45f, 0.6f } ,{ 0.225f, 1.3f } };

// initialize the color of each ball
const D3DXCOLOR sphereColor[20] = { d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW,
d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW,
d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW ,
d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW };

// -----------------------------------------------------------------------------
// Transform matrices
// -----------------------------------------------------------------------------
D3DXMATRIX g_mWorld;
D3DXMATRIX g_mView;
D3DXMATRIX g_mProj;

// -----------------------------------------------------------------------------
// Global variables
// -----------------------------------------------------------------------------
CWall g_legoPlane;
CSphere   g_sphere[20];
CWall g_legowall[4];
CSphere g_target_blueball;
CLight g_light;
CText Ttimer;      //게임실행시 증가하는 시간을 보여주는 text
CText Tstart;      //시작할때 뜨는 message
CText Tcomment;
CText Tcomment2;
CText Ttimetitle;   //시간의 제목
CText Ttime;      //얼마동안 살아남았는지를 알려주는 시간
CText Tgameover;   //게임이 종료되면 GAME OVER 출력
CText Tsecond;      //second라는 단어

CSound enterS;
CSound mainS;
CSound gameoverS;
CSound spaceS;

int enterCount = 0;
double g_camera_pos[3] = { 0.0, 5.0, -8.0 };

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------

void destroyAllLegoBlock(void)
{
}

// initialization
bool Setup()
{
	int i;

	D3DXMatrixIdentity(&g_mWorld);
	D3DXMatrixIdentity(&g_mView);
	D3DXMatrixIdentity(&g_mProj);

	// create plane and set the position
	if (false == g_legoPlane.create(Device, -1, -1, 25 * M_RADIUS2, 0.03f, (19 * sqrt(3) + 2) * M_RADIUS2, d3d::BLACK)) return false;
	g_legoPlane.setPosition(0.0f, -0.0006f / 5, 0.0f);


	// create walls and set the position. note that there are four walls
	if (false == g_legowall[0].create(Device, -1, -1, 25 * M_RADIUS2 + 0.24f, 0.3f, 0.12f, d3d::DARKRED)) return false;
	g_legowall[0].setPosition(0.0f, 0.12f, (9.5f * sqrt(3) + 1.0f) * M_RADIUS2 + 0.06f);   //위쪽 벽
	if (false == g_legowall[1].create(Device, -1, -1, 25 * M_RADIUS2 + 0.24f, 0.3f, 0.12f, d3d::DARKRED)) return false;
	g_legowall[1].setPosition(0.0f, 0.12f, (-9.5f * sqrt(3) - 1.0f) * M_RADIUS2 - 0.06f);   //아래쪽 벽
	if (false == g_legowall[2].create(Device, -1, -1, 0.12f, 0.3f, (19 * sqrt(3) + 2) * M_RADIUS2, d3d::DARKRED)) return false;
	g_legowall[2].setPosition(12.5f * M_RADIUS2 + 0.06f, 0.12f, 0.0f);   //오른쪽 벽
	if (false == g_legowall[3].create(Device, -1, -1, 0.12f, 0.3f, (19 * sqrt(3) + 2) * M_RADIUS2, d3d::DARKRED)) return false;
	g_legowall[3].setPosition(-12.5 * M_RADIUS2 - 0.06f, 0.12f, 0.0f);   //왼쪽 벽

	//create balls																	 // create balls and set the position
	for (i = 0; i < 20; i++) {
		if (false == g_sphere[i].create(Device, sphereColor[i])) return false;
		g_sphere[i].setCenter(spherePos[i][0], (float)M_RADIUS, spherePos[i][1]);
		g_sphere[i].setPower(0, 0);
	}

	// create blue ball for set direction
	if (false == g_target_blueball.create(Device, d3d::BLUE)) return false;
	g_target_blueball.setCenter(.0f, (float)M_RADIUS, .0f);

	//text setting
	Tgameover.Init(Device, 60, 30, 700, "Arial");
	Ttime.Init(Device, 27, 10, 700, "Arial");
	Tsecond.Init(Device, 27, 10, 700, "Arial");
	Ttimetitle.Init(Device, 27, 10, 700, "Arial");
	Ttimer.Init(Device, 27, 10, 700, "Arial");
	Tstart.Init(Device, 33, 15, 700, "Arial");
	Tcomment.Init(Device, 27, 10, 700, "Arial");
	Tcomment2.Init(Device, 25, 10, 700, "Arial");

	//sound setting
	enterS.CreateDirectSound(hwnd);
	enterS.LoadWave("PressEnter.wav");

	mainS.CreateDirectSound(hwnd);
	mainS.LoadWave("main.wav");

	gameoverS.CreateDirectSound(hwnd);
	gameoverS.LoadWave("gameover.wav");

	spaceS.CreateDirectSound(hwnd);
	spaceS.LoadWave("gamestart.wav");

	// light setting 
	D3DLIGHT9 lit;
	::ZeroMemory(&lit, sizeof(lit));
	lit.Type = D3DLIGHT_POINT;
	lit.Diffuse = d3d::WHITE;
	lit.Specular = d3d::WHITE * 0.9f;
	lit.Ambient = d3d::WHITE * 0.9f;
	lit.Position = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	lit.Range = 100.0f;
	lit.Attenuation0 = 0.0f;
	lit.Attenuation1 = 0.9f;
	lit.Attenuation2 = 0.0f;
	if (false == g_light.create(Device, lit))
		return false;

	// Position and aim the camera(게임특성상 위에서 거의 수직적으로 볼 수있게 코드를 고침)
	D3DXVECTOR3 pos(0.0f, 10.0f, -1.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 2.0f, 0.0f);
	D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &g_mView);

	// Set the projection matrix.
	D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 4,
		(float)Width / (float)Height, 1.0f, 100.0f);
	Device->SetTransform(D3DTS_PROJECTION, &g_mProj);

	// Set render states.
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

	g_light.setLight(Device, g_mWorld);
	return true;
}

void Cleanup(void)
{
	g_legoPlane.destroy();
	for (int i = 0; i < 4; i++) {
		g_legowall[i].destroy();
	}
	destroyAllLegoBlock();
	g_light.destroy();
}


// timeDelta represents the time between the current image frame and the last image frame.
// the distance of moving balls should be "velocity * timeDelta"
bool Display(float timeDelta)
{
	int i = 0;
	int j = 0;
	char TtimerArr[100];
	float tokenx, tokeny, tokenz = 0.0;
	char TstartArr[100] = "Press SpaceBar to start!";
	char TcommentArr[100] = "Press direction key to move blue ball!";
	char Tcomment2Arr[100] = "move blueball to avoid yellow ball!";
	char TgameoverArr[100] = "Game Over";
	char TtitleArr[100] = "Your score:";
	char TsencondArr[100] = "seconds";
	char TtimeArr[100];

	if (Device)
	{

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00afafaf, 1.0f, 0);
		Device->BeginScene();

		// draw plane, walls, and spheres
		g_legoPlane.draw(Device, g_mWorld);
		for (i = 0; i < 4; i++) {
			g_legowall[i].draw(Device, g_mWorld);
		}

		for (i = 0; i < 20; i++) {
			g_sphere[i].draw(Device, g_mWorld);
		}

		g_target_blueball.draw(Device, g_mWorld);

		g_light.draw(Device);


		//시작할려면 spacebar누르라는 text 처음에만 넣어줌
		if (pressSpace == false) {

			if (enterCount == 0) {
				Tcomment2.Print(Tcomment2Arr, 350, 600, d3d::WHITE);
			}
			else if (enterCount == 1) {
				Tcomment.Print(TcommentArr, 350, 600, d3d::WHITE);
			}
			else if (enterCount == 2) {
				Tstart.Print(TstartArr, 330, 100, d3d::WHITE);
			}
		}

		// update the position of each ball. during update, check whether each ball hit by walls.
		for (i = 0; i < 20; i++) {
			g_sphere[i].ballUpdate(timeDelta);
		}

		for (i = 0; i < 4; i++) {
			for (j = 0; j < 20; j++) {
				g_legowall[i].hitBy(g_sphere[j]);
			}
		}

		for (i = 0; i < 20; i++) {
			if (g_sphere[i].getCenter().x >((25.0 * M_RADIUS2 / 2.0) - M_RADIUS + 0.5) || g_sphere[i].getCenter().x < (-(25.0 * M_RADIUS2 / 2.0) + M_RADIUS - 0.5) ||
				g_sphere[i].getCenter().z >(((19.0 * sqrt(3) + 2.0) * M_RADIUS2 / 2.0) + M_RADIUS + 0.5) || g_sphere[i].getCenter().z < (-((19.0 * sqrt(3) + 2.0) * M_RADIUS2 / 2.0) - M_RADIUS) - 0.5) {
				tokenx = g_sphere[i].getCenter().x;
				tokeny = (float)M_RADIUS;
				tokenz = g_sphere[i].getCenter().x;
				if (tokenx > 0 && tokenz > 0) {   //1사분면에서 나갔으면
					g_sphere[i].setCenter(((25.0 * M_RADIUS2 / 2.0) - M_RADIUS) - 1.0, tokeny, (((19.0 * sqrt(3) + 2.0) * M_RADIUS2 / 2.0) + M_RADIUS) - 1.0);
				}
				else if (tokenx < 0 && tokenz > 0) {   //제2사분면에서 나갔으면
					g_sphere[i].setCenter(-((25.0 * M_RADIUS2 / 2.0) + M_RADIUS) + 1.0, tokeny, (((19.0 * sqrt(3) + 2.0) * M_RADIUS2 / 2.0) + M_RADIUS) - 1.0);
				}
				else if (tokenx < 0 && tokenz < 0) {   //제3사분면에서 나갔으면
					g_sphere[i].setCenter(-((25.0 * M_RADIUS2 / 2.0) + M_RADIUS) + 1.0, tokeny, (-((19.0 * sqrt(3) + 2.0) * M_RADIUS2 / 2.0) - M_RADIUS) + 1.0);
				}
				else if (tokenx > 0 && tokenz < 0) {   //제4사분면에서 나갔으면
					g_sphere[i].setCenter(((25.0 * M_RADIUS2 / 2.0) - M_RADIUS) - 1.0, tokeny, (-((19.0 * sqrt(3) + 2.0) * M_RADIUS2 / 2.0) - M_RADIUS) + 1.0);
				}

				tokenx = g_sphere->getVelocity_X();
				tokenz = g_sphere->getVelocity_Z();
				g_sphere[i].setPower(-tokenx, -tokenz);
			}
		}

		//play 중일땐 흐르는 시간을 계속 업데이트를 해서 얼마나 시간이 흘럿는지를 표시함
		if (isPlaying == true) {
			realend = clock();
			itoa((realend - begin) / CLOCKS_PER_SEC, TtimerArr, 10);
			Ttimer.Print(TtimerArr, 100, 150, d3d::BLACK);
		}

		if (isPlaying == false) {
			mainS.Stop();
		}

		// 파란공이 노란공과 만낫나 계속 확인하고 만나면 기록과 함께 게임 종료 창을 띄움
		for (i = 0; i < 20; i++) {
			if (g_sphere[i].hasIntersected(g_target_blueball)) {   //공끼리 부딪히면
					if (isPlaying == true) {
						gameoverS.Play(FALSE);
						end = clock();
						isPlaying = false;
					}
					Tgameover.Print(TgameoverArr, 350, 300, d3d::WHITE);   //게임종료창을 띄우고
					for (int j = 0; j < 20; j++) {                        //모든공들을 멈춤
						g_sphere[j].setPower(0.0, 0.0);
					}

					//기록 출력
					Ttimetitle.Print(TtitleArr, 380, 400, d3d::WHITE);
					itoa((end - begin) / CLOCKS_PER_SEC, TtimeArr, 10);
					Ttime.Print(TtimeArr, 518, 400, d3d::WHITE);
					Tsecond.Print(TsencondArr, 550, 400, d3d::WHITE);
			}
		}
		
		Device->EndScene();
		Device->Present(0, 0, 0, 0);
		Device->SetTexture(0, NULL);
	}
	return true;
}

LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool wire = false;
	static bool isReset = true;
	static int old_x = 0;
	static int old_y = 0;
	static enum { WORLD_MOVE, LIGHT_MOVE, BLOCK_MOVE } move = WORLD_MOVE;

	bool gameIsEnd = false;
	for (int t = 0; t < 20; t++) {
		if (g_target_blueball.hasIntersected(g_sphere[t]) == true) {
			gameIsEnd = true;
			break;
		}
	}

	switch (msg) {
	case WM_DESTROY:
	{
		::PostQuitMessage(0);
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wParam) {
		case VK_ESCAPE:
			::DestroyWindow(hwnd);
			break;
		case VK_RETURN:
			enterCount++;
			enterS.Play(FALSE);

			break;
		case VK_SPACE:
			pressSpace = true;
			if (isPlaying == false) {
				isPlaying = true;
				begin = clock();
				mainS.Play(FALSE);
				spaceS.Play(FALSE);
				
			}

			srand((unsigned int)time(NULL));
			for (int t = 0; t < 20; t++) {
				if (g_sphere[t].getCenter().x <= 0 && g_sphere[t].getCenter().z > 0) {//1사분면
					g_sphere[t].setPower(((float)rand() / (RAND_MAX + 1) * (1.0 - 0.5) + 0.5), ((float)rand() / (RAND_MAX + 1) * (1.0 - 0.5) + 0.5));
				}
				else if (g_sphere[t].getCenter().x <= 0 && g_sphere[t].getCenter().z > 0) {//2사분면
					g_sphere[t].setPower(-((float)rand() / (RAND_MAX + 1) * (1.0 - 0.5) + 0.5), ((float)rand() / (RAND_MAX + 1) * (1.0 - 0.5) + 0.5));
				}
				else if (g_sphere[t].getCenter().x <= 0 && g_sphere[t].getCenter().z <= 0) {//3사분면
					g_sphere[t].setPower(-((float)rand() / (RAND_MAX + 1) * (1.0 - 0.5) + 0.5), -(float)rand() / (RAND_MAX + 1) * (1.0 - 0.5) + 0.5);
				}
				else {//4사분면
					g_sphere[t].setPower(((float)rand() / (RAND_MAX + 1) * (1.0 - 0.5) + 0.5), -(float)rand() / (RAND_MAX + 1) * (1.0 - 0.5) + 0.5);
				}
			}

			if (pressSpace == true) {
				break;
			}

			break;
		case VK_RIGHT:
			if (g_target_blueball.getCenter().x >= ((25.0 * M_RADIUS2 / 2.0) - M_RADIUS)) {
				break;
			}// 오른쪽 벽을 벗어나려 하면 안먹힘
			else if (gameIsEnd) {
				break;
			}
			else {
				g_target_blueball.setCenter(g_target_blueball.getCenter().x + 0.08, g_target_blueball.getCenter().y, g_target_blueball.getCenter().z);
			}
			break;
		case VK_LEFT:
			if (g_target_blueball.getCenter().x <= (-(25.0 * M_RADIUS2 / 2.0) + M_RADIUS)) {
				break;
			}//왼쪽 벽을 벗어나려 하면 안먹힘
			else if (gameIsEnd) {
				break;
			}
			else {
				g_target_blueball.setCenter(g_target_blueball.getCenter().x - 0.08, g_target_blueball.getCenter().y, g_target_blueball.getCenter().z);
			}
			break;

		case VK_UP:
			if (g_target_blueball.getCenter().z >= (((19.0 * sqrt(3)) * M_RADIUS2 / 2.0) + M_RADIUS)) {
				break;
			}//위쪽 벽을 벗어나려 하면 안먹힘
			else if (gameIsEnd) {
				break;
			}
			else {
				g_target_blueball.setCenter(g_target_blueball.getCenter().x, g_target_blueball.getCenter().y, g_target_blueball.getCenter().z + 0.08);
			}
			break;

		case VK_DOWN:
			if (g_target_blueball.getCenter().z <= (-((19.0 * sqrt(3)) * M_RADIUS2 / 2.0) - M_RADIUS)) {
				break;
			}//아래쪽 벽을 벗어나려 하면 안먹힘
			else if (gameIsEnd) {
				break;
			}
			else {
				g_target_blueball.setCenter(g_target_blueball.getCenter().x, g_target_blueball.getCenter().y, g_target_blueball.getCenter().z - 0.08);
			}
			break;

		}
		break;
	}
	}

	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	srand(static_cast<unsigned int>(time(NULL)));

	if (!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device, &hwnd))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	Device->Release();

	return 0;
}
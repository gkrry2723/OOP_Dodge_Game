#pragma once
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

#define M_RADIUS 0.10   // ball radius
#define M_RADIUS2 0.21   // 게임틀 사이즈 지정을 위한 고정값
#define PI 3.14159265
#define M_HEIGHT 0.01
#define DECREASE_RATE 1 //속도 저하가 필요하지 않음

//CSound 관련 define문
#ifndef DSBCAPS_CTRLDEFAULT  //6.0이후 부터 없어졌으므로 define해줌
#define DSBCAPS_CTRLDEFAULT  (DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME)
#endif
#define DSVOLUME_TO_DB(volume) ((DWORD)(-30*(100-volume)))

//입력값을 dB(decibel)로 바꿔주는 매크로 함수
#define DSVOLUME_TO_DB(volume)   ((DWORD)(-30*(100-volume))) 

HWND hwnd = 0;

clock_t begin, end;
clock_t realend;

bool isPlaying = false;
bool pressSpace = false;




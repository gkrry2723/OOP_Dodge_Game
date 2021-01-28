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
#define M_RADIUS2 0.21   // ����Ʋ ������ ������ ���� ������
#define PI 3.14159265
#define M_HEIGHT 0.01
#define DECREASE_RATE 1 //�ӵ� ���ϰ� �ʿ����� ����

//CSound ���� define��
#ifndef DSBCAPS_CTRLDEFAULT  //6.0���� ���� ���������Ƿ� define����
#define DSBCAPS_CTRLDEFAULT  (DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME)
#endif
#define DSVOLUME_TO_DB(volume) ((DWORD)(-30*(100-volume)))

//�Է°��� dB(decibel)�� �ٲ��ִ� ��ũ�� �Լ�
#define DSVOLUME_TO_DB(volume)   ((DWORD)(-30*(100-volume))) 

HWND hwnd = 0;

clock_t begin, end;
clock_t realend;

bool isPlaying = false;
bool pressSpace = false;




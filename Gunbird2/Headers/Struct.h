#pragma once

#ifndef __STRUCT_H__
#define __STRUCT_H__

typedef struct tagInfo
{
	float	fX;
	float	fY;
	int		iCX;
	int		iCY;
}INFO;

typedef struct tagFrame
{
	int		iFrameCnt;
	int		iFrameStart;
	int		iFrameEnd;
	int		iFrameScene;
	DWORD	dwFrameTime;
	DWORD	dwFrameSpeed;
}FRAME;

typedef struct tagDamage
{
	int iEffectCnt;
	DWORD dwTime;
	DWORD dwDelay;
	bool bDamage;
}DAMAGE;

typedef struct tagBullet {
	bool flag;
	int count;
}BULLET;

typedef struct tagDelay {
	DWORD	dwOnTime;	// 시작 시간
	DWORD	dwOnDelay;	// 시작 딜레이
	DWORD	dwOffTime;	// 종료 시간
	DWORD	dwOffDelay;	// 종료 딜레이
	DWORD	dwCnt;		// 횟수
	bool	bOnFlag;	// 플래그
	bool	bOffFlag;	// 플래그
	bool	bAttack;	// 공격
}DELAY;

// 오브젝트 배치 정보 구조체
typedef struct tagCreateObjectInfo
{
	int iAppearance;		// 배치 타이밍, 스크롤 Y 좌표 기준의 값이다
	DWORD dwOutTime;		// 배치되고 퇴장하기까지의 시간
	float fPosX;			// 배치되기전 화면 바깥의 좌표 X
	float fPosY;			// 배치되기전 화면 바깥의 좌표 Y
	float fDestPosX;		// 바깥에서 화면 안으로 배치될 좌표 X
	float fDestPosY;		// 바깥에서 화면 안으로 배치될 좌표 Y
	float fOutPosX;			// 화면 밖으로 퇴장할 좌표 X
	float fOutPosY;			// 화면 밖으로 퇴장할 좌표 Y
	float fDestAngle;		// 배치 될 때 선회 각도
	float fOutAngle;		// 퇴장 할 때 선회 각도
	int iWidth;				// 가로 길이
	int iHeight;			// 세로 길이
	OBJTYPE::TYPE enType;	// 오브젝트 타입
	bool bFlag;				// 이미 생성했는가 여부
}CREATE_OBJECT;

#endif // !__STRUCT_H__

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
	DWORD	dwOnTime;	// ���� �ð�
	DWORD	dwOnDelay;	// ���� ������
	DWORD	dwOffTime;	// ���� �ð�
	DWORD	dwOffDelay;	// ���� ������
	DWORD	dwCnt;		// Ƚ��
	bool	bOnFlag;	// �÷���
	bool	bOffFlag;	// �÷���
	bool	bAttack;	// ����
}DELAY;

// ������Ʈ ��ġ ���� ����ü
typedef struct tagCreateObjectInfo
{
	int iAppearance;		// ��ġ Ÿ�̹�, ��ũ�� Y ��ǥ ������ ���̴�
	DWORD dwOutTime;		// ��ġ�ǰ� �����ϱ������ �ð�
	float fPosX;			// ��ġ�Ǳ��� ȭ�� �ٱ��� ��ǥ X
	float fPosY;			// ��ġ�Ǳ��� ȭ�� �ٱ��� ��ǥ Y
	float fDestPosX;		// �ٱ����� ȭ�� ������ ��ġ�� ��ǥ X
	float fDestPosY;		// �ٱ����� ȭ�� ������ ��ġ�� ��ǥ Y
	float fOutPosX;			// ȭ�� ������ ������ ��ǥ X
	float fOutPosY;			// ȭ�� ������ ������ ��ǥ Y
	float fDestAngle;		// ��ġ �� �� ��ȸ ����
	float fOutAngle;		// ���� �� �� ��ȸ ����
	int iWidth;				// ���� ����
	int iHeight;			// ���� ����
	OBJTYPE::TYPE enType;	// ������Ʈ Ÿ��
	bool bFlag;				// �̹� �����ߴ°� ����
}CREATE_OBJECT;

#endif // !__STRUCT_H__

// 기명준
// 마리온 유도 미사일
// 각도에 따라 이미지 인덱스가 달라짐
// 날아가면서 주기적으로 별 이펙트 생성

#pragma once

#ifndef __MARIONGUIDEDBULLET_H__
#define __MARIONGUIDEDBULLET_H__

#include "Bullet.h"
class CMarionGuidedBullet :
	public CBullet
{
private:
	enum ANGLE {
		A90, A70, A45, A20,
		A0, A_20, A_45, A_70,
		A_90, A_110, A_135, A_160,
		A180, A160, A135, A110,
		END
	};

public:
	CMarionGuidedBullet();
	virtual ~CMarionGuidedBullet();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	void Init_Angle();	// 각도 초기화

	void Set_BulletCnt(BULLET * _stpSubBulletCnt) { m_stpSubBulletCnt = _stpSubBulletCnt; }

private:
	DWORD m_dwEffectCreateTime;		// 이펙트 생성 시간
	DWORD m_dwEffectCreateDelay;	// 이펙트 생성 딜레이
	BULLET * m_stpSubBulletCnt;		// 플레이어 서브 미사일 개수 구조체 포인터(화면에 생성할 수있는 미사일 개수가 제한되어있음)
};

#endif // !__MARIONGUIDEDBULLET_H__



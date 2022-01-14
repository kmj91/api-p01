// 기명준
// 적 총알이 명중하거나 폭탄에 의해 제거되면 생성되는 이펙트
// 총알 종류에따라 다른 이펙트 출력

#pragma once

#ifndef __BULLETDESTROY_H__
#define __BULLETDESTROY_H__

#include "Effect.h"
class CBulletDestroy :
	public CEffect
{
public:
	CBulletDestroy();
	virtual ~CBulletDestroy();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	// 총알 종류에 따라 이미지 프레임 초기화
	void Set_BulletType(BULLETTYPE _eBullet);

public:
	// 총알 종류
	BULLETTYPE m_eBulletType;
};

#endif // !__BULLETDESTROY_H__



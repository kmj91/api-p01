#pragma once

#ifndef __MARION_H__
#define __MARION_H__

#include "Player.h"
class CMarion final :
	public CPlayer
{
public:
	CMarion();
	virtual ~CMarion();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update();
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	// 총알 파워 업
	virtual void PowerUp() override;
	// 총알 파워 다운
	virtual void PowerDown() override;
	// 키 입력
	// _Key : 입력된 키 enum 값
	virtual void Key_Input(int _Key);

private:
	void Scene_Change();
	void Move_Change();
	void Shot();
	void Create_Effect();			// 별 이펙트 생성

	template <typename T>
	CObj* Create_Bullet(float _fX, float _fY, float _fAngle)
	{
		return CAbstractFactory<T>::Create(_fX, _fY, _fAngle);
	}

private:
	DWORD		m_dwEffectCreateTime;	// 이펙트 생성 시간
	DWORD		m_dwEffectCreateDelay;	// 이펙트 생성 딜레이
	bool		m_bBombEffect;			// 폭탄 이펙트용 변수 - 번갈아가면서 위치 변경
	bool		m_bMeleeHit;			// 근접 공격 적중 확인용
	BULLET		m_tSubBulletCnt;		// 서브 유도 미사일용 구조체
};

#endif // !__MARION_H__


#pragma once
#include "Obj.h"

#ifndef __BULLET_H__
#define __BULLET_H__

class CBullet :
	public CObj
{
public:
	CBullet();
	virtual ~CBullet();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	float Get_Damage() { return m_fDamage; }
	void Set_Damage(float _fDamage) { m_fDamage = _fDamage; }
	void Set_BulletDelay(DWORD _dwDelay) { m_dwBulletDelay = _dwDelay; m_bOnBullet = false; }
	int Get_PlayerNum() { return m_iPlayerNum; }
	void Set_PlayerNum(int _iPlayerNum) { m_iPlayerNum = _iPlayerNum; }


	// 총알을 삭제하고 사탕 생성
	void Change_Candy();

protected:
	DWORD m_dwBulletTime;	// 객체 생성 시간
	DWORD m_dwBulletDelay;	// 딜레이
	float m_fDamage;		// 총알 데미지
	int m_iPlayerNum;		// 플레이어 번호	(1P or 2P) - 플레이어 총알만 사용
	bool m_bOnBullet;		// 총알 제어 플래그 - 기본 true, Set_BulletDelay() 함수 호출시 false, Bullet_3만 사용하는 중
};

#endif // !__BULLET_H__



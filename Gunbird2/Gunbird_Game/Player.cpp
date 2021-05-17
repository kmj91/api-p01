#include "stdafx.h"
#include "Player.h"


CPlayer::CPlayer()
	: m_ePreState(PLAYER::STATE_END), m_eCurState(PLAYER::STATE_END),
	m_dwKeyTime(0), m_dwKeyDelay(400), m_dwBulletTime(0), m_dwBulletDelay(100), m_dwRespawnTime(GetTickCount()), m_dwRespawnDelay(400),
	m_dwHpLockTime(GetTickCount()), m_dwHpLockDelay(3000), m_dwChargeTime(0), m_dwChargeDelay(350), m_dwChargeBulletTime(0), m_dwChargeBulletDelay(80),
	m_dwPowerDownTime(0), m_dwPowerDownDelay(1000),
	m_iPlayerNum(0), m_iShotPower(0), m_iShotCnt(0), m_iChargeBulletCnt(0),
	m_fAngelY(0.f), m_fMaxRotAngle(45.f), m_fRotSpeed(3.f), m_bRespawn(false), m_bOnShot(false), m_bImmutable(false)
{
}


CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
}

int CPlayer::Update()
{
	return 0;
}

void CPlayer::Late_Update()
{
}

void CPlayer::Render(HDC _DC)
{
}

void CPlayer::Release()
{
}
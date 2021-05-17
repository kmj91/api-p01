#include "stdafx.h"
#include "Marion.h"

#include "KeyMgr.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"
#include "GameUIMgr.h"

#include "MarionBullet_1.h"
#include "MarionBullet_2.h"
#include "MarionBullet_3.h"
#include "MarionBullet_4.h"
#include "MarionGuidedBullet.h"
#include "MarionChargeBullet.h"
#include "PlayerDead.h"
#include "MarionEffect.h"
#include "MarionBombEffect.h"
#include "Effect_MarionMeleeHit.h"
#include "UI_PowerUpDownText.h"
#include "PowerUp.h"
#include "Bomb.h"

CMarion::CMarion() 
	: m_dwEffectCreateTime(0), m_dwEffectCreateDelay(100), m_bBombEffect(false), m_bMeleeHit(false)
	, m_tSubBulletCnt{}
{
}


CMarion::~CMarion()
{
	Release();
}

void CMarion::Initialize()
{
	m_tInfo.fX = 200;
	m_tInfo.fY = WINCY + (MARION_MOVE_HEIGHT * 3 >> 1);
	m_tInfo.iCX = MARION_MOVE_WIDTH * 3;
	m_tInfo.iCY = MARION_MOVE_HEIGHT * 3;
	m_tHitRectPos = { 9 * 3, 8 * 3, 15 * 3, 19 * 3 };
	m_iImageWidth = MARION_MOVE_WIDTH;
	m_iImageHeight = MARION_MOVE_HEIGHT;

	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 100;

	m_fSpeed = 8.f;

	m_ePreState = PLAYER::RESPAWN;
	m_eCurState = PLAYER::RESPAWN;
	m_pFrameKey = L"Marion_Move";
	
	m_tMeleeRect = { 0,0,0,0 };

	//m_iShotPower = 3;
	m_dwChargeBulletDelay = 150;
}

int CMarion::Update()
{
	// 죽음
	if (m_bDead)
	{
		if (m_ePreState != PLAYER::DEAD)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_DEAD);
			CSoundMgr::Get_Instance()->PlaySound(L"Marion_Dead_01.wav", CSoundMgr::PLAYER_DEAD);

			m_eCurState = PLAYER::DEAD;
			m_bHpLock = true;
			m_iShotCnt = 0;
			CObj* pObj = CAbstractFactory<CPlayerDead>::Create(m_tInfo.fX, m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		}
	}
	// 스테이지 클리어
	else if (g_bStageClear) {
		// 화면 밖으로 이동
		m_tInfo.fY = m_tInfo.fY - m_fSpeed;
	}

	// 리스폰 처리
	if (m_bRespawn)
	{
		if (m_dwHpLockTime + m_dwHpLockDelay < GetTickCount())
		{
			m_bRespawn = false;
			m_bHpLock = false;
		}
	}

	switch (m_ePreState)
	{
	case PLAYER::IDLE:
	case PLAYER::LEFT_1:
	case PLAYER::LEFT_2:
	case PLAYER::RIGHT_1:
	case PLAYER::RIGHT_2:
		break;
	case PLAYER::CHARGE_START:
		if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		{
			m_eCurState = PLAYER::CHARGE_REPEAT;

			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SHOT);
			CSoundMgr::Get_Instance()->PlaySound(L"Marion_Charge.wav", CSoundMgr::PLAYER_SHOT);
		}
		break;
	case PLAYER::CHARGE_REPEAT:
		if (!m_bOnShot)
		{
			// 충전샷 레벨
			int iChargeLevel = CGameUIMgr::Get_Instance()->Get_ChargeLevel(m_iPlayerNum);
			// 1레벨 기본
			if (iChargeLevel == 1) {
				m_iChargeBulletCnt = 1;
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_CHARGE);
				CSoundMgr::Get_Instance()->PlaySound(L"Marion_ChargeLV1.wav", CSoundMgr::PLAYER_CHARGE);
			}
			// 2레벨 助けて
			else if (iChargeLevel == 2) {
				m_iChargeBulletCnt = 3;
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_CHARGE);
				CSoundMgr::Get_Instance()->PlaySound(L"Marion_ChargeLV2.wav", CSoundMgr::PLAYER_CHARGE);
			}
			// 3레벨 ファイアレビット
			else if (iChargeLevel == 3) {
				m_iChargeBulletCnt = 8;
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_CHARGE);
				CSoundMgr::Get_Instance()->PlaySound(L"Marion_ChargeLV3.wav", CSoundMgr::PLAYER_CHARGE);
			}

			// 충전량 소모
			CGameUIMgr::Get_Instance()->Add_ChargeLevel(-iChargeLevel, m_iPlayerNum);

			m_eCurState = PLAYER::CHARGE_END;
		}
		break;
	case PLAYER::CHARGE_END:
		// 충전 공격
		if (0 < m_iChargeBulletCnt && m_dwChargeBulletTime + m_dwChargeBulletDelay < GetTickCount())
		{
			m_dwChargeBulletTime = GetTickCount();
			--m_iChargeBulletCnt;

			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_CHARGE_BULLET);
			CSoundMgr::Get_Instance()->PlaySound(L"Marion_ChargeBullet.wav", CSoundMgr::PLAYER_CHARGE_BULLET);

			CObj* pObj = CAbstractFactory<CMarionChargeBullet>::Create(m_tInfo.fX, m_tInfo.fY - 30.f, 90.f);
			static_cast<CBullet*>(pObj)->Set_PlayerNum(m_iPlayerNum);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER_BULLET);
		}

		// 다 쏨
		if(m_iChargeBulletCnt == 0)
		{
			m_eCurState = PLAYER::IDLE;
			m_bImmutable = false;

			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;

			m_tInfo.iCX = MARION_MOVE_WIDTH * 3;
			m_tInfo.iCY = MARION_MOVE_HEIGHT * 3;
			m_iImageWidth = MARION_MOVE_WIDTH;
			m_iImageHeight = MARION_MOVE_HEIGHT;

			// 위치 변경
			m_tInfo.fY = m_tInfo.fY + (3.f * 3.f);
			// 충돌 렉트 위치값 변경
			m_tHitRectPos = { 9 * 3, 8 * 3, 15 * 3, 19 * 3 };
		}

		break;
	case PLAYER::BOMB:
		// 몬스터 총알 제거
		CObjMgr::Get_Instance()->Dead_MonsterBullet();
		// 몬스터에게 지속적인 데미지
		CObjMgr::Get_Instance()->Demage_Monster(1.f, m_iPlayerNum);

		// 폭탄 사용 종료
		if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		{
			m_eCurState = PLAYER::IDLE;
			m_bHpLock = false;
			m_bImmutable = false;

			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;

			m_tInfo.iCX = MARION_MOVE_WIDTH * 3;
			m_tInfo.iCY = MARION_MOVE_HEIGHT * 3;
			m_iImageWidth = MARION_MOVE_WIDTH;
			m_iImageHeight = MARION_MOVE_HEIGHT;

			// 충돌 렉트 위치값 변경
			m_tHitRectPos = { 9 * 3, 8 * 3, 15 * 3, 19 * 3 };

			// 이펙트 딜레이 변경
			m_dwEffectCreateDelay = 100.f;
		}
			
		break;
	case PLAYER::MELEE:
		// 근접 공격 시작 프레임
		if (7 <= m_tFrame.iFrameCnt && m_tFrame.iFrameCnt <= 23) {
			LONG lCX = LONG(m_tInfo.iCX >> 1);
			LONG lCY = LONG(m_tInfo.iCY >> 1);
			LONG lX = (LONG)m_tInfo.fX - lCX;
			LONG lY = (LONG)m_tInfo.fY - lCY;

			// 근접 렉트
			m_tMeleeRect.left = lX;
			m_tMeleeRect.top = lY;
			m_tMeleeRect.right = lX + (27 * 3);
			m_tMeleeRect.bottom = lY + (42 * 3);

			// 몬스터에게 지속적인 데미지
			bool bHit = CObjMgr::Get_Instance()->Demage_Monster(&m_tMeleeRect, 20.f, m_iPlayerNum);

			// 근접 공격 이펙트 생성
			if (!m_bMeleeHit && bHit) {
				m_bMeleeHit = true;

				CObj* pObj = CAbstractFactory<CEffect_MarionMeleeHit>::Create(m_tInfo.fX - (9.f * 3.f), m_tInfo.fY - (26.f * 3.f));
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			}
		}

		// 근접 공격 끝
		if (m_tFrame.iFrameCnt == 24) {
			// 근접 공격 렉트
			m_tMeleeRect = { 0,0,0,0 };
		}

		// 근접 공격 종료
		if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		{
			m_eCurState = PLAYER::IDLE;
			m_bHpLock = false;
			m_bImmutable = false;

			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;

			m_tInfo.iCX = MARION_MOVE_WIDTH * 3;
			m_tInfo.iCY = MARION_MOVE_HEIGHT * 3;
			m_iImageWidth = MARION_MOVE_WIDTH;
			m_iImageHeight = MARION_MOVE_HEIGHT;

			// 위치 변경
			m_tInfo.fX = m_tInfo.fX - (11.f * 3.f);
			m_tInfo.fY = m_tInfo.fY + (32.f * 3.f);
			// 충돌 렉트 위치값 변경
			m_tHitRectPos = { 9 * 3, 8 * 3, 15 * 3, 19 * 3 };
		}
		break;
	case PLAYER::DEAD:
		if (m_tRect.top < WINCY)		// 화면 바깥까지 떨어짐
		{
			m_tInfo.fY += 15.f;
		}
		else
		{
			if (CGameUIMgr::Get_Instance()->Get_Life(m_iPlayerNum) <= 0)
			{
				// 목숨이 없음
				m_eCurState = PLAYER::CONTINUE;		// 컨티뉴
				m_bImmutable = true;				// 스프라이트 고정
				m_bDead = false;
				CGameUIMgr::Get_Instance()->On_ContinueCountdown(m_iPlayerNum);
			}
			else
			{
				CGameUIMgr::Get_Instance()->Add_Life(-1, m_iPlayerNum);	// 라이프 감소

				m_eCurState = PLAYER::RESPAWN;		// 살아남
				m_bDead = false;
				m_dwRespawnTime = GetTickCount();
			}
		}
		break;
	case PLAYER::RESPAWN:
		if (m_dwRespawnTime + m_dwRespawnDelay > GetTickCount())
		{
			m_tInfo.fY -= m_fSpeed;
		}
		else
		{
			m_eCurState = PLAYER::IDLE;
			m_bRespawn = true;
			m_bImmutable = false;
			m_dwHpLockTime = GetTickCount();

			// 충돌 렉트 위치값 변경
			m_tHitRectPos = { 9 * 3, 8 * 3, 15 * 3, 19 * 3 };
		}
		break;
	case PLAYER::CONTINUE:
		// 라이프가 0 보다 크면
		if (CGameUIMgr::Get_Instance()->Get_Life(m_iPlayerNum) > 0) {
			m_eCurState = PLAYER::RESPAWN;		// 살아남
			m_dwRespawnTime = GetTickCount();
		}
		break;
	}

	// 공격
	if (0 < m_iShotCnt && m_dwBulletTime + m_dwBulletDelay < GetTickCount())
	{
		m_dwBulletTime = GetTickCount();
		--m_iShotCnt;
		Shot();
	}

	// 별 이펙트 생성
	Create_Effect();
	Scene_Change();
	Update_Rect();
	Frame_Move();

	return OBJ_NOEVENT;
}

void CMarion::Late_Update()
{
}

void CMarion::Render(HDC _DC)
{
	HDC hMemDC;

	switch (m_ePreState)
	{
	case PLAYER::IDLE:
	case PLAYER::LEFT_1:
	case PLAYER::LEFT_2:
	case PLAYER::RIGHT_1:
	case PLAYER::RIGHT_2:
		// 리스폰 중 깜빡임 처리
		if (m_bRespawn)
		{
			if (rand() % 2)
			{
				break;
			}
		}

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Marion_Move");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
		break;
	case PLAYER::CHARGE_START:
	case PLAYER::CHARGE_REPEAT:
	case PLAYER::CHARGE_END:
		// 리스폰 중 깜빡임 처리
		if (m_bRespawn)
		{
			if (rand() % 2)
			{
				break;
			}
		}

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Marion_Charge");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
		break;
	case PLAYER::BOMB:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Marion_Bomb");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
		break;
	case PLAYER::MELEE:
		// 리스폰 중 깜빡임 처리
		if (m_bRespawn)
		{
			if (rand() % 2)
			{
				break;
			}
		}

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Marion_Melee");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
		break;
	case PLAYER::DEAD:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Marion_Dead");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, 0, 0
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
		break;
	case PLAYER::RESPAWN:
		if (rand() % 2)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Marion_Move");
			GdiTransparentBlt(_DC
				, m_tRect.left, m_tRect.top
				, m_tInfo.iCX, m_tInfo.iCY
				, hMemDC
				, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
				, m_iImageWidth, m_iImageHeight
				, RGB(255, 0, 255));
		}
		break;
	case PLAYER::CONTINUE:
		break;
	case PLAYER::STATE_END:
		break;
	default:
		break;
	}

	// 충돌 박스
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
	Rectangle(_DC, m_tMeleeRect.left, m_tMeleeRect.top, m_tMeleeRect.right, m_tMeleeRect.bottom);
}

void CMarion::Release()
{
}

// 총알 파워 업
void CMarion::PowerUp()
{
	if (m_iShotPower < 3)
	{
		++m_iShotPower;
		if (m_iShotPower == 1)
		{
			// 기본 100, 400
			m_dwBulletDelay = 80;
			m_dwKeyDelay = 300;
		}
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_POWER_UP);
		CSoundMgr::Get_Instance()->PlaySound(L"Marion_PowerUp.wav", CSoundMgr::PLAYER_POWER_UP);

		return;
	}

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_POWER_UP);
	CSoundMgr::Get_Instance()->PlaySound(L"Marion_FullPower.wav", CSoundMgr::PLAYER_POWER_UP);
}

// 총알 파워 다운
void CMarion::PowerDown()
{
	if (0 < m_iShotPower)
	{
		// 파워 다운 딜레이
		if (m_dwPowerDownTime + m_dwPowerDownDelay > GetTickCount())
		{
			return;
		}

		--m_iShotPower;
		if (m_iShotPower == 0) {
			m_dwBulletDelay = 100;
			m_dwKeyDelay = 400;
		}

		// 파워 다운 텍스트 출력
		CObj* pObj = CAbstractFactory<CUI_PowerUpDownText>::Create(m_tInfo.fX, m_tInfo.fY);
		pObj->Set_SceneID(1);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::UI);

		m_dwPowerDownTime = GetTickCount();
	}
}

void CMarion::Key_Input(int _Key)
{
	if (m_ePreState == PLAYER::RESPAWN ||
		m_ePreState == PLAYER::DEAD ||
		m_ePreState == PLAYER::BOMB ||
		g_bStageClear)		// 키 조작 불가
		return;

	switch (_Key)
	{
		// 이동키를 입력하지 않을 경우
	case KEY::NO_KEY:
		if (m_fAngelY > 0)
		{
			if (m_fAngelY - m_fRotSpeed > 0)
				m_fAngelY -= m_fRotSpeed;
			else
				m_fAngelY = 0.f;
		}
		else
		{
			if(m_fAngelY + m_fRotSpeed < 0)
				m_fAngelY += m_fRotSpeed;
			else
				m_fAngelY = 0.f;
		}
		break;
		// 이동 관련 LL, LU, LD...
	case KEY::LL:
		if (m_tHitRect.left - m_fSpeed > 0)
			m_tInfo.fX -= m_fSpeed;

		if (m_fAngelY - m_fRotSpeed > -m_fMaxRotAngle)
			m_fAngelY -= m_fRotSpeed;
		break;
	case KEY::LU:
		if (m_tHitRect.left - m_fSpeed / sqrtf(2.f) > 0)
			m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
		if (m_tHitRect.top - m_fSpeed / sqrtf(2.f) > 100)
			m_tInfo.fY -= m_fSpeed / sqrtf(2.f);

		if (m_fAngelY - m_fRotSpeed > -m_fMaxRotAngle)
			m_fAngelY -= m_fRotSpeed;
		break;
	case KEY::LD:
		if (m_tHitRect.left - m_fSpeed / sqrtf(2.f) > 0)
			m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
		if (m_tHitRect.bottom + m_fSpeed / sqrtf(2.f) < WINCY - 100)
			m_tInfo.fY += m_fSpeed / sqrtf(2.f);
		
		if (m_fAngelY - m_fRotSpeed > -m_fMaxRotAngle)
			m_fAngelY -= m_fRotSpeed;
		break;
	case KEY::RR:
		if (m_tHitRect.right + m_fSpeed < WINCX)
			m_tInfo.fX += m_fSpeed;
		
		if (m_fAngelY + m_fRotSpeed < m_fMaxRotAngle)
			m_fAngelY += m_fRotSpeed;
		break;
	case KEY::RU:
		if (m_tHitRect.right + m_fSpeed / sqrtf(2.f) < WINCX)
			m_tInfo.fX += m_fSpeed / sqrtf(2.f);
		if (m_tHitRect.top - m_fSpeed / sqrtf(2.f) > 100)
			m_tInfo.fY -= m_fSpeed / sqrtf(2.f);
		
		if (m_fAngelY + m_fRotSpeed < m_fMaxRotAngle)
			m_fAngelY += m_fRotSpeed;
		break;
	case KEY::RD:
		if (m_tHitRect.right + m_fSpeed / sqrtf(2.f) < WINCX)
			m_tInfo.fX += m_fSpeed / sqrtf(2.f);
		if (m_tHitRect.bottom + m_fSpeed / sqrtf(2.f) < WINCY - 100)
			m_tInfo.fY += m_fSpeed / sqrtf(2.f);
		
		if (m_fAngelY + m_fRotSpeed < m_fMaxRotAngle)
			m_fAngelY += m_fRotSpeed;
		break;
	case KEY::UU:
		if (m_tHitRect.top - m_fSpeed > 100)
			m_tInfo.fY -= m_fSpeed;
		
		if (m_fAngelY > 0)
		{
			if (m_fAngelY - m_fRotSpeed > 0)
				m_fAngelY -= m_fRotSpeed;
			else
				m_fAngelY = 0.f;
		}
		else
		{
			if (m_fAngelY + m_fRotSpeed < 0)
				m_fAngelY += m_fRotSpeed;
			else
				m_fAngelY = 0.f;
		}
		break;
	case KEY::DD:
		if (m_tHitRect.bottom + m_fSpeed < WINCY - 100)
			m_tInfo.fY += m_fSpeed;
		
		if (m_fAngelY > 0)
		{
			if (m_fAngelY - m_fRotSpeed > 0)
				m_fAngelY -= m_fRotSpeed;
			else
				m_fAngelY = 0.f;
		}
		else
		{
			if (m_fAngelY + m_fRotSpeed < 0)
				m_fAngelY += m_fRotSpeed;
			else
				m_fAngelY = 0.f;
		}
		break;
		// 공격
	case KEY::ATK:
		if (m_dwKeyTime + m_dwKeyDelay < GetTickCount())
		{
			switch (m_ePreState)
			{
			case PLAYER::IDLE:
			case PLAYER::LEFT_1:
			case PLAYER::LEFT_2:
			case PLAYER::RIGHT_1:
			case PLAYER::RIGHT_2:
			case PLAYER::CHARGE_END:
				break;
			case PLAYER::CHARGE_START:
			case PLAYER::CHARGE_REPEAT:
			case PLAYER::DEAD:
			case PLAYER::RESPAWN:
				return;
			}

			if (!m_bOnShot)
			{
				m_bOnShot = true;
				m_dwChargeTime = GetTickCount();
			}

			if (m_iShotCnt <= 0)
			{
				m_iShotCnt = 3;
				m_dwBulletTime = GetTickCount();
				m_dwKeyTime = GetTickCount();
			}

			// 공격버튼을 계속 누르고있었으면
			if (m_dwChargeTime + m_dwChargeDelay < GetTickCount())
			{
				// 충전 공격
				if (m_ePreState != PLAYER::CHARGE_START &&
					1 <= CGameUIMgr::Get_Instance()->Get_ChargeLevel(m_iPlayerNum))
				{
					m_eCurState = PLAYER::CHARGE_START;
					m_bImmutable = true;
					m_iShotCnt = 0;
				}
			}
		}
		break;
		// 공격키에서 땜
		// 세미 오토 형식이라 이런 키값이 필요함
	case KEY::ATK_END:
		m_bOnShot = false;
		break;
		// 폭탄
	case KEY::BOMB:
		// 남은 폭탄이 없으면
		if (CGameUIMgr::Get_Instance()->Get_Bomb(m_iPlayerNum) == 0) {
			break;
		}

		// 폭탄 사용 개수 감소
		CGameUIMgr::Get_Instance()->Add_Bomb(-1, m_iPlayerNum);

		// 폭탄을 사용한 처음에만
		// 화면에 보이는 몬스터 총알 사탕으로 교체
		CObjMgr::Get_Instance()->Change_Candy();

		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_BOMB);
		CSoundMgr::Get_Instance()->PlaySound(L"Marion_Bomb.wav", CSoundMgr::PLAYER_BOMB);

		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_BOMB_EFFECT);
		CSoundMgr::Get_Instance()->PlaySound(L"Marion_BombEffect.wav", CSoundMgr::PLAYER_BOMB_EFFECT);

		m_eCurState = PLAYER::BOMB;
		m_bHpLock = true;
		m_bImmutable = true;
		break;
		// 근접 공격
	case KEY::MELEE:
		// 이미 근접 공격 중이면
		if (m_ePreState == PLAYER::MELEE) {
			break;
		}

		// 충전량이 없으면
		if (CGameUIMgr::Get_Instance()->Get_ChargeLevel(m_iPlayerNum) < 1) {
			break;
		}
		
		// 춘정량 소모
		CGameUIMgr::Get_Instance()->Add_ChargeLevel(-1, m_iPlayerNum);

		m_eCurState = PLAYER::MELEE;
		m_bImmutable = true;
		m_bMeleeHit = false;

		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_MELEE);
		CSoundMgr::Get_Instance()->PlaySound(L"Marion_Melee.wav", CSoundMgr::PLAYER_MELEE);
		break;
		// 컨티뉴
	case KEY::CONTINUE:
		if (m_ePreState == PLAYER::CONTINUE) {
			// 크레딧이 있으면
			if (0 < CGameUIMgr::Get_Instance()->Get_Credit())
			{
				// 크레딧 감소
				CGameUIMgr::Get_Instance()->Add_Credit(-1);
				// 크레딧 저장
				g_iCredit = CGameUIMgr::Get_Instance()->Get_Credit();
				// 목숨 증가
				CGameUIMgr::Get_Instance()->Add_Life(2, m_iPlayerNum);
				// 컨티뉴 정지
				CGameUIMgr::Get_Instance()->Off_ContinueCountdown(m_iPlayerNum);
				// 선택 사운드
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_DEAD);
				CSoundMgr::Get_Instance()->PlaySound(L"Marion_Start.wav", CSoundMgr::PLAYER_DEAD);
			}
		}
		break;
	}


	if (!m_bImmutable)
		Move_Change();
}

void CMarion::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case PLAYER::IDLE:
			m_tFrame.iFrameScene = 0;
			break;
		case PLAYER::LEFT_1:
			m_tFrame.iFrameScene = 1;
			break;
		case PLAYER::LEFT_2:
			m_tFrame.iFrameScene = 2;
			break;
		case PLAYER::RIGHT_1:
			m_tFrame.iFrameScene = 3;
			break;
		case PLAYER::RIGHT_2:
			m_tFrame.iFrameScene = 4;
			break;
			// 충전 공격 시작
			// 기본 상태랑 캐릭터 위치가 달라서 위치 보정 해줘야함
			// HitRect 위치도 변경해줘야 함
		case PLAYER::CHARGE_START:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 40;

			m_tInfo.iCX = MARION_CHARGE_WIDTH * 3;
			m_tInfo.iCY = MARION_CHARGE_HEIGHT * 3;
			m_iImageWidth = MARION_CHARGE_WIDTH;
			m_iImageHeight = MARION_CHARGE_HEIGHT;

			// 좌표 변경
			m_tInfo.fY = m_tInfo.fY - (3.f * 3.f);
			// 충돌 렉트 위치값 변경
			m_tHitRectPos = { 17 * 3, 23 * 3, 23 * 3, 34 * 3 };
			break;
		case PLAYER::CHARGE_REPEAT:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 40;
			break;
		case PLAYER::CHARGE_END:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 13;
			m_tFrame.iFrameScene = 2;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 40;
			break;
		case PLAYER::BOMB:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 31;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 70;

			m_tInfo.iCX = MARION_BOMB_WIDTH * 3;
			m_tInfo.iCY = MARION_BOMB_HEIGHT * 3;
			m_iImageWidth = MARION_BOMB_WIDTH;
			m_iImageHeight = MARION_BOMB_HEIGHT;

			// 이펙트 딜레이 변경
			m_dwEffectCreateDelay = 300.f;

			// 충돌 렉트 위치값 변경
			m_tHitRectPos = { 0, 0, 0, 0 };

			// 근접 공격 중이었으면 원상태로 복구
			if (m_ePreState == PLAYER::MELEE) {
				// 위치 변경
				m_tInfo.fX = m_tInfo.fX - (11.f * 3.f);
				m_tInfo.fY = m_tInfo.fY + (32.f * 3.f);
				// 근접 공격 렉트
				m_tMeleeRect = { 0,0,0,0 };
			}
			// 충전 공격 중이었으면 원상태로 복구
			else if (m_ePreState == PLAYER::CHARGE_START ||
					m_ePreState == PLAYER::CHARGE_REPEAT ||
					m_ePreState == PLAYER::CHARGE_END) {
				// 위치 변경
				m_tInfo.fY = m_tInfo.fY + (3.f * 3.f);
			}
			break;
			// 근접 공격
			// 기본 상태랑 캐릭터 위치가 달라서 위치 보정 해줘야함
			// HitRect 위치도 변경해줘야 함
		case PLAYER::MELEE:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 26;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 30;

			m_tInfo.iCX = MARION_MELEE_WIDTH * 3;
			m_tInfo.iCY = MARION_MELEE_HEIGHT * 3;
			m_iImageWidth = MARION_MELEE_WIDTH;
			m_iImageHeight = MARION_MELEE_HEIGHT;

			// 좌표 변경
			m_tInfo.fX = m_tInfo.fX + (11.f * 3.f);
			m_tInfo.fY = m_tInfo.fY - (32.f * 3.f);
			// 충돌 렉트 위치값 변경
			m_tHitRectPos = { 8 * 3, 73 * 3, 14 * 3, 84 * 3 };
			break;
		case PLAYER::DEAD:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;

			m_tInfo.iCX = MARION_MOVE_WIDTH * 3;
			m_tInfo.iCY = MARION_MOVE_HEIGHT * 3;
			m_iImageWidth = MARION_MOVE_WIDTH;
			m_iImageHeight = MARION_MOVE_HEIGHT;

			// 근접 공격 중이었으면 원상태로 복구
			if (m_ePreState == PLAYER::MELEE) {
				// 위치 변경
				m_tInfo.fX = m_tInfo.fX - (11.f * 3.f);
				m_tInfo.fY = m_tInfo.fY + (32.f * 3.f);
				// 근접 공격 렉트
				m_tMeleeRect = { 0,0,0,0 };
			}
			// 충전 공격 중이었으면 원상태로 복구
			else if (m_ePreState == PLAYER::CHARGE_START ||
				m_ePreState == PLAYER::CHARGE_REPEAT ||
				m_ePreState == PLAYER::CHARGE_END) {
				// 위치 변경
				m_tInfo.fY = m_tInfo.fY + (3.f * 3.f);
			}

			// 목숨이 없음
			// 파워업 2, 폭탄 1개 드랍
			if (CGameUIMgr::Get_Instance()->Get_Life(m_iPlayerNum) == 0) {
				// 파워업 아이템 드랍
				CObj* pObj = CAbstractFactory<CPowerUp>::Create(m_tInfo.fX, m_tInfo.fY);
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::ITEM);
				pObj = CAbstractFactory<CPowerUp>::Create(m_tInfo.fX, m_tInfo.fY);
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::ITEM);
				// 폭탄 아이템 드랍
				pObj = CAbstractFactory<CBomb>::Create(m_tInfo.fX, m_tInfo.fY);
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::ITEM);
			}
			// 파워 3레벨
			// 파워업 2개 드랍
			else if (m_iShotPower == 3) {
				// 파워업 아이템 드랍
				CObj* pObj = CAbstractFactory<CPowerUp>::Create(m_tInfo.fX, m_tInfo.fY);
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::ITEM);
				pObj = CAbstractFactory<CPowerUp>::Create(m_tInfo.fX, m_tInfo.fY);
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::ITEM);
			}
			else {
				// 파워업 아이템 드랍
				CObj* pObj = CAbstractFactory<CPowerUp>::Create(m_tInfo.fX, m_tInfo.fY);
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::ITEM);
			}
			// 충돌 렉트 위치값 변경
			m_tHitRectPos = { 0, 0, 0, 0 };
			// 충전 게이지 초기화
			CGameUIMgr::Get_Instance()->Set_ChargeLevel(1, m_iPlayerNum);
			// 폭탄 개수 초기화
			CGameUIMgr::Get_Instance()->Set_Bomb(2, m_iPlayerNum);
			// 총알 파워 0
			m_iShotPower = 0;
			m_dwBulletDelay = 100;
			m_dwKeyDelay = 400;
			break;
		case PLAYER::RESPAWN:
			m_tFrame.iFrameScene = 0;
			break;
		case PLAYER::CONTINUE:
			break;
		case PLAYER::STATE_END:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CMarion::Move_Change()
{
	if (m_fAngelY == 0.f)
		m_eCurState = PLAYER::IDLE;
	else if (m_fAngelY > -35.f && m_fAngelY < 0.f)
		m_eCurState = PLAYER::LEFT_1;
	else if (m_fAngelY > -m_fMaxRotAngle && m_fAngelY <= -35.f)
		m_eCurState = PLAYER::LEFT_2;
	else if (m_fAngelY < 35.f && m_fAngelY > 0.f)
		m_eCurState = PLAYER::RIGHT_1;
	else if (m_fAngelY < m_fMaxRotAngle && m_fAngelY >= 35.f)
		m_eCurState = PLAYER::RIGHT_2;
}

void CMarion::Shot()
{
	CObj* pObj;

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SHOT);
	CSoundMgr::Get_Instance()->PlaySound(L"Marion_Shot.wav", CSoundMgr::PLAYER_SHOT);

	switch (m_iShotPower)
	{
	case 0:
		// 기본 공격
		pObj = Create_Bullet<CMarionBullet_1>(m_tInfo.fX, m_tInfo.fY - 30.f, 90.f);
		pObj->Set_DrawID(2 - m_iShotCnt);
		static_cast<CBullet*>(pObj)->Set_PlayerNum(m_iPlayerNum);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER_BULLET);
		break;
	case 1:
		// 1단
		pObj = Create_Bullet<CMarionBullet_2>(m_tInfo.fX, m_tInfo.fY - 30.f, 90.f);
		pObj->Set_DrawID(2 - m_iShotCnt);
		static_cast<CBullet*>(pObj)->Set_PlayerNum(m_iPlayerNum);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER_BULLET);
		break;
	case 2:
		// 2단
		pObj = Create_Bullet<CMarionBullet_3>(m_tInfo.fX - 10.f, m_tInfo.fY - 20.f, 98.f );
		pObj->Set_DrawID(2 - m_iShotCnt);
		pObj->Set_SceneID(0);
		static_cast<CBullet*>(pObj)->Set_PlayerNum(m_iPlayerNum);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER_BULLET);
		pObj = Create_Bullet<CMarionBullet_3>(m_tInfo.fX, m_tInfo.fY - 30.f, 90.f);
		pObj->Set_DrawID(2 - m_iShotCnt);
		pObj->Set_SceneID(1);
		static_cast<CBullet*>(pObj)->Set_PlayerNum(m_iPlayerNum);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER_BULLET);
		pObj = Create_Bullet<CMarionBullet_3>(m_tInfo.fX + 10.f, m_tInfo.fY - 20.f, 82.f);
		pObj->Set_DrawID(2 - m_iShotCnt);
		pObj->Set_SceneID(2);
		static_cast<CBullet*>(pObj)->Set_PlayerNum(m_iPlayerNum);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER_BULLET);

		if (m_tSubBulletCnt.flag == false)
		{
			m_tSubBulletCnt.flag = true;
			m_tSubBulletCnt.count = 2;

			pObj = Create_Bullet<CMarionGuidedBullet>(m_tInfo.fX - 40.f, m_tInfo.fY - 20.f, 130.f);
			pObj->Set_Target(CObjMgr::Get_Instance()->Get_TargetMonster(pObj));
			static_cast<CMarionGuidedBullet*>(pObj)->Init_Angle();
			static_cast<CMarionGuidedBullet*>(pObj)->Set_BulletCnt(&m_tSubBulletCnt);
			static_cast<CBullet*>(pObj)->Set_PlayerNum(m_iPlayerNum);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER_BULLET);

			pObj = Create_Bullet<CMarionGuidedBullet>(m_tInfo.fX + 40.f, m_tInfo.fY - 20.f, 50.f);
			pObj->Set_Target(CObjMgr::Get_Instance()->Get_TargetMonster(pObj));
			static_cast<CMarionGuidedBullet*>(pObj)->Init_Angle();
			static_cast<CMarionGuidedBullet*>(pObj)->Set_BulletCnt(&m_tSubBulletCnt);
			static_cast<CBullet*>(pObj)->Set_PlayerNum(m_iPlayerNum);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER_BULLET);

			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_GUIDEDSHOT);
			CSoundMgr::Get_Instance()->PlaySound(L"Marion_GuidedShot.wav", CSoundMgr::PLAYER_GUIDEDSHOT);
		}
		break;
	case 3:
		// 3단
		pObj = Create_Bullet<CMarionBullet_4>(m_tInfo.fX - 20.f, m_tInfo.fY - 40.f, 100.f);
		pObj->Set_DrawID(2 - m_iShotCnt);
		pObj->Set_SceneID(0);
		static_cast<CBullet*>(pObj)->Set_PlayerNum(m_iPlayerNum);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER_BULLET);
		pObj = Create_Bullet<CMarionBullet_4>(m_tInfo.fX, m_tInfo.fY - 60.f, 90.f);
		pObj->Set_DrawID(2 - m_iShotCnt);
		pObj->Set_SceneID(1);
		static_cast<CBullet*>(pObj)->Set_PlayerNum(m_iPlayerNum);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER_BULLET);
		pObj = Create_Bullet<CMarionBullet_4>(m_tInfo.fX + 20.f, m_tInfo.fY - 40.f, 80.f);
		pObj->Set_DrawID(2 - m_iShotCnt);
		pObj->Set_SceneID(2);
		static_cast<CBullet*>(pObj)->Set_PlayerNum(m_iPlayerNum);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER_BULLET);

		if (m_tSubBulletCnt.flag == false)
		{
			m_tSubBulletCnt.flag = true;
			m_tSubBulletCnt.count = 6;

			pObj = Create_Bullet<CMarionGuidedBullet>(m_tInfo.fX - 20.f, m_tInfo.fY - 40.f, 100.f);
			pObj->Set_Target(CObjMgr::Get_Instance()->Get_TargetMonster(pObj));
			static_cast<CMarionGuidedBullet*>(pObj)->Init_Angle();
			static_cast<CMarionGuidedBullet*>(pObj)->Set_BulletCnt(&m_tSubBulletCnt);
			static_cast<CBullet*>(pObj)->Set_PlayerNum(m_iPlayerNum);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER_BULLET);

			pObj = Create_Bullet<CMarionGuidedBullet>(m_tInfo.fX + 20.f, m_tInfo.fY - 40.f, 80.f);
			pObj->Set_Target(CObjMgr::Get_Instance()->Get_TargetMonster(pObj));
			static_cast<CMarionGuidedBullet*>(pObj)->Init_Angle();
			static_cast<CMarionGuidedBullet*>(pObj)->Set_BulletCnt(&m_tSubBulletCnt);
			static_cast<CBullet*>(pObj)->Set_PlayerNum(m_iPlayerNum);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER_BULLET);



			pObj = Create_Bullet<CMarionGuidedBullet>(m_tInfo.fX - 50.f, m_tInfo.fY - 10.f, 130.f);
			pObj->Set_Target(CObjMgr::Get_Instance()->Get_TargetMonster(pObj));
			static_cast<CMarionGuidedBullet*>(pObj)->Init_Angle();
			static_cast<CMarionGuidedBullet*>(pObj)->Set_BulletCnt(&m_tSubBulletCnt);
			static_cast<CBullet*>(pObj)->Set_PlayerNum(m_iPlayerNum);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER_BULLET);

			pObj = Create_Bullet<CMarionGuidedBullet>(m_tInfo.fX + 50.f, m_tInfo.fY - 10.f, 50.f);
			pObj->Set_Target(CObjMgr::Get_Instance()->Get_TargetMonster(pObj));
			static_cast<CMarionGuidedBullet*>(pObj)->Init_Angle();
			static_cast<CMarionGuidedBullet*>(pObj)->Set_BulletCnt(&m_tSubBulletCnt);
			static_cast<CBullet*>(pObj)->Set_PlayerNum(m_iPlayerNum);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER_BULLET);



			pObj = Create_Bullet<CMarionGuidedBullet>(m_tInfo.fX - 60.f, m_tInfo.fY + 20.f, 160.f);
			pObj->Set_Target(CObjMgr::Get_Instance()->Get_TargetMonster(pObj));
			static_cast<CMarionGuidedBullet*>(pObj)->Init_Angle();
			static_cast<CMarionGuidedBullet*>(pObj)->Set_BulletCnt(&m_tSubBulletCnt);
			static_cast<CBullet*>(pObj)->Set_PlayerNum(m_iPlayerNum);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER_BULLET);

			pObj = Create_Bullet<CMarionGuidedBullet>(m_tInfo.fX + 60.f, m_tInfo.fY + 20.f, 20.f);
			pObj->Set_Target(CObjMgr::Get_Instance()->Get_TargetMonster(pObj));
			static_cast<CMarionGuidedBullet*>(pObj)->Init_Angle();
			static_cast<CMarionGuidedBullet*>(pObj)->Set_BulletCnt(&m_tSubBulletCnt);
			static_cast<CBullet*>(pObj)->Set_PlayerNum(m_iPlayerNum);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER_BULLET);

			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_GUIDEDSHOT);
			CSoundMgr::Get_Instance()->PlaySound(L"Marion_GuidedShot.wav", CSoundMgr::PLAYER_GUIDEDSHOT);
		}
		break;
	}

	
}

// 별 이펙트 생성
void CMarion::Create_Effect()
{
	CObj* pObj = nullptr;

	if (m_dwEffectCreateTime + m_dwEffectCreateDelay < GetTickCount())
	{
		m_dwEffectCreateTime = GetTickCount();

		switch (m_ePreState)
		{
		case PLAYER::IDLE:
		case PLAYER::LEFT_1:
		case PLAYER::LEFT_2:
		case PLAYER::RIGHT_1:
		case PLAYER::RIGHT_2:
			pObj = CAbstractFactory<CMarionEffect>::Create(m_tInfo.fX, m_tRect.bottom);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			break;
		case PLAYER::CHARGE_START:
		case PLAYER::CHARGE_REPEAT:
		case PLAYER::CHARGE_END:
			pObj = CAbstractFactory<CMarionEffect>::Create(m_tInfo.fX, m_tRect.bottom - (9.f * 3.f));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			break;
		case PLAYER::BOMB:
			// 번갈아가면서 생성
			if (m_bBombEffect) {
				m_bBombEffect = !m_bBombEffect;

				pObj = CAbstractFactory<CMarionBombEffect>::Create(m_tInfo.fX, m_tInfo.fY);
				pObj->Set_Angle(-90.f);
				static_cast<CMarionBombEffect*>(pObj)->Set_DestPos(m_tInfo.fX + 500, m_tInfo.fY - 200);
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

				pObj = CAbstractFactory<CMarionBombEffect>::Create(m_tInfo.fX, m_tInfo.fY);
				pObj->Set_Angle(-90.f);
				static_cast<CMarionBombEffect*>(pObj)->Set_DestPos(m_tInfo.fX - 500, m_tInfo.fY - 200);
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

				pObj = CAbstractFactory<CMarionBombEffect>::Create(m_tInfo.fX, m_tInfo.fY);
				pObj->Set_Angle(-90.f);
				static_cast<CMarionBombEffect*>(pObj)->Set_DestPos(m_tInfo.fX, m_tInfo.fY + 500);
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			}
			else {
				m_bBombEffect = !m_bBombEffect;

				pObj = CAbstractFactory<CMarionBombEffect>::Create(m_tInfo.fX, m_tInfo.fY);
				pObj->Set_Angle(-90.f);
				static_cast<CMarionBombEffect*>(pObj)->Set_DestPos(m_tInfo.fX + 500, m_tInfo.fY + 200);
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

				pObj = CAbstractFactory<CMarionBombEffect>::Create(m_tInfo.fX, m_tInfo.fY);
				pObj->Set_Angle(-90.f);
				static_cast<CMarionBombEffect*>(pObj)->Set_DestPos(m_tInfo.fX - 500, m_tInfo.fY + 200);
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

				pObj = CAbstractFactory<CMarionBombEffect>::Create(m_tInfo.fX, m_tInfo.fY);
				pObj->Set_Angle(-90.f);
				static_cast<CMarionBombEffect*>(pObj)->Set_DestPos(m_tInfo.fX, m_tInfo.fY - 500);
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			}
			break;
		case PLAYER::MELEE:
			pObj = CAbstractFactory<CMarionEffect>::Create(m_tRect.left + (11.f * 3.f), m_tRect.bottom - (9.f * 3.f));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			break;
		case PLAYER::DEAD:
		case PLAYER::RESPAWN:
		case PLAYER::CONTINUE:
			break;
		}
	}
}

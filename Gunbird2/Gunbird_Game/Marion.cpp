// 기명준
// 플레이어 마리온
// 몇몇 애니메이션 프레임의 이미지 길이가 달라서
// 상태가 변경된 후 위치 좌표를 초기화할 때 주의해야 함

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
	// 기본 위치 및 설정
	m_tInfo.fX = 200;
	m_tInfo.fY = WINCY + (MARION_MOVE_HEIGHT * 3 >> 1);
	m_tInfo.iCX = MARION_MOVE_WIDTH * 3;
	m_tInfo.iCY = MARION_MOVE_HEIGHT * 3;
	m_tHitRectPos = { 9 * 3, 8 * 3, 15 * 3, 19 * 3 };
	m_iImageWidth = MARION_MOVE_WIDTH;
	m_iImageHeight = MARION_MOVE_HEIGHT;
	
	// 기본 애니메이션 프레임 설정
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 100;

	// 이동 속도
	m_fSpeed = 8.f;

	// 기본 상태 - 리스폰
	m_ePreState = PLAYER::RESPAWN;
	m_eCurState = PLAYER::RESPAWN;
	// 프레임 키 - 이동
	m_pFrameKey = L"Marion_Move";
	// 근접 공격 렉트
	m_tMeleeRect = { 0,0,0,0 };

	//m_iShotPower = 3;
	// 충전 공격의 딜레이
	m_dwChargeBulletDelay = 150;
}

// 업데이트
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

	// 현재 상태에 따라서 처리할 것
	switch (m_ePreState)
	{
		// 기본 대기, 이동 상태는 따로 처리할 것이 없음
	case PLAYER::IDLE:
	case PLAYER::LEFT_1:
	case PLAYER::LEFT_2:
	case PLAYER::RIGHT_1:
	case PLAYER::RIGHT_2:
		break;
		// 충전 공격 시작
	case PLAYER::CHARGE_START:
		// 충전 시작 프레임 애니메이션이 끝에 도달했으면 반복 상태로 변경
		if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		{
			// 충전 반복 상태로 변경
			m_eCurState = PLAYER::CHARGE_REPEAT;
			// 충전 완료 사운드
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SHOT);
			CSoundMgr::Get_Instance()->PlaySound(L"Marion_Charge.wav", CSoundMgr::PLAYER_SHOT);
		}
		break;
		// 충전 공격 반복 상태 (공격키를 떼면 충전 공격)
	case PLAYER::CHARGE_REPEAT:
		// m_bOnShot가 false면 공격키를 뗌
		if (!m_bOnShot)
		{
			// 충전샷 레벨에 따라 충전 공격 횟수가 달라집니다
			// m_iChargeBulletCnt에 저장되는 값은 공격 횟수
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
			// 충전 공격 끝 상태로 변경
			m_eCurState = PLAYER::CHARGE_END;
		}
		break;
		// 충전 공격 끝
	case PLAYER::CHARGE_END:
		// 충전 공격 횟수가 남아있고 충전 공격 딜레이가 지났다면
		if (0 < m_iChargeBulletCnt && m_dwChargeBulletTime + m_dwChargeBulletDelay < GetTickCount())
		{
			// 현재 시간을 저장
			m_dwChargeBulletTime = GetTickCount();
			// 충전 공격 횟수 감소
			--m_iChargeBulletCnt;
			// 충전 공격 사운드
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_CHARGE_BULLET);
			CSoundMgr::Get_Instance()->PlaySound(L"Marion_ChargeBullet.wav", CSoundMgr::PLAYER_CHARGE_BULLET);
			// 충전 공격
			CObj* pObj = CAbstractFactory<CMarionChargeBullet>::Create(m_tInfo.fX, m_tInfo.fY - 30.f, 90.f);
			// 총알에게 몇 플레이어의 공격인지 저장
			static_cast<CBullet*>(pObj)->Set_PlayerNum(m_iPlayerNum);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER_BULLET);
		}

		// 다 쏨
		if(m_iChargeBulletCnt == 0)
		{
			// 대기 상태로 변경
			m_eCurState = PLAYER::IDLE;
			// 이미지 스프라이트 고정 해제
			m_bImmutable = false;
			// 애니메이션 프레임 초기화
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			// 이미지 길이 초기화
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
		// 폭탄
	case PLAYER::BOMB:
		// 몬스터 총알 제거
		CObjMgr::Get_Instance()->Dead_MonsterBullet();
		// 몬스터에게 지속적인 데미지
		CObjMgr::Get_Instance()->Demage_Monster(1.f, m_iPlayerNum);

		// 폭탄 사용 종료
		if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		{
			// 대기 상태로 변경
			m_eCurState = PLAYER::IDLE;
			// 무적 상태 해제
			m_bHpLock = false;
			// 이미지 스프라이트 고정 해제
			m_bImmutable = false;
			// 애니메이션 프레임 초기화
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			// 이미지 길이 초기화
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
		// 근접 공격
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
			// 대기 상태로 변경
			m_eCurState = PLAYER::IDLE;
			// 이미지 스프라이트 고정 해제
			m_bImmutable = false;
			// 애니메이션 프레임 초기화
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			// 이미지 길이 초기화
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
		// 죽음
	case PLAYER::DEAD:
		// 화면 바깥까지 떨어짐
		if (m_tRect.top < WINCY)
		{
			m_tInfo.fY += 15.f;
			break;
		}

		// 남은 목숨이 없다면
		if (CGameUIMgr::Get_Instance()->Get_Life(m_iPlayerNum) <= 0)
		{
			// 컨티뉴 상태로 변경
			m_eCurState = PLAYER::CONTINUE;
			// 스프라이트 고정
			m_bImmutable = true;
			// 죽음
			m_bDead = false;
			CGameUIMgr::Get_Instance()->On_ContinueCountdown(m_iPlayerNum);
		}
		// 남은 목숨이 있으면
		else
		{
			// 목숨 감소
			CGameUIMgr::Get_Instance()->Add_Life(-1, m_iPlayerNum);
			// 리스폰 상태로 변경
			m_eCurState = PLAYER::RESPAWN;
			// 죽음
			m_bDead = false;
			// 리스폰 시간 저장
			m_dwRespawnTime = GetTickCount();
		}
		break;
		// 리스폰
	case PLAYER::RESPAWN:
		// 리스폰 대기 시간까지 Y축 이동
		if (m_dwRespawnTime + m_dwRespawnDelay > GetTickCount())
		{
			m_tInfo.fY -= m_fSpeed;
		}
		// 대기 시간까지 이동 후
		else
		{
			// 대기 상태로 변경
			m_eCurState = PLAYER::IDLE;
			// 살아난 후에도 일정 시간 무적 상태임
			// m_bRespawn는 그 상태를 알리는 플래그
			m_bRespawn = true;
			// 이미지 스프라이트 고정 해제
			m_bImmutable = false;
			// 무적 시간을 위해 현재 시간 저장
			m_dwHpLockTime = GetTickCount();

			// 충돌 렉트 위치값 변경
			m_tHitRectPos = { 9 * 3, 8 * 3, 15 * 3, 19 * 3 };
		}
		break;
		// 컨티뉴
	case PLAYER::CONTINUE:
		// 라이프가 0 보다 크면
		if (CGameUIMgr::Get_Instance()->Get_Life(m_iPlayerNum) > 0) {
			m_eCurState = PLAYER::RESPAWN;		// 살아남
			m_dwRespawnTime = GetTickCount();
		}
		break;
	}

	// 일반 공격 횟수가 남았고 일반 공격 딜레이가 지났다면
	if (0 < m_iShotCnt && m_dwBulletTime + m_dwBulletDelay < GetTickCount())
	{
		// 현재 시간을 저장
		m_dwBulletTime = GetTickCount();
		// 일반 공격 횟수 감소
		--m_iShotCnt;
		// 일반 공격 처리
		Shot();
	}

	// 별 이펙트 생성
	Create_Effect();
	// 프레임 씬 변경 처리
	Scene_Change();
	// 이미지 RECT 정보 및 Hit RECT 정보 갱신
	Update_Rect();
	// 애니메이션 프레임 이동
	Frame_Move();

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CMarion::Late_Update()
{
}

// 렌더
void CMarion::Render(HDC _DC)
{
	HDC hMemDC;

	// 상태에 따라 렌더가 다름
	switch (m_ePreState)
	{
		// 대기 및 이동
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
		// 충전 공격
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
		// 폭탄
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
		// 근접 공격
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
		// 죽음
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
		// 리스폰
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
		// 컨티뉴
	case PLAYER::CONTINUE:
		break;
	case PLAYER::STATE_END:
		break;
	default:
		break;
	}

	// 만약 옵션에서 충돌 박스 보기를 켰다면 (넘버패드 1번 키)
	// 충돌 박스도 렌더 해줘야함
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
	// 최대 파워가 아니면
	if (m_iShotPower < 3)
	{
		// 총알 파워 증가
		++m_iShotPower;
		// 총알 파워가 1 레벨에 도달하면
		if (m_iShotPower == 1)
		{
			// 공격 간격이 좀더 빨라짐
			// 기본 100, 400 -> 80, 300 으로 변경
			m_dwBulletDelay = 80;
			m_dwKeyDelay = 300;
		}
		// 파워업 사운드
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_POWER_UP);
		CSoundMgr::Get_Instance()->PlaySound(L"Marion_PowerUp.wav", CSoundMgr::PLAYER_POWER_UP);

		return;
	}

	// 최대 파워 사운드
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_POWER_UP);
	CSoundMgr::Get_Instance()->PlaySound(L"Marion_FullPower.wav", CSoundMgr::PLAYER_POWER_UP);
}

// 총알 파워 다운
// 몬스터에게 충돌할 경우 파워 다운이 됨
void CMarion::PowerDown()
{
	// 총알 파워가 1 이상이면
	if (0 < m_iShotPower)
	{
		// 파워 다운 딜레이
		// 딜레이가 없으면 연속해서 파워 다운이 발생함
		if (m_dwPowerDownTime + m_dwPowerDownDelay > GetTickCount())
		{
			return;
		}

		// 총알 파워 감소
		--m_iShotPower;
		// 총알 파워가 0에 도달하면
		if (m_iShotPower == 0) {
			// 공격 간격 기본으로 변경
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

// 캐릭터 왼쪽 이동
// 씬에서 해당 키 입력이 들어오면 호출됨
void CMarion::Move_LL()
{
	// 키 조작 불가 상태 예외
	if (!CanKeyInput())
		return;

	// X좌표 감소
	if (m_tHitRect.left - m_fSpeed > 0)
		m_tInfo.fX -= m_fSpeed;
	// 기울기 감소
	if (m_fAngelY - m_fRotSpeed > -m_fMaxRotAngle)
		m_fAngelY -= m_fRotSpeed;

	// 이미지 스프라이트가 고정 상태가 아니면 이동에 의한 상태 변경 처리
	// 이동에 따라 플레이어 스프라이트가 변경됨
	if (!m_bImmutable)
		Move_Change();
}

// 캐릭터 왼쪽 위 이동
// 씬에서 해당 키 입력이 들어오면 호출됨
void CMarion::Move_LU()
{
	// 키 조작 불가 상태 예외
	if (!CanKeyInput())
		return;

	// X, Y 좌표 감소
	if (m_tHitRect.left - m_fSpeed / sqrtf(2.f) > 0)
		m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
	if (m_tHitRect.top - m_fSpeed / sqrtf(2.f) > 100)
		m_tInfo.fY -= m_fSpeed / sqrtf(2.f);
	// 기울기 감소
	if (m_fAngelY - m_fRotSpeed > -m_fMaxRotAngle)
		m_fAngelY -= m_fRotSpeed;

	// 이미지 스프라이트가 고정 상태가 아니면 이동에 의한 상태 변경 처리
	// 이동에 따라 플레이어 스프라이트가 변경됨
	if (!m_bImmutable)
		Move_Change();
}

// 캐릭터 왼쪽 아래 이동
// 씬에서 해당 키 입력이 들어오면 호출됨
void CMarion::Move_LD()
{
	// 키 조작 불가 상태 예외
	if (!CanKeyInput())
		return;

	// X 감소, Y 증가
	if (m_tHitRect.left - m_fSpeed / sqrtf(2.f) > 0)
		m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
	if (m_tHitRect.bottom + m_fSpeed / sqrtf(2.f) < WINCY - 100)
		m_tInfo.fY += m_fSpeed / sqrtf(2.f);
	// 기울기 감소
	if (m_fAngelY - m_fRotSpeed > -m_fMaxRotAngle)
		m_fAngelY -= m_fRotSpeed;

	// 이미지 스프라이트가 고정 상태가 아니면 이동에 의한 상태 변경 처리
	// 이동에 따라 플레이어 스프라이트가 변경됨
	if (!m_bImmutable)
		Move_Change();
}

// 캐릭터 오른쪽 이동
// 씬에서 해당 키 입력이 들어오면 호출됨
void CMarion::Move_RR()
{
	// 키 조작 불가 상태 예외
	if (!CanKeyInput())
		return;

	// X 좌표 증가
	if (m_tHitRect.right + m_fSpeed < WINCX)
		m_tInfo.fX += m_fSpeed;
	// 기울기 증가
	if (m_fAngelY + m_fRotSpeed < m_fMaxRotAngle)
		m_fAngelY += m_fRotSpeed;

	// 이미지 스프라이트가 고정 상태가 아니면 이동에 의한 상태 변경 처리
	// 이동에 따라 플레이어 스프라이트가 변경됨
	if (!m_bImmutable)
		Move_Change();
}

// 캐릭터 오른쪽 위 이동
// 씬에서 해당 키 입력이 들어오면 호출됨
void CMarion::Move_RU()
{
	// 키 조작 불가 상태 예외
	if (!CanKeyInput())
		return;

	// X 증가, Y 감소
	if (m_tHitRect.right + m_fSpeed / sqrtf(2.f) < WINCX)
		m_tInfo.fX += m_fSpeed / sqrtf(2.f);
	if (m_tHitRect.top - m_fSpeed / sqrtf(2.f) > 100)
		m_tInfo.fY -= m_fSpeed / sqrtf(2.f);
	// 기울기 증가
	if (m_fAngelY + m_fRotSpeed < m_fMaxRotAngle)
		m_fAngelY += m_fRotSpeed;

	// 이미지 스프라이트가 고정 상태가 아니면 이동에 의한 상태 변경 처리
	// 이동에 따라 플레이어 스프라이트가 변경됨
	if (!m_bImmutable)
		Move_Change();
}

// 캐릭터 오른쪽 아래 이동
// 씬에서 해당 키 입력이 들어오면 호출됨
void CMarion::Move_RD()
{
	// 키 조작 불가 상태 예외
	if (!CanKeyInput())
		return;

	// X, Y 좌표 증가
	if (m_tHitRect.right + m_fSpeed / sqrtf(2.f) < WINCX)
		m_tInfo.fX += m_fSpeed / sqrtf(2.f);
	if (m_tHitRect.bottom + m_fSpeed / sqrtf(2.f) < WINCY - 100)
		m_tInfo.fY += m_fSpeed / sqrtf(2.f);
	// 기울기 증가
	if (m_fAngelY + m_fRotSpeed < m_fMaxRotAngle)
		m_fAngelY += m_fRotSpeed;

	// 이미지 스프라이트가 고정 상태가 아니면 이동에 의한 상태 변경 처리
	// 이동에 따라 플레이어 스프라이트가 변경됨
	if (!m_bImmutable)
		Move_Change();
}

// 캐릭터 위쪽 이동
// 씬에서 해당 키 입력이 들어오면 호출됨
void CMarion::Move_UU()
{
	// 키 조작 불가 상태 예외
	if (!CanKeyInput())
		return;

	// Y 좌표 감소
	if (m_tHitRect.top - m_fSpeed > 100)
		m_tInfo.fY -= m_fSpeed;
	// 기울기가 0보다 크면
	if (m_fAngelY > 0)
	{
		// 기울기 감소 혹은 0으로 고정
		if (m_fAngelY - m_fRotSpeed > 0)
			m_fAngelY -= m_fRotSpeed;
		else
			m_fAngelY = 0.f;
	}
	// 0보다 작다면
	else
	{
		// 기울기 증가 혹은 0으로 고정
		if (m_fAngelY + m_fRotSpeed < 0)
			m_fAngelY += m_fRotSpeed;
		else
			m_fAngelY = 0.f;
	}

	// 이미지 스프라이트가 고정 상태가 아니면 이동에 의한 상태 변경 처리
	// 이동에 따라 플레이어 스프라이트가 변경됨
	if (!m_bImmutable)
		Move_Change();
}

// 캐릭 아래쪽 이동
// 씬에서 해당 키 입력이 들어오면 호출됨
void CMarion::Move_DD()
{
	// 키 조작 불가 상태 예외
	if (!CanKeyInput())
		return;

	// Y 좌표 증가
	if (m_tHitRect.bottom + m_fSpeed < WINCY - 100)
		m_tInfo.fY += m_fSpeed;
	// 기울기가 0보다 크면
	if (m_fAngelY > 0)
	{
		// 기울기 감소 혹은 0으로 고정
		if (m_fAngelY - m_fRotSpeed > 0)
			m_fAngelY -= m_fRotSpeed;
		else
			m_fAngelY = 0.f;
	}
	// 0보다 작다면
	else
	{
		// 기울기 증가 혹은 0으로 고정
		if (m_fAngelY + m_fRotSpeed < 0)
			m_fAngelY += m_fRotSpeed;
		else
			m_fAngelY = 0.f;
	}

	// 이미지 스프라이트가 고정 상태가 아니면 이동에 의한 상태 변경 처리
	// 이동에 따라 플레이어 스프라이트가 변경됨
	if (!m_bImmutable)
		Move_Change();
}

// 이동 키를 누르지 않으면 캐릭터 기울기를 0으로 점차 복구함
// 씬에서 아무 이동 키도 누르지 않으면 호출됨
void CMarion::Move_End()
{
	// 키 조작 불가 상태 예외
	if (!CanKeyInput())
		return;

	// 캐릭터 기울기가 0보다 크면
	if (m_fAngelY > 0)
	{
		// 기울기 감소 혹은 0으로 고정
		if (m_fAngelY - m_fRotSpeed > 0)
			m_fAngelY -= m_fRotSpeed;
		else
			m_fAngelY = 0.f;
	}
	// 캐릭터 키울기가 0보다 작으면
	else
	{
		// 기울기 증가 혹은 0으로 고정
		if (m_fAngelY + m_fRotSpeed < 0)
			m_fAngelY += m_fRotSpeed;
		else
			m_fAngelY = 0.f;
	}

	// 이미지 스프라이트가 고정 상태가 아니면 이동에 의한 상태 변경 처리
	// 이동에 따라 플레이어 스프라이트가 변경됨
	if (!m_bImmutable)
		Move_Change();
}

// 일반 공격
// 한번의 공격으로 3번 공격하는 세미 오토 방식
// 일정 시간 이상 누르면 충전 공격으로 바뀜
// 씬에서 해당 키 입력이 들어오면 호출됨
void CMarion::Attack()
{
	// 키 조작 불가 상태 예외
	if (!CanKeyInput())
		return;

	// 공격 조작 불가 상태 예외
	if (!CanAttack())
		return;

	// 공격 플래그가 false면
	if (!m_bOnShot)
	{
		// 공격 플래그 true로
		m_bOnShot = true;
		// 충전 공격 시간 시작
		m_dwChargeTime = GetTickCount();
	}

	// 공격 횟수가 0보다 작고 (세미오토 방식이라 한번의 공격으로 3번의 공격이 나감)
	// 키 입력 딜레이 시간이 지났으면 일반 공격 횟수 카운트 초기화
	if (m_iShotCnt <= 0 && m_dwKeyTime + m_dwKeyDelay < GetTickCount())
	{
		// 3으로 초기화 및 공격 시간 간격 초기화
		m_iShotCnt = 3;
		m_dwBulletTime = GetTickCount();
		m_dwKeyTime = GetTickCount();
	}

	// 공격버튼을 일정시간 계속 누르고 있고 충전 상태가 아니며 충전 게이지가 있다면 충전 공격 준비
	if (m_dwChargeTime + m_dwChargeDelay < GetTickCount() &&
		m_ePreState != PLAYER::CHARGE_START &&
		1 <= CGameUIMgr::Get_Instance()->Get_ChargeLevel(m_iPlayerNum))
	{
		// 충전 공격 상태로
		m_eCurState = PLAYER::CHARGE_START;
		// 이미지 스프라이트 고정
		m_bImmutable = true;
		// 남은 일반 공격 횟수 0으로 초기화
		m_iShotCnt = 0;
	}
}

// 일반 공격 키를 누르지 않으면 공격 플래그를 false로 초기화
void CMarion::Attack_End()
{
	// 키 조작 불가 상태 예외
	if (!CanKeyInput())
		return;

	// 공격 플래그 false로
	m_bOnShot = false;
}

// 폭탄 공격
// 남은 폭탄이 없으면 사용 불가 폭탄 1개를 소모
// 폭탄을 쓰는 순간 화면에 있는 적 총알을 전부 캔디로 교체함
// 이 후에 폭탄이 지속되는 동안 적이 공격해서 생성되는 총알은 제거됨
// 씬에서 해당 키 입력이 들어오면 호출됨
void CMarion::Bomb()
{
	// 키 조작 불가 상태 예외
	if (!CanKeyInput())
		return;

	// 남은 폭탄이 없으면 폭탄 공격 불가
	if (CGameUIMgr::Get_Instance()->Get_Bomb(m_iPlayerNum) == 0)
		return;

	// 폭탄 사용 개수 감소
	CGameUIMgr::Get_Instance()->Add_Bomb(-1, m_iPlayerNum);

	// 폭탄을 사용한 처음에만
	// 화면에 보이는 몬스터 총알 사탕으로 교체
	CObjMgr::Get_Instance()->Change_Candy();

	// 폭탄 공격 사운드
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_BOMB);
	CSoundMgr::Get_Instance()->PlaySound(L"Marion_Bomb.wav", CSoundMgr::PLAYER_BOMB);
	// 이펙트 사운드
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_BOMB_EFFECT);
	CSoundMgr::Get_Instance()->PlaySound(L"Marion_BombEffect.wav", CSoundMgr::PLAYER_BOMB_EFFECT);
	// 폭탄 공격 상태로 변경
	m_eCurState = PLAYER::BOMB;
	// 무적 상태
	m_bHpLock = true;
	// 이미지 스프라이트 고정
	m_bImmutable = true;
}

// 근접 공격
// 충전 게이지를 사용하여 공격
// 사용하는 동안 이미지 스프라이트 고정
// 씬에서 해당 키 입력이 들어오면 호출됨
void CMarion::Melee()
{
	// 키 조작 불가 상태 예외
	if (!CanKeyInput())
		return;

	// 이미 근접 공격 중이면 불가
	if (m_ePreState == PLAYER::MELEE)
		return;

	// 충전량이 없으면 사용 불가
	if (CGameUIMgr::Get_Instance()->Get_ChargeLevel(m_iPlayerNum) < 1)
		return;

	// 춘정량 소모
	CGameUIMgr::Get_Instance()->Add_ChargeLevel(-1, m_iPlayerNum);
	// 근접 공격 상태로 변경
	m_eCurState = PLAYER::MELEE;
	// 이미지 스프라이트 고정
	m_bImmutable = true;
	// 근접 공격 적중 플래그 초기화
	m_bMeleeHit = false;
	// 근접 공격 사운드
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_MELEE);
	CSoundMgr::Get_Instance()->PlaySound(L"Marion_Melee.wav", CSoundMgr::PLAYER_MELEE);
}

// 다시 하기
// 컨티뉴 상태일 때 이어하기 기능
// 씬에서 해당 키 입력이 들어오면 호출됨
void CMarion::Continue()
{
	// 컨티뉴 상태가 아니면 예외
	if (m_ePreState != PLAYER::CONTINUE)
		return;

	// 크레딧이 없으면 다시하기 못함
	if (0 == CGameUIMgr::Get_Instance()->Get_Credit())
		return;

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

// 프레임 씬 변경 처리
// 몇몇 애니메이션 프레임의 이미지 길이가 달라서
// 상태가 변경된 후 위치 좌표를 초기화할 때 주의해야 함
void CMarion::Scene_Change()
{
	// 이전 상태와 다르면
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
			// 대기
		case PLAYER::IDLE:
			m_tFrame.iFrameScene = 0;
			break;
			// 왼쪽 이동 1
		case PLAYER::LEFT_1:
			m_tFrame.iFrameScene = 1;
			break;
			// 왼쪽 이동 2
		case PLAYER::LEFT_2:
			m_tFrame.iFrameScene = 2;
			break;
			// 오른쪽 이동 1
		case PLAYER::RIGHT_1:
			m_tFrame.iFrameScene = 3;
			break;
			// 오른쪽 이동 2
		case PLAYER::RIGHT_2:
			m_tFrame.iFrameScene = 4;
			break;
			// 충전 공격 시작
			// 기본 상태랑 캐릭터 위치가 달라서 위치 보정 해줘야함
			// HitRect 위치도 변경해줘야 함
		case PLAYER::CHARGE_START:
			// 애니메이션 프레임 초기화
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 40;
			// 이미지 길이 초기화
			m_tInfo.iCX = MARION_CHARGE_WIDTH * 3;
			m_tInfo.iCY = MARION_CHARGE_HEIGHT * 3;
			m_iImageWidth = MARION_CHARGE_WIDTH;
			m_iImageHeight = MARION_CHARGE_HEIGHT;

			// 좌표 변경
			m_tInfo.fY = m_tInfo.fY - (3.f * 3.f);
			// 충돌 렉트 위치값 변경
			m_tHitRectPos = { 17 * 3, 23 * 3, 23 * 3, 34 * 3 };
			break;
			// 충전 공격 반복
		case PLAYER::CHARGE_REPEAT:
			// 애니메이션 프레임 초기화
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 40;
			break;
			// 충전 공격 끝
		case PLAYER::CHARGE_END:
			// 애니메이션 프레임 초기화
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 13;
			m_tFrame.iFrameScene = 2;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 40;
			break;
			// 폭탄 공격
		case PLAYER::BOMB:
			// 애니메이션 프레임 초기화
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 31;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 70;
			// 이미지 길이 초기화
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
			// 애니메이션 프레임 초기화
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 26;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 30;
			// 이미지 길이 초기화
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
			// 죽음
		case PLAYER::DEAD:
			// 애니메이션 프레임 초기화
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			// 이미지 길이 초기화
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

// 이동 상태 변경 처리
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

// 일반 공격 처리
void CMarion::Shot()
{
	CObj* pObj;
	// 일반 공격 사운드
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SHOT);
	CSoundMgr::Get_Instance()->PlaySound(L"Marion_Shot.wav", CSoundMgr::PLAYER_SHOT);
	// 파워 레벨에 따라 분류
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

		// 유도 미사일이 사용중이 아니면 (유도 미사일은 한 화면에서 제한된 숫자로만 존재함)
		if (m_tSubBulletCnt.flag == false)
		{
			// 유도 미사일 사용
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

		// 유도 미사일이 사용중이 아니면 (유도 미사일은 한 화면에서 제한된 숫자로만 존재함)
		if (m_tSubBulletCnt.flag == false)
		{
			// 유도 미사일 사용
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

	// 이펙트 생성 딜레이가 지났으면
	if (m_dwEffectCreateTime + m_dwEffectCreateDelay < GetTickCount())
	{
		// 현재 시간 저장
		m_dwEffectCreateTime = GetTickCount();

		// 상태에 따라 생성되는 이펙트 좌표나 방향 개수가 다름
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

// 키 처리가 가능한지 검사
// 키 처리 관련 멤버 함수에서 호출함
// 반환 값 : 키 조작이 불가능한 상태면 false, 가능하면 true
bool CMarion::CanKeyInput()
{
	// 키 조작 불가 상태 예외
	if (m_ePreState == PLAYER::RESPAWN ||
		m_ePreState == PLAYER::DEAD ||
		m_ePreState == PLAYER::BOMB ||
		g_bStageClear)
		return false;

	return true;
}

// 공격할 수 있는지 검사
// 공격키 처리 함수에서 호출함
// 반환 값 : 공격이 불가능한 상태면 false, 가능하면 true
bool CMarion::CanAttack()
{
	// 공격 조작 불가 상태 예외
	if (m_ePreState == PLAYER::CHARGE_START ||
		m_ePreState == PLAYER::CHARGE_REPEAT ||
		m_ePreState == PLAYER::DEAD ||
		m_ePreState == PLAYER::RESPAWN)
		return false;

	return true;
}

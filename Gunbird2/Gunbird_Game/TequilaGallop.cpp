// 기명준
// 2 페이즈 보스
// 등 공격 패턴 때 양쪽 팔 부분의 Hit Rect가 이동 함
// 팔이 파괴되면 반대 방향으로 반동 이동 함

#include "stdafx.h"
#include "TequilaGallop.h"

#include "BmpMgr.h"
#include "ObjMgr.h"
#include "GameUIMgr.h"
#include "SoundMgr.h"

#include "Bullet_1.h"
#include "Bullet_2.h"
#include "Bullet_3.h"
#include "Bullet_4.h"
#include "Explosion_04.h"
#include "Explosion_03.h"
#include "Explosion_02_1.h"
#include "Explosion_01.h"
#include "WheelSmoke.h"
#include "PipeSmoke.h"
#include "Effect_Charge.h"
#include "Explosion_05.h"


CTequilaGallop::CTequilaGallop()
	: m_ePreState(STATE::END), m_eCurState(STATE::END)
	, m_dwShotCnt(0), m_dwShotTime(0), m_dwShotDelay(0)
	, m_fShotAngle(0.f), m_fBodyPosY(0.f), m_fArmPosY(0.f), m_dwShakeTime(0), m_dwShakeDelay(200)
	, m_dwMoveTime(0), m_dwMoveDelay(1000), m_dwWhellSmokeTime(0), m_dwWhellSmokeDelay(200)
	, m_dwChargeTime(0), m_dwChargeDelay(960), m_dwSoundTime(0), m_dwSoundDelay(500)
	, m_dwUnderFrameCnt(0), m_dwPatternIndex(0)
	, m_tSubFrameArr{}, m_bDamageFlagArr{}
	, m_bMoveFlag(false), m_bDamageLeftMove(false), m_bDamageRightMove(false)
{
}


CTequilaGallop::~CTequilaGallop()
{
	Release();
}

void CTequilaGallop::Initialize()
{
	// 이미지 크기 설정
	m_tInfo.iCX = TEQUILA_GALLOP_WIDTH * 3;
	m_tInfo.iCY = TEQUILA_GALLOP_HEIGHT * 3;
	m_iImageWidth = TEQUILA_GALLOP_WIDTH;
	m_iImageHeight = TEQUILA_GALLOP_HEIGHT;
	// 체력
	m_fHp = 5000;
	//m_fHp = 100;
	// 충돌 렉트 위치 및 부분 체력 설정
	m_vecHitRectPos.reserve(3);
	m_vecHitRectPos.emplace_back(RECT{ 73 * 3, 55 * 3, 113 * 3, 131 * 3 });		// 인덱스 0번 상체
	m_vecHitRectPos.emplace_back(RECT{ 24 * 3, 55 * 3, 73 * 3, 122 * 3 });		// 인덱스 1번 왼팔
	m_vecHitRectPos.emplace_back(RECT{ 113 * 3, 55 * 3, 162 * 3, 122 * 3 });	// 인덱스 2번 오른팔
	m_vecHitRect.reserve(3);
	m_vecHitRect.emplace_back(RECT{});
	m_vecHitRect.emplace_back(RECT{});
	m_vecHitRect.emplace_back(RECT{});
	m_vecRectHp.emplace_back(5000.f);
	m_vecRectHp.emplace_back(1800.f);
	m_vecRectHp.emplace_back(1800.f);
	// 기본 상태
	m_eCurState = STATE::WAIT;
	// 공격 간격
	m_dwShotDelay = 2000;
	// 현재 시간 저장
	m_dwShotTime = GetTickCount();

	// 패턴
	m_vecPattern.reserve(6);
	m_vecPattern.emplace_back(STATE::R_ARM_ATTACK);
	m_vecPattern.emplace_back(STATE::FRONT_ATTACK);
	m_vecPattern.emplace_back(STATE::BACK_ATTACK);
	m_vecPattern.emplace_back(STATE::L_ARM_ATTACK);
	m_vecPattern.emplace_back(STATE::SHOULDER_ATTACK);
	m_vecPattern.emplace_back(STATE::BACK_ATTACK);
	// 부위 파괴 이펙트 관련 초기화
	m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].iEffectCnt = 30;
	m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].dwDelay = 50;
	m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].iEffectCnt = 30;
	m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].dwDelay = 50;

	// 점수
	m_iScore = 5000;
}

// 업데이트
int CTequilaGallop::Update()
{
	// 삭제
	if (m_bRemove)
		return OBJ_DEAD;
	// 죽음
	if (m_bDead)
	{
		// 처음 파괴되면
		if (!m_bDestroy && m_ePreState != STATE::DESTROY)
		{
			// 파괴 상태로 변경
			m_eCurState = STATE::DESTROY;
			// 파괴되서 HP 다시 락
			m_bHpLock = true;
			// 파괴 플래그 true
			m_bDestroy = true;

			// 파괴 이펙트 생성
			CObj* pObj = CObjFactory<CExplosion_04>::Create(m_tInfo.fX, m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			int irand = rand() % 3 + 1;
			pObj = CObjFactory<CExplosion_03>::Create(m_tInfo.fX - 165.f, m_tInfo.fY + 36.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(100 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CObjFactory<CExplosion_03>::Create(m_tInfo.fX + 165.f, m_tInfo.fY + 36.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(100 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			pObj = CObjFactory<CExplosion_02_1>::Create(m_tInfo.fX - 80.f, m_tInfo.fY);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(30 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CObjFactory<CExplosion_02_1>::Create(m_tInfo.fX + 80.f, m_tInfo.fY);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)50);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CObjFactory<CExplosion_01>::Create(m_tInfo.fX + irand * 30, m_tInfo.fY + 20.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(irand * 10));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CObjFactory<CExplosion_01>::Create(m_tInfo.fX - irand * 30, m_tInfo.fY + 50.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)100);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			// 공격중에 죽을경우 다음 공격상태로 전환되면서 죽는 경우가 있음
			// 그래서 상태 바로 전환
			Scene_Change();
		}
	}

	// 보스 행동
	Action();
	// 이동
	Move();
	// 데미지 파괴 이펙트
	DamageEffect();
	// 상체 흔들기
	// 주기적으로 흔들림
	Shake_Body();
	// 이미지 RECT 정보 및 Hit RECT 정보 갱신
	// Obj가 아닌 Boss의 Update_Rect 호출
	Update_Rect();
	// 프레임 씬 변경 처리
	Scene_Change();
	// 부분 이미지 프레임 이동
	Frame_MoveEX();

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CTequilaGallop::Late_Update()
{
	// 맵 하단 바깥으로 나가면 삭제
	if (WINCY <= m_tRect.top)
		m_bRemove = true;
}

// 렌더
void CTequilaGallop::Render(HDC _DC)
{
	HDC hMemDC;

	switch (m_ePreState)
	{
	case STATE::WAIT:
	case STATE::PHASE_MOVE:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TequilaGallop_Phase_Move");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
		break;
	case STATE::PHASE_CHANGE:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TequilaGallop_Phase_Change");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
		break;
	case STATE::READY:
	case STATE::NEXT_PATTERN:
	case STATE::L_ARM_ATTACK:
	case STATE::R_ARM_ATTACK:
	case STATE::FRONT_ATTACK:
	case STATE::BACK_ATTACK:
	case STATE::SHOULDER_ATTACK:
		// 하체
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TequilaGallop_Under");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));

		// 상체
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TequilaGallop_Body");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top + m_fBodyPosY
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt * m_iImageWidth, m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));

		// 팔
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TequilaGallop_Arm");
		// 왼팔
		if (!m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].bDamage)
		{
			GdiTransparentBlt(_DC
				, m_tRect.left, m_tRect.top + m_fBodyPosY + m_fArmPosY
				, m_tInfo.iCX, m_tInfo.iCY
				, hMemDC
				, m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameCnt * m_iImageWidth, m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameScene * m_iImageHeight
				, m_iImageWidth, m_iImageHeight
				, RGB(255, 0, 255));
		}

		// 오른팔
		if (!m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].bDamage)
		{
			GdiTransparentBlt(_DC
				, m_tRect.left, m_tRect.top + m_fBodyPosY + m_fArmPosY
				, m_tInfo.iCX, m_tInfo.iCY
				, hMemDC
				, m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameCnt * m_iImageWidth, m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameScene * m_iImageHeight
				, m_iImageWidth, m_iImageHeight
				, RGB(255, 0, 255));
		}

		break;
	case STATE::DESTROY:
	case STATE::FINAL_1:
	case STATE::FINAL_2:
	case STATE::FINAL_3:
	case STATE::FINAL_4:
	case STATE::FINAL_5:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TequilaGallop_Destroy");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Shark_Destroy");
		GdiTransparentBlt(_DC
			, m_tRect.left + (87 * 3), m_tRect.top + (66 * 3)
			, SHARK_DESTROY_WIDTH * 3, SHARK_DESTROY_HEIGHT * 3
			, hMemDC
			, m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt * SHARK_DESTROY_WIDTH, m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameScene * SHARK_DESTROY_HEIGHT
			, SHARK_DESTROY_WIDTH, SHARK_DESTROY_HEIGHT
			, RGB(255, 0, 255));

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Blade_Destroy");
		GdiTransparentBlt(_DC
			, m_tRect.left + (76 * 3), m_tRect.top + (82 * 3)
			, BLADE_DESTROY_WIDTH * 3, BLADE_DESTROY_HEIGHT * 3
			, hMemDC
			, m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameCnt * BLADE_DESTROY_WIDTH, m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameScene * BLADE_DESTROY_HEIGHT
			, BLADE_DESTROY_WIDTH, BLADE_DESTROY_HEIGHT
			, RGB(255, 0, 255));

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Gimmick_Destroy");
		GdiTransparentBlt(_DC
			, m_tRect.left + (93 * 3), m_tRect.top + (81 * 3)
			, GIMMICK_DESTROY_WIDTH * 3, GIMMICK_DESTROY_HEIGHT * 3
			, hMemDC
			, m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameCnt * GIMMICK_DESTROY_WIDTH, m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameScene * GIMMICK_DESTROY_HEIGHT
			, GIMMICK_DESTROY_WIDTH, GIMMICK_DESTROY_HEIGHT
			, RGB(255, 0, 255));
		break;
	}

	// 만약 옵션에서 충돌 박스 보기를 켰다면 (넘버패드 1번 키)
	// 충돌 박스도 렌더 해줘야함
	if (!g_bHitRectRender) {
		return;
	}
	size_t uiSize = m_vecHitRect.size();
	size_t uiCnt = 0;
	RECT* pRect = m_vecHitRect.data();
	while (uiCnt < uiSize) {
		Rectangle(_DC, pRect[uiCnt].left, pRect[uiCnt].top, pRect[uiCnt].right, pRect[uiCnt].bottom);
		++uiCnt;
	}
}

void CTequilaGallop::Release()
{
}

// 보스 부위별 파괴 데미지 체크
void CTequilaGallop::Check_Damage(int _iPlayerNum)
{
	CObj* pObj;
	LONG lCenterX;
	LONG lCenterY;

	// 왼팔
	if (!m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].bDamage && m_vecRectHp[static_cast<UINT>(SPRITE::L_ARM)] <= 0) {
		m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].bDamage = true;

		lCenterX = (m_vecHitRect[static_cast<UINT>(SPRITE::L_ARM)].right - m_vecHitRect[static_cast<UINT>(SPRITE::L_ARM)].left) >> 1;
		lCenterY = (m_vecHitRect[static_cast<UINT>(SPRITE::L_ARM)].bottom - m_vecHitRect[static_cast<UINT>(SPRITE::L_ARM)].top) >> 1;

		pObj = CObjFactory<CExplosion_02_1>::Create(
			static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::L_ARM)].left + lCenterX),
			static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::L_ARM)].top + lCenterY));
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		// 충돌 렉트 변경
		m_vecHitRectPos[static_cast<UINT>(SPRITE::L_ARM)] = { 0, 0, 0, 0 };

		// 부위 파괴 점수 획득
		CGameUIMgr::Get_Instance()->Add_Score(4000, _iPlayerNum);
	}

	// 오른팔
	if (!m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].bDamage && m_vecRectHp[static_cast<UINT>(SPRITE::R_ARM)] <= 0) {
		m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].bDamage = true;

		lCenterX = (m_vecHitRect[static_cast<UINT>(SPRITE::R_ARM)].right - m_vecHitRect[static_cast<UINT>(SPRITE::R_ARM)].left) >> 1;
		lCenterY = (m_vecHitRect[static_cast<UINT>(SPRITE::R_ARM)].bottom - m_vecHitRect[static_cast<UINT>(SPRITE::R_ARM)].top) >> 1;

		pObj = CObjFactory<CExplosion_02_1>::Create(
			static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::R_ARM)].left + lCenterX),
			static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::R_ARM)].top + lCenterY));
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		// 충돌 렉트 변경
		m_vecHitRectPos[static_cast<UINT>(SPRITE::R_ARM)] = { 0, 0, 0, 0 };

		// 부위 파괴 점수 획득
		CGameUIMgr::Get_Instance()->Add_Score(4000, _iPlayerNum);
	}
}

// 프레임 씬 변경 처리
void CTequilaGallop::Scene_Change()
{
	// 이전 상태와 다르면 교체
	if (m_ePreState != m_eCurState)
	{
		// 보스가 여러 프레임으로 구성되어있음
		switch (m_eCurState)
		{
			// 대기
		case STATE::WAIT:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 40;
			// 페이즈 이동
		case STATE::PHASE_MOVE:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 40;
			break;
			// 페이즈 변신
		case STATE::PHASE_CHANGE:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 40;
			break;
			// 준비
		case STATE::READY:
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameStart = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameScene = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameTime = GetTickCount();
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameSpeed = 999999;

			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameCnt = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameStart = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameEnd = 4;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameScene = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].dwFrameTime = GetTickCount();
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].dwFrameSpeed = 40;

			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameCnt = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameStart = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameEnd = 4;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameScene = 2;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].dwFrameTime = GetTickCount();
			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].dwFrameSpeed = 40;
			break;
			// 이동
		case STATE::NEXT_PATTERN:
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameStart = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameScene = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameTime = GetTickCount();
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameSpeed = 999999;

			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameCnt = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameStart = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameEnd = 4;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameScene = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].dwFrameTime = GetTickCount();
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].dwFrameSpeed = 40;

			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameCnt = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameStart = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameEnd = 4;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameScene = 2;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].dwFrameTime = GetTickCount();
			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].dwFrameSpeed = 40;
			break;
			// 왼팔 공격
		case STATE::L_ARM_ATTACK:
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameStart = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameScene =1;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameTime = GetTickCount();
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameSpeed = 999999;

			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameCnt = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameStart = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameEnd = 19;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameScene = 1;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].dwFrameTime = GetTickCount();
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].dwFrameSpeed = 100;

			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS_ATTACK);
			CSoundMgr::Get_Instance()->PlaySound(L"Boss_Attack1.wav", CSoundMgr::BOSS_ATTACK);
			break;
			// 오른팔 공격
		case STATE::R_ARM_ATTACK:
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameStart = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameScene = 1;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameTime = GetTickCount();
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameSpeed = 999999;

			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameCnt = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameStart = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameEnd = 19;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameScene = 3;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].dwFrameTime = GetTickCount();
			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].dwFrameSpeed = 100;

			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS_ATTACK);
			CSoundMgr::Get_Instance()->PlaySound(L"Boss_Attack1.wav", CSoundMgr::BOSS_ATTACK);
			break;
			// 가슴 공격
		case STATE::FRONT_ATTACK:
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameStart = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameScene = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameTime = GetTickCount();
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameSpeed = 999999;
			break;
			// 등 공격
		case STATE::BACK_ATTACK:
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameStart = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd = 8;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameScene = 2;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameTime = GetTickCount();
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameSpeed = 100;

			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS_ATTACK);
			CSoundMgr::Get_Instance()->PlaySound(L"Boss_Attack1.wav", CSoundMgr::BOSS_ATTACK);
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER);
			CSoundMgr::Get_Instance()->PlaySound(L"Boss_Attack2.wav", CSoundMgr::MONSTER);
			break;
			// 어깨 공격
		case STATE::SHOULDER_ATTACK:
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameStart = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameScene = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameTime = GetTickCount();
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameSpeed = 999999;
			break;
			// 파괴
		case STATE::DESTROY:
		case STATE::FINAL_1:
		case STATE::FINAL_2:
		case STATE::FINAL_3:
		case STATE::FINAL_4:
		case STATE::FINAL_5:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 40;

			// 샤크
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameStart = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd = 3;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameScene = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameTime = GetTickCount();
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameSpeed = 100;

			// 블레이드
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameCnt = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameStart = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameEnd = 3;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameScene = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].dwFrameTime = GetTickCount();
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].dwFrameSpeed = 100;

			// 기믹
			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameCnt = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameStart = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameEnd = 2;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameScene = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].dwFrameTime = GetTickCount();
			m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].dwFrameSpeed = 100;
			break;
		}
		m_ePreState = m_eCurState;
	}
}

// 부분 스프라이트 캐논 프레임 업데이트
void CTequilaGallop::Frame_MoveEX()
{
	// 하체
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameCnt;
		if (m_tFrame.iFrameCnt > m_tFrame.iFrameEnd)
		{
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			++m_dwUnderFrameCnt;

			// 덜컹 거렸으면 다시 프레임 복구
			if (m_tFrame.iFrameEnd == 19)
			{
				m_dwUnderFrameCnt = 0;
				m_tFrame.iFrameEnd = 3;
			}

			// 몇번을 주기로 덜컹 덜컹 거려야됨
			if (m_dwUnderFrameCnt == 60)
			{
				m_tFrame.iFrameEnd = 19;
			}
		}

		m_tFrame.dwFrameTime = GetTickCount();
	}

	// 부분 스프라이트
	// 상체, 왼팔, 오른팔
	for (int iCnt = 0; iCnt < static_cast<UINT>(SPRITE::END); ++iCnt)
	{
		if (m_tSubFrameArr[iCnt].iFrameStart <= m_tSubFrameArr[iCnt].iFrameEnd)
		{
			if (m_tSubFrameArr[iCnt].dwFrameTime + m_tSubFrameArr[iCnt].dwFrameSpeed < GetTickCount())
			{
				++m_tSubFrameArr[iCnt].iFrameCnt;
				if (m_tSubFrameArr[iCnt].iFrameCnt > m_tSubFrameArr[iCnt].iFrameEnd)
					m_tSubFrameArr[iCnt].iFrameCnt = m_tSubFrameArr[iCnt].iFrameStart;

				m_tSubFrameArr[iCnt].dwFrameTime = GetTickCount();
			}
		}
		else
		{
			if (m_tSubFrameArr[iCnt].dwFrameTime + m_tSubFrameArr[iCnt].dwFrameSpeed < GetTickCount())
			{
				--m_tSubFrameArr[iCnt].iFrameCnt;
				if (m_tSubFrameArr[iCnt].iFrameCnt < m_tSubFrameArr[iCnt].iFrameEnd)
					m_tSubFrameArr[iCnt].iFrameCnt = m_tSubFrameArr[iCnt].iFrameStart;

				m_tSubFrameArr[iCnt].dwFrameTime = GetTickCount();
			}
		}
	}
}

// 보스 행동
void CTequilaGallop::Action()
{
	switch (m_ePreState)
	{
		// 대기
		// 바로 시작하지않고 기다림
	case STATE::WAIT:
		if (m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			m_eCurState = STATE::PHASE_MOVE;

			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS_ATTACK);
			CSoundMgr::Get_Instance()->PlaySound(L"Boss_PhaseChange.wav", CSoundMgr::BOSS_ATTACK);
		}
		break;
		// 페이즈 이동
	case STATE::PHASE_MOVE:
		if (m_tInfo.fY > 100.f)
		{
			m_tInfo.fY -= 1.f;
		}
		else
		{
			m_eCurState = STATE::PHASE_CHANGE;
		}
		break;
		// 페이즈 변신
	case STATE::PHASE_CHANGE:
		if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		{
			m_eCurState = STATE::READY;

			// 하체 프레임
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 10;
		}
		break;
		// 보스 2차전 준비
	case STATE::READY:
		if (m_bHpLock)
		{
			if (m_tInfo.fY >= 0)
			{
				m_bHpLock = false;		// 이제부터 맞을 수 있음
			}
		}

		if (m_tInfo.fY < 200.f)
		{
			m_tInfo.fY += 2.f;
		}
		else
		{
			m_eCurState = STATE::NEXT_PATTERN;
		}

		break;
		// 이동
	case STATE::NEXT_PATTERN:
		// 다음 패턴 대기 시간이 아직 지나지 않았으면
		if (m_dwShotTime + m_dwShotDelay > GetTickCount())
			break;

		while (true)
		{
			// 이번 공격 패턴 저장
			m_eCurState = m_vecPattern[m_dwPatternIndex];
			// 다음 공격 패턴 인덱스로 
			m_dwPatternIndex = (m_dwPatternIndex + 1) % m_vecPattern.size();
			// 왼팔 공격 패턴인데 파괴된 상태면 다음 패턴
			if (m_eCurState == STATE::L_ARM_ATTACK &&
				m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].bDamage)
			{
				continue;
			}
			// 오른팔 공격 패턴인데 파괴된 상태면 다음 패턴
			else if (m_eCurState == STATE::R_ARM_ATTACK &&
				m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].bDamage)
			{
				continue;
			}

			// 반복문 나감
			break;
		}
		break;
		// 왼팔 공격
	case STATE::L_ARM_ATTACK:
		// 공격 도중 파괴 됬다면 중단해야됨
		if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].bDamage)
		{
			// 다음 패턴을 위해 초기화
			m_bAttack = false;
			m_eCurState = STATE::NEXT_PATTERN;
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 1000;
			break;
		}

		// 5번째 스프라이트 프레임에 공격함
		if (!m_bAttack && m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameCnt == 5)
		{
			// 공격
			m_bAttack = true;
			// 공격 간격
			m_dwShotDelay = 100;
			// 총알 각도 초기화
			m_fShotAngle = 0.f;
			// 공격 횟수 초기화
			m_dwShotCnt = 0;

			// 상체 스프라이트 변경
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd = 9;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameSpeed = 100;
		}

		// 공격
		if (m_bAttack &&
			m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			// 쏘는 각도 변경
			m_fShotAngle = m_fShotAngle + 18.f;
			// 총알 카운트 증가
			++m_dwShotCnt;
			// 7번 쐈으면 멈춤
			if (m_dwShotCnt >= 7) {
				m_bAttack = false;
			}
			// 왼팔 공격
			L_Arm_Shot();
			// 현재 공격 시간 저장
			m_dwShotTime = GetTickCount();
		}

		// 상체 프레임 고정
		if (m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt == m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd)
		{
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameStart = m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd;
		}

		// 공격 끝
		if (m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameCnt == m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameEnd)
		{
			// 다음 패턴을 위해 초기화
			m_bAttack = false;
			m_eCurState = STATE::NEXT_PATTERN;
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 1000;

			// 배기구 이펙트 출력
			Pipe_Smoke();
		}

		break;
		// 오른팔 공격
	case STATE::R_ARM_ATTACK:
		// 공격 도중 파괴 됬다면 중단해야됨
		if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].bDamage)
		{
			// 다음 패턴을 위해 초기화
			m_bAttack = false;
			m_eCurState = STATE::NEXT_PATTERN;
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 1000;
			break;
		}

		// 5번째 스프라이트 프레임에 공격함
		if (!m_bAttack && m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameCnt == 5)
		{
			// 공격
			m_bAttack = true;
			// 공격 간격
			m_dwShotDelay = 100;
			// 총알 각도 초기화
			m_fShotAngle = 0.f;
			// 공격 횟수 초기화
			m_dwShotCnt = 0;

			// 상체 스프라이트 변경
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd = 9;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameSpeed = 100;
		}

		// 공격
		if (m_bAttack &&
			m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			// 쏘는 각도 변경
			m_fShotAngle = m_fShotAngle + 18.f;
			// 총알 카운트 증가
			++m_dwShotCnt;
			// 7번 쐈으면 멈춤
			if (m_dwShotCnt >= 7) {
				m_bAttack = false;
			}
			// 오른팔 공격
			R_Arm_Shot();
			// 현재 공격 시간 저장
			m_dwShotTime = GetTickCount();
		}

		// 상체 프레임 고정
		if (m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt == m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd)
		{
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameStart = m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd;
		}

		// 공격 끝
		if (m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameCnt == m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameEnd)
		{
			// 다음 패턴을 위해 초기화
			m_bAttack = false;
			m_eCurState = STATE::NEXT_PATTERN;
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 1000;

			// 배기구 이펙트 출력
			Pipe_Smoke();
		}

		break;
		// 가슴 공격
	case STATE::FRONT_ATTACK:
		// 기를 모으고 공격함
		if (!m_bAttack)
		{
			// 공격
			m_bAttack = true;
			// 현재 충전 시간 저장
			m_dwChargeTime = GetTickCount();
			// 공격 간격
			m_dwShotDelay = 200;
			// 공격 횟수 초기화
			m_dwShotCnt = 0;

			// 충전 이펙트 생성
			// 왼쪽
			CObj* pObj = CObjFactory<CEffect_Charge>::Create(m_tInfo.fX - (14.f * 3.f), m_tInfo.fY + (27.f * 3.f));
			pObj->Set_Target(this);
			static_cast<CEffect*>(pObj)->Set_RelativePos(-14.f * 3.f, 27.f * 3.f);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			// 오른쪽
			pObj = CObjFactory<CEffect_Charge>::Create(m_tInfo.fX + (14.f * 3.f), m_tInfo.fY + (27.f * 3.f));
			pObj->Set_Target(this);
			pObj->Set_SceneID(1);
			static_cast<CEffect*>(pObj)->Set_RelativePos(14.f * 3.f, 27.f * 3.f);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS_ATTACK);
			CSoundMgr::Get_Instance()->PlaySound(L"Boss_Charge.wav", CSoundMgr::BOSS_ATTACK);
		}

		
		// 충전 시간이 지나지 않았으면
		if (m_dwChargeTime + m_dwChargeDelay > GetTickCount())
			break;

		// 공격
		if (m_bAttack &&
			m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			// 총알 카운트 증가
			++m_dwShotCnt;
			// 6번 쐈으면 멈춤
			if (m_dwShotCnt >= 6)
			{
				// 다음 패턴을 위해 초기화
				m_bAttack = false;
				m_eCurState = STATE::NEXT_PATTERN;
				m_dwShotTime = GetTickCount();
				m_dwShotDelay = 1000;
			}

			// 좌,우 번갈아가면서 공격함
			if (m_dwShotCnt % 2)
			{
				// 오른쪽
				Front_Shot(14.f * 3.f, 27.f * 3.f);
			}
			else
			{
				// 왼쪽
				Front_Shot(-14.f * 3.f, 27.f * 3.f);
			}

			m_dwShotTime = GetTickCount();
		}

		break;
		// 등 공격
	case STATE::BACK_ATTACK:
		// 등 공격 패턴 때 팔의 좌표가 바뀜
		// 프레임 진행에 따라 팔의 좌표 변경
		if (m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt == 1) {
			m_fArmPosY = 3.f * 3.f;
		}
		else if (m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt == 2) {
			m_fArmPosY = 7.f * 3.f;
		}
		else if (m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt == 3) {
			m_fArmPosY = 12.f * 3.f;
		}
		else if (m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt == 4) {
			m_fArmPosY = 16.f * 3.f;
		}
		else if (m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt == 5) {
			m_fArmPosY = 19.f * 3.f;
		}
		else if (m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt == 6) {
			m_fArmPosY = 24.f * 3.f;
		}


		// 8번째 프레임에서 공격 시작함
		if (!m_bAttack && m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt == 8)
		{
			// 공격
			m_bAttack = true;
			// 공격 간격
			m_dwShotDelay = 400;
			// 공격 횟수 초기화
			m_dwShotCnt = 0;
			// 프레임 고정
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameStart = 8;
		}

		// 공격
		if (m_bAttack &&
			m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			// 총알 카운트 증가
			++m_dwShotCnt;
			// 10번 쐈으면 멈춤
			if (m_dwShotCnt >= 10)
			{
				m_bAttack = false;
				// 프레임을 다시 처음으로 되돌아가야 함
				m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt = m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd - 1;
				m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameStart = m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd;
				m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd = 0;
			}

			// 좌,우 번갈아가면서 공격함
			if (m_dwShotCnt % 2)
			{
				// 오른쪽
				Back_Shot(37.f * 3.f, -15.f * 3.f);
			}
			else
			{
				// 왼쪽
				Back_Shot(-37.f * 3.f, -15.f * 3.f);
			}

			m_dwShotTime = GetTickCount();
		}

		// 공격 끝
		if (!m_bAttack &&
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt == m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd)
		{
			// 다음 패턴을 위해 초기화
			m_eCurState = STATE::NEXT_PATTERN;
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 1000;
			m_fArmPosY = 0.f;

			// 배기구 이펙트 출력
			Pipe_Smoke();
		}

		break;
		// 어깨 공격
	case STATE::SHOULDER_ATTACK:
		// 공격 도중 파괴 됬다면 중단해야됨
		if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].bDamage && m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].bDamage)
		{
			// 다음 패턴을 위해 초기화
			m_eCurState = STATE::NEXT_PATTERN;
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 1000;
			break;
		}

		// 처음 공격
		if (!m_bAttack)
		{
			// 공격 시작
			m_bAttack = true;
			// 현재 공격 시간 저장
			m_dwShotTime = GetTickCount();
			// 공격 딜레이
			m_dwShotDelay = 600;
			// 공격 횟수 초기화
			m_dwShotCnt = 0;
		}

		// 공격
		if (m_bAttack &&
			m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			// 총알 카운트 증가
			++m_dwShotCnt;
			// 2번 쐈으면 멈춤
			if (m_dwShotCnt >= 2)
			{
				m_bAttack = false;
			}

			Shoulder_Shot();
			m_dwShotTime = GetTickCount();
		}

		// 공격 끝
		if (!m_bAttack)
		{
			// 다음 패턴을 위해 초기화
			m_eCurState = STATE::NEXT_PATTERN;
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 1000;
		}
		break;
		// 파괴
	case STATE::DESTROY:
		if (m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			m_eCurState = STATE::FINAL_1;
			Destroy_Shot(0.f);
			Destroy_Shot(30.f);
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 100;
		}
		break;
		// FINAL_1 ~ FINAL_4 까지 파괴되면서 주위로 총알을 흩뿌림
	case STATE::FINAL_1:
		if (m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			m_eCurState = STATE::FINAL_2;
			Destroy_Shot(20.f);
			Destroy_Shot(50.f);
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 100;

			CObj* pObj = CObjFactory<CExplosion_04>::Create(m_tInfo.fX, m_tInfo.fY);		// 파괴 이펙트 생성
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			int irand = rand() % 3 + 1;
			pObj = CObjFactory<CExplosion_03>::Create(m_tInfo.fX - 165.f, m_tInfo.fY + 36.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(100 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CObjFactory<CExplosion_03>::Create(m_tInfo.fX + 165.f, m_tInfo.fY + 36.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(100 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			pObj = CObjFactory<CExplosion_02_1>::Create(m_tInfo.fX - 80.f, m_tInfo.fY);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(30 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CObjFactory<CExplosion_02_1>::Create(m_tInfo.fX + 80.f, m_tInfo.fY);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)50);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CObjFactory<CExplosion_01>::Create(m_tInfo.fX + irand * 30, m_tInfo.fY + 20.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(irand * 10));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CObjFactory<CExplosion_01>::Create(m_tInfo.fX - irand * 30, m_tInfo.fY + 50.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)100);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		}
		break;
	case STATE::FINAL_2:
		if (m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			m_eCurState = STATE::FINAL_3;
			Destroy_Shot(-15.f);
			Destroy_Shot(-45.f);
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 100;

			CObj* pObj = CObjFactory<CExplosion_04>::Create(m_tInfo.fX, m_tInfo.fY);		// 파괴 이펙트 생성
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			int irand = rand() % 3 + 1;
			pObj = CObjFactory<CExplosion_03>::Create(m_tInfo.fX - 165.f, m_tInfo.fY + 36.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(100 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CObjFactory<CExplosion_03>::Create(m_tInfo.fX + 165.f, m_tInfo.fY + 36.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(100 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			pObj = CObjFactory<CExplosion_02_1>::Create(m_tInfo.fX - 80.f, m_tInfo.fY);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(30 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CObjFactory<CExplosion_02_1>::Create(m_tInfo.fX + 80.f, m_tInfo.fY);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)50);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CObjFactory<CExplosion_01>::Create(m_tInfo.fX + irand * 30, m_tInfo.fY + 20.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(irand * 10));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CObjFactory<CExplosion_01>::Create(m_tInfo.fX - irand * 30, m_tInfo.fY + 50.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)100);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		}
		break;
	case STATE::FINAL_3:
		if (m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			m_eCurState = STATE::FINAL_4;
			Destroy_Shot(-5.f);
			Destroy_Shot(25.f);
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 100;

			CObj* pObj = CObjFactory<CExplosion_04>::Create(m_tInfo.fX, m_tInfo.fY);		// 파괴 이펙트 생성
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			int irand = rand() % 3 + 1;
			pObj = CObjFactory<CExplosion_03>::Create(m_tInfo.fX - 165.f, m_tInfo.fY + 36.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(100 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CObjFactory<CExplosion_03>::Create(m_tInfo.fX + 165.f, m_tInfo.fY + 36.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(100 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			pObj = CObjFactory<CExplosion_02_1>::Create(m_tInfo.fX - 80.f, m_tInfo.fY);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(30 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CObjFactory<CExplosion_02_1>::Create(m_tInfo.fX + 80.f, m_tInfo.fY);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)50);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CObjFactory<CExplosion_01>::Create(m_tInfo.fX + irand * 30, m_tInfo.fY + 20.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(irand * 10));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CObjFactory<CExplosion_01>::Create(m_tInfo.fX - irand * 30, m_tInfo.fY + 50.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)100);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		}
		break;
	case STATE::FINAL_4:
		if (m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			m_eCurState = STATE::FINAL_5;
			Destroy_Shot(5.f);
			Destroy_Shot(-25.f);
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 100;

			CObj* pObj = CObjFactory<CExplosion_04>::Create(m_tInfo.fX, m_tInfo.fY);		// 파괴 이펙트 생성
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			int irand = rand() % 3 + 1;
			pObj = CObjFactory<CExplosion_03>::Create(m_tInfo.fX - 165.f, m_tInfo.fY + 36.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(100 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CObjFactory<CExplosion_03>::Create(m_tInfo.fX + 165.f, m_tInfo.fY + 36.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(100 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			pObj = CObjFactory<CExplosion_02_1>::Create(m_tInfo.fX - 80.f, m_tInfo.fY);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(30 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CObjFactory<CExplosion_02_1>::Create(m_tInfo.fX + 80.f, m_tInfo.fY);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)50);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CObjFactory<CExplosion_01>::Create(m_tInfo.fX + irand * 30, m_tInfo.fY + 20.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(irand * 10));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CObjFactory<CExplosion_01>::Create(m_tInfo.fX - irand * 30, m_tInfo.fY + 50.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)100);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		}
		break;
	case STATE::FINAL_5:
		if (!g_bStopMapScroll)
		{
			// 맵 밖으로 퇴장
			m_tInfo.fY = m_tInfo.fY + 3.f;

			if (!g_bStageClear && m_tInfo.fY > 450.f) {
				// 스테이지 클리어 플래그 true
				g_bStageClear = true;
				// 클리어 사운드
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
				CSoundMgr::Get_Instance()->PlaySound(L"Stage_Clear.wav", CSoundMgr::BGM);
			}
		}
		break;
	}
}

// 이동
// 기본적으로 좌우로 반복 이동을 함
// 부위 파괴가 되면 반대 방향으로 빠르게 반동 이동 함
void CTequilaGallop::Move()
{
	// 바퀴 연기 이펙트 생성
	switch (m_ePreState)
	{
	case CTequilaGallop::STATE::WAIT:
	case CTequilaGallop::STATE::PHASE_MOVE:
		if (m_dwWhellSmokeTime + m_dwWhellSmokeDelay < GetTickCount()) {
			m_dwWhellSmokeTime = GetTickCount();

			// 왼쪽 위 바퀴
			CObj* pObj = CObjFactory<CWheelSmoke>::Create(m_tInfo.fX - (26.f * 3.f), m_tInfo.fY - (24.f * 3.f));
			pObj->Set_Speed(7.f);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::BACK_EFFECT);
			// 오른쪽 위 바퀴
			pObj = CObjFactory<CWheelSmoke>::Create(m_tInfo.fX + (26.f * 3.f), m_tInfo.fY - (24.f * 3.f));
			pObj->Set_Speed(7.f);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::BACK_EFFECT);
			// 왼쪽 아래 바퀴
			pObj = CObjFactory<CWheelSmoke>::Create(m_tInfo.fX - (26.f * 3.f), m_tInfo.fY + (76.f * 3.f));
			pObj->Set_Speed(7.f);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			// 오른쪽 아래 바퀴
			pObj = CObjFactory<CWheelSmoke>::Create(m_tInfo.fX + (26.f * 3.f), m_tInfo.fY + (76.f * 3.f));
			pObj->Set_Speed(7.f);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		}
		break;
	case CTequilaGallop::STATE::PHASE_CHANGE:
		m_dwWhellSmokeDelay = 40;
		break;
	case CTequilaGallop::STATE::READY:
	case CTequilaGallop::STATE::NEXT_PATTERN:
	case CTequilaGallop::STATE::L_ARM_ATTACK:
	case CTequilaGallop::STATE::R_ARM_ATTACK:
	case CTequilaGallop::STATE::FRONT_ATTACK:
	case CTequilaGallop::STATE::BACK_ATTACK:
	case CTequilaGallop::STATE::SHOULDER_ATTACK:
		// 이동 사운드
		if (m_dwSoundTime + m_dwSoundDelay < GetTickCount()) {
			m_dwSoundTime = GetTickCount();

			// 이동 사운드
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS);
			CSoundMgr::Get_Instance()->PlaySound(L"Boss_Wheel.wav", CSoundMgr::BOSS);
		}
	case CTequilaGallop::STATE::DESTROY:
	case CTequilaGallop::STATE::FINAL_1:
	case CTequilaGallop::STATE::FINAL_2:
	case CTequilaGallop::STATE::FINAL_3:
	case CTequilaGallop::STATE::FINAL_4:
	case CTequilaGallop::STATE::FINAL_5:
		if (m_dwWhellSmokeTime + m_dwWhellSmokeDelay < GetTickCount()) {
			m_dwWhellSmokeTime = GetTickCount();

			// 왼쪽 위 바퀴
			CObj* pObj = CObjFactory<CWheelSmoke>::Create(m_tInfo.fX - (60.f * 3.f), m_tInfo.fY - (2.f * 3.f));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::BACK_EFFECT);
			// 오른쪽 위 바퀴
			pObj = CObjFactory<CWheelSmoke>::Create(m_tInfo.fX + (60.f * 3.f), m_tInfo.fY - (2.f * 3.f));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::BACK_EFFECT);
			// 왼쪽 아래 바퀴
			pObj = CObjFactory<CWheelSmoke>::Create(m_tInfo.fX - (40.f * 3.f), m_tInfo.fY + (70.f * 3.f));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			// 오른쪽 아래 바퀴
			pObj = CObjFactory<CWheelSmoke>::Create(m_tInfo.fX + (40.f * 3.f), m_tInfo.fY + (70.f * 3.f));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		}
		break;
	}

	if (m_bHpLock || m_tInfo.fY < 200.f)
		return;

	// 부위 파괴 이동 검사
	if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].bDamage && !m_bDamageRightMove)
	{
		m_bDamageRightMove = true;
		m_fSpeed = 4.f;
	}
	else if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].bDamage && !m_bDamageLeftMove)
	{
		m_bDamageLeftMove = true;
		m_fSpeed = -4.f;
	}

	// 좌우 이동 결정
	if (m_fSpeed == 0)
	{
		// 오른쪽 이동
		if (rand() % 2)
		{
			m_fSpeed = 1.f;
		}
		// 왼쪽 이동
		else
		{
			m_fSpeed = -1.f;
		}
	}
	// 오른쪽 이동
	else if (m_fSpeed >= 1.f)
	{
		// 오른쪽 끝에 다왔으면 방향 전환
		if (m_tInfo.fX + m_fSpeed >= WINCX - 200.f)
		{
			// 도착
			if (!m_bMoveFlag)
			{
				m_bMoveFlag = true;
				m_dwMoveTime = GetTickCount();
			}
			else
			{
				if (m_dwMoveTime + m_dwMoveDelay < GetTickCount())
				{
					m_fSpeed = -1.f;
					m_bMoveFlag = false;
				}
			}
		}
	}
	// 왼쪽 이동
	else if (m_fSpeed <= -1.f)
	{
		// 왼쪽 끝에 다왔으면 방향 전환
		if (m_tInfo.fX + m_fSpeed <= 200.f)
		{
			// 도착
			if (!m_bMoveFlag)
			{
				m_bMoveFlag = true;
				m_dwMoveTime = GetTickCount();
			}
			else
			{
				if (m_dwMoveTime + m_dwMoveDelay < GetTickCount())
				{
					m_fSpeed = 1.f;
					m_bMoveFlag = false;
				}
			}
		}
	}
	// 이동
	float fMovePosX = m_tInfo.fX + m_fSpeed;
	if (200.f < fMovePosX && fMovePosX < WINCX - 200.f)
	{
		m_tInfo.fX = m_tInfo.fX + m_fSpeed;
	}
}

// 파괴 이펙트
void CTequilaGallop::DamageEffect()
{
	CObj* pObj;

	if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].bDamage && m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].iEffectCnt != 0)
	{
		if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].dwTime + m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].dwDelay < GetTickCount())
		{
			--m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].iEffectCnt;

			// 이펙트 생성
			pObj = CObjFactory<CExplosion_05>::Create(m_tInfo.fX - (30.f * 3.f), m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::FRONT_EFFECT);

			m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].dwTime = GetTickCount();
		}
	}

	if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].bDamage && m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].iEffectCnt != 0)
	{
		if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].dwTime + m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].dwDelay < GetTickCount())
		{
			--m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].iEffectCnt;

			// 이펙트 생성
			pObj = CObjFactory<CExplosion_05>::Create(m_tInfo.fX + (30.f * 3.f), m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::FRONT_EFFECT);

			m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].dwTime = GetTickCount();
		}
	}
}

// 상체 흔들기
// 주기적으로 흔들림
void CTequilaGallop::Shake_Body()
{
	if (m_bHpLock)
		return;

	if (m_dwShakeTime + m_dwShakeDelay < GetTickCount())
	{
		if (m_fBodyPosY == 0.f)
		{
			m_fBodyPosY = 2.f * 3.f;
			m_dwShakeTime = GetTickCount();
		}
		else if (m_fBodyPosY == 2.f * 3.f)
		{
			m_fBodyPosY = 0.f;
			m_dwShakeTime = GetTickCount();
		}
	}
}

// 배기구 이펙트 출력
void CTequilaGallop::Pipe_Smoke()
{
	// 왼쪽
	CObj* pObj = CObjFactory<CPipeSmoke>::Create(m_tInfo.fX - (16.f * 3.f), m_tInfo.fY - (66.f * 3.f));
	pObj->Set_Target(this);
	static_cast<CEffect*>(pObj)->Set_RelativePos(-16.f * 3.f, -66.f * 3.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
	// 오른쪽
	pObj = CObjFactory<CPipeSmoke>::Create(m_tInfo.fX + (16.f * 3.f), m_tInfo.fY - (66.f * 3.f));
	pObj->Set_Target(this);
	pObj->Set_SceneID(1);
	static_cast<CEffect*>(pObj)->Set_RelativePos(16.f * 3.f, -66.f * 3.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
}

// 왼팔 공격
void CTequilaGallop::L_Arm_Shot()
{
	CObj* pObj = CObjFactory<CBullet_2>::Create(m_tInfo.fX - (29.f * 3.f), m_tInfo.fY + (15.f * 3.f));
	pObj->Set_Angle(-30.f - m_fShotAngle);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CObjFactory<CBullet_2>::Create(m_tInfo.fX - (29.f * 3.f), m_tInfo.fY + (15.f * 3.f));
	pObj->Set_Angle(-150.f - m_fShotAngle);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CObjFactory<CBullet_2>::Create(m_tInfo.fX - (29.f * 3.f), m_tInfo.fY + (15.f * 3.f));
	pObj->Set_Angle(50.f - m_fShotAngle);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CObjFactory<CBullet_2>::Create(m_tInfo.fX - (29.f * 3.f), m_tInfo.fY + (15.f * 3.f));
	pObj->Set_Angle(130.f - m_fShotAngle);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
}

// 오른팔 공격
void CTequilaGallop::R_Arm_Shot()
{
	CObj* pObj = CObjFactory<CBullet_2>::Create(m_tInfo.fX + (29.f * 3.f), m_tInfo.fY + (15.f * 3.f));
	pObj->Set_Angle(-30.f - m_fShotAngle);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CObjFactory<CBullet_2>::Create(m_tInfo.fX + (29.f * 3.f), m_tInfo.fY + (15.f * 3.f));
	pObj->Set_Angle(-150.f - m_fShotAngle);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CObjFactory<CBullet_2>::Create(m_tInfo.fX + (29.f * 3.f), m_tInfo.fY + (15.f * 3.f));
	pObj->Set_Angle(50.f - m_fShotAngle);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CObjFactory<CBullet_2>::Create(m_tInfo.fX + (29.f * 3.f), m_tInfo.fY + (15.f * 3.f));
	pObj->Set_Angle(130.f - m_fShotAngle);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
}

// 가슴 공격
void CTequilaGallop::Front_Shot(float _fX, float _fY)
{
	float	fX = m_pTarget->Get_Info().fX - m_tInfo.fX;
	float	fY = m_pTarget->Get_Info().fY - m_tInfo.fY;
	float	fDia = sqrtf(fX * fX + fY * fY);
	float	fRad = acosf(fX / fDia);

	float fAngle = fRad * 180.f / PI;

	if (m_tInfo.fY < m_pTarget->Get_Info().fY)
		fAngle *= -1.f;

	CObj* pObj = CObjFactory<CBullet_1>::Create(m_tInfo.fX + _fX, m_tInfo.fY + _fY);
	pObj->Set_Angle(fAngle - 20.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CObjFactory<CBullet_1>::Create(m_tInfo.fX + _fX, m_tInfo.fY + _fY);
	pObj->Set_Angle(fAngle);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CObjFactory<CBullet_1>::Create(m_tInfo.fX + _fX, m_tInfo.fY + _fY);
	pObj->Set_Angle(fAngle + 20.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

}

// 등 공격
void CTequilaGallop::Back_Shot(float _fX, float _fY)
{
	CObj* pObj = CObjFactory<CBullet_3>::Create(m_tInfo.fX + _fX, m_tInfo.fY + _fY);
	pObj->Set_Angle(-50.f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(50);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CObjFactory<CBullet_3>::Create(m_tInfo.fX + _fX, m_tInfo.fY + _fY);
	pObj->Set_Angle(-80.f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(100);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CObjFactory<CBullet_3>::Create(m_tInfo.fX + _fX, m_tInfo.fY + _fY);
	pObj->Set_Angle(-110.f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(150);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CObjFactory<CBullet_3>::Create(m_tInfo.fX + _fX, m_tInfo.fY + _fY);
	pObj->Set_Angle(-140.f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(200);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);



	pObj = CObjFactory<CBullet_3>::Create(m_tInfo.fX + _fX, m_tInfo.fY + _fY);
	pObj->Set_Angle(180.f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(50);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CObjFactory<CBullet_3>::Create(m_tInfo.fX + _fX, m_tInfo.fY + _fY);
	pObj->Set_Angle(160.f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(100);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CObjFactory<CBullet_3>::Create(m_tInfo.fX + _fX, m_tInfo.fY + _fY);
	pObj->Set_Angle(140.f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(150);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CObjFactory<CBullet_3>::Create(m_tInfo.fX + _fX, m_tInfo.fY + _fY);
	pObj->Set_Angle(120.f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(200);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);



	pObj = CObjFactory<CBullet_3>::Create(m_tInfo.fX + _fX, m_tInfo.fY + _fY);
	pObj->Set_Angle(60.f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(50);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CObjFactory<CBullet_3>::Create(m_tInfo.fX + _fX, m_tInfo.fY + _fY);
	pObj->Set_Angle(40.f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(100);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CObjFactory<CBullet_3>::Create(m_tInfo.fX + _fX, m_tInfo.fY + _fY);
	pObj->Set_Angle(20.f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(150);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CObjFactory<CBullet_3>::Create(m_tInfo.fX + _fX, m_tInfo.fY + _fY);
	pObj->Set_Angle(0.f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(200);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
}

// 어깨 공격
void CTequilaGallop::Shoulder_Shot()
{
	CObj* pObj;

	// 왼쪽 팔이 파괴되지 않았으면 공격
	if (!m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].bDamage)
	{
		// 왼쪽 1
		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX - (30.f * 3.f), m_tInfo.fY + (12.f * 3.f));
		pObj->Set_Angle(-160.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(50);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX - (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(-169.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(100);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX - (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(-178.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(150);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX - (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(-187.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(200);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX - (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(-196.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(250);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		// 왼쪽 2
		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX - (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(-70.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(50);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX - (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(-79.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(100);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX - (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(-88.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(150);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX - (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(-97.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(200);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX - (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(-106.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(250);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		// 왼쪽 3
		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX - (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(20.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(50);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX - (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(11.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(100);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX - (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(2.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(150);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX - (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(-7.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(200);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX - (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(-16.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(250);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		// 왼쪽 4
		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX - (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(110.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(50);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX - (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(101.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(100);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX - (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(92.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(150);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX - (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(83.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(200);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX - (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(74.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(250);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
	}

	

	// 오른쪽 팔이 파괴되지 않았으면 공격
	if (!m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].bDamage)
	{
		// 오른쪽 1
		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX + (30.f * 3.f), m_tInfo.fY + (12.f * 3.f));
		pObj->Set_Angle(-20.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(50);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX + (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(-11.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(100);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX + (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(-2.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(150);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX + (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(7.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(200);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX + (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(16.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(250);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		// 오른쪽 2
		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX + (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(-110.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(50);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX + (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(-101.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(100);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX + (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(-92.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(150);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX + (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(-83.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(200);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX + (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(-74.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(250);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		// 오른쪽 3
		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX + (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(160.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(50);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX + (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(169.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(100);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX + (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(178.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(150);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX + (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(187.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(200);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX + (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(196.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(250);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		// 오른쪽 4
		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX + (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(70.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(50);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX + (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(79.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(100);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX + (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(88.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(150);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX + (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(97.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(200);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX + (30.f * 3.f), m_tInfo.fY - (12.f * 3.f));
		pObj->Set_Angle(106.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(250);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
	}
}

// 파괴된 후 공격
void CTequilaGallop::Destroy_Shot(float _fAngle)
{
	float fCnt = _fAngle;

	while (fCnt < 180.f)
	{
		CObj* pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX, m_tInfo.fY);
		pObj->Set_Angle(fCnt);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		fCnt = fCnt + 25.f;
	}

	fCnt = -180.f + _fAngle;
	while (fCnt < 0.f)
	{
		CObj* pObj = CObjFactory<CBullet_4>::Create(m_tInfo.fX, m_tInfo.fY);
		pObj->Set_Angle(fCnt);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		fCnt = fCnt + 25.f;
	}
}

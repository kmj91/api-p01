#include "stdafx.h"
#include "TequilaGround.h"

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
#include "Effect_Charge.h"
#include "Explosion_05.h"

#include "TequilaGallop.h"


CTequilaGround::CTequilaGround()
	: m_ePreState(STATE::END), m_eCurState(STATE::END)
	, m_fShotAngle(0.f)
	, m_dwDestroyTime(0), m_dwDestroyDelay(200), m_dwMoveTime(0), m_dwMoveDelay(500)
	, m_tPatternDelay{}, m_tCannonFrameArr{}, m_bDamageFlagArr{}
	, m_bMoveFlag(false)
{
}


CTequilaGround::~CTequilaGround()
{
	Release();
}

void CTequilaGround::Initialize()
{
	m_tInfo.iCX = TEQUILA_GROUND_WIDTH * 3;
	m_tInfo.iCY = TEQUILA_GROUND_HEIGHT * 3;
	m_iImageWidth = TEQUILA_GROUND_WIDTH;
	m_iImageHeight = TEQUILA_GROUND_HEIGHT;

	m_fHp = 4000;
	//m_fHp = 100;

	m_vecHitRectPos.reserve(5);
	m_vecHitRectPos.emplace_back(RECT{ 72 * 3, 117 * 3, 108 * 3, 177 * 3 });		// 인덱스 0번 왼쪽 서브 1
	m_vecHitRectPos.emplace_back(RECT{ 108 * 3, 117 * 3, 144 * 3, 177 * 3 });		// 인덱스 1번 오른쪽 서브 1
	m_vecHitRectPos.emplace_back(RECT{ 20 * 3, 66 * 3, 72 * 3, 126 * 3 });			// 인덱스 2번 왼쪽 서브 2
	m_vecHitRectPos.emplace_back(RECT{ 144 * 3, 66 * 3, 196 * 3, 126 * 3 });		// 인덱스 3번 오른쪽 서브 2
	m_vecHitRectPos.emplace_back(RECT{ 72 * 3, 45 * 3, 144 * 3, 117 * 3 });			// 인덱스 4번 뒤쪽 서브 3
	m_vecHitRect.reserve(5);
	m_vecHitRect.emplace_back(RECT{});
	m_vecHitRect.emplace_back(RECT{});
	m_vecHitRect.emplace_back(RECT{});
	m_vecHitRect.emplace_back(RECT{});
	m_vecHitRect.emplace_back(RECT{});
	m_vecRectHp.emplace_back(1000.f);
	m_vecRectHp.emplace_back(1000.f);
	m_vecRectHp.emplace_back(1000.f);
	m_vecRectHp.emplace_back(1000.f);
	m_vecRectHp.emplace_back(1000.f);

	m_eCurState = STATE::WAIT;

	// 패턴 딜레이
	m_tPatternDelay[static_cast<UINT>(PATTERN::IDLE)].dwOnDelay = 1000;
	m_tPatternDelay[static_cast<UINT>(PATTERN::IDLE)].dwOffDelay = 200;
	m_tPatternDelay[static_cast<UINT>(PATTERN::L_SUB)].dwOnDelay = 3000;
	m_tPatternDelay[static_cast<UINT>(PATTERN::L_SUB)].dwOffDelay = 200;
	m_tPatternDelay[static_cast<UINT>(PATTERN::R_SUB)].dwOnDelay = 3000;
	m_tPatternDelay[static_cast<UINT>(PATTERN::R_SUB)].dwOffDelay = 200;
	m_tPatternDelay[static_cast<UINT>(PATTERN::LL_SUB)].dwOnDelay = 3000;
	m_tPatternDelay[static_cast<UINT>(PATTERN::LL_SUB)].dwOffDelay = 200;
	m_tPatternDelay[static_cast<UINT>(PATTERN::RR_SUB)].dwOnDelay = 3000;
	m_tPatternDelay[static_cast<UINT>(PATTERN::RR_SUB)].dwOffDelay = 200;
	m_tPatternDelay[static_cast<UINT>(PATTERN::FRONT)].dwOnDelay = 8000;
	m_tPatternDelay[static_cast<UINT>(PATTERN::FRONT)].dwOffDelay = 1000;
	m_tPatternDelay[static_cast<UINT>(PATTERN::BACK)].dwOnDelay = 3000;
	m_tPatternDelay[static_cast<UINT>(PATTERN::BACK)].dwOffDelay = 200;

	m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].iEffectCnt = 20;
	m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].dwDelay = 50;
	m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].iEffectCnt = 20;
	m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].dwDelay = 50;
	m_bDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].iEffectCnt = 20;
	m_bDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].dwDelay = 50;
	m_bDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].iEffectCnt = 20;
	m_bDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].dwDelay = 50;
	m_bDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iEffectCnt = 20;
	m_bDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].dwDelay = 50;

	// 점수
	m_iScore = 5000;
}

int CTequilaGround::Update()
{
	if (m_bRemove)
		return OBJ_DEAD;

	if (m_bDead)
	{
		if (!m_bDestroy)
		{
			if (m_ePreState != STATE::DESTROY)
			{
				m_eCurState = STATE::DESTROY;
				m_bHpLock = true;		// 파괴되서 HP 다시 락
				m_bDestroy = true;

				// 2페이즈 보스 생성
				CObj* pObj = CAbstractFactory<CTequilaGallop>::Create(m_tInfo.fX, m_tInfo.fY);
				pObj->Set_Target(m_pTarget);
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::GROUND_BOSS, false);

				// 파괴 이펙트 생성
				pObj = CAbstractFactory<CExplosion_04>::Create(m_tInfo.fX, m_tInfo.fY);
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

				int irand = rand() % 3 + 1;
				pObj = CAbstractFactory<CExplosion_03>::Create(m_tInfo.fX - (55.f * 3.f), m_tInfo.fY + (12.f * 3.f));
				static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(100 + irand * 50));
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
				pObj = CAbstractFactory<CExplosion_03>::Create(m_tInfo.fX + (55.f * 3.f), m_tInfo.fY + (12.f * 3.f));
				static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(100 + irand * 50));
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

				pObj = CAbstractFactory<CExplosion_02_1>::Create(m_tInfo.fX - (27.f * 3.f), m_tInfo.fY);
				static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(30 + irand * 50));
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
				pObj = CAbstractFactory<CExplosion_02_1>::Create(m_tInfo.fX + (27.f * 3.f), m_tInfo.fY);
				static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)50);
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

				// 공격중에 죽을경우 다음 공격상태로 전환되면서 죽는 경우가 있음
				// 그래서 상태 바로 전환
				Scene_Change();
			}
		}	
	}

	// 보스 행동
	Action();
	// 이동
	Move();
	// 파괴 이펙트;
	DamageEffect();
	Update_Rect();
	Scene_Change();
	Frame_Move();
	Frame_MoveEX();

	return OBJ_NOEVENT;
}

void CTequilaGround::Late_Update()
{
	if (WINCY <= m_tRect.top)
		m_bRemove = true;
}

void CTequilaGround::Render(HDC _DC)
{
	HDC hMemDC;

	switch (m_ePreState)
	{
	case STATE::WAIT:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TequilaGround_CloseDoor");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
		break;
	case STATE::CLOSE_DOOR:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TequilaGround_CloseDoor");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
		break;
	case STATE::PAVED_ROAD:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TequilaGround_PavedRoad");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
		break;
	case STATE::IDLE:
	case STATE::ATTACK:
		// 몸통
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TequilaGround_Idle");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));

		// 캐논 1
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TequilaGround_SubCannon_1");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameCnt * m_iImageWidth, m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameCnt * m_iImageWidth, m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));

		// 캐논 2
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TequilaGround_SubCannon_2");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameCnt * m_iImageWidth, m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameCnt * m_iImageWidth, m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));

		// 캐논 3
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TequilaGround_SubCannon_3");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameCnt * m_iImageWidth, m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
		break;
	case STATE::DESTROY:
	case STATE::FINAL_1:
	case STATE::FINAL_2:
	case STATE::FINAL_3:
	case STATE::FINAL_4:
	case STATE::FINAL_5:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TequilaGround_Destroy");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, 0, 0
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
		break;
	}


	// 충돌 박스
	// 충돌 박스
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

void CTequilaGround::Release()
{
}

// 보스 부위별 파괴 데미지 체크
void CTequilaGround::Check_Damage(int _iPlayerNum)
{
	CObj* pObj;
	LONG lCenterX;
	LONG lCenterY;

	// 왼쪽 서브 1
	if (!m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].bDamage && m_vecRectHp[static_cast<UINT>(SPRITE::L_CANNON)] <= 0) {
		m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].bDamage = true;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameCnt = 8;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameStart = 8;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameEnd = 8;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameScene = 0;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].dwFrameTime = GetTickCount();
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].dwFrameSpeed = 999999;

		lCenterX = (m_vecHitRect[static_cast<UINT>(SPRITE::L_CANNON)].right - m_vecHitRect[static_cast<UINT>(SPRITE::L_CANNON)].left) >> 1;
		lCenterY = (m_vecHitRect[static_cast<UINT>(SPRITE::L_CANNON)].bottom - m_vecHitRect[static_cast<UINT>(SPRITE::L_CANNON)].top) >> 1;

		pObj = CAbstractFactory<CExplosion_02_1>::Create(
			static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::L_CANNON)].left + lCenterX),
			static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::L_CANNON)].top + lCenterY));
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		// 부위 파괴 점수 획득
		CGameUIMgr::Get_Instance()->Add_Score(2000, _iPlayerNum);
	}

	// 오른쪽 서브 1
	if (!m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].bDamage && m_vecRectHp[static_cast<UINT>(SPRITE::R_CANNON)] <= 0) {
		m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].bDamage = true;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameCnt = 8;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameStart = 8;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameEnd = 8;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameScene = 1;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].dwFrameTime = GetTickCount();
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].dwFrameSpeed = 999999;

		lCenterX = (m_vecHitRect[static_cast<UINT>(SPRITE::R_CANNON)].right - m_vecHitRect[static_cast<UINT>(SPRITE::R_CANNON)].left) >> 1;
		lCenterY = (m_vecHitRect[static_cast<UINT>(SPRITE::R_CANNON)].bottom - m_vecHitRect[static_cast<UINT>(SPRITE::R_CANNON)].top) >> 1;

		pObj = CAbstractFactory<CExplosion_02_1>::Create(
			static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::R_CANNON)].left + lCenterX),
			static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::R_CANNON)].top + lCenterY));
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		// 부위 파괴 점수 획득
		CGameUIMgr::Get_Instance()->Add_Score(2000, _iPlayerNum);
	}

	// 왼쪽 서브 2
	if (!m_bDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].bDamage && m_vecRectHp[static_cast<UINT>(SPRITE::LL_CANNON)] <= 0) {
		m_bDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].bDamage = true;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameCnt = 10;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameStart = 10;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameEnd = 10;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameScene = 0;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].dwFrameTime = GetTickCount();
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].dwFrameSpeed = 999999;

		lCenterX = (m_vecHitRect[static_cast<UINT>(SPRITE::LL_CANNON)].right - m_vecHitRect[static_cast<UINT>(SPRITE::LL_CANNON)].left) >> 1;
		lCenterY = (m_vecHitRect[static_cast<UINT>(SPRITE::LL_CANNON)].bottom - m_vecHitRect[static_cast<UINT>(SPRITE::LL_CANNON)].top) >> 1;

		pObj = CAbstractFactory<CExplosion_02_1>::Create(
			static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::LL_CANNON)].left + lCenterX),
			static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::LL_CANNON)].top + lCenterY));
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		// 부위 파괴 점수 획득
		CGameUIMgr::Get_Instance()->Add_Score(2000, _iPlayerNum);
	}

	// 오른쪽 서브 2
	if (!m_bDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].bDamage && m_vecRectHp[static_cast<UINT>(SPRITE::RR_CANNON)] <= 0) {
		m_bDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].bDamage = true;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameCnt = 10;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameStart = 10;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameEnd = 10;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameScene = 1;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].dwFrameTime = GetTickCount();
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].dwFrameSpeed = 999999;

		lCenterX = (m_vecHitRect[static_cast<UINT>(SPRITE::RR_CANNON)].right - m_vecHitRect[static_cast<UINT>(SPRITE::RR_CANNON)].left) >> 1;
		lCenterY = (m_vecHitRect[static_cast<UINT>(SPRITE::RR_CANNON)].bottom - m_vecHitRect[static_cast<UINT>(SPRITE::RR_CANNON)].top) >> 1;

		pObj = CAbstractFactory<CExplosion_02_1>::Create(
			static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::RR_CANNON)].left + lCenterX),
			static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::RR_CANNON)].top + lCenterY));
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		// 부위 파괴 점수 획득
		CGameUIMgr::Get_Instance()->Add_Score(2000, _iPlayerNum);
	}

	// 뒤쪽 서브 3
	if (!m_bDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].bDamage && m_vecRectHp[static_cast<UINT>(SPRITE::CENTER_CANNON)] <= 0) {
		m_bDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].bDamage = true;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameCnt = 13;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameStart = 13;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameEnd = 13;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameScene = 0;
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].dwFrameTime = GetTickCount();
		m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].dwFrameSpeed = 999999;

		lCenterX = (m_vecHitRect[static_cast<UINT>(SPRITE::CENTER_CANNON)].right - m_vecHitRect[static_cast<UINT>(SPRITE::CENTER_CANNON)].left) >> 1;
		lCenterY = (m_vecHitRect[static_cast<UINT>(SPRITE::CENTER_CANNON)].bottom - m_vecHitRect[static_cast<UINT>(SPRITE::CENTER_CANNON)].top) >> 1;

		pObj = CAbstractFactory<CExplosion_02_1>::Create(
			static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::CENTER_CANNON)].left + lCenterX),
			static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::CENTER_CANNON)].top + lCenterY));
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		// 부위 파괴 점수 획득
		CGameUIMgr::Get_Instance()->Add_Score(5000, _iPlayerNum);
	}
}

void CTequilaGround::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case STATE::WAIT:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 999999;
			break;
		case STATE::CLOSE_DOOR:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			break;
		case STATE::PAVED_ROAD:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 20;
			break;
		case STATE::IDLE:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 20;

			m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameCnt = 0;
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameStart = 0;
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameEnd = 0;
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameScene = 0;
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].dwFrameTime = GetTickCount();
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].dwFrameSpeed = 999999;

			m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameCnt = 0;
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameStart = 0;
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameEnd = 0;
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameScene = 1;
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].dwFrameTime = GetTickCount();
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].dwFrameSpeed = 999999;

			m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameCnt = 0;
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameStart = 0;
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameEnd = 0;
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameScene = 0;
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].dwFrameTime = GetTickCount();
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].dwFrameSpeed = 999999;

			m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameCnt = 0;
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameStart = 0;
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameEnd = 0;
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameScene = 1;
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].dwFrameTime = GetTickCount();
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].dwFrameSpeed = 999999;

			m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameCnt = 0;
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameStart = 0;
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameEnd = 0;
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameScene = 0;
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].dwFrameTime = GetTickCount();
			m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].dwFrameSpeed = 999999;
			break;
		case STATE::ATTACK:
			break;
			// 양옆 서브 캐논 앞쪽 캐논 공격
		//case STATE::SIDE_SUB_CANNON_FRONT_CANNON:
		//	if (!m_bDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)]) {
		//		m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameCnt = 0;
		//		m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameStart = 0;
		//		m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameEnd = 8;
		//		m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameScene = 0;
		//		m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].dwFrameTime = GetTickCount();
		//		m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].dwFrameSpeed = 60;
		//	}

		//	if (!m_bDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)]) {
		//		m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameCnt = 0;
		//		m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameStart = 0;
		//		m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameEnd = 8;
		//		m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameScene = 1;
		//		m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].dwFrameTime = GetTickCount();
		//		m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].dwFrameSpeed = 60;
		//	}

		//	{

		//	}
		//	break;
		case STATE::DESTROY:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

// 부분 스프라이트 캐논 프레임 업데이트
void CTequilaGround::Frame_MoveEX()
{
	for (int iCnt = 0; iCnt < static_cast<UINT>(SPRITE::END); ++iCnt)
	{
		if (m_tCannonFrameArr[iCnt].iFrameStart <= m_tCannonFrameArr[iCnt].iFrameEnd)
		{
			if (m_tCannonFrameArr[iCnt].dwFrameTime + m_tCannonFrameArr[iCnt].dwFrameSpeed < GetTickCount())
			{
				++m_tCannonFrameArr[iCnt].iFrameCnt;
				if (m_tCannonFrameArr[iCnt].iFrameCnt > m_tCannonFrameArr[iCnt].iFrameEnd)
					m_tCannonFrameArr[iCnt].iFrameCnt = m_tCannonFrameArr[iCnt].iFrameStart;

				m_tCannonFrameArr[iCnt].dwFrameTime = GetTickCount();
			}
		}
		else
		{
			if (m_tCannonFrameArr[iCnt].dwFrameTime + m_tCannonFrameArr[iCnt].dwFrameSpeed < GetTickCount())
			{
				--m_tCannonFrameArr[iCnt].iFrameCnt;
				if (m_tCannonFrameArr[iCnt].iFrameCnt < m_tCannonFrameArr[iCnt].iFrameEnd)
					m_tCannonFrameArr[iCnt].iFrameCnt = m_tCannonFrameArr[iCnt].iFrameEnd;

				m_tCannonFrameArr[iCnt].dwFrameTime = GetTickCount();
			}
		}
	}
}

// 보스 행동
void CTequilaGround::Action()
{
	switch (m_ePreState)
	{
	case STATE::WAIT:
		if (!g_bStopMapScroll)
		{
			m_tInfo.fY += g_fBackgroundSpeed * 3.f;			// 배경에 맞춰서 Y축 이동
		}

		if (g_bBossGO)
		{
			m_eCurState = STATE::CLOSE_DOOR;
		}
		break;
	case STATE::CLOSE_DOOR:
		if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		{
			m_eCurState = STATE::PAVED_ROAD;
			// 배경 스크롤 빠르게 변경
			g_fBackgroundSpeed = 2.f;
		}
		break;
	case STATE::PAVED_ROAD:
		if (m_tInfo.fY > -300.f)
		{
			m_tInfo.fY -= 4.f;
		}
		else
		{
			m_eCurState = STATE::IDLE;
		}
		break;
	case STATE::IDLE:
		if (m_bHpLock)
		{
			if (m_tInfo.fY >= 0)
			{
				m_bHpLock = false;		// 이제부터 처맞을 수 있음
			}
		}

		if (m_tInfo.fY < 300.f)
		{
			m_tInfo.fY += 2.f;
			m_tPatternDelay[static_cast<UINT>(PATTERN::BACK)].dwOnTime = GetTickCount();
		}
		else
		{
			if (m_tPatternDelay[static_cast<UINT>(PATTERN::IDLE)].dwOnTime + m_tPatternDelay[static_cast<UINT>(PATTERN::IDLE)].dwOnDelay < GetTickCount())
			{
				m_eCurState = STATE::ATTACK;
				m_tPatternDelay[static_cast<UINT>(PATTERN::IDLE)].dwOnTime = GetTickCount();
			}
		}

		break;
		// 공격
	case STATE::ATTACK:
		for (int iCnt = static_cast<int>(PATTERN::L_SUB); iCnt < static_cast<int>(PATTERN::END); ++iCnt)
		{
			if (m_tPatternDelay[iCnt].dwOnTime + m_tPatternDelay[iCnt].dwOnDelay < GetTickCount())
			{
				switch (static_cast<PATTERN>(iCnt))
				{
					// 정면 왼쪽 서브
				case PATTERN::L_SUB:
					// 옆쪽 서브 캐논이 둘다 파괴되어야 나옴
					if ((!m_bDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].bDamage || !m_bDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].bDamage) &&
						!m_bDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].bDamage) {
						break;
					}

					// 파괴됨
					if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].bDamage) {
						break;
					}

					// 첫 진입
					// 스프라이트 프레임 이동
					if (!m_tPatternDelay[iCnt].bOnFlag) {
						m_tPatternDelay[iCnt].bOnFlag = true;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameCnt = 0;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameStart = 0;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameEnd = 6;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameScene = 0;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].dwFrameTime = GetTickCount();
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].dwFrameSpeed = 60;
					}
					else {
						// 두번째 진입
						// 종료중이 아니면
						if (!m_tPatternDelay[iCnt].bOffFlag) {
							// 8번째 프레임에서 공격
							if (m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameCnt == 6) {
								if (!m_tPatternDelay[iCnt].bAttack) {
									m_tPatternDelay[iCnt].bAttack = true;
									m_tPatternDelay[iCnt].dwOffTime = GetTickCount();
									// 스프라이트 프레임 고정
									m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameStart = 6;
								}

								// 공격
								if (m_tPatternDelay[iCnt].bAttack &&
									m_tPatternDelay[iCnt].dwOffTime + m_tPatternDelay[iCnt].dwOffDelay < GetTickCount()) {
									// 정면 왼쪽 공격
									L_FrontSub_Shot();

									// 종료
									m_tPatternDelay[iCnt].bOffFlag = true;
									m_tPatternDelay[iCnt].dwOffTime = GetTickCount();
								}
							}
						}
						// 종료중
						else {
							// 세번째 진입
							// 공격 종료
							if (m_tPatternDelay[iCnt].bAttack &&
								m_tPatternDelay[iCnt].dwOffTime + m_tPatternDelay[iCnt].dwOffDelay < GetTickCount()) {
								m_tPatternDelay[iCnt].bAttack = false;
								// 스프라이트 프레임 되감기
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameCnt = 5;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameStart = 5;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameEnd = 0;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameScene = 0;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].dwFrameTime = GetTickCount();
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].dwFrameSpeed = 60;
							}
							// 네번째 진입
							else {
								// 되감기 완료
								if (m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameCnt == 0) {
									m_tPatternDelay[iCnt].bOnFlag = false;
									m_tPatternDelay[iCnt].bOffFlag = false;
									m_tPatternDelay[iCnt].dwOnTime = GetTickCount();
									// 2000 ~ 5000 값 랜덤
									m_tPatternDelay[iCnt].dwOnDelay = 2000 + 100 * (rand() % 31);
								}
							}
						}
					}
					break;
					// 정면 오른쪽 서브
				case PATTERN::R_SUB:
					// 옆쪽 서브 캐논 둘 혹은 뒤쪽 대형 캐논이 파괴되면 나옴
					if ((!m_bDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].bDamage || !m_bDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].bDamage) &&
						!m_bDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].bDamage) {
						break;
					}

					// 파괴됨
					if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].bDamage) {
						break;
					}

					// 첫 진입
					// 스프라이트 프레임 이동
					if (!m_tPatternDelay[iCnt].bOnFlag) {
						m_tPatternDelay[iCnt].bOnFlag = true;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameCnt = 0;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameStart = 0;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameEnd = 6;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameScene = 1;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].dwFrameTime = GetTickCount();
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].dwFrameSpeed = 60;
					}
					else {
						// 두번째 진입
						// 종료중이 아니면
						if (!m_tPatternDelay[iCnt].bOffFlag) {
							// 8번째 프레임에서 공격
							if (m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameCnt == 6) {
								if (!m_tPatternDelay[iCnt].bAttack) {
									m_tPatternDelay[iCnt].bAttack = true;
									m_tPatternDelay[iCnt].dwOffTime = GetTickCount();
									// 스프라이트 프레임 고정
									m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameStart = 6;
								}

								// 공격
								if (m_tPatternDelay[iCnt].bAttack &&
									m_tPatternDelay[iCnt].dwOffTime + m_tPatternDelay[iCnt].dwOffDelay < GetTickCount()) {
									// 정면 오른쪽 공격
									R_FrontSub_Shot();

									// 종료
									m_tPatternDelay[iCnt].bOffFlag = true;
									m_tPatternDelay[iCnt].dwOffTime = GetTickCount();
								}
							}
						}
						// 종료중
						else {
							// 세번째 진입
							// 공격 종료
							if (m_tPatternDelay[iCnt].bAttack &&
								m_tPatternDelay[iCnt].dwOffTime + m_tPatternDelay[iCnt].dwOffDelay < GetTickCount()) {
								m_tPatternDelay[iCnt].bAttack = false;
								// 스프라이트 프레임 되감기
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameCnt = 5;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameStart = 5;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameEnd = 0;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameScene = 1;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].dwFrameTime = GetTickCount();
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].dwFrameSpeed = 60;
							}
							// 네번째 진입
							else {
								// 되감기 완료
								if (m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameCnt == 0) {
									m_tPatternDelay[iCnt].bOnFlag = false;
									m_tPatternDelay[iCnt].bOffFlag = false;
									m_tPatternDelay[iCnt].dwOnTime = GetTickCount();
									// 2000 ~ 5000 값 랜덤
									m_tPatternDelay[iCnt].dwOnDelay = 2000 + 100 * (rand() % 31);
								}
							}
						}
					}
					break;
					// 왼쪽 서브
				case PATTERN::LL_SUB:
					// 파괴됨
					if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].bDamage) {
						break;
					}

					// 첫 진입
					// 스프라이트 프레임 이동
					if (!m_tPatternDelay[iCnt].bOnFlag) {
						m_tPatternDelay[iCnt].bOnFlag = true;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameCnt = 0;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameStart = 0;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameEnd = 8;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameScene = 0;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].dwFrameTime = GetTickCount();
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].dwFrameSpeed = 60;
					}
					else {
						// 두번째 진입
						// 종료중이 아니면
						if (!m_tPatternDelay[iCnt].bOffFlag) {
							// 8번째 프레임에서 공격
							if (m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameCnt == 8) {
								if (!m_tPatternDelay[iCnt].bAttack) {
									m_tPatternDelay[iCnt].bAttack = true;
									m_tPatternDelay[iCnt].dwOffTime = GetTickCount();
									// 스프라이트 프레임 고정
									m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameStart = 8;
								}

								// 공격
								if (m_tPatternDelay[iCnt].bAttack &&
									m_tPatternDelay[iCnt].dwOffTime + m_tPatternDelay[iCnt].dwOffDelay < GetTickCount()) {
									// 두종류 랜덤 공격
									if (rand() % 2) {
										L_SideSub_Shot();
									}
									else {
										L_SideSub_Buckshot();
									}

									// 종료
									m_tPatternDelay[iCnt].bOffFlag = true;
									m_tPatternDelay[iCnt].dwOffTime = GetTickCount();
								}
							}
						}
						// 종료중
						else {
							// 세번째 진입
							// 공격 종료
							if (m_tPatternDelay[iCnt].bAttack &&
								m_tPatternDelay[iCnt].dwOffTime + m_tPatternDelay[iCnt].dwOffDelay < GetTickCount()) {
								m_tPatternDelay[iCnt].bAttack = false;
								// 스프라이트 프레임 되감기
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameCnt = 7;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameStart = 7;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameEnd = 0;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameScene = 0;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].dwFrameTime = GetTickCount();
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].dwFrameSpeed = 60;
							}
							// 네번째 진입
							else {
								// 되감기 완료
								if (m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameCnt == 0) {
									m_tPatternDelay[iCnt].bOnFlag = false;
									m_tPatternDelay[iCnt].bOffFlag = false;
									m_tPatternDelay[iCnt].dwOnTime = GetTickCount();
									// 2000 ~ 5000 값 랜덤
									m_tPatternDelay[iCnt].dwOnDelay = 2000 + 100 * (rand() % 31);
								}
							}
						}
					}
					break;
					// 오른쪽 서브
				case PATTERN::RR_SUB:
					// 파괴됨
					if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].bDamage) {
						break;
					}

					// 첫 진입
					// 스프라이트 프레임 이동
					if (!m_tPatternDelay[iCnt].bOnFlag) {
						m_tPatternDelay[iCnt].bOnFlag = true;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameCnt = 0;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameStart = 0;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameEnd = 8;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameScene = 1;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].dwFrameTime = GetTickCount();
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].dwFrameSpeed = 60;
					}
					else {
						// 두번째 진입
						// 종료중이 아니면
						if (!m_tPatternDelay[iCnt].bOffFlag) {
							// 8번째 프레임에서 공격
							if (m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameCnt == 8) {
								if (!m_tPatternDelay[iCnt].bAttack) {
									m_tPatternDelay[iCnt].bAttack = true;
									m_tPatternDelay[iCnt].dwOffTime = GetTickCount();
									// 스프라이트 프레임 고정
									m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameStart = 8;
								}

								// 공격
								if (m_tPatternDelay[iCnt].bAttack &&
									m_tPatternDelay[iCnt].dwOffTime + m_tPatternDelay[iCnt].dwOffDelay < GetTickCount()) {
									// 두종류 랜덤 공격
									if (rand() % 2) {
										R_SideSub_Shot();
									}
									else {
										R_SideSub_Buckshot();
									}

									// 종료
									m_tPatternDelay[iCnt].bOffFlag = true;
									m_tPatternDelay[iCnt].dwOffTime = GetTickCount();
								}
							}
						}
						// 종료중
						else {
							// 세번째 진입
							// 공격 종료
							if (m_tPatternDelay[iCnt].bAttack &&
								m_tPatternDelay[iCnt].dwOffTime + m_tPatternDelay[iCnt].dwOffDelay < GetTickCount()) {
								m_tPatternDelay[iCnt].bAttack = false;
								// 스프라이트 프레임 되감기
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameCnt = 7;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameStart = 7;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameEnd = 0;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameScene = 1;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].dwFrameTime = GetTickCount();
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].dwFrameSpeed = 60;
							}
							// 네번째 진입
							else {
								// 되감기 완료
								if (m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameCnt == 0) {
									m_tPatternDelay[iCnt].bOnFlag = false;
									m_tPatternDelay[iCnt].bOffFlag = false;
									m_tPatternDelay[iCnt].dwOnTime = GetTickCount();
									// 2000 ~ 5000 값 랜덤
									m_tPatternDelay[iCnt].dwOnDelay = 2000 + 100 * (rand() % 31);
								}
							}
						}
					}
					break;
					// 정면 충전 공격
				case PATTERN::FRONT:
					// 뒤쪽 대형 캐논 공격 중이면 이패턴은 쉼
					if (m_tPatternDelay[static_cast<UINT>(PATTERN::BACK)].bOnFlag &&
						!m_tPatternDelay[iCnt].bAttack) {
						break;
					}

					// 첫 진입
					// 충전 이펙트 생성
					if (!m_tPatternDelay[iCnt].bAttack) {
						m_tPatternDelay[iCnt].bAttack = true;
						m_tPatternDelay[iCnt].dwOffTime = GetTickCount();

						// 충전 이펙트 생성
						CObj* pObj = CAbstractFactory<CEffect_Charge>::Create(m_tInfo.fX, m_tInfo.fY + (5.f * 3.f));
						pObj->Set_Target(this);
						static_cast<CEffect*>(pObj)->Set_RelativePos(0.f, 5.f * 3.f);
						CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

						CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS_ATTACK);
						CSoundMgr::Get_Instance()->PlaySound(L"Boss_Charge.wav", CSoundMgr::BOSS_ATTACK);
					}

					// 두번째 진입
					// 공격
					if (m_tPatternDelay[iCnt].bAttack &&
						m_tPatternDelay[iCnt].dwOffTime + m_tPatternDelay[iCnt].dwOffDelay < GetTickCount()) {
						// 정면 충전 공격
						Front_Shot();

						// 종료
						m_tPatternDelay[iCnt].bAttack = false;
						m_tPatternDelay[iCnt].dwOnTime = GetTickCount();
						// 8000 ~ 11000 값 랜덤
						m_tPatternDelay[iCnt].dwOnDelay = 8000 + 100 * (rand() % 31);
					}
					break;
					// 뒤쪽 대형 캐논
				case PATTERN::BACK:
					// 파괴됨
					if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].bDamage) {
						break;
					}

					// 첫 진입
					// 스프라이트 프레임 이동
					if (!m_tPatternDelay[iCnt].bOnFlag) {
						m_tPatternDelay[iCnt].bOnFlag = true;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameCnt = 0;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameStart = 0;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameEnd = 12;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameScene = 0;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].dwFrameTime = GetTickCount();
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].dwFrameSpeed = 60;

						CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS_ATTACK);
						CSoundMgr::Get_Instance()->PlaySound(L"Boss_Attack1.wav", CSoundMgr::BOSS_ATTACK);
						CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER);
						CSoundMgr::Get_Instance()->PlaySound(L"Boss_Attack2.wav", CSoundMgr::MONSTER);
					}
					else {
						// 두번째 진입
						// 종료중이 아니면
						if (!m_tPatternDelay[iCnt].bOffFlag) {
							// 12번째 프레임에서 공격
							if (m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameCnt == 12) {
								if (!m_tPatternDelay[iCnt].bAttack) {
									m_tPatternDelay[iCnt].bAttack = true;
									m_tPatternDelay[iCnt].dwOffTime = GetTickCount();
									// 스프라이트 프레임 고정
									m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameStart = 12;
								}

								// 공격
								if (m_tPatternDelay[iCnt].bAttack &&
									m_tPatternDelay[iCnt].dwOffTime + m_tPatternDelay[iCnt].dwOffDelay < GetTickCount()) {
									++m_tPatternDelay[iCnt].dwCnt;
									
									// 두종류 공격 패턴 번갈아가면서 공격
									if (m_bBackAttack) {
										// 총알 각도 변경
										m_fShotAngle = m_fShotAngle + 35.f;
										// 산탄
										Back_Buckshot();

										// 10번 공격하고 종료
										if (m_tPatternDelay[iCnt].dwCnt >= 10) {
											m_tPatternDelay[iCnt].dwCnt = 0;
											m_tPatternDelay[iCnt].bOffFlag = true;
											m_fShotAngle = 0.f;
											// 다음번에는 다른 공격 패턴
											m_bBackAttack = !m_bBackAttack;
										}
									}
									else {
										// 총알 각도 변경
										m_fShotAngle = m_fShotAngle + 38.5714f;

										// 연발
										Back_AutoShot();

										// 14번 공격하고 종료
										if (m_tPatternDelay[iCnt].dwCnt >= 14) {
											m_tPatternDelay[iCnt].dwCnt = 0;
											m_tPatternDelay[iCnt].bOffFlag = true;
											m_fShotAngle = 0.f;
											// 다음번에는 다른 공격 패턴
											m_bBackAttack = !m_bBackAttack;
										}
									}

									m_tPatternDelay[iCnt].dwOffTime = GetTickCount();
								}
							}
						}
						// 종료중
						else {
							// 세번째 진입
							// 공격 종료
							if (m_tPatternDelay[iCnt].bAttack &&
								m_tPatternDelay[iCnt].dwOffTime + m_tPatternDelay[iCnt].dwOffDelay < GetTickCount()) {
								m_tPatternDelay[iCnt].bAttack = false;
								// 스프라이트 프레임 되감기
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameCnt = 11;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameStart = 11;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameEnd = 0;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameScene = 0;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].dwFrameTime = GetTickCount();
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].dwFrameSpeed = 60;
							}
							// 네번째 진입
							else {
								// 되감기 완료
								if (m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameCnt == 0) {
									m_tPatternDelay[iCnt].bOnFlag = false;
									m_tPatternDelay[iCnt].bOffFlag = false;
									m_tPatternDelay[iCnt].dwOnTime = GetTickCount();
								}
							}
						}
					}
					break;
				default:
					break;
				}
			}
		}
		break;
	case STATE::DESTROY:
		if (m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			m_eCurState = STATE::FINAL_1;
			Shot_5(0.f);
			Shot_5(30.f);
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 100;
		}
		break;
	case STATE::FINAL_1:
		if (m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			m_eCurState = STATE::FINAL_2;
			Shot_5(20.f);
			Shot_5(50.f);
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 100;

			CObj* pObj = CAbstractFactory<CExplosion_04>::Create(m_tInfo.fX, m_tInfo.fY);		// 파괴 이펙트 생성
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			int irand = rand() % 3 + 1;
			pObj = CAbstractFactory<CExplosion_03>::Create(m_tInfo.fX - 165.f, m_tInfo.fY + 36.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(100 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CAbstractFactory<CExplosion_03>::Create(m_tInfo.fX + 165.f, m_tInfo.fY + 36.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(100 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			pObj = CAbstractFactory<CExplosion_02_1>::Create(m_tInfo.fX - 80.f, m_tInfo.fY);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(30 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CAbstractFactory<CExplosion_02_1>::Create(m_tInfo.fX + 80.f, m_tInfo.fY);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)50);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CAbstractFactory<CExplosion_01>::Create(m_tInfo.fX + irand * 30, m_tInfo.fY + 20.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(irand * 10));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CAbstractFactory<CExplosion_01>::Create(m_tInfo.fX - irand * 30, m_tInfo.fY + 50.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)100);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		}
		break;
	case STATE::FINAL_2:
		if (m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			m_eCurState = STATE::FINAL_3;
			Shot_5(-15.f);
			Shot_5(-45.f);
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 100;

			CObj* pObj = CAbstractFactory<CExplosion_04>::Create(m_tInfo.fX, m_tInfo.fY);		// 파괴 이펙트 생성
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			int irand = rand() % 3 + 1;
			pObj = CAbstractFactory<CExplosion_03>::Create(m_tInfo.fX - 165.f, m_tInfo.fY + 36.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(100 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CAbstractFactory<CExplosion_03>::Create(m_tInfo.fX + 165.f, m_tInfo.fY + 36.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(100 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			pObj = CAbstractFactory<CExplosion_02_1>::Create(m_tInfo.fX - 80.f, m_tInfo.fY);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(30 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CAbstractFactory<CExplosion_02_1>::Create(m_tInfo.fX + 80.f, m_tInfo.fY);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)50);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CAbstractFactory<CExplosion_01>::Create(m_tInfo.fX + irand * 30, m_tInfo.fY + 20.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(irand * 10));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CAbstractFactory<CExplosion_01>::Create(m_tInfo.fX - irand * 30, m_tInfo.fY + 50.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)100);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		}
		break;
	case STATE::FINAL_3:
		if (m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			m_eCurState = STATE::FINAL_4;
			Shot_5(-5.f);
			Shot_5(25.f);
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 100;

			CObj* pObj = CAbstractFactory<CExplosion_04>::Create(m_tInfo.fX, m_tInfo.fY);		// 파괴 이펙트 생성
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			int irand = rand() % 3 + 1;
			pObj = CAbstractFactory<CExplosion_03>::Create(m_tInfo.fX - 165.f, m_tInfo.fY + 36.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(100 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CAbstractFactory<CExplosion_03>::Create(m_tInfo.fX + 165.f, m_tInfo.fY + 36.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(100 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			pObj = CAbstractFactory<CExplosion_02_1>::Create(m_tInfo.fX - 80.f, m_tInfo.fY);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(30 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CAbstractFactory<CExplosion_02_1>::Create(m_tInfo.fX + 80.f, m_tInfo.fY);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)50);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CAbstractFactory<CExplosion_01>::Create(m_tInfo.fX + irand * 30, m_tInfo.fY + 20.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(irand * 10));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CAbstractFactory<CExplosion_01>::Create(m_tInfo.fX - irand * 30, m_tInfo.fY + 50.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)100);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		}
		break;
	case STATE::FINAL_4:
		if (m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			m_eCurState = STATE::FINAL_5;
			Shot_5(5.f);
			Shot_5(-25.f);
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 100;

			CObj* pObj = CAbstractFactory<CExplosion_04>::Create(m_tInfo.fX, m_tInfo.fY);		// 파괴 이펙트 생성
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			int irand = rand() % 3 + 1;
			pObj = CAbstractFactory<CExplosion_03>::Create(m_tInfo.fX - 165.f, m_tInfo.fY + 36.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(100 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CAbstractFactory<CExplosion_03>::Create(m_tInfo.fX + 165.f, m_tInfo.fY + 36.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(100 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			pObj = CAbstractFactory<CExplosion_02_1>::Create(m_tInfo.fX - 80.f, m_tInfo.fY);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(30 + irand * 50));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CAbstractFactory<CExplosion_02_1>::Create(m_tInfo.fX + 80.f, m_tInfo.fY);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)50);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CAbstractFactory<CExplosion_01>::Create(m_tInfo.fX + irand * 30, m_tInfo.fY + 20.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(irand * 10));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			pObj = CAbstractFactory<CExplosion_01>::Create(m_tInfo.fX - irand * 30, m_tInfo.fY + 50.f);
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)100);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		}
		break;
	case STATE::FINAL_5:
		if (!g_bStopMapScroll)
		{
			// 맵 밖으로 퇴장
			m_tInfo.fY = m_tInfo.fY + 4.f;
		}
		break;
	}


	// 파괴됨
	if (m_bDestroy)
	{
		// 파괴 이펙트 출력
		if (m_dwDestroyTime + m_dwDestroyDelay < GetTickCount()) {
			m_dwDestroyTime = GetTickCount();

			int irandX = rand() % 60 - 30;
			int irandY = rand() % 60 - 30;
			CObj* pObj = CAbstractFactory<CExplosion_02_1>::Create(m_tInfo.fX - ((55.f + irandX) * 3.f), m_tInfo.fY + ((12.f + irandY) * 3.f));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			pObj = CAbstractFactory<CExplosion_02_1>::Create(m_tInfo.fX + ((55.f + irandX) * 3.f), m_tInfo.fY + ((12.f + irandY) * 3.f));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			pObj = CAbstractFactory<CExplosion_02_1>::Create(m_tInfo.fX + (irandX * 3.f), m_tInfo.fY + ((50.f + irandY) * 3.f));
			static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)100);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		}
	}
}

// 보스 이동
void CTequilaGround::Move()
{
	if (m_bHpLock || m_tInfo.fY < 300.f)
		return;

	// 좌우 이동 결정
	if (m_fSpeed == 0)
	{
		// 오른쪽 이동
		if (rand() % 2)
		{
			m_fSpeed = 0.2f;
		}
		// 왼쪽 이동
		else
		{
			m_fSpeed = -0.2f;
		}
	}
	// 오른쪽 이동
	else if (m_fSpeed >= 0.2f)
	{
		// 오른쪽 끝에 다왔으면 방향 전환
		if (m_tInfo.fX + m_fSpeed >= (WINCX >> 1) + 20.f)
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
					m_fSpeed = -0.2f;
					m_bMoveFlag = false;
				}
			}
		}
	}
	// 왼쪽 이동
	else if (m_fSpeed <= -0.2f)
	{
		// 왼쪽 끝에 다왔으면 방향 전환
		if (m_tInfo.fX + m_fSpeed <= (WINCX >> 1) - 20.f)
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
					m_fSpeed = 0.2f;
					m_bMoveFlag = false;
				}
			}
		}
	}
	// 이동
	float fMovePosX = m_tInfo.fX + m_fSpeed;
	if ((WINCX >> 1) - 20.f < fMovePosX && fMovePosX < (WINCX >> 1) + 20.f)
	{
		m_tInfo.fX = m_tInfo.fX + m_fSpeed;
	}
}

// 파괴 이펙트
void CTequilaGround::DamageEffect()
{
	CObj* pObj;
	LONG lCenterX;
	LONG lCenterY;

	if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].bDamage && m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].iEffectCnt != 0)
	{
		if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].dwTime + m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].dwDelay < GetTickCount())
		{
			--m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].iEffectCnt;

			lCenterX = (m_vecHitRect[static_cast<UINT>(SPRITE::L_CANNON)].right - m_vecHitRect[static_cast<UINT>(SPRITE::L_CANNON)].left) >> 1;
			lCenterY = (m_vecHitRect[static_cast<UINT>(SPRITE::L_CANNON)].bottom - m_vecHitRect[static_cast<UINT>(SPRITE::L_CANNON)].top) >> 1;

			// 이펙트 생성
			pObj = CAbstractFactory<CExplosion_05>::Create(
				static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::L_CANNON)].left + lCenterX),
				static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::L_CANNON)].top + lCenterY));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].dwTime = GetTickCount();
		}
	}

	if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].bDamage && m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].iEffectCnt != 0)
	{
		if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].dwTime + m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].dwDelay < GetTickCount())
		{
			--m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].iEffectCnt;

			lCenterX = (m_vecHitRect[static_cast<UINT>(SPRITE::R_CANNON)].right - m_vecHitRect[static_cast<UINT>(SPRITE::R_CANNON)].left) >> 1;
			lCenterY = (m_vecHitRect[static_cast<UINT>(SPRITE::R_CANNON)].bottom - m_vecHitRect[static_cast<UINT>(SPRITE::R_CANNON)].top) >> 1;

			// 이펙트 생성
			pObj = CAbstractFactory<CExplosion_05>::Create(
				static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::R_CANNON)].left + lCenterX),
				static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::R_CANNON)].top + lCenterY));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].dwTime = GetTickCount();
		}
	}

	if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].bDamage && m_bDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].iEffectCnt != 0)
	{
		if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].dwTime + m_bDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].dwDelay < GetTickCount())
		{
			--m_bDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].iEffectCnt;

			lCenterX = (m_vecHitRect[static_cast<UINT>(SPRITE::LL_CANNON)].right - m_vecHitRect[static_cast<UINT>(SPRITE::LL_CANNON)].left) >> 1;
			lCenterY = (m_vecHitRect[static_cast<UINT>(SPRITE::LL_CANNON)].bottom - m_vecHitRect[static_cast<UINT>(SPRITE::LL_CANNON)].top) >> 1;

			// 이펙트 생성
			pObj = CAbstractFactory<CExplosion_05>::Create(
				static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::LL_CANNON)].left + lCenterX),
				static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::LL_CANNON)].top + lCenterY));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			m_bDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].dwTime = GetTickCount();
		}
	}

	if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].bDamage && m_bDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].iEffectCnt != 0)
	{
		if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].dwTime + m_bDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].dwDelay < GetTickCount())
		{
			--m_bDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].iEffectCnt;

			lCenterX = (m_vecHitRect[static_cast<UINT>(SPRITE::RR_CANNON)].right - m_vecHitRect[static_cast<UINT>(SPRITE::RR_CANNON)].left) >> 1;
			lCenterY = (m_vecHitRect[static_cast<UINT>(SPRITE::RR_CANNON)].bottom - m_vecHitRect[static_cast<UINT>(SPRITE::RR_CANNON)].top) >> 1;

			// 이펙트 생성
			pObj = CAbstractFactory<CExplosion_05>::Create(
				static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::RR_CANNON)].left + lCenterX),
				static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::RR_CANNON)].top + lCenterY));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			m_bDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].dwTime = GetTickCount();
		}
	}

	if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].bDamage && m_bDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iEffectCnt != 0)
	{
		if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].dwTime + m_bDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].dwDelay < GetTickCount())
		{
			--m_bDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iEffectCnt;

			lCenterX = (m_vecHitRect[static_cast<UINT>(SPRITE::CENTER_CANNON)].right - m_vecHitRect[static_cast<UINT>(SPRITE::CENTER_CANNON)].left) >> 1;
			lCenterY = (m_vecHitRect[static_cast<UINT>(SPRITE::CENTER_CANNON)].bottom - m_vecHitRect[static_cast<UINT>(SPRITE::CENTER_CANNON)].top) >> 1;

			// 이펙트 생성
			pObj = CAbstractFactory<CExplosion_05>::Create(
				static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::CENTER_CANNON)].left + lCenterX),
				static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::CENTER_CANNON)].top + lCenterY));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			m_bDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].dwTime = GetTickCount();
		}
	}
}

// 왼쪽 서브 캐논
void CTequilaGround::L_SideSub_Shot()
{
	float fX;
	float fY;
	float fDia;
	float fRad;
	float fAngle;
	CObj* pObj = nullptr;

	// 왼쪽 서브 2
	if (!m_bDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].bDamage) {
		// 각도 구하기
		fX = m_pTarget->Get_Info().fX - (m_tInfo.fX - 165.f);
		fY = m_pTarget->Get_Info().fY - (m_tInfo.fY + 36.f);
		fDia = sqrtf(fX * fX + fY * fY);
		fRad = acosf(fX / fDia);

		fAngle = fRad * 180.f / PI;

		if (m_tInfo.fY < m_pTarget->Get_Info().fY)
			fAngle *= -1.f;

		pObj = CAbstractFactory<CBullet_2>::Create(m_tInfo.fX - 165.f, m_tInfo.fY + 36.f);
		pObj->Set_Angle(fAngle);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_2>::Create(m_tInfo.fX - 165.f, m_tInfo.fY + 36.f);
		pObj->Set_Angle(fAngle);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(80);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
	}
}

// 오른쪽 서브 캐논
void CTequilaGround::R_SideSub_Shot()
{
	float fX;
	float fY;
	float fDia;
	float fRad;
	float fAngle;
	CObj* pObj = nullptr;

	// 오른쪽 서브 2
	if (!m_bDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].bDamage) {
		// 각도 구하기
		fX = m_pTarget->Get_Info().fX - (m_tInfo.fX + 165.f);
		fY = m_pTarget->Get_Info().fY - (m_tInfo.fY + 36.f);
		fDia = sqrtf(fX * fX + fY * fY);
		fRad = acosf(fX / fDia);

		fAngle = fRad * 180.f / PI;

		if (m_tInfo.fY < m_pTarget->Get_Info().fY)
			fAngle *= -1.f;

		pObj = CAbstractFactory<CBullet_2>::Create(m_tInfo.fX + 165.f, m_tInfo.fY + 36.f);
		pObj->Set_Angle(fAngle);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_2>::Create(m_tInfo.fX + 165.f, m_tInfo.fY + 36.f);
		pObj->Set_Angle(fAngle);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(80);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
	}
}

// 왼쪽 서브 캐논 산탄
void CTequilaGround::L_SideSub_Buckshot()
{
	float fX;
	float fY;
	float fDia;
	float fRad;
	float fAngle;
	CObj* pObj = nullptr;

	// 왼쪽 서브 2
	if (!m_bDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].bDamage) {
		// 각도 구하기
		fX = m_pTarget->Get_Info().fX - (m_tInfo.fX - 165.f);
		fY = m_pTarget->Get_Info().fY - (m_tInfo.fY + 36.f);
		fDia = sqrtf(fX * fX + fY * fY);
		fRad = acosf(fX / fDia);

		fAngle = fRad * 180.f / PI;

		if (m_tInfo.fY < m_pTarget->Get_Info().fY)
			fAngle *= -1.f;

		pObj = CAbstractFactory<CBullet_2>::Create(m_tInfo.fX - 165.f, m_tInfo.fY + 36.f);
		pObj->Set_Angle(fAngle);
		pObj->Set_Speed(9.f);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_2>::Create(m_tInfo.fX - 165.f, m_tInfo.fY + 36.f);
		pObj->Set_Angle(fAngle + 2.f);
		pObj->Set_Speed(8.6f);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_2>::Create(m_tInfo.fX - 165.f, m_tInfo.fY + 36.f);
		pObj->Set_Angle(fAngle - 2.f);
		pObj->Set_Speed(8.6f);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_2>::Create(m_tInfo.fX - 165.f, m_tInfo.fY + 36.f);
		pObj->Set_Angle(fAngle + 3.f);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_2>::Create(m_tInfo.fX - 165.f, m_tInfo.fY + 36.f);
		pObj->Set_Angle(fAngle - 3.f);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
	}
}

// 오른쪽 서브 캐논 산탄
void CTequilaGround::R_SideSub_Buckshot()
{
	float fX;
	float fY;
	float fDia;
	float fRad;
	float fAngle;
	CObj* pObj = nullptr;

	// 오른쪽 서브 2
	if (!m_bDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].bDamage) {
		// 각도 구하기
		fX = m_pTarget->Get_Info().fX - (m_tInfo.fX + 165.f);
		fY = m_pTarget->Get_Info().fY - (m_tInfo.fY + 36.f);
		fDia = sqrtf(fX * fX + fY * fY);
		fRad = acosf(fX / fDia);

		fAngle = fRad * 180.f / PI;

		if (m_tInfo.fY < m_pTarget->Get_Info().fY)
			fAngle *= -1.f;

		pObj = CAbstractFactory<CBullet_2>::Create(m_tInfo.fX + 165.f, m_tInfo.fY + 36.f);
		pObj->Set_Angle(fAngle);
		pObj->Set_Speed(9.f);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_2>::Create(m_tInfo.fX + 165.f, m_tInfo.fY + 36.f);
		pObj->Set_Angle(fAngle + 2.f);
		pObj->Set_Speed(8.6f);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_2>::Create(m_tInfo.fX + 165.f, m_tInfo.fY + 36.f);
		pObj->Set_Angle(fAngle - 2.f);
		pObj->Set_Speed(8.6f);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_2>::Create(m_tInfo.fX + 165.f, m_tInfo.fY + 36.f);
		pObj->Set_Angle(fAngle + 3.f);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_2>::Create(m_tInfo.fX + 165.f, m_tInfo.fY + 36.f);
		pObj->Set_Angle(fAngle - 3.f);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
	}
}

// 정면 캐논
void CTequilaGround::Front_Shot()
{
	// 정면
	CObj* pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX, m_tInfo.fY + (5.f * 3.f));
	pObj->Set_Angle(-90.f);
	pObj->Set_Speed(8.5f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX, m_tInfo.fY + (5.f * 3.f));
	pObj->Set_Angle(-86.f);
	pObj->Set_Speed(8.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX, m_tInfo.fY + (5.f * 3.f));
	pObj->Set_Angle(-94.f);
	pObj->Set_Speed(8.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX, m_tInfo.fY + (5.f * 3.f));
	pObj->Set_Angle(-87.8f);
	pObj->Set_Speed(7.5f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX, m_tInfo.fY + (5.f * 3.f));
	pObj->Set_Angle(-92.2f);
	pObj->Set_Speed(7.5f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX, m_tInfo.fY + (5.f * 3.f));
	pObj->Set_Angle(-88.5f);
	pObj->Set_Speed(7.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX, m_tInfo.fY + (5.f * 3.f));
	pObj->Set_Angle(-91.5f);
	pObj->Set_Speed(7.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
}

// 오른쪽 정면 서브 캐논
void CTequilaGround::L_FrontSub_Shot()
{
	float fX;
	float fY;
	float fDia;
	float fRad;
	float fAngle;
	CObj* pObj = nullptr;

	// 왼쪽 서브 1
	if (!m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].bDamage) {
		// 각도 구하기
		fX = m_pTarget->Get_Info().fX - (m_tInfo.fX - 72.f);
		fY = m_pTarget->Get_Info().fY - (m_tInfo.fY + 96.f);
		fDia = sqrtf(fX * fX + fY * fY);
		fRad = acosf(fX / fDia);

		fAngle = fRad * 180.f / PI;

		if (m_tInfo.fY < m_pTarget->Get_Info().fY)
			fAngle *= -1.f;

		pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX - 72.f, m_tInfo.fY + 96.f);
		pObj->Set_Angle(fAngle);
		pObj->Set_Speed(7.f);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX - 72.f, m_tInfo.fY + 96.f);
		pObj->Set_Angle(fAngle - 3.f);
		pObj->Set_Speed(7.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(60);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX - 72.f, m_tInfo.fY + 96.f);
		pObj->Set_Angle(fAngle + 3.f);
		pObj->Set_Speed(7.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(60);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX - 72.f, m_tInfo.fY + 96.f);
		pObj->Set_Angle(fAngle - 6.f);
		pObj->Set_Speed(7.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(120);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX - 72.f, m_tInfo.fY + 96.f);
		pObj->Set_Angle(fAngle + 6.f);
		pObj->Set_Speed(7.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(120);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX - 72.f, m_tInfo.fY + 96.f);
		pObj->Set_Angle(fAngle - 3.f);
		pObj->Set_Speed(7.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(180);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX - 72.f, m_tInfo.fY + 96.f);
		pObj->Set_Angle(fAngle + 3.f);
		pObj->Set_Speed(7.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(180);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX - 72.f, m_tInfo.fY + 96.f);
		pObj->Set_Angle(fAngle);
		pObj->Set_Speed(7.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(240);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
	}
}

// 오른쪽 정면 서브 캐논
void CTequilaGround::R_FrontSub_Shot()
{
	float fX;
	float fY;
	float fDia;
	float fRad;
	float fAngle;
	CObj* pObj = nullptr;

	// 오른쪽 서브 1
	if (!m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].bDamage) {
		// 각도 구하기
		fX = m_pTarget->Get_Info().fX - (m_tInfo.fX + 72.f);
		fY = m_pTarget->Get_Info().fY - (m_tInfo.fY + 96.f);
		fDia = sqrtf(fX * fX + fY * fY);
		fRad = acosf(fX / fDia);

		fAngle = fRad * 180.f / PI;

		if (m_tInfo.fY < m_pTarget->Get_Info().fY)
			fAngle *= -1.f;

		pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX + 72.f, m_tInfo.fY + 96.f);
		pObj->Set_Angle(fAngle);
		pObj->Set_Speed(7.f);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX + 72.f, m_tInfo.fY + 96.f);
		pObj->Set_Angle(fAngle - 3.f);
		pObj->Set_Speed(7.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(60);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX + 72.f, m_tInfo.fY + 96.f);
		pObj->Set_Angle(fAngle + 3.f);
		pObj->Set_Speed(7.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(60);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX + 72.f, m_tInfo.fY + 96.f);
		pObj->Set_Angle(fAngle - 6.f);
		pObj->Set_Speed(7.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(120);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX + 72.f, m_tInfo.fY + 96.f);
		pObj->Set_Angle(fAngle + 6.f);
		pObj->Set_Speed(7.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(120);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX + 72.f, m_tInfo.fY + 96.f);
		pObj->Set_Angle(fAngle - 3.f);
		pObj->Set_Speed(7.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(180);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX + 72.f, m_tInfo.fY + 96.f);
		pObj->Set_Angle(fAngle + 3.f);
		pObj->Set_Speed(7.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(180);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX + 72.f, m_tInfo.fY + 96.f);
		pObj->Set_Angle(fAngle);
		pObj->Set_Speed(7.f);
		static_cast<CBullet*>(pObj)->Set_BulletDelay(240);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
	}
}

// 뒤쪽 연발 공격
void CTequilaGround::Back_AutoShot()
{
	// 왼쪽
	CObj* pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX - (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(m_fShotAngle - 90.f);
	pObj->Set_Speed(6.5f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX - (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(m_fShotAngle - 85.f);
	pObj->Set_Speed(6.4f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(20);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX - (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(m_fShotAngle - 80.f);
	pObj->Set_Speed(6.3f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(40);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX - (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(m_fShotAngle - 75.f);
	pObj->Set_Speed(6.2f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(60);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);


	// 오른쪽
	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX + (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(-m_fShotAngle - 90.f);
	pObj->Set_Speed(6.5f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX + (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(-m_fShotAngle - 95.f);
	pObj->Set_Speed(6.4f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(20);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX + (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(-m_fShotAngle - 100.f);
	pObj->Set_Speed(6.3f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(40);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX + (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(-m_fShotAngle - 105.f);
	pObj->Set_Speed(6.2f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(60);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
}

// 뒤쪽 산탄 공격
void CTequilaGround::Back_Buckshot()
{
	// 왼쪽
	CObj* pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX - (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(-m_fShotAngle - 60.f);
	pObj->Set_Speed(6.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX - (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(-m_fShotAngle - 67.5f);
	pObj->Set_Speed(6.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX - (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(-m_fShotAngle - 75.f);
	pObj->Set_Speed(6.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX - (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(-m_fShotAngle - 82.5f);
	pObj->Set_Speed(6.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	// 한타이밍 늦게 더 빠르게
	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX - (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(-m_fShotAngle - 63.5f);
	pObj->Set_Speed(7.2f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(40);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX - (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(-m_fShotAngle - 71.f);
	pObj->Set_Speed(7.2f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(40);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX - (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(-m_fShotAngle - 78.5f);
	pObj->Set_Speed(7.2f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(40);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX - (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(-m_fShotAngle - 86.f);
	pObj->Set_Speed(7.2f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(40);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);


	// 오른쪽
	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX + (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(m_fShotAngle - 120.f);
	pObj->Set_Speed(6.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX + (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(m_fShotAngle - 112.5f);
	pObj->Set_Speed(6.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX + (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(m_fShotAngle - 105.f);
	pObj->Set_Speed(6.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX + (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(m_fShotAngle - 97.5f);
	pObj->Set_Speed(6.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	// 한타이밍 늦게 더 빠르게
	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX + (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(m_fShotAngle - 116.5f);
	pObj->Set_Speed(7.2f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(40);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX + (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(m_fShotAngle - 109.f);
	pObj->Set_Speed(7.2f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(40);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX + (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(m_fShotAngle - 101.5f);
	pObj->Set_Speed(7.2f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(40);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX + (27.f * 3), m_tInfo.fY - (64.f * 3.f));
	pObj->Set_Angle(m_fShotAngle - 94.f);
	pObj->Set_Speed(7.2f);
	static_cast<CBullet*>(pObj)->Set_BulletDelay(40);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
}

void CTequilaGround::Shot_4()
{
	CObj* pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX, m_tInfo.fY + 20.f);
	pObj->Set_Angle(-90.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX - 20.f, m_tInfo.fY - 20.f);
	pObj->Set_Angle(-90.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX + 20.f, m_tInfo.fY - 20.f);
	pObj->Set_Angle(-90.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
}

void CTequilaGround::Shot_5(float _fAngle)
{
	float fCnt = _fAngle;

	while (fCnt < 180.f)
	{
		CObj* pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX, m_tInfo.fY);
		pObj->Set_Angle(fCnt);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		fCnt = fCnt + 25.f;
	}

	fCnt = -180.f + _fAngle;
	while (fCnt < 0.f)
	{
		CObj* pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX, m_tInfo.fY);
		pObj->Set_Angle(fCnt);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		fCnt = fCnt + 25.f;
	}
}

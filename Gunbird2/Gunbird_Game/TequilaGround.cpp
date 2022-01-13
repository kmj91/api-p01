// �����
// 1 ������ ����
// �ı��� �� 2 ������ ������ ����

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
	, m_tPatternDelay{}, m_tCannonFrameArr{}, m_tDamageFlagArr{}
	, m_bMoveFlag(false)
{
}


CTequilaGround::~CTequilaGround()
{
	Release();
}

void CTequilaGround::Initialize()
{
	// �̹��� ũ�� ����
	m_tInfo.iCX = TEQUILA_GROUND_WIDTH * 3;
	m_tInfo.iCY = TEQUILA_GROUND_HEIGHT * 3;
	m_iImageWidth = TEQUILA_GROUND_WIDTH;
	m_iImageHeight = TEQUILA_GROUND_HEIGHT;
	// ü��
	m_fHp = 4000;
	//m_fHp = 100;
	// �浹 ��Ʈ ��ġ �� �κ� ü�� ����
	m_vecHitRectPos.reserve(5);
	m_vecHitRectPos.emplace_back(RECT{ 72 * 3, 117 * 3, 108 * 3, 177 * 3 });		// �ε��� 0�� ���� ���� 1
	m_vecHitRectPos.emplace_back(RECT{ 108 * 3, 117 * 3, 144 * 3, 177 * 3 });		// �ε��� 1�� ������ ���� 1
	m_vecHitRectPos.emplace_back(RECT{ 20 * 3, 66 * 3, 72 * 3, 126 * 3 });			// �ε��� 2�� ���� ���� 2
	m_vecHitRectPos.emplace_back(RECT{ 144 * 3, 66 * 3, 196 * 3, 126 * 3 });		// �ε��� 3�� ������ ���� 2
	m_vecHitRectPos.emplace_back(RECT{ 72 * 3, 45 * 3, 144 * 3, 117 * 3 });			// �ε��� 4�� ���� ���� 3
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
	// �⺻ ����
	m_eCurState = STATE::WAIT;

	// ���� ������
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
	// ���� �ı� ����Ʈ ���� �ʱ�ȭ
	m_tDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].iEffectCnt = 20;
	m_tDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].dwDelay = 50;
	m_tDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].iEffectCnt = 20;
	m_tDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].dwDelay = 50;
	m_tDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].iEffectCnt = 20;
	m_tDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].dwDelay = 50;
	m_tDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].iEffectCnt = 20;
	m_tDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].dwDelay = 50;
	m_tDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iEffectCnt = 20;
	m_tDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].dwDelay = 50;

	// ����
	m_iScore = 5000;
}

// ������Ʈ
int CTequilaGround::Update()
{
	// ����
	if (m_bRemove)
		return OBJ_DEAD;
	// ����
	if (m_bDead)
	{
		// ó�� �ı��Ǹ�
		if (!m_bDestroy && m_ePreState != STATE::DESTROY)
		{
			// �ı� ���·� ����
			m_eCurState = STATE::DESTROY;
			// �ı��Ǽ� HP �ٽ� ��
			m_bHpLock = true;
			// �ı� �÷��� true
			m_bDestroy = true;

			// 2������ ���� ����
			CObj* pObj = CAbstractFactory<CTequilaGallop>::Create(m_tInfo.fX, m_tInfo.fY);
			pObj->Set_Target(m_pTarget);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::GROUND_BOSS, false);

			// �ı� ����Ʈ ����
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

			// �����߿� ������� ���� ���ݻ��·� ��ȯ�Ǹ鼭 �״� ��찡 ����
			// �׷��� ���� �ٷ� ��ȯ
			Scene_Change();
		}	
	}

	// ���� �ൿ ����
	Action();
	// �̵�
	Move();
	// �ı� ����Ʈ;
	DamageEffect();
	// �̹��� RECT ���� �� Hit RECT ���� ����
	// Obj�� �ƴ� Boss�� Update_Rect ȣ��
	Update_Rect();
	// ������ �� ���� ó��
	Scene_Change();
	// �̹��� ������ �̵�
	Frame_Move();
	// �κ� �̹��� ������ �̵�
	Frame_MoveEX();

	return OBJ_NOEVENT;
}

// ����Ʈ ������Ʈ
void CTequilaGround::Late_Update()
{
	// �� �ϴ� �ٱ����� ������ ����
	if (WINCY <= m_tRect.top)
		m_bRemove = true;
}

// ����
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
		// ����
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TequilaGround_Idle");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));

		// ĳ�� 1
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

		// ĳ�� 2
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

		// ĳ�� 3
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

	// ���� �ɼǿ��� �浹 �ڽ� ���⸦ �״ٸ� (�ѹ��е� 1�� Ű)
	// �浹 �ڽ��� ���� �������
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

// ���� ������ �ı� ������ üũ
void CTequilaGround::Check_Damage(int _iPlayerNum)
{
	CObj* pObj;
	LONG lCenterX;
	LONG lCenterY;

	// ���� ���� 1
	if (!m_tDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].bDamage && m_vecRectHp[static_cast<UINT>(SPRITE::L_CANNON)] <= 0) {
		m_tDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].bDamage = true;
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

		// ���� �ı� ���� ȹ��
		CGameUIMgr::Get_Instance()->Add_Score(2000, _iPlayerNum);
	}

	// ������ ���� 1
	if (!m_tDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].bDamage && m_vecRectHp[static_cast<UINT>(SPRITE::R_CANNON)] <= 0) {
		m_tDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].bDamage = true;
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

		// ���� �ı� ���� ȹ��
		CGameUIMgr::Get_Instance()->Add_Score(2000, _iPlayerNum);
	}

	// ���� ���� 2
	if (!m_tDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].bDamage && m_vecRectHp[static_cast<UINT>(SPRITE::LL_CANNON)] <= 0) {
		m_tDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].bDamage = true;
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

		// ���� �ı� ���� ȹ��
		CGameUIMgr::Get_Instance()->Add_Score(2000, _iPlayerNum);
	}

	// ������ ���� 2
	if (!m_tDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].bDamage && m_vecRectHp[static_cast<UINT>(SPRITE::RR_CANNON)] <= 0) {
		m_tDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].bDamage = true;
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

		// ���� �ı� ���� ȹ��
		CGameUIMgr::Get_Instance()->Add_Score(2000, _iPlayerNum);
	}

	// ���� ���� 3
	if (!m_tDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].bDamage && m_vecRectHp[static_cast<UINT>(SPRITE::CENTER_CANNON)] <= 0) {
		m_tDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].bDamage = true;
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

		// ���� �ı� ���� ȹ��
		CGameUIMgr::Get_Instance()->Add_Score(5000, _iPlayerNum);
	}
}

// ������ �� ���� ó��
void CTequilaGround::Scene_Change()
{
	// ���� ���¿� �ٸ��� ��ü
	if (m_ePreState != m_eCurState)
	{
		// ������ ���� ���������� �����Ǿ�����
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
			// �翷 ���� ĳ�� ���� ĳ�� ����
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

// �κ� ��������Ʈ ĳ�� ������ ������Ʈ
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

// ���� �ൿ
void CTequilaGround::Action()
{
	switch (m_ePreState)
	{
		// ó���� ��� ���� ��
	case STATE::WAIT:
		if (!g_bStopMapScroll)
		{
			m_tInfo.fY += g_fBackgroundSpeed * 3.f;			// ��濡 ���缭 Y�� �̵�
		}

		if (g_bBossGO)
		{
			m_eCurState = STATE::CLOSE_DOOR;
		}
		break;
		// NPC���� �¿��
	case STATE::CLOSE_DOOR:
		if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		{
			m_eCurState = STATE::PAVED_ROAD;
			// ��� ��ũ�� ������ ����
			g_fBackgroundSpeed = 2.f;
		}
		break;
		// ������ ��� ��ũ�ѷ� �̵� ��
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
		// ������ ����
	case STATE::IDLE:
		if (m_bHpLock)
		{
			if (m_tInfo.fY >= 0)
			{
				m_bHpLock = false;		// �������� ó���� �� ����
			}
		}

		// Y�� ��ǥ 300 ��ŭ �̵� �� ���� ����
		if (m_tInfo.fY < 300.f)
		{
			m_tInfo.fY += 2.f;
			m_tPatternDelay[static_cast<UINT>(PATTERN::BACK)].dwOnTime = GetTickCount();
		}
		else
		{
			if (m_tPatternDelay[static_cast<UINT>(PATTERN::IDLE)].dwOnTime + m_tPatternDelay[static_cast<UINT>(PATTERN::IDLE)].dwOnDelay < GetTickCount())
			{
				// ���� ���·�
				m_eCurState = STATE::ATTACK;
				m_tPatternDelay[static_cast<UINT>(PATTERN::IDLE)].dwOnTime = GetTickCount();
			}
		}

		break;
		// ����
	case STATE::ATTACK:
		// �ݺ����� ���鼭 ���� ���ϵ��� �ϳ��� �����
		for (int iCnt = static_cast<int>(PATTERN::L_SUB); iCnt < static_cast<int>(PATTERN::END); ++iCnt) {
			// �� ���ϵ��� ���� ��� �ð��� ������ ������ ��
			if (m_tPatternDelay[iCnt].dwOnTime + m_tPatternDelay[iCnt].dwOnDelay < GetTickCount()) {
				switch (static_cast<PATTERN>(iCnt))
				{
					// ���� ���� ����
				case PATTERN::L_SUB:
					// ���� ���� ĳ���� �Ѵ� �ı��ǰų� ���� ĳ���� �ı��Ǿ�� ������ ����
					// �ı����� �ʾҴٸ� ���� �ߴ�
					if ((!m_tDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].bDamage || !m_tDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].bDamage) &&
						!m_tDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].bDamage) {
						break;
					}

					// �ı���ٸ� ���� �ߴ�
					if (m_tDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].bDamage) {
						break;
					}

					// 1. ó�� ���� ���� ���� ��
					if (!m_tPatternDelay[iCnt].bOnFlag) {
						// ���� �÷��� true
						m_tPatternDelay[iCnt].bOnFlag = true;
						// �κ� �̹��� ������ �ʱ�ȭ
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameCnt = 0;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameStart = 0;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameEnd = 6;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameScene = 0;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].dwFrameTime = GetTickCount();
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].dwFrameSpeed = 60;
					}
					else {
						// 2. ������ ���� ���� �ʾ��� ������ ���� ���� �ð� �� ����
						if (!m_tPatternDelay[iCnt].bOffFlag) {
							// 8��° �����ӿ��� ����
							if (m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameCnt == 6) {
								// ������ ���� �ʾҴٸ�
								if (!m_tPatternDelay[iCnt].bAttack) {
									// ���� �÷��� ture
									m_tPatternDelay[iCnt].bAttack = true;
									// ���� �ð� ����
									m_tPatternDelay[iCnt].dwOffTime = GetTickCount();
									// �̹��� ������ ����
									m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameStart = 6;
								}

								// ���� �÷��� true ���� ������ ���� �� ���� �ð��� ��������
								if (m_tPatternDelay[iCnt].bAttack &&
									m_tPatternDelay[iCnt].dwOffTime + m_tPatternDelay[iCnt].dwOffDelay < GetTickCount()) {
									// ���� ���� ����
									L_FrontSub_Shot();

									// ���� ����
									m_tPatternDelay[iCnt].bOffFlag = true;
									// ���� �ð� ����
									m_tPatternDelay[iCnt].dwOffTime = GetTickCount();
								}
							}
						}
						else {
							// 3. ���� �� ���� �ݱ�
							if (m_tPatternDelay[iCnt].bAttack &&
								m_tPatternDelay[iCnt].dwOffTime + m_tPatternDelay[iCnt].dwOffDelay < GetTickCount()) {
								// ���� �÷��� false
								m_tPatternDelay[iCnt].bAttack = false;
								// �̹��� ������ �ǰ���
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameCnt = 5;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameStart = 5;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameEnd = 0;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameScene = 0;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].dwFrameTime = GetTickCount();
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].dwFrameSpeed = 60;
							}
							else {
								// 4. ���� ����
								if (m_tCannonFrameArr[static_cast<UINT>(SPRITE::L_CANNON)].iFrameCnt == 0) {
									// ���� �ʱ�ȭ �� ���� ������ ���� ��� �ð� ���� �ʱ�ȭ ��
									m_tPatternDelay[iCnt].bOnFlag = false;
									m_tPatternDelay[iCnt].bOffFlag = false;
									m_tPatternDelay[iCnt].dwOnTime = GetTickCount();
									// 2000 ~ 5000 �� ����
									m_tPatternDelay[iCnt].dwOnDelay = 2000 + 100 * (rand() % 31);
								}
							}
						}
					}
					break;
					// ���� ������ ����
				case PATTERN::R_SUB:
					// ���� ���� ĳ���� �Ѵ� �ı��ǰų� ���� ĳ���� �ı��Ǿ�� ������ ����
					// �ı����� �ʾҴٸ� ���� �ߴ�
					if ((!m_tDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].bDamage || !m_tDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].bDamage) &&
						!m_tDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].bDamage) {
						break;
					}

					// �ı���ٸ� ���� �ߴ�
					if (m_tDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].bDamage) {
						break;
					}

					// 1. ó�� ���� ���� ���� ��
					if (!m_tPatternDelay[iCnt].bOnFlag) {
						// ���� �÷��� true
						m_tPatternDelay[iCnt].bOnFlag = true;
						// �κ� �̹��� ������ �ʱ�ȭ
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameCnt = 0;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameStart = 0;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameEnd = 6;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameScene = 1;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].dwFrameTime = GetTickCount();
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].dwFrameSpeed = 60;
					}
					else {
						// 2. ������ ���� ���� �ʾ��� ������ ���� ���� �ð� �� ����
						if (!m_tPatternDelay[iCnt].bOffFlag) {
							// 8��° �����ӿ��� ����
							if (m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameCnt == 6) {
								// ������ ���� �ʾҴٸ�
								if (!m_tPatternDelay[iCnt].bAttack) {
									// ���� �÷��� true
									m_tPatternDelay[iCnt].bAttack = true;
									// ���� �ð� ����
									m_tPatternDelay[iCnt].dwOffTime = GetTickCount();
									// �̹��� ������ ����
									m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameStart = 6;
								}

								// ���� �÷��� true ���� ������ ���� �� ���� �ð��� ��������
								if (m_tPatternDelay[iCnt].bAttack &&
									m_tPatternDelay[iCnt].dwOffTime + m_tPatternDelay[iCnt].dwOffDelay < GetTickCount()) {
									// ���� ������ ����
									R_FrontSub_Shot();

									// ���� ����
									m_tPatternDelay[iCnt].bOffFlag = true;
									// ���� �ð� ����
									m_tPatternDelay[iCnt].dwOffTime = GetTickCount();
								}
							}
						}
						else {
							// 3. ���� �� ���� �ݱ�
							if (m_tPatternDelay[iCnt].bAttack &&
								m_tPatternDelay[iCnt].dwOffTime + m_tPatternDelay[iCnt].dwOffDelay < GetTickCount()) {
								// ���� �÷��� false
								m_tPatternDelay[iCnt].bAttack = false;
								// �̹��� ������ �ǰ���
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameCnt = 5;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameStart = 5;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameEnd = 0;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameScene = 1;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].dwFrameTime = GetTickCount();
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].dwFrameSpeed = 60;
							}
							else {
								// 4. ���� ����
								if (m_tCannonFrameArr[static_cast<UINT>(SPRITE::R_CANNON)].iFrameCnt == 0) {
									// ���� �ʱ�ȭ �� ���� ������ ���� ��� �ð� ���� �ʱ�ȭ ��
									m_tPatternDelay[iCnt].bOnFlag = false;
									m_tPatternDelay[iCnt].bOffFlag = false;
									m_tPatternDelay[iCnt].dwOnTime = GetTickCount();
									// 2000 ~ 5000 �� ����
									m_tPatternDelay[iCnt].dwOnDelay = 2000 + 100 * (rand() % 31);
								}
							}
						}
					}
					break;
					// ���� ����
				case PATTERN::LL_SUB:
					// �ı���ٸ� ���� �ߴ�
					if (m_tDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].bDamage) {
						break;
					}

					// 1. ó�� ���� ���� ���� ��
					if (!m_tPatternDelay[iCnt].bOnFlag) {
						// ���� �÷��� true
						m_tPatternDelay[iCnt].bOnFlag = true;
						// �κ� �̹��� ������ �ʱ�ȭ
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameCnt = 0;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameStart = 0;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameEnd = 8;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameScene = 0;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].dwFrameTime = GetTickCount();
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].dwFrameSpeed = 60;
					}
					else {
						// 2. ������ ���� ���� �ʾ��� ������ ���� ���� �ð� �� ����
						if (!m_tPatternDelay[iCnt].bOffFlag) {
							// 8��° �����ӿ��� ����
							if (m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameCnt == 8) {
								// ������ ���� �ʾҴٸ�
								if (!m_tPatternDelay[iCnt].bAttack) {
									// ���� �÷��� true
									m_tPatternDelay[iCnt].bAttack = true;
									// ���� �ð� ����
									m_tPatternDelay[iCnt].dwOffTime = GetTickCount();
									// �̹��� ������ ����
									m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameStart = 8;
								}

								// ���� �÷��� true ���� ������ ���� �� ���� �ð��� ��������
								if (m_tPatternDelay[iCnt].bAttack &&
									m_tPatternDelay[iCnt].dwOffTime + m_tPatternDelay[iCnt].dwOffDelay < GetTickCount()) {
									// ������ ���� ����
									if (rand() % 2) {
										L_SideSub_Shot();
									}
									else {
										L_SideSub_Buckshot();
									}

									// ���� ����
									m_tPatternDelay[iCnt].bOffFlag = true;
									// ���� �ð� ����
									m_tPatternDelay[iCnt].dwOffTime = GetTickCount();
								}
							}
						}
						else {
							// 3. ���� �� ���� �ݱ�
							if (m_tPatternDelay[iCnt].bAttack &&
								m_tPatternDelay[iCnt].dwOffTime + m_tPatternDelay[iCnt].dwOffDelay < GetTickCount()) {
								// ���� �÷��� false
								m_tPatternDelay[iCnt].bAttack = false;
								// �̹��� ������ �ǰ���
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameCnt = 7;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameStart = 7;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameEnd = 0;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameScene = 0;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].dwFrameTime = GetTickCount();
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].dwFrameSpeed = 60;
							}
							else {
								// 4. ���� ����
								if (m_tCannonFrameArr[static_cast<UINT>(SPRITE::LL_CANNON)].iFrameCnt == 0) {
									// ���� �ʱ�ȭ �� ���� ������ ���� ��� �ð� ���� �ʱ�ȭ ��
									m_tPatternDelay[iCnt].bOnFlag = false;
									m_tPatternDelay[iCnt].bOffFlag = false;
									m_tPatternDelay[iCnt].dwOnTime = GetTickCount();
									// 2000 ~ 5000 �� ����
									m_tPatternDelay[iCnt].dwOnDelay = 2000 + 100 * (rand() % 31);
								}
							}
						}
					}
					break;
					// ������ ����
				case PATTERN::RR_SUB:
					// �ı���ٸ� ���� �ߴ�
					if (m_tDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].bDamage) {
						break;
					}

					// 1. ó�� ���� ���� ���� ��
					if (!m_tPatternDelay[iCnt].bOnFlag) {
						// ���� �÷��� true
						m_tPatternDelay[iCnt].bOnFlag = true;
						// �κ� �̹��� ������ �ʱ�ȭ
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameCnt = 0;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameStart = 0;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameEnd = 8;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameScene = 1;
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].dwFrameTime = GetTickCount();
						m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].dwFrameSpeed = 60;
					}
					else {
						// 2. ������ ���� ���� �ʾ��� ������ ���� ���� �ð� �� ����
						if (!m_tPatternDelay[iCnt].bOffFlag) {
							// 8��° �����ӿ��� ����
							if (m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameCnt == 8) {
								// ������ ���� �ʾҴٸ�
								if (!m_tPatternDelay[iCnt].bAttack) {
									// ���� �÷��� true
									m_tPatternDelay[iCnt].bAttack = true;
									// ���� �ð� ����
									m_tPatternDelay[iCnt].dwOffTime = GetTickCount();
									// �̹��� ������ ����
									m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameStart = 8;
								}

								// ���� �÷��� true ���� ������ ���� �� ���� �ð��� ��������
								if (m_tPatternDelay[iCnt].bAttack &&
									m_tPatternDelay[iCnt].dwOffTime + m_tPatternDelay[iCnt].dwOffDelay < GetTickCount()) {
									// ������ ���� ����
									if (rand() % 2) {
										R_SideSub_Shot();
									}
									else {
										R_SideSub_Buckshot();
									}

									// ���� ����
									m_tPatternDelay[iCnt].bOffFlag = true;
									// ���� �ð� ����
									m_tPatternDelay[iCnt].dwOffTime = GetTickCount();
								}
							}
						}
						else {
							// 3. ���� �� ���� �ݱ�
							if (m_tPatternDelay[iCnt].bAttack &&
								m_tPatternDelay[iCnt].dwOffTime + m_tPatternDelay[iCnt].dwOffDelay < GetTickCount()) {
								// ���� �÷��� false
								m_tPatternDelay[iCnt].bAttack = false;
								// �̹��� ������ �ǰ���
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameCnt = 7;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameStart = 7;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameEnd = 0;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameScene = 1;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].dwFrameTime = GetTickCount();
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].dwFrameSpeed = 60;
							}
							else {
								// 4. ���� ����
								if (m_tCannonFrameArr[static_cast<UINT>(SPRITE::RR_CANNON)].iFrameCnt == 0) {
									// ���� �ʱ�ȭ �� ���� ������ ���� ��� �ð� ���� �ʱ�ȭ ��
									m_tPatternDelay[iCnt].bOnFlag = false;
									m_tPatternDelay[iCnt].bOffFlag = false;
									m_tPatternDelay[iCnt].dwOnTime = GetTickCount();
									// 2000 ~ 5000 �� ����
									m_tPatternDelay[iCnt].dwOnDelay = 2000 + 100 * (rand() % 31);
								}
							}
						}
					}
					break;
					// ���� ���� ����
				case PATTERN::FRONT:
					// ���� ���� ĳ�� ���� ���̸� �� ������ �ߴ�
					if (m_tPatternDelay[static_cast<UINT>(PATTERN::BACK)].bOnFlag &&
						!m_tPatternDelay[iCnt].bAttack) {
						break;
					}

					// 1. ���� ����Ʈ ����
					if (!m_tPatternDelay[iCnt].bAttack) {
						// ���� �÷��� true
						m_tPatternDelay[iCnt].bOnFlag = true;
						// ���� �ð� ����
						m_tPatternDelay[iCnt].dwOffTime = GetTickCount();

						// ���� ����Ʈ ����
						CObj* pObj = CAbstractFactory<CEffect_Charge>::Create(m_tInfo.fX, m_tInfo.fY + (5.f * 3.f));
						pObj->Set_Target(this);
						static_cast<CEffect*>(pObj)->Set_RelativePos(0.f, 5.f * 3.f);
						CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

						CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS_ATTACK);
						CSoundMgr::Get_Instance()->PlaySound(L"Boss_Charge.wav", CSoundMgr::BOSS_ATTACK);
					}

					// 2. ���� �ð� �� ����
					if (m_tPatternDelay[iCnt].bAttack &&
						m_tPatternDelay[iCnt].dwOffTime + m_tPatternDelay[iCnt].dwOffDelay < GetTickCount()) {
						// ���� ���� ����
						Front_Shot();

						// ���� ����
						m_tPatternDelay[iCnt].bAttack = false;
						// ���� �ð� ����
						m_tPatternDelay[iCnt].dwOnTime = GetTickCount();
						// 8000 ~ 11000 �� ����
						m_tPatternDelay[iCnt].dwOnDelay = 8000 + 100 * (rand() % 31);
					}
					break;
					// ���� ���� ĳ��
				case PATTERN::BACK:
					// �ı���ٸ� ���� �ߴ�
					if (m_tDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].bDamage) {
						break;
					}

					// 1. ó�� ���� ���� ���� ��
					if (!m_tPatternDelay[iCnt].bOnFlag) {
						// ���� �÷��� true
						m_tPatternDelay[iCnt].bOnFlag = true;
						// �κ� �̹��� ������ �ʱ�ȭ
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
						// 2. ������ ���� ���� �ʾ��� ������ ���� ���� �ð� �� ����
						if (!m_tPatternDelay[iCnt].bOffFlag) {
							// 12��° �����ӿ��� ����
							if (m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameCnt == 12) {
								// ������ ���� �ʾҴٸ�
								if (!m_tPatternDelay[iCnt].bAttack) {
									// ���� �÷��� true
									m_tPatternDelay[iCnt].bAttack = true;
									// ���� �ð� ����
									m_tPatternDelay[iCnt].dwOffTime = GetTickCount();
									// �̹��� ������ ����
									m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameStart = 12;
								}

								// ���� �÷��� true ���� ������ ���� �� ���� �ð��� ��������
								if (m_tPatternDelay[iCnt].bAttack &&
									m_tPatternDelay[iCnt].dwOffTime + m_tPatternDelay[iCnt].dwOffDelay < GetTickCount()) {
									// ���� Ƚ�� ī��Ʈ ����
									++m_tPatternDelay[iCnt].dwCnt;
									
									// ������ ���� ���� �����ư��鼭 ����
									if (m_bBackAttack) {
										// �Ѿ� ���� ����
										m_fShotAngle = m_fShotAngle + 35.f;
										// ��ź
										Back_Buckshot();

										// 10�� �����ϰ� ����
										if (m_tPatternDelay[iCnt].dwCnt >= 10) {
											// ���� Ƚ�� ī��Ʈ �ʱ�ȭ
											m_tPatternDelay[iCnt].dwCnt = 0;
											// ���� ����
											m_tPatternDelay[iCnt].bOffFlag = true;
											// �Ѿ� ���� �ʱ�ȭ
											m_fShotAngle = 0.f;
											// ���������� �ٸ� ���� ����
											m_bBackAttack = !m_bBackAttack;
										}
									}
									else {
										// �Ѿ� ���� ����
										m_fShotAngle = m_fShotAngle + 38.5714f;

										// ����
										Back_AutoShot();

										// 14�� �����ϰ� ����
										if (m_tPatternDelay[iCnt].dwCnt >= 14) {
											// ���� Ƚ�� ī��Ʈ �ʱ�ȭ
											m_tPatternDelay[iCnt].dwCnt = 0;
											// ���� ����
											m_tPatternDelay[iCnt].bOffFlag = true;
											// �Ѿ� ���� �ʱ�ȭ
											m_fShotAngle = 0.f;
											// ���������� �ٸ� ���� ����
											m_bBackAttack = !m_bBackAttack;
										}
									}
									// ���� �ð� ����
									m_tPatternDelay[iCnt].dwOffTime = GetTickCount();
								}
							}
						}
						else {
							// 3. ���� �� ���� �ݱ�
							if (m_tPatternDelay[iCnt].bAttack &&
								m_tPatternDelay[iCnt].dwOffTime + m_tPatternDelay[iCnt].dwOffDelay < GetTickCount()) {
								// ���� �÷��� false
								m_tPatternDelay[iCnt].bAttack = false;
								// �̹��� ������ �ǰ���
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameCnt = 11;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameStart = 11;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameEnd = 0;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameScene = 0;
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].dwFrameTime = GetTickCount();
								m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].dwFrameSpeed = 60;
							}
							else {
								// 4. ���� ����
								if (m_tCannonFrameArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iFrameCnt == 0) {
									// ���� �ʱ�ȭ �� ���� ������ ���� ��� �ð� ���� �ʱ�ȭ ��
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
		// �ı�
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
		// FINAL_1 ~ FINAL_4 ���� �ı��Ǹ鼭 ������ �Ѿ��� ��Ѹ�
	case STATE::FINAL_1:
		if (m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			m_eCurState = STATE::FINAL_2;
			Destroy_Shot(20.f);
			Destroy_Shot(50.f);
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 100;

			CObj* pObj = CAbstractFactory<CExplosion_04>::Create(m_tInfo.fX, m_tInfo.fY);		// �ı� ����Ʈ ����
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
			Destroy_Shot(-15.f);
			Destroy_Shot(-45.f);
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 100;

			CObj* pObj = CAbstractFactory<CExplosion_04>::Create(m_tInfo.fX, m_tInfo.fY);		// �ı� ����Ʈ ����
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
			Destroy_Shot(-5.f);
			Destroy_Shot(25.f);
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 100;

			CObj* pObj = CAbstractFactory<CExplosion_04>::Create(m_tInfo.fX, m_tInfo.fY);		// �ı� ����Ʈ ����
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
			Destroy_Shot(5.f);
			Destroy_Shot(-25.f);
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 100;

			CObj* pObj = CAbstractFactory<CExplosion_04>::Create(m_tInfo.fX, m_tInfo.fY);		// �ı� ����Ʈ ����
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
		// ��� ��ũ���� ���� ���°� �ƴϸ�
		if (!g_bStopMapScroll)
		{
			// �� ������ ����
			m_tInfo.fY = m_tInfo.fY + 4.f;
		}
		break;
	}


	// �ı���
	if (m_bDestroy)
	{
		// �ı� ����Ʈ ���
		if (m_dwDestroyTime + m_dwDestroyDelay < GetTickCount()) {
			// ���� �ð� ����
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

// ���� �̵�
void CTequilaGround::Move()
{
	// ������ ���� ������ ����
	if (m_bHpLock || m_tInfo.fY < 300.f)
		return;

	// �¿� �̵� ����
	if (m_fSpeed == 0)
	{
		// ������ �̵�
		if (rand() % 2)
		{
			m_fSpeed = 0.2f;
		}
		// ���� �̵�
		else
		{
			m_fSpeed = -0.2f;
		}
	}
	// ������ �̵�
	else if (m_fSpeed >= 0.2f)
	{
		// ������ ���� �ٿ����� ���� ��ȯ
		if (m_tInfo.fX + m_fSpeed >= (WINCX >> 1) + 20.f)
		{
			// ����
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
	// ���� �̵�
	else if (m_fSpeed <= -0.2f)
	{
		// ���� ���� �ٿ����� ���� ��ȯ
		if (m_tInfo.fX + m_fSpeed <= (WINCX >> 1) - 20.f)
		{
			// ����
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
	// �̵�
	float fMovePosX = m_tInfo.fX + m_fSpeed;
	if ((WINCX >> 1) - 20.f < fMovePosX && fMovePosX < (WINCX >> 1) + 20.f)
	{
		m_tInfo.fX = m_tInfo.fX + m_fSpeed;
	}
}

// �ı� ����Ʈ
// ���� �ı��� �Ǹ� �ı��� ��ġ ��ǥ��
// iEffectCnt�� 0�̵� ������ ����Ʈ ����
void CTequilaGround::DamageEffect()
{
	CObj* pObj;
	LONG lCenterX;
	LONG lCenterY;

	if (m_tDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].bDamage && m_tDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].iEffectCnt != 0)
	{
		if (m_tDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].dwTime + m_tDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].dwDelay < GetTickCount())
		{
			--m_tDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].iEffectCnt;

			lCenterX = (m_vecHitRect[static_cast<UINT>(SPRITE::L_CANNON)].right - m_vecHitRect[static_cast<UINT>(SPRITE::L_CANNON)].left) >> 1;
			lCenterY = (m_vecHitRect[static_cast<UINT>(SPRITE::L_CANNON)].bottom - m_vecHitRect[static_cast<UINT>(SPRITE::L_CANNON)].top) >> 1;

			// ����Ʈ ����
			pObj = CAbstractFactory<CExplosion_05>::Create(
				static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::L_CANNON)].left + lCenterX),
				static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::L_CANNON)].top + lCenterY));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			m_tDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].dwTime = GetTickCount();
		}
	}

	if (m_tDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].bDamage && m_tDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].iEffectCnt != 0)
	{
		if (m_tDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].dwTime + m_tDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].dwDelay < GetTickCount())
		{
			--m_tDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].iEffectCnt;

			lCenterX = (m_vecHitRect[static_cast<UINT>(SPRITE::R_CANNON)].right - m_vecHitRect[static_cast<UINT>(SPRITE::R_CANNON)].left) >> 1;
			lCenterY = (m_vecHitRect[static_cast<UINT>(SPRITE::R_CANNON)].bottom - m_vecHitRect[static_cast<UINT>(SPRITE::R_CANNON)].top) >> 1;

			// ����Ʈ ����
			pObj = CAbstractFactory<CExplosion_05>::Create(
				static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::R_CANNON)].left + lCenterX),
				static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::R_CANNON)].top + lCenterY));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			m_tDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].dwTime = GetTickCount();
		}
	}

	if (m_tDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].bDamage && m_tDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].iEffectCnt != 0)
	{
		if (m_tDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].dwTime + m_tDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].dwDelay < GetTickCount())
		{
			--m_tDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].iEffectCnt;

			lCenterX = (m_vecHitRect[static_cast<UINT>(SPRITE::LL_CANNON)].right - m_vecHitRect[static_cast<UINT>(SPRITE::LL_CANNON)].left) >> 1;
			lCenterY = (m_vecHitRect[static_cast<UINT>(SPRITE::LL_CANNON)].bottom - m_vecHitRect[static_cast<UINT>(SPRITE::LL_CANNON)].top) >> 1;

			// ����Ʈ ����
			pObj = CAbstractFactory<CExplosion_05>::Create(
				static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::LL_CANNON)].left + lCenterX),
				static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::LL_CANNON)].top + lCenterY));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			m_tDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].dwTime = GetTickCount();
		}
	}

	if (m_tDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].bDamage && m_tDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].iEffectCnt != 0)
	{
		if (m_tDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].dwTime + m_tDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].dwDelay < GetTickCount())
		{
			--m_tDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].iEffectCnt;

			lCenterX = (m_vecHitRect[static_cast<UINT>(SPRITE::RR_CANNON)].right - m_vecHitRect[static_cast<UINT>(SPRITE::RR_CANNON)].left) >> 1;
			lCenterY = (m_vecHitRect[static_cast<UINT>(SPRITE::RR_CANNON)].bottom - m_vecHitRect[static_cast<UINT>(SPRITE::RR_CANNON)].top) >> 1;

			// ����Ʈ ����
			pObj = CAbstractFactory<CExplosion_05>::Create(
				static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::RR_CANNON)].left + lCenterX),
				static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::RR_CANNON)].top + lCenterY));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			m_tDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].dwTime = GetTickCount();
		}
	}

	if (m_tDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].bDamage && m_tDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iEffectCnt != 0)
	{
		if (m_tDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].dwTime + m_tDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].dwDelay < GetTickCount())
		{
			--m_tDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].iEffectCnt;

			lCenterX = (m_vecHitRect[static_cast<UINT>(SPRITE::CENTER_CANNON)].right - m_vecHitRect[static_cast<UINT>(SPRITE::CENTER_CANNON)].left) >> 1;
			lCenterY = (m_vecHitRect[static_cast<UINT>(SPRITE::CENTER_CANNON)].bottom - m_vecHitRect[static_cast<UINT>(SPRITE::CENTER_CANNON)].top) >> 1;

			// ����Ʈ ����
			pObj = CAbstractFactory<CExplosion_05>::Create(
				static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::CENTER_CANNON)].left + lCenterX),
				static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::CENTER_CANNON)].top + lCenterY));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			m_tDamageFlagArr[static_cast<UINT>(SPRITE::CENTER_CANNON)].dwTime = GetTickCount();
		}
	}
}

// ���� ���� ĳ��
void CTequilaGround::L_SideSub_Shot()
{
	float fX;
	float fY;
	float fDia;
	float fRad;
	float fAngle;
	CObj* pObj = nullptr;

	// ���� ���� 2
	if (!m_tDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].bDamage) {
		// ���� ���ϱ�
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

// ������ ���� ĳ��
void CTequilaGround::R_SideSub_Shot()
{
	float fX;
	float fY;
	float fDia;
	float fRad;
	float fAngle;
	CObj* pObj = nullptr;

	// ������ ���� 2
	if (!m_tDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].bDamage) {
		// ���� ���ϱ�
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

// ���� ���� ĳ�� ��ź
void CTequilaGround::L_SideSub_Buckshot()
{
	float fX;
	float fY;
	float fDia;
	float fRad;
	float fAngle;
	CObj* pObj = nullptr;

	// ���� ���� 2
	if (!m_tDamageFlagArr[static_cast<UINT>(SPRITE::LL_CANNON)].bDamage) {
		// ���� ���ϱ�
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

// ������ ���� ĳ�� ��ź
void CTequilaGround::R_SideSub_Buckshot()
{
	float fX;
	float fY;
	float fDia;
	float fRad;
	float fAngle;
	CObj* pObj = nullptr;

	// ������ ���� 2
	if (!m_tDamageFlagArr[static_cast<UINT>(SPRITE::RR_CANNON)].bDamage) {
		// ���� ���ϱ�
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

// ���� ĳ��
void CTequilaGround::Front_Shot()
{
	// ����
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

// ������ ���� ���� ĳ��
void CTequilaGround::L_FrontSub_Shot()
{
	float fX;
	float fY;
	float fDia;
	float fRad;
	float fAngle;
	CObj* pObj = nullptr;

	// ���� ���� 1
	if (!m_tDamageFlagArr[static_cast<UINT>(SPRITE::L_CANNON)].bDamage) {
		// ���� ���ϱ�
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

// ������ ���� ���� ĳ��
void CTequilaGround::R_FrontSub_Shot()
{
	float fX;
	float fY;
	float fDia;
	float fRad;
	float fAngle;
	CObj* pObj = nullptr;

	// ������ ���� 1
	if (!m_tDamageFlagArr[static_cast<UINT>(SPRITE::R_CANNON)].bDamage) {
		// ���� ���ϱ�
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

// ���� ���� ����
void CTequilaGround::Back_AutoShot()
{
	// ����
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


	// ������
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

// ���� ��ź ����
void CTequilaGround::Back_Buckshot()
{
	// ����
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

	// ��Ÿ�̹� �ʰ� �� ������
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


	// ������
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

	// ��Ÿ�̹� �ʰ� �� ������
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

void CTequilaGround::Destroy_Shot(float _fAngle)
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

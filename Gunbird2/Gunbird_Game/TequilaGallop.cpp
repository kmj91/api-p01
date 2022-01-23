// �����
// 2 ������ ����
// �� ���� ���� �� ���� �� �κ��� Hit Rect�� �̵� ��
// ���� �ı��Ǹ� �ݴ� �������� �ݵ� �̵� ��

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
	// �̹��� ũ�� ����
	m_tInfo.iCX = TEQUILA_GALLOP_WIDTH * 3;
	m_tInfo.iCY = TEQUILA_GALLOP_HEIGHT * 3;
	m_iImageWidth = TEQUILA_GALLOP_WIDTH;
	m_iImageHeight = TEQUILA_GALLOP_HEIGHT;
	// ü��
	m_fHp = 5000;
	//m_fHp = 100;
	// �浹 ��Ʈ ��ġ �� �κ� ü�� ����
	m_vecHitRectPos.reserve(3);
	m_vecHitRectPos.emplace_back(RECT{ 73 * 3, 55 * 3, 113 * 3, 131 * 3 });		// �ε��� 0�� ��ü
	m_vecHitRectPos.emplace_back(RECT{ 24 * 3, 55 * 3, 73 * 3, 122 * 3 });		// �ε��� 1�� ����
	m_vecHitRectPos.emplace_back(RECT{ 113 * 3, 55 * 3, 162 * 3, 122 * 3 });	// �ε��� 2�� ������
	m_vecHitRect.reserve(3);
	m_vecHitRect.emplace_back(RECT{});
	m_vecHitRect.emplace_back(RECT{});
	m_vecHitRect.emplace_back(RECT{});
	m_vecRectHp.emplace_back(5000.f);
	m_vecRectHp.emplace_back(1800.f);
	m_vecRectHp.emplace_back(1800.f);
	// �⺻ ����
	m_eCurState = STATE::WAIT;
	// ���� ����
	m_dwShotDelay = 2000;
	// ���� �ð� ����
	m_dwShotTime = GetTickCount();

	// ����
	m_vecPattern.reserve(6);
	m_vecPattern.emplace_back(STATE::R_ARM_ATTACK);
	m_vecPattern.emplace_back(STATE::FRONT_ATTACK);
	m_vecPattern.emplace_back(STATE::BACK_ATTACK);
	m_vecPattern.emplace_back(STATE::L_ARM_ATTACK);
	m_vecPattern.emplace_back(STATE::SHOULDER_ATTACK);
	m_vecPattern.emplace_back(STATE::BACK_ATTACK);
	// ���� �ı� ����Ʈ ���� �ʱ�ȭ
	m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].iEffectCnt = 30;
	m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].dwDelay = 50;
	m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].iEffectCnt = 30;
	m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].dwDelay = 50;

	// ����
	m_iScore = 5000;
}

// ������Ʈ
int CTequilaGallop::Update()
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

			// �ı� ����Ʈ ����
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

			// �����߿� ������� ���� ���ݻ��·� ��ȯ�Ǹ鼭 �״� ��찡 ����
			// �׷��� ���� �ٷ� ��ȯ
			Scene_Change();
		}
	}

	// ���� �ൿ
	Action();
	// �̵�
	Move();
	// ������ �ı� ����Ʈ
	DamageEffect();
	// ��ü ����
	// �ֱ������� ��鸲
	Shake_Body();
	// �̹��� RECT ���� �� Hit RECT ���� ����
	// Obj�� �ƴ� Boss�� Update_Rect ȣ��
	Update_Rect();
	// ������ �� ���� ó��
	Scene_Change();
	// �κ� �̹��� ������ �̵�
	Frame_MoveEX();

	return OBJ_NOEVENT;
}

// ����Ʈ ������Ʈ
void CTequilaGallop::Late_Update()
{
	// �� �ϴ� �ٱ����� ������ ����
	if (WINCY <= m_tRect.top)
		m_bRemove = true;
}

// ����
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
		// ��ü
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TequilaGallop_Under");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));

		// ��ü
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TequilaGallop_Body");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top + m_fBodyPosY
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt * m_iImageWidth, m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));

		// ��
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TequilaGallop_Arm");
		// ����
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

		// ������
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

void CTequilaGallop::Release()
{
}

// ���� ������ �ı� ������ üũ
void CTequilaGallop::Check_Damage(int _iPlayerNum)
{
	CObj* pObj;
	LONG lCenterX;
	LONG lCenterY;

	// ����
	if (!m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].bDamage && m_vecRectHp[static_cast<UINT>(SPRITE::L_ARM)] <= 0) {
		m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].bDamage = true;

		lCenterX = (m_vecHitRect[static_cast<UINT>(SPRITE::L_ARM)].right - m_vecHitRect[static_cast<UINT>(SPRITE::L_ARM)].left) >> 1;
		lCenterY = (m_vecHitRect[static_cast<UINT>(SPRITE::L_ARM)].bottom - m_vecHitRect[static_cast<UINT>(SPRITE::L_ARM)].top) >> 1;

		pObj = CObjFactory<CExplosion_02_1>::Create(
			static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::L_ARM)].left + lCenterX),
			static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::L_ARM)].top + lCenterY));
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		// �浹 ��Ʈ ����
		m_vecHitRectPos[static_cast<UINT>(SPRITE::L_ARM)] = { 0, 0, 0, 0 };

		// ���� �ı� ���� ȹ��
		CGameUIMgr::Get_Instance()->Add_Score(4000, _iPlayerNum);
	}

	// ������
	if (!m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].bDamage && m_vecRectHp[static_cast<UINT>(SPRITE::R_ARM)] <= 0) {
		m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].bDamage = true;

		lCenterX = (m_vecHitRect[static_cast<UINT>(SPRITE::R_ARM)].right - m_vecHitRect[static_cast<UINT>(SPRITE::R_ARM)].left) >> 1;
		lCenterY = (m_vecHitRect[static_cast<UINT>(SPRITE::R_ARM)].bottom - m_vecHitRect[static_cast<UINT>(SPRITE::R_ARM)].top) >> 1;

		pObj = CObjFactory<CExplosion_02_1>::Create(
			static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::R_ARM)].left + lCenterX),
			static_cast<float>(m_vecHitRect[static_cast<UINT>(SPRITE::R_ARM)].top + lCenterY));
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		// �浹 ��Ʈ ����
		m_vecHitRectPos[static_cast<UINT>(SPRITE::R_ARM)] = { 0, 0, 0, 0 };

		// ���� �ı� ���� ȹ��
		CGameUIMgr::Get_Instance()->Add_Score(4000, _iPlayerNum);
	}
}

// ������ �� ���� ó��
void CTequilaGallop::Scene_Change()
{
	// ���� ���¿� �ٸ��� ��ü
	if (m_ePreState != m_eCurState)
	{
		// ������ ���� ���������� �����Ǿ�����
		switch (m_eCurState)
		{
			// ���
		case STATE::WAIT:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 40;
			// ������ �̵�
		case STATE::PHASE_MOVE:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 40;
			break;
			// ������ ����
		case STATE::PHASE_CHANGE:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 40;
			break;
			// �غ�
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
			// �̵�
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
			// ���� ����
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
			// ������ ����
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
			// ���� ����
		case STATE::FRONT_ATTACK:
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameStart = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameScene = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameTime = GetTickCount();
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameSpeed = 999999;
			break;
			// �� ����
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
			// ��� ����
		case STATE::SHOULDER_ATTACK:
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameStart = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameScene = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameTime = GetTickCount();
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameSpeed = 999999;
			break;
			// �ı�
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

			// ��ũ
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameStart = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd = 3;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameScene = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameTime = GetTickCount();
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameSpeed = 100;

			// ���̵�
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameCnt = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameStart = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameEnd = 3;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameScene = 0;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].dwFrameTime = GetTickCount();
			m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].dwFrameSpeed = 100;

			// ���
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

// �κ� ��������Ʈ ĳ�� ������ ������Ʈ
void CTequilaGallop::Frame_MoveEX()
{
	// ��ü
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameCnt;
		if (m_tFrame.iFrameCnt > m_tFrame.iFrameEnd)
		{
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			++m_dwUnderFrameCnt;

			// ���� �ŷ����� �ٽ� ������ ����
			if (m_tFrame.iFrameEnd == 19)
			{
				m_dwUnderFrameCnt = 0;
				m_tFrame.iFrameEnd = 3;
			}

			// ����� �ֱ�� ���� ���� �ŷ��ߵ�
			if (m_dwUnderFrameCnt == 60)
			{
				m_tFrame.iFrameEnd = 19;
			}
		}

		m_tFrame.dwFrameTime = GetTickCount();
	}

	// �κ� ��������Ʈ
	// ��ü, ����, ������
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

// ���� �ൿ
void CTequilaGallop::Action()
{
	switch (m_ePreState)
	{
		// ���
		// �ٷ� ���������ʰ� ��ٸ�
	case STATE::WAIT:
		if (m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			m_eCurState = STATE::PHASE_MOVE;

			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS_ATTACK);
			CSoundMgr::Get_Instance()->PlaySound(L"Boss_PhaseChange.wav", CSoundMgr::BOSS_ATTACK);
		}
		break;
		// ������ �̵�
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
		// ������ ����
	case STATE::PHASE_CHANGE:
		if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		{
			m_eCurState = STATE::READY;

			// ��ü ������
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 10;
		}
		break;
		// ���� 2���� �غ�
	case STATE::READY:
		if (m_bHpLock)
		{
			if (m_tInfo.fY >= 0)
			{
				m_bHpLock = false;		// �������� ���� �� ����
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
		// �̵�
	case STATE::NEXT_PATTERN:
		// ���� ���� ��� �ð��� ���� ������ �ʾ�����
		if (m_dwShotTime + m_dwShotDelay > GetTickCount())
			break;

		while (true)
		{
			// �̹� ���� ���� ����
			m_eCurState = m_vecPattern[m_dwPatternIndex];
			// ���� ���� ���� �ε����� 
			m_dwPatternIndex = (m_dwPatternIndex + 1) % m_vecPattern.size();
			// ���� ���� �����ε� �ı��� ���¸� ���� ����
			if (m_eCurState == STATE::L_ARM_ATTACK &&
				m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].bDamage)
			{
				continue;
			}
			// ������ ���� �����ε� �ı��� ���¸� ���� ����
			else if (m_eCurState == STATE::R_ARM_ATTACK &&
				m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].bDamage)
			{
				continue;
			}

			// �ݺ��� ����
			break;
		}
		break;
		// ���� ����
	case STATE::L_ARM_ATTACK:
		// ���� ���� �ı� ��ٸ� �ߴ��ؾߵ�
		if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].bDamage)
		{
			// ���� ������ ���� �ʱ�ȭ
			m_bAttack = false;
			m_eCurState = STATE::NEXT_PATTERN;
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 1000;
			break;
		}

		// 5��° ��������Ʈ �����ӿ� ������
		if (!m_bAttack && m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameCnt == 5)
		{
			// ����
			m_bAttack = true;
			// ���� ����
			m_dwShotDelay = 100;
			// �Ѿ� ���� �ʱ�ȭ
			m_fShotAngle = 0.f;
			// ���� Ƚ�� �ʱ�ȭ
			m_dwShotCnt = 0;

			// ��ü ��������Ʈ ����
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd = 9;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameSpeed = 100;
		}

		// ����
		if (m_bAttack &&
			m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			// ��� ���� ����
			m_fShotAngle = m_fShotAngle + 18.f;
			// �Ѿ� ī��Ʈ ����
			++m_dwShotCnt;
			// 7�� ������ ����
			if (m_dwShotCnt >= 7) {
				m_bAttack = false;
			}
			// ���� ����
			L_Arm_Shot();
			// ���� ���� �ð� ����
			m_dwShotTime = GetTickCount();
		}

		// ��ü ������ ����
		if (m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt == m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd)
		{
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameStart = m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd;
		}

		// ���� ��
		if (m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameCnt == m_tSubFrameArr[static_cast<UINT>(SPRITE::L_ARM)].iFrameEnd)
		{
			// ���� ������ ���� �ʱ�ȭ
			m_bAttack = false;
			m_eCurState = STATE::NEXT_PATTERN;
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 1000;

			// ��ⱸ ����Ʈ ���
			Pipe_Smoke();
		}

		break;
		// ������ ����
	case STATE::R_ARM_ATTACK:
		// ���� ���� �ı� ��ٸ� �ߴ��ؾߵ�
		if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].bDamage)
		{
			// ���� ������ ���� �ʱ�ȭ
			m_bAttack = false;
			m_eCurState = STATE::NEXT_PATTERN;
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 1000;
			break;
		}

		// 5��° ��������Ʈ �����ӿ� ������
		if (!m_bAttack && m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameCnt == 5)
		{
			// ����
			m_bAttack = true;
			// ���� ����
			m_dwShotDelay = 100;
			// �Ѿ� ���� �ʱ�ȭ
			m_fShotAngle = 0.f;
			// ���� Ƚ�� �ʱ�ȭ
			m_dwShotCnt = 0;

			// ��ü ��������Ʈ ����
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd = 9;
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].dwFrameSpeed = 100;
		}

		// ����
		if (m_bAttack &&
			m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			// ��� ���� ����
			m_fShotAngle = m_fShotAngle + 18.f;
			// �Ѿ� ī��Ʈ ����
			++m_dwShotCnt;
			// 7�� ������ ����
			if (m_dwShotCnt >= 7) {
				m_bAttack = false;
			}
			// ������ ����
			R_Arm_Shot();
			// ���� ���� �ð� ����
			m_dwShotTime = GetTickCount();
		}

		// ��ü ������ ����
		if (m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt == m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd)
		{
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameStart = m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd;
		}

		// ���� ��
		if (m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameCnt == m_tSubFrameArr[static_cast<UINT>(SPRITE::R_ARM)].iFrameEnd)
		{
			// ���� ������ ���� �ʱ�ȭ
			m_bAttack = false;
			m_eCurState = STATE::NEXT_PATTERN;
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 1000;

			// ��ⱸ ����Ʈ ���
			Pipe_Smoke();
		}

		break;
		// ���� ����
	case STATE::FRONT_ATTACK:
		// �⸦ ������ ������
		if (!m_bAttack)
		{
			// ����
			m_bAttack = true;
			// ���� ���� �ð� ����
			m_dwChargeTime = GetTickCount();
			// ���� ����
			m_dwShotDelay = 200;
			// ���� Ƚ�� �ʱ�ȭ
			m_dwShotCnt = 0;

			// ���� ����Ʈ ����
			// ����
			CObj* pObj = CObjFactory<CEffect_Charge>::Create(m_tInfo.fX - (14.f * 3.f), m_tInfo.fY + (27.f * 3.f));
			pObj->Set_Target(this);
			static_cast<CEffect*>(pObj)->Set_RelativePos(-14.f * 3.f, 27.f * 3.f);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			// ������
			pObj = CObjFactory<CEffect_Charge>::Create(m_tInfo.fX + (14.f * 3.f), m_tInfo.fY + (27.f * 3.f));
			pObj->Set_Target(this);
			pObj->Set_SceneID(1);
			static_cast<CEffect*>(pObj)->Set_RelativePos(14.f * 3.f, 27.f * 3.f);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS_ATTACK);
			CSoundMgr::Get_Instance()->PlaySound(L"Boss_Charge.wav", CSoundMgr::BOSS_ATTACK);
		}

		
		// ���� �ð��� ������ �ʾ�����
		if (m_dwChargeTime + m_dwChargeDelay > GetTickCount())
			break;

		// ����
		if (m_bAttack &&
			m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			// �Ѿ� ī��Ʈ ����
			++m_dwShotCnt;
			// 6�� ������ ����
			if (m_dwShotCnt >= 6)
			{
				// ���� ������ ���� �ʱ�ȭ
				m_bAttack = false;
				m_eCurState = STATE::NEXT_PATTERN;
				m_dwShotTime = GetTickCount();
				m_dwShotDelay = 1000;
			}

			// ��,�� �����ư��鼭 ������
			if (m_dwShotCnt % 2)
			{
				// ������
				Front_Shot(14.f * 3.f, 27.f * 3.f);
			}
			else
			{
				// ����
				Front_Shot(-14.f * 3.f, 27.f * 3.f);
			}

			m_dwShotTime = GetTickCount();
		}

		break;
		// �� ����
	case STATE::BACK_ATTACK:
		// �� ���� ���� �� ���� ��ǥ�� �ٲ�
		// ������ ���࿡ ���� ���� ��ǥ ����
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


		// 8��° �����ӿ��� ���� ������
		if (!m_bAttack && m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt == 8)
		{
			// ����
			m_bAttack = true;
			// ���� ����
			m_dwShotDelay = 400;
			// ���� Ƚ�� �ʱ�ȭ
			m_dwShotCnt = 0;
			// ������ ����
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameStart = 8;
		}

		// ����
		if (m_bAttack &&
			m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			// �Ѿ� ī��Ʈ ����
			++m_dwShotCnt;
			// 10�� ������ ����
			if (m_dwShotCnt >= 10)
			{
				m_bAttack = false;
				// �������� �ٽ� ó������ �ǵ��ư��� ��
				m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt = m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd - 1;
				m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameStart = m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd;
				m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd = 0;
			}

			// ��,�� �����ư��鼭 ������
			if (m_dwShotCnt % 2)
			{
				// ������
				Back_Shot(37.f * 3.f, -15.f * 3.f);
			}
			else
			{
				// ����
				Back_Shot(-37.f * 3.f, -15.f * 3.f);
			}

			m_dwShotTime = GetTickCount();
		}

		// ���� ��
		if (!m_bAttack &&
			m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameCnt == m_tSubFrameArr[static_cast<UINT>(SPRITE::BODY)].iFrameEnd)
		{
			// ���� ������ ���� �ʱ�ȭ
			m_eCurState = STATE::NEXT_PATTERN;
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 1000;
			m_fArmPosY = 0.f;

			// ��ⱸ ����Ʈ ���
			Pipe_Smoke();
		}

		break;
		// ��� ����
	case STATE::SHOULDER_ATTACK:
		// ���� ���� �ı� ��ٸ� �ߴ��ؾߵ�
		if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].bDamage && m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].bDamage)
		{
			// ���� ������ ���� �ʱ�ȭ
			m_eCurState = STATE::NEXT_PATTERN;
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 1000;
			break;
		}

		// ó�� ����
		if (!m_bAttack)
		{
			// ���� ����
			m_bAttack = true;
			// ���� ���� �ð� ����
			m_dwShotTime = GetTickCount();
			// ���� ������
			m_dwShotDelay = 600;
			// ���� Ƚ�� �ʱ�ȭ
			m_dwShotCnt = 0;
		}

		// ����
		if (m_bAttack &&
			m_dwShotTime + m_dwShotDelay < GetTickCount())
		{
			// �Ѿ� ī��Ʈ ����
			++m_dwShotCnt;
			// 2�� ������ ����
			if (m_dwShotCnt >= 2)
			{
				m_bAttack = false;
			}

			Shoulder_Shot();
			m_dwShotTime = GetTickCount();
		}

		// ���� ��
		if (!m_bAttack)
		{
			// ���� ������ ���� �ʱ�ȭ
			m_eCurState = STATE::NEXT_PATTERN;
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 1000;
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

			CObj* pObj = CObjFactory<CExplosion_04>::Create(m_tInfo.fX, m_tInfo.fY);		// �ı� ����Ʈ ����
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

			CObj* pObj = CObjFactory<CExplosion_04>::Create(m_tInfo.fX, m_tInfo.fY);		// �ı� ����Ʈ ����
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

			CObj* pObj = CObjFactory<CExplosion_04>::Create(m_tInfo.fX, m_tInfo.fY);		// �ı� ����Ʈ ����
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

			CObj* pObj = CObjFactory<CExplosion_04>::Create(m_tInfo.fX, m_tInfo.fY);		// �ı� ����Ʈ ����
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
			// �� ������ ����
			m_tInfo.fY = m_tInfo.fY + 3.f;

			if (!g_bStageClear && m_tInfo.fY > 450.f) {
				// �������� Ŭ���� �÷��� true
				g_bStageClear = true;
				// Ŭ���� ����
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
				CSoundMgr::Get_Instance()->PlaySound(L"Stage_Clear.wav", CSoundMgr::BGM);
			}
		}
		break;
	}
}

// �̵�
// �⺻������ �¿�� �ݺ� �̵��� ��
// ���� �ı��� �Ǹ� �ݴ� �������� ������ �ݵ� �̵� ��
void CTequilaGallop::Move()
{
	// ���� ���� ����Ʈ ����
	switch (m_ePreState)
	{
	case CTequilaGallop::STATE::WAIT:
	case CTequilaGallop::STATE::PHASE_MOVE:
		if (m_dwWhellSmokeTime + m_dwWhellSmokeDelay < GetTickCount()) {
			m_dwWhellSmokeTime = GetTickCount();

			// ���� �� ����
			CObj* pObj = CObjFactory<CWheelSmoke>::Create(m_tInfo.fX - (26.f * 3.f), m_tInfo.fY - (24.f * 3.f));
			pObj->Set_Speed(7.f);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::BACK_EFFECT);
			// ������ �� ����
			pObj = CObjFactory<CWheelSmoke>::Create(m_tInfo.fX + (26.f * 3.f), m_tInfo.fY - (24.f * 3.f));
			pObj->Set_Speed(7.f);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::BACK_EFFECT);
			// ���� �Ʒ� ����
			pObj = CObjFactory<CWheelSmoke>::Create(m_tInfo.fX - (26.f * 3.f), m_tInfo.fY + (76.f * 3.f));
			pObj->Set_Speed(7.f);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			// ������ �Ʒ� ����
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
		// �̵� ����
		if (m_dwSoundTime + m_dwSoundDelay < GetTickCount()) {
			m_dwSoundTime = GetTickCount();

			// �̵� ����
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

			// ���� �� ����
			CObj* pObj = CObjFactory<CWheelSmoke>::Create(m_tInfo.fX - (60.f * 3.f), m_tInfo.fY - (2.f * 3.f));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::BACK_EFFECT);
			// ������ �� ����
			pObj = CObjFactory<CWheelSmoke>::Create(m_tInfo.fX + (60.f * 3.f), m_tInfo.fY - (2.f * 3.f));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::BACK_EFFECT);
			// ���� �Ʒ� ����
			pObj = CObjFactory<CWheelSmoke>::Create(m_tInfo.fX - (40.f * 3.f), m_tInfo.fY + (70.f * 3.f));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			// ������ �Ʒ� ����
			pObj = CObjFactory<CWheelSmoke>::Create(m_tInfo.fX + (40.f * 3.f), m_tInfo.fY + (70.f * 3.f));
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		}
		break;
	}

	if (m_bHpLock || m_tInfo.fY < 200.f)
		return;

	// ���� �ı� �̵� �˻�
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

	// �¿� �̵� ����
	if (m_fSpeed == 0)
	{
		// ������ �̵�
		if (rand() % 2)
		{
			m_fSpeed = 1.f;
		}
		// ���� �̵�
		else
		{
			m_fSpeed = -1.f;
		}
	}
	// ������ �̵�
	else if (m_fSpeed >= 1.f)
	{
		// ������ ���� �ٿ����� ���� ��ȯ
		if (m_tInfo.fX + m_fSpeed >= WINCX - 200.f)
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
					m_fSpeed = -1.f;
					m_bMoveFlag = false;
				}
			}
		}
	}
	// ���� �̵�
	else if (m_fSpeed <= -1.f)
	{
		// ���� ���� �ٿ����� ���� ��ȯ
		if (m_tInfo.fX + m_fSpeed <= 200.f)
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
					m_fSpeed = 1.f;
					m_bMoveFlag = false;
				}
			}
		}
	}
	// �̵�
	float fMovePosX = m_tInfo.fX + m_fSpeed;
	if (200.f < fMovePosX && fMovePosX < WINCX - 200.f)
	{
		m_tInfo.fX = m_tInfo.fX + m_fSpeed;
	}
}

// �ı� ����Ʈ
void CTequilaGallop::DamageEffect()
{
	CObj* pObj;

	if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].bDamage && m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].iEffectCnt != 0)
	{
		if (m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].dwTime + m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].dwDelay < GetTickCount())
		{
			--m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].iEffectCnt;

			// ����Ʈ ����
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

			// ����Ʈ ����
			pObj = CObjFactory<CExplosion_05>::Create(m_tInfo.fX + (30.f * 3.f), m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::FRONT_EFFECT);

			m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].dwTime = GetTickCount();
		}
	}
}

// ��ü ����
// �ֱ������� ��鸲
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

// ��ⱸ ����Ʈ ���
void CTequilaGallop::Pipe_Smoke()
{
	// ����
	CObj* pObj = CObjFactory<CPipeSmoke>::Create(m_tInfo.fX - (16.f * 3.f), m_tInfo.fY - (66.f * 3.f));
	pObj->Set_Target(this);
	static_cast<CEffect*>(pObj)->Set_RelativePos(-16.f * 3.f, -66.f * 3.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
	// ������
	pObj = CObjFactory<CPipeSmoke>::Create(m_tInfo.fX + (16.f * 3.f), m_tInfo.fY - (66.f * 3.f));
	pObj->Set_Target(this);
	pObj->Set_SceneID(1);
	static_cast<CEffect*>(pObj)->Set_RelativePos(16.f * 3.f, -66.f * 3.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
}

// ���� ����
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

// ������ ����
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

// ���� ����
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

// �� ����
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

// ��� ����
void CTequilaGallop::Shoulder_Shot()
{
	CObj* pObj;

	// ���� ���� �ı����� �ʾ����� ����
	if (!m_bDamageFlagArr[static_cast<UINT>(SPRITE::L_ARM)].bDamage)
	{
		// ���� 1
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

		// ���� 2
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

		// ���� 3
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

		// ���� 4
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

	

	// ������ ���� �ı����� �ʾ����� ����
	if (!m_bDamageFlagArr[static_cast<UINT>(SPRITE::R_ARM)].bDamage)
	{
		// ������ 1
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

		// ������ 2
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

		// ������ 3
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

		// ������ 4
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

// �ı��� �� ����
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

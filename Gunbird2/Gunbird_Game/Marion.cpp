// �����
// �÷��̾� ������
// ��� �ִϸ��̼� �������� �̹��� ���̰� �޶�
// ���°� ����� �� ��ġ ��ǥ�� �ʱ�ȭ�� �� �����ؾ� ��

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
	// �⺻ ��ġ �� ����
	m_tInfo.fX = 200;
	m_tInfo.fY = WINCY + (MARION_MOVE_HEIGHT * 3 >> 1);
	m_tInfo.iCX = MARION_MOVE_WIDTH * 3;
	m_tInfo.iCY = MARION_MOVE_HEIGHT * 3;
	m_tHitRectPos = { 9 * 3, 8 * 3, 15 * 3, 19 * 3 };
	m_iImageWidth = MARION_MOVE_WIDTH;
	m_iImageHeight = MARION_MOVE_HEIGHT;
	
	// �⺻ �ִϸ��̼� ������ ����
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 100;

	// �̵� �ӵ�
	m_fSpeed = 8.f;

	// �⺻ ���� - ������
	m_ePreState = PLAYER::RESPAWN;
	m_eCurState = PLAYER::RESPAWN;
	// ������ Ű - �̵�
	m_pFrameKey = L"Marion_Move";
	// ���� ���� ��Ʈ
	m_tMeleeRect = { 0,0,0,0 };

	//m_iShotPower = 3;
	// ���� ������ ������
	m_dwChargeBulletDelay = 150;
}

// ������Ʈ
int CMarion::Update()
{
	// ����
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
	// �������� Ŭ����
	else if (g_bStageClear) {
		// ȭ�� ������ �̵�
		m_tInfo.fY = m_tInfo.fY - m_fSpeed;
	}

	// ������ ó��
	if (m_bRespawn)
	{
		if (m_dwHpLockTime + m_dwHpLockDelay < GetTickCount())
		{
			m_bRespawn = false;
			m_bHpLock = false;
		}
	}

	// ���� ���¿� ���� ó���� ��
	switch (m_ePreState)
	{
		// �⺻ ���, �̵� ���´� ���� ó���� ���� ����
	case PLAYER::IDLE:
	case PLAYER::LEFT_1:
	case PLAYER::LEFT_2:
	case PLAYER::RIGHT_1:
	case PLAYER::RIGHT_2:
		break;
		// ���� ���� ����
	case PLAYER::CHARGE_START:
		// ���� ���� ������ �ִϸ��̼��� ���� ���������� �ݺ� ���·� ����
		if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		{
			// ���� �ݺ� ���·� ����
			m_eCurState = PLAYER::CHARGE_REPEAT;
			// ���� �Ϸ� ����
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SHOT);
			CSoundMgr::Get_Instance()->PlaySound(L"Marion_Charge.wav", CSoundMgr::PLAYER_SHOT);
		}
		break;
		// ���� ���� �ݺ� ���� (����Ű�� ���� ���� ����)
	case PLAYER::CHARGE_REPEAT:
		// m_bOnShot�� false�� ����Ű�� ��
		if (!m_bOnShot)
		{
			// ������ ������ ���� ���� ���� Ƚ���� �޶����ϴ�
			// m_iChargeBulletCnt�� ����Ǵ� ���� ���� Ƚ��
			int iChargeLevel = CGameUIMgr::Get_Instance()->Get_ChargeLevel(m_iPlayerNum);
			// 1���� �⺻
			if (iChargeLevel == 1) {
				m_iChargeBulletCnt = 1;
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_CHARGE);
				CSoundMgr::Get_Instance()->PlaySound(L"Marion_ChargeLV1.wav", CSoundMgr::PLAYER_CHARGE);
			}
			// 2���� 𾪱��
			else if (iChargeLevel == 2) {
				m_iChargeBulletCnt = 3;
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_CHARGE);
				CSoundMgr::Get_Instance()->PlaySound(L"Marion_ChargeLV2.wav", CSoundMgr::PLAYER_CHARGE);
			}
			// 3���� �ի�������ӫë�
			else if (iChargeLevel == 3) {
				m_iChargeBulletCnt = 8;
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_CHARGE);
				CSoundMgr::Get_Instance()->PlaySound(L"Marion_ChargeLV3.wav", CSoundMgr::PLAYER_CHARGE);
			}

			// ������ �Ҹ�
			CGameUIMgr::Get_Instance()->Add_ChargeLevel(-iChargeLevel, m_iPlayerNum);
			// ���� ���� �� ���·� ����
			m_eCurState = PLAYER::CHARGE_END;
		}
		break;
		// ���� ���� ��
	case PLAYER::CHARGE_END:
		// ���� ���� Ƚ���� �����ְ� ���� ���� �����̰� �����ٸ�
		if (0 < m_iChargeBulletCnt && m_dwChargeBulletTime + m_dwChargeBulletDelay < GetTickCount())
		{
			// ���� �ð��� ����
			m_dwChargeBulletTime = GetTickCount();
			// ���� ���� Ƚ�� ����
			--m_iChargeBulletCnt;
			// ���� ���� ����
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_CHARGE_BULLET);
			CSoundMgr::Get_Instance()->PlaySound(L"Marion_ChargeBullet.wav", CSoundMgr::PLAYER_CHARGE_BULLET);
			// ���� ����
			CObj* pObj = CAbstractFactory<CMarionChargeBullet>::Create(m_tInfo.fX, m_tInfo.fY - 30.f, 90.f);
			// �Ѿ˿��� �� �÷��̾��� �������� ����
			static_cast<CBullet*>(pObj)->Set_PlayerNum(m_iPlayerNum);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER_BULLET);
		}

		// �� ��
		if(m_iChargeBulletCnt == 0)
		{
			// ��� ���·� ����
			m_eCurState = PLAYER::IDLE;
			// �̹��� ��������Ʈ ���� ����
			m_bImmutable = false;
			// �ִϸ��̼� ������ �ʱ�ȭ
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			// �̹��� ���� �ʱ�ȭ
			m_tInfo.iCX = MARION_MOVE_WIDTH * 3;
			m_tInfo.iCY = MARION_MOVE_HEIGHT * 3;
			m_iImageWidth = MARION_MOVE_WIDTH;
			m_iImageHeight = MARION_MOVE_HEIGHT;

			// ��ġ ����
			m_tInfo.fY = m_tInfo.fY + (3.f * 3.f);
			// �浹 ��Ʈ ��ġ�� ����
			m_tHitRectPos = { 9 * 3, 8 * 3, 15 * 3, 19 * 3 };
		}

		break;
		// ��ź
	case PLAYER::BOMB:
		// ���� �Ѿ� ����
		CObjMgr::Get_Instance()->Dead_MonsterBullet();
		// ���Ϳ��� �������� ������
		CObjMgr::Get_Instance()->Demage_Monster(1.f, m_iPlayerNum);

		// ��ź ��� ����
		if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		{
			// ��� ���·� ����
			m_eCurState = PLAYER::IDLE;
			// ���� ���� ����
			m_bHpLock = false;
			// �̹��� ��������Ʈ ���� ����
			m_bImmutable = false;
			// �ִϸ��̼� ������ �ʱ�ȭ
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			// �̹��� ���� �ʱ�ȭ
			m_tInfo.iCX = MARION_MOVE_WIDTH * 3;
			m_tInfo.iCY = MARION_MOVE_HEIGHT * 3;
			m_iImageWidth = MARION_MOVE_WIDTH;
			m_iImageHeight = MARION_MOVE_HEIGHT;

			// �浹 ��Ʈ ��ġ�� ����
			m_tHitRectPos = { 9 * 3, 8 * 3, 15 * 3, 19 * 3 };

			// ����Ʈ ������ ����
			m_dwEffectCreateDelay = 100.f;
		}
			
		break;
		// ���� ����
	case PLAYER::MELEE:
		// ���� ���� ���� ������
		if (7 <= m_tFrame.iFrameCnt && m_tFrame.iFrameCnt <= 23) {
			LONG lCX = LONG(m_tInfo.iCX >> 1);
			LONG lCY = LONG(m_tInfo.iCY >> 1);
			LONG lX = (LONG)m_tInfo.fX - lCX;
			LONG lY = (LONG)m_tInfo.fY - lCY;

			// ���� ��Ʈ
			m_tMeleeRect.left = lX;
			m_tMeleeRect.top = lY;
			m_tMeleeRect.right = lX + (27 * 3);
			m_tMeleeRect.bottom = lY + (42 * 3);

			// ���Ϳ��� �������� ������
			bool bHit = CObjMgr::Get_Instance()->Demage_Monster(&m_tMeleeRect, 20.f, m_iPlayerNum);

			// ���� ���� ����Ʈ ����
			if (!m_bMeleeHit && bHit) {
				m_bMeleeHit = true;

				CObj* pObj = CAbstractFactory<CEffect_MarionMeleeHit>::Create(m_tInfo.fX - (9.f * 3.f), m_tInfo.fY - (26.f * 3.f));
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
			}
		}

		// ���� ���� ��
		if (m_tFrame.iFrameCnt == 24) {
			// ���� ���� ��Ʈ
			m_tMeleeRect = { 0,0,0,0 };
		}

		// ���� ���� ����
		if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		{
			// ��� ���·� ����
			m_eCurState = PLAYER::IDLE;
			// �̹��� ��������Ʈ ���� ����
			m_bImmutable = false;
			// �ִϸ��̼� ������ �ʱ�ȭ
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			// �̹��� ���� �ʱ�ȭ
			m_tInfo.iCX = MARION_MOVE_WIDTH * 3;
			m_tInfo.iCY = MARION_MOVE_HEIGHT * 3;
			m_iImageWidth = MARION_MOVE_WIDTH;
			m_iImageHeight = MARION_MOVE_HEIGHT;

			// ��ġ ����
			m_tInfo.fX = m_tInfo.fX - (11.f * 3.f);
			m_tInfo.fY = m_tInfo.fY + (32.f * 3.f);
			// �浹 ��Ʈ ��ġ�� ����
			m_tHitRectPos = { 9 * 3, 8 * 3, 15 * 3, 19 * 3 };
		}
		break;
		// ����
	case PLAYER::DEAD:
		// ȭ�� �ٱ����� ������
		if (m_tRect.top < WINCY)
		{
			m_tInfo.fY += 15.f;
			break;
		}

		// ���� ����� ���ٸ�
		if (CGameUIMgr::Get_Instance()->Get_Life(m_iPlayerNum) <= 0)
		{
			// ��Ƽ�� ���·� ����
			m_eCurState = PLAYER::CONTINUE;
			// ��������Ʈ ����
			m_bImmutable = true;
			// ����
			m_bDead = false;
			CGameUIMgr::Get_Instance()->On_ContinueCountdown(m_iPlayerNum);
		}
		// ���� ����� ������
		else
		{
			// ��� ����
			CGameUIMgr::Get_Instance()->Add_Life(-1, m_iPlayerNum);
			// ������ ���·� ����
			m_eCurState = PLAYER::RESPAWN;
			// ����
			m_bDead = false;
			// ������ �ð� ����
			m_dwRespawnTime = GetTickCount();
		}
		break;
		// ������
	case PLAYER::RESPAWN:
		// ������ ��� �ð����� Y�� �̵�
		if (m_dwRespawnTime + m_dwRespawnDelay > GetTickCount())
		{
			m_tInfo.fY -= m_fSpeed;
		}
		// ��� �ð����� �̵� ��
		else
		{
			// ��� ���·� ����
			m_eCurState = PLAYER::IDLE;
			// ��Ƴ� �Ŀ��� ���� �ð� ���� ������
			// m_bRespawn�� �� ���¸� �˸��� �÷���
			m_bRespawn = true;
			// �̹��� ��������Ʈ ���� ����
			m_bImmutable = false;
			// ���� �ð��� ���� ���� �ð� ����
			m_dwHpLockTime = GetTickCount();

			// �浹 ��Ʈ ��ġ�� ����
			m_tHitRectPos = { 9 * 3, 8 * 3, 15 * 3, 19 * 3 };
		}
		break;
		// ��Ƽ��
	case PLAYER::CONTINUE:
		// �������� 0 ���� ũ��
		if (CGameUIMgr::Get_Instance()->Get_Life(m_iPlayerNum) > 0) {
			m_eCurState = PLAYER::RESPAWN;		// ��Ƴ�
			m_dwRespawnTime = GetTickCount();
		}
		break;
	}

	// �Ϲ� ���� Ƚ���� ���Ұ� �Ϲ� ���� �����̰� �����ٸ�
	if (0 < m_iShotCnt && m_dwBulletTime + m_dwBulletDelay < GetTickCount())
	{
		// ���� �ð��� ����
		m_dwBulletTime = GetTickCount();
		// �Ϲ� ���� Ƚ�� ����
		--m_iShotCnt;
		// �Ϲ� ���� ó��
		Shot();
	}

	// �� ����Ʈ ����
	Create_Effect();
	// ������ �� ���� ó��
	Scene_Change();
	// �̹��� RECT ���� �� Hit RECT ���� ����
	Update_Rect();
	// �ִϸ��̼� ������ �̵�
	Frame_Move();

	return OBJ_NOEVENT;
}

// ����Ʈ ������Ʈ
void CMarion::Late_Update()
{
}

// ����
void CMarion::Render(HDC _DC)
{
	HDC hMemDC;

	// ���¿� ���� ������ �ٸ�
	switch (m_ePreState)
	{
		// ��� �� �̵�
	case PLAYER::IDLE:
	case PLAYER::LEFT_1:
	case PLAYER::LEFT_2:
	case PLAYER::RIGHT_1:
	case PLAYER::RIGHT_2:
		// ������ �� ������ ó��
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
		// ���� ����
	case PLAYER::CHARGE_START:
	case PLAYER::CHARGE_REPEAT:
	case PLAYER::CHARGE_END:
		// ������ �� ������ ó��
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
		// ��ź
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
		// ���� ����
	case PLAYER::MELEE:
		// ������ �� ������ ó��
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
		// ����
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
		// ������
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
		// ��Ƽ��
	case PLAYER::CONTINUE:
		break;
	case PLAYER::STATE_END:
		break;
	default:
		break;
	}

	// ���� �ɼǿ��� �浹 �ڽ� ���⸦ �״ٸ� (�ѹ��е� 1�� Ű)
	// �浹 �ڽ��� ���� �������
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
	Rectangle(_DC, m_tMeleeRect.left, m_tMeleeRect.top, m_tMeleeRect.right, m_tMeleeRect.bottom);
}

void CMarion::Release()
{
}

// �Ѿ� �Ŀ� ��
void CMarion::PowerUp()
{
	// �ִ� �Ŀ��� �ƴϸ�
	if (m_iShotPower < 3)
	{
		// �Ѿ� �Ŀ� ����
		++m_iShotPower;
		// �Ѿ� �Ŀ��� 1 ������ �����ϸ�
		if (m_iShotPower == 1)
		{
			// ���� ������ ���� ������
			// �⺻ 100, 400 -> 80, 300 ���� ����
			m_dwBulletDelay = 80;
			m_dwKeyDelay = 300;
		}
		// �Ŀ��� ����
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_POWER_UP);
		CSoundMgr::Get_Instance()->PlaySound(L"Marion_PowerUp.wav", CSoundMgr::PLAYER_POWER_UP);

		return;
	}

	// �ִ� �Ŀ� ����
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_POWER_UP);
	CSoundMgr::Get_Instance()->PlaySound(L"Marion_FullPower.wav", CSoundMgr::PLAYER_POWER_UP);
}

// �Ѿ� �Ŀ� �ٿ�
// ���Ϳ��� �浹�� ��� �Ŀ� �ٿ��� ��
void CMarion::PowerDown()
{
	// �Ѿ� �Ŀ��� 1 �̻��̸�
	if (0 < m_iShotPower)
	{
		// �Ŀ� �ٿ� ������
		// �����̰� ������ �����ؼ� �Ŀ� �ٿ��� �߻���
		if (m_dwPowerDownTime + m_dwPowerDownDelay > GetTickCount())
		{
			return;
		}

		// �Ѿ� �Ŀ� ����
		--m_iShotPower;
		// �Ѿ� �Ŀ��� 0�� �����ϸ�
		if (m_iShotPower == 0) {
			// ���� ���� �⺻���� ����
			m_dwBulletDelay = 100;
			m_dwKeyDelay = 400;
		}

		// �Ŀ� �ٿ� �ؽ�Ʈ ���
		CObj* pObj = CAbstractFactory<CUI_PowerUpDownText>::Create(m_tInfo.fX, m_tInfo.fY);
		pObj->Set_SceneID(1);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::UI);

		m_dwPowerDownTime = GetTickCount();
	}
}

// ĳ���� ���� �̵�
// ������ �ش� Ű �Է��� ������ ȣ���
void CMarion::Move_LL()
{
	// Ű ���� �Ұ� ���� ����
	if (!CanKeyInput())
		return;

	// X��ǥ ����
	if (m_tHitRect.left - m_fSpeed > 0)
		m_tInfo.fX -= m_fSpeed;
	// ���� ����
	if (m_fAngelY - m_fRotSpeed > -m_fMaxRotAngle)
		m_fAngelY -= m_fRotSpeed;

	// �̹��� ��������Ʈ�� ���� ���°� �ƴϸ� �̵��� ���� ���� ���� ó��
	// �̵��� ���� �÷��̾� ��������Ʈ�� �����
	if (!m_bImmutable)
		Move_Change();
}

// ĳ���� ���� �� �̵�
// ������ �ش� Ű �Է��� ������ ȣ���
void CMarion::Move_LU()
{
	// Ű ���� �Ұ� ���� ����
	if (!CanKeyInput())
		return;

	// X, Y ��ǥ ����
	if (m_tHitRect.left - m_fSpeed / sqrtf(2.f) > 0)
		m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
	if (m_tHitRect.top - m_fSpeed / sqrtf(2.f) > 100)
		m_tInfo.fY -= m_fSpeed / sqrtf(2.f);
	// ���� ����
	if (m_fAngelY - m_fRotSpeed > -m_fMaxRotAngle)
		m_fAngelY -= m_fRotSpeed;

	// �̹��� ��������Ʈ�� ���� ���°� �ƴϸ� �̵��� ���� ���� ���� ó��
	// �̵��� ���� �÷��̾� ��������Ʈ�� �����
	if (!m_bImmutable)
		Move_Change();
}

// ĳ���� ���� �Ʒ� �̵�
// ������ �ش� Ű �Է��� ������ ȣ���
void CMarion::Move_LD()
{
	// Ű ���� �Ұ� ���� ����
	if (!CanKeyInput())
		return;

	// X ����, Y ����
	if (m_tHitRect.left - m_fSpeed / sqrtf(2.f) > 0)
		m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
	if (m_tHitRect.bottom + m_fSpeed / sqrtf(2.f) < WINCY - 100)
		m_tInfo.fY += m_fSpeed / sqrtf(2.f);
	// ���� ����
	if (m_fAngelY - m_fRotSpeed > -m_fMaxRotAngle)
		m_fAngelY -= m_fRotSpeed;

	// �̹��� ��������Ʈ�� ���� ���°� �ƴϸ� �̵��� ���� ���� ���� ó��
	// �̵��� ���� �÷��̾� ��������Ʈ�� �����
	if (!m_bImmutable)
		Move_Change();
}

// ĳ���� ������ �̵�
// ������ �ش� Ű �Է��� ������ ȣ���
void CMarion::Move_RR()
{
	// Ű ���� �Ұ� ���� ����
	if (!CanKeyInput())
		return;

	// X ��ǥ ����
	if (m_tHitRect.right + m_fSpeed < WINCX)
		m_tInfo.fX += m_fSpeed;
	// ���� ����
	if (m_fAngelY + m_fRotSpeed < m_fMaxRotAngle)
		m_fAngelY += m_fRotSpeed;

	// �̹��� ��������Ʈ�� ���� ���°� �ƴϸ� �̵��� ���� ���� ���� ó��
	// �̵��� ���� �÷��̾� ��������Ʈ�� �����
	if (!m_bImmutable)
		Move_Change();
}

// ĳ���� ������ �� �̵�
// ������ �ش� Ű �Է��� ������ ȣ���
void CMarion::Move_RU()
{
	// Ű ���� �Ұ� ���� ����
	if (!CanKeyInput())
		return;

	// X ����, Y ����
	if (m_tHitRect.right + m_fSpeed / sqrtf(2.f) < WINCX)
		m_tInfo.fX += m_fSpeed / sqrtf(2.f);
	if (m_tHitRect.top - m_fSpeed / sqrtf(2.f) > 100)
		m_tInfo.fY -= m_fSpeed / sqrtf(2.f);
	// ���� ����
	if (m_fAngelY + m_fRotSpeed < m_fMaxRotAngle)
		m_fAngelY += m_fRotSpeed;

	// �̹��� ��������Ʈ�� ���� ���°� �ƴϸ� �̵��� ���� ���� ���� ó��
	// �̵��� ���� �÷��̾� ��������Ʈ�� �����
	if (!m_bImmutable)
		Move_Change();
}

// ĳ���� ������ �Ʒ� �̵�
// ������ �ش� Ű �Է��� ������ ȣ���
void CMarion::Move_RD()
{
	// Ű ���� �Ұ� ���� ����
	if (!CanKeyInput())
		return;

	// X, Y ��ǥ ����
	if (m_tHitRect.right + m_fSpeed / sqrtf(2.f) < WINCX)
		m_tInfo.fX += m_fSpeed / sqrtf(2.f);
	if (m_tHitRect.bottom + m_fSpeed / sqrtf(2.f) < WINCY - 100)
		m_tInfo.fY += m_fSpeed / sqrtf(2.f);
	// ���� ����
	if (m_fAngelY + m_fRotSpeed < m_fMaxRotAngle)
		m_fAngelY += m_fRotSpeed;

	// �̹��� ��������Ʈ�� ���� ���°� �ƴϸ� �̵��� ���� ���� ���� ó��
	// �̵��� ���� �÷��̾� ��������Ʈ�� �����
	if (!m_bImmutable)
		Move_Change();
}

// ĳ���� ���� �̵�
// ������ �ش� Ű �Է��� ������ ȣ���
void CMarion::Move_UU()
{
	// Ű ���� �Ұ� ���� ����
	if (!CanKeyInput())
		return;

	// Y ��ǥ ����
	if (m_tHitRect.top - m_fSpeed > 100)
		m_tInfo.fY -= m_fSpeed;
	// ���Ⱑ 0���� ũ��
	if (m_fAngelY > 0)
	{
		// ���� ���� Ȥ�� 0���� ����
		if (m_fAngelY - m_fRotSpeed > 0)
			m_fAngelY -= m_fRotSpeed;
		else
			m_fAngelY = 0.f;
	}
	// 0���� �۴ٸ�
	else
	{
		// ���� ���� Ȥ�� 0���� ����
		if (m_fAngelY + m_fRotSpeed < 0)
			m_fAngelY += m_fRotSpeed;
		else
			m_fAngelY = 0.f;
	}

	// �̹��� ��������Ʈ�� ���� ���°� �ƴϸ� �̵��� ���� ���� ���� ó��
	// �̵��� ���� �÷��̾� ��������Ʈ�� �����
	if (!m_bImmutable)
		Move_Change();
}

// ĳ�� �Ʒ��� �̵�
// ������ �ش� Ű �Է��� ������ ȣ���
void CMarion::Move_DD()
{
	// Ű ���� �Ұ� ���� ����
	if (!CanKeyInput())
		return;

	// Y ��ǥ ����
	if (m_tHitRect.bottom + m_fSpeed < WINCY - 100)
		m_tInfo.fY += m_fSpeed;
	// ���Ⱑ 0���� ũ��
	if (m_fAngelY > 0)
	{
		// ���� ���� Ȥ�� 0���� ����
		if (m_fAngelY - m_fRotSpeed > 0)
			m_fAngelY -= m_fRotSpeed;
		else
			m_fAngelY = 0.f;
	}
	// 0���� �۴ٸ�
	else
	{
		// ���� ���� Ȥ�� 0���� ����
		if (m_fAngelY + m_fRotSpeed < 0)
			m_fAngelY += m_fRotSpeed;
		else
			m_fAngelY = 0.f;
	}

	// �̹��� ��������Ʈ�� ���� ���°� �ƴϸ� �̵��� ���� ���� ���� ó��
	// �̵��� ���� �÷��̾� ��������Ʈ�� �����
	if (!m_bImmutable)
		Move_Change();
}

// �̵� Ű�� ������ ������ ĳ���� ���⸦ 0���� ���� ������
// ������ �ƹ� �̵� Ű�� ������ ������ ȣ���
void CMarion::Move_End()
{
	// Ű ���� �Ұ� ���� ����
	if (!CanKeyInput())
		return;

	// ĳ���� ���Ⱑ 0���� ũ��
	if (m_fAngelY > 0)
	{
		// ���� ���� Ȥ�� 0���� ����
		if (m_fAngelY - m_fRotSpeed > 0)
			m_fAngelY -= m_fRotSpeed;
		else
			m_fAngelY = 0.f;
	}
	// ĳ���� Ű��Ⱑ 0���� ������
	else
	{
		// ���� ���� Ȥ�� 0���� ����
		if (m_fAngelY + m_fRotSpeed < 0)
			m_fAngelY += m_fRotSpeed;
		else
			m_fAngelY = 0.f;
	}

	// �̹��� ��������Ʈ�� ���� ���°� �ƴϸ� �̵��� ���� ���� ���� ó��
	// �̵��� ���� �÷��̾� ��������Ʈ�� �����
	if (!m_bImmutable)
		Move_Change();
}

// �Ϲ� ����
// �ѹ��� �������� 3�� �����ϴ� ���� ���� ���
// ���� �ð� �̻� ������ ���� �������� �ٲ�
// ������ �ش� Ű �Է��� ������ ȣ���
void CMarion::Attack()
{
	// Ű ���� �Ұ� ���� ����
	if (!CanKeyInput())
		return;

	// ���� ���� �Ұ� ���� ����
	if (!CanAttack())
		return;

	// ���� �÷��װ� false��
	if (!m_bOnShot)
	{
		// ���� �÷��� true��
		m_bOnShot = true;
		// ���� ���� �ð� ����
		m_dwChargeTime = GetTickCount();
	}

	// ���� Ƚ���� 0���� �۰� (���̿��� ����̶� �ѹ��� �������� 3���� ������ ����)
	// Ű �Է� ������ �ð��� �������� �Ϲ� ���� Ƚ�� ī��Ʈ �ʱ�ȭ
	if (m_iShotCnt <= 0 && m_dwKeyTime + m_dwKeyDelay < GetTickCount())
	{
		// 3���� �ʱ�ȭ �� ���� �ð� ���� �ʱ�ȭ
		m_iShotCnt = 3;
		m_dwBulletTime = GetTickCount();
		m_dwKeyTime = GetTickCount();
	}

	// ���ݹ�ư�� �����ð� ��� ������ �ְ� ���� ���°� �ƴϸ� ���� �������� �ִٸ� ���� ���� �غ�
	if (m_dwChargeTime + m_dwChargeDelay < GetTickCount() &&
		m_ePreState != PLAYER::CHARGE_START &&
		1 <= CGameUIMgr::Get_Instance()->Get_ChargeLevel(m_iPlayerNum))
	{
		// ���� ���� ���·�
		m_eCurState = PLAYER::CHARGE_START;
		// �̹��� ��������Ʈ ����
		m_bImmutable = true;
		// ���� �Ϲ� ���� Ƚ�� 0���� �ʱ�ȭ
		m_iShotCnt = 0;
	}
}

// �Ϲ� ���� Ű�� ������ ������ ���� �÷��׸� false�� �ʱ�ȭ
void CMarion::Attack_End()
{
	// Ű ���� �Ұ� ���� ����
	if (!CanKeyInput())
		return;

	// ���� �÷��� false��
	m_bOnShot = false;
}

// ��ź ����
// ���� ��ź�� ������ ��� �Ұ� ��ź 1���� �Ҹ�
// ��ź�� ���� ���� ȭ�鿡 �ִ� �� �Ѿ��� ���� ĵ��� ��ü��
// �� �Ŀ� ��ź�� ���ӵǴ� ���� ���� �����ؼ� �����Ǵ� �Ѿ��� ���ŵ�
// ������ �ش� Ű �Է��� ������ ȣ���
void CMarion::Bomb()
{
	// Ű ���� �Ұ� ���� ����
	if (!CanKeyInput())
		return;

	// ���� ��ź�� ������ ��ź ���� �Ұ�
	if (CGameUIMgr::Get_Instance()->Get_Bomb(m_iPlayerNum) == 0)
		return;

	// ��ź ��� ���� ����
	CGameUIMgr::Get_Instance()->Add_Bomb(-1, m_iPlayerNum);

	// ��ź�� ����� ó������
	// ȭ�鿡 ���̴� ���� �Ѿ� �������� ��ü
	CObjMgr::Get_Instance()->Change_Candy();

	// ��ź ���� ����
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_BOMB);
	CSoundMgr::Get_Instance()->PlaySound(L"Marion_Bomb.wav", CSoundMgr::PLAYER_BOMB);
	// ����Ʈ ����
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_BOMB_EFFECT);
	CSoundMgr::Get_Instance()->PlaySound(L"Marion_BombEffect.wav", CSoundMgr::PLAYER_BOMB_EFFECT);
	// ��ź ���� ���·� ����
	m_eCurState = PLAYER::BOMB;
	// ���� ����
	m_bHpLock = true;
	// �̹��� ��������Ʈ ����
	m_bImmutable = true;
}

// ���� ����
// ���� �������� ����Ͽ� ����
// ����ϴ� ���� �̹��� ��������Ʈ ����
// ������ �ش� Ű �Է��� ������ ȣ���
void CMarion::Melee()
{
	// Ű ���� �Ұ� ���� ����
	if (!CanKeyInput())
		return;

	// �̹� ���� ���� ���̸� �Ұ�
	if (m_ePreState == PLAYER::MELEE)
		return;

	// �������� ������ ��� �Ұ�
	if (CGameUIMgr::Get_Instance()->Get_ChargeLevel(m_iPlayerNum) < 1)
		return;

	// ������ �Ҹ�
	CGameUIMgr::Get_Instance()->Add_ChargeLevel(-1, m_iPlayerNum);
	// ���� ���� ���·� ����
	m_eCurState = PLAYER::MELEE;
	// �̹��� ��������Ʈ ����
	m_bImmutable = true;
	// ���� ���� ���� �÷��� �ʱ�ȭ
	m_bMeleeHit = false;
	// ���� ���� ����
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_MELEE);
	CSoundMgr::Get_Instance()->PlaySound(L"Marion_Melee.wav", CSoundMgr::PLAYER_MELEE);
}

// �ٽ� �ϱ�
// ��Ƽ�� ������ �� �̾��ϱ� ���
// ������ �ش� Ű �Է��� ������ ȣ���
void CMarion::Continue()
{
	// ��Ƽ�� ���°� �ƴϸ� ����
	if (m_ePreState != PLAYER::CONTINUE)
		return;

	// ũ������ ������ �ٽ��ϱ� ����
	if (0 == CGameUIMgr::Get_Instance()->Get_Credit())
		return;

	// ũ���� ����
	CGameUIMgr::Get_Instance()->Add_Credit(-1);
	// ũ���� ����
	g_iCredit = CGameUIMgr::Get_Instance()->Get_Credit();
	// ��� ����
	CGameUIMgr::Get_Instance()->Add_Life(2, m_iPlayerNum);
	// ��Ƽ�� ����
	CGameUIMgr::Get_Instance()->Off_ContinueCountdown(m_iPlayerNum);
	// ���� ����
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_DEAD);
	CSoundMgr::Get_Instance()->PlaySound(L"Marion_Start.wav", CSoundMgr::PLAYER_DEAD);
}

// ������ �� ���� ó��
// ��� �ִϸ��̼� �������� �̹��� ���̰� �޶�
// ���°� ����� �� ��ġ ��ǥ�� �ʱ�ȭ�� �� �����ؾ� ��
void CMarion::Scene_Change()
{
	// ���� ���¿� �ٸ���
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
			// ���
		case PLAYER::IDLE:
			m_tFrame.iFrameScene = 0;
			break;
			// ���� �̵� 1
		case PLAYER::LEFT_1:
			m_tFrame.iFrameScene = 1;
			break;
			// ���� �̵� 2
		case PLAYER::LEFT_2:
			m_tFrame.iFrameScene = 2;
			break;
			// ������ �̵� 1
		case PLAYER::RIGHT_1:
			m_tFrame.iFrameScene = 3;
			break;
			// ������ �̵� 2
		case PLAYER::RIGHT_2:
			m_tFrame.iFrameScene = 4;
			break;
			// ���� ���� ����
			// �⺻ ���¶� ĳ���� ��ġ�� �޶� ��ġ ���� �������
			// HitRect ��ġ�� ��������� ��
		case PLAYER::CHARGE_START:
			// �ִϸ��̼� ������ �ʱ�ȭ
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 40;
			// �̹��� ���� �ʱ�ȭ
			m_tInfo.iCX = MARION_CHARGE_WIDTH * 3;
			m_tInfo.iCY = MARION_CHARGE_HEIGHT * 3;
			m_iImageWidth = MARION_CHARGE_WIDTH;
			m_iImageHeight = MARION_CHARGE_HEIGHT;

			// ��ǥ ����
			m_tInfo.fY = m_tInfo.fY - (3.f * 3.f);
			// �浹 ��Ʈ ��ġ�� ����
			m_tHitRectPos = { 17 * 3, 23 * 3, 23 * 3, 34 * 3 };
			break;
			// ���� ���� �ݺ�
		case PLAYER::CHARGE_REPEAT:
			// �ִϸ��̼� ������ �ʱ�ȭ
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 40;
			break;
			// ���� ���� ��
		case PLAYER::CHARGE_END:
			// �ִϸ��̼� ������ �ʱ�ȭ
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 13;
			m_tFrame.iFrameScene = 2;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 40;
			break;
			// ��ź ����
		case PLAYER::BOMB:
			// �ִϸ��̼� ������ �ʱ�ȭ
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 31;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 70;
			// �̹��� ���� �ʱ�ȭ
			m_tInfo.iCX = MARION_BOMB_WIDTH * 3;
			m_tInfo.iCY = MARION_BOMB_HEIGHT * 3;
			m_iImageWidth = MARION_BOMB_WIDTH;
			m_iImageHeight = MARION_BOMB_HEIGHT;

			// ����Ʈ ������ ����
			m_dwEffectCreateDelay = 300.f;

			// �浹 ��Ʈ ��ġ�� ����
			m_tHitRectPos = { 0, 0, 0, 0 };

			// ���� ���� ���̾����� �����·� ����
			if (m_ePreState == PLAYER::MELEE) {
				// ��ġ ����
				m_tInfo.fX = m_tInfo.fX - (11.f * 3.f);
				m_tInfo.fY = m_tInfo.fY + (32.f * 3.f);
				// ���� ���� ��Ʈ
				m_tMeleeRect = { 0,0,0,0 };
			}
			// ���� ���� ���̾����� �����·� ����
			else if (m_ePreState == PLAYER::CHARGE_START ||
					m_ePreState == PLAYER::CHARGE_REPEAT ||
					m_ePreState == PLAYER::CHARGE_END) {
				// ��ġ ����
				m_tInfo.fY = m_tInfo.fY + (3.f * 3.f);
			}
			break;
			// ���� ����
			// �⺻ ���¶� ĳ���� ��ġ�� �޶� ��ġ ���� �������
			// HitRect ��ġ�� ��������� ��
		case PLAYER::MELEE:
			// �ִϸ��̼� ������ �ʱ�ȭ
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 26;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 30;
			// �̹��� ���� �ʱ�ȭ
			m_tInfo.iCX = MARION_MELEE_WIDTH * 3;
			m_tInfo.iCY = MARION_MELEE_HEIGHT * 3;
			m_iImageWidth = MARION_MELEE_WIDTH;
			m_iImageHeight = MARION_MELEE_HEIGHT;

			// ��ǥ ����
			m_tInfo.fX = m_tInfo.fX + (11.f * 3.f);
			m_tInfo.fY = m_tInfo.fY - (32.f * 3.f);
			// �浹 ��Ʈ ��ġ�� ����
			m_tHitRectPos = { 8 * 3, 73 * 3, 14 * 3, 84 * 3 };
			break;
			// ����
		case PLAYER::DEAD:
			// �ִϸ��̼� ������ �ʱ�ȭ
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 100;
			// �̹��� ���� �ʱ�ȭ
			m_tInfo.iCX = MARION_MOVE_WIDTH * 3;
			m_tInfo.iCY = MARION_MOVE_HEIGHT * 3;
			m_iImageWidth = MARION_MOVE_WIDTH;
			m_iImageHeight = MARION_MOVE_HEIGHT;

			// ���� ���� ���̾����� �����·� ����
			if (m_ePreState == PLAYER::MELEE) {
				// ��ġ ����
				m_tInfo.fX = m_tInfo.fX - (11.f * 3.f);
				m_tInfo.fY = m_tInfo.fY + (32.f * 3.f);
				// ���� ���� ��Ʈ
				m_tMeleeRect = { 0,0,0,0 };
			}
			// ���� ���� ���̾����� �����·� ����
			else if (m_ePreState == PLAYER::CHARGE_START ||
				m_ePreState == PLAYER::CHARGE_REPEAT ||
				m_ePreState == PLAYER::CHARGE_END) {
				// ��ġ ����
				m_tInfo.fY = m_tInfo.fY + (3.f * 3.f);
			}

			// ����� ����
			// �Ŀ��� 2, ��ź 1�� ���
			if (CGameUIMgr::Get_Instance()->Get_Life(m_iPlayerNum) == 0) {
				// �Ŀ��� ������ ���
				CObj* pObj = CAbstractFactory<CPowerUp>::Create(m_tInfo.fX, m_tInfo.fY);
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::ITEM);
				pObj = CAbstractFactory<CPowerUp>::Create(m_tInfo.fX, m_tInfo.fY);
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::ITEM);
				// ��ź ������ ���
				pObj = CAbstractFactory<CBomb>::Create(m_tInfo.fX, m_tInfo.fY);
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::ITEM);
			}
			// �Ŀ� 3����
			// �Ŀ��� 2�� ���
			else if (m_iShotPower == 3) {
				// �Ŀ��� ������ ���
				CObj* pObj = CAbstractFactory<CPowerUp>::Create(m_tInfo.fX, m_tInfo.fY);
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::ITEM);
				pObj = CAbstractFactory<CPowerUp>::Create(m_tInfo.fX, m_tInfo.fY);
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::ITEM);
			}
			else {
				// �Ŀ��� ������ ���
				CObj* pObj = CAbstractFactory<CPowerUp>::Create(m_tInfo.fX, m_tInfo.fY);
				CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::ITEM);
			}
			// �浹 ��Ʈ ��ġ�� ����
			m_tHitRectPos = { 0, 0, 0, 0 };
			// ���� ������ �ʱ�ȭ
			CGameUIMgr::Get_Instance()->Set_ChargeLevel(1, m_iPlayerNum);
			// ��ź ���� �ʱ�ȭ
			CGameUIMgr::Get_Instance()->Set_Bomb(2, m_iPlayerNum);
			// �Ѿ� �Ŀ� 0
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

// �̵� ���� ���� ó��
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

// �Ϲ� ���� ó��
void CMarion::Shot()
{
	CObj* pObj;
	// �Ϲ� ���� ����
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SHOT);
	CSoundMgr::Get_Instance()->PlaySound(L"Marion_Shot.wav", CSoundMgr::PLAYER_SHOT);
	// �Ŀ� ������ ���� �з�
	switch (m_iShotPower)
	{
	case 0:
		// �⺻ ����
		pObj = Create_Bullet<CMarionBullet_1>(m_tInfo.fX, m_tInfo.fY - 30.f, 90.f);
		pObj->Set_DrawID(2 - m_iShotCnt);
		static_cast<CBullet*>(pObj)->Set_PlayerNum(m_iPlayerNum);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER_BULLET);
		break;
	case 1:
		// 1��
		pObj = Create_Bullet<CMarionBullet_2>(m_tInfo.fX, m_tInfo.fY - 30.f, 90.f);
		pObj->Set_DrawID(2 - m_iShotCnt);
		static_cast<CBullet*>(pObj)->Set_PlayerNum(m_iPlayerNum);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER_BULLET);
		break;
	case 2:
		// 2��
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

		// ���� �̻����� ������� �ƴϸ� (���� �̻����� �� ȭ�鿡�� ���ѵ� ���ڷθ� ������)
		if (m_tSubBulletCnt.flag == false)
		{
			// ���� �̻��� ���
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
		// 3��
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

		// ���� �̻����� ������� �ƴϸ� (���� �̻����� �� ȭ�鿡�� ���ѵ� ���ڷθ� ������)
		if (m_tSubBulletCnt.flag == false)
		{
			// ���� �̻��� ���
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

// �� ����Ʈ ����
void CMarion::Create_Effect()
{
	CObj* pObj = nullptr;

	// ����Ʈ ���� �����̰� ��������
	if (m_dwEffectCreateTime + m_dwEffectCreateDelay < GetTickCount())
	{
		// ���� �ð� ����
		m_dwEffectCreateTime = GetTickCount();

		// ���¿� ���� �����Ǵ� ����Ʈ ��ǥ�� ���� ������ �ٸ�
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
			// �����ư��鼭 ����
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

// Ű ó���� �������� �˻�
// Ű ó�� ���� ��� �Լ����� ȣ����
// ��ȯ �� : Ű ������ �Ұ����� ���¸� false, �����ϸ� true
bool CMarion::CanKeyInput()
{
	// Ű ���� �Ұ� ���� ����
	if (m_ePreState == PLAYER::RESPAWN ||
		m_ePreState == PLAYER::DEAD ||
		m_ePreState == PLAYER::BOMB ||
		g_bStageClear)
		return false;

	return true;
}

// ������ �� �ִ��� �˻�
// ����Ű ó�� �Լ����� ȣ����
// ��ȯ �� : ������ �Ұ����� ���¸� false, �����ϸ� true
bool CMarion::CanAttack()
{
	// ���� ���� �Ұ� ���� ����
	if (m_ePreState == PLAYER::CHARGE_START ||
		m_ePreState == PLAYER::CHARGE_REPEAT ||
		m_ePreState == PLAYER::DEAD ||
		m_ePreState == PLAYER::RESPAWN)
		return false;

	return true;
}

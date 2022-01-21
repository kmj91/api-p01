// �����
// UI �Ŵ���

#include "stdafx.h"
#include "GameUIMgr.h"

#include "ObjMgr.h"
#include "SoundMgr.h"

#include "UI_PlayerLife.h"
#include "UI_MainContinue.h"
#include "UI_Static.h"
#include "UI_Score.h"
#include "UI_ChargeBar.h"
#include "UI_Bomb.h"
#include "UI_Credit.h"
#include "UI_TitleLogo.h"


CGameUIMgr* CGameUIMgr::m_pInstance = nullptr;

CGameUIMgr::CGameUIMgr()
	: m_pPlayerLife_1(nullptr), m_pPlayerLife_2(nullptr)
	, m_pMainContinue(nullptr)
	, m_pPlayerNum_1(nullptr), m_pPlayerNum_2(nullptr), m_pScore_1(nullptr), m_pScore_2(nullptr)
	, m_pStageNum(nullptr)
	, m_pChargeBar_1(nullptr), m_pChargeBar_2(nullptr), m_pBomb_1(nullptr), m_pBomb_2(nullptr)
	, m_pTitleLogo(nullptr), m_pCredit(nullptr)
	, m_bPlayer_1(false), m_bPlayer_2(false)
{
}


CGameUIMgr::~CGameUIMgr()
{
	Release();
}

// Ÿ��Ʋ �ΰ� �ʱ�ȭ
void CGameUIMgr::Init_Logo()
{
	// Ÿ��Ʋ �ΰ�
	m_pTitleLogo = CObjFactory<CUI_TitleLogo>::Create();
	CObjMgr::Get_Instance()->Add_Object(m_pTitleLogo, OBJID::UI);

	// ũ����
	m_pCredit = CObjFactory<CUI_Credit>::Create(float(WINCX >> 1), WINCY - (19.f * 3.f));
	CObjMgr::Get_Instance()->Add_Object(m_pCredit, OBJID::UI);
}

// ĳ���� ���� �޴� �ʱ�ȭ
void CGameUIMgr::Init_SeleteMenu()
{
	// ũ����
	m_pCredit = CObjFactory<CUI_Credit>::Create((WINCX >> 1) + (70.f * 3.f), WINCY - (19.f * 3.f));
	CObjMgr::Get_Instance()->Add_Object(m_pCredit, OBJID::UI);
}

// ���� �������� �ʱ�ȭ
void CGameUIMgr::Init_Stage()
{
	if (m_bPlayer_1) {
		// �÷��̾� ������
		m_pPlayerLife_1 = CObjFactory<CUI_PlayerLife>::Create(3.f * 3.f, 19.f * 3.f);
		m_pPlayerLife_1->Set_DrawID(g_ePlayer_1_Name);
		m_pPlayerLife_1->Add_Life(2);
		CObjMgr::Get_Instance()->Add_Object(m_pPlayerLife_1, OBJID::UI);

		// �÷��̾� ��ȣ
		m_pPlayerNum_1 = CObjFactory<CUI_Static>::Create(10.f * 3.f, 7.f * 3.f);
		m_pPlayerNum_1->Set_FrameKey(L"UI_PlayerNum");
		m_pPlayerNum_1->Set_ImageSize(UI_PLAYER_NUM_WIDTH, UI_PLAYER_NUM_HEIGHT);
		CObjMgr::Get_Instance()->Add_Object(m_pPlayerNum_1, OBJID::UI);

		// ���ھ�
		m_pScore_1 = CObjFactory<CUI_Score>::Create(85.f * 3.f, 6.f * 3.f);
		CObjMgr::Get_Instance()->Add_Object(m_pScore_1, OBJID::UI);

		// ���� ��
		m_pChargeBar_1 = CObjFactory<CUI_ChargeBar>::Create(13.f * 3.f, WINCY - (22.f * 3.f));
		m_pChargeBar_1->Set_Charge(40);
		m_pChargeBar_1->Set_ChargeLevel(1);
		CObjMgr::Get_Instance()->Add_Object(m_pChargeBar_1, OBJID::UI);

		// ��ź ����
		m_pBomb_1 = CObjFactory<CUI_Bomb>::Create(87.f * 3.f, WINCY - (19.f * 3.f));
		m_pBomb_1->Add_Bomb(2);
		CObjMgr::Get_Instance()->Add_Object(m_pBomb_1, OBJID::UI);
	}

	if (m_bPlayer_2) {
		// �÷��̾� ������
		m_pPlayerLife_2 = CObjFactory<CUI_PlayerLife>::Create(115.f * 3.f, 19.f * 3.f);
		m_pPlayerLife_2->Set_DrawID(g_ePlayer_2_Name);
		m_pPlayerLife_2->Add_Life(2);
		CObjMgr::Get_Instance()->Add_Object(m_pPlayerLife_2, OBJID::UI);
	}

	// ��Ƽ��
	m_pMainContinue = CObjFactory<CUI_MainContinue>::Create(441.f, 414.f);
	CObjMgr::Get_Instance()->Add_Object(m_pMainContinue, OBJID::UI);

	// �������� ��ȣ
	m_pStageNum = CObjFactory<CUI_Static>::Create();
	m_pStageNum->Set_Pos((WINCX >> 1) - 6.f * 3.f, 6.f * 3.f);
	m_pStageNum->Set_FrameKey(L"UI_StageNum");
	m_pStageNum->Set_ImageSize(UI_STAGE_NUM_WIDTH, UI_STAGE_NUM_HEIGHT);
	CObjMgr::Get_Instance()->Add_Object(m_pStageNum, OBJID::UI);

	// ũ����
	m_pCredit = CObjFactory<CUI_Credit>::Create((WINCX >> 1) + (70.f * 3.f), WINCY - (19.f * 3.f));
	CObjMgr::Get_Instance()->Add_Object(m_pCredit, OBJID::UI);
}

void CGameUIMgr::Release()
{
}

// �÷��̾� �������� ��� �����ؼ� �÷��̾�κ��� �̾��ϱ⸦ ��ٸ��ϴ�
// �ٸ� �÷��̾ ���� ���̶�� ���� UI��
// ȥ�� �÷��� ���̶�� ū UI�� ȣ���մϴ�
void CGameUIMgr::On_ContinueCountdown(int _iPlayerNum)
{
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		// 1 �÷��̾� ��Ƽ�� ON
		g_bGameContinuePlayer_1 = true;
		// �ٸ� �÷��̾� ������
		if (m_bPlayer_2 && !g_bGameContinuePlayer_2) {
			// ���� UI
			return;
		}
		// ū UI
		// ���� ����
		g_bCountdownStop = true;
		m_pMainContinue->Start_Countdown();
	}
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		// 2 �÷��̾� ��Ƽ�� ON
		g_bGameContinuePlayer_2 = true;
		// �ٸ� �÷��̾� ������
		if (m_bPlayer_1 && !g_bGameContinuePlayer_1) {
			// ���� UI
			return;
		}
		// ū UI
		// ���� ����
		g_bCountdownStop = true;
	}
	else {
		throw;
	}
}

// ��Ƽ�� ī��Ʈ ����
void CGameUIMgr::Off_ContinueCountdown(int _iPlayerNum)
{
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		g_bGameContinuePlayer_1 = false;
		// �ٸ� �÷��̾� ������
		if (m_bPlayer_2 && !g_bGameContinuePlayer_2) {
			// ���� UI
			return;
		}
		// ū UI
		// ���� ����
		g_bCountdownStop = false;
		m_pMainContinue->Stop_Countdown();
	}
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		g_bGameContinuePlayer_2 = false;
		// �ٸ� �÷��̾� ������
		if (m_bPlayer_1 && !g_bGameContinuePlayer_1) {
			// ���� UI
			return;
		}
		// ū UI
		// ���� ����
		g_bCountdownStop = false;
	}
	else {
		throw;
	}
}

// ī��Ʈ ����
void CGameUIMgr::NextMainCount()
{
	// ī��Ʈ ����
	m_pMainContinue->NextCount();
}

// ������ ���
int CGameUIMgr::Get_Life(int _iPlayerNum)
{
	int iLife = 0;

	// 1 �÷��̾�
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		iLife = m_pPlayerLife_1->Get_Life();
	}
	// 2 �÷��̾�
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		iLife = m_pPlayerLife_2->Get_Life();
	}
	else {
		throw;
	}

	return iLife;
}

// ������ �߰�
void CGameUIMgr::Add_Life(int _iLife, int _iPlayerNum)
{
	// 1 �÷��̾�
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		m_pPlayerLife_1->Add_Life(_iLife);
	}
	// 2 �÷��̾�
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		m_pPlayerLife_2->Add_Life(_iLife);
	}
	else {
		throw;
	}
}

// ���ھ� ���
int CGameUIMgr::Get_Score(int _iPlayerNum)
{
	int iScore = 0;

	// 1 �÷��̾�
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		iScore = m_pScore_1->Get_Score();
	}
	// 2 �÷��̾�
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		iScore = m_pScore_2->Get_Score();
	}
	else {
		throw;
	}

	return iScore;
}

// ���ھ� �߰�
void CGameUIMgr::Add_Score(int _iScore, int _iPlayerNum)
{
	// 1 �÷��̾�
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		m_pScore_1->Add_Score(_iScore);
	}
	// 2 �÷��̾�
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		m_pScore_2->Add_Score(_iScore);
	}
	else {
		throw;
	}
}

// ������ ���
int CGameUIMgr::Get_Charge(int _iPlayerNum)
{
	int iCharge = 0;

	// 1 �÷��̾�
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		iCharge = m_pChargeBar_1->Get_Charge();
	}
	// 2 �÷��̾�
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		iCharge = m_pChargeBar_2->Get_Charge();
	}
	else {
		throw;
	}

	return iCharge;
}

// ������ �߰�
void CGameUIMgr::Add_Charge(int _iCharge, int _iPlayerNum)
{
	int iCharge;		// ������
	int iChargeLevel;	// ���� ����

	// 1 �÷��̾�
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		// ���ϱ��� ���� ���� ����
		iChargeLevel = m_pChargeBar_1->Get_ChargeLevel();
		// 240�� �Ѱų� ������ �Ǹ� �ȵ�
		iCharge = m_pChargeBar_1->Get_Charge();
		iCharge = iCharge + _iCharge;
		if (iCharge < 0) {
			return;
		}
		else if (240 < iCharge) {
			return;
		}

		// ���� ���ϰ�
		m_pChargeBar_1->Add_Charge(_iCharge);
		// 39���ϸ� 0����
		if (iCharge <= 39) {
			m_pChargeBar_1->Set_ChargeLevel(0);
		}
		// 40�̻� 119���ϸ� 1����
		else if (40 <= iCharge && iCharge <= 119) {
			m_pChargeBar_1->Set_ChargeLevel(1);
		}
		// 120�̻� 239���ϸ� 2����
		else if (120 <= iCharge && iCharge <= 239) {
			m_pChargeBar_1->Set_ChargeLevel(2);
		}
		// 240 MAX 3����
		else if(240 == iCharge)
		{
			m_pChargeBar_1->Set_ChargeLevel(3);
		}

		// ���� ���� ��
		if (iChargeLevel < m_pChargeBar_1->Get_ChargeLevel()) {
			// ���� �� Ŀ������
			// ���� ���� ���
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ITEM);
			CSoundMgr::Get_Instance()->PlaySound(L"UI_ChargeReadyOK.wav", CSoundMgr::ITEM);
		}
	}
	// 2 �÷��̾�
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		// ���ϱ��� ���� ���� ����
		iChargeLevel = m_pChargeBar_2->Get_ChargeLevel();
		// 240�� �Ѱų� ������ �Ǹ� �ȵ�
		iCharge = m_pChargeBar_2->Get_Charge();
		iCharge = iCharge + _iCharge;
		if (iCharge < 0) {
			return;
		}
		else if (240 < iCharge) {
			return;
		}

		// ���� ���ϰ�
		m_pChargeBar_2->Add_Charge(_iCharge);
		// 39���ϸ� 0����
		if (iCharge <= 39) {
			m_pChargeBar_2->Set_ChargeLevel(0);
		}
		// 40�̻� 119���ϸ� 1����
		else if (40 <= iCharge && iCharge <= 119) {
			m_pChargeBar_2->Set_ChargeLevel(1);
		}
		// 120�̻� 239���ϸ� 2����
		else if (120 <= iCharge && iCharge <= 239) {
			m_pChargeBar_2->Set_ChargeLevel(2);
		}
		// 240 MAX 3����
		else if (240 == iCharge)
		{
			m_pChargeBar_2->Set_ChargeLevel(3);
		}

		// ���� ���� ��
		if (iChargeLevel < m_pChargeBar_2->Get_ChargeLevel()) {
			// ���� �� Ŀ������
			// ���� ���� ���
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ITEM);
			CSoundMgr::Get_Instance()->PlaySound(L"UI_ChargeReadyOK.wav", CSoundMgr::ITEM);
		}
	}
}

// ���� ���� ���
int CGameUIMgr::Get_ChargeLevel(int _iPlayerNum)
{
	int iChargeLevel = 0;

	// 1 �÷��̾�
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		iChargeLevel = m_pChargeBar_1->Get_ChargeLevel();
	}
	// 2 �÷��̾�
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		iChargeLevel = m_pChargeBar_2->Get_ChargeLevel();
	}
	else {
		throw;
	}

	return iChargeLevel;
}

// ���� ���� ����
void CGameUIMgr::Set_ChargeLevel(int _iChargeLevel, int _iPlayerNum)
{
	// 1�÷��̾�
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		if (_iChargeLevel == 1) {
			m_pChargeBar_1->Set_Charge(40);
		}
		else if (_iChargeLevel == 2) {
			m_pChargeBar_1->Set_Charge(120);
		}
		else if (_iChargeLevel == 3) {
			m_pChargeBar_1->Set_Charge(240);
		}
		else {
			return;
		}
		// ���� ����
		m_pChargeBar_1->Set_ChargeLevel(_iChargeLevel);
	}
	// 2�÷��̾�
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		if (_iChargeLevel == 1) {
			m_pChargeBar_2->Set_Charge(40);
		}
		else if (_iChargeLevel == 2) {
			m_pChargeBar_2->Set_Charge(120);
		}
		else if (_iChargeLevel == 3) {
			m_pChargeBar_2->Set_Charge(240);
		}
		else {
			return;
		}
		// ���� ����
		m_pChargeBar_2->Set_ChargeLevel(_iChargeLevel);
	}
	else {
		throw;
	}
}

// ���� ���� �߰�
void CGameUIMgr::Add_ChargeLevel(int _iChargeLevel, int _iPlayerNum)
{
	int iCharge;		// ������
	int iChargeLevel;	// ���� ����

	// 1�÷��̾�
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		// 3�� �Ѱų� ������ �Ǹ� �ȵ�
		iChargeLevel = m_pChargeBar_1->Get_ChargeLevel();
		iChargeLevel = iChargeLevel + _iChargeLevel;
		if (iChargeLevel < 0) {
			return;
		}
		else if (3 < iChargeLevel) {
			return;
		}
		
		// ����
		if (_iChargeLevel < 0) {
			if (_iChargeLevel == -1) {
				m_pChargeBar_1->Add_Charge(-40);
			}
			else if (_iChargeLevel == -2) {
				m_pChargeBar_1->Add_Charge(-120);
			}
			else if (_iChargeLevel == -3) {
				m_pChargeBar_1->Add_Charge(-240);
			}
		}
		// ���
		else {
			if (_iChargeLevel == 1) {
				m_pChargeBar_1->Add_Charge(40);
			}
			else if (_iChargeLevel == 2) {
				m_pChargeBar_1->Add_Charge(120);
			}
			else if (_iChargeLevel == 3) {
				m_pChargeBar_1->Add_Charge(240);
			}
		}

		// ������ Ȯ��
		iCharge = m_pChargeBar_1->Get_Charge();
		// ���� ����
		// 39���ϸ� 0����
		if (iCharge <= 39) {
			m_pChargeBar_1->Set_ChargeLevel(0);
		}
		// 40�̻� 119���ϸ� 1����
		else if (40 <= iCharge && iCharge <= 119) {
			m_pChargeBar_1->Set_ChargeLevel(1);
		}
		// 120�̻� 239���ϸ� 2����
		else if (120 <= iCharge && iCharge <= 239) {
			m_pChargeBar_1->Set_ChargeLevel(2);
		}
		// 240 MAX 3����
		else if (240 == iCharge)
		{
			m_pChargeBar_1->Set_ChargeLevel(3);
		}
	}
	// 2�÷��̾�
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		// 3�� �Ѱų� ������ �Ǹ� �ȵ�
		iChargeLevel = m_pChargeBar_2->Get_ChargeLevel();
		iChargeLevel = iChargeLevel + _iChargeLevel;
		if (iChargeLevel < 0) {
			return;
		}
		else if (3 < iChargeLevel) {
			return;
		}

		// ����
		if (_iChargeLevel < 0) {
			if (_iChargeLevel == -1) {
				m_pChargeBar_2->Add_Charge(-40);
			}
			else if (_iChargeLevel == -2) {
				m_pChargeBar_2->Add_Charge(-120);
			}
			else if (_iChargeLevel == -3) {
				m_pChargeBar_2->Add_Charge(-240);
			}
		}
		// ���
		else {
			if (_iChargeLevel == 1) {
				m_pChargeBar_2->Add_Charge(40);
			}
			else if (_iChargeLevel == 2) {
				m_pChargeBar_2->Add_Charge(120);
			}
			else if (_iChargeLevel == 3) {
				m_pChargeBar_2->Add_Charge(240);
			}
		}

		// ������ Ȯ��
		iCharge = m_pChargeBar_2->Get_Charge();
		// ���� ����
		// 39���ϸ� 0����
		if (iCharge <= 39) {
			m_pChargeBar_2->Set_ChargeLevel(0);
		}
		// 40�̻� 119���ϸ� 1����
		else if (40 <= iCharge && iCharge <= 119) {
			m_pChargeBar_2->Set_ChargeLevel(1);
		}
		// 120�̻� 239���ϸ� 2����
		else if (120 <= iCharge && iCharge <= 239) {
			m_pChargeBar_2->Set_ChargeLevel(2);
		}
		// 240 MAX 3����
		else if (240 == iCharge)
		{
			m_pChargeBar_2->Set_ChargeLevel(3);
		}
	}
	else {
		throw;
	}
}

// ��ź �� ���
int CGameUIMgr::Get_Bomb(int _iPlayerNum)
{
	int iBomb = 0;

	// 1 �÷��̾�
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		iBomb = m_pBomb_1->Get_Bomb();
	}
	// 2 �÷��̾�
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		iBomb = m_pBomb_2->Get_Bomb();
	}
	else {
		throw;
	}

	return iBomb;
}

// ��ź �� ����
void CGameUIMgr::Set_Bomb(int _iBomb, int _iPlayerNum)
{
	// 1 �÷��̾�
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		m_pBomb_1->Set_Bomb(_iBomb);
	}
	// 2 �÷��̾�
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		m_pBomb_2->Set_Bomb(_iBomb);
	}
	else {
		throw;
	}
}

// ��ź �� �߰�
void CGameUIMgr::Add_Bomb(int _iBomb, int _iPlayerNum)
{
	// 1 �÷��̾�
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		m_pBomb_1->Add_Bomb(_iBomb);
	}
	// 2 �÷��̾�
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		m_pBomb_2->Add_Bomb(_iBomb);
	}
	else {
		throw;
	}
}

// ũ���� ���
int CGameUIMgr::Get_Credit()
{
	return m_pCredit->Get_Credit();
}

// ũ���� ����
void CGameUIMgr::Set_Credit(int _iCredit)
{
	m_pCredit->Set_Credit(_iCredit);
}

// ũ���� �߰�
void CGameUIMgr::Add_Credit(int _iCredit)
{
	int iCredit = m_pCredit->Get_Credit();
	iCredit = iCredit + _iCredit;
	if (0 <= iCredit && iCredit < 100) {
		m_pCredit->Add_Credit(_iCredit);
	}
}

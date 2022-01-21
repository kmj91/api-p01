// 기명준
// UI 매니저

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

// 타이틀 로고 초기화
void CGameUIMgr::Init_Logo()
{
	// 타이틀 로고
	m_pTitleLogo = CObjFactory<CUI_TitleLogo>::Create();
	CObjMgr::Get_Instance()->Add_Object(m_pTitleLogo, OBJID::UI);

	// 크레딧
	m_pCredit = CObjFactory<CUI_Credit>::Create(float(WINCX >> 1), WINCY - (19.f * 3.f));
	CObjMgr::Get_Instance()->Add_Object(m_pCredit, OBJID::UI);
}

// 캐릭터 선택 메뉴 초기화
void CGameUIMgr::Init_SeleteMenu()
{
	// 크레딧
	m_pCredit = CObjFactory<CUI_Credit>::Create((WINCX >> 1) + (70.f * 3.f), WINCY - (19.f * 3.f));
	CObjMgr::Get_Instance()->Add_Object(m_pCredit, OBJID::UI);
}

// 게임 스테이지 초기화
void CGameUIMgr::Init_Stage()
{
	if (m_bPlayer_1) {
		// 플레이어 라이프
		m_pPlayerLife_1 = CObjFactory<CUI_PlayerLife>::Create(3.f * 3.f, 19.f * 3.f);
		m_pPlayerLife_1->Set_DrawID(g_ePlayer_1_Name);
		m_pPlayerLife_1->Add_Life(2);
		CObjMgr::Get_Instance()->Add_Object(m_pPlayerLife_1, OBJID::UI);

		// 플레이어 번호
		m_pPlayerNum_1 = CObjFactory<CUI_Static>::Create(10.f * 3.f, 7.f * 3.f);
		m_pPlayerNum_1->Set_FrameKey(L"UI_PlayerNum");
		m_pPlayerNum_1->Set_ImageSize(UI_PLAYER_NUM_WIDTH, UI_PLAYER_NUM_HEIGHT);
		CObjMgr::Get_Instance()->Add_Object(m_pPlayerNum_1, OBJID::UI);

		// 스코어
		m_pScore_1 = CObjFactory<CUI_Score>::Create(85.f * 3.f, 6.f * 3.f);
		CObjMgr::Get_Instance()->Add_Object(m_pScore_1, OBJID::UI);

		// 차지 바
		m_pChargeBar_1 = CObjFactory<CUI_ChargeBar>::Create(13.f * 3.f, WINCY - (22.f * 3.f));
		m_pChargeBar_1->Set_Charge(40);
		m_pChargeBar_1->Set_ChargeLevel(1);
		CObjMgr::Get_Instance()->Add_Object(m_pChargeBar_1, OBJID::UI);

		// 폭탄 개수
		m_pBomb_1 = CObjFactory<CUI_Bomb>::Create(87.f * 3.f, WINCY - (19.f * 3.f));
		m_pBomb_1->Add_Bomb(2);
		CObjMgr::Get_Instance()->Add_Object(m_pBomb_1, OBJID::UI);
	}

	if (m_bPlayer_2) {
		// 플레이어 라이프
		m_pPlayerLife_2 = CObjFactory<CUI_PlayerLife>::Create(115.f * 3.f, 19.f * 3.f);
		m_pPlayerLife_2->Set_DrawID(g_ePlayer_2_Name);
		m_pPlayerLife_2->Add_Life(2);
		CObjMgr::Get_Instance()->Add_Object(m_pPlayerLife_2, OBJID::UI);
	}

	// 컨티뉴
	m_pMainContinue = CObjFactory<CUI_MainContinue>::Create(441.f, 414.f);
	CObjMgr::Get_Instance()->Add_Object(m_pMainContinue, OBJID::UI);

	// 스테이지 번호
	m_pStageNum = CObjFactory<CUI_Static>::Create();
	m_pStageNum->Set_Pos((WINCX >> 1) - 6.f * 3.f, 6.f * 3.f);
	m_pStageNum->Set_FrameKey(L"UI_StageNum");
	m_pStageNum->Set_ImageSize(UI_STAGE_NUM_WIDTH, UI_STAGE_NUM_HEIGHT);
	CObjMgr::Get_Instance()->Add_Object(m_pStageNum, OBJID::UI);

	// 크레딧
	m_pCredit = CObjFactory<CUI_Credit>::Create((WINCX >> 1) + (70.f * 3.f), WINCY - (19.f * 3.f));
	CObjMgr::Get_Instance()->Add_Object(m_pCredit, OBJID::UI);
}

void CGameUIMgr::Release()
{
}

// 플레이어 라이프를 모두 소진해서 플레이어로부터 이어하기를 기다립니다
// 다른 플레이어가 게임 중이라면 작은 UI를
// 혼자 플레이 중이라면 큰 UI를 호출합니다
void CGameUIMgr::On_ContinueCountdown(int _iPlayerNum)
{
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		// 1 플레이어 컨티뉴 ON
		g_bGameContinuePlayer_1 = true;
		// 다른 플레이어 게임중
		if (m_bPlayer_2 && !g_bGameContinuePlayer_2) {
			// 작은 UI
			return;
		}
		// 큰 UI
		// 게임 정지
		g_bCountdownStop = true;
		m_pMainContinue->Start_Countdown();
	}
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		// 2 플레이어 컨티뉴 ON
		g_bGameContinuePlayer_2 = true;
		// 다른 플레이어 게임중
		if (m_bPlayer_1 && !g_bGameContinuePlayer_1) {
			// 작은 UI
			return;
		}
		// 큰 UI
		// 게임 정지
		g_bCountdownStop = true;
	}
	else {
		throw;
	}
}

// 컨티뉴 카운트 끄기
void CGameUIMgr::Off_ContinueCountdown(int _iPlayerNum)
{
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		g_bGameContinuePlayer_1 = false;
		// 다른 플레이어 게임중
		if (m_bPlayer_2 && !g_bGameContinuePlayer_2) {
			// 작은 UI
			return;
		}
		// 큰 UI
		// 게임 정지
		g_bCountdownStop = false;
		m_pMainContinue->Stop_Countdown();
	}
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		g_bGameContinuePlayer_2 = false;
		// 다른 플레이어 게임중
		if (m_bPlayer_1 && !g_bGameContinuePlayer_1) {
			// 작은 UI
			return;
		}
		// 큰 UI
		// 게임 정지
		g_bCountdownStop = false;
	}
	else {
		throw;
	}
}

// 카운트 진행
void CGameUIMgr::NextMainCount()
{
	// 카운트 감소
	m_pMainContinue->NextCount();
}

// 라이프 얻기
int CGameUIMgr::Get_Life(int _iPlayerNum)
{
	int iLife = 0;

	// 1 플레이어
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		iLife = m_pPlayerLife_1->Get_Life();
	}
	// 2 플레이어
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		iLife = m_pPlayerLife_2->Get_Life();
	}
	else {
		throw;
	}

	return iLife;
}

// 라이프 추가
void CGameUIMgr::Add_Life(int _iLife, int _iPlayerNum)
{
	// 1 플레이어
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		m_pPlayerLife_1->Add_Life(_iLife);
	}
	// 2 플레이어
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		m_pPlayerLife_2->Add_Life(_iLife);
	}
	else {
		throw;
	}
}

// 스코어 얻기
int CGameUIMgr::Get_Score(int _iPlayerNum)
{
	int iScore = 0;

	// 1 플레이어
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		iScore = m_pScore_1->Get_Score();
	}
	// 2 플레이어
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		iScore = m_pScore_2->Get_Score();
	}
	else {
		throw;
	}

	return iScore;
}

// 스코어 추가
void CGameUIMgr::Add_Score(int _iScore, int _iPlayerNum)
{
	// 1 플레이어
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		m_pScore_1->Add_Score(_iScore);
	}
	// 2 플레이어
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		m_pScore_2->Add_Score(_iScore);
	}
	else {
		throw;
	}
}

// 충전량 얻기
int CGameUIMgr::Get_Charge(int _iPlayerNum)
{
	int iCharge = 0;

	// 1 플레이어
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		iCharge = m_pChargeBar_1->Get_Charge();
	}
	// 2 플레이어
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		iCharge = m_pChargeBar_2->Get_Charge();
	}
	else {
		throw;
	}

	return iCharge;
}

// 충전량 추가
void CGameUIMgr::Add_Charge(int _iCharge, int _iPlayerNum)
{
	int iCharge;		// 충전량
	int iChargeLevel;	// 충전 레벨

	// 1 플레이어
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		// 더하기전 충전 레벨 저장
		iChargeLevel = m_pChargeBar_1->Get_ChargeLevel();
		// 240을 넘거나 음수가 되면 안됨
		iCharge = m_pChargeBar_1->Get_Charge();
		iCharge = iCharge + _iCharge;
		if (iCharge < 0) {
			return;
		}
		else if (240 < iCharge) {
			return;
		}

		// 값을 더하고
		m_pChargeBar_1->Add_Charge(_iCharge);
		// 39이하면 0레벨
		if (iCharge <= 39) {
			m_pChargeBar_1->Set_ChargeLevel(0);
		}
		// 40이상 119이하면 1레벨
		else if (40 <= iCharge && iCharge <= 119) {
			m_pChargeBar_1->Set_ChargeLevel(1);
		}
		// 120이상 239이하면 2레벨
		else if (120 <= iCharge && iCharge <= 239) {
			m_pChargeBar_1->Set_ChargeLevel(2);
		}
		// 240 MAX 3레벨
		else if(240 == iCharge)
		{
			m_pChargeBar_1->Set_ChargeLevel(3);
		}

		// 충전 레벨 비교
		if (iChargeLevel < m_pChargeBar_1->Get_ChargeLevel()) {
			// 값이 더 커졌으면
			// 충전 사운드 출력
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ITEM);
			CSoundMgr::Get_Instance()->PlaySound(L"UI_ChargeReadyOK.wav", CSoundMgr::ITEM);
		}
	}
	// 2 플레이어
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		// 더하기전 충전 레벨 저장
		iChargeLevel = m_pChargeBar_2->Get_ChargeLevel();
		// 240을 넘거나 음수가 되면 안됨
		iCharge = m_pChargeBar_2->Get_Charge();
		iCharge = iCharge + _iCharge;
		if (iCharge < 0) {
			return;
		}
		else if (240 < iCharge) {
			return;
		}

		// 값을 더하고
		m_pChargeBar_2->Add_Charge(_iCharge);
		// 39이하면 0레벨
		if (iCharge <= 39) {
			m_pChargeBar_2->Set_ChargeLevel(0);
		}
		// 40이상 119이하면 1레벨
		else if (40 <= iCharge && iCharge <= 119) {
			m_pChargeBar_2->Set_ChargeLevel(1);
		}
		// 120이상 239이하면 2레벨
		else if (120 <= iCharge && iCharge <= 239) {
			m_pChargeBar_2->Set_ChargeLevel(2);
		}
		// 240 MAX 3레벨
		else if (240 == iCharge)
		{
			m_pChargeBar_2->Set_ChargeLevel(3);
		}

		// 충전 레벨 비교
		if (iChargeLevel < m_pChargeBar_2->Get_ChargeLevel()) {
			// 값이 더 커졌으면
			// 충전 사운드 출력
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ITEM);
			CSoundMgr::Get_Instance()->PlaySound(L"UI_ChargeReadyOK.wav", CSoundMgr::ITEM);
		}
	}
}

// 충전 레벨 얻기
int CGameUIMgr::Get_ChargeLevel(int _iPlayerNum)
{
	int iChargeLevel = 0;

	// 1 플레이어
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		iChargeLevel = m_pChargeBar_1->Get_ChargeLevel();
	}
	// 2 플레이어
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		iChargeLevel = m_pChargeBar_2->Get_ChargeLevel();
	}
	else {
		throw;
	}

	return iChargeLevel;
}

// 충전 레벨 셋팅
void CGameUIMgr::Set_ChargeLevel(int _iChargeLevel, int _iPlayerNum)
{
	// 1플레이어
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
		// 레벨 변경
		m_pChargeBar_1->Set_ChargeLevel(_iChargeLevel);
	}
	// 2플레이어
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
		// 레벨 변경
		m_pChargeBar_2->Set_ChargeLevel(_iChargeLevel);
	}
	else {
		throw;
	}
}

// 충전 레벨 추가
void CGameUIMgr::Add_ChargeLevel(int _iChargeLevel, int _iPlayerNum)
{
	int iCharge;		// 충전량
	int iChargeLevel;	// 충전 레벨

	// 1플레이어
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		// 3을 넘거나 음수가 되면 안됨
		iChargeLevel = m_pChargeBar_1->Get_ChargeLevel();
		iChargeLevel = iChargeLevel + _iChargeLevel;
		if (iChargeLevel < 0) {
			return;
		}
		else if (3 < iChargeLevel) {
			return;
		}
		
		// 음수
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
		// 양수
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

		// 충전량 확인
		iCharge = m_pChargeBar_1->Get_Charge();
		// 레벨 변경
		// 39이하면 0레벨
		if (iCharge <= 39) {
			m_pChargeBar_1->Set_ChargeLevel(0);
		}
		// 40이상 119이하면 1레벨
		else if (40 <= iCharge && iCharge <= 119) {
			m_pChargeBar_1->Set_ChargeLevel(1);
		}
		// 120이상 239이하면 2레벨
		else if (120 <= iCharge && iCharge <= 239) {
			m_pChargeBar_1->Set_ChargeLevel(2);
		}
		// 240 MAX 3레벨
		else if (240 == iCharge)
		{
			m_pChargeBar_1->Set_ChargeLevel(3);
		}
	}
	// 2플레이어
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		// 3을 넘거나 음수가 되면 안됨
		iChargeLevel = m_pChargeBar_2->Get_ChargeLevel();
		iChargeLevel = iChargeLevel + _iChargeLevel;
		if (iChargeLevel < 0) {
			return;
		}
		else if (3 < iChargeLevel) {
			return;
		}

		// 음수
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
		// 양수
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

		// 충전량 확인
		iCharge = m_pChargeBar_2->Get_Charge();
		// 레벨 변경
		// 39이하면 0레벨
		if (iCharge <= 39) {
			m_pChargeBar_2->Set_ChargeLevel(0);
		}
		// 40이상 119이하면 1레벨
		else if (40 <= iCharge && iCharge <= 119) {
			m_pChargeBar_2->Set_ChargeLevel(1);
		}
		// 120이상 239이하면 2레벨
		else if (120 <= iCharge && iCharge <= 239) {
			m_pChargeBar_2->Set_ChargeLevel(2);
		}
		// 240 MAX 3레벨
		else if (240 == iCharge)
		{
			m_pChargeBar_2->Set_ChargeLevel(3);
		}
	}
	else {
		throw;
	}
}

// 폭탄 수 얻기
int CGameUIMgr::Get_Bomb(int _iPlayerNum)
{
	int iBomb = 0;

	// 1 플레이어
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		iBomb = m_pBomb_1->Get_Bomb();
	}
	// 2 플레이어
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		iBomb = m_pBomb_2->Get_Bomb();
	}
	else {
		throw;
	}

	return iBomb;
}

// 폭탄 수 셋팅
void CGameUIMgr::Set_Bomb(int _iBomb, int _iPlayerNum)
{
	// 1 플레이어
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		m_pBomb_1->Set_Bomb(_iBomb);
	}
	// 2 플레이어
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		m_pBomb_2->Set_Bomb(_iBomb);
	}
	else {
		throw;
	}
}

// 폭탄 수 추가
void CGameUIMgr::Add_Bomb(int _iBomb, int _iPlayerNum)
{
	// 1 플레이어
	if (_iPlayerNum == 1 && m_bPlayer_1) {
		m_pBomb_1->Add_Bomb(_iBomb);
	}
	// 2 플레이어
	else if (_iPlayerNum == 2 && m_bPlayer_2) {
		m_pBomb_2->Add_Bomb(_iBomb);
	}
	else {
		throw;
	}
}

// 크레딧 얻기
int CGameUIMgr::Get_Credit()
{
	return m_pCredit->Get_Credit();
}

// 크레딧 셋팅
void CGameUIMgr::Set_Credit(int _iCredit)
{
	m_pCredit->Set_Credit(_iCredit);
}

// 크레딧 추가
void CGameUIMgr::Add_Credit(int _iCredit)
{
	int iCredit = m_pCredit->Get_Credit();
	iCredit = iCredit + _iCredit;
	if (0 <= iCredit && iCredit < 100) {
		m_pCredit->Add_Credit(_iCredit);
	}
}

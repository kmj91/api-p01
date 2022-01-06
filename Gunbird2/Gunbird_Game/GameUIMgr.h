// 기명준
// UI 매니저

#pragma once

#ifndef __GAMEUI_H__
#define __GAMEUI_H__

class CUI_PlayerLife;
class CUI_MainContinue;
class CUI_Static;
class CUI_Score;
class CUI_ChargeBar;
class CUI_Bomb;
class CUI_TitleLogo;
class CUI_Credit;

class CGameUIMgr final
{
private:
	CGameUIMgr();
	~CGameUIMgr();

public:
	void Init_Logo();			// 로고
	void Init_SeleteMenu();		// 캐릭터 선택 메뉴
	void Init_Stage();			// 게임 스테이지
	void Release();

	void On_Player1() { m_bPlayer_1 = true; }
	void Off_Player1() { m_bPlayer_1 = false; }
	void On_Player2() { m_bPlayer_2 = true; }
	void Off_Player2() { m_bPlayer_2 = false; }

	// 컨티뉴 카운트 보이기
	void On_ContinueCountdown(int _iPlayerNum);
	// 컨티뉴 카운트 끄기
	void Off_ContinueCountdown(int _iPlayerNum);
	// 메인 카운트 감소
	void NextMainCount();

	// 플레이어 라이프
	int Get_Life(int _iPlayerNum);
	void Add_Life(int _iLife, int _iPlayerNum);
	// 플레이어 점수
	int Get_Score(int _iPlayerNum);
	void Add_Score(int _iScore, int _iPlayerNum);
	// 충전샷 게이지
	int Get_Charge(int _iPlayerNum);
	void Add_Charge(int _iCharge, int _iPlayerNum);
	// 충전샷 레벨
	int Get_ChargeLevel(int _iPlayerNum);
	void Set_ChargeLevel(int _iChargeLevel, int _iPlayerNum);
	void Add_ChargeLevel(int _iChargeLevel, int _iPlayerNum);
	// 폭탄 수
	int Get_Bomb(int _iPlayerNum);
	void Set_Bomb(int _iBomb, int _iPlayerNum);
	void Add_Bomb(int _iBomb, int _iPlayerNum);
	// 크레딧
	int Get_Credit();
	void Set_Credit(int _iCredit);
	void Add_Credit(int _iCredit);

public:
	static CGameUIMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CGameUIMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CGameUIMgr*		m_pInstance;
	CUI_PlayerLife*			m_pPlayerLife_1;
	CUI_PlayerLife*			m_pPlayerLife_2;
	CUI_MainContinue*		m_pMainContinue;
	CUI_Static*				m_pPlayerNum_1;
	CUI_Static*				m_pPlayerNum_2;
	CUI_Score*				m_pScore_1;
	CUI_Score*				m_pScore_2;
	CUI_Static*				m_pStageNum;
	CUI_ChargeBar*			m_pChargeBar_1;
	CUI_ChargeBar*			m_pChargeBar_2;
	CUI_Bomb*				m_pBomb_1;
	CUI_Bomb*				m_pBomb_2;
	CUI_TitleLogo*			m_pTitleLogo;
	CUI_Credit*				m_pCredit;
	//CObj*					m_pContinuePlayer_1;
	//CObj*					m_pContinuePlayer_2;
	bool					m_bPlayer_1;		// 플레이어 1 게임중
	bool					m_bPlayer_2;		// 플레이어 2 게임중
};

#endif // !__GAMEUI_H__



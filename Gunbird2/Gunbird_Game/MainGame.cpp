#include "stdafx.h"
#include "MainGame.h"

#include "SceneMgr.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "BmpMgr.h"
#include "SoundMgr.h"
#include "GameUIMgr.h"

#include "Player.h"


PLAYER::NAME	g_ePlayer_1_Name;
PLAYER::NAME	g_ePlayer_2_Name;
CPlayer*		g_pPlayer_1;				// 1 플레이어 전역 변수
CPlayer*		g_pPlayer_2;				// 2 플레이어 전역 변수
int				g_iCredit;					// 크레딧
float			g_fBackgroundSpeed;			// 배경 스크롤 속도
bool			g_bStopMapScroll;			// 배경 스크롤 정지
bool			g_bBossGO;					// 보스전 시작을 알림
bool			g_bGameContinuePlayer_1;	// 플레이어 1 다시하기
bool			g_bGameContinuePlayer_2;	// 플레이어 2 다시하기
bool			g_bCountdownStop;			// 게임 정지
BOOL			g_bActiveApp;				// 윈도우 활성화 플래그
bool			g_bGameOver;				// 게임 오버 - 타이틀 화면으로 씬 전환 플래그
bool			g_bHitRectRender;			// 충돌 렉트 랜더 플래그
bool			g_bStageClear;				// 스테이지 클리어 플래그

CMainGame::CMainGame()
	: m_DC(GetDC(g_hWnd)), m_iFPS(0), m_dwTime(GetTickCount()), m_szFPS{}
{
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	// 전역 변수 초기화
	g_ePlayer_1_Name = PLAYER::NAME_END;
	g_ePlayer_2_Name = PLAYER::NAME_END;
	g_pPlayer_1 = nullptr;
	g_pPlayer_2 = nullptr;
	g_iCredit = 0;
	g_fBackgroundSpeed = BACKGROUND_SPEED;
	g_bStopMapScroll = false;
	g_bBossGO = false;
	g_bGameContinuePlayer_1 = false;
	g_bGameContinuePlayer_2 = false;
	g_bCountdownStop = false;
	g_bActiveApp = TRUE;
	g_bGameOver = false;
	g_bHitRectRender = false;
	g_bStageClear = false;

	CSoundMgr::Get_Instance()->Initialize();

	//CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_SELECT_MENU);
	//CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_CG_STAGE_1);
	CSceneMgr::Get_Instance()->Request_SceneChange(CSceneMgr::SCENE_LOGO);
	CSceneMgr::Get_Instance()->SceneChange_Update();
}

void CMainGame::Update()
{
	CSceneMgr::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
	CSceneMgr::Get_Instance()->Late_Update();
	CKeyMgr::Get_Instance()->Key_Update();
}

void CMainGame::Render()
{
	HDC HBackBuffer = CBmpMgr::Get_Instance()->Find_Image(L"BackBuffer");

	CSceneMgr::Get_Instance()->Render(HBackBuffer);

	BitBlt(m_DC, 0, 0, WINCX, WINCY, HBackBuffer, 0, 0, SRCCOPY);

	++m_iFPS;
	if (m_dwTime + 1000 < GetTickCount())
	{
		swprintf_s(m_szFPS, L"FPS: %d", m_iFPS);
		SetWindowText(g_hWnd, m_szFPS);

		m_dwTime = GetTickCount();
		m_iFPS = 0;
	}
}

void CMainGame::SceneChange_Update()
{
	CSceneMgr::Get_Instance()->SceneChange_Update();
}

void CMainGame::Release()
{
	CSceneMgr::Destroy_Instance();
	CObjMgr::Destroy_Instance();
	CKeyMgr::Destroy_Instance();
	CBmpMgr::Destroy_Instance();
	CSoundMgr::Destroy_Instance();
	CGameUIMgr::Destroy_Instance();

	ReleaseDC(g_hWnd, m_DC);
}

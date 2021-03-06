// 기명준
// 로고 씬
// 게임의 제일 처음, 코인을 추가해서 게임 시작 가능

#include "stdafx.h"
#include "Logo.h"

#include "ObjMgr.h"
#include "KeyMgr.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "GameUIMgr.h"


CLogo::CLogo()
{
}


CLogo::~CLogo()
{
	Release();
}

void CLogo::Initialize()
{
	// 버퍼
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/BackBuffer.bmp", L"BackBuffer");

	// BG
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/BG_Black.bmp", L"BG_Black");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/BG_White.bmp", L"BG_White");
	
	// UI
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/CG/CG_Kusuri_BG.bmp", L"CG_Kusuri_BG");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_TitleLogo_1.bmp", L"UI_TitleLogo_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_TitleLogo_2.bmp", L"UI_TitleLogo_2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_Credit.bmp", L"UI_Credit");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_SmallNumber.bmp", L"UI_SmallNumber");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_InsertCoins.bmp", L"UI_InsertCoins");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_Start.bmp", L"UI_Start");

	// 게임 UI 생성
	CGameUIMgr::Get_Instance()->Init_Logo();
	CGameUIMgr::Get_Instance()->Set_Credit(g_iCredit);

	g_bStageClear = false;
}

// 업데이트
void CLogo::Update()
{
	// 키 입력 처리
	Key_Check();

	// 일시 정지
	if (m_isPause)
		return;

	// 오브젝트 업데이트
	CObjMgr::Get_Instance()->Update();
}

// 레이트 업데이트
void CLogo::Late_update()
{
	// 일시 정지
	if (m_isPause)
		return;

	// 오브젝트 레이트 업데이트
	CObjMgr::Get_Instance()->Late_Update();
}

// 렌더
void CLogo::Render(HDC _DC)
{
	// 오브젝트 렌더
	CObjMgr::Get_Instance()->Render(_DC);
}

void CLogo::Release()
{
	CBmpMgr::Get_Instance()->Release();
	CObjMgr::Get_Instance()->Release();
	CSoundMgr::Get_Instance()->StopAll();
}

// 키 입력 처리
void CLogo::Key_Check()
{
	// 윈도우가 활성화 됬을 때만 키처리
	if (!g_bActiveApp) {
		return;
	}

	// 중지
	// 랜더를 제외한 모든 업데이트 중단
	if (CKeyMgr::Get_Instance()->Key_Down('P'))
	{
		m_isPause = !m_isPause;
	}

	// 일시 정지
	if (m_isPause)
		return;

	// 1 key 
	if (CKeyMgr::Get_Instance()->Key_Down('1'))
	{
		// 크레딧이 있으면
		if (0 < CGameUIMgr::Get_Instance()->Get_Credit())
		{
			// 크레딧 감소
			CGameUIMgr::Get_Instance()->Add_Credit(-1);
			// 크레딧 저장
			g_iCredit = CGameUIMgr::Get_Instance()->Get_Credit();
			// 씬 체인지
			CSceneMgr::Get_Instance()->Request_SceneChange(CSceneMgr::SCENE_SELECT_MENU);
		}
	}

	// 5 key 
	if (CKeyMgr::Get_Instance()->Key_Down('5'))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI);
		CSoundMgr::Get_Instance()->PlaySound(L"AddCredit.wav", CSoundMgr::UI);
		CGameUIMgr::Get_Instance()->Add_Credit(1);
	}
}

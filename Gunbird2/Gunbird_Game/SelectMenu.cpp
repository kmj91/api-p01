#include "stdafx.h"
#include "SelectMenu.h"

#include "ObjMgr.h"
#include "KeyMgr.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "GameUIMgr.h"

#include "PlayerSelect.h"


CSelectMenu::CSelectMenu()
{
}


CSelectMenu::~CSelectMenu()
{
	Release();
}

void CSelectMenu::Initialize()
{
	// 버퍼
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/BackBuffer.bmp", L"BackBuffer");

	// BG
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/SelectMenu/BG_SelectMenu.bmp", L"BG_SelectMenu");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/SelectMenu/BG_Select.bmp", L"BG_Select");

	// UI
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/SelectMenu/Character_Select.bmp", L"CharacterSelect");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/SelectMenu/PlayerSelect.bmp", L"PlayerSelect");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_Credit.bmp", L"UI_Credit");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_SmallNumber.bmp", L"UI_SmallNumber");

	// 플레이어
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Alucard/Alucard_Move.bmp", L"Alucard_Move");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Marion/Marion_Move.bmp", L"Marion_Move");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Valpiro/Valpiro_Move.bmp", L"Valpiro_Move");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Tavia/Tavia_Move.bmp", L"Tavia_Move");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Heicob/Heicob_Move.bmp", L"Heicob_Move");

	m_tFrame[PLAYER::ALUCARD].iFrameCnt = 0;
	m_tFrame[PLAYER::ALUCARD].iFrameStart = 0;
	m_tFrame[PLAYER::ALUCARD].iFrameEnd = 5;
	m_tFrame[PLAYER::ALUCARD].iFrameScene = 0;
	m_tFrame[PLAYER::ALUCARD].dwFrameTime = GetTickCount();
	m_tFrame[PLAYER::ALUCARD].dwFrameSpeed = 100;

	m_tFrame[PLAYER::MARION].iFrameCnt = 0;
	m_tFrame[PLAYER::MARION].iFrameStart = 0;
	m_tFrame[PLAYER::MARION].iFrameEnd = 3;
	m_tFrame[PLAYER::MARION].iFrameScene = 0;
	m_tFrame[PLAYER::MARION].dwFrameTime = GetTickCount();
	m_tFrame[PLAYER::MARION].dwFrameSpeed = 100;

	m_tFrame[PLAYER::VALPIRO].iFrameCnt = 0;
	m_tFrame[PLAYER::VALPIRO].iFrameStart = 0;
	m_tFrame[PLAYER::VALPIRO].iFrameEnd = 3;
	m_tFrame[PLAYER::VALPIRO].iFrameScene = 0;
	m_tFrame[PLAYER::VALPIRO].dwFrameTime = GetTickCount();
	m_tFrame[PLAYER::VALPIRO].dwFrameSpeed = 100;

	m_tFrame[PLAYER::TAVIA].iFrameCnt = 0;
	m_tFrame[PLAYER::TAVIA].iFrameStart = 0;
	m_tFrame[PLAYER::TAVIA].iFrameEnd = 3;
	m_tFrame[PLAYER::TAVIA].iFrameScene = 0;
	m_tFrame[PLAYER::TAVIA].dwFrameTime = GetTickCount();
	m_tFrame[PLAYER::TAVIA].dwFrameSpeed = 100;

	m_tFrame[PLAYER::HEI_COB].iFrameCnt = 0;
	m_tFrame[PLAYER::HEI_COB].iFrameStart = 0;
	m_tFrame[PLAYER::HEI_COB].iFrameEnd = 3;
	m_tFrame[PLAYER::HEI_COB].iFrameScene = 0;
	m_tFrame[PLAYER::HEI_COB].dwFrameTime = GetTickCount();
	m_tFrame[PLAYER::HEI_COB].dwFrameSpeed = 100;

	// 플레이어
	m_pPlayer = CAbstractFactory<CPlayerSelect>::Create();
	CObjMgr::Get_Instance()->Add_Object(m_pPlayer, OBJID::PLAYER);

	// 게임 UI 생성
	CGameUIMgr::Get_Instance()->Init_Logo();
	CGameUIMgr::Get_Instance()->Set_Credit(g_iCredit);

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
	CSoundMgr::Get_Instance()->PlayBGM(L"SelectMenu.wav");
}

void CSelectMenu::Update()
{
	// 키 입력 처리
	Key_Check();

	if (m_isPause)
		return;
	
	Frame_Move();

	CObjMgr::Get_Instance()->Update();
}

void CSelectMenu::Late_update()
{
	if (m_isPause)
		return;

	CObjMgr::Get_Instance()->Late_Update();
}

void CSelectMenu::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"BG_SelectMenu");
	GdiTransparentBlt(_DC
		, 0, 0
		, PUBLIC_BMP_WIDTH * 3, PUBLIC_BMP_HEIGHT * 3
		, hMemDC
		, 0, 0
		, PUBLIC_BMP_WIDTH, PUBLIC_BMP_HEIGHT
		, RGB(255, 0, 255));

	// 오브젝트 리스트를 먼저 랜더
	CObjMgr::Get_Instance()->Render(_DC);


	// 선택창 블랜더 BG
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"BG_Select");
	AlphaBlend(_DC
		, 0, WINCY - (68 * 3)
		, 224 * 3, 40 * 3
		, hMemDC
		, 0, 0
		, 224, 40
		, { AC_SRC_OVER, 0, 125, 0 });
	// 플레이어 모습 랜더
	// 아카드
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Alucard_Move");
	GdiTransparentBlt(_DC
		, 38, 777
		, ALUCARD_MOVE_WIDTH * 3, ALUCARD_MOVE_HEIGHT * 3
		, hMemDC
		, m_tFrame[PLAYER::ALUCARD].iFrameCnt * ALUCARD_MOVE_WIDTH, m_tFrame[PLAYER::ALUCARD].iFrameScene * ALUCARD_MOVE_HEIGHT
		, ALUCARD_MOVE_WIDTH, ALUCARD_MOVE_HEIGHT
		, RGB(255, 0, 255));

	// 마리온
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Marion_Move");
	GdiTransparentBlt(_DC
		, 148, 780
		, MARION_MOVE_WIDTH * 3, MARION_MOVE_HEIGHT * 3
		, hMemDC
		, m_tFrame[PLAYER::MARION].iFrameCnt * MARION_MOVE_WIDTH, m_tFrame[PLAYER::MARION].iFrameScene * MARION_MOVE_HEIGHT
		, MARION_MOVE_WIDTH, MARION_MOVE_HEIGHT
		, RGB(255, 0, 255));

	// 발피로
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Valpiro_Move");
	GdiTransparentBlt(_DC
		, 234, 779
		, VALPIRO_MOVE_WIDTH * 3, VALPIRO_MOVE_HEIGHT * 3
		, hMemDC
		, m_tFrame[PLAYER::VALPIRO].iFrameCnt * VALPIRO_MOVE_WIDTH, m_tFrame[PLAYER::VALPIRO].iFrameScene * VALPIRO_MOVE_HEIGHT
		, VALPIRO_MOVE_WIDTH, VALPIRO_MOVE_HEIGHT
		, RGB(255, 0, 255));

	// 타비아
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Tavia_Move");
	GdiTransparentBlt(_DC
		, 356, 783
		, TAVIA_MOVE_WIDTH * 3, TAVIA_MOVE_HEIGHT * 3
		, hMemDC
		, m_tFrame[PLAYER::TAVIA].iFrameCnt * TAVIA_MOVE_WIDTH, m_tFrame[PLAYER::TAVIA].iFrameScene * TAVIA_MOVE_HEIGHT
		, TAVIA_MOVE_WIDTH, TAVIA_MOVE_HEIGHT
		, RGB(255, 0, 255));

	// 헤이콥
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Heicob_Move");
	GdiTransparentBlt(_DC
		, 434, 777
		, HEICOB_MOVE_WIDTH * 3, HEICOB_MOVE_HEIGHT * 3
		, hMemDC
		, m_tFrame[PLAYER::HEI_COB].iFrameCnt * HEICOB_MOVE_WIDTH, m_tFrame[PLAYER::HEI_COB].iFrameScene * HEICOB_MOVE_HEIGHT
		, HEICOB_MOVE_WIDTH, HEICOB_MOVE_HEIGHT
		, RGB(255, 0, 255));

	
}

void CSelectMenu::Release()
{
	CBmpMgr::Get_Instance()->Release();
	CObjMgr::Get_Instance()->Release();
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
}

void CSelectMenu::Key_Check()
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

	if (m_isPause)
		return;

	// 5 key 
	if (CKeyMgr::Get_Instance()->Key_Down('5'))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI);
		CSoundMgr::Get_Instance()->PlaySound(L"AddCredit.wav", CSoundMgr::UI);
		CGameUIMgr::Get_Instance()->Add_Credit(1);
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_LEFT))
	{
		static_cast<CPlayerSelect*>(m_pPlayer)->Key_Input(VK_LEFT);
	}
	else if (CKeyMgr::Get_Instance()->Key_Down(VK_RIGHT))
	{
		static_cast<CPlayerSelect*>(m_pPlayer)->Key_Input(VK_RIGHT);
	}
	else if (CKeyMgr::Get_Instance()->Key_Down('A'))
	{
		g_ePlayer_1_Name = static_cast<CPlayerSelect*>(m_pPlayer)->Get_State();

		if (g_ePlayer_1_Name == PLAYER::MARION)
		{
			// 선택 사운드
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_DEAD);
			CSoundMgr::Get_Instance()->PlaySound(L"Marion_Start.wav", CSoundMgr::PLAYER_DEAD);
			// 크레딧 저장
			g_iCredit = CGameUIMgr::Get_Instance()->Get_Credit();
			// 씬 전환
			CSceneMgr::Get_Instance()->Request_SceneChange(CSceneMgr::SCENE_STAGE);
			return;
		}
	}
	
}

void CSelectMenu::Frame_Move()
{
	int iCnt = 0;
	while (iCnt < static_cast<int>(PLAYER::NAME_END))
	{
		if (m_tFrame[iCnt].dwFrameTime + m_tFrame[iCnt].dwFrameSpeed < GetTickCount())
		{
			++m_tFrame[iCnt].iFrameCnt;
			if (m_tFrame[iCnt].iFrameCnt > m_tFrame[iCnt].iFrameEnd)
				m_tFrame[iCnt].iFrameCnt = m_tFrame[iCnt].iFrameStart;

			m_tFrame[iCnt].dwFrameTime = GetTickCount();
		}
		++iCnt;
	}
}

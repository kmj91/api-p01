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
	// ����
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

	// ���� UI ����
	CGameUIMgr::Get_Instance()->Init_Logo();
	CGameUIMgr::Get_Instance()->Set_Credit(g_iCredit);

	g_bStageClear = false;
}

void CLogo::Update()
{
	// Ű �Է� ó��
	Key_Check();

	// �� ��ȯ
	if (g_bStageClear) {
		// �� ü����
		CSceneMgr::Get_Instance()->Request_SceneChange(CSceneMgr::SCENE_SELECT_MENU);
	}

	if (m_isPause)
		return;

	CObjMgr::Get_Instance()->Update();
}

void CLogo::Late_update()
{
	if (m_isPause)
		return;

	CObjMgr::Get_Instance()->Late_Update();
}

void CLogo::Render(HDC _DC)
{
	CObjMgr::Get_Instance()->Render(_DC);
}

void CLogo::Release()
{
	CBmpMgr::Get_Instance()->Release();
	CObjMgr::Get_Instance()->Release();
	CSoundMgr::Get_Instance()->StopAll();
}

void CLogo::Key_Check()
{
	// �����찡 Ȱ��ȭ ���� ���� Űó��
	if (!g_bActiveApp) {
		return;
	}

	// ����
	// ������ ������ ��� ������Ʈ �ߴ�
	if (CKeyMgr::Get_Instance()->Key_Down('P'))
	{
		m_isPause = !m_isPause;
	}

	if (m_isPause)
		return;

	// 1 key 
	if (CKeyMgr::Get_Instance()->Key_Down('1'))
	{
		// ũ������ ������
		if (0 < CGameUIMgr::Get_Instance()->Get_Credit())
		{
			// ũ���� ����
			CGameUIMgr::Get_Instance()->Add_Credit(-1);
			// ũ���� ����
			g_iCredit = CGameUIMgr::Get_Instance()->Get_Credit();
			// �� ü����
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

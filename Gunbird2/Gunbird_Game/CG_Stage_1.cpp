#include "stdafx.h"
#include "CG_Stage_1.h"

#include "ObjMgr.h"
#include "KeyMgr.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"

#include "CG_Kusuri.h"


CCG_Stage_1::CCG_Stage_1()
{
}


CCG_Stage_1::~CCG_Stage_1()
{
	Release();
}

void CCG_Stage_1::Initialize()
{
	// 버퍼
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/BackBuffer.bmp", L"BackBuffer");

	// CG
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/CG/CG_Kusuri_BG.bmp", L"CG_Kusuri_BG");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/CG/CG_Kusuri_1.bmp", L"CG_Kusuri_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/CG/CG_Marion_01.bmp", L"CG_Marion_01");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/CG/CG_Usagi_01.bmp", L"CG_Usagi_01");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/CG/CG_TextBox.bmp", L"CG_TextBox");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/CG/CG_Text_Stage_1.bmp", L"CG_Text_Stage_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_TheEnd.bmp", L"UI_TheEnd");

	// 오브젝트 생성
	CObj* pObj = CAbstractFactory<CCG_Kusuri>::Create();
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::UI);

	g_bStageClear = false;
}

void CCG_Stage_1::Update()
{
	// 키 입력 처리
	Key_Check();

	// 씬 전환
	if (g_bStageClear) {
		// 씬 체인지
		CSceneMgr::Get_Instance()->Request_SceneChange(CSceneMgr::SCENE_LOGO);
	}

	if (m_isPause)
		return;

	CObjMgr::Get_Instance()->Update();
}

void CCG_Stage_1::Late_update()
{
	if (m_isPause)
		return;

	CObjMgr::Get_Instance()->Late_Update();
}

void CCG_Stage_1::Render(HDC _DC)
{
	CObjMgr::Get_Instance()->Render(_DC);
}

void CCG_Stage_1::Release()
{
	CBmpMgr::Get_Instance()->Release();
	CObjMgr::Get_Instance()->Release();
	CSoundMgr::Get_Instance()->StopAll();
}

void CCG_Stage_1::Key_Check()
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
}

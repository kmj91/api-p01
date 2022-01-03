// 기명준
// 게임 스테이지 씬
// 게임 플레이에 필요한 몬스터 생성 및 키 처리

#include "stdafx.h"
#include "Stage.h"

#include "ObjMgr.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"

#include "Player.h"


CStage::CStage()
	: m_pBackground(nullptr), m_iStage(1), m_arrCreateObjcet{}
{
}


CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
	Load_Stage();
}

void CStage::Update()
{
	// 게임 오버 씬 전환
	if (g_bGameOver) {
		g_bGameOver = false;
		g_bCountdownStop = false;
		CSoundMgr::Get_Instance()->StopAll();
		CSceneMgr::Get_Instance()->Request_SceneChange(CSceneMgr::SCENE_LOGO);
	}

	// 게임 클리어 씬 전환
	if (g_bStageClear) {
		if (g_pPlayer_1->Get_Info().fY < -100.f) {
			// 씬 체인지
			CSceneMgr::Get_Instance()->Request_SceneChange(CSceneMgr::SCENE_CG_STAGE_1);
		}
	}

	// 키 입력 처리
	Key_Check();

	// 게임 컨티뉴 중
	if (m_isPause || g_bCountdownStop) {
		// UI만 업데이트
		CObjMgr::Get_Instance()->UI_Update();
		return;
	}

	Create_Object();	// 오브젝트 만들기

	CObjMgr::Get_Instance()->Update();
}

void CStage::Late_update()
{
	// 게임 컨티뉴 중
	if (m_isPause || g_bCountdownStop) {
		// UI만 업데이트
		CObjMgr::Get_Instance()->UI_LateUpdate();
		return;
	}

	CObjMgr::Get_Instance()->Late_Update();
}

void CStage::Render(HDC _DC)
{
	CObjMgr::Get_Instance()->Render(_DC);
}

void CStage::Release()
{
	CBmpMgr::Get_Instance()->Release();
	CObjMgr::Get_Instance()->Release();
}
// 기명준
// 씬 매니저
// 씬 관리

#include "stdafx.h"
#include "SceneMgr.h"

#include "Stage.h"
#include "SelectMenu.h"
#include "CG_Stage_1.h"
#include "Logo.h"


CSceneMgr* CSceneMgr::m_pInstance = nullptr;
CSceneMgr::CSceneMgr()
	: m_pScene(nullptr), m_ePreScene(SCENE_END), m_eCurScene(SCENE_END)
{
}


CSceneMgr::~CSceneMgr()
{
	Release();
}

// 씬 업데이트
void CSceneMgr::Update()
{
	m_pScene->Update();
}

// 씬 레이트 업데이트
void CSceneMgr::Late_Update()
{
	m_pScene->Late_update();
}

// 씬 렌더
void CSceneMgr::Render(HDC _DC)
{
	m_pScene->Render(_DC);
}

// 씬 교체 요청
void CSceneMgr::Request_SceneChange(SCENEID _eScene)
{
	m_eCurScene = _eScene;
}

// 씬 교체
void CSceneMgr::SceneChange_Update()
{
	// 교체할 씬이 있으면
	if (m_ePreScene != m_eCurScene)
	{
		// 현재 씬 제거
		SAFE_DELETE(m_pScene);

		switch (m_eCurScene)
		{
			// 맨 처음 로고 타이틀
		case CSceneMgr::SCENE_LOGO:
			m_pScene = new CLogo;
			break;
			// 캐릭터 선택
		case CSceneMgr::SCENE_SELECT_MENU:
			m_pScene = new CSelectMenu;
			break;
			// 게임 스테이지
		case CSceneMgr::SCENE_STAGE:
			m_pScene = new CStage;
			break;
			// 스테이지 클리어 대화
		case CSceneMgr::SCENE_CG_STAGE_1:
			m_pScene = new CCG_Stage_1;
			break;
		}
		// 교체할 씬 초기화
		m_pScene->Initialize();
		// 교체할 씬을 현제 씬으로 저장
		m_ePreScene = m_eCurScene;
	}
}

void CSceneMgr::Release()
{
	SAFE_DELETE(m_pScene);
}

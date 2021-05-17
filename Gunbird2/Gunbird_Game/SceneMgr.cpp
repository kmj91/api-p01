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

void CSceneMgr::Update()
{
	m_pScene->Update();
}

void CSceneMgr::Late_Update()
{
	m_pScene->Late_update();
}

void CSceneMgr::Render(HDC _DC)
{
	m_pScene->Render(_DC);
}

void CSceneMgr::Request_SceneChange(SCENEID _eScene)
{
	m_eCurScene = _eScene;
}

void CSceneMgr::SceneChange_Update()
{
	if (m_ePreScene != m_eCurScene)
	{
		SAFE_DELETE(m_pScene);

		switch (m_eCurScene)
		{
		case CSceneMgr::SCENE_LOGO:
			m_pScene = new CLogo;
			break;
		case CSceneMgr::SCENE_STAGE:
			m_pScene = new CStage;
			break;
		case CSceneMgr::SCENE_SELECT_MENU:
			m_pScene = new CSelectMenu;
			break;
		case CSceneMgr::SCENE_CG_STAGE_1:
			m_pScene = new CCG_Stage_1;
			break;
		}
		m_pScene->Initialize();
		m_ePreScene = m_eCurScene;
	}
}

void CSceneMgr::Release()
{
	SAFE_DELETE(m_pScene);
}

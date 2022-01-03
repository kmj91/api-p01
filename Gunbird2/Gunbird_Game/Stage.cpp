// �����
// ���� �������� ��
// ���� �÷��̿� �ʿ��� ���� ���� �� Ű ó��

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
	// ���� ���� �� ��ȯ
	if (g_bGameOver) {
		g_bGameOver = false;
		g_bCountdownStop = false;
		CSoundMgr::Get_Instance()->StopAll();
		CSceneMgr::Get_Instance()->Request_SceneChange(CSceneMgr::SCENE_LOGO);
	}

	// ���� Ŭ���� �� ��ȯ
	if (g_bStageClear) {
		if (g_pPlayer_1->Get_Info().fY < -100.f) {
			// �� ü����
			CSceneMgr::Get_Instance()->Request_SceneChange(CSceneMgr::SCENE_CG_STAGE_1);
		}
	}

	// Ű �Է� ó��
	Key_Check();

	// ���� ��Ƽ�� ��
	if (m_isPause || g_bCountdownStop) {
		// UI�� ������Ʈ
		CObjMgr::Get_Instance()->UI_Update();
		return;
	}

	Create_Object();	// ������Ʈ �����

	CObjMgr::Get_Instance()->Update();
}

void CStage::Late_update()
{
	// ���� ��Ƽ�� ��
	if (m_isPause || g_bCountdownStop) {
		// UI�� ������Ʈ
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
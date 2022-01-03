// 기명준
// 씬 매니저
// 씬 관리

#pragma once

#ifndef __SCENEMGR_H__
#define __SCENEMGR_H__


class CScene;
class CSceneMgr
{
private:
	CSceneMgr();
	~CSceneMgr();

public:
	enum SCENEID { SCENE_LOGO, SCENE_SELECT_MENU, SCENE_STAGE, SCENE_CG_STAGE_1, SCENE_END };

public:
	void Update();									// 씬 업데이트
	void Late_Update();								// 씬 레이트 업데이트
	void Render(HDC _DC);							// 씬 렌더
	void Request_SceneChange(SCENEID _eScene);		// 씬 교체 요청
	void SceneChange_Update();						// 씬 교체
	void Release();

public:
	static CSceneMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSceneMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CSceneMgr*	m_pInstance;
	CScene*				m_pScene;			// 현재 씬 포인터
	SCENEID				m_ePreScene;		// 이전 씬 번호
	SCENEID				m_eCurScene;		// 교체할 씬 번호
};


#endif // !__SCENEMGR_H__

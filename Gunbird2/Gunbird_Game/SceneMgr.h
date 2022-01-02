// ±â¸íÁØ
// ¾À ¸Å´ÏÀú
// ¾À °ü¸®

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
	void Update();									// ¾À ¾÷µ¥ÀÌÆ®
	void Late_Update();								// ¾À ·¹ÀÌÆ® ¾÷µ¥ÀÌÆ®
	void Render(HDC _DC);							// ¾À ·»´õ
	void Request_SceneChange(SCENEID _eScene);		// ¾À ±³Ã¼ ¿äÃ»
	void SceneChange_Update();						// ¾À ±³Ã¼
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
	CScene*				m_pScene;
	SCENEID				m_ePreScene;
	SCENEID				m_eCurScene;
};


#endif // !__SCENEMGR_H__

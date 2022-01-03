// �����
// �� �Ŵ���
// �� ����

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
	void Update();									// �� ������Ʈ
	void Late_Update();								// �� ����Ʈ ������Ʈ
	void Render(HDC _DC);							// �� ����
	void Request_SceneChange(SCENEID _eScene);		// �� ��ü ��û
	void SceneChange_Update();						// �� ��ü
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
	CScene*				m_pScene;			// ���� �� ������
	SCENEID				m_ePreScene;		// ���� �� ��ȣ
	SCENEID				m_eCurScene;		// ��ü�� �� ��ȣ
};


#endif // !__SCENEMGR_H__

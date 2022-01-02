// 기명준
// 게임 로직 처리 및 매니저 관리

#pragma once

#ifndef __MAINGAME_H__
#define __MAINGAME_H__

class CMainGame
{
public:
	explicit CMainGame();
	~CMainGame();

	void Initialize();
	void Update();
	void Late_Update();
	void Render();
	void SceneChange_Update();
	void Release();

private:
	HDC				m_DC;
	int				m_iFPS;
	DWORD			m_dwTime;
	TCHAR			m_szFPS[16];
};

#endif // !__MAINGAME_H__



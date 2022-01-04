// 기명준
// 키 처리 매니저

#include "stdafx.h"
#include "KeyMgr.h"

CKeyMgr* CKeyMgr::m_pInstance = nullptr;

CKeyMgr::CKeyMgr()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}


CKeyMgr::~CKeyMgr()
{
}

// 키를 계속 누름
bool CKeyMgr::Key_Pressing(int _key)
{
	if (GetAsyncKeyState(_key) & 0x8000)
		return true;
	return false;
}

// 키 누름
bool CKeyMgr::Key_Down(int _key)
{
	if (!m_bKeyState[_key] && (GetAsyncKeyState(_key) & 0x8000))
	{
		m_bKeyState[_key] = !m_bKeyState[_key];
		return true;
	}
	return false;
}

// 키 뗌
bool CKeyMgr::Key_Up(int _key)
{
	if (m_bKeyState[_key] && !(GetAsyncKeyState(_key) & 0x8000))
	{
		m_bKeyState[_key] = !m_bKeyState[_key];
		return true;
	}
	return false;
}

// 키 초기화
// Key_Down과 Key_Up을 위한 bool값 초기화
void CKeyMgr::Key_Update()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		// 이전에 키를 눌러서 true지만 지금은 해당 키를 안 누름 false로 바꿈
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
		// 이전에 키를 안눌러서 false지만 지금은 해당 키를 누름 true로 바꿈
		if (!m_bKeyState[i] && (GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
	}
}

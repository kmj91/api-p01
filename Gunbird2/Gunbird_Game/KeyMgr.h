// 기명준
// 키 처리 매니저

#pragma once

#ifndef __KEYMGR_H__
#define __KEYMGR_H__

#define VK_MAX 0xff

class CKeyMgr
{
public:
	CKeyMgr();
	~CKeyMgr();

public:
	bool Key_Pressing(int _key);
	bool Key_Down(int _key);
	bool Key_Up(int _key);
	void Key_Update();

public:
	static CKeyMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CKeyMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CKeyMgr*		m_pInstance;
	bool				m_bKeyState[VK_MAX];
};


#endif // !__KEYMGR_H__

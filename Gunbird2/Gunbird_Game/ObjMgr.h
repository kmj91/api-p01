#pragma once

#ifndef __OBJMGR_H__
#define __OBJMGR_H__

class CObj;
class CObjMgr
{
private:
	CObjMgr();
	~CObjMgr();

public:
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

	// UI�� ������Ʈ
	void UI_Update();
	void UI_LateUpdate();

	// ����Ʈ�� ������Ʈ �߰�
	// _bOption : true�� emplace_back, false�� emplace_front
	void Add_Object(CObj* _pObj, OBJID::ID _eID, bool _bOption = true);
	void Delete_ID(OBJID::ID _eID);
	void Dead_ID(OBJID::ID _eID);
	CObj* Get_Target(CObj* _Dst, OBJID::ID _eID);
	CObj* Get_TargetMonster(CObj* _Dst);

	// ���� �Ѿ��� ĵ��� ��ü
	void Change_Candy();
	// ���� �Ѿ��� ����
	void Dead_MonsterBullet();
	// ���Ϳ��� ������
	void Demage_Monster(float _fDamage, int _iPlayerNum);
	// ��Ʈ �ȿ� ���Ϳ��� ������
	bool Demage_Monster(RECT* _stpRect, float _fDamage, int _iPlayerNum);

	static CObjMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CObjMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	list<CObj*>		m_listObj[OBJID::END];
	static CObjMgr*	m_pInstance;
};

#endif // !__OBJMGR_H__



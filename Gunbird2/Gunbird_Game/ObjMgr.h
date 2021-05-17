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

	// UI만 업데이트
	void UI_Update();
	void UI_LateUpdate();

	// 리스트에 오브젝트 추가
	// _bOption : true면 emplace_back, false면 emplace_front
	void Add_Object(CObj* _pObj, OBJID::ID _eID, bool _bOption = true);
	void Delete_ID(OBJID::ID _eID);
	void Dead_ID(OBJID::ID _eID);
	CObj* Get_Target(CObj* _Dst, OBJID::ID _eID);
	CObj* Get_TargetMonster(CObj* _Dst);

	// 몬스터 총알을 캔디로 교체
	void Change_Candy();
	// 몬스터 총알을 삭제
	void Dead_MonsterBullet();
	// 몬스터에게 데미지
	void Demage_Monster(float _fDamage, int _iPlayerNum);
	// 렉트 안에 몬스터에게 데미지
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



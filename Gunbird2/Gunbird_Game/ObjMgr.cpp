#include "stdafx.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "CollisionMgr.h"

#include "GameUIMgr.h"

#include "Bullet.h"
#include "Boss.h"
#include "Monster.h"


CObjMgr* CObjMgr::m_pInstance = nullptr;
CObjMgr::CObjMgr()
{
}


CObjMgr::~CObjMgr()
{
	Release();
}

void CObjMgr::Update()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		auto& iter = m_listObj[i].begin();
		auto& iter_end = m_listObj[i].end();
		while(iter != iter_end)
		{
			int iEvent = (*iter)->Update();
			if (OBJ_DEAD == iEvent)
			{
				SAFE_DELETE(*iter);
				iter = m_listObj[i].erase(iter);
			}
			else
				++iter;
		}
	}

	CCollisionMgr::Collision_Monster(m_listObj[OBJID::PLAYER_BULLET], m_listObj);
	CCollisionMgr::Collision_Player(m_listObj[OBJID::PLAYER], m_listObj);
	CCollisionMgr::Collision_Item(m_listObj[OBJID::PLAYER], m_listObj[OBJID::ITEM]);
	CCollisionMgr::Collision_Sector(m_listObj[OBJID::SECTOR], m_listObj[OBJID::NPC]);
	CCollisionMgr::Collision_RemoveSector(m_listObj[OBJID::REMOVE_SECTOR], m_listObj[OBJID::NPC]);
}

void CObjMgr::Late_Update()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		auto& iter = m_listObj[i].begin();
		auto& iter_end = m_listObj[i].end();
		while (iter != iter_end)
		{
			(*iter)->Late_Update();
			++iter;
		}
	}
}

void CObjMgr::Render(HDC _DC)
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		auto& iter = m_listObj[i].begin();
		auto& iter_end = m_listObj[i].end();
		while (iter != iter_end)
		{
			(*iter)->Render(_DC);
			++iter;
		}
	}
}

void CObjMgr::Release()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		for_each(m_listObj[i].begin(), m_listObj[i].end(), Safe_Delete<CObj*>);
		m_listObj[i].clear();
	}
}

// UI�� ������Ʈ
void CObjMgr::UI_Update()
{
	auto& iter = m_listObj[OBJID::UI].begin();
	auto& iter_end = m_listObj[OBJID::UI].end();
	while (iter != iter_end)
	{
		int iEvent = (*iter)->Update();
		if (OBJ_DEAD == iEvent)
		{
			SAFE_DELETE(*iter);
			iter = m_listObj[OBJID::UI].erase(iter);
		}
		else
			++iter;
	}
}

// UI�� ������Ʈ
void CObjMgr::UI_LateUpdate()
{
	auto& iter = m_listObj[OBJID::UI].begin();
	auto& iter_end = m_listObj[OBJID::UI].end();
	while (iter != iter_end)
	{
		(*iter)->Late_Update();
		++iter;
	}
}

void CObjMgr::Add_Object(CObj * _pObj, OBJID::ID _eID, bool _bOption)
{
	if (_bOption) 
	{
		m_listObj[_eID].emplace_back(_pObj);
	}
	else 
	{
		m_listObj[_eID].emplace_front(_pObj);
	}
}

void CObjMgr::Delete_ID(OBJID::ID _eID)
{
	for_each(m_listObj[_eID].begin(), m_listObj[_eID].end(), Safe_Delete<CObj*>);
	m_listObj[_eID].clear();
}

void CObjMgr::Dead_ID(OBJID::ID _eID)
{
	for (auto& pObj : m_listObj[_eID])
	{
		pObj->Set_Dead();
	}
}

CObj * CObjMgr::Get_Target(CObj * _Dst, OBJID::ID _eID)
{
	CObj*	pTarget = nullptr;
	float	fDis = 0.f;

	for (auto& pObj : m_listObj[_eID])
	{
		float	fX = pObj->Get_Info().fX - _Dst->Get_Info().fX;
		float	fY = pObj->Get_Info().fY - _Dst->Get_Info().fY;
		float	fDia = sqrtf(fX * fX + fY * fY);

		if (fDia < fDis || !fDis)
		{
			fDis = fDia;
			pTarget = pObj;
		}
	}

	return pTarget;
}

// ����� ����
CObj * CObjMgr::Get_TargetMonster(CObj * _Dst)
{
	CObj*	pTarget = nullptr;
	float	fDis = 0.f;

	for (auto& pObj : m_listObj[OBJID::AIR_MONSTER])
	{
		if (pObj->isDead() || pObj->isHpLock())
		{
			continue;
		}

		float	fX = pObj->Get_Info().fX - _Dst->Get_Info().fX;
		float	fY = pObj->Get_Info().fY - _Dst->Get_Info().fY;
		float	fDia = sqrtf(fX * fX + fY * fY);

		if (fDia < fDis || !fDis)
		{
			fDis = fDia;
			pTarget = pObj;
		}
	}

	for (auto& pObj : m_listObj[OBJID::GROUND_MONSTER])
	{
		if (pObj->isDead() || pObj->isHpLock())
		{
			continue;
		}

		float	fX = pObj->Get_Info().fX - _Dst->Get_Info().fX;
		float	fY = pObj->Get_Info().fY - _Dst->Get_Info().fY;
		float	fDia = sqrtf(fX * fX + fY * fY);

		if (fDia < fDis || !fDis)
		{
			fDis = fDia;
			pTarget = pObj;
		}
	}

	for (auto& pObj : m_listObj[OBJID::UPPER_MONSTER])
	{
		if (pObj->isDead() || pObj->isHpLock())
		{
			continue;
		}

		float	fX = pObj->Get_Info().fX - _Dst->Get_Info().fX;
		float	fY = pObj->Get_Info().fY - _Dst->Get_Info().fY;
		float	fDia = sqrtf(fX * fX + fY * fY);

		if (fDia < fDis || !fDis)
		{
			fDis = fDia;
			pTarget = pObj;
		}
	}

	for (auto& pObj : m_listObj[OBJID::GROUND_BOSS])
	{
		if (pObj->isDead() || pObj->isHpLock())
		{
			continue;
		}

		float	fX = pObj->Get_Info().fX - _Dst->Get_Info().fX;
		float	fY = pObj->Get_Info().fY - _Dst->Get_Info().fY;
		float	fDia = sqrtf(fX * fX + fY * fY);

		if (fDia < fDis || !fDis)
		{
			fDis = fDia;
			pTarget = pObj;
		}
	}

	return pTarget;
}

// ���� �Ѿ��� ĵ��� ��ü
void CObjMgr::Change_Candy()
{
	for (auto& pObj : m_listObj[OBJID::MONSTER_BULLET])
	{
		static_cast<CBullet*>(pObj)->Change_Candy();
	}
}

// ���� �Ѿ��� ����
void CObjMgr::Dead_MonsterBullet()
{
	for (auto& pObj : m_listObj[OBJID::MONSTER_BULLET])
	{
		pObj->Set_Dead();
	}
}

// ���� ���� �������� ������
void CObjMgr::Demage_Monster(float _fDamage, int _iPlayerNum)
{
	for (auto& pObj : m_listObj[OBJID::AIR_MONSTER])
	{
		if (pObj->isHpLock())
			continue;

		pObj->Hit(_fDamage);
		// ���Ͱ� �׾�����
		if (pObj->isDead()) {
			// ���� ȹ��
			CGameUIMgr::Get_Instance()->Add_Score(static_cast<CMonster*>(pObj)->Get_Score(), _iPlayerNum);
		}
	}

	for (auto& pObj : m_listObj[OBJID::UPPER_MONSTER])
	{
		if (pObj->isHpLock())
			continue;

		pObj->Hit(_fDamage);
		// ���Ͱ� �׾�����
		if (pObj->isDead()) {
			// ���� ȹ��
			CGameUIMgr::Get_Instance()->Add_Score(static_cast<CMonster*>(pObj)->Get_Score(), _iPlayerNum);
		}
	}

	for (auto& pObj : m_listObj[OBJID::GROUND_MONSTER])
	{
		if (pObj->isHpLock())
			continue;

		pObj->Hit(_fDamage);
		// ���Ͱ� �׾�����
		if (pObj->isDead()) {
			// ���� ȹ��
			CGameUIMgr::Get_Instance()->Add_Score(static_cast<CMonster*>(pObj)->Get_Score(), _iPlayerNum);
		}
	}

	for (auto& pObj : m_listObj[OBJID::GROUND_BOSS])
	{
		static_cast<CBoss*>(pObj)->All_Hit(_fDamage, _iPlayerNum);
	}

	for (auto& pObj : m_listObj[OBJID::MAP_OBJECT])
	{
		if (pObj->isHpLock())
			continue;

		pObj->Hit(_fDamage);
	}
}

// ��Ʈ �ȿ� ���Ϳ��� ������
bool CObjMgr::Demage_Monster(RECT * _stpRect, float _fDamage, int _iPlayerNum)
{
	RECT rc = {};
	bool bHit = false;

	for (auto& pObj : m_listObj[OBJID::AIR_MONSTER])
	{
		if (IntersectRect(&rc, _stpRect, &pObj->Get_HitRect())) {
			if (pObj->isHpLock())
				continue;

			bHit = true;

			pObj->Hit(_fDamage);
			// ���Ͱ� �׾�����
			if (pObj->isDead()) {
				// ���� ȹ��
				CGameUIMgr::Get_Instance()->Add_Score(static_cast<CMonster*>(pObj)->Get_Score(), _iPlayerNum);
			}
		}
	}

	for (auto& pObj : m_listObj[OBJID::UPPER_MONSTER])
	{
		if (IntersectRect(&rc, _stpRect, &pObj->Get_HitRect())) {
			if (pObj->isHpLock())
				continue;

			bHit = true;

			pObj->Hit(_fDamage);
			// ���Ͱ� �׾�����
			if (pObj->isDead()) {
				// ���� ȹ��
				CGameUIMgr::Get_Instance()->Add_Score(static_cast<CMonster*>(pObj)->Get_Score(), _iPlayerNum);
			}
		}
	}

	for (auto& pObj : m_listObj[OBJID::GROUND_MONSTER])
	{
		if (IntersectRect(&rc, _stpRect, &pObj->Get_HitRect())) {
			if (pObj->isHpLock())
				continue;

			bHit = true;

			pObj->Hit(_fDamage);
			// ���Ͱ� �׾�����
			if (pObj->isDead()) {
				// ���� ȹ��
				CGameUIMgr::Get_Instance()->Add_Score(static_cast<CMonster*>(pObj)->Get_Score(), _iPlayerNum);
			}
		}
	}

	for (auto& pObj : m_listObj[OBJID::GROUND_BOSS])
	{
		// ������ �������� ��Ʈ�� ������ �־ ���� �Լ��� ȣ���ؼ� �ǰ�ó���� �Ѵ�
		bHit = static_cast<CBoss*>(pObj)->Check_Hit(_stpRect, _fDamage, _iPlayerNum);
	}

	for (auto& pObj : m_listObj[OBJID::MAP_OBJECT])
	{
		if (IntersectRect(&rc, _stpRect, &pObj->Get_HitRect())) {
			if (pObj->isHpLock())
				continue;

			bHit = true;

			pObj->Hit(_fDamage);
		}
	}

	return bHit;
}

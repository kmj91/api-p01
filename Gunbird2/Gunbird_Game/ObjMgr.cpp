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

// UI만 업데이트
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

// UI만 업데이트
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

// 가까운 몬스터
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

// 몬스터 총알을 캔디로 교체
void CObjMgr::Change_Candy()
{
	for (auto& pObj : m_listObj[OBJID::MONSTER_BULLET])
	{
		static_cast<CBullet*>(pObj)->Change_Candy();
	}
}

// 몬스터 총알을 삭제
void CObjMgr::Dead_MonsterBullet()
{
	for (auto& pObj : m_listObj[OBJID::MONSTER_BULLET])
	{
		pObj->Set_Dead();
	}
}

// 몬스터 에게 지속적인 데미지
void CObjMgr::Demage_Monster(float _fDamage, int _iPlayerNum)
{
	for (auto& pObj : m_listObj[OBJID::AIR_MONSTER])
	{
		if (pObj->isHpLock())
			continue;

		pObj->Hit(_fDamage);
		// 몬스터가 죽었으면
		if (pObj->isDead()) {
			// 점수 획득
			CGameUIMgr::Get_Instance()->Add_Score(static_cast<CMonster*>(pObj)->Get_Score(), _iPlayerNum);
		}
	}

	for (auto& pObj : m_listObj[OBJID::UPPER_MONSTER])
	{
		if (pObj->isHpLock())
			continue;

		pObj->Hit(_fDamage);
		// 몬스터가 죽었으면
		if (pObj->isDead()) {
			// 점수 획득
			CGameUIMgr::Get_Instance()->Add_Score(static_cast<CMonster*>(pObj)->Get_Score(), _iPlayerNum);
		}
	}

	for (auto& pObj : m_listObj[OBJID::GROUND_MONSTER])
	{
		if (pObj->isHpLock())
			continue;

		pObj->Hit(_fDamage);
		// 몬스터가 죽었으면
		if (pObj->isDead()) {
			// 점수 획득
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

// 렉트 안에 몬스터에게 데미지
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
			// 몬스터가 죽었으면
			if (pObj->isDead()) {
				// 점수 획득
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
			// 몬스터가 죽었으면
			if (pObj->isDead()) {
				// 점수 획득
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
			// 몬스터가 죽었으면
			if (pObj->isDead()) {
				// 점수 획득
				CGameUIMgr::Get_Instance()->Add_Score(static_cast<CMonster*>(pObj)->Get_Score(), _iPlayerNum);
			}
		}
	}

	for (auto& pObj : m_listObj[OBJID::GROUND_BOSS])
	{
		// 보스는 여러개의 렉트를 가지고 있어서 따로 함수를 호출해서 피격처리를 한다
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

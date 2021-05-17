#include "stdafx.h"
#include "CollisionMgr.h"

#include "GameUIMgr.h"

#include "Obj.h"
#include "Player.h"
#include "Bullet.h"
#include "NPC.h"
#include "Sector.h"
#include "Item.h"
#include "Boss.h"
#include "Monster.h"


CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

// _Dst : 플레이어 총알
// _Src : 몬스터들
void CCollisionMgr::Collision_Monster(list<CObj*>& _Dst, list<CObj*>* _Src)
{
	RECT rc = {};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src[OBJID::AIR_MONSTER])
		{
			if (IntersectRect(&rc, &Dst->Get_HitRect(), &Src->Get_HitRect()))
			{
				if (Src->isHpLock())
					continue;
				Dst->Hit(Src->Get_Hp());
				Src->Hit(static_cast<CBullet*>(Dst)->Get_Damage());
				// 몬스터가 죽었으면
				if (Src->isDead()) {
					// 점수 획득
					CGameUIMgr::Get_Instance()->Add_Score(static_cast<CMonster*>(Src)->Get_Score(),
														static_cast<CBullet*>(Dst)->Get_PlayerNum());
				}

				goto END_FOR;	// 2중 for문 탈출
			}
		}

		for (auto& Src : _Src[OBJID::UPPER_MONSTER])
		{
			if (IntersectRect(&rc, &Dst->Get_HitRect(), &Src->Get_HitRect()))
			{
				if (Src->isHpLock())
					continue;
				Dst->Hit(Src->Get_Hp());
				Src->Hit(static_cast<CBullet*>(Dst)->Get_Damage());
				// 몬스터가 죽었으면
				if (Src->isDead()) {
					// 점수 획득
					CGameUIMgr::Get_Instance()->Add_Score(static_cast<CMonster*>(Src)->Get_Score(),
						static_cast<CBullet*>(Dst)->Get_PlayerNum());
				}

				goto END_FOR;	// 2중 for문 탈출
			}
		}

		for (auto& Src : _Src[OBJID::GROUND_MONSTER])
		{
			if (IntersectRect(&rc, &Dst->Get_HitRect(), &Src->Get_HitRect()))
			{
				if (Src->isHpLock())
					continue;
				Dst->Hit(Src->Get_Hp());
				Src->Hit(static_cast<CBullet*>(Dst)->Get_Damage());
				// 몬스터가 죽었으면
				if (Src->isDead()) {
					// 점수 획득
					CGameUIMgr::Get_Instance()->Add_Score(static_cast<CMonster*>(Src)->Get_Score(),
						static_cast<CBullet*>(Dst)->Get_PlayerNum());
				}

				goto END_FOR;	// 2중 for문 탈출
			}
		}

		// 지상 보스
		for (auto& Src : _Src[OBJID::GROUND_BOSS])
		{
			// 보스는 여러개의 렉트를 가지고 있어서 따로 함수를 호출해서 피격처리를 한다
			if (static_cast<CBoss*>(Src)->Check_Hit(&Dst->Get_HitRect(),
													static_cast<CBullet*>(Dst)->Get_Damage(),
													static_cast<CBullet*>(Dst)->Get_PlayerNum()))
			{
				Dst->Hit(Src->Get_Hp());
				// 몬스터가 죽었으면
				if (Src->isDead()) {
					// 점수 획득
					CGameUIMgr::Get_Instance()->Add_Score(static_cast<CMonster*>(Src)->Get_Score(),
						static_cast<CBullet*>(Dst)->Get_PlayerNum());
				}

				goto END_FOR;	// 2중 for문 탈출
			}
		}

		for (auto& Src : _Src[OBJID::MAP_OBJECT])
		{
			if (IntersectRect(&rc, &Dst->Get_HitRect(), &Src->Get_HitRect()))
			{
				if (Src->isHpLock())
					continue;
				Dst->Hit(Src->Get_Hp());
				Src->Hit(static_cast<CBullet*>(Dst)->Get_Damage());

				goto END_FOR;	// 2중 for문 탈출
			}
		}

		// 충돌하지 않음
		continue;

		// 충돌할 경우
	END_FOR:
		// 충전량 증가
		CGameUIMgr::Get_Instance()->Add_Charge(1, static_cast<CBullet*>(Dst)->Get_PlayerNum());
		continue;
	}
}

// _Dst : 플레이어
// _Src : 몬스터 및 총알
void CCollisionMgr::Collision_Player(list<CObj*>& _Dst, list<CObj*>* _Src)
{
	RECT rc = {};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src[OBJID::MONSTER_BULLET])
		{
			if (IntersectRect(&rc, &Dst->Get_HitRect(), &Src->Get_HitRect()))
			{
				if (Dst->isHpLock())	// 무적 상태임
					continue;
				Src->Hit(1.f);
				Dst->Hit(1.f);
				goto END_FOR;	// 2중 for문 탈출
			}
		}

		// 공중 몬스터와 부딪히면 파워 다운 시스템 있음...
		for (auto& Src : _Src[OBJID::AIR_MONSTER])
		{
			if (IntersectRect(&rc, &Dst->Get_HitRect(), &Src->Get_HitRect()))
			{
				static_cast<CPlayer*>(Dst)->PowerDown();
				goto END_FOR;	// 2중 for문 탈출
			}
		}
	END_FOR:
		continue;
	}
}

void CCollisionMgr::Collision_Rect(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				Dst->Set_Dead();
				Src->Set_Dead();
			}
		}
	}
}

void CCollisionMgr::Collision_RectEx(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	float fX = 0.f, fY = 0.f;
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Rect(Dst, Src, &fX, &fY))
			{
				if (fX > fY)
				{
					if (Dst->Get_Info().fY < Src->Get_Info().fY)
						Src->Set_PosY(fY);
					else
						Src->Set_PosY(-fY);
				}
				else
				{
					if (Dst->Get_Info().fX < Src->Get_Info().fX)
						Src->Set_PosX(fX);
					else
						Src->Set_PosX(-fX);
				}
			}
		}
	}
}

void CCollisionMgr::Collision_Sphere(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Sphere(Dst, Src))
			{
				Dst->Set_Dead();
				Src->Set_Dead();
			}
		}
	}
}

// _Dst : 플레이어
// _Src : 아이템
void CCollisionMgr::Collision_Item(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_HitRect(), &Src->Get_Rect()))
			{
				switch (static_cast<CItem*>(Src)->Get_ItemType())
				{
				case ITEM::POWER_UP:
					// 파워업
					static_cast<CPlayer*>(Dst)->PowerUp();
					Src->Set_Dead();
					break;
				case ITEM::BOMB:
					// 폭탄 개수 증가
					CGameUIMgr::Get_Instance()->Add_Bomb(1, static_cast<CPlayer*>(Dst)->Get_PlayerNum());
					Src->Set_Dead();
					break;
				case ITEM::SCORE:
					Src->Set_Dead();
					CGameUIMgr::Get_Instance()->Add_Score(static_cast<CItem*>(Src)->Get_Score(),
														static_cast<CPlayer*>(Dst)->Get_PlayerNum());
					break;
				}
			}
		}
	}
}

void CCollisionMgr::Collision_Sector(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Pos(Dst, Src))
			{
				static_cast<CNPC*>(Src)->Move_DestPos(static_cast<CSector*>(Dst)->Get_DestPosX(), static_cast<CSector*>(Dst)->Get_DestPosY());
			}
		}
	}
}

void CCollisionMgr::Collision_RemoveSector(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Pos(Dst, Src))
			{
				Src->Set_Dead();
			}
		}
	}
}

bool CCollisionMgr::Check_Sphere(CObj* _Dst, CObj* _Src)
{
	float fX = abs(_Dst->Get_Info().fX - _Src->Get_Info().fX);
	float fY = _Dst->Get_Info().fY - _Src->Get_Info().fY;
	float fDia = sqrtf(fX * fX + fY * fY);

	float fRad = (float)((_Dst->Get_Info().iCX + _Src->Get_Info().iCX) >> 1);

	if (fRad > fDia)
		return true;
	return false;
}

bool CCollisionMgr::Check_Rect(CObj* _Dst, CObj* _Src, float* _x, float* _y)
{
	float fDisX = abs(_Dst->Get_Info().fX - _Src->Get_Info().fX);
	float fDisY = abs(_Dst->Get_Info().fY - _Src->Get_Info().fY);

	float fCX = (float)((_Dst->Get_Info().iCX + _Src->Get_Info().iCX) >> 1);
	float fCY = (float)((_Dst->Get_Info().iCY + _Src->Get_Info().iCY) >> 1);

	if (fDisX <= fCX && fDisY <= fCY)
	{
		*_x = fCX - fDisX;
		*_y = fCY - fDisY;
		return true;
	}
	return false;
}

bool CCollisionMgr::Check_Pos(CObj * _Dst, CObj * _Src)
{
	RECT rect = _Dst->Get_Rect();
	float fX = _Src->Get_Info().fX;
	float fY = _Src->Get_Info().fY;

	if (rect.left <= fX && rect.top <= fY
		&& rect.right >= fX && rect.bottom >= fY)
	{
		return true;
	}

	return false;
}

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

// _Dst : �÷��̾� �Ѿ�
// _Src : ���͵�
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
				// ���Ͱ� �׾�����
				if (Src->isDead()) {
					// ���� ȹ��
					CGameUIMgr::Get_Instance()->Add_Score(static_cast<CMonster*>(Src)->Get_Score(),
														static_cast<CBullet*>(Dst)->Get_PlayerNum());
				}

				goto END_FOR;	// 2�� for�� Ż��
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
				// ���Ͱ� �׾�����
				if (Src->isDead()) {
					// ���� ȹ��
					CGameUIMgr::Get_Instance()->Add_Score(static_cast<CMonster*>(Src)->Get_Score(),
						static_cast<CBullet*>(Dst)->Get_PlayerNum());
				}

				goto END_FOR;	// 2�� for�� Ż��
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
				// ���Ͱ� �׾�����
				if (Src->isDead()) {
					// ���� ȹ��
					CGameUIMgr::Get_Instance()->Add_Score(static_cast<CMonster*>(Src)->Get_Score(),
						static_cast<CBullet*>(Dst)->Get_PlayerNum());
				}

				goto END_FOR;	// 2�� for�� Ż��
			}
		}

		// ���� ����
		for (auto& Src : _Src[OBJID::GROUND_BOSS])
		{
			// ������ �������� ��Ʈ�� ������ �־ ���� �Լ��� ȣ���ؼ� �ǰ�ó���� �Ѵ�
			if (static_cast<CBoss*>(Src)->Check_Hit(&Dst->Get_HitRect(),
													static_cast<CBullet*>(Dst)->Get_Damage(),
													static_cast<CBullet*>(Dst)->Get_PlayerNum()))
			{
				Dst->Hit(Src->Get_Hp());
				// ���Ͱ� �׾�����
				if (Src->isDead()) {
					// ���� ȹ��
					CGameUIMgr::Get_Instance()->Add_Score(static_cast<CMonster*>(Src)->Get_Score(),
						static_cast<CBullet*>(Dst)->Get_PlayerNum());
				}

				goto END_FOR;	// 2�� for�� Ż��
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

				goto END_FOR;	// 2�� for�� Ż��
			}
		}

		// �浹���� ����
		continue;

		// �浹�� ���
	END_FOR:
		// ������ ����
		CGameUIMgr::Get_Instance()->Add_Charge(1, static_cast<CBullet*>(Dst)->Get_PlayerNum());
		continue;
	}
}

// _Dst : �÷��̾�
// _Src : ���� �� �Ѿ�
void CCollisionMgr::Collision_Player(list<CObj*>& _Dst, list<CObj*>* _Src)
{
	RECT rc = {};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src[OBJID::MONSTER_BULLET])
		{
			if (IntersectRect(&rc, &Dst->Get_HitRect(), &Src->Get_HitRect()))
			{
				if (Dst->isHpLock())	// ���� ������
					continue;
				Src->Hit(1.f);
				Dst->Hit(1.f);
				goto END_FOR;	// 2�� for�� Ż��
			}
		}

		// ���� ���Ϳ� �ε����� �Ŀ� �ٿ� �ý��� ����...
		for (auto& Src : _Src[OBJID::AIR_MONSTER])
		{
			if (IntersectRect(&rc, &Dst->Get_HitRect(), &Src->Get_HitRect()))
			{
				static_cast<CPlayer*>(Dst)->PowerDown();
				goto END_FOR;	// 2�� for�� Ż��
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

// _Dst : �÷��̾�
// _Src : ������
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
					// �Ŀ���
					static_cast<CPlayer*>(Dst)->PowerUp();
					Src->Set_Dead();
					break;
				case ITEM::BOMB:
					// ��ź ���� ����
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

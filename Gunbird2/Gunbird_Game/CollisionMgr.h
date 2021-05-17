#pragma once

#ifndef __COLLISIONMGR_H__
#define __COLLISIONMGR_H__

class CObj;
class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	static void Collision_Monster(list<CObj*>& _Dst, list<CObj*>* _Src);
	static void Collision_Player(list<CObj*>& _Dst, list<CObj*>* _Src);
	static void Collision_Rect(list<CObj*>& _Dst, list<CObj*>& _Src);
	static void Collision_RectEx(list<CObj*>& _Dst, list<CObj*>& _Src);
	static void Collision_Sphere(list<CObj*>& _Dst, list<CObj*>& _Src);
	static void Collision_Item(list<CObj*>& _Dst, list<CObj*>& _Src);				// 아이템 충돌
	static void Collision_Sector(list<CObj*>& _Dst, list<CObj*>& _Src);				// NPC 이동
	static void Collision_RemoveSector(list<CObj*>& _Dst, list<CObj*>& _Src);		// NPC 삭제
	

	static bool Check_Sphere(CObj* _Dst, CObj* _Src);
	static bool Check_Rect(CObj* _Dst, CObj* _Src, float* _x, float* _y);
	static bool Check_Pos(CObj* _Dst, CObj* _Src);

};


#endif // !__COLLISIONMGR_H__


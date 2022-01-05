// 기명준
// 충돌 처리 매니저
// 게임 오브젝트간에 상호 충돌 처리

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
	static void Collision_Monster(list<CObj*>& _Dst, list<CObj*>* _Src);			// 플레이어 공격  -> 몬스터 충돌
	static void Collision_Player(list<CObj*>& _Dst, list<CObj*>* _Src);				// 몬스터 공격 -> 플레이어 충돌
	static void Collision_Rect(list<CObj*>& _Dst, list<CObj*>& _Src);				// RECT 충돌
	static void Collision_RectEx(list<CObj*>& _Dst, list<CObj*>& _Src);				// RECT 충돌 + 좌표 보정
	static void Collision_Sphere(list<CObj*>& _Dst, list<CObj*>& _Src);				// 구체 충돌
	static void Collision_Item(list<CObj*>& _Dst, list<CObj*>& _Src);				// 아이템 충돌
	static void Collision_Sector(list<CObj*>& _Dst, list<CObj*>& _Src);				// NPC 이동
	static void Collision_RemoveSector(list<CObj*>& _Dst, list<CObj*>& _Src);		// NPC 삭제
	
private:
	static bool Check_Sphere(CObj* _Dst, CObj* _Src);								// 구체 충돌
	static bool Check_Rect(CObj* _Dst, CObj* _Src, float* _x, float* _y);			// RECT 충돌
	static bool Check_Pos(CObj* _Dst, CObj* _Src);									// 두 오브젝트 충돌 (RECT)

};


#endif // !__COLLISIONMGR_H__


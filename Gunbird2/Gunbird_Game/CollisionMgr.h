// �����
// �浹 ó�� �Ŵ���
// ���� ������Ʈ���� ��ȣ �浹 ó��

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
	static void Collision_Monster(list<CObj*>& _Dst, list<CObj*>* _Src);			// �÷��̾� ����  -> ���� �浹
	static void Collision_Player(list<CObj*>& _Dst, list<CObj*>* _Src);				// ���� ���� -> �÷��̾� �浹
	static void Collision_Rect(list<CObj*>& _Dst, list<CObj*>& _Src);				// RECT �浹
	static void Collision_RectEx(list<CObj*>& _Dst, list<CObj*>& _Src);				// RECT �浹 + ��ǥ ����
	static void Collision_Sphere(list<CObj*>& _Dst, list<CObj*>& _Src);				// ��ü �浹
	static void Collision_Item(list<CObj*>& _Dst, list<CObj*>& _Src);				// ������ �浹
	static void Collision_Sector(list<CObj*>& _Dst, list<CObj*>& _Src);				// NPC �̵�
	static void Collision_RemoveSector(list<CObj*>& _Dst, list<CObj*>& _Src);		// NPC ����
	
private:
	static bool Check_Sphere(CObj* _Dst, CObj* _Src);								// ��ü �浹
	static bool Check_Rect(CObj* _Dst, CObj* _Src, float* _x, float* _y);			// RECT �浹
	static bool Check_Pos(CObj* _Dst, CObj* _Src);									// �� ������Ʈ �浹 (RECT)

};


#endif // !__COLLISIONMGR_H__


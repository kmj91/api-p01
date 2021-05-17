#include "stdafx.h"
#include "Bullet.h"

#include "ObjMgr.h"

#include "Candy.h"


CBullet::CBullet() : m_dwBulletTime(GetTickCount()), m_dwBulletDelay(0), m_fDamage(1.f), m_iPlayerNum(0), m_bOnBullet(true)
{
}


CBullet::~CBullet()
{
	Release();
}

void CBullet::Initialize()
{
}

int CBullet::Update()
{
	return OBJ_NOEVENT;
}

void CBullet::Late_Update()
{
}

void CBullet::Render(HDC _DC)
{
}

void CBullet::Release()
{
}

void CBullet::Change_Candy()
{
	m_bRemove = true;

	CObj* pObj = CAbstractFactory<CCandy>::Create(m_tInfo.fX, m_tInfo.fY);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::ITEM);
}

// 기명준
// 총알 인터페이스
// 총알 데미지와 지연 생성을 위한 시간 값과 딜레이를 가지고 있음

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

// 총알을 삭제하고 사탕 생성
void CBullet::Change_Candy()
{
	// 삭제
	m_bRemove = true;
	// 사탕 생성
	CObj* pObj = CObjFactory<CCandy>::Create(m_tInfo.fX, m_tInfo.fY);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::ITEM);
}

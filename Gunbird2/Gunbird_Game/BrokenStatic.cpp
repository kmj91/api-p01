#include "stdafx.h"
#include "BrokenStatic.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "Effect.h"
#include "Explosion_04.h"
#include "Explosion_02_1.h"
#include "Explosion_01.h"


CBrokenStatic::CBrokenStatic()
{
}


CBrokenStatic::~CBrokenStatic()
{
}

void CBrokenStatic::Initialize()
{
	Release();
}

int CBrokenStatic::Update()
{
	if (m_bRemove)
		return OBJ_DEAD;

	if (m_bDead)
	{
		float fX = m_tInfo.fX + (m_tInfo.iCX / 2);
		float fY = m_tInfo.fY + (m_tInfo.iCY / 2);

		CObj* pObj = CAbstractFactory<CExplosion_04>::Create(fX, fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		int irand = rand() % 3 + 1;
		pObj = CAbstractFactory<CExplosion_02_1>::Create(fX - 80.f, fY);
		static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(30 + irand * 50));
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		pObj = CAbstractFactory<CExplosion_02_1>::Create(fX + 80.f, fY);
		static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)50);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		pObj = CAbstractFactory<CExplosion_01>::Create(fX + irand * 30, fY + 20.f);
		static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(irand * 10));
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		pObj = CAbstractFactory<CExplosion_01>::Create(fX - irand * 30, fY + 50.f);
		static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)100);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		return OBJ_DEAD;
	}
		

	m_tInfo.fY += g_fBackgroundSpeed * 3.f;			// 배경에 맞춰서 Y축 이동

	Update_RectEx();

	return OBJ_NOEVENT;
}

void CBrokenStatic::Late_Update()
{
	if (m_bHpLock)
		if (0 <= m_tRect.bottom)
			m_bHpLock = false;

	if (WINCY <= m_tRect.top)
		m_bRemove = true;
}

void CBrokenStatic::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(_DC
		, (int)m_tInfo.fX, (int)m_tInfo.fY			// 중점좌표없이 left top을 x y 로 봄
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));

	// 충돌 박스
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
}

void CBrokenStatic::Release()
{
}

void CBrokenStatic::Update_RectEx()
{
	m_tRect.left = (LONG)m_tInfo.fX;
	m_tRect.top = (LONG)m_tInfo.fY;
	m_tRect.right = (LONG)(m_tInfo.fX + m_tInfo.iCX);
	m_tRect.bottom = (LONG)(m_tInfo.fY + m_tInfo.iCY);

	// 충돌 렉트
	m_tHitRect.left = (LONG)m_tInfo.fX + m_tHitRectPos.left;
	m_tHitRect.top = (LONG)m_tInfo.fY + m_tHitRectPos.top;
	m_tHitRect.right = (LONG)m_tInfo.fX + m_tHitRectPos.right;
	m_tHitRect.bottom = (LONG)m_tInfo.fY + m_tHitRectPos.bottom;
}

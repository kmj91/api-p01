// 기명준
// 파괴되는 고정 오브젝트
// 배경의 일부이거나 높이를 표현하기 위해 덮어씌우는 오브젝트
// 파괴되기 때문에 CStaticObj 구분됨

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

// 업데이트
int CBrokenStatic::Update()
{
	// 삭제
	if (m_bRemove)
		return OBJ_DEAD;
	// 죽음
	if (m_bDead)
	{
		float fX = m_tInfo.fX + (m_tInfo.iCX / 2);
		float fY = m_tInfo.fY + (m_tInfo.iCY / 2);

		CObj* pObj = CObjFactory<CExplosion_04>::Create(fX, fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		int irand = rand() % 3 + 1;
		pObj = CObjFactory<CExplosion_02_1>::Create(fX - 80.f, fY);
		static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(30 + irand * 50));
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		pObj = CObjFactory<CExplosion_02_1>::Create(fX + 80.f, fY);
		static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)50);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		pObj = CObjFactory<CExplosion_01>::Create(fX + irand * 30, fY + 20.f);
		static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(irand * 10));
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		pObj = CObjFactory<CExplosion_01>::Create(fX - irand * 30, fY + 50.f);
		static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)100);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		return OBJ_DEAD;
	}
		
	// 배경에 맞춰서 Y축 이동
	m_tInfo.fY += g_fBackgroundSpeed * 3.f;
	// 이미지 RECT 정보 및 Hit RECT 정보 갱신
	Update_RectEx();

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CBrokenStatic::Late_Update()
{
	if (m_bHpLock)
		if (0 <= m_tRect.bottom)
			m_bHpLock = false;

	if (WINCY <= m_tRect.top)
		m_bRemove = true;
}

// 렌더
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

	// 만약 옵션에서 충돌 박스 보기를 켰다면 (넘버패드 1번 키)
	// 충돌 박스도 렌더 해줘야함
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
}

void CBrokenStatic::Release()
{
}

// 이미지 RECT 정보 및 Hit RECT 정보 갱신
// 이미지를 배치할 때 이미지 중점 좌표로 배치를 안하고 왼쪽 상단 꼭지점으로 배치함
// 그래서 기존의 Update_Rect() 함수를 사용하지 않음
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

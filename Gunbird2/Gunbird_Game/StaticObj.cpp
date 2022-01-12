// 기명준
// 고정 오브젝트
// 배경의 일부이거나 높이를 표현하기 위해 덮어씌우는 오브젝트

#include "stdafx.h"
#include "StaticObj.h"

#include "BmpMgr.h"


CStaticObj::CStaticObj()
{
}


CStaticObj::~CStaticObj()
{
	Release();
}

void CStaticObj::Initialize()
{
}

// 업데이트
int CStaticObj::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	// 배경에 맞춰서 Y축 이동
	if (!g_bStopMapScroll)
		m_tInfo.fY += g_fBackgroundSpeed * 3.f;

	// 이미지 RECT 정보 갱신
	// 배경의 일부이기 때문에 Hit RECT는 갱신 불필요
	Update_RectEx();

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CStaticObj::Late_Update()
{
	if (WINCY <= m_tRect.top)
		m_bDead = true;
}

// 렌더
void CStaticObj::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(_DC
		, (int)m_tInfo.fX, (int)m_tInfo.fY			// 중점좌표없이 left top을 x y 로 봄
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CStaticObj::Release()
{
}

// 이미지 RECT 정보 갱신
// 배경의 일부이기 때문에 Hit RECT는 갱신 불필요
// 이미지를 배치할 때 이미지 중점 좌표로 배치를 안하고 왼쪽 상단 꼭지점으로 배치함
// 그래서 기존의 Update_Rect() 함수를 사용하지 않음
void CStaticObj::Update_RectEx()
{
	m_tRect.left = (LONG)m_tInfo.fX;
	m_tRect.top = (LONG)m_tInfo.fY;
	m_tRect.right = (LONG)(m_tInfo.fX + m_tInfo.iCX);
	m_tRect.bottom = (LONG)(m_tInfo.fY + m_tInfo.iCY);
}

// 기명준
// 이동하는 고정 오브젝트
// 배경의 일부이거나 높이를 표현하기 위해 덮어씌우는 오브젝트

#include "stdafx.h"
#include "MoveStatic.h"

#include "BmpMgr.h"


CMoveStatic::CMoveStatic()
{
}


CMoveStatic::~CMoveStatic()
{
	Release();
}

void CMoveStatic::Initialize()
{
}

// 업데이트
int CMoveStatic::Update()
{
	// 삭제
	if (m_bDead)
		return OBJ_DEAD;

	// 배경에 맞춰서 Y축 이동
	m_tInfo.fY += g_fBackgroundSpeed * 3.f;
	// 배경 스크롤 충분히 진행해서 목표 Y축보다 크면
	if (m_tInfo.fY > m_fDestPosY)
	{
		// 목적지도 배경 속도에 맞춰서 Y축 이동해야됨
		m_fDestPosY += g_fBackgroundSpeed * 3.f;

		float	fX = m_fDestPosX - m_tInfo.fX;		// 방향 좌표 X
		float	fY = m_fDestPosY - m_tInfo.fY;		// 방향 좌표 Y
		float	fDia = sqrtf(fX * fX + fY * fY);	// 현재 위치에서 목적지까지 거리
		float fSpeed = m_fSpeed;					// 이동 속도

		// 목적지까지의 거리가 이동 속도보다 작다면
		// 목표 거리만큼 이동할 수 있게 속도 값을 바꿈
		if (fDia < fSpeed)
			fSpeed = fDia;

		// 목적지에 도착했는지?
		if (fDia - fSpeed != 0) {
			// 좌표 이동
			m_tInfo.fX += fX / fDia * fSpeed;
			m_tInfo.fY += fY / fDia * fSpeed;
		}
	}

	// 이미지 RECT 정보 갱신
	Update_RectEx();

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CMoveStatic::Late_Update()
{
	if (WINCY <= m_tRect.top || WINCX <= m_tRect.left)
		m_bDead = true;
}

// 렌더
void CMoveStatic::Render(HDC _DC)
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

void CMoveStatic::Release()
{
}

// 이미지 RECT 정보 갱신
// 이미지를 배치할 때 이미지 중점 좌표로 배치를 안하고 왼쪽 상단 꼭지점으로 배치함
// 그래서 기존의 Update_Rect() 함수를 사용하지 않음
void CMoveStatic::Update_RectEx()
{
	m_tRect.left = (LONG)m_tInfo.fX;
	m_tRect.top = (LONG)m_tInfo.fY;
	m_tRect.right = (LONG)(m_tInfo.fX + m_tInfo.iCX);
	m_tRect.bottom = (LONG)(m_tInfo.fY + m_tInfo.iCY);
}

// 기명준
// 충전 이펙트

#include "stdafx.h"
#include "Effect_Charge.h"

#include "BmpMgr.h"


CEffect_Charge::CEffect_Charge()
{
}


CEffect_Charge::~CEffect_Charge()
{
	Release();
}

void CEffect_Charge::Initialize()
{
	// 이미지 크기 초기화
	m_tInfo.iCX = EFFECT_CHARGE_WIDTH * 3;
	m_tInfo.iCY = EFFECT_CHARGE_HEIGHT * 3;
	m_iImageWidth = EFFECT_CHARGE_WIDTH;
	m_iImageHeight = EFFECT_CHARGE_HEIGHT;
	// 이미지 프레임 초기화
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 32;
	m_tFrame.iFrameScene = rand() % 2;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 30;
}

// 업데이트
int CEffect_Charge::Update()
{
	// 삭제
	if (m_bDead)
		return OBJ_DEAD;

	// 이펙트 딜레이 시간이 지나면 플래그 true
	if (!m_bOnEffect && m_dwEffectTime + m_dwEffectDelay > GetTickCount())
		return OBJ_NOEVENT;
	else
		m_bOnEffect = true;

	// 상대 좌표로 위치를 맞춤
	m_tInfo.fX = m_pTarget->Get_Info().fX + m_fRelativePosX;
	m_tInfo.fY = m_pTarget->Get_Info().fY + m_fRelativePosY;

	// 이미지 RECT 정보 갱신
	Update_Rect();
	// 이미지 프레임 이동
	Frame_Move();
	// 프레임이 끝이면 삭제
	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		m_bDead = true;

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CEffect_Charge::Late_Update()
{
}

// 렌더
void CEffect_Charge::Render(HDC _DC)
{
	if (!m_bOnEffect)
		return;

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Effect_Charge");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CEffect_Charge::Release()
{
}

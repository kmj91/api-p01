// 기명준
// 마리온 충전 공격 명중 이펙트

#include "stdafx.h"
#include "MarionChargeHitEffect.h"

#include "BmpMgr.h"


CMarionChargeHitEffect::CMarionChargeHitEffect()
{
}


CMarionChargeHitEffect::~CMarionChargeHitEffect()
{
	Release();
}

void CMarionChargeHitEffect::Initialize()
{
	// 이미지 크기 초기화
	m_tInfo.iCX = MARION_CHARGE_BULLET_HIT_WIDTH * 3;
	m_tInfo.iCY = MARION_CHARGE_BULLET_HIT_HEIGHT * 3;
	m_iImageWidth = MARION_CHARGE_BULLET_HIT_WIDTH;
	m_iImageHeight = MARION_CHARGE_BULLET_HIT_HEIGHT;
	// 이미지 프레임 초기화
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 15;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 20;
}

// 업데이트
int CMarionChargeHitEffect::Update()
{
	// 삭제
	if (m_bDead)
		return OBJ_DEAD;
	// 이미지 프레임 이동
	Frame_Move();
	// 프레임이 끝이면 삭제
	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		m_bDead = true;

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CMarionChargeHitEffect::Late_Update()
{
	// 이 오브젝트가 Update에서 생성되서 Update를 건너 뛰고 Late_Update를 하는 경우가 발생함
	// 그래서 Late_Udpate에서 갱신함
	// 이미지 RECT 정보 갱신
	Update_Rect();
}

// 렌더
void CMarionChargeHitEffect::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Marion_Charge_Bullet_Hit");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CMarionChargeHitEffect::Release()
{
}

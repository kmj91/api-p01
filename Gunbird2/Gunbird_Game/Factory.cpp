// 기명준
// 공장
// 파괴되면 폭탄 아이템 드랍

#include "stdafx.h"
#include "Factory.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "Explosion_04.h"

#include "Bomb.h"


CFactory::CFactory()
	:m_ePreState(CFactory::END), m_eCurState(CFactory::END), m_bRemove(false)
{
}


CFactory::~CFactory()
{
	Release();
}

void CFactory::Initialize()
{
	// 기본 위치 및 설정
	m_tInfo.iCX = FACTORY_WIDTH * 3;
	m_tInfo.iCY = FACTORY_HEIGHT * 3;
	m_tHitRectPos = { 19 * 3, 20 * 3, 89 * 3, 95 * 3 };
	m_iImageWidth = FACTORY_WIDTH;
	m_iImageHeight = FACTORY_HEIGHT;
	// 체력
	m_fHp = 500.f;
	// 기본 상태
	m_eCurState = CFactory::IDLE;
	// 무적 해제
	m_bHpLock = false;

	// 점수
	m_iScore = 1000;
}

// 업데이트
int CFactory::Update()
{
	// 삭제
	if (m_bRemove)
		return OBJ_DEAD;
	// 죽음
	if (m_bDead)
	{
		if (m_ePreState != CFactory::DESTROY)
		{
			// 파괴 상태로 변경
			m_eCurState = CFactory::DESTROY;
			// 파괴되서 HP 다시 락
			m_bHpLock = true;
			// 파괴 이펙트 생성
			CObj* pObj = CAbstractFactory<CExplosion_04>::Create(m_tInfo.fX, m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			// 폭탄 아이템 드랍
			pObj = CAbstractFactory<CBomb>::Create(m_tInfo.fX, m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::ITEM);

			// 충돌 렉트
			m_tHitRectPos = { 0,0,0,0 };
		}
	}

	// 배경에 맞춰서 Y축 이동
	m_tInfo.fY += g_fBackgroundSpeed * 3.f;

	// 대기 표지판에 숨은 상태
	if (m_ePreState == CFactory::IDLE)
	{
		// 목표 Y축 좌표에 도착하면
		if (m_tInfo.fY > 260.f)
		{
			// 상태 변경
			m_eCurState = CFactory::OPEN;
			// 숨겨주던 오브젝트 파괴
			CObjMgr::Get_Instance()->Dead_ID(OBJID::MAP_OBJECT);
		}
	}
	// 오픈
	else if (m_ePreState == CFactory::OPEN)
	{
		if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		{
			m_eCurState = CFactory::ATTACK;
		}
	}
	// 공격
	else if (m_ePreState == CFactory::ATTACK)
	{
		if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		{
			m_eCurState = CFactory::CLOSED;
		}
	}
	// 닫음
	else if (m_ePreState == CFactory::CLOSED)
	{
		if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		{
			m_eCurState = CFactory::IDEL_END;
		}
	}

	// 이미지 RECT 정보 및 Hit RECT 정보 갱신
	Update_Rect();
	// 프레임 씬 변경 처리
	Scene_Change();
	// 이미지 프레임 이동
	Frame_Move();

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CFactory::Late_Update()
{
	// 맵 하단 밖으로 나가면 삭제
	if (WINCY <= m_tRect.top)
		m_bRemove = true;
}

// 렌더
void CFactory::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Factory");
	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
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

void CFactory::Release()
{
}

// 프레임 씬 변경 처리
void CFactory::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
			// 대기 표지판에 숨은 상태
		case CFactory::IDLE:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 999999;
			break;
			// 오픈
		case CFactory::OPEN:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
			// 공격
		case CFactory::ATTACK:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
			// 닫음
		case CFactory::CLOSED:
			m_tFrame.iFrameCnt = 9;
			m_tFrame.iFrameStart = 9;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
		case CFactory::DESTROY:
			m_tFrame.iFrameCnt = 8;
			m_tFrame.iFrameStart = 8;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 999999;
			break;
		case CFactory::IDEL_END:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 999999;
			break;
		}
		m_ePreState = m_eCurState;
	}
}

// 기명준
// 몬스터 파괴 이펙트
// 딜레이 값에 따라 지연되서 터지거나 함

#include "stdafx.h"
#include "Explosion_02_1.h"

#include  "BmpMgr.h"
#include "SoundMgr.h"


CExplosion_02_1::CExplosion_02_1()
{
}


CExplosion_02_1::~CExplosion_02_1()
{
	Release();
}

void CExplosion_02_1::Initialize()
{
	// 이미지 크기 초기화
	m_tInfo.iCX = EXPLOSION_02_1_WIDTH * 3;
	m_tInfo.iCY = EXPLOSION_02_1_HEIGHT * 3;
	m_iImageWidth = EXPLOSION_02_1_WIDTH;
	m_iImageHeight = EXPLOSION_02_1_HEIGHT;
	// 이미지 프레임 초기화
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 20;
	m_tFrame.iFrameScene = rand() % 2;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 30;
}

// 업데이트
int CExplosion_02_1::Update()
{
	// 삭제
	if (m_bDead)
		return OBJ_DEAD;
	// 이펙트 딜레이 시간이 지나면 플래그 true
	if (!m_bOnEffect && m_dwEffectTime + m_dwEffectDelay > GetTickCount())
		return OBJ_NOEVENT;
	else {
		// 플래그 true
		m_bOnEffect = true;
		// 사운드 재생
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EXPLOSION_2);
		CSoundMgr::Get_Instance()->PlaySound(L"Explosion_02.wav", CSoundMgr::EXPLOSION_2);
	}

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
void CExplosion_02_1::Late_Update()
{
}

// 렌더
void CExplosion_02_1::Render(HDC _DC)
{
	if (!m_bOnEffect)
		return;

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Explosion_02_1");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CExplosion_02_1::Release()
{
}

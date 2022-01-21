// 기명준
// 게임 스테이지 배경 스크롤
// 보스전이 Y축 좌표 몇 부터 시작하는지에 대해 처리
// 보스전부터 일정 길이의 배경 스크롤이 반복 처리

#include "stdafx.h"
#include "BG_Stage_1.h"

#include "BmpMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

#include "Stage1_BossTextBox.h"


CBG_Stage_1::CBG_Stage_1() : m_bBossRoom(false)
{
}


CBG_Stage_1::~CBG_Stage_1()
{
	Release();
}

void CBG_Stage_1::Initialize()
{
	m_tInfo.fX = 0.f;
	m_tInfo.fY = BACKGROUND_TOTAL_HEIGHT - BACKGROUND_HEIGHT;
	//m_tInfo.fY = 1800.f;	// 스테이지 보스전 직전 스크롤 좌표 값
	m_tInfo.iCX = BACKGROUND_WIDTH * 3;
	m_tInfo.iCY = BACKGROUND_HEIGHT * 3;
	m_iImageWidth = BACKGROUND_WIDTH;
	m_iImageHeight = BACKGROUND_HEIGHT;

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
	CSoundMgr::Get_Instance()->PlayBGM(L"JustFly.wav");
}

int CBG_Stage_1::Update()
{
	if(!g_bStopMapScroll)
		m_tInfo.fY = m_tInfo.fY - g_fBackgroundSpeed;

	if (!m_bBossRoom)
	{
		if (BACKGROUND_TOTAL_HEIGHT - m_tInfo.fY >= 1865.f)		// 보스전
		{
			m_bBossRoom = true;
			g_bStopMapScroll = true;

			CObj* pObj = CObjFactory<CStage1_BossTextBox>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::UI);

			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
			CSoundMgr::Get_Instance()->PlayBGM(L"Boss.wav");
		}
	}

	if (m_tInfo.fY <= 0)			// 보스전 스크롤 반복
	{
		m_tInfo.fY = 960.f;
	}
	

	return OBJ_NOEVENT;
}


void CBG_Stage_1::Late_Update()
{
}

void CBG_Stage_1::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Stage1");

	GdiTransparentBlt(_DC
		, 0, 0
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, 0, (int)m_tInfo.fY
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CBG_Stage_1::Release()
{
}

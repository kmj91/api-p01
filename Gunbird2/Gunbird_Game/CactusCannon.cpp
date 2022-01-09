// 기명준
// 숨은 선인장 캐논
// 배치 전까지 무적 배치 후 무적 해제
// 파괴되면 점수 아이템 생성과 스프라이트를 파괴된 잔해로 변경

#include "stdafx.h"
#include "CactusCannon.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "Explosion_02_1.h"
#include "Bullet_2.h"
#include "Coin.h"


CCactusCannon::CCactusCannon() : m_ePreState(CCactusCannon::END), m_eCurState(CCactusCannon::END), m_fDestPosY(0.f),
	m_dwShotTime(0), m_dwShotDelay(0), m_bOnObject(false)
{
}


CCactusCannon::~CCactusCannon()
{
	Release();
}

void CCactusCannon::Initialize()
{
	// 기본 위치 및 설정
	m_tInfo.iCX = CANNON_WIDTH * 3;
	m_tInfo.iCY = CANNON_HEIGHT * 3;
	m_tHitRectPos = { 6 * 3, 1 * 3, 26 * 3, 33 * 3 };
	m_iImageWidth = CANNON_WIDTH;
	m_iImageHeight = CANNON_HEIGHT;
	// 체력
	m_fHp = 60.f;
	// 이미지 번호
	m_iDrawID = CCactusCannon::A_90;
	// 기본 상태
	m_eCurState = CCactusCannon::HIDE;
	// 공격 간격
	m_dwShotDelay = 500;

	// 점수
	m_iScore = 500;
}

// 업데이트
int CCactusCannon::Update()
{
	// 삭제
	if (m_bRemove)
		return OBJ_DEAD;
	// 죽음
	if (m_bDead)
	{
		if (m_ePreState != CCactusCannon::DESTROY)
		{
			m_eCurState = CCactusCannon::DESTROY;
			m_bHpLock = true;		// 파괴되서 HP 다시 락
			CObj* pObj = CAbstractFactory<CExplosion_02_1>::Create(m_tInfo.fX, m_tInfo.fY);		// 파괴 이펙트 생성
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			// 아이템 코인 생성
			pObj = CAbstractFactory<CCoin>::Create(m_tInfo.fX, m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::ITEM);

			// 충돌 렉트
			m_tHitRectPos = { 0,0,0,0 };
		}
	}

	// 배경에 맞춰서 Y축 이동
	m_tInfo.fY += g_fBackgroundSpeed * 3.f;

	// 배치 후
	if (m_ePreState == CCactusCannon::ON) {
		// 조준
		Aim();
		if (m_dwShotTime + m_dwShotDelay < GetTickCount()) {
			// 공격
			Shot();
			m_dwShotTime = GetTickCount();
			m_dwShotDelay = 1000;
		}
	}
	// 배치 되기 전
	else if (m_ePreState == CCactusCannon::HIDE) {
		// 아직 숨은 상태
		if (!m_bOnObject) {
			// 목표 Y 좌표를 넘어가면
			if (m_tInfo.fY > m_fDestPosY) {
				// 숨어있다가 나옴
				m_bOnObject = true;
				m_tFrame.iFrameCnt = 0;
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 6;
				m_tFrame.iFrameScene = 0;
				m_tFrame.dwFrameTime = GetTickCount();
				m_tFrame.dwFrameSpeed = 80;
			}
		}
		// 숨어있다가 나옴
		else {
			// 애니메이션 프레임이 끝에 도달하면
			if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd) {
				// 무적 해제
				m_bHpLock = false;
				// 상태 변경
				m_eCurState = CCactusCannon::ON;
				m_dwShotTime = GetTickCount();
			}
		}
	}

	// 이미지 RECT 정보 및 Hit RECT 정보 갱신
	Update_Rect();
	// 프레임 씬 변경 처리
	Scene_Change();
	// 애니메이션 프레임 이동
	Frame_Move();

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CCactusCannon::Late_Update()
{
	// 맵 하단 밖으로 나가면 삭제
	if (WINCY <= m_tRect.top)
		m_bRemove = true;
}

// 렌더
void CCactusCannon::Render(HDC _DC)
{
	HDC hMemDC;

	// 일반 캐논과 다른점이 기본 캐논 이미지 도트 위에 선인장 이미지가 있음...
	if (m_ePreState == CCactusCannon::HIDE)
	{
		// 캐논
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Cannon_2");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));

		int iY;
		if (m_tFrame.iFrameCnt == 0)
			iY = 13 * 3;
		else if (m_tFrame.iFrameCnt == 1)
			iY = 17 * 3;
		else if (m_tFrame.iFrameCnt == 2)
			iY = 21 * 3;
		else if (m_tFrame.iFrameCnt == 3)
			iY = 25 * 3;
		else if (m_tFrame.iFrameCnt == 4)
			iY = 28 * 3;
		else if (m_tFrame.iFrameCnt == 5)
			iY = 29 * 3;
		else if (m_tFrame.iFrameCnt == 6)
			iY = 29 * 3;

		// 선인장
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Cactus");
		GdiTransparentBlt(_DC
			, (int)(m_tInfo.fX - ((CACTUS_WIDTH * 3) >> 1)) - 2, (int)((m_tInfo.fY - iY) - ((CACTUS_HEIGHT * 3) >> 1))
			, CACTUS_WIDTH * 3, CACTUS_HEIGHT * 3
			, hMemDC
			, 0, 0
			, CACTUS_WIDTH, CACTUS_HEIGHT
			, RGB(255, 0, 255));
	}
	else if (m_ePreState == CCactusCannon::ON)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Cannon_1");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_iDrawID * m_iImageWidth, 0
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));

		// 선인장
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Cactus");
		GdiTransparentBlt(_DC
			, (int)(m_tInfo.fX - ((CACTUS_WIDTH * 3) >> 1)) - 2, (int)((m_tInfo.fY - 29 * 3) - ((CACTUS_HEIGHT * 3) >> 1))
			, CACTUS_WIDTH * 3, CACTUS_HEIGHT * 3
			, hMemDC
			, 0, 0
			, CACTUS_WIDTH, CACTUS_HEIGHT
			, RGB(255, 0, 255));
	}
	else if (m_ePreState == CCactusCannon::DESTROY)
	{
		// 잔해
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Cannon_2");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
	}

	// 만약 옵션에서 충돌 박스 보기를 켰다면 (넘버패드 1번 키)
	// 충돌 박스도 렌더 해줘야함
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
}

void CCactusCannon::Release()
{
}

// 프레임 씬 변경 처리
void CCactusCannon::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
			// 숨어 있음
		case CCactusCannon::HIDE:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 999999;
			break;
			// 여기서 제어하지 않음 Aim()에서 제어함
		case CCactusCannon::ON:
			break;
			// 파괴
		case CCactusCannon::DESTROY:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 300;
			break;
		}
		m_ePreState = m_eCurState;
	}
}

// 조준
void CCactusCannon::Aim()
{
	float fX = m_pTarget->Get_Info().fX - m_tInfo.fX;	// 방향 좌표 X
	float fY = m_pTarget->Get_Info().fY - m_tInfo.fY;	// 방향 좌표 Y
	float fDia = sqrtf(fX * fX + fY * fY);				// 현재 위치에서 목적지까지 거리
	// arccos(cos) = 라디안 값을 구함
	float fRad = acosf(fX / fDia);
	// 라디안 값으로 호도각을 구함
	m_fAngle = fRad * 180.f / PI;
	// 타겟의 Y 좌표가 나의 Y 좌표 보다 크면 부호 반전
	if (m_tInfo.fY < m_pTarget->Get_Info().fY)
		m_fAngle *= -1.f;

	// 위에서 구한 호도각으로 스프라이트 인덱스 번호 변경
	if (-5.f <= m_fAngle && m_fAngle < 5.f)
		m_iDrawID = CCactusCannon::A0;
	if (-10.f <= m_fAngle && m_fAngle < -5.f)
		m_iDrawID = CCactusCannon::A_5;
	else if (-20.f <= m_fAngle && m_fAngle < -10.f)
		m_iDrawID = CCactusCannon::A_15;
	else if (-30.f <= m_fAngle && m_fAngle < -20.f)
		m_iDrawID = CCactusCannon::A_30;
	else if (-45.f <= m_fAngle && m_fAngle < -30.f)
		m_iDrawID = CCactusCannon::A_45;
	else if (-60.f <= m_fAngle && m_fAngle < -45.f)
		m_iDrawID = CCactusCannon::A_60;
	else if (-75.f <= m_fAngle && m_fAngle < -60.f)
		m_iDrawID = CCactusCannon::A_75;
	else if (-85.f <= m_fAngle && m_fAngle < -75.f)
		m_iDrawID = CCactusCannon::A_85;
	else if (-95.f <= m_fAngle && m_fAngle < -85.f)
		m_iDrawID = CCactusCannon::A_90;
	else if (-105.f <= m_fAngle && m_fAngle < -95.f)
		m_iDrawID = CCactusCannon::A_95;
	else if (-120.f <= m_fAngle && m_fAngle < -105.f)
		m_iDrawID = CCactusCannon::A_105;
	else if (-135.f <= m_fAngle && m_fAngle < -120.f)
		m_iDrawID = CCactusCannon::A_120;
	else if (-150.f <= m_fAngle && m_fAngle < -135.f)
		m_iDrawID = CCactusCannon::A_135;
	else if (-160.f <= m_fAngle && m_fAngle < -150.f)
		m_iDrawID = CCactusCannon::A_150;
	else if (-170.f <= m_fAngle && m_fAngle < -160.f)
		m_iDrawID = CCactusCannon::A_165;
	else if (-175.f <= m_fAngle && m_fAngle < -170.f)
		m_iDrawID = CCactusCannon::A_175;
	else if (-175.f > m_fAngle || 175.f <= m_fAngle)
		m_iDrawID = CCactusCannon::A180;
	else if (170.f <= m_fAngle && m_fAngle < 175.f)
		m_iDrawID = CCactusCannon::A175;
	else if (160.f <= m_fAngle && m_fAngle < 170.f)
		m_iDrawID = CCactusCannon::A165;
	else if (150.f <= m_fAngle && m_fAngle < 160.f)
		m_iDrawID = CCactusCannon::A150;
	else if (135.f <= m_fAngle && m_fAngle < 150.f)
		m_iDrawID = CCactusCannon::A135;
	else if (120.f <= m_fAngle && m_fAngle < 135.f)
		m_iDrawID = CCactusCannon::A120;
	else if (105.f <= m_fAngle && m_fAngle < 120.f)
		m_iDrawID = CCactusCannon::A105;
	else if (95.f <= m_fAngle && m_fAngle < 105.f)
		m_iDrawID = CCactusCannon::A95;
	else if (85.f <= m_fAngle && m_fAngle < 95.f)
		m_iDrawID = CCactusCannon::A90;
	else if (75.f <= m_fAngle && m_fAngle < 85.f)
		m_iDrawID = CCactusCannon::A85;
	else if (60.f <= m_fAngle && m_fAngle < 75.f)
		m_iDrawID = CCactusCannon::A75;
	else if (45.f <= m_fAngle && m_fAngle < 60.f)
		m_iDrawID = CCactusCannon::A60;
	else if (30.f <= m_fAngle && m_fAngle < 45.f)
		m_iDrawID = CCactusCannon::A45;
	else if (20.f <= m_fAngle && m_fAngle < 30.f)
		m_iDrawID = CCactusCannon::A30;
	else if (10.f <= m_fAngle && m_fAngle < 20.f)
		m_iDrawID = CCactusCannon::A15;
	else if (5.f <= m_fAngle && m_fAngle < 10.f)
		m_iDrawID = CCactusCannon::A5;
}

// 공격
void CCactusCannon::Shot()
{
	// 총알 오브젝트 생성
	CObj* pObj = CAbstractFactory<CBullet_2>::Create(m_tInfo.fX, m_tInfo.fY);
	pObj->Set_Angle(m_fAngle);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
}

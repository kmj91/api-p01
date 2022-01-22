// 기명준
// 이동 캐논
// 배치 전까지 무적 배치 후 무적 해제
// 파괴되면 점수 아이템 생성과 스프라이트를 파괴된 잔해로 변경

#include "stdafx.h"
#include "MoverCannon.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "Explosion_02_1.h"
#include "Bullet_2.h"
#include "Coin.h"


CMoverCannon::CMoverCannon()
	: m_ePreState(CMoverCannon::END), m_eCurState(CMoverCannon::END), m_fDestPosX(0.f), m_fDestPosY(0.f),
	m_dwShotTime(GetTickCount()), m_dwShotDelay(0)
{
}


CMoverCannon::~CMoverCannon()
{
	Release();
}

void CMoverCannon::Initialize()
{
	// 기본 위치 및 설정
	m_tInfo.iCX = MOVER_CANNON_WIDTH * 3;
	m_tInfo.iCY = MOVER_CANNON_HEIGHT * 3;
	m_tHitRectPos = { 6 * 3, 1 * 3, 26 * 3, 33 * 3 };
	m_iImageWidth = MOVER_CANNON_WIDTH;
	m_iImageHeight = MOVER_CANNON_HEIGHT;
	// 이동 속도
	m_fSpeed = 6.0f;
	// 체력
	m_fHp = 60.f;
	// 이미지 번호
	m_iDrawID = CMoverCannon::A90;
	// 기본 상태
	m_eCurState = CMoverCannon::IDEL;
	// 공격 간격
	m_dwShotDelay = 2000;

	// 점수
	m_iScore = 600;
}

// 업데이트
int CMoverCannon::Update()
{
	// 삭제
	if (m_bRemove)
		return OBJ_DEAD;
	// 죽음
	if (m_bDead)
	{
		if (m_ePreState != CMoverCannon::DESTROY)
		{
			m_eCurState = CMoverCannon::DESTROY;
			m_bHpLock = true;		// 파괴되서 HP 다시 락
			CObj* pObj = CObjFactory<CExplosion_02_1>::Create(m_tInfo.fX, m_tInfo.fY);		// 파괴 이펙트 생성
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

			// 아이템 코인 생성
			pObj = CObjFactory<CCoin>::Create(m_tInfo.fX, m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::ITEM);

			// 충돌 렉트
			m_tHitRectPos = { 0,0,0,0 };
		}
	}

	// 배경에 맞춰서 Y축 이동
	m_tInfo.fY += g_fBackgroundSpeed * 3.f;
	// 목적지도 배경 속도에 맞춰서 Y축 이동해야됨
	m_fDestPosY += g_fBackgroundSpeed * 3.f;

	// 파괴되지 않았음
	if (m_ePreState != CMoverCannon::DESTROY) {
		// 조준
		Aim();
		// 충분한 공격 대기 시간이 지났으면
		if (m_dwShotTime + m_dwShotDelay < GetTickCount()) {
			// 공격
			Shot();
			m_dwShotTime = GetTickCount();
		}
	}

	// 기본 상태
	if (m_ePreState == CMoverCannon::IDEL)
	{
		// 이동
		if (Move()) {
			// 이동 완료
			m_eCurState = CMoverCannon::ON;
			// 이제 파괴 될 수 있음
			m_bHpLock = false;
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
void CMoverCannon::Late_Update()
{
	// 맵 하단 밖으로 나가면 삭제
	if (WINCY <= m_tRect.top)
		m_bRemove = true;
}

// 렌더
void CMoverCannon::Render(HDC _DC)
{
	HDC hMemDC;
	if (m_ePreState == CMoverCannon::IDEL || m_ePreState == CMoverCannon::ON)
	{
		// 하체
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"MoverCannon_2");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));

		// 상체
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"MoverCannon_1");
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_iDrawID * m_iImageWidth, 0
			, m_iImageWidth, m_iImageHeight
			, RGB(255, 0, 255));
	}
	if (m_ePreState == CMoverCannon::DESTROY)
	{
		// 잔해
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"MoverCannon_2");
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

void CMoverCannon::Release()
{
}

// 프레임 씬 변경 처리
void CMoverCannon::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
			// 대기
		case CMoverCannon::IDEL:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 80;
			break;
			// 여기서 제어하지 않음 Aim()에서 제어함
		case CMoverCannon::ON:
			break;
			// 파괴
		case CMoverCannon::DESTROY:
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

// 이동
bool CMoverCannon::Move()
{
	float fX = m_fDestPosX - m_tInfo.fX;	// 방향 좌표 X
	float fY = m_fDestPosY - m_tInfo.fY;	// 방향 좌표 Y
	float fDia = sqrtf(fX * fX + fY * fY);	// 현재 위치에서 목적지까지 거리
	float fSpeed = m_fSpeed;				// 이동 속도

	// 목적지까지의 거리가 이동 속도보다 작다면
	// 목표 거리만큼 이동할 수 있게 속도 값을 바꿈
	if (fDia < fSpeed)
		fSpeed = fDia;
	// 좌표 이동
	m_tInfo.fX += fX / fDia * fSpeed;
	m_tInfo.fY += fY / fDia * fSpeed;
	
	// 목적지에 도착했는지?
	if (fDia - fSpeed == 0)
		return true;

	return false;
}

// 조준
void CMoverCannon::Aim()
{
	float	fX = m_pTarget->Get_Info().fX - m_tInfo.fX;	// 방향 좌표 X
	float	fY = m_pTarget->Get_Info().fY - m_tInfo.fY;	// 방향 좌표 Y
	float	fDia = sqrtf(fX * fX + fY * fY);			// 현재 위치에서 목적지까지 거리
	// arccos(cos) = 라디안 값을 구함
	float	fRad = acosf(fX / fDia);
	// 라디안 값으로 호도각을 구함
	m_fAngle = fRad * 180.f / PI;
	// 타겟의 Y 좌표가 나의 Y 좌표 보다 크면 부호 반전
	if (m_tInfo.fY < m_pTarget->Get_Info().fY)
		m_fAngle *= -1.f;

	// 위에서 구한 호도각으로 스프라이트 인덱스 번호 변경
	if (-5.f <= m_fAngle && m_fAngle < 5.f)
		m_iDrawID = CMoverCannon::A0;
	if (-10.f <= m_fAngle && m_fAngle < -5.f)
		m_iDrawID = CMoverCannon::A_5;
	else if (-20.f <= m_fAngle && m_fAngle < -10.f)
		m_iDrawID = CMoverCannon::A_15;
	else if (-30.f <= m_fAngle && m_fAngle < -20.f)
		m_iDrawID = CMoverCannon::A_30;
	else if (-45.f <= m_fAngle && m_fAngle < -30.f)
		m_iDrawID = CMoverCannon::A_45;
	else if (-60.f <= m_fAngle && m_fAngle < -45.f)
		m_iDrawID = CMoverCannon::A_60;
	else if (-75.f <= m_fAngle && m_fAngle < -60.f)
		m_iDrawID = CMoverCannon::A_75;
	else if (-85.f <= m_fAngle && m_fAngle < -75.f)
		m_iDrawID = CMoverCannon::A_85;
	else if (-95.f <= m_fAngle && m_fAngle < -85.f)
		m_iDrawID = CMoverCannon::A_90;
	else if (-105.f <= m_fAngle && m_fAngle < -95.f)
		m_iDrawID = CMoverCannon::A_95;
	else if (-120.f <= m_fAngle && m_fAngle < -105.f)
		m_iDrawID = CMoverCannon::A_105;
	else if (-135.f <= m_fAngle && m_fAngle < -120.f)
		m_iDrawID = CMoverCannon::A_120;
	else if (-150.f <= m_fAngle && m_fAngle < -135.f)
		m_iDrawID = CMoverCannon::A_135;
	else if (-160.f <= m_fAngle && m_fAngle < -150.f)
		m_iDrawID = CMoverCannon::A_150;
	else if (-170.f <= m_fAngle && m_fAngle < -160.f)
		m_iDrawID = CMoverCannon::A_165;
	else if (-175.f <= m_fAngle && m_fAngle < -170.f)
		m_iDrawID = CMoverCannon::A_175;
	else if (-175.f < m_fAngle && 175.f <= m_fAngle )
		m_iDrawID = CMoverCannon::A180;
	else if (170.f <= m_fAngle && m_fAngle < 175.f)
		m_iDrawID = CMoverCannon::A175;
	else if (160.f <= m_fAngle && m_fAngle < 170.f)
		m_iDrawID = CMoverCannon::A165;
	else if (150.f <= m_fAngle && m_fAngle < 160.f)
		m_iDrawID = CMoverCannon::A150;
	else if (135.f <= m_fAngle && m_fAngle < 150.f)
		m_iDrawID = CMoverCannon::A135;
	else if (120.f <= m_fAngle && m_fAngle < 135.f)
		m_iDrawID = CMoverCannon::A120;
	else if (105.f <= m_fAngle && m_fAngle < 120.f)
		m_iDrawID = CMoverCannon::A105;
	else if (95.f <= m_fAngle && m_fAngle < 105.f)
		m_iDrawID = CMoverCannon::A95;
	else if (85.f <= m_fAngle && m_fAngle < 95.f)
		m_iDrawID = CMoverCannon::A90;
	else if (75.f <= m_fAngle && m_fAngle < 85.f)
		m_iDrawID = CMoverCannon::A85;
	else if (60.f <= m_fAngle && m_fAngle < 75.f)
		m_iDrawID = CMoverCannon::A75;
	else if (45.f <= m_fAngle && m_fAngle < 60.f)
		m_iDrawID = CMoverCannon::A60;
	else if (30.f <= m_fAngle && m_fAngle < 45.f)
		m_iDrawID = CMoverCannon::A45;
	else if (20.f <= m_fAngle && m_fAngle < 30.f)
		m_iDrawID = CMoverCannon::A30;
	else if (10.f <= m_fAngle && m_fAngle < 20.f)
		m_iDrawID = CMoverCannon::A15;
	else if (5.f <= m_fAngle && m_fAngle < 10.f)
		m_iDrawID = CMoverCannon::A5;
}

// 공격
void CMoverCannon::Shot()
{
	// 총알 오브젝트 생성
	CObj* pObj = CObjFactory<CBullet_2>::Create(m_tInfo.fX, m_tInfo.fY);
	pObj->Set_Angle(m_fAngle);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
}

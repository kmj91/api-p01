// 기명준
// 중간 보스 로봇 샤크 공중 몬스터
// 파괴되면 파워업 아이템 드랍

#include "stdafx.h"
#include "RobotShark.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "Explosion_03.h"
#include "Explosion_01.h"
#include "Bullet_3.h"
#include "Bullet_1.h"
#include "Bullet_4.h"

#include "Bomb.h"


CRobotShark::CRobotShark()
	: m_fDestPosX(0.f), m_fDestPosY(0.f), m_fOutPosX(0.f), m_fOutPosY(0.f),
	m_dwShotTime(0), m_dwShotDelay(0), m_eAttackState(CRobotShark::END),
	m_tBodyFrame{}, m_tPersonFrame{}, m_bOnObject(false), m_bOutObject(false)
{
}


CRobotShark::~CRobotShark()
{
	Release();
}

void CRobotShark::Initialize()
{
	// 기본 위치 및 설정
	m_tInfo.iCX = ROBOT_SHARK_WIDTH * 3;
	m_tInfo.iCY = ROBOT_SHARK_HEIGHT * 3;
	m_tHitRectPos = { 5 * 3, 2 * 3, 69 * 3, 62 * 3 };
	m_iImageWidth = ROBOT_SHARK_WIDTH;
	m_iImageHeight = ROBOT_SHARK_HEIGHT;

	// 추진기
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iFrameScene = 2;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 40;

	// 몸
	m_tBodyFrame.iFrameCnt = 0;
	m_tBodyFrame.iFrameStart = 0;
	m_tBodyFrame.iFrameEnd = 1;
	m_tBodyFrame.iFrameScene = 0;
	m_tBodyFrame.dwFrameTime = GetTickCount();
	m_tBodyFrame.dwFrameSpeed = 100;

	// 탑승자
	m_tPersonFrame.iFrameCnt = 0;
	m_tPersonFrame.iFrameStart = 0;
	m_tPersonFrame.iFrameEnd = 0;
	m_tPersonFrame.iFrameScene = 1;
	m_tPersonFrame.dwFrameTime = GetTickCount();
	m_tPersonFrame.dwFrameSpeed = 999999;
	// 이동 속도
	m_fSpeed = 5.0f;
	// 체력
	m_fHp = 500;
	// 공격 간격
	m_dwShotDelay = 1200;
	// 공격 패턴 상태
	m_eAttackState = CRobotShark::SHOT_1;

	// 점수
	m_iScore = 2000;
}

// 업데이트
int CRobotShark::Update()
{
	// 삭제
	if (m_bRemove)
		return OBJ_DEAD;
	// 죽음
	if (m_bDead) {
		CObj* pObj = CAbstractFactory<CExplosion_03>::Create(m_tInfo.fX, m_tInfo.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		int irand = rand() % 3 + 1;
		pObj = CAbstractFactory<CExplosion_01>::Create(m_tInfo.fX + irand * 30, m_tInfo.fY + 20.f);
		static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)(irand * 10));
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		pObj = CAbstractFactory<CExplosion_01>::Create(m_tInfo.fX - irand * 30, m_tInfo.fY + 50.f);
		static_cast<CEffect*>(pObj)->Set_EffectDelay((DWORD)100);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		// 폭탄 아이템 드랍
		pObj = CAbstractFactory<CBomb>::Create(m_tInfo.fX, m_tInfo.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::ITEM);

		return OBJ_DEAD;
	}

	// 배치 되기 전
	if (!m_bOnObject) {
		// 배치 좌표로 이동
		if (Move(m_fDestPosX, m_fDestPosY)) {
			// 목적지에 도착
			m_bOnObject = true;
			// 이동 속도 변경
			m_fSpeed = 1.f;
		}
	}
	// 배치 된 후
	else {
		// 공격 시간이 됬으면
		if (m_dwShotTime + m_dwShotDelay < GetTickCount()) {
			// 공격 처리
			switch (m_eAttackState)
			{
			case CRobotShark::SHOT_1:
				m_eAttackState = CRobotShark::SHOT_2;
				Shot_1();
				// 스프라이트 프레임
				m_tPersonFrame.iFrameStart = 1;
				m_tPersonFrame.iFrameEnd = 2;
				m_tPersonFrame.dwFrameSpeed = 1000;
				break;
			case CRobotShark::SHOT_2:
				m_eAttackState = CRobotShark::SHOT_3;
				Shot_2();
				// 스프라이트 프레임
				m_tPersonFrame.iFrameStart = 3;
				m_tPersonFrame.iFrameEnd = 4;
				m_tPersonFrame.dwFrameSpeed = 300;
				break;
			case CRobotShark::SHOT_3:
				m_eAttackState = CRobotShark::SHOT_4;
				m_dwShotDelay = 100;
				Shot_3();
				break;
			case CRobotShark::SHOT_4:
				m_eAttackState = CRobotShark::SHOT_1;
				m_dwShotDelay = 2000;
				Shot_3();
				break;
			}
			// 공격 후 시간 저장
			m_dwShotTime = GetTickCount();
		}

		// 퇴장하기 전
		if (!m_bOutObject) {
			// 이동
			if (Move(m_fDestPosX - 300.f, m_fDestPosY)) {
				// 퇴장 플래그
				m_bOutObject = true;
				// 상태 변경
				m_eAttackState = CRobotShark::END;
				// 선회 각도
				m_fAngle = -60.f;
				// 이동 속도 변경
				m_fSpeed = 3.f;
			}
		}
		// 맵 바깥으로 퇴장
		else {
			// 선회 이동
			if (MoveAngle(m_fOutPosX, m_fOutPosY)) {
				// 도착하면 삭제
				m_bRemove = true;
			}
		}
	}

	// 이미지 RECT 정보 및 Hit RECT 정보 갱신
	Update_Rect();
	// 이미지 프레임 이동
	Frame_Move();
	// 몸통 이미지 프레임 이동
	Frame_MoveEX();

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CRobotShark::Late_Update()
{
	// 모든 게임 오브젝트는 초기화 때 무적 상태
	// 몬스터는 일단 화면에 보이고 나서 무적 해제 됨
	if (m_bHpLock)
		if (0 <= m_tRect.top && 0 <= m_tRect.left && WINCX >= m_tRect.right)
			m_bHpLock = false;
}

// 렌더
void CRobotShark::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"RobotShark");

	// 추진기 이펙트
	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));

	// 몸체
	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tBodyFrame.iFrameCnt * m_iImageWidth, m_tBodyFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));

	// 탑승자
	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tPersonFrame.iFrameCnt * m_iImageWidth, m_tPersonFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));

	// 만약 옵션에서 충돌 박스 보기를 켰다면 (넘버패드 1번 키)
	// 충돌 박스도 렌더 해줘야함
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
}

void CRobotShark::Release()
{
}

// 이동
// _fDestX : 목적지 좌표 X
// _fDestY : 목적지 좌표 Y
// 반환 값 : 목적지에 도착했으면 true 아니면 false
bool CRobotShark::Move(float _fDestX, float _fDestY)
{
	float fX = _fDestX - m_tInfo.fX;		// 방향 좌표 X
	float fY = _fDestY - m_tInfo.fY;		// 방향 좌표 Y
	float fDia = sqrtf(fX * fX + fY * fY);	// 현재 위치에서 목적지까지 거리
	float fSpeed = m_fSpeed;				// 이동 속도

	// 목적지까지의 거리가 이동 속도보다 작다면
	// 목표 거리만큼 이동할 수 있게 속도 값을 바꿈
	if (fDia < fSpeed)
		fSpeed = fDia;

	// 목적지에 도착하지 않았으면 이동 처리
	m_tInfo.fX += fX / fDia * fSpeed;
	m_tInfo.fY += fY / fDia * fSpeed;

	// 목적지에 도착했는지?
	if (fDia - fSpeed == 0)
		return true;

	return false;
}

// 선회 이동
// _fDestX : 목적지 좌표 X
// _fDestY : 목적지 좌표 Y
// 반환 값 : 목적지에 도착했으면 true 아니면 false
bool CRobotShark::MoveAngle(float _fDestX, float _fDestY)
{
	float fX = _fDestX - m_tInfo.fX;		// 방향 좌표 X
	float fY = _fDestY - m_tInfo.fY;		// 방향 좌표 Y
	float fDia = sqrtf(fX * fX + fY * fY);	// 현재 위치에서 목적지까지 거리
	float fvx;					// 직선 이동 좌표 X
	float fvy;					// 직선 이동 좌표 Y
	float fRad;					// _fAngle 라디안 값
	float fvx2;					// 회전 결과 좌표 값 X
	float fvy2;					// 회전 결과 좌표 값 Y
	float fSpeed = m_fSpeed;	// 이동 속도

	// 목적지까지의 거리가 이동 속도보다 작다면
	// 목표 거리만큼 이동할 수 있게 속도 값을 바꿈
	if (fDia < fSpeed)
		fSpeed = fDia;

	// fX / fDia 결과는 이동 거리가 1일 때 이동하는 X 좌표 값 단위벡터
	// 단위벡터에 fSpeed(이동 속도)를 곱해서 원하는 만큼 이동하는 좌표 값을 구함
	// fvx, fvy는 직선이동했을 때의 좌표 값
	fvx = fX / fDia * fSpeed;
	fvy = fY / fDia * fSpeed;

	// 선회 각도 라디안 값
	fRad = PI / 180.f * m_fAngle;
	// 회전 이동한 좌표 값
	fvx2 = cos(fRad) * fvx - sin(fRad) * fvy;
	fvy2 = sin(fRad) * fvx + cos(fRad) * fvy;
	// 위에서 구한 값을 현재 좌표에 더해서 좌표 이동
	m_tInfo.fX += fvx2;
	m_tInfo.fY += fvy2;

	// 목적지에 도착했는지?
	if (fDia - fSpeed == 0)
		return true;

	return false;
}

// 몸통 이미지 프레임 이동
void CRobotShark::Frame_MoveEX()
{
	if (m_tBodyFrame.dwFrameTime + m_tBodyFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tBodyFrame.iFrameCnt;
		if (m_tBodyFrame.iFrameCnt > m_tBodyFrame.iFrameEnd)
			m_tBodyFrame.iFrameCnt = m_tBodyFrame.iFrameStart;

		m_tBodyFrame.dwFrameTime = GetTickCount();
	}

	if (m_tPersonFrame.dwFrameTime + m_tPersonFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tPersonFrame.iFrameCnt;
		if (m_tPersonFrame.iFrameCnt > m_tPersonFrame.iFrameEnd)
			m_tPersonFrame.iFrameCnt = m_tBodyFrame.iFrameStart;

		m_tPersonFrame.dwFrameTime = GetTickCount();
	}
}

// 공격 패턴 1
// 흩뿌리기
void CRobotShark::Shot_1()
{
	float fCnt = 0;

	while (fCnt < 180.f)
	{
		CObj* pObj = CAbstractFactory<CBullet_3>::Create(m_tInfo.fX - 30.f, m_tInfo.fY - 114.f);
		pObj->Set_Angle(fCnt);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		fCnt = fCnt + 25.f;
	}

	fCnt = -180.f;
	while (fCnt < 0.f)
	{
		CObj* pObj = CAbstractFactory<CBullet_3>::Create(m_tInfo.fX - 30.f, m_tInfo.fY - 114.f);
		pObj->Set_Angle(fCnt);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		fCnt = fCnt + 25.f;
	}

	while (fCnt < 180.f)
	{
		CObj* pObj = CAbstractFactory<CBullet_3>::Create(m_tInfo.fX + 33.f, m_tInfo.fY - 114.f);
		pObj->Set_Angle(fCnt);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		fCnt = fCnt + 25.f;
	}

	fCnt = -180.f;
	while (fCnt < 0.f)
	{
		CObj* pObj = CAbstractFactory<CBullet_3>::Create(m_tInfo.fX + 33.f, m_tInfo.fY - 114.f);
		pObj->Set_Angle(fCnt);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

		fCnt = fCnt + 25.f;
	}
}

// 공격 패턴 2
// 팔 직선 공격
void CRobotShark::Shot_2()
{
	// 왼팔
	CObj* pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX - 87.f, m_tInfo.fY + 60.f);
	pObj->Set_Angle(-90.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX - 99.f, m_tInfo.fY + 48.f);
	pObj->Set_Angle(-90.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	// 오른팔
	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX + 87.f, m_tInfo.fY + 60.f);
	pObj->Set_Angle(-90.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_4>::Create(m_tInfo.fX + 99.f, m_tInfo.fY + 48.f);
	pObj->Set_Angle(-90.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
}

// 공격 패턴 3
// 팔 교차 공격
void CRobotShark::Shot_3()
{
	// 왼팔
	CObj* pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX - 99.f, m_tInfo.fY + 30.f);
	pObj->Set_Angle(-130.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
	pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX - 99.f, m_tInfo.fY + 30.f);
	pObj->Set_Angle(-120.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX - 69.f, m_tInfo.fY + 30.f);
	pObj->Set_Angle(-50.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
	pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX - 69.f, m_tInfo.fY + 30.f);
	pObj->Set_Angle(-60.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	// 오른팔
	pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX + 99.f, m_tInfo.fY + 30.f);
	pObj->Set_Angle(-130.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
	pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX + 99.f, m_tInfo.fY + 30.f);
	pObj->Set_Angle(-120.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);

	pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX + 69.f, m_tInfo.fY + 30.f);
	pObj->Set_Angle(-50.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
	pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX + 69.f, m_tInfo.fY + 30.f);
	pObj->Set_Angle(-60.f);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
}


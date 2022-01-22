#include "stdafx.h"
#include "RedBeanie.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "Explosion_02_1.h"
#include "Bullet_2.h"


CRedBeanie::CRedBeanie()
	: m_ePreState(CRedBeanie::END), m_eCurState(CRedBeanie::END), m_fDestPosY(0.f),
	m_dwShotTime(0), m_dwShotDelay(0), m_iShot(0), m_tBodyFrame{}
{
}


CRedBeanie::~CRedBeanie()
{
	Release();
}

void CRedBeanie::Initialize()
{
	// 기본 위치 및 설정
	m_tInfo.iCX = RED_BEANIE_WIDTH * 3;
	m_tInfo.iCY = RED_BEANIE_HEIGHT * 3;
	m_tHitRectPos = { 18 * 3, 7 * 3, 41 * 3, 38 * 3 };
	m_iImageWidth = RED_BEANIE_WIDTH;
	m_iImageHeight = RED_BEANIE_HEIGHT;
	// 기본 애니메이션 프레임 설정
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 40;
	// 이동 속도
	m_fSpeed = 5.0f;
	// 체력
	m_fHp = 80.f;
	// 기본 상태
	m_eCurState = CRedBeanie::IDLE;
	// 공격 간격
	m_dwShotDelay = 200;
	// 공격 횟수
	m_iShot = 2;

	// 점수
	m_iScore = 1000;
}

// 업데이트
int CRedBeanie::Update()
{
	// 삭제
	if (m_bRemove)
		return OBJ_DEAD;
	// 죽음
	if (m_bDead)
	{
		// 파괴 이펙트 생성
		CObj* pObj = CObjFactory<CExplosion_02_1>::Create(m_tInfo.fX, m_tInfo.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
		return OBJ_DEAD;
	}
		

	// 목적지까지 아직 도달하지 못했으면
	if(m_fDestPosY > m_tInfo.fY)
		m_tInfo.fY += m_fSpeed;
	// 목적지까지 도착
	else {
		// 목적지까지 왔으면 공격
		if (m_ePreState == CRedBeanie::IDLE)
			m_eCurState = CRedBeanie::ATTACK;
		else if (m_ePreState == CRedBeanie::ATTACK) {
			// 공격 프레임 이펙트 13 이후부터
			if (m_tBodyFrame.iFrameCnt >= 13) {
				if (m_dwShotTime + m_dwShotDelay < GetTickCount()) {
					if (m_iShot > 0) {
						--m_iShot;
						Shot();
						m_dwShotTime = GetTickCount();
					}
				}
			}

			// 공격이 끝났으면 상태를 이동으로 변경
			if (m_tBodyFrame.iFrameCnt == m_tBodyFrame.iFrameEnd)
				m_eCurState = CRedBeanie::MOVE;
		}
		// 맵 하단으로 퇴장
		else if (m_ePreState == CRedBeanie::MOVE) {
			m_tInfo.fY += m_fSpeed;		 //Y축 이동
		}
	}

	// 이미지 RECT 정보 및 Hit RECT 정보 갱신
	Update_Rect();
	// 프레임 씬 변경 처리
	Scene_Change();
	// 이미지 프레임 이동
	Frame_Move();
	// 이펙트 이미지 프레임 이동
	Frame_MoveEX();

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CRedBeanie::Late_Update()
{
	// 맵 안으로 들어오기 전까지 무적 들어온 후 무적 해제
	if (m_bHpLock)
		if (0 <= m_tRect.bottom)
			m_bHpLock = false;

	// 맵 하단 밖으로 나가면 삭제
	if (WINCY + 100 <= m_tRect.top)
		m_bRemove = true;
}

// 렌더
void CRedBeanie::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"RedBeanie_2");
	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));

	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"RedBeanie_1");
	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tBodyFrame.iFrameCnt * m_iImageWidth, m_tBodyFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));


	// 만약 옵션에서 충돌 박스 보기를 켰다면 (넘버패드 1번 키)
	// 충돌 박스도 렌더 해줘야함
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
}

void CRedBeanie::Release()
{
}

// 프레임 씬 변경 처리
void CRedBeanie::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
			// 목적지 까지 도착하기 전
		case CRedBeanie::IDLE:
			m_tBodyFrame.iFrameCnt = 0;
			m_tBodyFrame.iFrameStart = 0;
			m_tBodyFrame.iFrameEnd = 0;
			m_tBodyFrame.iFrameScene = 0;
			m_tBodyFrame.dwFrameTime = GetTickCount();
			m_tBodyFrame.dwFrameSpeed = 999999;
			break;
			// 도착 후 공격
		case CRedBeanie::ATTACK:
			m_tBodyFrame.iFrameCnt = 0;
			m_tBodyFrame.iFrameStart = 0;
			m_tBodyFrame.iFrameEnd = 26;
			m_tBodyFrame.iFrameScene = 0;
			m_tBodyFrame.dwFrameTime = GetTickCount();
			m_tBodyFrame.dwFrameSpeed = 80;
			break;
			// 공격 완료 후 퇴장
		case CRedBeanie::MOVE:
			m_tBodyFrame.iFrameCnt = 0;
			m_tBodyFrame.iFrameStart = 0;
			m_tBodyFrame.iFrameEnd = 0;
			m_tBodyFrame.iFrameScene = 0;
			m_tBodyFrame.dwFrameTime = GetTickCount();
			m_tBodyFrame.dwFrameSpeed = 999999;
			break;
		}
		m_ePreState = m_eCurState;
	}
}

// 이펙트 이미지 프레임 이동
void CRedBeanie::Frame_MoveEX()
{
	if (m_tBodyFrame.dwFrameTime + m_tBodyFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tBodyFrame.iFrameCnt;
		if (m_tBodyFrame.iFrameCnt > m_tBodyFrame.iFrameEnd)
			m_tBodyFrame.iFrameCnt = m_tBodyFrame.iFrameStart;

		m_tBodyFrame.dwFrameTime = GetTickCount();
	}
}

// 공격
void CRedBeanie::Shot()
{
	float fX = m_pTarget->Get_Info().fX - m_tInfo.fX;	// 방향 좌표 X
	float fY = m_pTarget->Get_Info().fY - m_tInfo.fY;	// 방향 좌표 Y
	float fDia = sqrtf(fX * fX + fY * fY);				// 현재 위치에서 목적지까지 거리
	// arccos(cos) = 라디안 값을 구함
	float fRad = acosf(fX / fDia);
	// 라디안 값으로 각도를 구함
	m_fAngle = fRad * 180.f / PI;
	// 타겟의 Y 좌표가 나의 Y 좌표 보다 크면 부호 반전
	if (m_tInfo.fY < m_pTarget->Get_Info().fY)
		m_fAngle *= -1.f;

	// 총알 오브젝트 생성
	CObj* pObj = CObjFactory<CBullet_2>::Create(m_tInfo.fX, m_tInfo.fY);
	pObj->Set_Angle(m_fAngle);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
}

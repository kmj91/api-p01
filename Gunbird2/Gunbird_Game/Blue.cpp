// 기명준
// 적 몬스터 파란색 헬리콥터
// 공중 몬스터 선회하면서 등장 그리고 일정 시간 후 퇴장

#include "stdafx.h"
#include "Blue.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "Explosion_01.h"
#include "Bullet_1.h"


CBlue::CBlue()
	: m_ePreState(CBlue::END), m_eCurState(CBlue::END), m_dwOnTime(0), m_dwOutTime(0),
	m_fDestPosX(0.f), m_fDestPosY(0.f), m_fOutPosX(0.f), m_fOutPosY(0.f), m_fDestAngle(0.f), m_fOutAngle(0.f),
	m_iShot(0), m_bOnObject(false), m_bOutObject(false)
{
}


CBlue::~CBlue()
{
	Release();
}

void CBlue::Initialize()
{
	// 기본 위치 및 설정
	m_tInfo.iCX = BLUE_WIDTH * 3;
	m_tInfo.iCY = BLUE_HEIGHT * 3;
	m_tHitRectPos = { 10 * 3, 11 * 3, 32 * 3, 31 * 3 };
	m_iImageWidth = BLUE_WIDTH;
	m_iImageHeight = BLUE_HEIGHT;
	// 기본 애니메이션 프레임 설정
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 40;
	// 이동 속도
	m_fSpeed = 7.0f;
	// 기본 상태
	m_eCurState = CBlue::FRONT;

	// 점수
	m_iScore = 200;
}

int CBlue::Update()
{
	// 삭제
	if (m_bRemove)
		return OBJ_DEAD;
	// 죽음
	if (m_bDead)
	{
		CObj* pObj = CAbstractFactory<CExplosion_01>::Create(m_tInfo.fX, m_tInfo.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		return OBJ_DEAD;
	}

	// 배치 되기 전
	if (!m_bOnObject)
	{
		// 배치 좌표로 이동
		if(Move(m_fDestPosX, m_fDestPosY, m_fDestAngle))
		{
			// 목적지에 도착
			m_eCurState = CBlue::FRONT;
			m_bOnObject = true;
			m_dwOnTime = GetTickCount();
		}
	}
	// 배치 된 후
	else
	{
		// 퇴장 하기 전
		if (!m_bOutObject)
		{
			// 퇴장 시간인가?
			DWORD dwTime = GetTickCount();
			if ((dwTime - m_dwOnTime) > m_dwOutTime) {
				m_bOutObject = true;
				m_iShot = 1;
			}
		}
		// 퇴장
		else
		{
			// 퇴장 좌표로 이동
			if (Move(m_fOutPosX, m_fOutPosY, m_fOutAngle))
			{
				// 삭제 플래그 true
				m_bRemove = true;
			}
		}
	}

	// 총알이 남아있으면
	if (m_iShot > 0)
	{
		// 총알 감소
		--m_iShot;
		// 총알 공격
		Shot();
	}

	// 이미지 RECT 정보 및 Hit RECT 정보 갱신
	Update_Rect();
	// 프레임 씬 변경 처리
	Scene_Change();
	// 애니메이션 프레임 이동
	Frame_Move();

	return OBJ_NOEVENT;
}

void CBlue::Late_Update()
{
	// 모든 게임 오브젝트는 초기화 때 무적 상태
	if (m_bHpLock)
	{
		// 몬스터는 일단 화면에 보이고 나서 무적 해제 됨
		if (0 <= m_tRect.bottom && 0 <= m_tRect.right && WINCX >= m_tRect.left)
		{
			// 무적 상태 해제
			m_bHpLock = false;
			// 총알 한발 충전
			m_iShot = 1;
		}
	}
}

void CBlue::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Blue");

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

void CBlue::Release()
{
}

// 프레임 씬 변경 처리
void CBlue::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CBlue::FRONT:
			m_tFrame.iFrameScene = CBlue::FRONT;
			break;
		case CBlue::LEFT_1:
			m_tFrame.iFrameScene = CBlue::LEFT_1;
			break;
		case CBlue::LEFT_2:
			m_tFrame.iFrameScene = CBlue::LEFT_2;
			break;
		case CBlue::LEFT_3:
			m_tFrame.iFrameScene = CBlue::LEFT_3;
			break;
		case CBlue::RIGHT_1:
			m_tFrame.iFrameScene = CBlue::RIGHT_1;
			break;
		case CBlue::RIGHT_2:
			m_tFrame.iFrameScene = CBlue::RIGHT_2;
			break;
		case CBlue::RIGHT_3:
			m_tFrame.iFrameScene = CBlue::RIGHT_3;
			break;
		case CBlue::END:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

// 파라미터 좌표로 이동
// _fDestX : 목적지 좌표 X
// _fDestY : 목적지 좌표 Y
// _fAngle : 선회 각도
// 반환 값 : 목적지에 도착했으면 true 아니면 false
bool CBlue::Move(float _fDestX, float _fDestY, float _fAngle)
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

	// 각도가 너무 크면
	// 원점 거리에 도달하기 직전에 너무 빙글빙글 돔
	if (abs(_fAngle) > 30 && fDia < 30) {
		// 보정해줌
		_fAngle = _fAngle / 2.f;
	}

	// 선회 각도 라디안 값
	fRad = PI / 180.f * _fAngle;
	// 회전 이동한 좌표 값
	fvx2 = cos(fRad) * fvx - sin(fRad) * fvy;
	fvy2 = sin(fRad) * fvx + cos(fRad) * fvy;
	// 위에서 구한 값을 현재 좌표에 더해서 좌표 이동
	m_tInfo.fX += fvx2;
	m_tInfo.fY += fvy2;

	// X 좌표 이동 값에 따라 상태를 바꿈
	// 상태는 렌더를 할때 스프라이트 이미지의 기울기 정도임
	if (fvx2 > 4.f) {
		m_eCurState = CBlue::RIGHT_3;
	}
	else if (fvx2 > 2.f) {
		m_eCurState = CBlue::RIGHT_2;
	}
	else if (fvx2 > 0.5f) {
		m_eCurState = CBlue::RIGHT_1;
	}
	else if (fvx2 < -4.f) {
		m_eCurState = CBlue::LEFT_3;
	}
	else if (fvx2 < -2.f) {
		m_eCurState = CBlue::LEFT_2;
	}
	else if (fvx2 < -0.5f) {
		m_eCurState = CBlue::LEFT_1;
	}
	else {
		m_eCurState = CBlue::FRONT;
	}

	// 목적지에 도착했는지?
	if (fDia - fSpeed == 0)
		return true;

	return false;
}

// 총알 공격
void CBlue::Shot()
{
	float	fX = m_pTarget->Get_Info().fX - m_tInfo.fX;	// 방향 좌표 X
	float	fY = m_pTarget->Get_Info().fY - m_tInfo.fY;	// 방향 좌표 Y
	float	fDia = sqrtf(fX * fX + fY * fY);			// 거리
	// arccos(cos) = 라디안 값을 구함
	float	fRad = acosf(fX / fDia);
	// 라디안 값으로 호도각을 구함
	float fAngle = fRad * 180.f / PI;
	// 타겟의 Y 좌표가 나의 Y 좌표 보다 크면 부호 반전
	if (m_tInfo.fY < m_pTarget->Get_Info().fY)
		fAngle *= -1.f;

	// 너무 완벽하게 플레이어 위치를 추적하지 못하게 자릿수 버림
	fAngle = (float)((int)fAngle / 10 * 10);
	// 총알 오브젝트 생성
	CObj* pObj = CAbstractFactory<CBullet_1>::Create(m_tInfo.fX, m_tInfo.fY - 30.f);
	pObj->Set_Angle(fAngle);
	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER_BULLET);
}

#include "stdafx.h"
#include "MarionBombEffect.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "MarionEffect.h"


CMarionBombEffect::CMarionBombEffect()
	: m_fDestX(0.f), m_fDestY(0.f), m_dwEffectCreateTime(0), m_dwEffectCreateDelay(100)
{
}


CMarionBombEffect::~CMarionBombEffect()
{
	Release();
}

void CMarionBombEffect::Initialize()
{
	// 이미지 크기 초기화
	m_tInfo.iCX = MARION_GUIDED_BULLET_WIDTH * 3;
	m_tInfo.iCY = MARION_GUIDED_BULLET_HEIGHT * 3;
	m_iImageWidth = MARION_GUIDED_BULLET_WIDTH;
	m_iImageHeight = MARION_GUIDED_BULLET_HEIGHT;
	// 이동 속도
	m_fSpeed = 10.f;
}

// 업데이트
int CMarionBombEffect::Update()
{
	// 삭제
	if (m_bRemove)
		return OBJ_DEAD;

	// 이펙트 딜레이 시간이 지나면 플래그 true
	if (!m_bOnEffect && m_dwEffectTime + m_dwEffectDelay > GetTickCount())
		return OBJ_NOEVENT;
	else
		m_bOnEffect = true;

	// 회전 이동
	if (Move()) {
		// 도착하면 삭제
		m_bRemove = true;
	}
	// 각도에 따른 이미지 변경
	Angle();

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CMarionBombEffect::Late_Update()
{
	// 이 오브젝트가 Update에서 생성되서 Update를 건너 뛰고 Late_Update를 하는 경우가 발생함
	// 그래서 Late_Udpate에서 갱신함
	// 이미지 RECT 정보 갱신
	Update_Rect();

	// 맵 바깥으로 나가면 삭제
	if (0 >= m_tRect.right || 0 >= m_tRect.bottom
		|| WINCX <= m_tRect.left || WINCY <= m_tRect.top)
		m_bRemove = true;
}

// 렌더
void CMarionBombEffect::Render(HDC _DC)
{
	// 플래그가 true가 아니면 렌더를 하지 않음
	if (!m_bOnEffect)
		return;

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Marion_Guided_Bullet");

	GdiTransparentBlt(_DC
		, m_tInfo.fX, m_tInfo.fY
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_iDrawID * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CMarionBombEffect::Release()
{
}

// 회전 이동
bool CMarionBombEffect::Move()
{
	float fX = m_fDestX - m_tInfo.fX;		// 방향 좌표 X
	float fY = m_fDestY - m_tInfo.fY;		// 방향 좌표 Y
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

	// 이펙트 생성
	if (m_dwEffectCreateTime + m_dwEffectCreateDelay < GetTickCount())
	{
		m_dwEffectCreateTime = GetTickCount();
		CObj* pObj = CObjFactory<CMarionEffect>::Create(m_tInfo.fX, m_tRect.bottom);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
	}

	// 목적지에 도착했는지?
	if (fDia - fSpeed == 0)
		return true;

	return false;
}

// 각도에 따른 이미지 변경
void CMarionBombEffect::Angle()
{
	float fX = m_fDestX - m_tInfo.fX;		// 방향 좌표 X
	float fY = m_fDestY - m_tInfo.fY;		// 방향 좌표 Y
	float fDia = sqrtf(fX * fX + fY * fY);	// 거리
	// arccos(cos) = 라디안 값을 구함
	float fRad = acosf(fX / fDia);
	// 라디안 값으로 각도를 구함
	float fAngle = fRad * 180.f / PI;
	// 목적지의 Y 좌표가 나의 Y 좌표 보다 크면 부호 반전
	if (m_tInfo.fY < m_fDestY)
		fAngle *= -1.f;

	// 보정
	if (180 < fAngle) {
		fAngle = fAngle - 360;
	}
	else if (fAngle < -180) {
		fAngle = fAngle + 360;
	}

	// 각도에 따라 이미지 인덱스 번호 교체
	if (80.f < fAngle && fAngle < 100.f)
		m_iDrawID = CMarionBombEffect::A90;
	else if (60.f <= fAngle && fAngle <= 80.f)
		m_iDrawID = CMarionBombEffect::A70;
	else if (30.f <= fAngle && fAngle < 60.f)
		m_iDrawID = CMarionBombEffect::A45;
	else if (10.f <= fAngle && fAngle < 30.f)
		m_iDrawID = CMarionBombEffect::A20;
	else if (-10.f < fAngle && fAngle < 10.f)
		m_iDrawID = CMarionBombEffect::A0;
	else if (-30.f <= fAngle && fAngle <= -10.f)
		m_iDrawID = CMarionBombEffect::A_20;
	else if (-60.f <= fAngle && fAngle < -30.f)
		m_iDrawID = CMarionBombEffect::A_45;
	else if (-80.f <= fAngle && fAngle < -60.f)
		m_iDrawID = CMarionBombEffect::A_70;
	else if (-100.f < fAngle && fAngle < -80.f)
		m_iDrawID = CMarionBombEffect::A_90;
	else if (-120.f <= fAngle && fAngle <= -100.f)
		m_iDrawID = CMarionBombEffect::A_110;
	else if (-150.f <= fAngle && fAngle < -120.f)
		m_iDrawID = CMarionBombEffect::A_135;
	else if (-170.f <= fAngle && fAngle < -150.f)
		m_iDrawID = CMarionBombEffect::A_160;
	else if (170.f < fAngle && fAngle < -170.f)
		m_iDrawID = CMarionBombEffect::A180;
	else if (150.f <= fAngle && fAngle <= 170.f)
		m_iDrawID = CMarionBombEffect::A160;
	else if (120.f <= fAngle && fAngle < 150.f)
		m_iDrawID = CMarionBombEffect::A135;
	else if (100.f <= fAngle && fAngle < 120.f)
		m_iDrawID = CMarionBombEffect::A110;
}

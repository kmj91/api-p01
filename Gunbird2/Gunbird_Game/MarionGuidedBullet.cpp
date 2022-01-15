// 기명준
// 마리온 유도 미사일

#include "stdafx.h"
#include "MarionGuidedBullet.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

#include "BulletHit.h"
#include "MarionEffect.h"


CMarionGuidedBullet::CMarionGuidedBullet()
	: m_dwEffectCreateTime(0), m_dwEffectCreateDelay(100)
	, m_stpSubBulletCnt(nullptr)
{
}


CMarionGuidedBullet::~CMarionGuidedBullet()
{
	Release();
}

void CMarionGuidedBullet::Initialize()
{
	// 이미지 위치 및 크기 초기화
	m_tInfo.iCX = MARION_GUIDED_BULLET_WIDTH * 3;
	m_tInfo.iCY = MARION_GUIDED_BULLET_HEIGHT * 3;
	m_tHitRectPos = { 0, 0, MARION_GUIDED_BULLET_WIDTH * 3, MARION_GUIDED_BULLET_HEIGHT * 3 };
	m_iImageWidth = MARION_GUIDED_BULLET_WIDTH;
	m_iImageHeight = MARION_GUIDED_BULLET_HEIGHT;
	// 이동 속도
	m_fSpeed = 16.f;
	// 데미지
	m_fDamage = 2.f;
}

// 업데이트
int CMarionGuidedBullet::Update()
{
	// 삭제
	if (m_bRemove)
	{
		--m_stpSubBulletCnt->count;
		if (m_stpSubBulletCnt->count == 0)		// 화면에서 총알 객체가 모두 사라져야 생성할 수 있음
		{
			m_stpSubBulletCnt->flag = false;
		}
		return OBJ_DEAD;
	}	
	// 충돌
	if (m_bDead)
	{
		--m_stpSubBulletCnt->count;
		if (m_stpSubBulletCnt->count == 0)		// 화면에서 총알 객체가 모두 사라져야 생성할 수 있음
		{
			m_stpSubBulletCnt->flag = false;
		}

		CObj* pObj = CAbstractFactory<CBulletHit>::Create(m_tInfo.fX, m_tInfo.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		return OBJ_DEAD;
	}
		
	// 이동
	m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * m_fSpeed;
	m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * m_fSpeed;

	// 이펙트 생성
	if (m_dwEffectCreateTime + m_dwEffectCreateDelay < GetTickCount())
	{
		m_dwEffectCreateTime = GetTickCount();
		CObj* pObj = CAbstractFactory<CMarionEffect>::Create(m_tInfo.fX, m_tRect.bottom);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);
	}

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CMarionGuidedBullet::Late_Update()
{
	// 이 오브젝트가 Update에서 생성되서 Update를 건너 뛰고 Late_Update를 하는 경우가 발생함
	// 그래서 Late_Udpate에서 갱신함
	// 이미지 RECT 및 Hit RECT 정보 갱신
	Update_Rect();

	// 맵 바깥으로 나가면 삭제
	if (0 >= m_tRect.right || 0 >= m_tRect.bottom
		|| WINCX <= m_tRect.left || WINCY <= m_tRect.top)
		m_bRemove = true;
}

// 렌더
void CMarionGuidedBullet::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Marion_Guided_Bullet");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_iDrawID * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));

	// 만약 옵션에서 충돌 박스 보기를 켰다면 (넘버패드 1번 키)
	// 충돌 박스도 렌더 해줘야함
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
}

void CMarionGuidedBullet::Release()
{
}

// 각도 초기화
// 타겟과의 각도를 구해서 이미지 인덱스를 교체함
void CMarionGuidedBullet::Init_Angle()
{
	// 타겟이 있으면 해당 타겟을 목표로
	// 없으면 디폴트 각도로 날아감
	if (m_pTarget)
	{
		float fX = m_pTarget->Get_Info().fX - m_tInfo.fX;	// 방향 좌표 X
		float fY = m_pTarget->Get_Info().fY - m_tInfo.fY;	// 방향 좌표 Y
		float fDia = sqrtf(fX * fX + fY * fY);				// 거리
		// arccos(cos) = 라디안 값을 구함
		float fRad = acosf(fX / fDia);
		// 라디안 값으로 호도각을 구함
		m_fAngle = fRad * 180.f / PI;
		// 타겟의 Y 좌표가 나의 Y 좌표 보다 크면 부호 반전
		if (m_tInfo.fY < m_pTarget->Get_Info().fY)
			m_fAngle *= -1.f;
	}

	// 각도를 비교 이미지 인덱스를 교체
	if (80.f < m_fAngle && m_fAngle < 100.f)
		m_iDrawID = CMarionGuidedBullet::A90;
	else if (60.f <= m_fAngle && m_fAngle <= 80.f)
		m_iDrawID = CMarionGuidedBullet::A70;
	else if (30.f <= m_fAngle && m_fAngle < 60.f)
		m_iDrawID = CMarionGuidedBullet::A45;
	else if (10.f <= m_fAngle && m_fAngle < 30.f)
		m_iDrawID = CMarionGuidedBullet::A20;
	else if (-10.f < m_fAngle && m_fAngle < 10.f)
		m_iDrawID = CMarionGuidedBullet::A0;
	else if (-30.f <= m_fAngle && m_fAngle <= -10.f)
		m_iDrawID = CMarionGuidedBullet::A_20;
	else if (-60.f <= m_fAngle && m_fAngle < -30.f)
		m_iDrawID = CMarionGuidedBullet::A_45;
	else if (-80.f <= m_fAngle && m_fAngle < -60.f)
		m_iDrawID = CMarionGuidedBullet::A_70;
	else if (-100.f < m_fAngle && m_fAngle < -80.f)
		m_iDrawID = CMarionGuidedBullet::A_90;
	else if (-120.f <= m_fAngle && m_fAngle <= -100.f)
		m_iDrawID = CMarionGuidedBullet::A_110;
	else if (-150.f <= m_fAngle && m_fAngle < -120.f)
		m_iDrawID = CMarionGuidedBullet::A_135;
	else if (-170.f <= m_fAngle && m_fAngle < -150.f)
		m_iDrawID = CMarionGuidedBullet::A_160;
	else if (170.f < m_fAngle && m_fAngle < -170.f)
		m_iDrawID = CMarionGuidedBullet::A180;
	else if (150.f <= m_fAngle && m_fAngle <= 170.f)
		m_iDrawID = CMarionGuidedBullet::A160;
	else if (120.f <= m_fAngle && m_fAngle < 150.f)
		m_iDrawID = CMarionGuidedBullet::A135;
	else if (100.f <= m_fAngle && m_fAngle < 120.f)
		m_iDrawID = CMarionGuidedBullet::A110;
}

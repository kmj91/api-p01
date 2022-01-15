// 기명준
// 마리온 충전 공격 총알

#include "stdafx.h"
#include "MarionChargeBullet.h"

#include "BmpMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

#include "MarionChargeHitEffect.h"


CMarionChargeBullet::CMarionChargeBullet() : m_ePreState(CMarionChargeBullet::END), m_eCurState(CMarionChargeBullet::END)
{
}


CMarionChargeBullet::~CMarionChargeBullet()
{
	Release();
}

void CMarionChargeBullet::Initialize()
{
	// 이미지 위치 및 크기 초기화
	m_tInfo.iCX = MARION_CHARGE_BULLET_WIDTH * 3;
	m_tInfo.iCY = MARION_CHARGE_BULLET_HEIGHT * 3;
	m_tHitRectPos = { 2 * 3, 2 * 3, 34 * 3, 36 * 3 };
	m_iImageWidth = MARION_CHARGE_BULLET_WIDTH;
	m_iImageHeight = MARION_CHARGE_BULLET_HEIGHT;
	// 이동 속도
	m_fSpeed = 1.f;
	// 체력(명중하면서 적 체력만큼 총알 체력도 차감 0이되면 소멸)
	m_fHp = 50.f;
	// 데미지
	m_fDamage = 500.f;
	// 기본 상태
	m_eCurState = CMarionChargeBullet::START;
}

// 업데이트
int CMarionChargeBullet::Update()
{
	// 삭제
	if (m_bRemove)
		return OBJ_DEAD;
	// 명중
	if (m_bDead)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_CHARGE_BULLET_HIT);
		CSoundMgr::Get_Instance()->PlaySound(L"Marion_ChargeBulletHit.wav", CSoundMgr::PLAYER_CHARGE_BULLET_HIT);

		CObj* pObj = CAbstractFactory<CMarionChargeHitEffect>::Create(m_tInfo.fX, m_tInfo.fY - 100.f);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::EFFECT);

		return OBJ_DEAD;
	}

	// 이동
	m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * m_fSpeed;
	m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * m_fSpeed;

	// 처음에는 작은데 점점 커지면서
	// 일정 크기 이후 이미지 프레임을 반복 이동속도 증가
	if (m_ePreState == CMarionChargeBullet::START) {
		if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		{
			m_eCurState = CMarionChargeBullet::REPEAT;
			m_fSpeed = 10.f;
		}
	}

	// 프레임 씬 변경 처리
	Scene_Change();
	// 이미지 프레임 이동
	Frame_Move();

	return OBJ_NOEVENT;
}

// 레이트 업데이트
void CMarionChargeBullet::Late_Update()
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
void CMarionChargeBullet::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Marion_Charge_Bullet");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));

	// 만약 옵션에서 충돌 박스 보기를 켰다면 (넘버패드 1번 키)
	// 충돌 박스도 렌더 해줘야함
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tHitRect.left, m_tHitRect.top, m_tHitRect.right, m_tHitRect.bottom);
}

void CMarionChargeBullet::Release()
{
}

// 프레임 씬 변경 처리
void CMarionChargeBullet::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CMarionChargeBullet::START:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 40;
			break;
		case CMarionChargeBullet::REPEAT:
			m_tFrame.iFrameCnt = 0;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 40;
			break;
		}
		m_ePreState = m_eCurState;
	}
}

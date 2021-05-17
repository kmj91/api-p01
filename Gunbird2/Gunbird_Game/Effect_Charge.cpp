#include "stdafx.h"
#include "Effect_Charge.h"

#include "BmpMgr.h"


CEffect_Charge::CEffect_Charge()
{
}


CEffect_Charge::~CEffect_Charge()
{
	Release();
}

void CEffect_Charge::Initialize()
{
	m_tInfo.iCX = EFFECT_CHARGE_WIDTH * 3;
	m_tInfo.iCY = EFFECT_CHARGE_HEIGHT * 3;
	m_iImageWidth = EFFECT_CHARGE_WIDTH;
	m_iImageHeight = EFFECT_CHARGE_HEIGHT;

	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 32;
	m_tFrame.iFrameScene = rand() % 2;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 30;
}

int CEffect_Charge::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_dwEffectTime + m_dwEffectDelay > GetTickCount())		// ����Ʈ ������
		return OBJ_NOEVENT;
	else
	{
		if (!m_bOnEffect)
		{
			m_bOnEffect = true;
		}
	}

	// ��� ��ǥ�� ��ġ�� ����
	m_tInfo.fX = m_pTarget->Get_Info().fX + m_fRelativePosX;
	m_tInfo.fY = m_pTarget->Get_Info().fY + m_fRelativePosY;

	Update_Rect();
	Frame_Move();

	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)		// �������� ���̸� �����
		m_bDead = true;

	return OBJ_NOEVENT;
}

void CEffect_Charge::Late_Update()
{
}

void CEffect_Charge::Render(HDC _DC)
{
	if (!m_bOnEffect)
		return;

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Effect_Charge");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));
}

void CEffect_Charge::Release()
{
}

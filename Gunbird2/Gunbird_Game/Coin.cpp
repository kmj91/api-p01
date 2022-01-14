// �����
// ���� �ı��� �� ����ϴ� ���� ������
// �� ��ũ�Ѱ� ���� �ӵ��� �̵� �� (���� ������Ʈ ���)
// �浹�ϸ� ���� ȹ��

#include "stdafx.h"
#include "Coin.h"

#include "BmpMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

#include "UI_ScoreText.h"


CCoin::CCoin()
{
}


CCoin::~CCoin()
{
	Release();
}

void CCoin::Initialize()
{
	// �̹��� ũ�� �� Hit RECT �ʱ�ȭ
	m_tInfo.iCX = COIN_WIDTH * 3;
	m_tInfo.iCY = COIN_HEIGHT * 3;
	m_tHitRectPos = { 0, 0, COIN_WIDTH * 3, COIN_HEIGHT * 3 };
	m_iImageWidth = COIN_WIDTH;
	m_iImageHeight = COIN_HEIGHT;
	// �̹��� ������ �ʱ�ȭ
	m_tFrame.iFrameCnt = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 80;
	// ������ Ÿ�� �ʱ�ȭ
	m_enItemType = ITEM::SCORE;
	// ����
	m_iScore = 500;
}

// ������Ʈ
int CCoin::Update()
{
	// ����
	if (m_bRemove)
		return OBJ_DEAD;

	// �÷��̾�� �浹�ؼ� ȹ��� ��Ȳ
	if (m_bDead) {
		// ���� �ؽ�Ʈ ���
		CObj* pObj = CAbstractFactory<CUI_ScoreText>::Create(m_tInfo.fX, m_tInfo.fY);
		pObj->Set_SceneID(1);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::UI);

		// ȹ�� ����
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ITEM);
		CSoundMgr::Get_Instance()->PlaySound(L"Item_Coin.wav", CSoundMgr::ITEM);

		return OBJ_DEAD;
	}

	// ��濡 ���缭 Y�� �̵�
	m_tInfo.fY += g_fBackgroundSpeed * 3.f;
	// �̹��� ������ �̵�
	Frame_Move();

	return OBJ_NOEVENT;
}

// ����Ʈ ������Ʈ
void CCoin::Late_Update()
{
	// �� ������Ʈ�� Update���� �����Ǽ� Update�� �ǳ� �ٰ� Late_Update�� �ϴ� ��찡 �߻���
	// �׷��� Late_Udpate���� ������
	// �̹��� RECT ���� �� Hit RECT ���� ����
	Update_Rect();

	if (0 >= m_tRect.right || 0 >= m_tRect.bottom
		|| WINCX <= m_tRect.left || WINCY <= m_tRect.top)
		m_bRemove = true;
}

// ����
void CCoin::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Coin");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameCnt * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));

	// ���� �ɼǿ��� �浹 �ڽ� ���⸦ �״ٸ� (�ѹ��е� 1�� Ű)
	// �浹 �ڽ��� ���� �������
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CCoin::Release()
{
}

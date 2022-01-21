// �����
// ��ź ���� �� �Ѿ��� ��ȯ�� ���� ������Ʈ
// ���������� �������ٰ� �� �ϴ����� �� ������
// �浹�ϸ� ���� ȹ��

#include "stdafx.h"
#include "Candy.h"

#include "BmpMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

#include "UI_ScoreText.h"


CCandy::CCandy() : m_fForce(0.f)
{
}


CCandy::~CCandy()
{
	Release();
}

void CCandy::Initialize()
{
	// �̹��� ũ�� �� Hit RECT �ʱ�ȭ
	m_tInfo.iCX = CANDY_WIDTH * 3;
	m_tInfo.iCY = CANDY_HEIGHT * 3;
	m_tHitRectPos = { 0, 0, CANDY_WIDTH * 3, CANDY_HEIGHT * 3 };
	m_iImageWidth = CANDY_WIDTH;
	m_iImageHeight = CANDY_HEIGHT;
	// ���� ������ ���� ���� ����
	m_tFrame.iFrameScene = rand() % 3;
	m_iDrawID = rand() % 22;
	// ���� ����
	m_fAngle = (float)(90 + (rand() % 20 - 10));
	// �̵� �ӵ�
	m_fSpeed = 3.f;
	// ������ Ÿ�� �ʱ�ȭ
	m_enItemType = ITEM::SCORE;
	// ����
	m_iScore = 200;
}

// ������Ʈ
int CCandy::Update()
{
	// ����
	if (m_bRemove)
		return OBJ_DEAD;

	if (m_bDead) {
		// ���� �ؽ�Ʈ ���
		CObj* pObj = CObjFactory<CUI_ScoreText>::Create(m_tInfo.fX, m_tInfo.fY);
		pObj->Set_SceneID(0);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::UI);

		// ȹ�� ����
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ITEM);
		CSoundMgr::Get_Instance()->PlaySound(L"Item_Candy.wav", CSoundMgr::ITEM);

		return OBJ_DEAD;
	}

	// 10���� ������ ���������� ����
	if (m_fForce < 10.f)
		m_fForce += 0.05f;

	// �̵�
	// �ӵ� ���� +��ȣ���� -�� ���ϸ鼭 ���� ������ �� �ϴ����� ������
	m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * (m_fSpeed - m_fForce);
	m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * (m_fSpeed - m_fForce);

	return OBJ_NOEVENT;
}

// ����Ʈ ������Ʈ
void CCandy::Late_Update()
{
	// �� ������Ʈ�� Update���� �����Ǽ� Update�� �ǳ� �ٰ� Late_Update�� �ϴ� ��찡 �߻���
	// �׷��� Late_Udpate���� ������
	// �̹��� RECT ���� �� Hit RECT ���� ����
	Update_Rect();

	// �� �ٱ����� ������ ����
	if (0 >= m_tRect.right || 0 >= m_tRect.bottom
		|| WINCX <= m_tRect.left || WINCY <= m_tRect.top)
		m_bRemove = true;
}

// ����
void CCandy::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Candy");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_iDrawID * m_iImageWidth, m_tFrame.iFrameScene * m_iImageHeight
		, m_iImageWidth, m_iImageHeight
		, RGB(255, 0, 255));

	// ���� �ɼǿ��� �浹 �ڽ� ���⸦ �״ٸ� (�ѹ��е� 1�� Ű)
	// �浹 �ڽ��� ���� �������
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CCandy::Release()
{
}

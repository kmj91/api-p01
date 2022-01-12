// �����
// NPC �̵��� ���� ������ �ʴ� �簢 ����
// �̵� ��ǥ�� ����

#include "stdafx.h"
#include "Sector.h"


CSector::CSector() : m_fDestPosX(0.f), m_fDestPosY(0.f)
{
}


CSector::~CSector()
{
	Release();
}

void CSector::Initialize()
{
}

// ������Ʈ
int CSector::Update()
{
	// ����
	if (m_bDead)
		return OBJ_DEAD;

	// ��� ��ũ���� ���� ���°� �ƴϸ�
	if (!g_bStopMapScroll)
		m_tInfo.fY += g_fBackgroundSpeed * 3.f;			// ��濡 ���缭 Y�� �̵�

	// ��ġ RECT ���� �� Hit RECT ���� ����
	Update_Rect();

	return OBJ_NOEVENT;
}

// ����Ʈ ������Ʈ
void CSector::Late_Update()
{
	// �� �ϴ� ������ ������ ����
	if (WINCY <= m_tRect.top)
		m_bDead = true;
}

// ����
void CSector::Render(HDC _DC)
{
	// ���� �ɼǿ��� �浹 �ڽ� ���⸦ �״ٸ� (�ѹ��е� 1�� Ű)
	// �浹 �ڽ��� ���� �������
	if (!g_bHitRectRender) {
		return;
	}
	Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CSector::Release()
{
}

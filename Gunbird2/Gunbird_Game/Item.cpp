// 기명준
// 아이템 인터페이스
// 아이템 종류를 구분하는 것과 점수 값을 멤버 변수로 가지고 있음

#include "stdafx.h"
#include "Item.h"


CItem::CItem()
	: m_enItemType(ITEM::END), m_iScore(0)
{
}


CItem::~CItem()
{
}

void CItem::Initialize()
{
}

int CItem::Update()
{
	return 0;
}

void CItem::Late_Update()
{
}

void CItem::Render(HDC _DC)
{
}

void CItem::Release()
{
}

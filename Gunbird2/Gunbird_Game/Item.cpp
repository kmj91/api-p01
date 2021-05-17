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

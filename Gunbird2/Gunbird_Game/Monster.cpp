#include "stdafx.h"
#include "Monster.h"


CMonster::CMonster() : m_iScore(0)
{
}


CMonster::~CMonster()
{
	Release();
}

void CMonster::Initialize()
{
}

int CMonster::Update()
{
	return OBJ_NOEVENT;
}

void CMonster::Late_Update()
{
}

void CMonster::Render(HDC _DC)
{
}

void CMonster::Release()
{
}

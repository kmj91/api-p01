// 기명준
// 몬스터 부모 클래스
// 플레이어가 점수를 얻을 때 필요한 멤버 변수를 공통으로 가지고있음

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

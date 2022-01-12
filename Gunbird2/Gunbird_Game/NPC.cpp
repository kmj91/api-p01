// 기명준
// NPC 부모 인터페이스
// NPC들은 이동하거나 가만히있거나 함 배경 취급

#include "stdafx.h"
#include "NPC.h"


CNPC::CNPC() : m_fDestPosX(0.f), m_fDestPosY(0.f)
{
}


CNPC::~CNPC()
{
}

void CNPC::Initialize()
{
}

int CNPC::Update()
{
	return 0;
}

void CNPC::Late_Update()
{
}

void CNPC::Render(HDC _DC)
{
}

void CNPC::Release()
{
}

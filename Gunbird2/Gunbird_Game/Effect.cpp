#include "stdafx.h"
#include "Effect.h"


CEffect::CEffect()
	: m_dwEffectTime(GetTickCount()), m_dwEffectDelay(0)
	, m_fRelativePosX(0.f), m_fRelativePosY(0.f)
	, m_bOnEffect(false)
{
}


CEffect::~CEffect()
{
	Release();
}

void CEffect::Initialize()
{
}

int CEffect::Update()
{
	return 0;
}

void CEffect::Late_Update()
{
}

void CEffect::Render(HDC _DC)
{
}

void CEffect::Release()
{
}

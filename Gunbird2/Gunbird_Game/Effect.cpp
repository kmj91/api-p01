// 기명준
// 이펙트 인터페이스
// 이펙트 생성 시간과 딜레이를 이용해서 지연해서 이펙트를 발생하게 하기도 함
// 상대 좌표를 가지고 타겟 + 상대좌표를 통해 위치 좌표를 생성하기도 함

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

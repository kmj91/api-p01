// 기명준
// 게임 엔딩 씬 CG 오브젝트
// 이미지 프레임 끝에 도달하면 텍스트 윈도우 생성

#pragma once

#ifndef __CG_KUSURI_H__
#define __CG_KUSURI_H__

#include "Obj.h"
class CCG_Kusuri :
	public CObj
{
public:
	CCG_Kusuri();
	virtual ~CCG_Kusuri();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	int		m_iBGFrameCnt;
	bool	m_bFlag;
};

#endif // !__CG_KUSURI_H__



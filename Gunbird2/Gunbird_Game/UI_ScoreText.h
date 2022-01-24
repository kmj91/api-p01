// 기명준
// 획득 점수 텍스트
// 점수 아이템 상호작용 때 뜨는 텍스트
// 고정 이미지 사용
// 생성하는 쪽에서 씬 인덱스를 초기화 받음

#pragma once

#ifndef __UI_SCORETEXT_H__
#define __UI_SCORETEXT_H__

#include "Obj.h"
class CUI_ScoreText :
	public CObj
{
public:
	CUI_ScoreText();
	virtual ~CUI_ScoreText();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	DWORD			m_dwTime;
	DWORD			m_dwDelay;
};

#endif // !__UI_SCORETEXT_H__



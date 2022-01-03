// 기명준
// 캐릭터 선택 표시 오브젝트
// 선택 포커스 조종 및 화면에서 선택한 캐릭터 표시

#pragma once

#ifndef __SELECTPLAYER_H__
#define __SELECTPLAYER_H__

#include "Obj.h"
class CPlayerSelect :
	public CObj
{
public:
	CPlayerSelect();
	virtual ~CPlayerSelect();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	void Key_Input(int _Key);			// 키 입력 처리
	PLAYER::NAME Get_State();			// 현재 선택한 캐릭터 번호 얻기

private:
	void CharacterChange();				// 캐릭터 교체

private:
	PLAYER::NAME		m_ePreCharacter;	// 이전 캐릭터 번호
	PLAYER::NAME		m_eCurCharacter;	// 교체할 캐릭터 번호
};

#endif // !__SELECTPLAYER_H__



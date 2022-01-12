// 기명준
// NPC 부모 인터페이스
// NPC들은 이동하거나 가만히있거나 함 배경 취급

#pragma once

#ifndef __NPC_H__
#define __NPC_H__

#include "Obj.h"
class CNPC abstract :
	public CObj
{
public:
	CNPC();
	virtual ~CNPC();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	void Set_DestPos(float _fX, float _fY) { m_fDestPosX = _fX; m_fDestPosY = _fY; }
	// 더함 대입이 아님 이는 화면상의 유닛을 좀더 컨트롤하기 쉽게 하기 위함임
	void Move_DestPos(float _fX, float _fY) { m_fDestPosX = m_tInfo.fX + _fX; m_fDestPosY = m_tInfo.fY + _fY; }

protected:
	float		m_fDestPosX;		// 이동 좌표 X	(외부에서 해당 좌표를 갱신 받는다)
	float		m_fDestPosY;		// 이동 좌표 Y
};

#endif // !__NPC_H__



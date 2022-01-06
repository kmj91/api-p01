// �����
// UI ����
// ���ٸ� ó������ �ʱ�ȭ ���� ������Ű�� ������ ��

#pragma once

#ifndef __UI_STATIC_H__
#define __UI_STATIC_H__

#include "Obj.h"
class CUI_Static final :
	public CObj
{
public:
	CUI_Static();
	virtual ~CUI_Static();

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__UI_STATIC_H__



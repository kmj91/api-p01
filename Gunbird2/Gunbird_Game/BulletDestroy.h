#pragma once

#ifndef __BULLETDESTROY_H__
#define __BULLETDESTROY_H__

#include "Effect.h"
class CBulletDestroy :
	public CEffect
{
public:
	CBulletDestroy();
	virtual ~CBulletDestroy();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	// ÃÑ¾Ë Á¾·ù
	void Set_BulletType(BULLETTYPE _eBullet);

public:
	// ÃÑ¾Ë Á¾·ù
	BULLETTYPE m_eBulletType;
};

#endif // !__BULLETDESTROY_H__



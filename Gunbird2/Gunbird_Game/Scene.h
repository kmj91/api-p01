#pragma once

#ifndef __SCENE_H__
#define __SCENE_H__

class CScene
{
public:
	CScene();
	virtual ~CScene();

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Late_update() = 0;
	virtual void Render(HDC _DC) = 0;
	virtual void Release() = 0;

protected:
	bool m_isPause;
};

#endif // !__SCENE_H__



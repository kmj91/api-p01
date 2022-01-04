// �����
// ��� ���� ������Ʈ���� �θ�

#pragma once

#ifndef __OBJ_H__
#define __OBJ_H__

class CObj abstract
{
public:
	explicit CObj();
	virtual ~CObj();

	virtual void Initialize() = 0;
	virtual int Update() = 0;
	virtual void Late_Update() = 0;
	virtual void Render(HDC _DC) = 0;
	virtual void Release() = 0;

	// ���� ����
	// �Ķ���� �� ��ŭ ü�� ����, ü���� 0 ���ϰ��Ǹ� m_bDead�� true��
	// _fDamage : ���� ���ط�
	void Hit(float _fDamage);
	void Set_Pos(float _x, float _y) { m_tInfo.fX = _x; m_tInfo.fY = _y; }
	void Set_PosX(float _x) { m_tInfo.fX += _x; }
	void Set_PosY(float _y) { m_tInfo.fY += _y; }
	void Set_Dead() { m_bDead = true; }
	void Set_Remove() { m_bRemove = true; }
	void Set_Angle(float _fAngle) { m_fAngle = _fAngle; }
	void Set_Target(CObj* _pTarget) { m_pTarget = _pTarget; }
	void Set_FrameKey(TCHAR* _pFrameKey) { m_pFrameKey = _pFrameKey; }
	void Set_DrawID(int _iDrawID) { m_iDrawID = _iDrawID; }
	void Set_SceneID(int _iSceneID) { m_tFrame.iFrameScene = _iSceneID; }
	void Set_ImageSize(int _iWidth, int _iHeight) { m_iImageWidth = _iWidth; m_iImageHeight = _iHeight; m_tInfo.iCX = _iWidth * 3; m_tInfo.iCY = _iHeight * 3; }
	void Set_Size(int _iWidth, int _iHeight) { m_tInfo.iCX = _iWidth; m_tInfo.iCY = _iHeight; }		// ���� �̹��� ũ�⿡�� 3�� ���� ������ (���ӿ����� ũ��)
	void Set_HitRectPos(RECT& _tHitRectPos) { m_tHitRectPos = _tHitRectPos; }
	void Set_Hp(float _fHp) { m_fHp = _fHp; }
	void Set_Speed(float _fSpeed) { m_fSpeed = _fSpeed; }

	const INFO& Get_Info() const { return m_tInfo; }
	const RECT& Get_Rect() const { return m_tRect; }
	const RECT& Get_HitRect() const { return m_tHitRect; }
	int Get_DrawID() const { return m_iDrawID; }
	float Get_Hp() const { return m_fHp; }
	bool isHpLock() { return m_bHpLock; }
	bool isDead() { return m_bDead; }

protected:
	// ���� ������Ʈ ��ǥ�� �������� �̹��� ��Ʈ ���� ����
	void Update_Rect();
	// �̹��� ������ �̵�
	void Frame_Move();

protected:
	INFO				m_tInfo;		// ������Ʈ ��ǥ, ũ�� ����
	RECT				m_tRect;		// ������Ʈ �̹��� ��Ʈ ����
	RECT				m_tHitRectPos;	// ������Ʈ �̹��� ���� ��Ʈ�ڽ� ��ġ
	RECT				m_tHitRect;		// ������Ʈ ��ġ ���� ������ ��Ʈ�ڽ�
	FRAME				m_tFrame;		// �̹��� ������ ����
	float				m_fSpeed;		// �ӵ�
	float				m_fAngle;		// ����
	CObj*				m_pTarget;		// Ÿ��
	TCHAR*				m_pFrameKey;	// �̹��� ������ Ű
	int					m_iDrawID;		// �׷����� �̹��� ��ȣ
	int					m_iImageWidth;	// �̹��� ���� ����
	int					m_iImageHeight;	// �̹��� ���� ����
	float				m_fHp;			// ü��
	bool				m_bDead;		// ����
	bool				m_bHpLock;		// ü���� �������� ����
	bool				m_bRemove;		// ����
};

#endif // !__OBJ_H__



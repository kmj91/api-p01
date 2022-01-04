// 기명준
// 모든 게임 오브젝트들의 부모

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

	// 공격 받음
	// 파라미터 값 만큼 체력 차감, 체력이 0 이하가되면 m_bDead가 true로
	// _fDamage : 받은 피해량
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
	void Set_Size(int _iWidth, int _iHeight) { m_tInfo.iCX = _iWidth; m_tInfo.iCY = _iHeight; }		// 실제 이미지 크기에서 3배 곱한 사이즈 (게임에서의 크기)
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
	// 현재 오브젝트 좌표를 기준으로 이미지 렉트 정보 갱신
	void Update_Rect();
	// 이미지 프레임 이동
	void Frame_Move();

protected:
	INFO				m_tInfo;		// 오브젝트 좌표, 크기 정보
	RECT				m_tRect;		// 오브젝트 이미지 렉트 정보
	RECT				m_tHitRectPos;	// 오브젝트 이미지 기준 히트박스 위치
	RECT				m_tHitRect;		// 오브젝트 위치 값을 적용한 히트박스
	FRAME				m_tFrame;		// 이미지 프레임 정보
	float				m_fSpeed;		// 속도
	float				m_fAngle;		// 각도
	CObj*				m_pTarget;		// 타겟
	TCHAR*				m_pFrameKey;	// 이미지 프레임 키
	int					m_iDrawID;		// 그려야할 이미지 번호
	int					m_iImageWidth;	// 이미지 가로 길이
	int					m_iImageHeight;	// 이미지 세로 길이
	float				m_fHp;			// 체력
	bool				m_bDead;		// 죽음
	bool				m_bHpLock;		// 체력이 떨어지지 않음
	bool				m_bRemove;		// 삭제
};

#endif // !__OBJ_H__



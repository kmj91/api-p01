#pragma once
#ifndef __OBJFACTORY_H__
#define __OBJFACTORY_H__

class CObj;

template <typename T>
class CObjFactory
{
public:
	static T* Create()
	{
		T* pObj = new T;
		pObj->Initialize();

		return pObj;
	}
	static T* Create(float _x, float _y)
	{
		T* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_x, _y);

		return pObj;
	}
	static T* Create(float _x, float _y, float _Angle)
	{
		T* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_x, _y);
		pObj->Set_Angle(_Angle);

		return pObj;
	}
	static T* Create(float _x, float _y, BULLETTYPE _eBullet)
	{
		T* pObj = new T;
		pObj->Set_Pos(_x, _y);
		pObj->Set_BulletType(_eBullet);

		return pObj;
	}
	static T* Create(CObj* _pTarget)
	{
		T* pObj = new T;
		pObj->Initialize();
		pObj->Set_Target(_pTarget);

		return pObj;
	}
	static T* Create_Air(CREATE_OBJECT* _pObjectInfo)
	{
		T* pObj = new T;
		pObj->Initialize();
		// ������ ��ġ
		pObj->Set_Pos(_pObjectInfo->fPosX, _pObjectInfo->fPosY);
		// Ÿ��
		if (g_pPlayer_1)
			pObj->Set_Target(g_pPlayer_1);
		else
			pObj->Set_Target(g_pPlayer_2);
		static_cast<T*>(pObj)->Set_DestPos(_pObjectInfo->fDestPosX, _pObjectInfo->fDestPosY);	// ������
		static_cast<T*>(pObj)->Set_OutPos(_pObjectInfo->fOutPosX, _pObjectInfo->fOutPosY);	// ���� ��ġ
		static_cast<T*>(pObj)->Set_OutTime(_pObjectInfo->dwOutTime);		// ȭ�鿡 ���̴� �ð�	
		static_cast<T*>(pObj)->Set_DestAngle(_pObjectInfo->fDestAngle);	// ��ġ ��ȸ ����
		static_cast<T*>(pObj)->Set_OutAngle(_pObjectInfo->fOutAngle);	// ���� ��ȸ ����

		return pObj;
	}
	static T* Create_DestY(CREATE_OBJECT* _pObjectInfo)
	{
		T* pObj = new T;
		pObj->Initialize();
		// ������ ��ġ
		pObj->Set_Pos(_pObjectInfo->fPosX, _pObjectInfo->fPosY);
		// Ÿ��
		if (g_pPlayer_1)				
			pObj->Set_Target(g_pPlayer_1);
		else
			pObj->Set_Target(g_pPlayer_2);
		// ������ Y ��ǥ
		static_cast<T*>(pObj)->Set_DestPos(_pObjectInfo->fDestPosY);

		return pObj;
	}
	static T* Create_Dest(CREATE_OBJECT* _pObjectInfo)
	{
		T* pObj = new T;
		pObj->Initialize();
		// ������ ��ġ
		pObj->Set_Pos(_pObjectInfo->fPosX, _pObjectInfo->fPosY);
		// Ÿ��
		if (g_pPlayer_1)
			pObj->Set_Target(g_pPlayer_1);
		else
			pObj->Set_Target(g_pPlayer_2);
		// ������ X, Y ��ǥ
		static_cast<T*>(pObj)->Set_DestPos(_pObjectInfo->fDestPosX, _pObjectInfo->fDestPosY);

		return pObj;
	}
	static T* Create_Target(CREATE_OBJECT* _pObjectInfo)
	{
		T* pObj = new T;
		pObj->Initialize();
		// ������ ��ġ
		pObj->Set_Pos(_pObjectInfo->fPosX, _pObjectInfo->fPosY);
		// Ÿ��
		if (g_pPlayer_1)
			pObj->Set_Target(g_pPlayer_1);
		else
			pObj->Set_Target(g_pPlayer_2);

		return pObj;
	}
	static T* Create_DestOut(CREATE_OBJECT* _pObjectInfo)
	{
		T* pObj = new T;
		pObj->Initialize();
		// ������ ��ġ
		pObj->Set_Pos(_pObjectInfo->fPosX, _pObjectInfo->fPosY);
		static_cast<T*>(pObj)->Set_DestPos(_pObjectInfo->fDestPosX, _pObjectInfo->fDestPosY);	// ������
		static_cast<T*>(pObj)->Set_OutPos(_pObjectInfo->fOutPosX, _pObjectInfo->fOutPosY);		// ���� ��ġ

		return pObj;
	}
	static T* Create_StaticObj(CREATE_OBJECT* _pObjectInfo, WCHAR* _FrameKey, int _Width, int _Height)
	{
		T* pObj = new T;
		pObj->Initialize();
		// ������ ��ġ
		pObj->Set_Pos(_pObjectInfo->fPosX, _pObjectInfo->fPosY);
		pObj->Set_FrameKey(_FrameKey);			// �̹��� ������ Ű
		pObj->Set_ImageSize(_Width, _Height);	// �̹��� ������

		return pObj;
	}
	static T* Create_BrokenStatic(CREATE_OBJECT* _pObjectInfo, WCHAR* _FrameKey, int _Width, int _Height, RECT _Hit, float _Hp)
	{
		T* pObj = new T;
		pObj->Initialize();
		// ������ ��ġ
		pObj->Set_Pos(_pObjectInfo->fPosX, _pObjectInfo->fPosY);
		pObj->Set_FrameKey(_FrameKey);			// �̹��� ������ Ű
		pObj->Set_ImageSize(_Width, _Height);	// �̹��� ������
		pObj->Set_HitRectPos(_Hit);				// Hit REct ��ġ
		pObj->Set_Hp(_Hp);						// ü��

		return pObj;
	}
	static T* Create_MoveStatic(CREATE_OBJECT* _pObjectInfo, WCHAR* _FrameKey, int _Width, int _Height, float _Speed)
	{
		T* pObj = new T;
		pObj->Initialize();
		// ������ ��ġ
		pObj->Set_Pos(_pObjectInfo->fPosX, _pObjectInfo->fPosY);
		pObj->Set_FrameKey(_FrameKey);
		pObj->Set_ImageSize(_Width, _Height);
		pObj->Set_Speed(_Speed);
		static_cast<T*>(pObj)->Set_DestPos(_pObjectInfo->fDestPosX, _pObjectInfo->fDestPosY);	// ������

		return pObj;
	}
	static T* Create_Sector(CREATE_OBJECT* _pObjectInfo)
	{
		T* pObj = new T;
		pObj->Initialize();
		// ������ ��ġ
		pObj->Set_Pos(_pObjectInfo->fPosX, _pObjectInfo->fPosY);
		// ũ��
		pObj->Set_Size(_pObjectInfo->iWidth, _pObjectInfo->iHeight);
		// �̵� ��ġ
		static_cast<T*>(pObj)->Set_DestPos(_pObjectInfo->fDestPosX, _pObjectInfo->fDestPosY);

		return pObj;
	}
};

#endif // !__OBJFACTORY_H__

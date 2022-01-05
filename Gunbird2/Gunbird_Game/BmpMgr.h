// �����
// bmp �̹��� ���ҽ� �Ŵ���

#pragma once

#ifndef __BMPMGR_H__
#define __BMPMGR_H__

class CMyBitmap;
class CBmpMgr
{
private:
	CBmpMgr();
	~CBmpMgr();

public:
	// �̹��� ������ �ҷ��� HDC�� ����
	// _pFileName : ���� �ּ�
	// _pImagekey : map�� Ű ��
	void Insert_Bmp(const TCHAR* _pFileName, const TCHAR* _pImagekey);
	// �̹��� HDC ��ȯ
	// _pImagekey : �̹��� Ű ��
	// ��ȯ �� : �̹��� HDC
	HDC Find_Image(const TCHAR* _pImagekey);
	void Release();

public:
	static CBmpMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CBmpMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CBmpMgr*					m_pInstance;
	map<const TCHAR*, CMyBitmap*>	m_mapBit;
};


#endif // !__BMPMGR_H__

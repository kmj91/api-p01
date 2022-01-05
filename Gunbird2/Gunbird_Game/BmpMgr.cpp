// �����
// bmp �̹��� ���ҽ� �Ŵ���

#include "stdafx.h"
#include "BmpMgr.h"
#include "MyBitmap.h"

CBmpMgr* CBmpMgr::m_pInstance = nullptr;
CBmpMgr::CBmpMgr()
{
}


CBmpMgr::~CBmpMgr()
{
	Release();
}

// �̹��� ������ �ҷ��� HDC�� ����
// _pFileName : ���� �ּ�
// _pImagekey : map�� Ű ��
void CBmpMgr::Insert_Bmp(const TCHAR* _pFileName, const TCHAR* _pImagekey)
{
	// Ű ���� ����ϴ��� Ȯ��, ������ end�� ��ȯ
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), CStrCmp(_pImagekey));
	// ������ �ش� Ű ������ ����
	if (m_mapBit.end() == iter)
	{
		CMyBitmap* pBmp = new CMyBitmap;
		// �̹��� ���� �ҷ�����
		pBmp->Load_Bmp(_pFileName);
		m_mapBit.emplace(_pImagekey, pBmp);
	}
}

// �̹��� HDC ��ȯ
// _pImagekey : �̹��� Ű ��
// ��ȯ �� : �̹��� HDC
HDC CBmpMgr::Find_Image(const TCHAR* _pImagekey)
{
	// Ű ���� ����ϴ� �� Ȯ��, ������ end�� ��ȯ
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), CStrCmp(_pImagekey));
	// �ش� Ű ���� ����ϴ� �̹����� ����
	if (m_mapBit.end() == iter)
		return nullptr;
	// HDC ��ȯ
	return iter->second->Get_MemDC();
}

void CBmpMgr::Release()
{
	for (auto& iter : m_mapBit)
	{
		if (nullptr != iter.second)
			Safe_Delete(iter.second);
	}
	m_mapBit.clear();
}

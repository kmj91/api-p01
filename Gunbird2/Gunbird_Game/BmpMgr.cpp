// 기명준
// bmp 이미지 리소스 매니저

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

// 이미지 파일을 불러와 HDC로 저장
// _pFileName : 파일 주소
// _pImagekey : map의 키 값
void CBmpMgr::Insert_Bmp(const TCHAR* _pFileName, const TCHAR* _pImagekey)
{
	// 키 값을 사용하는지 확인, 없으면 end를 반환
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), CStrCmp(_pImagekey));
	// 없으면 해당 키 값으로 생성
	if (m_mapBit.end() == iter)
	{
		CMyBitmap* pBmp = new CMyBitmap;
		// 이미지 파일 불러오기
		pBmp->Load_Bmp(_pFileName);
		m_mapBit.emplace(_pImagekey, pBmp);
	}
}

// 이미지 HDC 반환
// _pImagekey : 이미지 키 값
// 반환 값 : 이미지 HDC
HDC CBmpMgr::Find_Image(const TCHAR* _pImagekey)
{
	// 키 값을 사용하는 지 확인, 없으면 end를 반환
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), CStrCmp(_pImagekey));
	// 해당 키 값을 사용하는 이미지가 없음
	if (m_mapBit.end() == iter)
		return nullptr;
	// HDC 반환
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

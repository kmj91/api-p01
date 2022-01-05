// 기명준
// bmp 이미지 리소스 매니저

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
	// 이미지 파일을 불러와 HDC로 저장
	// _pFileName : 파일 주소
	// _pImagekey : map의 키 값
	void Insert_Bmp(const TCHAR* _pFileName, const TCHAR* _pImagekey);
	// 이미지 HDC 반환
	// _pImagekey : 이미지 키 값
	// 반환 값 : 이미지 HDC
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

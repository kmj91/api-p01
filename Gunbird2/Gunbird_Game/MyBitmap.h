// 기명준
// bmp 이미지 리소스
// 불러온 bmp 이미지 파일의 정보를 HDC로 저장

#pragma once

#ifndef __MYBITMAP_H__
#define __MYBITMAP_H__


class CMyBitmap
{
public:
	CMyBitmap();
	~CMyBitmap();

public:
	// 이미지의 HDC 반환
	HDC Get_MemDC() { return m_hMemDC; }
	// bmp 이미지 파일 HDC로 저장
	// _pFileName : 이미지 파일 주소
	void Load_Bmp(const TCHAR* _pFileName);
	void Release();

private:
	HDC			m_hMemDC;
	HBITMAP		m_hBitmap;
	HBITMAP		m_hOldBmp;
};

#endif // !__MYBITMAP_H__

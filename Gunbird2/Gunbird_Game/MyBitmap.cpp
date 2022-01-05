// 기명준
// bmp 이미지 리소스
// 불러온 bmp 이미지 파일의 정보를 HDC로 저장

#include "stdafx.h"
#include "MyBitmap.h"


CMyBitmap::CMyBitmap()
{
}


CMyBitmap::~CMyBitmap()
{
	Release();
}

// bmp 이미지 파일 HDC로 저장
// _pFileName : 이미지 파일 주소
void CMyBitmap::Load_Bmp(const TCHAR * _pFileName)
{
	HDC hDC = GetDC(g_hWnd);
	m_hMemDC = CreateCompatibleDC(hDC);
	ReleaseDC(g_hWnd, hDC);

	m_hBitmap = (HBITMAP)LoadImage(NULL, _pFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
}

void CMyBitmap::Release()
{
	SelectObject(m_hMemDC, m_hOldBmp);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}

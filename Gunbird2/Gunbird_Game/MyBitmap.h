// �����
// bmp �̹��� ���ҽ�
// �ҷ��� bmp �̹��� ������ ������ HDC�� ����

#pragma once

#ifndef __MYBITMAP_H__
#define __MYBITMAP_H__


class CMyBitmap
{
public:
	CMyBitmap();
	~CMyBitmap();

public:
	// �̹����� HDC ��ȯ
	HDC Get_MemDC() { return m_hMemDC; }
	// bmp �̹��� ���� HDC�� ����
	// _pFileName : �̹��� ���� �ּ�
	void Load_Bmp(const TCHAR* _pFileName);
	void Release();

private:
	HDC			m_hMemDC;
	HBITMAP		m_hBitmap;
	HBITMAP		m_hOldBmp;
};

#endif // !__MYBITMAP_H__

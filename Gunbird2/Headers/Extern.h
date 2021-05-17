#pragma once

#ifndef __EXTERN_H__
#define __EXTERN_H__

class CPlayer;

extern HWND				g_hWnd;
extern PLAYER::NAME		g_ePlayer_1_Name;
extern PLAYER::NAME		g_ePlayer_2_Name;
extern CPlayer*			g_pPlayer_1;				// 1 �÷��̾� ���� ����
extern CPlayer*			g_pPlayer_2;				// 2 �÷��̾� ���� ����
extern int				g_iCredit;					// ũ����
extern float			g_fBackgroundSpeed;			// ��� ��ũ�� �ӵ�
extern bool				g_bStopMapScroll;			// ��� ��ũ�� ����
extern bool				g_bBossGO;					// ������ ������ �˸�
extern bool				g_bGameContinuePlayer_1;	// �÷��̾� 1 �ٽ��ϱ�
extern bool				g_bGameContinuePlayer_2;	// �÷��̾� 2 �ٽ��ϱ�
extern bool				g_bCountdownStop;			// ���� ����
extern BOOL				g_bActiveApp;				// ������ Ȱ��ȭ �÷���
extern bool				g_bGameOver;				// ���� ���� - Ÿ��Ʋ ȭ������ �� ��ȯ �÷���
extern bool				g_bHitRectRender;			// �浹 ��Ʈ ���� �÷���
extern bool				g_bStageClear;				// �������� Ŭ���� �÷���

#endif // !__EXTERN_H__

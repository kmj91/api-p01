#pragma once

#ifndef __EXTERN_H__
#define __EXTERN_H__

class CPlayer;

extern HWND				g_hWnd;
extern PLAYER::NAME		g_ePlayer_1_Name;
extern PLAYER::NAME		g_ePlayer_2_Name;
extern CPlayer*			g_pPlayer_1;				// 1 플레이어 전역 변수
extern CPlayer*			g_pPlayer_2;				// 2 플레이어 전역 변수
extern int				g_iCredit;					// 크레딧
extern float			g_fBackgroundSpeed;			// 배경 스크롤 속도
extern bool				g_bStopMapScroll;			// 배경 스크롤 정지
extern bool				g_bBossGO;					// 보스전 시작을 알림
extern bool				g_bGameContinuePlayer_1;	// 플레이어 1 다시하기
extern bool				g_bGameContinuePlayer_2;	// 플레이어 2 다시하기
extern bool				g_bCountdownStop;			// 게임 정지
extern BOOL				g_bActiveApp;				// 윈도우 활성화 플래그
extern bool				g_bGameOver;				// 게임 오버 - 타이틀 화면으로 씬 전환 플래그
extern bool				g_bHitRectRender;			// 충돌 렉트 랜더 플래그
extern bool				g_bStageClear;				// 스테이지 클리어 플래그

#endif // !__EXTERN_H__

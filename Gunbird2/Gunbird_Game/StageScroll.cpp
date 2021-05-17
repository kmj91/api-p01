#include "stdafx.h"
#include "Stage.h"

#include "KeyMgr.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "GameUIMgr.h"
#include "SoundMgr.h"

// 스테이지
#include "BG_Stage_1.h"
// 플레이어
#include "Marion.h"
// 몬스터
#include "Rocket.h"
#include "Blue.h"
#include "Red.h"
#include "MoverCannon.h"
#include "Cannon.h"
#include "CactusCannon.h"
#include "RobotBlue.h"
#include "RedBeanie.h"
#include "Factory.h"
#include "RobotShark.h"
// 스프라이트 출력용
#include "StaticObj.h"
#include "BrokenStatic.h"
#include "MoveStatic.h"
// NPC
#include "Shark.h"
#include "Gimmick.h"
#include "Blade.h"
// 섹터
#include "Sector.h"
// 보스
#include "TequilaGround.h"


void CStage::Load_Stage()
{
	// 배경 스크롤 속도 초기화
	g_fBackgroundSpeed = BACKGROUND_SPEED;
	// 보스전 시작 플래그 초기화
	g_bBossGO = false;
	// 스테이지 클리어 플래그 초기화
	g_bStageClear = false;

	switch (m_iStage)
	{
	case 1:
		Stage_1();
		break;
	}
}

void CStage::Key_Check()
{
	// 윈도우가 활성화 됬을 때만 키처리
	if (!g_bActiveApp) {
		return;
	}

	// 중지
	// 랜더를 제외한 모든 업데이트 중단
	if (CKeyMgr::Get_Instance()->Key_Down('P'))
	{
		m_isPause = !m_isPause;
	}

	// 퍼즈면 키처리 P 말고는 안함
	if (m_isPause)
		return;

	// 디버그용 히트 렉트
	if (CKeyMgr::Get_Instance()->Key_Down(VK_NUMPAD1))
	{
		g_bHitRectRender = !g_bHitRectRender;
	}

	// 1 플레이어 시작하기
	if (CKeyMgr::Get_Instance()->Key_Down('1'))
	{
		g_pPlayer_1->Key_Input(KEY::CONTINUE);
	}

	// 5 key 
	if (CKeyMgr::Get_Instance()->Key_Down('5'))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI);
		CSoundMgr::Get_Instance()->PlaySound(L"AddCredit.wav", CSoundMgr::UI);
		CGameUIMgr::Get_Instance()->Add_Credit(1);
	}

	// 게임 계속하기 카운트
	if (g_bCountdownStop)
		goto COUNTDOWN_STOP;

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		{
			g_pPlayer_1->Key_Input(KEY::LU);
		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		{
			g_pPlayer_1->Key_Input(KEY::LD);
		}
		else
			g_pPlayer_1->Key_Input(KEY::LL);
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		{
			g_pPlayer_1->Key_Input(KEY::RU);
		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		{
			g_pPlayer_1->Key_Input(KEY::RD);
		}
		else
			g_pPlayer_1->Key_Input(KEY::RR);
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
	{
		g_pPlayer_1->Key_Input(KEY::UU);
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
	{
		g_pPlayer_1->Key_Input(KEY::DD);
	}
	else
		g_pPlayer_1->Key_Input(KEY::NO_KEY);

	// 공격
	if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
	{
		g_pPlayer_1->Key_Input(KEY::ATK);
	}
	// 공격키에서 땜
	// 세미 오토 형식이라 이런 키값이 필요함
	else
	{
		g_pPlayer_1->Key_Input(KEY::ATK_END);
	}

	// 폭탄
	if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
	{
		g_pPlayer_1->Key_Input(KEY::BOMB);
	}

	// 근접 공격
	if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
	{
		g_pPlayer_1->Key_Input(KEY::MELEE);
	}

	return;


	// 게임 계속하기 카운트
COUNTDOWN_STOP:

	if (CKeyMgr::Get_Instance()->Key_Down('A'))
	{
		CGameUIMgr::Get_Instance()->NextMainCount();
	}

	return;
}

void CStage::Create_Object()
{
	switch (m_iStage)
	{
	case 1:
		Create_Object_Stage_1();
		break;
	}
}

void CStage::Stage_1()
{
	// 버퍼
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/BackBuffer.bmp", L"BackBuffer");
	
	// 스테이지
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stage/Stage1_Background.bmp", L"Stage1");

	// 맵 오브젝트
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stage/Stage1_Sign.bmp", L"Stage1_Sign");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stage/Stage1_Roof.bmp", L"Stage1_Roof");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stage/Stage1_Shop.bmp", L"Stage1_Shop");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stage/Stage1_Shop_Roof.bmp", L"Stage1_Shop_Roof");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stage/Stage1_BrokenSign.bmp", L"Stage1_BrokenSign");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stage/Stage1_RightRock.bmp", L"Stage1_RightRock");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stage/Stage1_LeftRock.bmp", L"Stage1_LeftRock");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stage/Stage1_Bridge.bmp", L"Stage1_Bridge");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stage/Stage1_Handrail.bmp", L"Stage1_Handrail");
	
	// 플레이어
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Marion/Marion_Move.bmp", L"Marion_Move");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Marion/Marion_Dead.bmp", L"Marion_Dead");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Marion/Marion_Charge.bmp", L"Marion_Charge");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Marion/Marion_Bomb.bmp", L"Marion_Bomb");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Marion/Marion_Melee.bmp", L"Marion_Melee");
	
	// 플레이어 총알
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Marion/Marion_Bullet_Power_1.bmp", L"Marion_Bullet_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Marion/Marion_Bullet_Power_2.bmp", L"Marion_Bullet_2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Marion/Marion_Bullet_Power_3.bmp", L"Marion_Bullet_3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Marion/Marion_Bullet_Power_4.bmp", L"Marion_Bullet_4");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Marion/Marion_Guided_Bullet.bmp", L"Marion_Guided_Bullet");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Marion/Marion_Charge_Bullet.bmp", L"Marion_Charge_Bullet");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Marion/Marion_Charge_Bullet_Hit.bmp", L"Marion_Charge_Bullet_Hit");
	
	// 적
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Rocket.bmp", L"Rocket");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Blue.bmp", L"Blue");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Red.bmp", L"Red");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/MoverCannon_1.bmp", L"MoverCannon_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/MoverCannon_2.bmp", L"MoverCannon_2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Cannon_1.bmp", L"Cannon_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Cannon_2.bmp", L"Cannon_2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Cactus.bmp", L"Cactus");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/RobotShark.bmp", L"RobotShark");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/RobotReg.bmp", L"RobotReg");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/RobotBody_1.bmp", L"RobotBody_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/RobotEtc.bmp", L"RobotEtc");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/RedBeanie_1.bmp", L"RedBeanie_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/RedBeanie_2.bmp", L"RedBeanie_2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Factory.bmp", L"Factory");

	// 적 총알
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/Bullet_1.bmp", L"Bullet_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/Bullet_1_Destroy.bmp", L"Bullet_1_Destroy");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/Bullet_2.bmp", L"Bullet_2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/Bullet_2_Destroy.bmp", L"Bullet_2_Destroy");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/Bullet_3.bmp", L"Bullet_3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/Bullet_3_Destroy.bmp", L"Bullet_3_Destroy");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/Bullet_4.bmp", L"Bullet_4");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/Bullet_4_Destroy.bmp", L"Bullet_4_Destroy");

	// 이펙트
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/BulletHit.bmp", L"BulletHit");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Explosion_01.bmp", L"Explosion_01");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Explosion_02_1.bmp", L"Explosion_02_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Explosion_03.bmp", L"Explosion_03");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Explosion_04.bmp", L"Explosion_04");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/PlayerDead.bmp", L"PlayerDead");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/WheelSmoke.bmp", L"WheelSmoke");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/PipeSmoke.bmp", L"PipeSmoke");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/MarionEffect.bmp", L"MarionEffect");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Effect_Charge.bmp", L"Effect_Charge");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Effect_MarionMeleeHit.bmp", L"Effect_MarionMeleeHit");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Explosion_05.bmp", L"Explosion_05");
	
	// 아이템
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/PowerUp.bmp", L"PowerUp");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Candy.bmp", L"Candy");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Bomb.bmp", L"Bomb");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Coin.bmp", L"Coin");

	// NPC
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/NPC/Shark_Move.bmp", L"Shark_Move");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/NPC/Shark_Destroy.bmp", L"Shark_Destroy");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/NPC/Gimmick_Move.bmp", L"Gimmick_Move");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/NPC/Gimmick_Destroy.bmp", L"Gimmick_Destroy");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/NPC/Blade_Move.bmp", L"Blade_Move");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/NPC/Blade_Destroy.bmp", L"Blade_Destroy");

	// 보스
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Stage1_Boss_TequilaGround_CloseDoor.bmp", L"TequilaGround_CloseDoor");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Stage1_Boss_TequilaGround_PavedRoad.bmp", L"TequilaGround_PavedRoad");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Stage1_Boss_TequilaGround_Idle.bmp", L"TequilaGround_Idle");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Stage1_Boss_TequilaGround_SubCannon_1.bmp", L"TequilaGround_SubCannon_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Stage1_Boss_TequilaGround_SubCannon_2.bmp", L"TequilaGround_SubCannon_2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Stage1_Boss_TequilaGround_SubCannon_3.bmp", L"TequilaGround_SubCannon_3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Stage1_Boss_TequilaGround_Destroy.bmp", L"TequilaGround_Destroy");
	// 보스 2페이즈
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Stage1_Boss_TequilaGallop_Phase_Move.bmp", L"TequilaGallop_Phase_Move");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Stage1_Boss_TequilaGallop_Phase_Change.bmp", L"TequilaGallop_Phase_Change");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Stage1_Boss_TequilaGallop_Under.bmp", L"TequilaGallop_Under");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Stage1_Boss_TequilaGallop_Body.bmp", L"TequilaGallop_Body");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Stage1_Boss_TequilaGallop_Arm.bmp", L"TequilaGallop_Arm");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Stage1_Boss_TequilaGallop_Destroy.bmp", L"TequilaGallop_Destroy");

	// UI
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/TextBox.bmp", L"TextBox");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Shark_Standing.bmp", L"Shark_Standing");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Gimmick_Standing.bmp", L"Gimmick_Standing");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Blade_Standing.bmp", L"Blade_Standing");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/BossText_1.bmp", L"BossText_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/BossText_2.bmp", L"BossText_2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_PlayerLife.bmp", L"UI_PlayerLife");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_BigNumber.bmp", L"UI_BigNumber");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_SmallNumber.bmp", L"UI_SmallNumber");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_MainContinue.bmp", L"UI_MainContinue");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_PlayerNum.bmp", L"UI_PlayerNum");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_StageNum.bmp", L"UI_StageNum");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_ChargeBar.bmp", L"UI_ChargeBar");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_BombNum.bmp", L"UI_BombNum");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_PowerUpDownText.bmp", L"UI_PowerUpDownText");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_ScoreText.bmp", L"UI_ScoreText");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_Credit.bmp", L"UI_Credit");

	// CG
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/CG/CG_Kusuri_BG.bmp", L"CG_Kusuri_BG");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/CG/CG_Kusuri_1.bmp", L"CG_Kusuri_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/CG/CG_Marion_01.bmp", L"CG_Marion_01");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/CG/CG_Marion_02.bmp", L"CG_Marion_02");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/CG/CG_Marion_03.bmp", L"CG_Marion_03");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/CG/CG_Marion_04.bmp", L"CG_Marion_04");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/CG/CG_Marion_05.bmp", L"CG_Marion_05");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/CG/CG_Marion_06.bmp", L"CG_Marion_06");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/CG/CG_Marion_07.bmp", L"CG_Marion_07");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/CG/CG_Usagi_01.bmp", L"CG_Usagi_01");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/CG/CG_Usagi_02.bmp", L"CG_Usagi_02");


	// 배경 생성
	m_pBackground = CAbstractFactory<CBG_Stage_1>::Create();
	CObjMgr::Get_Instance()->Add_Object(m_pBackground, OBJID::BACKGROUND);

	// 플레이어 생성
	switch (g_ePlayer_1_Name)
	{
	case PLAYER::ALUCARD:
		break;
	case PLAYER::MARION:
		g_pPlayer_1 = CAbstractFactory<CMarion>::Create();
		g_pPlayer_1->Set_PlayerNum(1);
		CObjMgr::Get_Instance()->Add_Object(g_pPlayer_1, OBJID::PLAYER);
		CGameUIMgr::Get_Instance()->On_Player1();	// 플레이어 1 UI 준비
		break;
	case PLAYER::NAME_END:
		break;
	default:
		break;
	}

	// 게임 UI 생성
	CGameUIMgr::Get_Instance()->Init_Stage();
	CGameUIMgr::Get_Instance()->Set_Credit(g_iCredit);

#pragma region Stage_1_Monster_Set
	//int iAppearance;		// 배치 타이밍, 스크롤 Y 좌표 기준의 값이다
	//DWORD dwOutTime;		// 배치되고 퇴장하기까지의 시간
	//float fPosX;			// 배치되기전 화면 바깥의 좌표 X
	//float fPosY;			// 배치되기전 화면 바깥의 좌표 Y
	//float fDestPosX;		// 바깥에서 화면 안으로 배치될 좌표 X
	//float fDestPosY;		// 바깥에서 화면 안으로 배치될 좌표 Y
	//float fOutPosX;			// 화면 밖으로 퇴장할 좌표 X
	//float fOutPosY;			// 화면 밖으로 퇴장할 좌표 Y
	//float fDestAngle;		// 배치 될 때 선회 각도
	//float fOutAngle;		// 퇴장 할 때 선회 각도
	//OBJTYPE::TYPE enType;	// 오브젝트 타입
	
	// 빨강 세트
	m_arrCreateObjcet[0].iAppearance = 330;
	m_arrCreateObjcet[0].dwOutTime = 700;
	m_arrCreateObjcet[0].fPosX = 300.f;
	m_arrCreateObjcet[0].fPosY = -100.f;
	m_arrCreateObjcet[0].fDestPosX = 400.f;
	m_arrCreateObjcet[0].fDestPosY = 500.f;
	m_arrCreateObjcet[0].fOutPosX = 600.f;
	m_arrCreateObjcet[0].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[0].fDestAngle = 30.f;
	m_arrCreateObjcet[0].fOutAngle = 30.f;
	m_arrCreateObjcet[0].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[1].iAppearance = 332;
	m_arrCreateObjcet[1].dwOutTime = 700;
	m_arrCreateObjcet[1].fPosX = 200.f;
	m_arrCreateObjcet[1].fPosY = -100.f;
	m_arrCreateObjcet[1].fDestPosX = 300.f;
	m_arrCreateObjcet[1].fDestPosY = 490.f;
	m_arrCreateObjcet[1].fOutPosX = 500.f;
	m_arrCreateObjcet[1].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[1].fDestAngle = 30.f;
	m_arrCreateObjcet[1].fOutAngle = 30.f;
	m_arrCreateObjcet[1].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[2].iAppearance = 334;
	m_arrCreateObjcet[2].dwOutTime = 700;
	m_arrCreateObjcet[2].fPosX = 100.f;
	m_arrCreateObjcet[2].fPosY = -100.f;
	m_arrCreateObjcet[2].fDestPosX = 200.f;
	m_arrCreateObjcet[2].fDestPosY = 480.f;
	m_arrCreateObjcet[2].fOutPosX = 400.f;
	m_arrCreateObjcet[2].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[2].fDestAngle = 30.f;
	m_arrCreateObjcet[2].fOutAngle = 30.f;
	m_arrCreateObjcet[2].enType = OBJTYPE::RED;

	// 파랑 세트
	m_arrCreateObjcet[3].iAppearance = 340;
	m_arrCreateObjcet[3].dwOutTime = 700;
	m_arrCreateObjcet[3].fPosX = 370.f;
	m_arrCreateObjcet[3].fPosY = -100.f;
	m_arrCreateObjcet[3].fDestPosX = 170.f;
	m_arrCreateObjcet[3].fDestPosY = 400.f;
	m_arrCreateObjcet[3].fOutPosX = 110.f;
	m_arrCreateObjcet[3].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[3].fDestAngle = -30.f;
	m_arrCreateObjcet[3].fOutAngle = -30.f;
	m_arrCreateObjcet[3].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[4].iAppearance = 342;
	m_arrCreateObjcet[4].dwOutTime = 700;
	m_arrCreateObjcet[4].fPosX = 470.f;
	m_arrCreateObjcet[4].fPosY = -100.f;
	m_arrCreateObjcet[4].fDestPosX = 270.f;
	m_arrCreateObjcet[4].fDestPosY = 390.f;
	m_arrCreateObjcet[4].fOutPosX = 130.f;
	m_arrCreateObjcet[4].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[4].fDestAngle = -30.f;
	m_arrCreateObjcet[4].fOutAngle = -30.f;
	m_arrCreateObjcet[4].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[5].iAppearance = 344;
	m_arrCreateObjcet[5].dwOutTime = 700;
	m_arrCreateObjcet[5].fPosX = 570.f;
	m_arrCreateObjcet[5].fPosY = -100.f;
	m_arrCreateObjcet[5].fDestPosX = 370.f;
	m_arrCreateObjcet[5].fDestPosY = 380.f;
	m_arrCreateObjcet[5].fOutPosX = 150.f;
	m_arrCreateObjcet[5].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[5].fDestAngle = -30.f;
	m_arrCreateObjcet[5].fOutAngle = -30.f;
	m_arrCreateObjcet[5].enType = OBJTYPE::BLUE;

	// 로켓 세트
	m_arrCreateObjcet[6].iAppearance = 400;
	m_arrCreateObjcet[6].fPosX = 236.f;
	m_arrCreateObjcet[6].fPosY = -100.f;
	m_arrCreateObjcet[6].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[7].iAppearance = 400;
	m_arrCreateObjcet[7].fPosX = 436.f;
	m_arrCreateObjcet[7].fPosY = -100.f;
	m_arrCreateObjcet[7].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[8].iAppearance = 408;
	m_arrCreateObjcet[8].fPosX = 136.f;
	m_arrCreateObjcet[8].fPosY = -100.f;
	m_arrCreateObjcet[8].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[9].iAppearance = 408;
	m_arrCreateObjcet[9].fPosX = 336.f;
	m_arrCreateObjcet[9].fPosY = -100.f;
	m_arrCreateObjcet[9].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[10].iAppearance = 408;
	m_arrCreateObjcet[10].fPosX = 536.f;
	m_arrCreateObjcet[10].fPosY = -100.f;
	m_arrCreateObjcet[10].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[11].iAppearance = 414;
	m_arrCreateObjcet[11].fPosX = 36.f;
	m_arrCreateObjcet[11].fPosY = -100.f;
	m_arrCreateObjcet[11].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[12].iAppearance = 414;
	m_arrCreateObjcet[12].fPosX = 636.f;
	m_arrCreateObjcet[12].fPosY = -100.f;
	m_arrCreateObjcet[12].enType = OBJTYPE::ROCKET;

	// 이동 포대
	m_arrCreateObjcet[13].iAppearance = 360;
	m_arrCreateObjcet[13].fPosX = WINCX + 100.f;
	m_arrCreateObjcet[13].fPosY = 200.f;
	m_arrCreateObjcet[13].fDestPosX = 550.f;
	m_arrCreateObjcet[13].fDestPosY = 200.f;
	m_arrCreateObjcet[13].enType = OBJTYPE::MOVER_CANNON;

	m_arrCreateObjcet[14].iAppearance = 420;
	m_arrCreateObjcet[14].fPosX = WINCX + 100.f;
	m_arrCreateObjcet[14].fPosY = 200.f;
	m_arrCreateObjcet[14].fDestPosX = 560.f;
	m_arrCreateObjcet[14].fDestPosY = 200.f;
	m_arrCreateObjcet[14].enType = OBJTYPE::MOVER_CANNON;
	
	// 파랑 세트
	m_arrCreateObjcet[15].iAppearance = 420;
	m_arrCreateObjcet[15].dwOutTime = 200;
	m_arrCreateObjcet[15].fPosX = 170.f;
	m_arrCreateObjcet[15].fPosY = -100.f;
	m_arrCreateObjcet[15].fDestPosX = 420.f;
	m_arrCreateObjcet[15].fDestPosY = 400.f;
	m_arrCreateObjcet[15].fOutPosX = 110.f;
	m_arrCreateObjcet[15].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[15].fDestAngle = 60.f;
	m_arrCreateObjcet[15].fOutAngle = 30.f;
	m_arrCreateObjcet[15].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[16].iAppearance = 422;
	m_arrCreateObjcet[16].dwOutTime = 200;
	m_arrCreateObjcet[16].fPosX = 270.f;
	m_arrCreateObjcet[16].fPosY = -100.f;
	m_arrCreateObjcet[16].fDestPosX = 520.f;
	m_arrCreateObjcet[16].fDestPosY = 390.f;
	m_arrCreateObjcet[16].fOutPosX = 130.f;
	m_arrCreateObjcet[16].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[16].fDestAngle = 60.f;
	m_arrCreateObjcet[16].fOutAngle = 30.f;
	m_arrCreateObjcet[16].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[17].iAppearance = 424;
	m_arrCreateObjcet[17].dwOutTime = 200;
	m_arrCreateObjcet[17].fPosX = 370.f;
	m_arrCreateObjcet[17].fPosY = -100.f;
	m_arrCreateObjcet[17].fDestPosX = 620.f;
	m_arrCreateObjcet[17].fDestPosY = 380.f;
	m_arrCreateObjcet[17].fOutPosX = 150.f;
	m_arrCreateObjcet[17].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[17].fDestAngle = 60.f;
	m_arrCreateObjcet[17].fOutAngle = 30.f;
	m_arrCreateObjcet[17].enType = OBJTYPE::BLUE;
	
	// 파랑 세트
	m_arrCreateObjcet[18].iAppearance = 430;
	m_arrCreateObjcet[18].dwOutTime = 200;
	m_arrCreateObjcet[18].fPosX = 370.f;
	m_arrCreateObjcet[18].fPosY = -100.f;
	m_arrCreateObjcet[18].fDestPosX = 270.f;
	m_arrCreateObjcet[18].fDestPosY = 400.f;
	m_arrCreateObjcet[18].fOutPosX = 600.f;
	m_arrCreateObjcet[18].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[18].fDestAngle = -20.f;
	m_arrCreateObjcet[18].fOutAngle = -30.f;
	m_arrCreateObjcet[18].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[19].iAppearance = 432;
	m_arrCreateObjcet[19].dwOutTime = 200;
	m_arrCreateObjcet[19].fPosX = 470.f;
	m_arrCreateObjcet[19].fPosY = -100.f;
	m_arrCreateObjcet[19].fDestPosX = 370.f;
	m_arrCreateObjcet[19].fDestPosY = 390.f;
	m_arrCreateObjcet[19].fOutPosX = 650.f;
	m_arrCreateObjcet[19].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[19].fDestAngle = -20.f;
	m_arrCreateObjcet[19].fOutAngle = -30.f;
	m_arrCreateObjcet[19].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[20].iAppearance = 434;
	m_arrCreateObjcet[20].dwOutTime = 200;
	m_arrCreateObjcet[20].fPosX = 570.f;
	m_arrCreateObjcet[20].fPosY = -100.f;
	m_arrCreateObjcet[20].fDestPosX = 470.f;
	m_arrCreateObjcet[20].fDestPosY = 380.f;
	m_arrCreateObjcet[20].fOutPosX = 700.f;
	m_arrCreateObjcet[20].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[20].fDestAngle = -20.f;
	m_arrCreateObjcet[20].fOutAngle = -30.f;
	m_arrCreateObjcet[20].enType = OBJTYPE::BLUE;
	
	// 빨강 세트
	m_arrCreateObjcet[21].iAppearance = 450;
	m_arrCreateObjcet[21].dwOutTime = 100;
	m_arrCreateObjcet[21].fPosX = 200.f;
	m_arrCreateObjcet[21].fPosY = -100.f;
	m_arrCreateObjcet[21].fDestPosX = 550.f;
	m_arrCreateObjcet[21].fDestPosY = 400.f;
	m_arrCreateObjcet[21].fOutPosX = 200.f;
	m_arrCreateObjcet[21].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[21].fDestAngle = 20.f;
	m_arrCreateObjcet[21].fOutAngle = 10.f;
	m_arrCreateObjcet[21].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[22].iAppearance = 452;
	m_arrCreateObjcet[22].dwOutTime = 100;
	m_arrCreateObjcet[22].fPosX = 100.f;
	m_arrCreateObjcet[22].fPosY = -100.f;
	m_arrCreateObjcet[22].fDestPosX = 450.f;
	m_arrCreateObjcet[22].fDestPosY = 390.f;
	m_arrCreateObjcet[22].fOutPosX = 100.f;
	m_arrCreateObjcet[22].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[22].fDestAngle = 20.f;
	m_arrCreateObjcet[22].fOutAngle = 10.f;
	m_arrCreateObjcet[22].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[23].iAppearance = 454;
	m_arrCreateObjcet[23].dwOutTime = 100;
	m_arrCreateObjcet[23].fPosX = 0.f;
	m_arrCreateObjcet[23].fPosY = -100.f;
	m_arrCreateObjcet[23].fDestPosX = 350.f;
	m_arrCreateObjcet[23].fDestPosY = 380.f;
	m_arrCreateObjcet[23].fOutPosX = 0.f;
	m_arrCreateObjcet[23].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[23].fDestAngle = 20.f;
	m_arrCreateObjcet[23].fOutAngle = 10.f;
	m_arrCreateObjcet[23].enType = OBJTYPE::RED;

	// 이동 포대
	m_arrCreateObjcet[24].iAppearance = 470;
	m_arrCreateObjcet[24].fPosX = 200.f;
	m_arrCreateObjcet[24].fPosY = 150.f;
	m_arrCreateObjcet[24].fDestPosX = 490.f;
	m_arrCreateObjcet[24].fDestPosY = 150.f;
	m_arrCreateObjcet[24].enType = OBJTYPE::MOVER_CANNON;
	
	// 파랑 세트
	m_arrCreateObjcet[25].iAppearance = 480;
	m_arrCreateObjcet[25].dwOutTime = 50;
	m_arrCreateObjcet[25].fPosX = 570.f;
	m_arrCreateObjcet[25].fPosY = -100.f;
	m_arrCreateObjcet[25].fDestPosX = 70.f;
	m_arrCreateObjcet[25].fDestPosY = 300.f;
	m_arrCreateObjcet[25].fOutPosX = 500.f;
	m_arrCreateObjcet[25].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[25].fDestAngle = -20.f;
	m_arrCreateObjcet[25].fOutAngle = -20.f;
	m_arrCreateObjcet[25].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[26].iAppearance = 482;
	m_arrCreateObjcet[26].dwOutTime = 50;
	m_arrCreateObjcet[26].fPosX = 670.f;
	m_arrCreateObjcet[26].fPosY = -100.f;
	m_arrCreateObjcet[26].fDestPosX = 170.f;
	m_arrCreateObjcet[26].fDestPosY = 290.f;
	m_arrCreateObjcet[26].fOutPosX = 530.f;
	m_arrCreateObjcet[26].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[26].fDestAngle = -20.f;
	m_arrCreateObjcet[26].fOutAngle = -20.f;
	m_arrCreateObjcet[26].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[27].iAppearance = 484;
	m_arrCreateObjcet[27].dwOutTime = 50;
	m_arrCreateObjcet[27].fPosX = 770.f;
	m_arrCreateObjcet[27].fPosY = -100.f;
	m_arrCreateObjcet[27].fDestPosX = 270.f;
	m_arrCreateObjcet[27].fDestPosY = 280.f;
	m_arrCreateObjcet[27].fOutPosX = 550.f;
	m_arrCreateObjcet[27].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[27].fDestAngle = -20.f;
	m_arrCreateObjcet[27].fOutAngle = -20.f;
	m_arrCreateObjcet[27].enType = OBJTYPE::BLUE;

	// 맵 오브젝트
	m_arrCreateObjcet[28].iAppearance = 100;
	m_arrCreateObjcet[28].fPosX = 588.f;
	m_arrCreateObjcet[28].fPosY = 78.f;
	m_arrCreateObjcet[28].enType = OBJTYPE::STAGE_1_SIGN;

	m_arrCreateObjcet[29].iAppearance = 375;
	m_arrCreateObjcet[29].fPosX = 0.f;
	m_arrCreateObjcet[29].fPosY = -STAGE_1_ROOF_HEIGHT * 3.f;
	m_arrCreateObjcet[29].enType = OBJTYPE::STAGE_1_ROOF;

	m_arrCreateObjcet[30].iAppearance = 470;
	m_arrCreateObjcet[30].fPosX = 60.f;
	m_arrCreateObjcet[30].fPosY = -STAGE_1_SHOP_HEIGHT * 3.f;
	m_arrCreateObjcet[30].enType = OBJTYPE::STAGE_1_SHOP;

	m_arrCreateObjcet[31].iAppearance = 630;
	m_arrCreateObjcet[31].fPosX = 102.f;
	m_arrCreateObjcet[31].fPosY = -STAGE_1_SHOP_ROOF_HEIGHT * 3.f;
	m_arrCreateObjcet[31].enType = OBJTYPE::STAGE_1_SHOP_ROOF;
	
	// 중보
	m_arrCreateObjcet[32].iAppearance = 500;
	m_arrCreateObjcet[32].dwOutTime = 7000;
	m_arrCreateObjcet[32].fPosX = 500.f;
	m_arrCreateObjcet[32].fPosY = -100.f;
	m_arrCreateObjcet[32].fDestPosX = 500.f;
	m_arrCreateObjcet[32].fDestPosY = 200.f;
	m_arrCreateObjcet[32].fOutPosX = 400.f;
	m_arrCreateObjcet[32].fOutPosY = -ROBOT_SHARK_HEIGHT * 2.f;
	m_arrCreateObjcet[32].enType = OBJTYPE::ROBOT_SHARK;

	// 고정 포대
	// 1
	m_arrCreateObjcet[33].iAppearance = 580;
	m_arrCreateObjcet[33].fPosX = 402.f;
	m_arrCreateObjcet[33].fPosY = -CANNON_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[33].fDestPosY = 100.f;
	m_arrCreateObjcet[33].enType = OBJTYPE::CANNON;

	// 2
	m_arrCreateObjcet[34].iAppearance = 615;
	m_arrCreateObjcet[34].fPosX = 252.f;
	m_arrCreateObjcet[34].fPosY = -CANNON_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[34].fDestPosY = 100.f;
	m_arrCreateObjcet[34].enType = OBJTYPE::CANNON;

	m_arrCreateObjcet[35].iAppearance = 615;
	m_arrCreateObjcet[35].fPosX = 502.f;
	m_arrCreateObjcet[35].fPosY = -CANNON_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[35].fDestPosY = 100.f;
	m_arrCreateObjcet[35].enType = OBJTYPE::CANNON;

	// 3
	m_arrCreateObjcet[36].iAppearance = 650;
	m_arrCreateObjcet[36].fPosX = 357.f;
	m_arrCreateObjcet[36].fPosY = -CANNON_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[36].fDestPosY = 100.f;
	m_arrCreateObjcet[36].enType = OBJTYPE::CANNON;

	m_arrCreateObjcet[37].iAppearance = 650;
	m_arrCreateObjcet[37].fPosX = 607.f;
	m_arrCreateObjcet[37].fPosY = -CANNON_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[37].fDestPosY = 100.f;
	m_arrCreateObjcet[37].enType = OBJTYPE::CANNON;

	// 4
	m_arrCreateObjcet[38].iAppearance = 685;
	m_arrCreateObjcet[38].fPosX = 457.f;
	m_arrCreateObjcet[38].fPosY = -CANNON_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[38].fDestPosY = 100.f;
	m_arrCreateObjcet[38].enType = OBJTYPE::CANNON;
	
	// 2족 보행 로봇
	m_arrCreateObjcet[39].iAppearance = 700;
	m_arrCreateObjcet[39].fPosX = -ROBOT_BLUE_WIDTH;
	m_arrCreateObjcet[39].fPosY = 260.f;
	m_arrCreateObjcet[39].enType = OBJTYPE::ROBOT_BLUE;
	
	// 맵 오브젝트
	m_arrCreateObjcet[40].iAppearance = 842;
	m_arrCreateObjcet[40].fPosX = 0.f;
	m_arrCreateObjcet[40].fPosY = -STAGE_1_BROKEN_SIGN_HEIGHT * 3.f;
	m_arrCreateObjcet[40].enType = OBJTYPE::STAGE_1_BROKEN_SIGN;

	// 폭탄 공장
	m_arrCreateObjcet[41].iAppearance = 922;
	m_arrCreateObjcet[41].fPosX = 120.f;
	m_arrCreateObjcet[41].fPosY = -(FACTORY_HEIGHT * 3.f) / 2;
	m_arrCreateObjcet[41].enType = OBJTYPE::FACTORY;

	// 빨간 두건
	m_arrCreateObjcet[42].iAppearance = 842;
	m_arrCreateObjcet[42].fPosX = 200.f;
	m_arrCreateObjcet[42].fPosY = -RED_BEANIE_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[42].fDestPosY = 300.f;
	m_arrCreateObjcet[42].enType = OBJTYPE::RED_BEANIE;

	m_arrCreateObjcet[43].iAppearance = 890;
	m_arrCreateObjcet[43].fPosX = 500.f;
	m_arrCreateObjcet[43].fPosY = -RED_BEANIE_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[43].fDestPosY = 300.f;
	m_arrCreateObjcet[43].enType = OBJTYPE::RED_BEANIE;

	m_arrCreateObjcet[44].iAppearance = 940;
	m_arrCreateObjcet[44].fPosX = 300.f;
	m_arrCreateObjcet[44].fPosY = -RED_BEANIE_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[44].fDestPosY = 300.f;
	m_arrCreateObjcet[44].enType = OBJTYPE::RED_BEANIE;

	// 이동 포대
	m_arrCreateObjcet[45].iAppearance = 960;
	m_arrCreateObjcet[45].fPosX = WINCX + 100.f;
	m_arrCreateObjcet[45].fPosY = 200.f;
	m_arrCreateObjcet[45].fDestPosX = 570.f;
	m_arrCreateObjcet[45].fDestPosY = 200.f;
	m_arrCreateObjcet[45].enType = OBJTYPE::MOVER_CANNON;
	
	// 파랑 세트
	m_arrCreateObjcet[46].iAppearance = 604;
	m_arrCreateObjcet[46].dwOutTime = 100;
	m_arrCreateObjcet[46].fPosX = 50.f;
	m_arrCreateObjcet[46].fPosY = -100.f;
	m_arrCreateObjcet[46].fDestPosX = 270.f;
	m_arrCreateObjcet[46].fDestPosY = 400.f;
	m_arrCreateObjcet[46].fOutPosX = 0.f;
	m_arrCreateObjcet[46].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[46].fDestAngle = 20.f;
	m_arrCreateObjcet[46].fOutAngle = 20.f;
	m_arrCreateObjcet[46].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[47].iAppearance = 602;
	m_arrCreateObjcet[47].dwOutTime = 100;
	m_arrCreateObjcet[47].fPosX = 100.f;
	m_arrCreateObjcet[47].fPosY = -100.f;
	m_arrCreateObjcet[47].fDestPosX = 370.f;
	m_arrCreateObjcet[47].fDestPosY = 410.f;
	m_arrCreateObjcet[47].fOutPosX = 50.f;
	m_arrCreateObjcet[47].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[47].fDestAngle = 20.f;
	m_arrCreateObjcet[47].fOutAngle = 20.f;
	m_arrCreateObjcet[47].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[48].iAppearance = 600;
	m_arrCreateObjcet[48].dwOutTime = 100;
	m_arrCreateObjcet[48].fPosX = 150.f;
	m_arrCreateObjcet[48].fPosY = -100.f;
	m_arrCreateObjcet[48].fDestPosX = 470.f;
	m_arrCreateObjcet[48].fDestPosY = 420.f;
	m_arrCreateObjcet[48].fOutPosX = 100.f;
	m_arrCreateObjcet[48].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[48].fDestAngle = 20.f;
	m_arrCreateObjcet[48].fOutAngle = 20.f;
	m_arrCreateObjcet[48].enType = OBJTYPE::BLUE;
	
	// 파랑 세트
	m_arrCreateObjcet[49].iAppearance = 630;
	m_arrCreateObjcet[49].dwOutTime = 50;
	m_arrCreateObjcet[49].fPosX = 570.f;
	m_arrCreateObjcet[49].fPosY = -100.f;
	m_arrCreateObjcet[49].fDestPosX = 70.f;
	m_arrCreateObjcet[49].fDestPosY = 300.f;
	m_arrCreateObjcet[49].fOutPosX = 500.f;
	m_arrCreateObjcet[49].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[49].fDestAngle = -20.f;
	m_arrCreateObjcet[49].fOutAngle = -30.f;
	m_arrCreateObjcet[49].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[50].iAppearance = 632;
	m_arrCreateObjcet[50].dwOutTime = 50;
	m_arrCreateObjcet[50].fPosX = 670.f;
	m_arrCreateObjcet[50].fPosY = -100.f;
	m_arrCreateObjcet[50].fDestPosX = 170.f;
	m_arrCreateObjcet[50].fDestPosY = 290.f;
	m_arrCreateObjcet[50].fOutPosX = 550.f;
	m_arrCreateObjcet[50].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[50].fDestAngle = -20.f;
	m_arrCreateObjcet[50].fOutAngle = -30.f;
	m_arrCreateObjcet[50].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[51].iAppearance = 634;
	m_arrCreateObjcet[51].dwOutTime = 50;
	m_arrCreateObjcet[51].fPosX = 770.f;
	m_arrCreateObjcet[51].fPosY = -100.f;
	m_arrCreateObjcet[51].fDestPosX = 270.f;
	m_arrCreateObjcet[51].fDestPosY = 280.f;
	m_arrCreateObjcet[51].fOutPosX = 600.f;
	m_arrCreateObjcet[51].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[51].fDestAngle = -20.f;
	m_arrCreateObjcet[51].fOutAngle = -30.f;
	m_arrCreateObjcet[51].enType = OBJTYPE::BLUE;
	
	// 파랑 세트
	m_arrCreateObjcet[52].iAppearance = 950;
	m_arrCreateObjcet[52].dwOutTime = 100;
	m_arrCreateObjcet[52].fPosX = 570.f;
	m_arrCreateObjcet[52].fPosY = -100.f;
	m_arrCreateObjcet[52].fDestPosX = 360.f;
	m_arrCreateObjcet[52].fDestPosY = 480.f;
	m_arrCreateObjcet[52].fOutPosX = 600.f;
	m_arrCreateObjcet[52].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[52].fDestAngle = -10.f;
	m_arrCreateObjcet[52].fOutAngle = 10.f;
	m_arrCreateObjcet[52].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[53].iAppearance = 952;
	m_arrCreateObjcet[53].dwOutTime = 100;
	m_arrCreateObjcet[53].fPosX = 620.f;
	m_arrCreateObjcet[53].fPosY = -120.f;
	m_arrCreateObjcet[53].fDestPosX = 460.f;
	m_arrCreateObjcet[53].fDestPosY = 470.f;
	m_arrCreateObjcet[53].fOutPosX = 650.f;
	m_arrCreateObjcet[53].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[53].fDestAngle = -10.f;
	m_arrCreateObjcet[53].fOutAngle = 10.f;
	m_arrCreateObjcet[53].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[54].iAppearance = 954;
	m_arrCreateObjcet[54].dwOutTime = 100;
	m_arrCreateObjcet[54].fPosX = 670.f;
	m_arrCreateObjcet[54].fPosY = -140.f;
	m_arrCreateObjcet[54].fDestPosX = 560.f;
	m_arrCreateObjcet[54].fDestPosY = 460.f;
	m_arrCreateObjcet[54].fOutPosX = 700.f;
	m_arrCreateObjcet[54].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[54].fDestAngle = -10.f;
	m_arrCreateObjcet[54].fOutAngle = 10.f;
	m_arrCreateObjcet[54].enType = OBJTYPE::BLUE;
	
	// 파랑 세트
	m_arrCreateObjcet[55].iAppearance = 960;
	m_arrCreateObjcet[55].dwOutTime = 100;
	m_arrCreateObjcet[55].fPosX = -100.f;
	m_arrCreateObjcet[55].fPosY = -100.f;
	m_arrCreateObjcet[55].fDestPosX = 360.f;
	m_arrCreateObjcet[55].fDestPosY = 380.f;
	m_arrCreateObjcet[55].fOutPosX = 200.f;
	m_arrCreateObjcet[55].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[55].fDestAngle = 50.f;
	m_arrCreateObjcet[55].fOutAngle = 10.f;
	m_arrCreateObjcet[55].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[56].iAppearance = 962;
	m_arrCreateObjcet[56].dwOutTime = 100;
	m_arrCreateObjcet[56].fPosX = 0.f;
	m_arrCreateObjcet[56].fPosY = -120.f;
	m_arrCreateObjcet[56].fDestPosX = 460.f;
	m_arrCreateObjcet[56].fDestPosY = 370.f;
	m_arrCreateObjcet[56].fOutPosX = 250.f;
	m_arrCreateObjcet[56].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[56].fDestAngle = 50.f;
	m_arrCreateObjcet[56].fOutAngle = 10.f;
	m_arrCreateObjcet[56].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[57].iAppearance = 964;
	m_arrCreateObjcet[57].dwOutTime = 100;
	m_arrCreateObjcet[57].fPosX = 100.f;
	m_arrCreateObjcet[57].fPosY = -140.f;
	m_arrCreateObjcet[57].fDestPosX = 560.f;
	m_arrCreateObjcet[57].fDestPosY = 360.f;
	m_arrCreateObjcet[57].fOutPosX = 300.f;
	m_arrCreateObjcet[57].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[57].fDestAngle = 50.f;
	m_arrCreateObjcet[57].fOutAngle = 10.f;
	m_arrCreateObjcet[57].enType = OBJTYPE::BLUE;
	
	// 빨간 두건
	m_arrCreateObjcet[58].iAppearance = 1030;
	m_arrCreateObjcet[58].fPosX = 280.f;
	m_arrCreateObjcet[58].fPosY = -RED_BEANIE_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[58].fDestPosY = 300.f;
	m_arrCreateObjcet[58].enType = OBJTYPE::RED_BEANIE;

	m_arrCreateObjcet[59].iAppearance = 1030;
	m_arrCreateObjcet[59].fPosX = 500.f;
	m_arrCreateObjcet[59].fPosY = -RED_BEANIE_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[59].fDestPosY = 300.f;
	m_arrCreateObjcet[59].enType = OBJTYPE::RED_BEANIE;
	
	// 빨강 세트
	m_arrCreateObjcet[60].iAppearance = 1050;
	m_arrCreateObjcet[60].dwOutTime = 100;
	m_arrCreateObjcet[60].fPosX = 570.f;
	m_arrCreateObjcet[60].fPosY = -100.f;
	m_arrCreateObjcet[60].fDestPosX = 310.f;
	m_arrCreateObjcet[60].fDestPosY = 430.f;
	m_arrCreateObjcet[60].fOutPosX = 600.f;
	m_arrCreateObjcet[60].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[60].fDestAngle = -10.f;
	m_arrCreateObjcet[60].fOutAngle = -30.f;
	m_arrCreateObjcet[60].enType = OBJTYPE::RED;

	m_arrCreateObjcet[61].iAppearance = 1052;
	m_arrCreateObjcet[61].dwOutTime = 100;
	m_arrCreateObjcet[61].fPosX = 620.f;
	m_arrCreateObjcet[61].fPosY = -120.f;
	m_arrCreateObjcet[61].fDestPosX = 410.f;
	m_arrCreateObjcet[61].fDestPosY = 420.f;
	m_arrCreateObjcet[61].fOutPosX = 650.f;
	m_arrCreateObjcet[61].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[61].fDestAngle = -10.f;
	m_arrCreateObjcet[61].fOutAngle = -30.f;
	m_arrCreateObjcet[61].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[62].iAppearance = 1054;
	m_arrCreateObjcet[62].dwOutTime = 100;
	m_arrCreateObjcet[62].fPosX = 670.f;
	m_arrCreateObjcet[62].fPosY = -140.f;
	m_arrCreateObjcet[62].fDestPosX = 510.f;
	m_arrCreateObjcet[62].fDestPosY = 410.f;
	m_arrCreateObjcet[62].fOutPosX = 700.f;
	m_arrCreateObjcet[62].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[62].fDestAngle = -10.f;
	m_arrCreateObjcet[62].fOutAngle = -30.f;
	m_arrCreateObjcet[62].enType = OBJTYPE::BLUE;
	
	// 파랑 세트
	m_arrCreateObjcet[63].iAppearance = 1060;
	m_arrCreateObjcet[63].dwOutTime = 100;
	m_arrCreateObjcet[63].fPosX = -100.f;
	m_arrCreateObjcet[63].fPosY = 100.f;
	m_arrCreateObjcet[63].fDestPosX = 510.f;
	m_arrCreateObjcet[63].fDestPosY = 330.f;
	m_arrCreateObjcet[63].fOutPosX = 300.f;
	m_arrCreateObjcet[63].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[63].fDestAngle = 10.f;
	m_arrCreateObjcet[63].fOutAngle = 30.f;
	m_arrCreateObjcet[63].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[64].iAppearance = 1062;
	m_arrCreateObjcet[64].dwOutTime = 100;
	m_arrCreateObjcet[64].fPosX = -200.f;
	m_arrCreateObjcet[64].fPosY = 100.f;
	m_arrCreateObjcet[64].fDestPosX = 410.f;
	m_arrCreateObjcet[64].fDestPosY = 320.f;
	m_arrCreateObjcet[64].fOutPosX = 250.f;
	m_arrCreateObjcet[64].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[64].fDestAngle = 10.f;
	m_arrCreateObjcet[64].fOutAngle = 30.f;
	m_arrCreateObjcet[64].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[65].iAppearance = 1064;
	m_arrCreateObjcet[65].dwOutTime = 100;
	m_arrCreateObjcet[65].fPosX = -300.f;
	m_arrCreateObjcet[65].fPosY = 100.f;
	m_arrCreateObjcet[65].fDestPosX = 310.f;
	m_arrCreateObjcet[65].fDestPosY = 310.f;
	m_arrCreateObjcet[65].fOutPosX = 200.f;
	m_arrCreateObjcet[65].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[65].fDestAngle = 10.f;
	m_arrCreateObjcet[65].fOutAngle = 30.f;
	m_arrCreateObjcet[65].enType = OBJTYPE::BLUE;
	
	// 빨간 두건
	m_arrCreateObjcet[66].iAppearance = 1060;
	m_arrCreateObjcet[66].fPosX = 450.f;
	m_arrCreateObjcet[66].fPosY = -RED_BEANIE_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[66].fDestPosY = 300.f;
	m_arrCreateObjcet[66].enType = OBJTYPE::RED_BEANIE;

	m_arrCreateObjcet[67].iAppearance = 1080;
	m_arrCreateObjcet[67].fPosX = 200.f;
	m_arrCreateObjcet[67].fPosY = -RED_BEANIE_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[67].fDestPosY = 300.f;
	m_arrCreateObjcet[67].enType = OBJTYPE::RED_BEANIE;
	
	m_arrCreateObjcet[68].iAppearance = 1090;
	m_arrCreateObjcet[68].fPosX = 530.f;
	m_arrCreateObjcet[68].fPosY = -RED_BEANIE_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[68].fDestPosY = 300.f;
	m_arrCreateObjcet[68].enType = OBJTYPE::RED_BEANIE;
	
	m_arrCreateObjcet[69].iAppearance = 1110;
	m_arrCreateObjcet[69].fPosX = 120.f;
	m_arrCreateObjcet[69].fPosY = -RED_BEANIE_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[69].fDestPosY = 300.f;
	m_arrCreateObjcet[69].enType = OBJTYPE::RED_BEANIE;
	
	m_arrCreateObjcet[70].iAppearance = 1120;
	m_arrCreateObjcet[70].fPosX = 550.f;
	m_arrCreateObjcet[70].fPosY = -RED_BEANIE_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[70].fDestPosY = 300.f;
	m_arrCreateObjcet[70].enType = OBJTYPE::RED_BEANIE;
	
	// 파랑 세트
	m_arrCreateObjcet[71].iAppearance = 1160;
	m_arrCreateObjcet[71].dwOutTime = 100;
	m_arrCreateObjcet[71].fPosX = -50.f;
	m_arrCreateObjcet[71].fPosY = -100.f;
	m_arrCreateObjcet[71].fDestPosX = 410.f;
	m_arrCreateObjcet[71].fDestPosY = 530.f;
	m_arrCreateObjcet[71].fOutPosX = 150.f;
	m_arrCreateObjcet[71].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[71].fDestAngle = 10.f;
	m_arrCreateObjcet[71].fOutAngle = 30.f;
	m_arrCreateObjcet[71].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[72].iAppearance = 1162;
	m_arrCreateObjcet[72].dwOutTime = 100;
	m_arrCreateObjcet[72].fPosX = -100.f;
	m_arrCreateObjcet[72].fPosY = -100.f;
	m_arrCreateObjcet[72].fDestPosX = 310.f;
	m_arrCreateObjcet[72].fDestPosY = 520.f;
	m_arrCreateObjcet[72].fOutPosX = 100.f;
	m_arrCreateObjcet[72].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[72].fDestAngle = 10.f;
	m_arrCreateObjcet[72].fOutAngle = 30.f;
	m_arrCreateObjcet[72].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[73].iAppearance = 1164;
	m_arrCreateObjcet[73].dwOutTime = 100;
	m_arrCreateObjcet[73].fPosX = -150.f;
	m_arrCreateObjcet[73].fPosY = -100.f;
	m_arrCreateObjcet[73].fDestPosX = 210.f;
	m_arrCreateObjcet[73].fDestPosY = 510.f;
	m_arrCreateObjcet[73].fOutPosX = 50.f;
	m_arrCreateObjcet[73].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[73].fDestAngle = 10.f;
	m_arrCreateObjcet[73].fOutAngle = 30.f;
	m_arrCreateObjcet[73].enType = OBJTYPE::BLUE;
	
	// 파랑 세트
	m_arrCreateObjcet[74].iAppearance = 1190;
	m_arrCreateObjcet[74].dwOutTime = 100;
	m_arrCreateObjcet[74].fPosX = 570.f;
	m_arrCreateObjcet[74].fPosY = -100.f;
	m_arrCreateObjcet[74].fDestPosX = 180.f;
	m_arrCreateObjcet[74].fDestPosY = 430.f;
	m_arrCreateObjcet[74].fOutPosX = 600.f;
	m_arrCreateObjcet[74].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[74].fDestAngle = -10.f;
	m_arrCreateObjcet[74].fOutAngle = -30.f;
	m_arrCreateObjcet[74].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[75].iAppearance = 1192;
	m_arrCreateObjcet[75].dwOutTime = 100;
	m_arrCreateObjcet[75].fPosX = 620.f;
	m_arrCreateObjcet[75].fPosY = -120.f;
	m_arrCreateObjcet[75].fDestPosX = 280.f;
	m_arrCreateObjcet[75].fDestPosY = 420.f;
	m_arrCreateObjcet[75].fOutPosX = 650.f;
	m_arrCreateObjcet[75].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[75].fDestAngle = -10.f;
	m_arrCreateObjcet[75].fOutAngle = -30.f;
	m_arrCreateObjcet[75].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[76].iAppearance = 1194;
	m_arrCreateObjcet[76].dwOutTime = 100;
	m_arrCreateObjcet[76].fPosX = 670.f;
	m_arrCreateObjcet[76].fPosY = -140.f;
	m_arrCreateObjcet[76].fDestPosX = 380.f;
	m_arrCreateObjcet[76].fDestPosY = 410.f;
	m_arrCreateObjcet[76].fOutPosX = 700.f;
	m_arrCreateObjcet[76].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[76].fDestAngle = -10.f;
	m_arrCreateObjcet[76].fOutAngle = -30.f;
	m_arrCreateObjcet[76].enType = OBJTYPE::BLUE;
	
	// 파랑 세트
	m_arrCreateObjcet[77].iAppearance = 1260;
	m_arrCreateObjcet[77].dwOutTime = 100;
	m_arrCreateObjcet[77].fPosX = 570.f;
	m_arrCreateObjcet[77].fPosY = -100.f;
	m_arrCreateObjcet[77].fDestPosX = 310.f;
	m_arrCreateObjcet[77].fDestPosY = 430.f;
	m_arrCreateObjcet[77].fOutPosX = 600.f;
	m_arrCreateObjcet[77].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[77].fDestAngle = -10.f;
	m_arrCreateObjcet[77].fOutAngle = -30.f;
	m_arrCreateObjcet[77].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[78].iAppearance = 1262;
	m_arrCreateObjcet[78].dwOutTime = 100;
	m_arrCreateObjcet[78].fPosX = 620.f;
	m_arrCreateObjcet[78].fPosY = -120.f;
	m_arrCreateObjcet[78].fDestPosX = 410.f;
	m_arrCreateObjcet[78].fDestPosY = 420.f;
	m_arrCreateObjcet[78].fOutPosX = 650.f;
	m_arrCreateObjcet[78].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[78].fDestAngle = -10.f;
	m_arrCreateObjcet[78].fOutAngle = -30.f;
	m_arrCreateObjcet[78].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[79].iAppearance = 1264;
	m_arrCreateObjcet[79].dwOutTime = 100;
	m_arrCreateObjcet[79].fPosX = 670.f;
	m_arrCreateObjcet[79].fPosY = -140.f;
	m_arrCreateObjcet[79].fDestPosX = 510.f;
	m_arrCreateObjcet[79].fDestPosY = 410.f;
	m_arrCreateObjcet[79].fOutPosX = 700.f;
	m_arrCreateObjcet[79].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[79].fDestAngle = -10.f;
	m_arrCreateObjcet[79].fOutAngle = -30.f;
	m_arrCreateObjcet[79].enType = OBJTYPE::BLUE;
	
	// 파랑 세트
	m_arrCreateObjcet[80].iAppearance = 1270;
	m_arrCreateObjcet[80].dwOutTime = 100;
	m_arrCreateObjcet[80].fPosX = -100.f;
	m_arrCreateObjcet[80].fPosY = 0.f;
	m_arrCreateObjcet[80].fDestPosX = 510.f;
	m_arrCreateObjcet[80].fDestPosY = 330.f;
	m_arrCreateObjcet[80].fOutPosX = 300.f;
	m_arrCreateObjcet[80].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[80].fDestAngle = 10.f;
	m_arrCreateObjcet[80].fOutAngle = 30.f;
	m_arrCreateObjcet[80].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[81].iAppearance = 1272;
	m_arrCreateObjcet[81].dwOutTime = 100;
	m_arrCreateObjcet[81].fPosX = -200.f;
	m_arrCreateObjcet[81].fPosY = 0.f;
	m_arrCreateObjcet[81].fDestPosX = 410.f;
	m_arrCreateObjcet[81].fDestPosY = 320.f;
	m_arrCreateObjcet[81].fOutPosX = 250.f;
	m_arrCreateObjcet[81].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[81].fDestAngle = 10.f;
	m_arrCreateObjcet[81].fOutAngle = 30.f;
	m_arrCreateObjcet[81].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[82].iAppearance = 1274;
	m_arrCreateObjcet[82].dwOutTime = 100;
	m_arrCreateObjcet[82].fPosX = -300.f;
	m_arrCreateObjcet[82].fPosY = 0.f;
	m_arrCreateObjcet[82].fDestPosX = 310.f;
	m_arrCreateObjcet[82].fDestPosY = 310.f;
	m_arrCreateObjcet[82].fOutPosX = 200.f;
	m_arrCreateObjcet[82].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[82].fDestAngle = 10.f;
	m_arrCreateObjcet[82].fOutAngle = 30.f;
	m_arrCreateObjcet[82].enType = OBJTYPE::BLUE;
	
	// 파랑 세트
	m_arrCreateObjcet[83].iAppearance = 1400;
	m_arrCreateObjcet[83].dwOutTime = 200;
	m_arrCreateObjcet[83].fPosX = 170.f;
	m_arrCreateObjcet[83].fPosY = -100.f;
	m_arrCreateObjcet[83].fDestPosX = 420.f;
	m_arrCreateObjcet[83].fDestPosY = 400.f;
	m_arrCreateObjcet[83].fOutPosX = 110.f;
	m_arrCreateObjcet[83].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[83].fDestAngle = 60.f;
	m_arrCreateObjcet[83].fOutAngle = 30.f;
	m_arrCreateObjcet[83].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[84].iAppearance = 1402;
	m_arrCreateObjcet[84].dwOutTime = 200;
	m_arrCreateObjcet[84].fPosX = 270.f;
	m_arrCreateObjcet[84].fPosY = -100.f;
	m_arrCreateObjcet[84].fDestPosX = 520.f;
	m_arrCreateObjcet[84].fDestPosY = 390.f;
	m_arrCreateObjcet[84].fOutPosX = 130.f;
	m_arrCreateObjcet[84].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[84].fDestAngle = 60.f;
	m_arrCreateObjcet[84].fOutAngle = 30.f;
	m_arrCreateObjcet[84].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[85].iAppearance = 1404;
	m_arrCreateObjcet[85].dwOutTime = 200;
	m_arrCreateObjcet[85].fPosX = 370.f;
	m_arrCreateObjcet[85].fPosY = -100.f;
	m_arrCreateObjcet[85].fDestPosX = 620.f;
	m_arrCreateObjcet[85].fDestPosY = 380.f;
	m_arrCreateObjcet[85].fOutPosX = 150.f;
	m_arrCreateObjcet[85].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[85].fDestAngle = 60.f;
	m_arrCreateObjcet[85].fOutAngle = 30.f;
	m_arrCreateObjcet[85].enType = OBJTYPE::BLUE;

	// 파랑 세트
	m_arrCreateObjcet[86].iAppearance = 1420;
	m_arrCreateObjcet[86].dwOutTime = 200;
	m_arrCreateObjcet[86].fPosX = 370.f;
	m_arrCreateObjcet[86].fPosY = -100.f;
	m_arrCreateObjcet[86].fDestPosX = 270.f;
	m_arrCreateObjcet[86].fDestPosY = 400.f;
	m_arrCreateObjcet[86].fOutPosX = 600.f;
	m_arrCreateObjcet[86].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[86].fDestAngle = -20.f;
	m_arrCreateObjcet[86].fOutAngle = -30.f;
	m_arrCreateObjcet[86].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[87].iAppearance = 1422;
	m_arrCreateObjcet[87].dwOutTime = 200;
	m_arrCreateObjcet[87].fPosX = 470.f;
	m_arrCreateObjcet[87].fPosY = -100.f;
	m_arrCreateObjcet[87].fDestPosX = 370.f;
	m_arrCreateObjcet[87].fDestPosY = 390.f;
	m_arrCreateObjcet[87].fOutPosX = 650.f;
	m_arrCreateObjcet[87].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[87].fDestAngle = -20.f;
	m_arrCreateObjcet[87].fOutAngle = -30.f;
	m_arrCreateObjcet[87].enType = OBJTYPE::BLUE;

	m_arrCreateObjcet[88].iAppearance = 1424;
	m_arrCreateObjcet[88].dwOutTime = 200;
	m_arrCreateObjcet[88].fPosX = 570.f;
	m_arrCreateObjcet[88].fPosY = -100.f;
	m_arrCreateObjcet[88].fDestPosX = 470.f;
	m_arrCreateObjcet[88].fDestPosY = 380.f;
	m_arrCreateObjcet[88].fOutPosX = 700.f;
	m_arrCreateObjcet[88].fOutPosY = -BLUE_HEIGHT * 2.f;
	m_arrCreateObjcet[88].fDestAngle = -20.f;
	m_arrCreateObjcet[88].fOutAngle = -30.f;
	m_arrCreateObjcet[88].enType = OBJTYPE::BLUE;
	
	// 선인장 고정 포대
	m_arrCreateObjcet[89].iAppearance = 1150;
	m_arrCreateObjcet[89].fPosX = 450.f;
	m_arrCreateObjcet[89].fPosY = -CANNON_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[89].fDestPosY = 100.f;
	m_arrCreateObjcet[89].enType = OBJTYPE::CACTUS_CANNON;

	m_arrCreateObjcet[90].iAppearance = 1160;
	m_arrCreateObjcet[90].fPosX = 45.f;
	m_arrCreateObjcet[90].fPosY = -CANNON_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[90].fDestPosY = 100.f;
	m_arrCreateObjcet[90].enType = OBJTYPE::CACTUS_CANNON;

	m_arrCreateObjcet[91].iAppearance = 1170;
	m_arrCreateObjcet[91].fPosX = 180.f;
	m_arrCreateObjcet[91].fPosY = -CANNON_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[91].fDestPosY = 100.f;
	m_arrCreateObjcet[91].enType = OBJTYPE::CACTUS_CANNON;

	m_arrCreateObjcet[92].iAppearance = 1205;
	m_arrCreateObjcet[92].fPosX = 630.f;
	m_arrCreateObjcet[92].fPosY = -CANNON_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[92].fDestPosY = 100.f;
	m_arrCreateObjcet[92].enType = OBJTYPE::CACTUS_CANNON;

	m_arrCreateObjcet[93].iAppearance = 1230;
	m_arrCreateObjcet[93].fPosX = 360.f;
	m_arrCreateObjcet[93].fPosY = -CANNON_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[93].fDestPosY = 100.f;
	m_arrCreateObjcet[93].enType = OBJTYPE::CACTUS_CANNON;

	m_arrCreateObjcet[94].iAppearance = 1236;
	m_arrCreateObjcet[94].fPosX = 120.f;
	m_arrCreateObjcet[94].fPosY = -CANNON_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[94].fDestPosY = 100.f;
	m_arrCreateObjcet[94].enType = OBJTYPE::CACTUS_CANNON;

	m_arrCreateObjcet[95].iAppearance = 1257;
	m_arrCreateObjcet[95].fPosX = 492.f;
	m_arrCreateObjcet[95].fPosY = -CANNON_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[95].fDestPosY = 100.f;
	m_arrCreateObjcet[95].enType = OBJTYPE::CACTUS_CANNON;

	m_arrCreateObjcet[96].iAppearance = 1282;
	m_arrCreateObjcet[96].fPosX = 177.f;
	m_arrCreateObjcet[96].fPosY = -CANNON_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[96].fDestPosY = 100.f;
	m_arrCreateObjcet[96].enType = OBJTYPE::CACTUS_CANNON;

	m_arrCreateObjcet[97].iAppearance = 1296;
	m_arrCreateObjcet[97].fPosX = 51.f;
	m_arrCreateObjcet[97].fPosY = -CANNON_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[97].fDestPosY = 100.f;
	m_arrCreateObjcet[97].enType = OBJTYPE::CACTUS_CANNON;

	m_arrCreateObjcet[98].iAppearance = 1299;
	m_arrCreateObjcet[98].fPosX = 351.f;
	m_arrCreateObjcet[98].fPosY = -CANNON_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[98].fDestPosY = 100.f;
	m_arrCreateObjcet[98].enType = OBJTYPE::CACTUS_CANNON;

	m_arrCreateObjcet[99].iAppearance = 1357;
	m_arrCreateObjcet[99].fPosX = 489.f;
	m_arrCreateObjcet[99].fPosY = -CANNON_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[99].fDestPosY = 100.f;
	m_arrCreateObjcet[99].enType = OBJTYPE::CACTUS_CANNON;

	m_arrCreateObjcet[100].iAppearance = 1366;
	m_arrCreateObjcet[100].fPosX = 189.f;
	m_arrCreateObjcet[100].fPosY = -CANNON_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[100].fDestPosY = 100.f;
	m_arrCreateObjcet[100].enType = OBJTYPE::CACTUS_CANNON;

	m_arrCreateObjcet[101].iAppearance = 1383;
	m_arrCreateObjcet[101].fPosX = 51.f;
	m_arrCreateObjcet[101].fPosY = -CANNON_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[101].fDestPosY = 100.f;
	m_arrCreateObjcet[101].enType = OBJTYPE::CACTUS_CANNON;

	m_arrCreateObjcet[102].iAppearance = 1407;
	m_arrCreateObjcet[102].fPosX = 330.f;
	m_arrCreateObjcet[102].fPosY = -CANNON_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[102].fDestPosY = 100.f;
	m_arrCreateObjcet[102].enType = OBJTYPE::CACTUS_CANNON;

	m_arrCreateObjcet[103].iAppearance = 1423;
	m_arrCreateObjcet[103].fPosX = 585.f;
	m_arrCreateObjcet[103].fPosY = -CANNON_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[103].fDestPosY = 100.f;
	m_arrCreateObjcet[103].enType = OBJTYPE::CACTUS_CANNON;

	m_arrCreateObjcet[104].iAppearance = 1440;
	m_arrCreateObjcet[104].fPosX = 219.f;
	m_arrCreateObjcet[104].fPosY = -CANNON_HEIGHT * 3.f / 2.f;
	m_arrCreateObjcet[104].fDestPosY = 100.f;
	m_arrCreateObjcet[104].enType = OBJTYPE::CACTUS_CANNON;

	// 암석 오른쪽
	m_arrCreateObjcet[105].iAppearance = 1550;
	m_arrCreateObjcet[105].fPosX = 305.f;
	m_arrCreateObjcet[105].fPosY = -STAGE_1_RIGHT_ROCK_HEIGHT * 3.f;
	m_arrCreateObjcet[105].fDestPosX = WINCX + 50.f;
	m_arrCreateObjcet[105].fDestPosY = -450.f;
	m_arrCreateObjcet[105].enType = OBJTYPE::STAGE_1_RIGHT_ROCK;

	// 암석 왼쪽
	m_arrCreateObjcet[106].iAppearance = 1600;
	m_arrCreateObjcet[106].fPosX = 0.f;
	m_arrCreateObjcet[106].fPosY = -STAGE_1_LEFT_ROCK_HEIGHT * 3.f;
	m_arrCreateObjcet[106].fDestPosX = -STAGE_1_LEFT_ROCK_WIDTH * 3.f - 50.f;
	m_arrCreateObjcet[106].fDestPosY = -600.f;
	m_arrCreateObjcet[106].enType = OBJTYPE::STAGE_1_LEFT_ROCK;

	// 공적단
	m_arrCreateObjcet[107].iAppearance = 1220;
	m_arrCreateObjcet[107].fPosX = -SHARK_WIDTH * 3.f;
	m_arrCreateObjcet[107].fPosY = 480.f;
	m_arrCreateObjcet[107].fDestPosX = 260.f;
	m_arrCreateObjcet[107].fDestPosY = 280.f;
	m_arrCreateObjcet[107].enType = OBJTYPE::SHARK;

	m_arrCreateObjcet[108].iAppearance = 1225;
	m_arrCreateObjcet[108].fPosX = -GIMMICK_WIDTH * 3.f;
	m_arrCreateObjcet[108].fPosY = 495.f;
	m_arrCreateObjcet[108].fDestPosX = 260.f;
	m_arrCreateObjcet[108].fDestPosY = 295.f;
	m_arrCreateObjcet[108].enType = OBJTYPE::GIMMICK;

	m_arrCreateObjcet[109].iAppearance = 1230;
	m_arrCreateObjcet[109].fPosX = -BLADE_WIDTH * 3.f;
	m_arrCreateObjcet[109].fPosY = 520.f;
	m_arrCreateObjcet[109].fDestPosX = 260.f;
	m_arrCreateObjcet[109].fDestPosY = 320.f;
	m_arrCreateObjcet[109].enType = OBJTYPE::BLADE;
	
	// 섹터
	m_arrCreateObjcet[110].iAppearance = 1220;
	m_arrCreateObjcet[110].fPosX = 270.f;
	m_arrCreateObjcet[110].fPosY = 280.f;
	m_arrCreateObjcet[110].fDestPosX = 0.f;
	m_arrCreateObjcet[110].fDestPosY = -1000.f;
	m_arrCreateObjcet[110].iWidth = 30;
	m_arrCreateObjcet[110].iHeight = 100;
	m_arrCreateObjcet[110].enType = OBJTYPE::SECTOR;

	// 섹터
	m_arrCreateObjcet[111].iAppearance = 1400;
	m_arrCreateObjcet[111].fPosX = 270.f;
	m_arrCreateObjcet[111].fPosY = 60.f;
	m_arrCreateObjcet[111].fDestPosX = 150.f;
	m_arrCreateObjcet[111].fDestPosY = -150.f;
	m_arrCreateObjcet[111].iWidth = 30;
	m_arrCreateObjcet[111].iHeight = 30;
	m_arrCreateObjcet[111].enType = OBJTYPE::SECTOR;

	// 섹터
	m_arrCreateObjcet[112].iAppearance = 1400;
	m_arrCreateObjcet[112].fPosX = 420.f;
	m_arrCreateObjcet[112].fPosY = -90.f;
	m_arrCreateObjcet[112].fDestPosX = 0.f;
	m_arrCreateObjcet[112].fDestPosY = -300.f;
	m_arrCreateObjcet[112].iWidth = 30;
	m_arrCreateObjcet[112].iHeight = 30;
	m_arrCreateObjcet[112].enType = OBJTYPE::SECTOR;

	// 섹터
	m_arrCreateObjcet[113].iAppearance = 1500;
	m_arrCreateObjcet[113].fPosX = 420.f;
	m_arrCreateObjcet[113].fPosY = -0.f;
	m_arrCreateObjcet[113].fDestPosX = -80;
	m_arrCreateObjcet[113].fDestPosY = -80.f;
	m_arrCreateObjcet[113].iWidth = 30;
	m_arrCreateObjcet[113].iHeight = 30;
	m_arrCreateObjcet[113].enType = OBJTYPE::SECTOR;

	// 섹터
	m_arrCreateObjcet[114].iAppearance = 1520;
	m_arrCreateObjcet[114].fPosX = 330.f;
	m_arrCreateObjcet[114].fPosY = 0.f;
	m_arrCreateObjcet[114].fDestPosX = 0.f;
	m_arrCreateObjcet[114].fDestPosY = -300.f;
	m_arrCreateObjcet[114].iWidth = 30;
	m_arrCreateObjcet[114].iHeight = 30;
	m_arrCreateObjcet[114].enType = OBJTYPE::SECTOR;

	// 섹터
	m_arrCreateObjcet[115].iAppearance = 1600;
	m_arrCreateObjcet[115].fPosX = 330.f;
	m_arrCreateObjcet[115].fPosY = -30.f;
	m_arrCreateObjcet[115].fDestPosX = 600.f;
	m_arrCreateObjcet[115].fDestPosY = 0.f;
	m_arrCreateObjcet[115].iWidth = 30;
	m_arrCreateObjcet[115].iHeight = 30;
	m_arrCreateObjcet[115].enType = OBJTYPE::SECTOR;
	
	// 공적단
	m_arrCreateObjcet[116].iAppearance = 1730;
	m_arrCreateObjcet[116].fPosX = WINCX - 50;
	m_arrCreateObjcet[116].fPosY = -10.f;
	m_arrCreateObjcet[116].fDestPosX = 285.f;
	m_arrCreateObjcet[116].fDestPosY = -10.f;
	m_arrCreateObjcet[116].enType = OBJTYPE::GIMMICK;

	m_arrCreateObjcet[117].iAppearance = 1730;
	m_arrCreateObjcet[117].fPosX = WINCX;
	m_arrCreateObjcet[117].fPosY = -5.f;
	m_arrCreateObjcet[117].fDestPosX = 375.f;
	m_arrCreateObjcet[117].fDestPosY = -5.f;
	m_arrCreateObjcet[117].enType = OBJTYPE::BLADE;
	
	m_arrCreateObjcet[118].iAppearance = 1730;
	m_arrCreateObjcet[118].fPosX = WINCX - 100;
	m_arrCreateObjcet[118].fPosY = 14.f;
	m_arrCreateObjcet[118].fDestPosX = 330.f;
	m_arrCreateObjcet[118].fDestPosY = 14.f;
	m_arrCreateObjcet[118].enType = OBJTYPE::SHARK;
	
	// 보스
	m_arrCreateObjcet[119].iAppearance = 1715;
	m_arrCreateObjcet[119].fPosX = 330;
	m_arrCreateObjcet[119].fPosY = (-TEQUILA_GROUND_HEIGHT * 3) >> 1;
	m_arrCreateObjcet[119].enType = OBJTYPE::TEQUILA_GROUND;

	// 브릿지
	m_arrCreateObjcet[120].iAppearance = 1540;//1738;		// 암벽과 겹치기 때문에 미리 생성해야됨 안그러면 정렬 하던가...
	m_arrCreateObjcet[120].fPosX = 0.f;
	m_arrCreateObjcet[120].fPosY = (-STAGE_1_BRIDGE_HEIGHT * 3.f) - 594.f;
	m_arrCreateObjcet[120].enType = OBJTYPE::STAGE_1_BRIDGE;
	
	// 로켓 세트
	m_arrCreateObjcet[121].iAppearance = 1590;
	m_arrCreateObjcet[121].fPosX = 236.f;
	m_arrCreateObjcet[121].fPosY = -100.f;
	m_arrCreateObjcet[121].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[122].iAppearance = 1590;
	m_arrCreateObjcet[122].fPosX = 436.f;
	m_arrCreateObjcet[122].fPosY = -100.f;
	m_arrCreateObjcet[122].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[123].iAppearance = 1598;
	m_arrCreateObjcet[123].fPosX = 136.f;
	m_arrCreateObjcet[123].fPosY = -100.f;
	m_arrCreateObjcet[123].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[124].iAppearance = 1598;
	m_arrCreateObjcet[124].fPosX = 336.f;
	m_arrCreateObjcet[124].fPosY = -100.f;
	m_arrCreateObjcet[124].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[125].iAppearance = 1598;
	m_arrCreateObjcet[125].fPosX = 536.f;
	m_arrCreateObjcet[125].fPosY = -100.f;
	m_arrCreateObjcet[125].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[126].iAppearance = 1604;
	m_arrCreateObjcet[126].fPosX = 36.f;
	m_arrCreateObjcet[126].fPosY = -100.f;
	m_arrCreateObjcet[126].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[127].iAppearance = 1604;
	m_arrCreateObjcet[127].fPosX = 636.f;
	m_arrCreateObjcet[127].fPosY = -100.f;
	m_arrCreateObjcet[127].enType = OBJTYPE::ROCKET;

	// 로켓 세트
	m_arrCreateObjcet[128].iAppearance = 1615;
	m_arrCreateObjcet[128].fPosX = 236.f;
	m_arrCreateObjcet[128].fPosY = -100.f;
	m_arrCreateObjcet[128].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[129].iAppearance = 1615;
	m_arrCreateObjcet[129].fPosX = 436.f;
	m_arrCreateObjcet[129].fPosY = -100.f;
	m_arrCreateObjcet[129].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[130].iAppearance = 1620;
	m_arrCreateObjcet[130].fPosX = 136.f;
	m_arrCreateObjcet[130].fPosY = -100.f;
	m_arrCreateObjcet[130].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[131].iAppearance = 1620;
	m_arrCreateObjcet[131].fPosX = 336.f;
	m_arrCreateObjcet[131].fPosY = -100.f;
	m_arrCreateObjcet[131].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[132].iAppearance = 1620;
	m_arrCreateObjcet[132].fPosX = 536.f;
	m_arrCreateObjcet[132].fPosY = -100.f;
	m_arrCreateObjcet[132].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[133].iAppearance = 1625;
	m_arrCreateObjcet[133].fPosX = 36.f;
	m_arrCreateObjcet[133].fPosY = -100.f;
	m_arrCreateObjcet[133].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[134].iAppearance = 1625;
	m_arrCreateObjcet[134].fPosX = 636.f;
	m_arrCreateObjcet[134].fPosY = -100.f;
	m_arrCreateObjcet[134].enType = OBJTYPE::ROCKET;

	// 로켓 세트
	m_arrCreateObjcet[135].iAppearance = 1625;
	m_arrCreateObjcet[135].fPosX = 236.f;
	m_arrCreateObjcet[135].fPosY = -100.f;
	m_arrCreateObjcet[135].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[136].iAppearance = 1625;
	m_arrCreateObjcet[136].fPosX = 436.f;
	m_arrCreateObjcet[136].fPosY = -100.f;
	m_arrCreateObjcet[136].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[137].iAppearance = 1630;
	m_arrCreateObjcet[137].fPosX = 136.f;
	m_arrCreateObjcet[137].fPosY = -100.f;
	m_arrCreateObjcet[137].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[138].iAppearance = 1630;
	m_arrCreateObjcet[138].fPosX = 336.f;
	m_arrCreateObjcet[138].fPosY = -100.f;
	m_arrCreateObjcet[138].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[139].iAppearance = 1630;
	m_arrCreateObjcet[139].fPosX = 536.f;
	m_arrCreateObjcet[139].fPosY = -100.f;
	m_arrCreateObjcet[139].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[140].iAppearance = 1635;
	m_arrCreateObjcet[140].fPosX = 36.f;
	m_arrCreateObjcet[140].fPosY = -100.f;
	m_arrCreateObjcet[140].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[141].iAppearance = 1635;
	m_arrCreateObjcet[141].fPosX = 636.f;
	m_arrCreateObjcet[141].fPosY = -100.f;
	m_arrCreateObjcet[141].enType = OBJTYPE::ROCKET;

	// 로켓 세트
	m_arrCreateObjcet[142].iAppearance = 1635;
	m_arrCreateObjcet[142].fPosX = 236.f;
	m_arrCreateObjcet[142].fPosY = -100.f;
	m_arrCreateObjcet[142].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[143].iAppearance = 1635;
	m_arrCreateObjcet[143].fPosX = 436.f;
	m_arrCreateObjcet[143].fPosY = -100.f;
	m_arrCreateObjcet[143].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[144].iAppearance = 1640;
	m_arrCreateObjcet[144].fPosX = 136.f;
	m_arrCreateObjcet[144].fPosY = -100.f;
	m_arrCreateObjcet[144].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[145].iAppearance = 1640;
	m_arrCreateObjcet[145].fPosX = 336.f;
	m_arrCreateObjcet[145].fPosY = -100.f;
	m_arrCreateObjcet[145].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[146].iAppearance = 1640;
	m_arrCreateObjcet[146].fPosX = 536.f;
	m_arrCreateObjcet[146].fPosY = -100.f;
	m_arrCreateObjcet[146].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[147].iAppearance = 1645;
	m_arrCreateObjcet[147].fPosX = 36.f;
	m_arrCreateObjcet[147].fPosY = -100.f;
	m_arrCreateObjcet[147].enType = OBJTYPE::ROCKET;

	m_arrCreateObjcet[148].iAppearance = 1645;
	m_arrCreateObjcet[148].fPosX = 636.f;
	m_arrCreateObjcet[148].fPosY = -100.f;
	m_arrCreateObjcet[148].enType = OBJTYPE::ROCKET;
#pragma endregion
}

void CStage::Create_Object_Stage_1()
{
	CObj* pObj;
	CREATE_OBJECT* pObjectInfo;

	for (int iCnt = 0; iCnt < 300; ++iCnt)
	{
		if (m_arrCreateObjcet[iCnt].enType == OBJTYPE::NO_OBJECT)
			continue;

		if (m_arrCreateObjcet[iCnt].bFlag)
			continue;

		if ((float)BACKGROUND_TOTAL_HEIGHT - m_pBackground->Get_Info().fY < m_arrCreateObjcet[iCnt].iAppearance
			|| (float)BACKGROUND_TOTAL_HEIGHT - m_pBackground->Get_Info().fY - BACKGROUND_SPEED > m_arrCreateObjcet[iCnt].iAppearance)
			continue;

		pObjectInfo = &m_arrCreateObjcet[iCnt];

		switch (pObjectInfo->enType)
		{
		case OBJTYPE::ROCKET:
			pObj = CAbstractFactory<CRocket>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::AIR_MONSTER);
			pObj->Set_Pos(pObjectInfo->fPosX, pObjectInfo->fPosY);				// 리스폰 위치
			pObjectInfo->bFlag = true;	// 생성함
			break;
		case OBJTYPE::BLUE:
			pObj = CAbstractFactory<CBlue>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::AIR_MONSTER);
			pObj->Set_Pos(pObjectInfo->fPosX, pObjectInfo->fPosY);				// 리스폰 위치
			if (g_pPlayer_1)				// 타겟
				pObj->Set_Target(g_pPlayer_1);
			else
				pObj->Set_Target(g_pPlayer_2);
			static_cast<CBlue*>(pObj)->Set_DestPos(pObjectInfo->fDestPosX, pObjectInfo->fDestPosY);	// 목적지
			static_cast<CBlue*>(pObj)->Set_OutPos(pObjectInfo->fOutPosX, pObjectInfo->fOutPosY);		// 퇴장 위치
			static_cast<CBlue*>(pObj)->Set_OutTime(pObjectInfo->dwOutTime);		// 화면에 보이는 시간	
			static_cast<CBlue*>(pObj)->Set_DestAngle(pObjectInfo->fDestAngle);		// 배치 선회 각도
			static_cast<CBlue*>(pObj)->Set_OutAngle(pObjectInfo->fOutAngle);		// 퇴장 선회 각도
			pObjectInfo->bFlag = true;	// 생성함
			break;
		case OBJTYPE::RED:
			pObj = CAbstractFactory<CRed>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::AIR_MONSTER);
			pObj->Set_Pos(pObjectInfo->fPosX, pObjectInfo->fPosY);				// 리스폰 위치
			if (g_pPlayer_1)				// 타겟
				pObj->Set_Target(g_pPlayer_1);
			else
				pObj->Set_Target(g_pPlayer_2);
			static_cast<CRed*>(pObj)->Set_DestPos(pObjectInfo->fDestPosX, pObjectInfo->fDestPosY);		// 목적지
			static_cast<CRed*>(pObj)->Set_OutPos(pObjectInfo->fOutPosX, pObjectInfo->fOutPosY);		// 퇴장 위치
			static_cast<CRed*>(pObj)->Set_OutTime(pObjectInfo->dwOutTime);			// 화면에 보이는 시간	
			static_cast<CRed*>(pObj)->Set_DestAngle(pObjectInfo->fDestAngle);		// 배치 선회 각도
			static_cast<CRed*>(pObj)->Set_OutAngle(pObjectInfo->fOutAngle);		// 퇴장 선회 각도
			pObjectInfo->bFlag = true;	// 생성함
			break;
		case OBJTYPE::CANNON:
			pObj = CAbstractFactory<CCannon>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::UPPER_MONSTER);	// 지붕위에 있어야되서 지상몹보다 높은 위치
			pObj->Set_Pos(pObjectInfo->fPosX, pObjectInfo->fPosY);				// 리스폰 위치
			if (g_pPlayer_1)				// 타겟
				pObj->Set_Target(g_pPlayer_1);
			else
				pObj->Set_Target(g_pPlayer_2);
			static_cast<CCannon*>(pObj)->Set_DestPos(pObjectInfo->fDestPosY);	// 숨어있다가 나올 타이밍
			pObjectInfo->bFlag = true;	// 생성함
			break;
		case OBJTYPE::CACTUS_CANNON:
			pObj = CAbstractFactory<CCactusCannon>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::GROUND_MONSTER);
			pObj->Set_Pos(pObjectInfo->fPosX, pObjectInfo->fPosY);				// 리스폰 위치
			if (g_pPlayer_1)				// 타겟
				pObj->Set_Target(g_pPlayer_1);
			else
				pObj->Set_Target(g_pPlayer_2);
			static_cast<CCactusCannon*>(pObj)->Set_DestPos(pObjectInfo->fDestPosY);	// 숨어있다가 나올 타이밍
			pObjectInfo->bFlag = true;	// 생성함
			break;
		case OBJTYPE::MOVER_CANNON:
			pObj = CAbstractFactory<CMoverCannon>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::GROUND_MONSTER);
			pObj->Set_Pos(pObjectInfo->fPosX, pObjectInfo->fPosY);				// 리스폰 위치
			if(g_pPlayer_1)				// 타겟
				pObj->Set_Target(g_pPlayer_1);
			else
				pObj->Set_Target(g_pPlayer_2);
			static_cast<CMoverCannon*>(pObj)->Set_DestPos(pObjectInfo->fDestPosX, pObjectInfo->fDestPosY);		// 목적지
			pObjectInfo->bFlag = true;	// 생성함
			break;
		case OBJTYPE::ROBOT_BLUE:
			pObj = CAbstractFactory<CRobotBlue>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::GROUND_MONSTER);
			pObj->Set_Pos(pObjectInfo->fPosX, pObjectInfo->fPosY);				// 리스폰 위치
			if (g_pPlayer_1)				// 타겟
				pObj->Set_Target(g_pPlayer_1);
			else
				pObj->Set_Target(g_pPlayer_2);
			pObjectInfo->bFlag = true;	// 생성함
			break;
		case OBJTYPE::RED_BEANIE:
			pObj = CAbstractFactory<CRedBeanie>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::AIR_MONSTER);
			pObj->Set_Pos(pObjectInfo->fPosX, pObjectInfo->fPosY);				// 리스폰 위치
			if (g_pPlayer_1)				// 타겟
				pObj->Set_Target(g_pPlayer_1);
			else
				pObj->Set_Target(g_pPlayer_2);
			static_cast<CRedBeanie*>(pObj)->Set_DestPos(pObjectInfo->fDestPosY);		// 목적지
			pObjectInfo->bFlag = true;	// 생성함
			break;
		case OBJTYPE::FACTORY:
			pObj = CAbstractFactory<CFactory>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::GROUND_MONSTER);
			pObj->Set_Pos(pObjectInfo->fPosX, pObjectInfo->fPosY);				// 리스폰 위치
			if (g_pPlayer_1)				// 타겟
				pObj->Set_Target(g_pPlayer_1);
			else
				pObj->Set_Target(g_pPlayer_2);
			pObjectInfo->bFlag = true;	// 생성함
			break;
		case OBJTYPE::ROBOT_SHARK:
			pObj = CAbstractFactory<CRobotShark>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::AIR_MONSTER);
			pObj->Set_Pos(pObjectInfo->fPosX, pObjectInfo->fPosY);				// 리스폰 위치
			static_cast<CRobotShark*>(pObj)->Set_DestPos(pObjectInfo->fDestPosX, pObjectInfo->fDestPosY);		// 목적지
			static_cast<CRobotShark*>(pObj)->Set_OutPos(pObjectInfo->fOutPosX, pObjectInfo->fOutPosY);		// 퇴장 위치
			static_cast<CRobotShark*>(pObj)->Set_OutTime(pObjectInfo->dwOutTime);		// 화면에 보이는 시간	
			pObjectInfo->bFlag = true;	// 생성함
			break;
		case OBJTYPE::STAGE_1_SIGN:
			pObj = CAbstractFactory<CStaticObj>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MAP_OBJECT);
			pObj->Set_Pos(pObjectInfo->fPosX, pObjectInfo->fPosY);				// 리스폰 위치
			pObj->Set_FrameKey(L"Stage1_Sign");
			pObj->Set_ImageSize(STAGE_1_SIGN_WIDTH, STAGE_1_SIGN_HEIGHT);
			pObjectInfo->bFlag = true;	// 생성함
			break;
		case OBJTYPE::STAGE_1_ROOF:
			pObj = CAbstractFactory<CStaticObj>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MAP_OBJECT);
			pObj->Set_Pos(pObjectInfo->fPosX, pObjectInfo->fPosY);				// 리스폰 위치
			pObj->Set_FrameKey(L"Stage1_Roof");
			pObj->Set_ImageSize(STAGE_1_ROOF_WIDTH, STAGE_1_ROOF_HEIGHT);
			pObjectInfo->bFlag = true;	// 생성함
			break;
		case OBJTYPE::STAGE_1_SHOP:
			pObj = CAbstractFactory<CStaticObj>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MAP_OBJECT);
			pObj->Set_Pos(pObjectInfo->fPosX, pObjectInfo->fPosY);				// 리스폰 위치
			pObj->Set_FrameKey(L"Stage1_Shop");
			pObj->Set_ImageSize(STAGE_1_SHOP_WIDTH, STAGE_1_SHOP_HEIGHT);
			pObjectInfo->bFlag = true;	// 생성함
			break;
		case OBJTYPE::STAGE_1_SHOP_ROOF:
			pObj = CAbstractFactory<CStaticObj>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MAP_OBJECT);
			pObj->Set_Pos(pObjectInfo->fPosX, pObjectInfo->fPosY);				// 리스폰 위치
			pObj->Set_FrameKey(L"Stage1_Shop_Roof");
			pObj->Set_ImageSize(STAGE_1_SHOP_ROOF_WIDTH, STAGE_1_SHOP_ROOF_HEIGHT);
			pObjectInfo->bFlag = true;	// 생성함
			break;
		case OBJTYPE::STAGE_1_BROKEN_SIGN:
			pObj = CAbstractFactory<CBrokenStatic>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MAP_OBJECT);
			pObj->Set_Pos(pObjectInfo->fPosX, pObjectInfo->fPosY);				// 리스폰 위치
			pObj->Set_FrameKey(L"Stage1_BrokenSign");
			pObj->Set_ImageSize(STAGE_1_BROKEN_SIGN_WIDTH, STAGE_1_BROKEN_SIGN_HEIGHT);
			pObj->Set_HitRectPos(RECT{ 15 * 3, 69 * 3, 122 * 3, 147 * 3 });
			pObj->Set_Hp(600.f);		// 체력
			pObjectInfo->bFlag = true;	// 생성함
			break;
		case OBJTYPE::STAGE_1_RIGHT_ROCK:
			pObj = CAbstractFactory<CMoveStatic>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MAP_OBJECT);
			pObj->Set_Pos(pObjectInfo->fPosX, pObjectInfo->fPosY);				// 리스폰 위치
			pObj->Set_FrameKey(L"Stage1_RightRock");
			pObj->Set_Speed(1.f);
			pObj->Set_ImageSize(STAGE_1_RIGHT_ROCK_WIDTH, STAGE_1_RIGHT_ROCK_HEIGHT);
			static_cast<CMoveStatic*>(pObj)->Set_DestPos(pObjectInfo->fDestPosX, pObjectInfo->fDestPosY);	// 이동할 타이밍
			pObjectInfo->bFlag = true;	// 생성함
			break;
		case OBJTYPE::STAGE_1_LEFT_ROCK:
			pObj = CAbstractFactory<CMoveStatic>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MAP_OBJECT);
			pObj->Set_Pos(pObjectInfo->fPosX, pObjectInfo->fPosY);				// 리스폰 위치
			pObj->Set_FrameKey(L"Stage1_LeftRock");
			pObj->Set_Speed(1.f);
			pObj->Set_ImageSize(STAGE_1_LEFT_ROCK_WIDTH, STAGE_1_LEFT_ROCK_HEIGHT);
			static_cast<CMoveStatic*>(pObj)->Set_DestPos(pObjectInfo->fDestPosX, pObjectInfo->fDestPosY);	// 이동할 타이밍
			pObjectInfo->bFlag = true;	// 생성함
			break;
		case OBJTYPE::STAGE_1_BRIDGE:
			pObj = CAbstractFactory<CStaticObj>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MAP_OBJECT);
			pObj->Set_Pos(pObjectInfo->fPosX, pObjectInfo->fPosY);				// 리스폰 위치
			pObj->Set_FrameKey(L"Stage1_Bridge");
			pObj->Set_ImageSize(STAGE_1_BRIDGE_WIDTH, STAGE_1_BRIDGE_HEIGHT);
			pObjectInfo->bFlag = true;	// 생성함
			break;
		case OBJTYPE::SHARK:
			pObj = CAbstractFactory<CShark>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::NPC);
			pObj->Set_Pos(pObjectInfo->fPosX, pObjectInfo->fPosY);				// 리스폰 위치
			static_cast<CShark*>(pObj)->Set_DestPos(pObjectInfo->fDestPosX, pObjectInfo->fDestPosY);	// 이동 위치
			pObjectInfo->bFlag = true;	// 생성함
			break;
		case OBJTYPE::GIMMICK:
			pObj = CAbstractFactory<CGimmick>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::NPC);
			pObj->Set_Pos(pObjectInfo->fPosX, pObjectInfo->fPosY);				// 리스폰 위치
			static_cast<CGimmick*>(pObj)->Set_DestPos(pObjectInfo->fDestPosX, pObjectInfo->fDestPosY);	// 이동 위치
			pObjectInfo->bFlag = true;	// 생성함
			break;
		case OBJTYPE::BLADE:
			pObj = CAbstractFactory<CBlade>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::NPC);
			pObj->Set_Pos(pObjectInfo->fPosX, pObjectInfo->fPosY);				// 리스폰 위치
			static_cast<CBlade*>(pObj)->Set_DestPos(pObjectInfo->fDestPosX, pObjectInfo->fDestPosY);	// 이동 위치
			pObjectInfo->bFlag = true;	// 생성함
			break;
		case OBJTYPE::SECTOR:
			pObj = CAbstractFactory<CSector>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::SECTOR);
			pObj->Set_Pos(pObjectInfo->fPosX, pObjectInfo->fPosY);				// 리스폰 위치
			pObj->Set_Size(pObjectInfo->iWidth, pObjectInfo->iHeight);			// 크기
			static_cast<CSector*>(pObj)->Set_DestPos(pObjectInfo->fDestPosX, pObjectInfo->fDestPosY);	// 이동 위치
			pObjectInfo->bFlag = true;	// 생성함
			break;
		case OBJTYPE::TEQUILA_GROUND:
			pObj = CAbstractFactory<CTequilaGround>::Create();
			CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::GROUND_BOSS);
			pObj->Set_Pos(pObjectInfo->fPosX, pObjectInfo->fPosY);				// 리스폰 위치
			if (g_pPlayer_1)				// 타겟
				pObj->Set_Target(g_pPlayer_1);
			else
				pObj->Set_Target(g_pPlayer_2);
			pObjectInfo->bFlag = true;	// 생성함
			break;
		}
	}
}

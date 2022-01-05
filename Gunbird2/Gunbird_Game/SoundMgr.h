// 기명준
// 사운드 매니저

#pragma once
class CSoundMgr
{
public:
	static CSoundMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSoundMgr; 

		return m_pInstance; 
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance; 
			m_pInstance = nullptr; 
		}
	}
public:
	enum CHANNELID {BGM, PLAYER_POWER_UP, PLAYER_CHARGE, PLAYER_BOMB, PLAYER_BOMB_EFFECT,
		PLAYER_DEAD, PLAYER_SHOT, PLAYER_GUIDEDSHOT, PLAYER_CHARGE_BULLET, PLAYER_CHARGE_BULLET_HIT, PLAYER_MELEE,
		MONSTER, EXPLOSION_1, EXPLOSION_2, EXPLOSION_3, EXPLOSION_4, UI, ITEM, BOSS, BOSS_ATTACK, MAXCHANNEL};
private:
	CSoundMgr();
	~CSoundMgr();

public:
	void Initialize(); 

	void Release(); 
public:
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID);
	void PlayBGM(TCHAR* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();

private:
	void LoadSoundFile(); 

private:
	static CSoundMgr* m_pInstance; 
	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound; 
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL]; 
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem; 

};


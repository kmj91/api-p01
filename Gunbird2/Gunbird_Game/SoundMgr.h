// �����
// ���� �Ŵ���

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
	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD_SOUND*> m_mapSound; 
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL]; 
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem; 

};


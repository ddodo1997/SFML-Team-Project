#pragma once
class SoundMgr : public Singleton<SoundMgr>
{
	friend Singleton<SoundMgr>;

protected:

	SoundMgr() = default;
	~SoundMgr() = default;

	SoundMgr(const SoundMgr&) = delete;
	SoundMgr& operator=(const SoundMgr&) = delete;

	sf::Sound bgm;

	std::list<sf::Sound*> playing;
	std::list<sf::Sound*> waiting;

	float sfxVolume = 100.f;
	float bgmVolume = 100.f;

public:
	void SetBgmVolume(float v) 
	{ 
		bgmVolume = v; 
		bgm.setVolume(bgmVolume);
	}

	void Init(int totalChannels = 64);
	void Release();
	void Update(float dt);

	void PlayBgm(std::string id, bool loop = true);
	void PlayBgm(sf::SoundBuffer& buffer, bool loop = true);
	void StopBgm();
	
	void PlaySfx(std::string id);
	void PlaySfx(sf::SoundBuffer& buffer);

	void SetSfxVolume(float v);
	void StopAllSfx();
};

#define SOUND_MGR (SoundMgr::Instance())
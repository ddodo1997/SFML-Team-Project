#pragma once

class Player;

class UiHudL : public GameObject
{
protected:	
	Player* player;

	//sf::Shader invertShader;
	//sf::RenderTexture renderTexture;
	//float fade = 0.f;
	//bool fadingIn = true;


	// Pause 관련
	bool isPaused = false;
	bool wasPaused = false;
	bool isResuming = false;

	sf::RectangleShape pauseShader;
	sf::RectangleShape pauseTopBox;
	sf::RectangleShape pauseBottomBox;
	float pauseTimer = 0.0;
	float resumeTimer = 0.0;

	std::vector<sf::Sprite*> pauseText;
	std::vector<sf::Sprite*> pauseTextResume;
	std::vector<sf::Sprite*> pauseTextRestart;
	std::vector<sf::Sprite*> pauseTextQuit;

	sf::Vector2f dscb = { 13, 41 };
	float colorRotator = 0.f;
	float colorCyclingDuration = 6.f;

	int selectedFunctionPause = 0;

	// Mouse 관련
	sf::Sprite mouseSprite;
	Animator cursorAnimator;

	float realDt = 0.f;

	sf::Text currentWeaponStatus;

	sf::Text volumeDisplayer;

	// BL Box 관련
	sf::RectangleShape boxBL;
	sf::Vector2f boxBLpos = { 0.f,960.f };
	sf::Vector2f boxBLtargetPos = { 0.f,960.f };
	sf::Vector2f boxBLdir = { 0.f,1.f };
	float boxBLspeed = 300.f;
	sf::Text textBL;
	sf::Text textBL2;
	float textBLrotationTimer = 0.5 * Utils::PI;
	float textBL2rotationTimer = 0.f;
	const float textBLrotationDuration = 2.4f;
	float rotationAngle = 0.f;

	bool preventEscRepeat = false;

	int weaponTypeIndex = -1;

	int bgmVol;	

public:
	UiHudL(const std::string& name = "");
	~UiHudL();

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;

	void ResetPauseContent();
	void ReleasePauseContent();

	void SetPlayer(Player* player) { this->player = player; }

	void Update(float dt) override;
	void UpdateBoxBL(float dt);
	void UpdateMouseSprite(float dt);
	void UpdateWeaponStatus(Weapon::WeaponStatus weaponStatus, int remainingBullet);
	void UpdateVolumeDisplay(float dt);

	void ResetPaused();
	void UpdatePaused(float realDt);
	void UpdatePausedContents(float realDt);

	void Resume();
	void Restart();
	void Quit();

	void UpdatePausedContent(float realDt);

	bool IsPaused() { return isPaused; }

	void Draw(sf::RenderWindow& window) override;
};

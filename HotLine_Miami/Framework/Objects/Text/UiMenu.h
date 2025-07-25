#pragma once

class UiMenu : public GameObject
{
protected: 
	sf::Sprite mainSpr;
	sf::Sprite mainSpr2;
	sf::RenderStates renderState;
	sf::Transform transform;
	
	std::vector<sf::Sprite*> mainTextStartGame;
	std::vector<sf::Sprite*> mainTextContinue;
	std::vector<sf::Sprite*> mainTextOption;
	std::vector<sf::Sprite*> mainEditorMode;
	std::vector<sf::Sprite*> mainTextExit;

	std::vector<sf::Sprite*> mainTextStartGame2;
	std::vector<sf::Sprite*> mainTextOption2;
	std::vector<sf::Sprite*> mainEditorMode2;
	std::vector<sf::Sprite*> mainTextExit2;

	std::vector<sf::Sprite*> optionTextVolume;
	std::vector<sf::Sprite*> optionTextVolume2;

	std::vector<sf::Sprite*> volumeTextMusic;
	std::vector<sf::Sprite*> volumeTextSfx;

	std::vector<sf::Sprite*> volumeTextMusic2;
	std::vector<sf::Sprite*> volumeTextSfx2;

	bool isOnMainMenu = true;
	int mainMenuIndex = 0;

	bool isOnOption = false;
	int optionIndex = 0;

	bool isOnVolume = false;
	int volumeIndex = 0;

	float keyInputDuration = 0.f;
	bool keyInputEnabled = true;

	bool repeatPreventerEnterKey = false;
	bool repeatPreventerEscKey = false;

	float rotationTimerMenuContent = 0.f;
	float roataionTimerTitleContent = 0.f;
	const float rotationDurationMenuContent = 2.4f;
	const float rotationDurationTitleContent = 4.5f;

public:
	UiMenu(const std::string& name = "UiMenu");
	~UiMenu() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;

	void ResetMenuContent();

	void Update(float dt) override;

	void UpdateMainMenu(float realDt);
	void UpdateMainMenuKey(float realDt);
	void UpdateOption(float realDt);
	void UpdateOptionKey(float realDt);
	void UpdateVolume(float realDt);
	void UpdateVolumeKey(float realDt);
	void UpdateRotationTitle(float realDt);
	void UpdateRotationMenu(float realDt);
	
	void OnVolumeChange(bool isBgm = true);

	void Draw(sf::RenderWindow& window) override;
};

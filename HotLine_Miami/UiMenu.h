#pragma once

class UiMenu : public GameObject
{
protected: 
	sf::Sprite mainSpr;
	sf::Sprite mainSpr2;
	
	std::vector<sf::Sprite*> mainTextStartGame;
	std::vector<sf::Sprite*> mainTextOption;
	std::vector<sf::Sprite*> mainTextExit;

	std::vector<sf::Sprite*> optionTextVolume;

	std::vector<sf::Sprite*> volumeTextMusic;
	std::vector<sf::Sprite*> volumeTextSfx;

	bool isOnMainMenu = true;
	int mainMenuIndex = 0;

	bool isOnOption = false;
	int optionIndex = 0;

	bool isOnVolume = false;
	int volumeIndex = 0;

	bool repeatPreventerEnterKey = false;
	bool repeatPreventerEscKey = false;

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

	void Draw(sf::RenderWindow& window) override;
};

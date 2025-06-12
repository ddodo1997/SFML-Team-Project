#include "stdafx.h"
#include "UiMenu.h"

UiMenu::UiMenu(const std::string& name)
	: GameObject(name)
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 0;
}

void UiMenu::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiMenu::SetRotation(float angle)
{
	rotation = angle;
}

void UiMenu::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiMenu::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiMenu::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiMenu::Init()
{

}

void UiMenu::Release()
{

}

void UiMenu::Reset()
{
	
	
	ResetMenuContent();
}

void UiMenu::ResetMenuContent()
{
	rotationTimerMenuContent = 0.f;
	roataionTimerTitleContent = 0.f;

	isOnMainMenu = true;
	mainMenuIndex = 0;
	isOnOption = false;
	optionIndex = 0;
	isOnVolume = false;
	volumeIndex = 0;

	sf::Vector2f alphabetScale = { 4.f,4.f };
	sf::Vector2f posCalibration = { 0.f, -350.f };

	auto wSizeHalf = FRAMEWORK.GetWindowSizeF() * 0.5f;
	mainSpr.setTexture(TEXTURE_MGR.Get("graphics/ETC/MainMenuScene/sprTitle_0.png"));
	mainSpr.setPosition(wSizeHalf + posCalibration);
	mainSpr.setScale(alphabetScale);
	mainSpr.setColor(sf::Color::Cyan);
	Utils::SetOrigin(mainSpr, Origins::MC);

	mainSpr2.setTexture(TEXTURE_MGR.Get("graphics/ETC/MainMenuScene/sprTitle_1.png"));
	mainSpr2.setPosition(wSizeHalf + posCalibration);
	mainSpr2.setScale(alphabetScale);
	mainSpr2.setColor(sf::Color::Red);
	Utils::SetOrigin(mainSpr2, Origins::MC);

	std::string path = ALPHABET_TABLE->GetSpirtePath();
	std::string tempText = "START GAME"; 
	sf::Vector2f textPos = { 0.f,0.f };
	sf::Vector2f textDefaultPos = { 0.f,450.f };
	sf::Vector2f alphabetOrigin = { 960.f,700.f };
	sf::Vector2i alphabetHalfsize = ALPHABET_TABLE->Get(tempText.front()).texCoord.getSize() / 2;
	int totalAlphabetWidth = 0;
	float verticalTextInterval = 25.f * alphabetScale.y;

	for (int i = 0; i < tempText.size(); ++i)
	{
		totalAlphabetWidth += ALPHABET_TABLE->Get(tempText[i]).characterWidth;
	}

	textDefaultPos.x = FRAMEWORK.GetWindowSizeF().x * 0.5f - totalAlphabetWidth * 0.5f * alphabetScale.x;

	for (int i = 0; i < tempText.size(); ++i)
	{
		sf::Sprite* alphabetSprite = new sf::Sprite(TEXTURE_MGR.Get(path), ALPHABET_TABLE->Get(tempText[i]).texCoord);
		alphabetSprite->setPosition(textDefaultPos + textPos + alphabetOrigin * 0.8f);
		alphabetSprite->setScale(alphabetScale);
		alphabetSprite->setOrigin(alphabetOrigin*0.2f);
		textPos.x += ALPHABET_TABLE->Get(tempText[i]).characterWidth * alphabetScale.x;
		mainTextStartGame.push_back(alphabetSprite);
	}

	tempText = "VOLUME";
	textPos = { 0.f,0.f };
	alphabetScale = { 4.f,4.f };
	totalAlphabetWidth = 0;
	verticalTextInterval = 25.f * alphabetScale.y;

	for (int i = 0; i < tempText.size(); ++i)
	{
		totalAlphabetWidth += ALPHABET_TABLE->Get(tempText[i]).characterWidth;
	}

	textDefaultPos.x = FRAMEWORK.GetWindowSizeF().x * 0.5f - totalAlphabetWidth * 0.5f * alphabetScale.x;

	for (int i = 0; i < tempText.size(); ++i)
	{
		sf::Sprite* alphabetSprite = new sf::Sprite(TEXTURE_MGR.Get(path), ALPHABET_TABLE->Get(tempText[i]).texCoord);
		alphabetSprite->setPosition(textDefaultPos + textPos + alphabetOrigin * 0.8f);
		alphabetSprite->setScale(alphabetScale);
		alphabetSprite->setOrigin(alphabetOrigin * 0.2f);
		textPos.x += ALPHABET_TABLE->Get(tempText[i]).characterWidth * alphabetScale.x;
		optionTextVolume.push_back(alphabetSprite);
	}

	int tempVol = SOUND_MGR.GetBgmVolume();
	tempText = "MUSIC  VOLUME " + std::to_string(tempVol) + "%";
	textPos = { 0.f,0.f };
	alphabetScale = { 4.f,4.f };
	totalAlphabetWidth = 0;
	verticalTextInterval = 25.f * alphabetScale.y;

	for (int i = 0; i < tempText.size(); ++i)
	{
		totalAlphabetWidth += ALPHABET_TABLE->Get(tempText[i]).characterWidth;
	}

	textDefaultPos.x = FRAMEWORK.GetWindowSizeF().x * 0.5f - totalAlphabetWidth * 0.5f * alphabetScale.x;

	for (int i = 0; i < tempText.size(); ++i)
	{
		sf::Sprite* alphabetSprite = new sf::Sprite(TEXTURE_MGR.Get(path), ALPHABET_TABLE->Get(tempText[i]).texCoord);
		alphabetSprite->setPosition(textDefaultPos + textPos + alphabetOrigin * 0.8f);
		alphabetSprite->setScale(alphabetScale);
		alphabetSprite->setOrigin(alphabetOrigin * 0.2f);
		textPos.x += ALPHABET_TABLE->Get(tempText[i]).characterWidth * alphabetScale.x;
		volumeTextMusic.push_back(alphabetSprite);
	}

	tempText = "CONTINUE";
	textPos = { 0.f,0.f };
	alphabetScale = { 4.f,4.f };
	totalAlphabetWidth = 0;
	verticalTextInterval = 25.f * alphabetScale.y;

	for (int i = 0; i < tempText.size(); ++i)
	{
		totalAlphabetWidth += ALPHABET_TABLE->Get(tempText[i]).characterWidth;
	}

	textDefaultPos.x = FRAMEWORK.GetWindowSizeF().x * 0.5f - totalAlphabetWidth * 0.5f * alphabetScale.x;
	textDefaultPos.y += verticalTextInterval;

	for (int i = 0; i < tempText.size(); ++i)
	{
		sf::Sprite* alphabetSprite = new sf::Sprite(TEXTURE_MGR.Get(path), ALPHABET_TABLE->Get(tempText[i]).texCoord);
		alphabetSprite->setPosition(textDefaultPos + textPos + alphabetOrigin * 0.8f);
		alphabetSprite->setScale(alphabetScale);
		alphabetSprite->setOrigin(alphabetOrigin * 0.2f);
		textPos.x += ALPHABET_TABLE->Get(tempText[i]).characterWidth * alphabetScale.x;
		mainTextContinue.push_back(alphabetSprite);
	}

	tempVol = SOUND_MGR.GetSfxVolume();
	tempText = "SFX  VOLUME " + std::to_string(tempVol) + "%";
	textPos = { 0.f,0.f };
	alphabetScale = { 4.f,4.f };
	totalAlphabetWidth = 0;
	verticalTextInterval = 25.f * alphabetScale.y;

	for (int i = 0; i < tempText.size(); ++i)
	{
		totalAlphabetWidth += ALPHABET_TABLE->Get(tempText[i]).characterWidth;
	}

	textDefaultPos.x = FRAMEWORK.GetWindowSizeF().x * 0.5f - totalAlphabetWidth * 0.5f * alphabetScale.x;

	for (int i = 0; i < tempText.size(); ++i)
	{
		sf::Sprite* alphabetSprite = new sf::Sprite(TEXTURE_MGR.Get(path), ALPHABET_TABLE->Get(tempText[i]).texCoord);
		alphabetSprite->setPosition(textDefaultPos + textPos + alphabetOrigin * 0.8f);
		alphabetSprite->setScale(alphabetScale);
		alphabetSprite->setOrigin(alphabetOrigin * 0.2f);
		textPos.x += ALPHABET_TABLE->Get(tempText[i]).characterWidth * alphabetScale.x;
		volumeTextSfx.push_back(alphabetSprite);
	}

	tempText = "OPTION";
	textPos = { 0.f,0.f };
	alphabetScale = { 4.f,4.f };
	totalAlphabetWidth = 0;
	verticalTextInterval = 25.f * alphabetScale.y;

	for (int i = 0; i < tempText.size(); ++i)
	{
		totalAlphabetWidth += ALPHABET_TABLE->Get(tempText[i]).characterWidth;
	}

	textDefaultPos.x = FRAMEWORK.GetWindowSizeF().x * 0.5f - totalAlphabetWidth * 0.5f * alphabetScale.x;
	textDefaultPos.y += verticalTextInterval;

	for (int i = 0; i < tempText.size(); ++i)
	{
		sf::Sprite* alphabetSprite = new sf::Sprite(TEXTURE_MGR.Get(path), ALPHABET_TABLE->Get(tempText[i]).texCoord);
		alphabetSprite->setPosition(textDefaultPos + textPos + alphabetOrigin * 0.8f);
		alphabetSprite->setScale(alphabetScale);
		alphabetSprite->setOrigin(alphabetOrigin * 0.2f);
		textPos.x += ALPHABET_TABLE->Get(tempText[i]).characterWidth * alphabetScale.x;
		mainTextOption.push_back(alphabetSprite);
	}

	tempText = "EDITOR MODE";
	textPos = { 0.f,0.f };
	alphabetScale = { 4.f,4.f };
	totalAlphabetWidth = 0;
	verticalTextInterval = 25.f * alphabetScale.y;

	for (int i = 0; i < tempText.size(); ++i)
	{
		totalAlphabetWidth += ALPHABET_TABLE->Get(tempText[i]).characterWidth;
	}

	textDefaultPos.x = FRAMEWORK.GetWindowSizeF().x * 0.5f - totalAlphabetWidth * 0.5f * alphabetScale.x;
	textDefaultPos.y += verticalTextInterval;

	for (int i = 0; i < tempText.size(); ++i)
	{
		sf::Sprite* alphabetSprite = new sf::Sprite(TEXTURE_MGR.Get(path), ALPHABET_TABLE->Get(tempText[i]).texCoord);
		alphabetSprite->setPosition(textDefaultPos + textPos + alphabetOrigin * 0.8f);
		alphabetSprite->setScale(alphabetScale);
		alphabetSprite->setOrigin(alphabetOrigin * 0.2f);
		textPos.x += ALPHABET_TABLE->Get(tempText[i]).characterWidth * alphabetScale.x;
		mainEditorMode.push_back(alphabetSprite);
	}

	tempText = "EXIT GAME";
	textPos = { 0.f,0.f };
	alphabetScale = { 4.f,4.f };
	totalAlphabetWidth = 0;
	verticalTextInterval = 25.f * alphabetScale.y;

	for (int i = 0; i < tempText.size(); ++i)
	{
		totalAlphabetWidth += ALPHABET_TABLE->Get(tempText[i]).characterWidth;
	}

	textDefaultPos.x = FRAMEWORK.GetWindowSizeF().x * 0.5f - totalAlphabetWidth * 0.5f * alphabetScale.x;
	textDefaultPos.y += verticalTextInterval;

	for (int i = 0; i < tempText.size(); ++i)
	{
		sf::Sprite* alphabetSprite = new sf::Sprite(TEXTURE_MGR.Get(path), ALPHABET_TABLE->Get(tempText[i]).texCoord);
		alphabetSprite->setPosition(textDefaultPos + textPos + alphabetOrigin * 0.8f);
		alphabetSprite->setScale(alphabetScale);
		alphabetSprite->setOrigin(alphabetOrigin * 0.2f);
		textPos.x += ALPHABET_TABLE->Get(tempText[i]).characterWidth * alphabetScale.x;
		mainTextExit.push_back(alphabetSprite);
	}
}

void UiMenu::Update(float dt)
{
	keyInputDuration += FRAMEWORK.GetRealDeltaTime();
	if (keyInputDuration > 0.02f)
	{
		keyInputDuration = 0.f;
		keyInputEnabled = true;
	}

	if (isOnMainMenu)
		UpdateMainMenuKey(FRAMEWORK.GetRealDeltaTime());
	if (isOnOption)
		UpdateOptionKey(FRAMEWORK.GetRealDeltaTime());
	if (isOnVolume)
		UpdateVolumeKey(FRAMEWORK.GetRealDeltaTime());

	UpdateMainMenu(FRAMEWORK.GetRealDeltaTime());
	UpdateOption(FRAMEWORK.GetRealDeltaTime());
	UpdateVolume(FRAMEWORK.GetRealDeltaTime());
	UpdateRotationTitle(FRAMEWORK.GetRealDeltaTime());
	//UpdateRotationMenu(FRAMEWORK.GetRealDeltaTime());

	repeatPreventerEnterKey = false;
	repeatPreventerEscKey = false;
}

void UiMenu::UpdateMainMenu(float realDt)
{
	if (mainMenuIndex == 0)
	{
		for (auto alphabetSprite : mainTextStartGame)
		{
			alphabetSprite->setColor(sf::Color::Magenta);
		}
	}
	else
	{
		for (auto alphabetSprite : mainTextStartGame)
		{
			alphabetSprite->setColor(sf::Color::White);
		}
	}

	if (mainMenuIndex == 1)
	{
		for (auto alphabetSprite : mainTextContinue)
		{
			alphabetSprite->setColor(sf::Color::Magenta);
		}
	}
	else
	{
		for (auto alphabetSprite : mainTextContinue)
		{
			alphabetSprite->setColor(sf::Color::White);
		}
	}

	if (mainMenuIndex == 2)
	{
		for (auto alphabetSprite : mainTextOption)
		{
			alphabetSprite->setColor(sf::Color::Magenta);
		}
	}
	else
	{
		for (auto alphabetSprite : mainTextOption)
		{
			alphabetSprite->setColor(sf::Color::White);
		}
	}

	if (mainMenuIndex == 3)
	{
		for (auto alphabetSprite : mainEditorMode)
		{
			alphabetSprite->setColor(sf::Color::Magenta);
		}
	}
	else
	{
		for (auto alphabetSprite : mainEditorMode)
		{
			alphabetSprite->setColor(sf::Color::White);
		}
	}

	if (mainMenuIndex == 4)
	{
		for (auto alphabetSprite : mainTextExit)
		{
			alphabetSprite->setColor(sf::Color::Magenta);
		}
	}
	else
	{
		for (auto alphabetSprite : mainTextExit)
		{
			alphabetSprite->setColor(sf::Color::White);
		}
	}
}

void UiMenu::UpdateMainMenuKey(float realDt)
{
	if (InputMgr::GetKeyDown(sf::Keyboard::W) || InputMgr::GetKeyDown(sf::Keyboard::Up))
	{
		mainMenuIndex--;
		if(mainMenuIndex == -1)
		{
			mainMenuIndex = 4;
		}
		mainMenuIndex = Utils::Clamp(mainMenuIndex, 0, 4);
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::S) || InputMgr::GetKeyDown(sf::Keyboard::Down))
	{
		mainMenuIndex++;
		if (mainMenuIndex == 5)
		{
			mainMenuIndex = 0;
		}
		mainMenuIndex = Utils::Clamp(mainMenuIndex, 0, 4);
	}

	if (!repeatPreventerEnterKey && mainMenuIndex == 0 && InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		STAGE_TABLE->SetCurrentStageIndex(0);
		repeatPreventerEnterKey = true;
		SCENE_MGR.ChangeScene(SceneIds::SceneGame);
		FRAMEWORK.SetTimeScale(1.f);
	}

	if (!repeatPreventerEnterKey &&
		mainMenuIndex == 1 && InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		// Continue 동작 넣으세요
		STAGE_TABLE->SetCurrentStageIndex(STAGE_TABLE->GetSavedStageIndex());
		repeatPreventerEnterKey = true;
		SCENE_MGR.ChangeScene(SceneIds::SceneGame);
		FRAMEWORK.SetTimeScale(1.f);
	}

	if (!repeatPreventerEnterKey &&
		mainMenuIndex == 2 && InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{		
		repeatPreventerEnterKey = true;
		isOnMainMenu = false;
		isOnOption = true;
	}

	if (!repeatPreventerEnterKey && mainMenuIndex == 3 && InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		repeatPreventerEnterKey = true;
		SCENE_MGR.ChangeScene(SceneIds::SceneEditor);
		FRAMEWORK.SetTimeScale(1.f);
	}

	if (!repeatPreventerEnterKey && mainMenuIndex == 4 && InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		repeatPreventerEnterKey = true;
		FRAMEWORK.GetWindow().close();
	}
}

void UiMenu::UpdateOption(float realDt)
{	
	if (optionIndex == 0)
	{
		for (auto alphabetSprite : optionTextVolume)
		{
			alphabetSprite->setColor(sf::Color::Magenta);
		}
	}
	else
	{
		for (auto alphabetSprite : optionTextVolume)
		{
			alphabetSprite->setColor(sf::Color::White);
		}
	}

}

void UiMenu::UpdateOptionKey(float realDt)
{
	if (InputMgr::GetKeyDown(sf::Keyboard::W) || InputMgr::GetKeyDown(sf::Keyboard::Up))
	{
		optionIndex--;
		if (optionIndex == -1)
		{
			optionIndex = 0;
		}
		optionIndex = Utils::Clamp(optionIndex, 0, 0);
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::S) || InputMgr::GetKeyDown(sf::Keyboard::Down))
	{
		optionIndex++;
		if (optionIndex == 1)
		{
			optionIndex = 0;
		}
		optionIndex = Utils::Clamp(optionIndex, 0, 0);
	}

	if (!repeatPreventerEnterKey && optionIndex == 0 && InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		repeatPreventerEnterKey = true;
		isOnOption = false;
		isOnVolume = true;
	}

	if (!repeatPreventerEscKey && InputMgr::GetKeyDown(sf::Keyboard::Escape))
	{
		repeatPreventerEscKey = true;
		isOnMainMenu = true;
		isOnOption = false;
	}
}

void UiMenu::UpdateVolume(float realDt)
{
	if (volumeIndex == 0)
	{
		for (auto alphabetSprite : volumeTextMusic)
		{
			alphabetSprite->setColor(sf::Color::Magenta);
		}
	}
	else
	{
		for (auto alphabetSprite : volumeTextMusic)
		{
			alphabetSprite->setColor(sf::Color::White);
		}
	}

	if (volumeIndex == 1)
	{
		for (auto alphabetSprite : volumeTextSfx)
		{
			alphabetSprite->setColor(sf::Color::Magenta);
		}
	}
	else
	{
		for (auto alphabetSprite : volumeTextSfx)
		{
			alphabetSprite->setColor(sf::Color::White);
		}
	}

}

void UiMenu::UpdateVolumeKey(float realDt)
{
	if (InputMgr::GetKeyDown(sf::Keyboard::W) || InputMgr::GetKeyDown(sf::Keyboard::Up))
	{
		volumeIndex--;
		if (volumeIndex == -1)
		{
			volumeIndex = 1;
		}
		volumeIndex = Utils::Clamp(volumeIndex, 0, 1);
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::S) || InputMgr::GetKeyDown(sf::Keyboard::Down))
	{
		volumeIndex++;
		if (volumeIndex == 2)
		{
			volumeIndex = 0;
		}
		volumeIndex = Utils::Clamp(volumeIndex, 0, 1);
	}

	if (keyInputEnabled)
	{
		if (volumeIndex == 0 && (InputMgr::GetKey(sf::Keyboard::A) || InputMgr::GetKey(sf::Keyboard::Left)))
		{
			int newVol = SOUND_MGR.GetBgmVolume();
			newVol--;
			newVol = Utils::Clamp(newVol, 0, 100);
			SOUND_MGR.SetBgmVolume(newVol);
			OnVolumeChange();
		}

		if (volumeIndex == 0 && (InputMgr::GetKey(sf::Keyboard::D) || InputMgr::GetKey(sf::Keyboard::Right)))
		{
			int newVol = SOUND_MGR.GetBgmVolume();
			newVol++;
			newVol = Utils::Clamp(newVol, 0, 100);
			SOUND_MGR.SetBgmVolume(newVol);
			OnVolumeChange();
		}

		if (volumeIndex == 1 && (InputMgr::GetKey(sf::Keyboard::A) || InputMgr::GetKey(sf::Keyboard::Left)))
		{
			int newVol = SOUND_MGR.GetSfxVolume();
			newVol--;
			newVol = Utils::Clamp(newVol, 0, 100);
			SOUND_MGR.SetSfxVolume(newVol);
			OnVolumeChange(false);
		}

		if (volumeIndex == 1 && (InputMgr::GetKey(sf::Keyboard::D) || InputMgr::GetKey(sf::Keyboard::Right)))
		{
			int newVol = SOUND_MGR.GetSfxVolume();
			newVol++;
			newVol = Utils::Clamp(newVol, 0, 100);
			SOUND_MGR.SetSfxVolume(newVol);
			OnVolumeChange(false);
		}
		keyInputEnabled = false;
	}

	if (!repeatPreventerEscKey && InputMgr::GetKeyDown(sf::Keyboard::Escape))
	{
		repeatPreventerEscKey = true;
		isOnOption = true;
		isOnVolume = false;
	}
}

void UiMenu::UpdateRotationTitle(float realDt)
{
	roataionTimerTitleContent += realDt * 0.6;
	float speedMultiplier = 1.f;
	float rotationMultiplier = 1.f;
	float rotationAmplitude = 1.f;
	sf::Vector2f alphabetOrigin = { 960.f,700.f };

	if (rotationTimerMenuContent > 2.f * Utils::PI)
		rotationTimerMenuContent -= 2.f * Utils::PI;

	mainSpr.setRotation(std::sin(roataionTimerTitleContent * rotationMultiplier) * rotationDurationTitleContent * rotationAmplitude);
	mainSpr2.setRotation(std::sin(roataionTimerTitleContent * rotationMultiplier) * rotationDurationTitleContent * rotationAmplitude);
}

void UiMenu::UpdateRotationMenu(float realDt)
{
	rotationTimerMenuContent += realDt;
	float speedMultiplier = 1.f;
	float rotationMultiplier = 1.f;
	float rotationAmplitude = 1.f;
	sf::Vector2f alphabetOrigin = { 960.f,700.f };

	if (rotationTimerMenuContent > 2.f * Utils::PI)
		rotationTimerMenuContent -= 2.f * Utils::PI;

	float rotationTimer1 = rotationTimerMenuContent * speedMultiplier;

	for (auto alphabetSprite : mainTextStartGame)
	{
		alphabetSprite->setRotation(std::sin(rotationTimerMenuContent * rotationMultiplier) * rotationDurationMenuContent * rotationAmplitude);
		alphabetSprite->setOrigin(alphabetOrigin * 0.2f);
	}
	for (auto alphabetSprite : mainTextContinue)
	{
		alphabetSprite->setRotation(std::sin(rotationTimerMenuContent * rotationMultiplier) * rotationDurationMenuContent * rotationAmplitude);
		alphabetSprite->setOrigin(alphabetOrigin * 0.2f);
	}
	for (auto alphabetSprite : mainTextOption)
	{
		alphabetSprite->setRotation(std::sin(rotationTimerMenuContent * rotationMultiplier) * rotationDurationMenuContent * rotationAmplitude);
		alphabetSprite->setOrigin(alphabetOrigin * 0.2f);
	}
	for (auto alphabetSprite : mainEditorMode)
	{
		alphabetSprite->setRotation(std::sin(rotationTimerMenuContent * rotationMultiplier) * rotationDurationMenuContent * rotationAmplitude);
		alphabetSprite->setOrigin(alphabetOrigin * 0.2f);
	}
	for (auto alphabetSprite : mainTextExit)
	{
		alphabetSprite->setRotation(std::sin(rotationTimerMenuContent * rotationMultiplier) * rotationDurationMenuContent * rotationAmplitude);
		alphabetSprite->setOrigin(alphabetOrigin * 0.2f);
	}

	for (auto alphabetSprite : optionTextVolume)
	{
		alphabetSprite->setRotation(std::sin(rotationTimerMenuContent * rotationMultiplier) * rotationDurationMenuContent * rotationAmplitude);
		alphabetSprite->setOrigin(alphabetOrigin * 0.2f);
	}

	for (auto alphabetSprite : volumeTextMusic2)
	{
		alphabetSprite->setRotation(std::sin(rotationTimerMenuContent * rotationMultiplier) * rotationDurationMenuContent * rotationAmplitude);
		alphabetSprite->setOrigin(alphabetOrigin * 0.2f);
	}
}

void UiMenu::OnVolumeChange(bool isBgm)
{
	std::string path = ALPHABET_TABLE->GetSpirtePath();
	std::string tempText;
	sf::Vector2f textPos;
	sf::Vector2f textDefaultPos = { 0.f,450.f };
	sf::Vector2f alphabetScale = { 4.f,4.f };
	sf::Vector2f alphabetOrigin = { 960.f,700.f };
	int totalAlphabetWidth = 0;
	float verticalTextInterval = 25.f * alphabetScale.y;

	if (isBgm)
	{
		int tempVol = SOUND_MGR.GetBgmVolume();
		tempText = "MUSIC  VOLUME " + std::to_string(tempVol) + "%";
	}
	else
	{
		int tempVol = SOUND_MGR.GetSfxVolume();
		tempText = "SFX  VOLUME " + std::to_string(tempVol) + "%";
	}
	sf::Vector2i alphabetHalfsize = ALPHABET_TABLE->Get(tempText.front()).texCoord.getSize() / 2;
	textPos = { 0.f,0.f };

	for (int i = 0; i < tempText.size(); ++i)
	{
		totalAlphabetWidth += ALPHABET_TABLE->Get(tempText[i]).characterWidth;
	}

	textDefaultPos.x = FRAMEWORK.GetWindowSizeF().x * 0.5f - totalAlphabetWidth * 0.5f * alphabetScale.x;

	if (isBgm)
		volumeTextMusic.clear();
	else
		volumeTextSfx.clear();

	if(!isBgm)
		textDefaultPos.y += verticalTextInterval;

	for (int i = 0; i < tempText.size(); ++i)
	{
		sf::Sprite* alphabetSprite = new sf::Sprite(TEXTURE_MGR.Get(path), ALPHABET_TABLE->Get(tempText[i]).texCoord);
		alphabetSprite->setPosition(textDefaultPos + textPos + alphabetOrigin * 0.8f);
		alphabetSprite->setScale(alphabetScale);
		//alphabetSprite->setRotation(std::sin(rotationTimerMenuContent) * rotationDurationMenuContent);
		alphabetSprite->setOrigin(alphabetOrigin * 0.2f);
		textPos.x += ALPHABET_TABLE->Get(tempText[i]).characterWidth * alphabetScale.x;
		if(isBgm)
			volumeTextMusic.push_back(alphabetSprite);
		else
			volumeTextSfx.push_back(alphabetSprite);
	}
}

void UiMenu::Draw(sf::RenderWindow& window)
{
	if (isOnMainMenu)
	{
		window.draw(mainSpr);
		window.draw(mainSpr2);

		for (auto alphabetSprite : mainTextStartGame)
		{
			window.draw(*alphabetSprite);
		}
		for (auto alphabetSprite : mainTextContinue)
		{
			window.draw(*alphabetSprite);
		}
		for (auto alphabetSprite : mainTextOption)
		{
			window.draw(*alphabetSprite);
		}
		for (auto alphabetSprite : mainEditorMode)
		{
			window.draw(*alphabetSprite);
		}
		for (auto alphabetSprite : mainTextExit)
		{
			window.draw(*alphabetSprite);
		}
	}

	if (isOnOption)
	{
		for (auto alphabetSprite : optionTextVolume)
		{
			window.draw(*alphabetSprite);
		}
	}

	if (isOnVolume)
	{
		for (auto alphabetSprite : volumeTextMusic)
		{
			window.draw(*alphabetSprite);
		}
		for (auto alphabetSprite : volumeTextSfx)
		{
			window.draw(*alphabetSprite);
		}
	}
}

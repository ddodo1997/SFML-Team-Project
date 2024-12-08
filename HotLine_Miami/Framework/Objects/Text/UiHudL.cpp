#include "stdafx.h"
#include "UiHudL.h"
#include "Player.h"

UiHudL::UiHudL(const std::string& name)
	: GameObject(name)
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 0;
}

UiHudL::~UiHudL()
{
	ReleasePauseContent();
}

void UiHudL::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiHudL::SetRotation(float angle)
{
	rotation = angle;
}

void UiHudL::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiHudL::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiHudL::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiHudL::Init()
{
	Reset();
}

void UiHudL::Release()
{
	//ReleasePauseContent();
}

void UiHudL::Reset()
{	
	isClearedStage = false;

	currentWeaponStatus.setFont(FONT_MGR.Get("fonts/DS-DIGI.ttf"));
	currentWeaponStatus.setCharacterSize(30.f);
	currentWeaponStatus.setFillColor(sf::Color::White);
	currentWeaponStatus.setString(std::to_string(InputMgr::GetMousePosition().y));
	currentWeaponStatus.setPosition({ 30.f,30.f });
	Utils::SetOrigin(currentWeaponStatus, Origins::TL);

	volumeDisplayer.setFont(FONT_MGR.Get("fonts/DS-DIGI.ttf"));
	volumeDisplayer.setCharacterSize(30.f);
	volumeDisplayer.setFillColor(sf::Color::White);
	volumeDisplayer.setString(std::to_string(InputMgr::GetMousePosition().y));
	volumeDisplayer.setPosition({ 30.f,280.f });
	Utils::SetOrigin(volumeDisplayer, Origins::TL);

	cursorAnimator.SetTarget(&mouseSprite);
	cursorAnimator.Play("animations/cursor.json");
	mouseSprite.setScale(3.f, 3.f);
	Utils::SetOrigin(mouseSprite, Origins::MC);

	boxBL.setFillColor(sf::Color::Black);
	boxBL.setSize({ 430.f, 102.f });
	boxBLpos = { 0.f,960.f };
	boxBL.setPosition(boxBLpos);
	Utils::SetOrigin(boxBL, Origins::TL);

	textBL.setFont(FONT_MGR.Get("fonts/ByteBounce.ttf"));
	textBL.setCharacterSize(80.f);
	textBL.setFillColor(sf::Color::White);
	textBL.setString("NO GUN!");
	textBL.setPosition(boxBL.getGlobalBounds().width/2.f, boxBL.getGlobalBounds().top + boxBL.getGlobalBounds().height/5.f);
	Utils::SetOrigin(textBL, Origins::MC);

	textBL2.setFont(FONT_MGR.Get("fonts/ByteBounce.ttf"));
	textBL2.setCharacterSize(80.f);
	textBL2.setFillColor(sf::Color::Magenta);
	textBL2.setString("NO GUN!");
	textBL2.setPosition(boxBL.getGlobalBounds().width / 2.f, boxBL.getGlobalBounds().top + boxBL.getGlobalBounds().height / 5.f);
	Utils::SetOrigin(textBL2, Origins::MC);

	pauseShader.setFillColor(sf::Color(0, 0, 0, 100));
	pauseShader.setSize({ 1920.f, 1080.f });
	pauseShader.setPosition(FRAMEWORK.GetWindowSizeF()*0.5f);
	Utils::SetOrigin(pauseShader, Origins::MC);

	pauseTopBox.setFillColor(sf::Color::Black);
	pauseTopBox.setSize({ 1920.f, 180.f });
	pauseTopBox.setPosition({0.f, -180.f});
	Utils::SetOrigin(pauseTopBox, Origins::TL);

	pauseBottomBox.setFillColor(sf::Color::Black);
	pauseBottomBox.setSize({ 1920.f, 180.f });
	pauseBottomBox.setPosition({ 0.f, 1080.f });
	Utils::SetOrigin(pauseBottomBox, Origins::TL);

	//invertShader.loadFromFile("graphics/ETC/invertShader.frag", sf::Shader::Fragment);
	//renderTexture.create(FRAMEWORK.GetWindowSize().x, FRAMEWORK.GetWindowSize().y);
	//fade = 0.f;
	//fadingIn = true;
	ResetPauseContent();
}

void UiHudL::ResetPauseContent()
{
	std::string path = ALPHABET_TABLE->GetSpirtePath(true);
	std::string tempText = "PAUSE"; // 이 5가지 외 스프라이트 없음
	sf::Vector2f textPos = { 0.f,0.f };
	sf::Vector2f textDefaultPos = { 0.f,300.f };
	sf::Vector2f alphabetScale = { 4.f,4.f };
	int totalAlphabetWidth = 0;
	float verticalTextInterval = 25.f * alphabetScale.y;

	for (int i = 0; i < tempText.size(); ++i)
	{
		totalAlphabetWidth += ALPHABET_TABLE->Get(tempText[i],true).characterWidth;
	}

	textDefaultPos.x = FRAMEWORK.GetWindowSizeF().x * 0.5f - totalAlphabetWidth * 0.5f * alphabetScale.x;

	for (int i = 0; i < tempText.size(); ++i)
	{
		sf::Sprite* alphabetSprite = new sf::Sprite(TEXTURE_MGR.Get(path,true), ALPHABET_TABLE->Get(tempText[i], true).texCoord);
		alphabetSprite->setPosition(textDefaultPos + textPos);
		alphabetSprite->setScale(alphabetScale);
		Utils::SetOrigin(*alphabetSprite, Origins::TL);
		textPos.x += ALPHABET_TABLE->Get(tempText[i],true).characterWidth * alphabetScale.x;
		pauseText.push_back(alphabetSprite);
	}

	path = ALPHABET_TABLE->GetSpirtePath();
	tempText = "Resume";
	textPos = { 0.f,0.f };
	textDefaultPos = { 0.f,500.f };
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
		sf::Sprite* alphabetSprite = new sf::Sprite(TEXTURE_MGR.Get(path,true), ALPHABET_TABLE->Get(tempText[i]).texCoord);
		alphabetSprite->setPosition(textDefaultPos + textPos);
		alphabetSprite->setScale(alphabetScale);
		Utils::SetOrigin(*alphabetSprite, Origins::TL);
		textPos.x += ALPHABET_TABLE->Get(tempText[i]).characterWidth * alphabetScale.x;
		pauseTextResume.push_back(alphabetSprite);
	}

	tempText = "Restart";
	totalAlphabetWidth = 0;
	textPos = { 0.f, 0.f };

	for (int i = 0; i < tempText.size(); ++i)
	{
		totalAlphabetWidth += ALPHABET_TABLE->Get(tempText[i]).characterWidth;
	}

	textDefaultPos.x = FRAMEWORK.GetWindowSizeF().x * 0.5f - totalAlphabetWidth * 0.5f * alphabetScale.x;
	textDefaultPos.y += verticalTextInterval;

	for (int i = 0; i < tempText.size(); ++i)
	{
		sf::Sprite* alphabetSprite = new sf::Sprite(TEXTURE_MGR.Get(path,true), ALPHABET_TABLE->Get(tempText[i]).texCoord);
		alphabetSprite->setPosition(textDefaultPos + textPos);
		alphabetSprite->setScale(alphabetScale);
		Utils::SetOrigin(*alphabetSprite, Origins::TL);
		textPos.x += ALPHABET_TABLE->Get(tempText[i]).characterWidth * alphabetScale.x;
		pauseTextRestart.push_back(alphabetSprite);
	}

	tempText = "Quit";
	totalAlphabetWidth = 0;
	textPos = { 0.f, 0.f };

	for (int i = 0; i < tempText.size(); ++i)
	{
		totalAlphabetWidth += ALPHABET_TABLE->Get(tempText[i]).characterWidth;
	}

	textDefaultPos.x = FRAMEWORK.GetWindowSizeF().x * 0.5f - totalAlphabetWidth * 0.5f * alphabetScale.x;
	textDefaultPos.y += verticalTextInterval;

	for (int i = 0; i < tempText.size(); ++i)
	{
		sf::Sprite* alphabetSprite = new sf::Sprite(TEXTURE_MGR.Get(path), ALPHABET_TABLE->Get(tempText[i]).texCoord);
		alphabetSprite->setPosition(textDefaultPos + textPos);
		alphabetSprite->setScale(alphabetScale);
		Utils::SetOrigin(*alphabetSprite, Origins::TL);
		textPos.x += ALPHABET_TABLE->Get(tempText[i]).characterWidth * alphabetScale.x;
		pauseTextQuit.push_back(alphabetSprite);
	}
}

void UiHudL::ReleasePauseContent()
{
	for (auto alphabetSprite : pauseText)
	{
		delete alphabetSprite;
	}
	pauseText.clear();

	for (auto alphabetSprite : pauseTextResume)
	{
		delete alphabetSprite;
	}
	pauseTextResume.clear();

	for (auto alphabetSprite : pauseTextRestart)
	{
		delete alphabetSprite;
	}
	pauseTextRestart.clear();

	for (auto alphabetSprite : pauseTextQuit)
	{
		delete alphabetSprite;
	}
	pauseTextQuit.clear();
}

void UiHudL::Update(float dt)
{
	realDt = FRAMEWORK.GetRealDeltaTime();
	UpdateVolumeDisplay(dt);
	UpdateMouseSprite(dt);
	UpdateBoxBL(realDt);

	if (InputMgr::GetKeyDown(sf::Keyboard::Escape) && !isPaused)
	{
		isPaused = true;
		preventEscRepeat = true;
		FRAMEWORK.SetTimeScale(0.f);
	}
	if (isPaused && !preventEscRepeat)
	{
		UpdatePaused(realDt);
	}

	preventEscRepeat = false;
}

void UiHudL::UpdateBoxBL(float dt)
{
	auto ws = player->GetWeaponStatus();
	if (ws.isRangedWeapon == true)
	{
		boxBLtargetPos = { 0.f,960.f };
		textBL.setString(std::to_string(ws.remainingBullet) + "/" + std::to_string(ws.maxBullet) + "rndS");
		textBL2.setString(std::to_string(ws.remainingBullet) + "/" + std::to_string(ws.maxBullet) + "rndS");
	}
	else
	{
		boxBLtargetPos = { 0.f,1080.f };
		textBL.setString("NO GUNS!");
		textBL2.setString("NO GUNS!");
	}

	float rotationMultiplier = 1.f;
	float rotationAmplitude = 1.f;
	float speedMultiplier = 1.f;

	if (player->IsDead())
	{
		boxBLtargetPos = { 0.f,960.f };
		textBL.setString("R to restart!");
		textBL2.setString("R to restart!");
		rotationMultiplier = 8.f;
		rotationAmplitude = 2.f;
		speedMultiplier = 3.f;
	}
	
	if (boxBLpos.y < boxBLtargetPos.y)
	{
		boxBLpos = boxBLpos + boxBLdir * boxBLspeed * dt;
	}
	if (boxBLpos.y > boxBLtargetPos.y)
	{
		boxBLpos = boxBLpos - boxBLdir * boxBLspeed * dt * speedMultiplier;
	}
	float stopThreshold = std::abs(boxBLdir.y * boxBLspeed * dt);
	if (std::fabs(boxBLpos.y - boxBLtargetPos.y) < stopThreshold)
	{
		boxBLpos = boxBLtargetPos;
	}
	boxBL.setPosition(boxBLpos);

	textBLrotationTimer += dt;
	textBL2rotationTimer += dt;
	float textBLaxisX;
	float textBL2axisX;

	if (textBLrotationTimer > 2.f * Utils::PI)
		textBLrotationTimer -= 2.f * Utils::PI;
	if (textBL2rotationTimer > 2.f * Utils::PI)
		textBL2rotationTimer -= 2.f * Utils::PI;

	float rotationTimer1 = textBLrotationTimer * speedMultiplier;
	float rotationTimer2 = textBL2rotationTimer * speedMultiplier;

	if (rotationTimer1 > 2.f * Utils::PI)
		rotationTimer1 -= 2.f * Utils::PI;
	if (rotationTimer1 > 2.f * Utils::PI)
		rotationTimer1 -= 2.f * Utils::PI;

	if (textBLrotationTimer > 1.5f * Utils::PI)
	{
		textBLaxisX = -0.5f * Utils::PI + (textBLrotationTimer - 1.5f * Utils::PI);
	}
	else if (textBLrotationTimer > 1.0f * Utils::PI)
	{
		textBLaxisX = -(textBLrotationTimer - Utils::PI);
	}
	else if (textBLrotationTimer > 0.5f * Utils::PI)
	{
		textBLaxisX = 0.5f * Utils::PI - (textBLrotationTimer - Utils::PI * 0.5f);
	}
	else
	{
		textBLaxisX = textBLrotationTimer;
	}

	if (textBL2rotationTimer > 1.5f * Utils::PI)
	{
		textBL2axisX = -0.5f * Utils::PI + (textBL2rotationTimer - 1.5f * Utils::PI);
	}
	else if (textBL2rotationTimer > 1.0f * Utils::PI)
	{
		textBL2axisX = -(textBL2rotationTimer - Utils::PI);
	}
	else if (textBL2rotationTimer > 0.5f * Utils::PI)
	{
		textBL2axisX = 0.5f * Utils::PI - (textBL2rotationTimer - Utils::PI * 0.5f);
	}
	else
	{
		textBL2axisX = textBL2rotationTimer;
	}
	
		
	textBL.setRotation(std::sin(textBLrotationTimer * rotationMultiplier) * textBLrotationDuration * rotationAmplitude);
	textBL.setPosition(boxBL.getPosition().x + boxBL.getSize().x / 2.f - 3.f * textBLaxisX, boxBL.getPosition().y + 20.f + 3.f * textBLaxisX);
	Utils::SetOrigin(textBL, Origins::MC);

	textBL2.setRotation(std::sin(textBLrotationTimer * rotationMultiplier) * textBLrotationDuration * rotationAmplitude);
	textBL2.setPosition(boxBL.getPosition().x + boxBL.getSize().x / 2.f - 3.f * textBL2axisX, boxBL.getPosition().y + 20.f - 3.f * textBL2axisX);
	Utils::SetOrigin(textBL2, Origins::MC);
}

void UiHudL::UpdateMouseSprite(float dt)
{
	if(!(cursorAnimator.IsPlaying()))
		cursorAnimator.Play("animations/cursor.json");
	mouseSprite.setPosition(VIEW_MGR.GetMouseSpritePos());
	Utils::SetOrigin(mouseSprite, Origins::MC);
	cursorAnimator.Update(dt);
}

void UiHudL::UpdateWeaponStatus(Weapon::WeaponStatus weaponStatus, int remainingBullet)
{
	weaponTypeIndex = (int)weaponStatus.weaponType;
	std::string weaponId;
	if ((int)weaponStatus.weaponType == -1)
	{
		weaponId = "None";
	}
	if ((int)weaponStatus.weaponType == 0)
	{
		weaponId = "Bat";
	}
	if ((int)weaponStatus.weaponType == 1)
	{
		weaponId = "Knife";
	}
	if ((int)weaponStatus.weaponType == 2)
	{
		weaponId = "Machinegun";
	}
	if ((int)weaponStatus.weaponType == 3)
	{
		weaponId = "Shotgun";
	}
	std::string bulletStatus = "\nNo guns";
	if (weaponStatus.isRangedWeapon)
		bulletStatus = "\nBullets : " + std::to_string(remainingBullet)
		+ " \/ " + std::to_string(weaponStatus.maxBullet);
	currentWeaponStatus.setString("Weapon ID: " + weaponId + bulletStatus);
	Utils::SetOrigin(currentWeaponStatus, Origins::TL);
}

void UiHudL::UpdateVolumeDisplay(float dt)
{
	std::string bgmVol = std::to_string((int)SOUND_MGR.GetBgmVolume());
	std::string sfxVol = std::to_string((int)SOUND_MGR.GetSfxVolume());
	
	volumeDisplayer.setString("Bgm Vol : " + bgmVol + "\nSfx Vol : " + sfxVol);
}

void UiHudL::ResetPaused()
{
	for (auto alphabetSprite : pauseText)
	{
		alphabetSprite->setColor(sf::Color::White);
	}
	for (auto alphabetSprite : pauseTextResume)
	{
		alphabetSprite->setColor(sf::Color::White);
	}
	for (auto alphabetSprite : pauseTextRestart)
	{
		alphabetSprite->setColor(sf::Color::White);
	}
	for (auto alphabetSprite : pauseTextQuit)
	{
		alphabetSprite->setColor(sf::Color::White);
	}
}

void UiHudL::UpdatePaused(float realDt)
{
	if (!wasPaused)
		ResetPaused();

	wasPaused = isPaused;
	//if (fadingIn)
	//{
	//	fade += realDt/6.f;
	//	if (fade >= 1.f)
	//		fadingIn = false;
	//}
	//else
	//{
	//	fade -= realDt / 6.f;
	//	if (fade <= 0.f)
	//		fadingIn = true;
	//}

	colorRotator += realDt;
	if (colorRotator > colorCyclingDuration)
		colorRotator -= colorCyclingDuration;

	float valR = 0.f;
	float valG = 0.f;

	if (colorRotator < colorCyclingDuration / 3.f)
	{
		valR = 1.f - (colorRotator / (colorCyclingDuration / 3.f));
		valG = colorRotator / (colorCyclingDuration / 3.f);
	}
	else if (colorRotator < colorCyclingDuration * 2.f / 3.f)
	{
		valR = 0.f;
		valG = 1.f - (colorRotator - colorCyclingDuration / 3.f) / (colorCyclingDuration / 3.f);
	}
	else
	{
		valR = (colorRotator - colorCyclingDuration * 2.f / 3.f) / (colorCyclingDuration / 3.f);
		valG = 0.f;
	}
	sf::Color shaderColor = { (sf::Uint8)(dscb.x + (dscb.y - dscb.x) * valR), (sf::Uint8)(dscb.x + (dscb.y - dscb.x) * valG), (sf::Uint8)dscb.y, 150 };

	pauseShader.setFillColor(shaderColor);

	if(pauseTimer < 0.4f)
		pauseTimer += realDt;
	pauseTimer = Utils::Clamp(pauseTimer, 0.f, 0.4f);

	pauseTopBox.setPosition(0.f, -180.f + 180 / 0.4 * pauseTimer);
	pauseBottomBox.setPosition(0.f, 1080.f - 180 / 0.4 * pauseTimer);

	UpdatePausedContent(realDt);

	if (pauseTimer >= 0.4f && (InputMgr::GetKeyDown(sf::Keyboard::W) || InputMgr::GetKeyDown(sf::Keyboard::Up)))
	{
		selectedFunctionPause--;
		selectedFunctionPause = Utils::Clamp(selectedFunctionPause, 0, 2);
	}
	if (pauseTimer >= 0.4f && (InputMgr::GetKeyDown(sf::Keyboard::S) || InputMgr::GetKeyDown(sf::Keyboard::Down)))
	{
		selectedFunctionPause++;
		selectedFunctionPause = Utils::Clamp(selectedFunctionPause, 0, 2);
	}

	if (pauseTimer >= 0.4f && selectedFunctionPause == 0 && (InputMgr::GetKeyDown(sf::Keyboard::Enter)))
	{
		Resume();
	}
	if (pauseTimer >= 0.4f && selectedFunctionPause == 1 && (InputMgr::GetKeyDown(sf::Keyboard::Enter)))
	{
		Restart();
	}
	if (pauseTimer >= 0.4f && selectedFunctionPause == 2 && (InputMgr::GetKeyDown(sf::Keyboard::Enter)))
	{
		Quit();
	}

	if (pauseTimer >= 0.4f && InputMgr::GetKeyDown(sf::Keyboard::Escape))
	{
		Resume();
	}

	if (pauseTimer >= 0.4f)
	{
		UpdatePausedContents(realDt);
	}

	if (isResuming)
	{
		resumeTimer += realDt;

		pauseTopBox.setPosition(0.f, -180 / 0.4 * resumeTimer);
		pauseBottomBox.setPosition(0.f, 900.f + 180 / 0.4 * resumeTimer);

		if (resumeTimer > 0.4f)
		{
			isResuming = false;
			isPaused = false;
			pauseTimer = 0.f;
			resumeTimer = 0.f;
		}
	}
	else
	{
		VIEW_MGR.UpdatePausedView(realDt);
	}
}

void UiHudL::UpdatePausedContents(float realDt)
{
	if (selectedFunctionPause == 0)
	{
		for (auto alphabetSprite : pauseTextResume)
		{
			alphabetSprite->setColor(sf::Color::Magenta);
		}
	}
	else
	{
		for (auto alphabetSprite : pauseTextResume)
		{
			alphabetSprite->setColor(sf::Color::White);
		}
	}
	if (selectedFunctionPause == 1)
	{
		for (auto alphabetSprite : pauseTextRestart)
		{
			alphabetSprite->setColor(sf::Color::Magenta);
		}
	}
	else
	{
		for (auto alphabetSprite : pauseTextRestart)
		{
			alphabetSprite->setColor(sf::Color::White);
		}
	}
	if (selectedFunctionPause == 2)
	{
		for (auto alphabetSprite : pauseTextQuit)
		{
			alphabetSprite->setColor(sf::Color::Magenta);
		}
	}
	else
	{
		for (auto alphabetSprite : pauseTextQuit)
		{
			alphabetSprite->setColor(sf::Color::White);
		}
	}
}

void UiHudL::Resume()
{
	//fade = 0.f;
	selectedFunctionPause = 0;
	pauseTimer = 0.f;
	isResuming = true;
	wasPaused = false;
	preventEscRepeat = true;
	FRAMEWORK.SetTimeScale(1.f);
}

void UiHudL::Restart()
{
	selectedFunctionPause = 0;
	SCENE_MGR.ChangeScene(SceneIds::SceneGame);
	isPaused = false;
	wasPaused = false;
	FRAMEWORK.SetTimeScale(1.f);
}

void UiHudL::Quit()
{
	selectedFunctionPause = 0;
	SCENE_MGR.ChangeScene(SceneIds::SceneMenu);
	isPaused = false;
	wasPaused = false;
	FRAMEWORK.SetTimeScale(1.f);
}

void UiHudL::UpdatePausedContent(float realDt)
{
	
}

void UiHudL::Draw(sf::RenderWindow& window)
{
	window.draw(currentWeaponStatus);
	window.draw(volumeDisplayer);
	if (weaponTypeIndex != -1)
	{
		window.draw(currentWeaponStatus);
	}
	window.draw(boxBL);
	window.draw(textBL2);
	window.draw(textBL);
	window.draw(mouseSprite);

	if (isPaused)
	{
		window.draw(pauseShader);
		window.draw(pauseTopBox);
		window.draw(pauseBottomBox);
		for (auto alphabetSprite : pauseText)
		{
			window.draw(*alphabetSprite);
		}
		for (auto alphabetSprite : pauseTextResume)
		{
			window.draw(*alphabetSprite);
		}
		for (auto alphabetSprite : pauseTextRestart)
		{
			window.draw(*alphabetSprite);
		}
		for (auto alphabetSprite : pauseTextQuit)
		{
			window.draw(*alphabetSprite);
		}
	}
}

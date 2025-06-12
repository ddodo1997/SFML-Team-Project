#include "stdafx.h"
#include "UiHud.h"
#include "Player.h"

UiHud::UiHud(const std::string& name)
	: GameObject(name)
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 0;
}

UiHud::~UiHud()
{
	ReleasePauseContent();
}

void UiHud::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiHud::SetRotation(float angle)
{
	rotation = angle;
}

void UiHud::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiHud::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiHud::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiHud::Init()
{
	Reset();
}

void UiHud::Release()
{
	//ReleasePauseContent();
}

void UiHud::Reset()
{	
	isClearedStage = false;

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

void UiHud::ResetPauseContent()
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
	tempText = "STAGE CLEAR";
	textPos = { 0.f,0.f };
	textDefaultPos = { 0.f,500.f };
	alphabetScale = { 3.f,3.f };
	totalAlphabetWidth = 0;
	verticalTextInterval = 25.f * alphabetScale.y;
	sf::Vector2f text2posCalibration = { 4.f,4.f };

	for (int i = 0; i < tempText.size(); ++i)
	{
		totalAlphabetWidth += ALPHABET_TABLE->Get(tempText[i]).characterWidth;
	}

	textDefaultPos.x = FRAMEWORK.GetWindowSizeF().x * 0.5f - totalAlphabetWidth * 0.5f * alphabetScale.x;

	for (int i = 0; i < tempText.size(); ++i)
	{
		sf::Sprite* alphabetSprite = new sf::Sprite(TEXTURE_MGR.Get(path, true), ALPHABET_TABLE->Get(tempText[i]).texCoord);
		alphabetSprite->setPosition(textDefaultPos + textPos);
		alphabetSprite->setScale(alphabetScale);
		Utils::SetOrigin(*alphabetSprite, Origins::TL);
		alphabetSprite->setColor(sf::Color::Yellow);
		stageClearText.push_back(alphabetSprite);
		textPos.x += ALPHABET_TABLE->Get(tempText[i]).characterWidth * alphabetScale.x;
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
		sf::Sprite* alphabetSprite = new sf::Sprite(TEXTURE_MGR.Get(path, true), ALPHABET_TABLE->Get(tempText[i]).texCoord);
		alphabetSprite->setPosition(textDefaultPos + textPos);
		alphabetSprite->setScale(alphabetScale);
		Utils::SetOrigin(*alphabetSprite, Origins::TL);
		pauseTextResume.push_back(alphabetSprite);

		sf::Sprite* alphabetSprite2 = new sf::Sprite(TEXTURE_MGR.Get(path, true), ALPHABET_TABLE->Get(tempText[i]).texCoord);
		alphabetSprite2->setPosition(textDefaultPos + textPos + text2posCalibration);
		alphabetSprite2->setScale(alphabetScale);
		Utils::SetOrigin(*alphabetSprite2, Origins::TL);
		pauseTextResume2.push_back(alphabetSprite2);
		textPos.x += ALPHABET_TABLE->Get(tempText[i]).characterWidth * alphabetScale.x;
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
		pauseTextRestart.push_back(alphabetSprite);

		sf::Sprite* alphabetSprite2 = new sf::Sprite(TEXTURE_MGR.Get(path, true), ALPHABET_TABLE->Get(tempText[i]).texCoord);
		alphabetSprite2->setPosition(textDefaultPos + textPos + text2posCalibration);
		alphabetSprite2->setScale(alphabetScale);
		Utils::SetOrigin(*alphabetSprite2, Origins::TL);
		pauseTextRestart2.push_back(alphabetSprite2);
		textPos.x += ALPHABET_TABLE->Get(tempText[i]).characterWidth * alphabetScale.x;
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
		pauseTextQuit.push_back(alphabetSprite);

		sf::Sprite* alphabetSprite2 = new sf::Sprite(TEXTURE_MGR.Get(path, true), ALPHABET_TABLE->Get(tempText[i]).texCoord);
		alphabetSprite2->setPosition(textDefaultPos + textPos + text2posCalibration);
		alphabetSprite2->setScale(alphabetScale);
		Utils::SetOrigin(*alphabetSprite2, Origins::TL);
		pauseTextQuit2.push_back(alphabetSprite2);
		textPos.x += ALPHABET_TABLE->Get(tempText[i]).characterWidth * alphabetScale.x;
	}
}

void UiHud::ReleasePauseContent()
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
	for (auto alphabetSprite : pauseTextResume2)
	{
		delete alphabetSprite;
	}
	pauseTextResume2.clear();

	for (auto alphabetSprite : pauseTextRestart)
	{
		delete alphabetSprite;
	}
	pauseTextRestart.clear();
	for (auto alphabetSprite : pauseTextRestart2)
	{
		delete alphabetSprite;
	}
	pauseTextRestart2.clear();

	for (auto alphabetSprite : pauseTextQuit)
	{
		delete alphabetSprite;
	}
	pauseTextQuit.clear();
	for (auto alphabetSprite : pauseTextQuit2)
	{
		delete alphabetSprite;
	}
	pauseTextQuit2.clear();
}

void UiHud::Update(float dt)
{
	realDt = FRAMEWORK.GetRealDeltaTime();
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

void UiHud::UpdateBoxBL(float dt)
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

void UiHud::UpdateMouseSprite(float dt)
{
	if(!(cursorAnimator.IsPlaying()))
		cursorAnimator.Play("animations/cursor.json");
	mouseSprite.setPosition(VIEW_MGR.GetMouseSpritePos());
	Utils::SetOrigin(mouseSprite, Origins::MC);
	cursorAnimator.Update(dt);
}

void UiHud::ResetPaused()
{
	for (auto alphabetSprite : pauseText)
	{
		alphabetSprite->setColor(sf::Color::White);
	}
	for (auto alphabetSprite : pauseTextResume)
	{
		alphabetSprite->setColor(sf::Color::White);
	}
	for (auto alphabetSprite : pauseTextResume2)
	{
		alphabetSprite->setColor(sf::Color::White);
	}
	for (auto alphabetSprite : pauseTextRestart)
	{
		alphabetSprite->setColor(sf::Color::White);
	}
	for (auto alphabetSprite : pauseTextRestart2)
	{
		alphabetSprite->setColor(sf::Color::White);
	}
	for (auto alphabetSprite : pauseTextQuit)
	{
		alphabetSprite->setColor(sf::Color::White);
	}
	for (auto alphabetSprite : pauseTextQuit2)
	{
		alphabetSprite->setColor(sf::Color::White);
	}
}

void UiHud::UpdatePaused(float realDt)
{
	//if (!wasPaused)
	//	ResetPaused();

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


	colorRotatorTimerShader += realDt;
	if (colorRotatorTimerShader > colorCyclingDurationShader)
		colorRotatorTimerShader -= colorCyclingDurationShader;

	colorRotatorTimerPausedContent += realDt;
	if (colorRotatorTimerPausedContent > colorCyclingDurationPausedContent)
		colorRotatorTimerPausedContent -= colorCyclingDurationPausedContent;

	float valR = 0.f;
	float valG = 0.f;

	if (colorRotatorTimerShader < colorCyclingDurationShader / 3.f)
	{
		valR = 1.f - (colorRotatorTimerShader / (colorCyclingDurationShader / 3.f));
		valG = colorRotatorTimerShader / (colorCyclingDurationShader / 3.f);
	}
	else if (colorRotatorTimerShader < colorCyclingDurationShader * 2.f / 3.f)
	{
		valR = 0.f;
		valG = 1.f - (colorRotatorTimerShader - colorCyclingDurationShader / 3.f) / (colorCyclingDurationShader / 3.f);
	}
	else
	{
		valR = (colorRotatorTimerShader - colorCyclingDurationShader * 2.f / 3.f) / (colorCyclingDurationShader / 3.f);
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

	//if (pauseTimer < 0.4f)
	//{
	//	UpdateSlideInEffect(realDt);
	//}

	if (pauseTimer >= 0.4f && (InputMgr::GetKeyDown(sf::Keyboard::W) || InputMgr::GetKeyDown(sf::Keyboard::Up)))
	{
		selectedFunctionPause--;
		if (selectedFunctionPause == -1)
			selectedFunctionPause = 2;
		selectedFunctionPause = Utils::Clamp(selectedFunctionPause, 0, 2);
	}
	if (pauseTimer >= 0.4f && (InputMgr::GetKeyDown(sf::Keyboard::S) || InputMgr::GetKeyDown(sf::Keyboard::Down)))
	{
		selectedFunctionPause++;
		if (selectedFunctionPause == 3)
			selectedFunctionPause = 0;
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

void UiHud::UpdatePausedContents(float realDt)
{
	sf::Uint8 valA;
	sf::Uint8 valB;

	if (colorRotatorTimerPausedContent > colorCyclingDurationPausedContent * 0.5f)
	{
		valA = 255 * (colorRotatorTimerPausedContent / (colorCyclingDurationPausedContent * 0.5f));
		valB = 255 - valA;
	}
	else
	{
		valB = 255 * (colorRotatorTimerPausedContent / (colorCyclingDurationPausedContent * 0.5f));
		valA = 255 - valB;
	}

	sf::Color cMtoW = { 255,valB, 255, 255 };
	sf::Color cWtoM = { 255, valA, 255, 255 };

	sf::Color cCtoW = { valB, 255, 255, 255	};
	sf::Color cWtoC = { valA, 255, 255, 255	};

	if (selectedFunctionPause == 0)
	{
		for (auto alphabetSprite : pauseTextResume)
		{
			alphabetSprite->setColor(cMtoW);
		}
		for (auto alphabetSprite : pauseTextResume2)
		{
			alphabetSprite->setColor(cWtoM);
		}
	}
	else
	{
		for (auto alphabetSprite : pauseTextResume)
		{
			alphabetSprite->setColor(cCtoW);
		}
		for (auto alphabetSprite : pauseTextResume2)
		{
			alphabetSprite->setColor(cWtoC);
		}
	}

	if (selectedFunctionPause == 1)
	{
		for (auto alphabetSprite : pauseTextRestart)
		{
			alphabetSprite->setColor(cMtoW);
		}
		for (auto alphabetSprite : pauseTextRestart2)
		{
			alphabetSprite->setColor(cWtoM);
		}
	}
	else
	{
		for (auto alphabetSprite : pauseTextRestart)
		{
			alphabetSprite->setColor(cCtoW);
		}
		for (auto alphabetSprite : pauseTextRestart2)
		{
			alphabetSprite->setColor(cWtoC);
		}
	}

	if (selectedFunctionPause == 2)
	{
		for (auto alphabetSprite : pauseTextQuit)
		{
			alphabetSprite->setColor(cMtoW);
		}
		for (auto alphabetSprite : pauseTextQuit2)
		{
			alphabetSprite->setColor(cWtoM);
		}
	}
	else
	{
		for (auto alphabetSprite : pauseTextQuit)
		{
			alphabetSprite->setColor(cCtoW);
		}
		for (auto alphabetSprite : pauseTextQuit2)
		{
			alphabetSprite->setColor(cWtoC);
		}
	}
}

void UiHud::UpdateSlideInEffect(float realDt)
{
	sf::Vector2f slideInFromR = { 1200.f / 0.4f , 0.f };
	sf::Vector2f slideInFromL = { -1200.f / 0.4f , 0.f };

	std::string path = ALPHABET_TABLE->GetSpirtePath(true);
	std::string tempText = "PAUSE"; // 이 5가지 외 스프라이트 없음
	sf::Vector2f textPos = { 0.f,0.f };
	sf::Vector2f textDefaultPos = { 0.f,300.f };
	sf::Vector2f alphabetScale = { 4.f,4.f };
	sf::Vector2f text2posCalibration = { 4.f,4.f };
	int totalAlphabetWidth = 0;
	float verticalTextInterval = 25.f * alphabetScale.y;

	if (pauseTimer < 0.4f)
	{
		for (int i = 0; i < tempText.size(); ++i)
		{
			totalAlphabetWidth += ALPHABET_TABLE->Get(tempText[i], true).characterWidth;
		}

		textDefaultPos.x = FRAMEWORK.GetWindowSizeF().x * 0.5f - totalAlphabetWidth * 0.5f * alphabetScale.x;

		for (int i = 0; i < tempText.size(); ++i)
		{
			pauseText[i]->setPosition(textDefaultPos + textPos + (slideInFromR * (0.4f - pauseTimer)));
			Utils::SetOrigin(*pauseText[i], Origins::TL);
			textPos.x += ALPHABET_TABLE->Get(tempText[i], true).characterWidth * alphabetScale.x;
		}

		tempText = "Resume";
		textPos = { 0.f,0.f };
		textDefaultPos = { 0.f,500.f };
		totalAlphabetWidth = 0;
		verticalTextInterval = 25.f * alphabetScale.y;

		for (int i = 0; i < tempText.size(); ++i)
		{
			totalAlphabetWidth += ALPHABET_TABLE->Get(tempText[i]).characterWidth;
		}

		textDefaultPos.x = FRAMEWORK.GetWindowSizeF().x * 0.5f - totalAlphabetWidth * 0.5f * alphabetScale.x;

		for (int i = 0; i < tempText.size(); ++i)
		{
			pauseTextResume[i]->setPosition(textDefaultPos + textPos + (slideInFromR * (0.4f - pauseTimer)));
			Utils::SetOrigin(*pauseTextResume[i], Origins::TL);
			textPos.x += ALPHABET_TABLE->Get(tempText[i]).characterWidth * alphabetScale.x;
		}

		textPos = { 0.f,0.f };

		for (int i = 0; i < tempText.size(); ++i)
		{
			pauseTextResume2[i]->setPosition(textDefaultPos + textPos + text2posCalibration + (slideInFromR * (0.4f - pauseTimer)));
			Utils::SetOrigin(*pauseTextResume2[i], Origins::TL);
			textPos.x += ALPHABET_TABLE->Get(tempText[i]).characterWidth * alphabetScale.x;
			++i;
		}

		tempText = "Restart";
		textPos = { 0.f,0.f };
		textDefaultPos = { 0.f,500.f };
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
			pauseTextRestart[i]->setPosition(textDefaultPos + textPos + (slideInFromR * (0.4f - pauseTimer)));
			Utils::SetOrigin(*pauseTextRestart[i], Origins::TL);
			textPos.x += ALPHABET_TABLE->Get(tempText[i]).characterWidth * alphabetScale.x;
		}

		textPos = { 0.f,0.f };

		for (int i = 0; i < tempText.size(); ++i)
		{
			pauseTextRestart2[i]->setPosition(textDefaultPos + textPos + text2posCalibration + (slideInFromR * (0.4f - pauseTimer)));
			Utils::SetOrigin(*pauseTextRestart2[i], Origins::TL);
			textPos.x += ALPHABET_TABLE->Get(tempText[i]).characterWidth * alphabetScale.x;
		}

		tempText = "Quit";
		textPos = { 0.f,0.f };
		textDefaultPos = { 0.f,500.f };
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
			pauseTextQuit[i]->setPosition(textDefaultPos + textPos + (slideInFromR * (0.4f - pauseTimer)));
			Utils::SetOrigin(*pauseTextQuit[i], Origins::TL);
			textPos.x += ALPHABET_TABLE->Get(tempText[i]).characterWidth * alphabetScale.x;
		}

		textPos = { 0.f,0.f };

		for (int i = 0; i < tempText.size(); ++i)
		{
			pauseTextQuit2[i]->setPosition(textDefaultPos + textPos + text2posCalibration + (slideInFromR * (0.4f - pauseTimer)));
			Utils::SetOrigin(*pauseTextQuit2[i], Origins::TL);
			textPos.x += ALPHABET_TABLE->Get(tempText[i]).characterWidth * alphabetScale.x;
		}
	}
}

void UiHud::Resume()
{
	//fade = 0.f;
	selectedFunctionPause = 0;
	pauseTimer = 0.f;
	isResuming = true;
	wasPaused = false;
	preventEscRepeat = true;
	FRAMEWORK.SetTimeScale(1.f);
}

void UiHud::Restart()
{
	selectedFunctionPause = 0;
	SCENE_MGR.ChangeScene(SceneIds::SceneGame);
	isPaused = false;
	wasPaused = false;
	FRAMEWORK.SetTimeScale(1.f);
}

void UiHud::Quit()
{
	selectedFunctionPause = 0;
	SCENE_MGR.ChangeScene(SceneIds::SceneMenu);
	isPaused = false;
	wasPaused = false;
	FRAMEWORK.SetTimeScale(1.f);
}

void UiHud::UpdatePausedContent(float realDt)
{
	
}

void UiHud::Draw(sf::RenderWindow& window)
{
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
		for (auto alphabetSprite : pauseTextResume2)
		{
			window.draw(*alphabetSprite);
		}
		for (auto alphabetSprite : pauseTextResume)
		{
			window.draw(*alphabetSprite);
		}
		for (auto alphabetSprite : pauseTextRestart2)
		{
			window.draw(*alphabetSprite);
		}
		for (auto alphabetSprite : pauseTextRestart)
		{
			window.draw(*alphabetSprite);
		}
		for (auto alphabetSprite : pauseTextQuit2)
		{
			window.draw(*alphabetSprite);
		}
		for (auto alphabetSprite : pauseTextQuit)
		{
			window.draw(*alphabetSprite);
		}
	}
	if (!isPaused && isClearedStage)
	{
		for (auto alphabetSprite : stageClearText)
		{
			window.draw(*alphabetSprite);
		}
	}
}

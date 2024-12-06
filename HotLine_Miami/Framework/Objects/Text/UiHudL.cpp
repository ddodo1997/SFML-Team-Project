#include "stdafx.h"
#include "UiHudL.h"
#include "Player.h"

UiHudL::UiHudL(const std::string& name)
	: GameObject(name)
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 0;
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
}

void UiHudL::Reset()
{	
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
	
	float speedMultiplier = 2.f;
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
		
	textBL.setRotation(std::sin(textBLrotationTimer) * textBLrotationDuration);
	textBL.setPosition(boxBL.getPosition().x + boxBL.getSize().x / 2.f - 3.f * textBLaxisX, boxBL.getPosition().y + 20.f + 3.f * textBLaxisX);
	Utils::SetOrigin(textBL, Origins::MC);

	textBL2.setRotation(std::sin(textBLrotationTimer) * textBLrotationDuration);
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

void UiHudL::UpdatePaused(float realDt)
{
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
	sf::Color shaderColor = { (sf::Uint8)(dscb.x + (dscb.y - dscb.x) * valR), (sf::Uint8)(dscb.x + (dscb.y - dscb.x) * valG), (sf::Uint8)dscb.y, 120 };

	pauseShader.setFillColor(shaderColor);

	if(pauseTimer < 0.4f)
		pauseTimer += realDt;
	pauseTimer = Utils::Clamp(pauseTimer, 0.f, 0.4f);

	pauseTopBox.setPosition(0.f, -180.f + 180 / 0.4 * pauseTimer);
	pauseBottomBox.setPosition(0.f, 1080.f - 180 / 0.4 * pauseTimer);



	if (pauseTimer >= 0.4f && InputMgr::GetKeyDown(sf::Keyboard::Escape))
	{
		//fade = 0.f;
		pauseTimer = 0.f;
		isResuming = true;
		preventEscRepeat = true;
		FRAMEWORK.SetTimeScale(1.f);
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
	}
}

#include "stdafx.h"
#include "ViewMgr.h"
#include "Player.h"
#include <Windows.h>

void ViewMgr::Init()
{
	Reset();
}

void ViewMgr::Reset()
{
	auto wSize = FRAMEWORK.GetWindowSizeF();
	worldView.setSize(wSize);
	uiView.setSize(wSize);

	worldView.setCenter(0.f, 0.f);
	uiView.setCenter(wSize / 2.f);
	playerPos = { 0.f,0.f };

	mPosBound.left = boundEdgeCut;
	mPosBound.top = boundEdgeCut;
	mPosBound.width = 1920 - boundEdgeCut;
	mPosBound.height = 1080 - boundEdgeCut;

	vaBackground.setPrimitiveType(sf::PrimitiveType::Quads);
	vaBackground.resize(4);

	vaBackground[0].position = { 0.f,0.f };
	vaBackground[1].position = { 1920.f,0.f };
	vaBackground[2].position = { 1920.f,1080.f };
	vaBackground[3].position = { 0.f,1080.f };

	flashEffectTimer = 200.f;

	FRAMEWORK.GetWindow().setMouseCursorVisible(isCursorVisible);
}

void ViewMgr::Update(float dt)
{
	if (player != nullptr)
	{
		look = player->GetLook();
		playerPos = player->GetPosition();
	}

	if (SCENE_MGR.GetCurrentSceneId() == SceneIds::SceneGame)
	{
		UpdateBackground(FRAMEWORK.GetRealDeltaTime());
		UpdateViewRotation(dt);
	}
	if (SCENE_MGR.GetCurrentSceneId() == SceneIds::SceneMenu)
	{
		UpdateMenuBackground(FRAMEWORK.GetRealDeltaTime());
	}
	if (SCENE_MGR.GetCurrentSceneId() == SceneIds::SceneEditor)
	{
		UpdateMenuBackground(FRAMEWORK.GetRealDeltaTime());
	}

	if (InputMgr::GetKey(sf::Keyboard::LShift))
	{
		UpdateFurtherView(dt);
	}
	else
	{
		UpdateDefaultView(dt);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Grave))
	{
		isCursorVisible = !isCursorVisible;
		FRAMEWORK.GetWindow().setMouseCursorVisible(isCursorVisible);

		if (isCursorVisible)
		{
			UnlockCursor();
		}
		else
		{
			LockCursor(FRAMEWORK.GetWindow());
		}
	}
}

void ViewMgr::UpdateViewRotation(float dt)
{
	sf::Vector2f distanceFromDefaultPos;
	distanceFromDefaultPos = defaultPlayerPos - playerPos;

	float rotationCycle = 600.f;

	if (distanceFromDefaultPos.x > rotationCycle)
	{
		distanceFromDefaultPos.x -= rotationCycle;
	}
	if (distanceFromDefaultPos.x < -rotationCycle)
	{
		distanceFromDefaultPos.x += rotationCycle;
	}
	if (distanceFromDefaultPos.y > rotationCycle)
	{
		distanceFromDefaultPos.y -= rotationCycle;
	}
	if (distanceFromDefaultPos.y < -rotationCycle)
	{
		distanceFromDefaultPos.y += rotationCycle;
	}
	
	float rotationX = 0.f;
	float rotationY = 0.f;

	if (distanceFromDefaultPos.x >= rotationCycle*0.5f)
	{
		rotationX = rotationCycle - distanceFromDefaultPos.x; // 100 ~ 0
	}
	else if (distanceFromDefaultPos.x <= -rotationCycle * 0.5f) // -100 ~ 0
	{
		rotationX = -rotationCycle - distanceFromDefaultPos.x;
	}
	else
	{
		rotationX = distanceFromDefaultPos.x; // -100 ~ 100
	}

	if (distanceFromDefaultPos.y >= rotationCycle * 0.5f)
	{
		rotationY = rotationCycle - distanceFromDefaultPos.y; // 100 ~ 0
	}
	else if (distanceFromDefaultPos.y <= -rotationCycle * 0.5f) // -100 ~ 0
	{
		rotationY = -rotationCycle - distanceFromDefaultPos.y;
	}
	else
	{
		rotationY = distanceFromDefaultPos.y; // -100 ~ 100
	}

	viewRotation = (rotationX + rotationY)*0.006f;	
	
	worldViewCurrentScene->setRotation(viewRotation);
}

void ViewMgr::UpdateFurtherView(float dt)
{
	UpdateFurtherViewMousePos(dt);
}

void ViewMgr::UpdateDefaultView(float dt)
{
	mouseSpritePos = InputMgr::GetMousePosition();
	direction = look;
	if(Utils::SqrMagnitude(look) > 1.f)
		direction = Utils::GetNormal(look);

	worldViewTargetPos = ((sf::Vector2f(playerPos.x + direction.x * defaultViewDistance, 
		playerPos.y + direction.y * defaultViewDistance)));

	worldViewDirection = Utils::GetNormal(worldViewTargetPos - worldViewCenterPos);

	distanceToTargetView = Utils::Magnitude(worldViewTargetPos - worldViewCenterPos);

	worldViewCenterPos = worldViewCenterPos + worldViewDirection * distanceToTargetView * viewMoveSpeed * dt;
	if(Utils::SqrMagnitude(worldViewTargetPos-worldViewCenterPos) > 1.f)
		worldViewCurrentScene->setCenter(worldViewCenterPos);	
}

void ViewMgr::UpdatePausedView(float realDt)
{
	pausedViewTimer += realDt * 2.f;

	if (pausedViewTimer > 2 * Utils::PI)
		pausedViewTimer -= 2 * Utils::PI;

	//worldViewCenterPos += Utils::RadianToNormal(std::sin(pausedViewTimer));
	worldViewCurrentScene->setCenter(worldViewCenterPos + Utils::RadianToNormal(std::sin(pausedViewTimer)));
}

void ViewMgr::UpdateFurtherViewMousePos(float dt)
{
	mouseSpritePos = InputMgr::GetMousePosition();

	float viewDistanceMultiplier = 1.f;
	if (player->GetCurrentMask() == Player::Mask::Giraffe)
		viewDistanceMultiplier = 2.f;

	mouseSpritePos.x = Utils::Clamp(mouseSpritePos.x, mPosBound.left, mPosBound.width);
	mouseSpritePos.y = Utils::Clamp(mouseSpritePos.y, mPosBound.top, mPosBound.height);

	sf::Vector2f halfWindowSize = { 960.f, 540.f };
	sf::Vector2f viewScale = { 0.2f,0.2f };

	worldViewTargetPos.x = playerPos.x + (((float)mouseSpritePos.x - halfWindowSize.x) / (halfWindowSize.x - boundEdgeCut)) * halfWindowSize.x * viewScale.x * viewDistanceMultiplier;
	worldViewTargetPos.y = playerPos.y + (((float)mouseSpritePos.y - halfWindowSize.y) / (halfWindowSize.y - boundEdgeCut)) * halfWindowSize.y * viewScale.y * viewDistanceMultiplier;

	direction = look;
	if (Utils::SqrMagnitude(look) > 1.f)
		direction = Utils::GetNormal(look);

	worldViewDirection = Utils::GetNormal(worldViewTargetPos - worldViewCenterPos);

	distanceToTargetView = Utils::Magnitude(worldViewTargetPos - worldViewCenterPos);

	worldViewCenterPos = worldViewCenterPos + worldViewDirection * distanceToTargetView * viewMoveSpeed * dt * (1.f/ viewDistanceMultiplier);
	if (Utils::SqrMagnitude(worldViewTargetPos - worldViewCenterPos) > 1.f)
		worldViewCurrentScene->setCenter(worldViewCenterPos);
}

void ViewMgr::UpdateBackground(float realDt)
{
	flashEffectTimer += realDt * 800.f;
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
		valG = 1.f - (colorRotator - colorCyclingDuration/3.f) / (colorCyclingDuration / 3.f);
	}
	else
	{
		valR = (colorRotator - colorCyclingDuration * 2.f / 3.f) / (colorCyclingDuration / 3.f);
		valG = 0.f;
	}	

	sf::Color brightSideColor = { (sf::Uint8)(bscb.x + (bscb.y - bscb.x) * valR), (sf::Uint8)(bscb.x + (bscb.y - bscb.x) * valG), (sf::Uint8)bscb.y };
	sf::Color darkSideColor = { (sf::Uint8)(dscb.x + (dscb.y - dscb.x) * valR), (sf::Uint8)(dscb.x + (dscb.y - dscb.x) * valG), (sf::Uint8)dscb.y };

	if (flashEffectTimer < 200.f)
	{
		float temp = flashEffectTimer / 30;
		if ((int)temp % 2 == 0)
		{
			brightSideColor = sf::Color(193, 65, 53);
			darkSideColor = sf::Color(193, 65, 53);
		}
	}

	vaBackground[0].color = brightSideColor;
	vaBackground[1].color = brightSideColor;
	vaBackground[2].color = darkSideColor;
	vaBackground[3].color = darkSideColor;		
}

void ViewMgr::UpdateMenuBackground(float realDt)
{
	UpdateBackground(realDt);
}

void ViewMgr::DrawBackground()
{
	auto tempView = FRAMEWORK.GetWindow().getView();
	FRAMEWORK.GetWindow().setView(*uiViewCurrentScene);
	FRAMEWORK.GetWindow().draw(vaBackground);
	FRAMEWORK.GetWindow().setView(tempView);
}

sf::Vector2f ViewMgr::ScreenToWorld(sf::Vector2i screenPos)
{
	return FRAMEWORK.GetWindow().mapPixelToCoords(screenPos, worldView);
}


sf::Vector2i ViewMgr::WorldToScreen(sf::Vector2f worldPos)
{
	return FRAMEWORK.GetWindow().mapCoordsToPixel(worldPos, worldView);
}

sf::Vector2f ViewMgr::ScreenToUi(sf::Vector2i screenPos)
{
	return FRAMEWORK.GetWindow().mapPixelToCoords(screenPos, uiView);
}

sf::Vector2i ViewMgr::UiToScreen(sf::Vector2f worldPos)
{
	return FRAMEWORK.GetWindow().mapCoordsToPixel(worldPos, uiView);
}

void ViewMgr::SetWorldViewSize(sf::Vector2f size)
{
	worldView.setSize(size);
}

sf::Vector2f ViewMgr::GetWorldViewSize()
{
	return worldView.getSize();
}

void ViewMgr::SetWorldViewScale(sf::Vector2f scale)
{
	worldViewScale = scale;
	worldView.setSize({ defaultFHDSize.x * worldViewScale.x, defaultFHDSize.y * worldViewScale.y });
}

void ViewMgr::SetUiViewSize(sf::Vector2f size)
{
	uiView.setSize(size);
}

sf::Vector2f ViewMgr::GetUiViewSize()
{
	return uiView.getSize();
}

void ViewMgr::LockCursor(sf::RenderWindow& window)
{
	RECT rect;
	HWND hwnd = window.getSystemHandle(); // SFML â�� �ڵ��� �����ɴϴ�.

	GetClientRect(hwnd, &rect);           // â ������ Ŭ���̾�Ʈ ������ �����ɴϴ�.
	MapWindowPoints(hwnd, nullptr, reinterpret_cast<POINT*>(&rect), 2); // ȭ�� ��ǥ�� ��ȯ
	ClipCursor(&rect);                    // Ŀ���� �ش� ������ �����մϴ�.
}

void ViewMgr::UnlockCursor()
{
	ClipCursor(nullptr);                  // ������ �����մϴ�.
}

void ViewMgr::ResetFlashEffectTimer()
{
	flashEffectTimer = 0.f;
}

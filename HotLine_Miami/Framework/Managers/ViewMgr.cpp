#include "stdafx.h"
#include "ViewMgr.h"
#include "Player.h"

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

	mPosBound.left = boundEdgeCut;
	mPosBound.top = boundEdgeCut;
	mPosBound.width = 1920 - boundEdgeCut;
	mPosBound.height = 1080 - boundEdgeCut;
}

void ViewMgr::Update(float dt)
{
	look = player->GetLook();
	playerPos = player->GetPosition();

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
	}

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


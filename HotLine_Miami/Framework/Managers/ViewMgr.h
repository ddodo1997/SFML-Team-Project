#pragma once

class Player;

class ViewMgr : public Singleton<ViewMgr>
{
	friend Singleton<ViewMgr>;
private:
	ViewMgr() = default;
	~ViewMgr() = default;
	ViewMgr(const ViewMgr& other) = delete;
	ViewMgr& operator=(const ViewMgr& other) = delete;

protected:
	sf::View worldView;
	sf::View uiView;

	sf::View* worldViewCurrentScene;
	sf::View* uiViewCurrentScene;

	Player* player;
	sf::Vector2f playerPos;
	sf::Vector2f look;
	sf::Vector2f worldViewCenterPos = { 0, 0 };
	sf::Vector2f worldViewTargetPos = { 0, 0 };
	
	sf::IntRect mPosBound;
	int boundEdgeCut = 200;

	sf::Vector2i mouseSpritePos;
	sf::Vector2f direction;

	sf::Vector2f worldViewDirection;

	const float defaultViewDistance = 5.f;

	float distanceToTargetView = 0;

	float viewMoveSpeed = 10.f;

	bool isCursorVisible = true;

	const sf::Vector2f defaultFHDSize = { 1920.f, 1080.f };
	sf::Vector2f worldViewScale;

	sf::Vector2f defaultPlayerPos = { 50.f, 150.f };
	float viewRotation;

public:
	void Init();
	void Reset();
	void Update(float dt);

	void UpdateViewRotation(float dt);

	void UpdateFurtherView(float dt);
	void UpdateDefaultView(float dt);

	void UpdateFurtherViewMousePos(float dt);

	const sf::Vector2f& GetMouseSpritePos() { return (sf::Vector2f)mouseSpritePos; }

	sf::Vector2f ScreenToWorld(sf::Vector2i screenPos);
	sf::Vector2i WorldToScreen(sf::Vector2f worldPos);
	sf::Vector2f ScreenToUi(sf::Vector2i screenPos);
	sf::Vector2i UiToScreen(sf::Vector2f worldPos);

	void SetWorldViewSize(sf::Vector2f size);
	sf::Vector2f GetWorldViewSize();

	void SetCurrentSceneWorldView(sf::View* view) { worldViewCurrentScene = view; }
	void SetCurrentSceneUiView(sf::View* view) { uiViewCurrentScene = view; }
	void SetCurrentScenePlayer(Player* player) { this->player = player; }

	void SetWorldViewScale(sf::Vector2f scale);

	void SetUiViewSize(sf::Vector2f size);
	sf::Vector2f GetUiViewSize();
};

#define VIEW_MGR (ViewMgr::Instance())
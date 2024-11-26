#pragma once

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

public:
	void Init();
	void Reset();
	void Update(float dt);

	sf::Vector2f ScreenToWorld(sf::Vector2i screenPos);
	sf::Vector2i WorldToScreen(sf::Vector2f worldPos);
	sf::Vector2f ScreenToUi(sf::Vector2i screenPos);
	sf::Vector2i UiToScreen(sf::Vector2f worldPos);
};

#define VIEW_MGR (ViewMgr::Instance())
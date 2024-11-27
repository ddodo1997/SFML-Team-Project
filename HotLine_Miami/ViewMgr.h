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

	const sf::Vector2f defaultFHDSize = { 1920.f, 1080.f };
	sf::Vector2f worldViewScale;

public:
	void Init();
	void Reset();
	void Update(float dt);

	sf::Vector2f ScreenToWorld(sf::Vector2i screenPos);
	sf::Vector2i WorldToScreen(sf::Vector2f worldPos);
	sf::Vector2f ScreenToUi(sf::Vector2i screenPos);
	sf::Vector2i UiToScreen(sf::Vector2f worldPos);

	void SetWorldViewSize(sf::Vector2f size);
	sf::Vector2f GetWorldViewSize();

	void SetWorldViewScale(sf::Vector2f scale);

	void SetUiViewSize(sf::Vector2f size);
	sf::Vector2f GetUiViewSize();
};

#define VIEW_MGR (ViewMgr::Instance())
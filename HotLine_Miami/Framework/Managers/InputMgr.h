#pragma once
#include <list>

enum class Axis
{
	Horizontal,
	Vertical,
};

struct AxisInfo
{
	Axis axis;
	std::list<int> positives;		// 1.0f
	std::list<int> negatives;		// -1.0f
	float sensi = 10.f;
	float value = 0.f;

	void AddKey(bool positive, int code) 
	{ 
		if (positive) 
			positives.push_back(code);
		else 
			negatives.push_back(code); 
	}
};


class InputMgr
{
private:
	static std::list<int> downKeys;
	static std::list<int> heldKeys;
	static std::list<int> upKeys;

	static sf::Vector2i mousePosition;

	static std::unordered_map<Axis, AxisInfo> axisInfoMap;

public:
	static void Init();
	static void Clear();

	static void UpdateEvent(const sf::Event& ev);
	static void Update(float dt);

	static bool GetKeyDown(sf::Keyboard::Key key);
	static bool GetKey(sf::Keyboard::Key key);
	static bool GetKeyUp(sf::Keyboard::Key key);

	static bool GetMouseButtonDown(sf::Mouse::Button button);
	static bool GetMouseButton(sf::Mouse::Button button);
	static bool GetMouseButtonUp(sf::Mouse::Button button);

	static sf::Vector2i GetMousePosition() { return mousePosition; }

	static float GetAxisRaw(Axis axis);	// -1.0 0 1.0
	static float GetAxis(Axis axis);	// -1.0 ~ 1.0

private:
	static bool Contains(const std::list<int>& list, int code);
	static void Remove(std::list<int>& list, int code);
};


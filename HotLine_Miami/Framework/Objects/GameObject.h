#pragma once

class GameObject
{
protected:
	std::string name;
	
	bool active = true;

	sf::Vector2f position;
	float rotation = 0.f;
	sf::Vector2f scale;

	Origins originPreset;
	sf::Vector2f origin;

	HitBox hitBox;
	bool isDevMode = false;
public:
	SortingLayers sortingLayer = SortingLayers::Default;
	int sortingOrder = 0;

	GameObject(const std::string& name = "");
	virtual ~GameObject() = default;

	const std::string& GetName() const { return name; }
	void SetName(const std::string& n) { name = n; }

	bool IsActive() const { return active; }
	void SetActive(bool a) { active = a; }

	sf::Vector2f GetPosition() const { return position; }
	virtual void SetPosition(const sf::Vector2f& pos) { position = pos; }

	float GetRotation() const { return rotation; }
	virtual void SetRotation(float angle) { rotation = angle; }
	sf::Vector2f GetScale() const { return scale; }
	virtual void SetScale(const sf::Vector2f& scale) { this->scale = scale; }

	sf::Vector2f GetOrigin() const { return origin; }
	virtual void SetOrigin(Origins preset);
	virtual void SetOrigin(const sf::Vector2f& newOrigin)
	{
		origin = newOrigin;
		originPreset = Origins::Custom;
	}

	virtual sf::FloatRect GetLocalBounds()const { return { 0.f,0.f,0.f,0.f }; }
	virtual sf::FloatRect GetGlobalBounds()const { return { 0.f,0.f,0.f,0.f }; }
	
	virtual const HitBox& GetHitBox() const { return hitBox; }

	virtual void Init() = 0;
	virtual void Release() = 0;

	virtual void Reset();

	virtual void Update(float dt) = 0;
	virtual void LateUpdate(float dt) {}
	virtual void FixedUpdate(float dt) {}
	virtual void Draw(sf::RenderWindow& window) = 0;

	virtual void OnLocallize(Languages lang);

	void SetDevMode() { isDevMode = !isDevMode; }
};

struct DrawOrderComparer
{
	bool operator()(GameObject* a, GameObject* b) const
	{
		if (a->sortingLayer != b->sortingLayer)
			return a->sortingLayer > b->sortingLayer;
		return a->sortingOrder > b->sortingOrder;
	}
};


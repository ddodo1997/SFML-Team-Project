#pragma once
class UiFrame;
class Scene
{
protected:
	std::list<GameObject*> gameObjects;

	std::list<GameObject*> addGameObjects;
	std::list<GameObject*> removeGameObjects;

	sf::View worldView;
	sf::View uiView;

	sf::Vector2f windowSize = { 1920,1080 };

public:
	SceneIds id;

	Scene(SceneIds id);
	virtual ~Scene() = default;

	virtual void Init();
	virtual void Release();

	virtual void Enter();
	virtual void Exit();

	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void FixedUpdate(float dt);

	virtual void OnPreDraw();
	virtual void Draw(sf::RenderWindow& window);
	virtual void OnPostDraw();

	virtual void OnLocallize(Languages lang);

	template<typename T>
	T* AddGo(T* obj)
	{
		addGameObjects.push_back(obj);
		return obj;
	}

	virtual void RemoveGo(GameObject* obj);
	virtual GameObject* FindGo(const std::string& name);
	virtual int FindGoAll(const std::string& name, std::list<GameObject*>& list);

	void ApplyAddGo();
	void ApplyRemoveGO();

	sf::Vector2f ScreenToWorld(sf::Vector2i screenPos);
	sf::Vector2i WorldToScreen(sf::Vector2f worldPos);

	sf::Vector2f ScreenToUi(sf::Vector2i screenPos);
	sf::Vector2i UiToScreen(sf::Vector2f worldPos);
};


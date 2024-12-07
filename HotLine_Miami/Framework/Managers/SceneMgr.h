#pragma once

class Scene;

class SceneMgr : public Singleton<SceneMgr>
{
	friend class Singleton<SceneMgr>;

protected:
	std::vector<Scene*> scenes;

	SceneIds startScene = SceneIds::DevS;
	SceneIds currentScene;

	SceneIds nextScene = SceneIds::None;

	SceneMgr() = default;
	virtual ~SceneMgr() = default;

	SceneMgr(const SceneMgr&) = delete;
	SceneMgr& operator=(const SceneMgr&) = delete;

	void OnPreDraw();
	void OnPostDraw();

public:
	void Init();
	void Release();

	Scene* GetCurrentScene() { return scenes[(int)currentScene]; }
	SceneIds GetCurrentSceneId() const { return currentScene; }
	void ChangeScene(SceneIds id);

	void Update(float dt);
	void LateUpdate(float dt);
	void FixedUpdate(float dt);

	void Draw(sf::RenderWindow& window);
};


#define SCENE_MGR (SceneMgr::Instance())
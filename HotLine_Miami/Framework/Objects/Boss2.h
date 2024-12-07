#pragma once
#include "Panther.h"
#include "BodyGuard.h"
#include "MafiaBoss.h"
#include "Fountain.h"

class SceneGame;
class Player;

class Boss2 : public GameObject
{
public:
	enum class Phase
	{
		None = -1,
		Panther,
		BodyGuard,
		MafiaBoss,
		Clear,
	};
protected:
	Panther panther1;
	Panther panther2;
	BodyGuard bodyGuard;
	MafiaBoss mafiaBoss;
	Fountain fountain1;
	Fountain fountain2;

	sf::FloatRect awakeBound;

	SceneGame* sceneGame;
	Player* player;

	Phase currentPhase = Phase::None;
public:
	Boss2(const std::string& name = "");
	~Boss2() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	std::vector<Panther*> GetPanthers();
	BodyGuard* GetBodyGuard() { return &bodyGuard; }
	MafiaBoss* GetMafiaBoss() { return &mafiaBoss; }
	Fountain* GetFountain1() { return &fountain1; }
	Fountain* GetFountain2() { return &fountain2; }
	void Init() override;
	void Release() override;
	void Reset() override;
	void SetPlayer();
	void Update(float dt) override;
	void Phase1(float dt);
	void Phase2(float dt);
	void Phase3(float dt);

	void ChangePhase(Phase phase);

	void Draw(sf::RenderWindow& window) override;
};

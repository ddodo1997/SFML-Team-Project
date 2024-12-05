#pragma once

class UiHudL : public GameObject
{
protected:	
	sf::Sprite mouseSprite;
	Animator cursorAnimator;

	sf::Text currentWeaponStatus;

	sf::Text volumeDisplayer;

	int weaponTypeIndex = -1;

	int bgmVol;	

public:
	UiHudL(const std::string& name = "");
	~UiHudL() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void UpdateMouseSprite(float dt);
	void UpdateWeaponStatus(Weapon::WeaponStatus weaponStatus, int remainingBullet);
	void UpdateVolumeDisplay(float dt);

	void Draw(sf::RenderWindow& window) override;
};

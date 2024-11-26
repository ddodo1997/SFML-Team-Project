#pragma once
struct AnimationEvent
{
	std::string id;
	int frame = 0;
	std::list<std::function<void()>> actions;
	bool operator==(const AnimationEvent& other) const
	{
		return (id == other.id) && (frame == other.frame);
	}
};

struct AnimationEventHash
{
	std::size_t operator()(const std::pair<std::string, int>& pair) const
	{
		std::size_t h1 = std::hash<std::string>()(pair.first) >> 1;
		std::size_t h2 = std::hash<int>()(pair.second) << 1;
		return h1 ^ h2;
	}
};

class Animator
{
protected:
	std::unordered_map<std::pair<std::string, int>, AnimationEvent, AnimationEventHash> events;
	std::queue<std::string> playQueue;

	AnimationClip* currentClip;
	sf::Sprite* sprite = nullptr;

	bool isPlaying = false;

	int currentFrame = 0;
	int totalFrame = 0;
	float frameDuration = 0.f;

	float accumTime = 0.f;

	bool isReverse = false;

public:
	Animator() = default;
	~Animator() = default;

	void SetTarget(sf::Sprite* sprite) { this->sprite = sprite; }
	void AddEvent(const std::string& id, int frame, std::function<void()> action);
	void ClearEvent() { events.clear(); }
	void Update(float dt);

	bool IsPlaying() const { return isPlaying; }

	void Play(const std::string& clipId, bool clearQueue = true);
	void Play(AnimationClip* clip, bool clearQueue = true);
	void PlayQueue(const std::string& clipId);

	//void Pause(bool pause);
	void Stop();

	void SetFrame(const AnimationFrame& frame);

	const std::string& GetCurrentClipId() const
	{
		if (currentClip == nullptr)
			return "";
		return currentClip->id;
	}

	void SetSpeed(float speed);

	explicit operator bool() const {
		return sprite != nullptr;
	}
};


#include "stdafx.h"
#include "Animator.h"

void Animator::AddEvent(const std::string& id, int frame, std::function<void()> action)
{
	std::pair<std::string, int> key(id, frame);
	auto find = events.find(key);
	if (find == events.end())
	{
		events.insert({ key, {id,frame } });
	}
	events[key].actions.push_back(action);
}

void Animator::Update(float dt)
{
	//현재 플레이중이 아니라면 업데이트 하지않음
	if (!isPlaying)
		return;
	//누적 시간에 델타타임을 누적
	accumTime += dt;
	//누적 시간이 프레임 변환 간격보다 적다면 업데이트 하지 않음
	if (accumTime < frameDuration)
		return;	
	
	auto find = events.find({ currentClip->id, currentFrame });
	if (find != events.end())
	{
		auto& ev = *find;
		for (auto& func : ev.second.actions)
		{
			if (func)
				func();
		}
	}

	if (!isReverse)
	{
		//현재 프레임을 다음 프레임으로 바꾸고, 누적시간을 0으로 초기화
		currentFrame++;
		accumTime = 0.f;
		//만약 현재 프레임이 마지막 프레임 이라면
		if (currentFrame == totalFrame)
		{
			if (!playQueue.empty())
			{
				std::string clipId = playQueue.front();
				Play(clipId, false);
				playQueue.pop();
				return;
			}
			
			//루프 타입을 검사해서, 마지막 프레임에서 멈추거나, 처음 프레임으로 돌린다
			switch (currentClip->loopType)
			{
			case AnimationLoopTypes::Single:
				currentFrame = totalFrame - 1;
				break;
			case AnimationLoopTypes::Loop:
				currentFrame = 0;
				break;
			case AnimationLoopTypes::PingPong:
				currentFrame = totalFrame - 1;
				isReverse = true;
				break;
			}
		}
	}
	else
	{
		currentFrame--;
		accumTime = 0.f;
		if (currentFrame <= 0)
		{
			if (!playQueue.empty())
			{
				std::string clipId = playQueue.front();
				Play(clipId, false);
				playQueue.pop();
				return;
			}
			switch (currentClip->loopType)
			{
			case AnimationLoopTypes::Single:
				currentFrame = 0;
				break;
			case AnimationLoopTypes::Loop:
				currentFrame = totalFrame - 1;
				break;
			case AnimationLoopTypes::PingPong:
				currentFrame = 0;
				isReverse = false;
				break;
			}
		}
	}
	//프레임을 설정한다.
	SetFrame(currentClip->frames[currentFrame]);
}

void Animator::Play(const std::string& clipId, bool clearQueue)
{
	Play(&ANI_CLIP_MGR.Get(clipId),clearQueue);
}

void Animator::Play(AnimationClip* clip, bool clearQueue)
{
	if (clearQueue)
	{
		while (!playQueue.empty())
			playQueue.pop();
	}

	//플레이중으로 바꾸기
	isPlaying = true;

	//현재 클립을 매개변수 클립으로 바꾸고
	currentClip = clip;
	//첫 프레임으로 바꾸고
	currentFrame = 0;
	//총 프레임 수를 현재 클립의 사이즈로 바꾼다
	totalFrame = currentClip->frames.size();

	//프레임 변환 시간을 설정하고
	frameDuration = 1.f / currentClip->fps;

	//누적 시간을 0으로 설정하고
	accumTime = 0.f;

	//프레임을 현재 클립의 현재 프레임(현재 0) 으로 설정한다.
	SetFrame(currentClip->frames[currentFrame]);
}

void Animator::PlayQueue(const std::string& clipId)
{
	playQueue.push(clipId);
}

void Animator::Stop()
{
	accumTime = 0.f;
	isPlaying = false;
}

void Animator::SetFrame(const AnimationFrame& frame)
{
	sprite->setTexture(TEXTURE_MGR.Get(frame.texId));
	sprite->setTextureRect(frame.texCoord);
}

void Animator::SetSpeed(float speed)
{
	if (speed < 0.f)
	{
		isReverse = true;
		currentFrame = totalFrame - 1;
	}
	else
		isReverse = false;

	frameDuration /= std::fabs(speed);
}

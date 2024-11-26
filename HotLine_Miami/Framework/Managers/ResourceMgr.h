#pragma once

template<typename T>
class ResourceMgr : public Singleton<ResourceMgr<T>>
{
	friend Singleton<ResourceMgr<T>>;

protected:
	std::unordered_set<std::string> notUnloadAllResources;
	std::unordered_map<std::string, T*> resources;

	ResourceMgr() = default;
	~ResourceMgr()
	{
		for (auto pair : resources)
		{
			delete pair.second;
		}
	}

	ResourceMgr(const ResourceMgr&) = delete;
	ResourceMgr& operator=(const ResourceMgr&) = delete;

public:
	static T Empty;

	void UnloadAll()
	{
		auto it = resources.begin();
		while (it != resources.end())
		{
			if (notUnloadAllResources.find(it->first) == notUnloadAllResources.end())
			{
				delete it->second;
				it = resources.erase(it);
			}
			else
			{
				it++;
			}
		}
	}

	bool Load(const std::string& id, bool notUnloadAll = false)
	{
		if (resources.find(id) != resources.end())
			return false;

		T* resource = new T();
		bool success = resource->loadFromFile(id);
		if (success)
		{
			resources.insert({ id, resource });
			if(notUnloadAll)
				notUnloadAllResources.insert(id);
		}
		else
		{
			delete resource;
		}

		return success;
	}

	bool Unload(const std::string& id)
	{
		auto it = resources.find(id);
		if (it == resources.end())
			return false;

		delete it->second;
		resources.erase(it);

		auto find = notUnloadAllResources.find(id);
		if (find != notUnloadAllResources.end())
		{
			notUnloadAllResources.erase(find);
		}
		return true;
	}

	T& Get(const std::string& id)
	{
		auto it = resources.find(id);
		if (it == resources.end() && !Load(id))
		{
			return Empty;
		}
		return *(resources[id]);
	}
};

template<typename T>
T ResourceMgr<T>::Empty;

#define TEXTURE_MGR (ResourceMgr<sf::Texture>::Instance())
#define FONT_MGR (ResourceMgr<sf::Font>::Instance())
#define SOUNDBUFFER_MGR (ResourceMgr<sf::SoundBuffer>::Instance())
#define ANI_CLIP_MGR (ResourceMgr<AnimationClip>::Instance())
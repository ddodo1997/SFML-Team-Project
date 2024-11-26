#pragma once

template<typename T>
class ObjectPool
{
private:
	std::list<T*> unused;
	std::list<T*> used;

public:
	ObjectPool(int initSize = 10)
	{
		for (int i = 0; i < initSize; ++i)
		{
			auto obj = new T();
			obj->Init();
			unused.push_back(obj);
		}
	}

	~ObjectPool()
	{
		for (auto obj : unused)
		{
			delete obj;
		}
		for (auto obj : used)
		{
			delete obj;
		}
	}

	T* Take()
	{
		if (unused.empty())
		{
			auto obj = new T();
			obj->Init();
			obj->Reset();
			used.push_back(obj);
			return obj;
		}

		auto obj = unused.front();
		unused.pop_front();
		used.push_back(obj);
		obj->SetActive(true);
		obj->Reset();
		return obj;
	}

	void Return(T* obj)
	{
		auto find = std::find(used.begin(), used.end(), obj);
		if (find == used.end())
		{
			std::cout << "Invaild Obj" << std::endl;
			return;
		}
		used.erase(find);
		obj->SetActive(false);
		unused.push_back(obj);
	}
};
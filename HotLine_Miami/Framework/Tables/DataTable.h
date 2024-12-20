#pragma once
class DataTable
{
public:
	enum class Types
	{
		String,
		Walls,
		Enemies,
		Decos,
		Stages,
		Item,
		Weapon,
		Alphabet,
	};
	static const int TotalType = 8;

protected:
	Types type;

public:

	DataTable(Types type) :type(type) {}
	~DataTable() = default;

	virtual bool Load() = 0;
	virtual void Release() = 0;
};
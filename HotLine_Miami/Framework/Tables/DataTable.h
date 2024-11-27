#pragma once
class DataTable
{
public:
	enum class Types
	{
		String,
		Stages,
		Floors,
		Decos,
		Walls,
		Enemies,
	};
	static const int TotalType = 6;

protected:
	Types type;

public:

	DataTable(Types type) :type(type) {}
	~DataTable() = default;

	virtual bool Load() = 0;
	virtual void Release() = 0;
};
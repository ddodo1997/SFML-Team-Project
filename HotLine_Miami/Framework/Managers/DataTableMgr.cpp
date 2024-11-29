#include "stdafx.h"
#include "DataTableMgr.h"
#include "EnemyTable.h"
#include "DecorationTable.h"
#include "WallTable.h"
#include "StageTable.h"

DataTableMgr::~DataTableMgr()
{
	Release();
}

void DataTableMgr::Init()
{
	Release();
	tables.insert({ DataTable::Types::Weapon, new WeaponTable() });
	tables.insert({ DataTable::Types::Walls, new WallTable() });
	tables.insert({ DataTable::Types::Enemies, new EnemyTable() });
	tables.insert({ DataTable::Types::Decos, new DecorationTable() });
	tables.insert({ DataTable::Types::Stages, new StageTable() });
	//tables.insert({ DataTable::Types::String ,new StringTable() });
	for (auto t : tables)
	{
		t.second->Load();
	}
}

void DataTableMgr::Release()
{
	for (auto t : tables)
	{
		t.second->Release();
		delete t.second;
	}
	tables.clear();
}

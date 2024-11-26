#pragma once
class DataTableMgr :
    public Singleton<DataTableMgr>
{
	friend class Singleton<DataTableMgr>;

protected:
	DataTableMgr() = default;
	virtual ~DataTableMgr();

	DataTableMgr(const DataTableMgr&) = delete;
	DataTableMgr& operator=(const DataTableMgr&) = delete;

	std::unordered_map<DataTable::Types, DataTable*> tables;

public:
	void Init();
	void Release();

	template<typename T>
	T* Get(DataTable::Types t);
};

template<typename T>
inline T* DataTableMgr::Get(DataTable::Types t)
{
	auto find = tables.find(t);
	if(find == tables.end())
		return nullptr;

	return dynamic_cast<T*>(find->second);
}

#define DATATABLE_MGR (DataTableMgr::Instance())
#define STRING_TABLE (DATATABLE_MGR.Get<StringTable>(DataTable::Types::String))
//#define ITEM_TABLE (DATATABLE_MGR.Get<StringTable>(DataTable::Types::Item))
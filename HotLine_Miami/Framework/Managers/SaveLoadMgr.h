#pragma once
#include "SaveData.h"
#include "Singleton.h"
class SaveLoadMgr :
    public Singleton<SaveLoadMgr>
{
public:
    friend Singleton<SaveLoadMgr>;
protected:
    std::string savePath = "save/save.json";

    SaveLoadMgr() = default;
    ~SaveLoadMgr() = default;
    SaveLoadMgr(const SaveLoadMgr&) = delete;
    SaveLoadMgr& operator=(const SaveLoadMgr&) = delete;
public:
    void Save(const SaveDataVC& saveData);
    SaveDataVC Load();
};

#define SAVELOAD_MGR (Singleton<SaveLoadMgr>::Instance())
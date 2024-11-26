#include "stdafx.h"
#include "SaveLoadMgr.h"

void SaveLoadMgr::Save(const SaveDataVC& saveData)
{
	json j = saveData;
	std::ofstream f(savePath);
	f << j.dump(4) << std::endl;
	f.close();
}

SaveDataVC SaveLoadMgr::Load()
{
	std::ifstream f(savePath);
	json j = json::parse(f);

	int version = j["version"];

	SaveData* saveData = nullptr;
	switch (version)
	{
	case 1:
	{
		SaveDataV1 v1 = j.get<SaveDataV1>();
		saveData = new SaveDataV1(v1);
		break;
	}
	}
	f.close();

	while (saveData->version < 1)
	{
		SaveData* oldData = saveData;
		saveData = saveData->VersionUp();
		delete oldData;
	}

	SaveDataVC ret(*((SaveDataVC*)saveData));
	delete saveData;
	return ret;
}

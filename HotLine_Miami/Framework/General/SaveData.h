#pragma once

class SaveData
{
public:
	int version = 0;
	virtual SaveData* VersionUp() = 0;
};

class SaveDataV1 : public SaveData
{
public:
	SaveDataV1() { version = 1; }

	SaveData* VersionUp() override { return nullptr; }
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SaveDataV1, version);
};


typedef SaveDataV1 SaveDataVC;
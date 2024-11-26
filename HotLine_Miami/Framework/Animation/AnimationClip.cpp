#include "stdafx.h"
#include "AnimationClip.h"
using json = nlohmann::json;
bool AnimationClip::loadFromFile(const std::string& filePath)
{
	std::ifstream fStream(filePath);
	json data = json::parse(fStream);
	id = data["ID"];
	loopType = data["LoopType"];
	fps = data["FPS"];

	for (int i = 0; i < data["Frames"].size(); i++)
	{
		frames.push_back({ data["texId"],
			{
				data["Frames"][i][0],
				data["Frames"][i][1],
				data["Frames"][i][2],
				data["Frames"][i][3],
			}
		});
	}


	///////////////////////////////////////////////////
	/// animation json parsing test code
	/// print all to consol
	///////////////////////////////////////////////////
	//std::cout << id << " " << fps << " " << (int)loopType << std::endl;
	//for (auto& a : frames)
	//{
	//	std::cout << a.texId << ": " << a.texCoord.left << ", " << a.texCoord.top << ", " << a.texCoord.width << ", " << a.texCoord.height << std::endl;
	//}
	//std::cout << std::endl;
	//std::cout << std::endl;
	///////////////////////////////////////////////////


	fStream.close();
	return true;
}

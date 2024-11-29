#pragma once

enum class Languages
{
	English,
	Count,
};

enum class SortingLayers
{
	Background,
	Foreground,
	Default,
	UI,
};

enum class SceneIds
{
	None = -1,
	Dev_K,
	DevL,
	DevS,
	Count,
};

enum class Origins
{
	// T M B
	// L C R
	TL, TC, TR,
	ML, MC, MR,
	BL, BC, BR,
	Custom,
};

enum class Sides
{
	Left,
	Right,
	None,
};
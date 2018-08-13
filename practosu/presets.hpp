#pragma once
#include "stdafx.h"

struct preset
{
	std::string sPresetName;
	int32_t sAR = -1;
	int32_t sCS = -1;
	int32_t sOD = -1;
	int32_t sHP = -1;
	std::string sFilename;
	std::string sCreator;
	std::string sVersion;
	float sSpeed = -1.0;
};

namespace presets
{
	inline bool mInitialized = false;
	inline std::vector<preset> mPresets;

	void initialize();
	std::string convertToString(preset aPreset);
	void writePreset(preset aPreset);
	preset convertToPreset(std::string aString);
	preset getPresetByName(std::string aName);
	std::vector<std::string> presetNames();
}
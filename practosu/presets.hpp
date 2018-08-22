#pragma once
#include "stdafx.h"

// Struct used to form a preset.
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

// presets allow the user to quickly apply a variety of
// settings to a given beatmap. No checking is done on
// this data, so errors may arise if unexpected data
// is used.
namespace presets
{
	// Flag for when loading of presets is complete.
	inline bool mInitialized = false;
	// List of all presets loaded from presets file.
	inline std::vector<preset> mPresets;

	// Initialize presets std::vector.
	void initialize();
	// Convert a preset type to a string for storage in
	// the presets file.
	std::string convertToString(preset aPreset);
	// Write a preset type to a file using convertToString.
	void writePreset(preset aPreset);
	// Convert a given string to a preset type.
	preset convertToPreset(std::string aString);
	// Find preset in mPresets using the preset name.
	preset getPresetByName(std::string aName);
	// Returns a list of all preset names in mPresets.
	std::vector<std::string> presetNames();
}
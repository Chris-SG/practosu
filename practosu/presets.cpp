#include "stdafx.h"
#include "presets.hpp"
#include <sstream>

using namespace std;
namespace fs = std::experimental::filesystem;

namespace presets
{
	void initialize()
	{
		// Open presets file.
		fstream lFileStream;
		lFileStream.open("presets.txt", fstream::in | fstream::out);

		std::string lString;
		mPresets.clear();
		// Load presets from file.
		while(std::getline(lFileStream, lString))
			mPresets.push_back(convertToPreset(lString));
		mInitialized = true;
		lFileStream.close();
	}

	preset convertToPreset(std::string aString)
	{
		// Parse a string as a preset type. No error checking is done!
		std::stringstream lSs;
		std::string lToken;
		lSs << aString;
		preset lPreset;
		lSs.get();
		while (lSs.peek() != '\"')
		{
			// Will get stuck in an infinite loop if no closing quotation mark.
			lPreset.sPresetName += lSs.get();
		}
		lSs.get();
		lSs.get();
		std::getline(lSs, lToken, ',');
		lPreset.sHP = stoi(lToken);
		std::getline(lSs, lToken, ',');
		lPreset.sCS = stoi(lToken);
		std::getline(lSs, lToken, ',');
		lPreset.sOD = stoi(lToken);
		std::getline(lSs, lToken, ',');
		lPreset.sAR = stoi(lToken);
		if (lSs.peek() == '\"')
		{
			lSs.get();
			while (lSs.peek() != '\"')
			{
				// Will get stuck in an infinite loop if no closing quotation mark.
				lPreset.sFilename += lSs.get();
			}
			lSs.get();
		}
		lSs.get();
		if (lSs.peek() == '\"')
		{
			lSs.get();
			while (lSs.peek() != '\"')
			{
				// Will get stuck in an infinite loop if no closing quotation mark.
				lPreset.sCreator += lSs.get();
			}
			lSs.get();
		}
		lSs.get();
		if (lSs.peek() == '\"')
		{
			lSs.get();
			while (lSs.peek() != '\"')
			{
				// Will get stuck in an infinite loop if no closing quotation mark.
				lPreset.sVersion += lSs.get();
			}
			lSs.get();
		}
		lSs.get();
		lSs >> lPreset.sSpeed;

		return lPreset;
	}

	void writePreset(preset aPreset)
	{
		// Insert new preset to end of file.
		auto lPreset = convertToString(aPreset);
		fstream lFileStream;
		lFileStream.open("presets.txt", fstream::in | fstream::out | fstream::app);
		lFileStream << lPreset << std::endl;
		lFileStream.close();
	}

	std::string convertToString(preset aPreset)
	{
		// Convert a preset object into a string for file storage.
		std::string lRet = "\"";
		lRet += aPreset.sPresetName + "\"," + std::to_string(aPreset.sHP) + "," + std::to_string(aPreset.sCS) + "," + std::to_string(aPreset.sOD) + "," + std::to_string(aPreset.sAR) + ",";
		if (!aPreset.sFilename.empty())
		{
			lRet += "\"" + aPreset.sFilename + "\"";
		}
		lRet += ",";
		if (!aPreset.sCreator.empty())
		{
			lRet += "\"" + aPreset.sCreator + "\"";
		}
		lRet += ",";
		if (!aPreset.sVersion.empty())
		{
			lRet += "\"" + aPreset.sVersion + "\"";
		}
		lRet += ",";
		lRet += std::to_string(aPreset.sSpeed);
		return lRet;
	}

	std::vector<std::string> presetNames()
	{
		// Load all preset names.
		initialize();
		std::vector<std::string> lNames;
		for (preset lPreset : mPresets)
			lNames.push_back(lPreset.sPresetName);

		return lNames;
	}

	preset getPresetByName(std::string aName)
	{
		// Find preset by given name.
		for(preset lPreset : mPresets)
		{
			if (lPreset.sPresetName == aName)
				return lPreset;
		}
		return preset{};
	}
}
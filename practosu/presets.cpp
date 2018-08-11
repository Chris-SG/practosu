#include "stdafx.h"
#include "presets.hpp"
#include <sstream>

using namespace std;
namespace fs = std::experimental::filesystem;

namespace presets
{
	void initialize()
	{
		fstream lFileStream;
		lFileStream.open("presets.csv", fstream::in | fstream::out);

		std::string lString;
		while(std::getline(lFileStream, lString))
			mPresets.push_back(convertToPreset(lString));
		mInitialized = true;
	}

	preset convertToPreset(std::string aString)
	{
		std::stringstream lSs;
		lSs << aString;
		preset lPreset;
		lSs.get();
		while (lSs.peek() != '\"')
		{
			lPreset.sPresetName += lSs.get();
		}
		lSs.get();
		lSs >> lPreset.sAR;
		lSs.get();
		lSs >> lPreset.sCS;
		lSs.get();
		lSs >> lPreset.sOD;
		lSs.get();
		lSs >> lPreset.sHP;
		lSs.get();
		if (lSs.peek() == '\"')
		{
			lSs.get();
			while (lSs.peek() != '\"')
			{
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
				lPreset.sVersion += lSs.get();
			}
			lSs.get();
		}
		lSs.get();
		lSs >> lPreset.sSpeed;

		return lPreset;
	}

	std::string convertToString(preset aPreset)
	{
		std::string lRet = "\"";
		lRet += aPreset.sPresetName + "\"," + std::to_string(aPreset.sAR) + "," + std::to_string(aPreset.sCS) + "," + std::to_string(aPreset.sOD) + "," + std::to_string(aPreset.sHP) + ",";
		if (!aPreset.sFilename.empty())
		{
			lRet += "\"" + aPreset.sFilename + "\",";
		}
		if (!aPreset.sCreator.empty())
		{
			lRet += "\"" + aPreset.sCreator + "\",";
		}
		if (!aPreset.sVersion.empty())
		{
			lRet += "\"" + aPreset.sVersion + "\",";
		}
		lRet += aPreset.sSpeed;
		return lRet;
	}



	std::vector<std::string> presetNames()
	{
		if (!mInitialized)
			initialize();
		std::vector<std::string> lNames;
		for (preset lPreset : mPresets)
			lNames.push_back(lPreset.sPresetName);

		return lNames;
	}
}
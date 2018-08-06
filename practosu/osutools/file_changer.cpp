
#include "stdafx.h"
#include "file_changer.hpp"
#include "file_structs.hpp"
#include "func.hpp"

using namespace std;
namespace fs = std::experimental::filesystem;

namespace osu_tools
{
	namespace file_changer
	{

		void set_speed_multiplier(float aMulti, osu_file& aOsuFile, string aNewFilename)
		{
			std::string lFullPath = osu_tools::func::get_beatmap_directory().string() + aOsuFile.sFolderName;
			if(!fs::exists(lFullPath + "\\" + aOsuFile.sAudioFilename))
			{
				std::string lCommandLine = "-nostats -loglevel 0 -i \"" + lFullPath + "\\" + aOsuFile.sAudioFilename + "\" -filter:a \"";
				auto lMulti = aMulti;
				if (lMulti > 2.0)
				{
					do
					{
						lCommandLine += "atempo=2.0,";
						lMulti /= 2.0;
					} while (lMulti > 2.0);
					lCommandLine += "atempo=" + std::to_string(lMulti);
				}
				else if (aMulti < 0.5)
				{
					do
					{
						lCommandLine += "atempo=0.5,";
						lMulti /= 0.5;
					} while (lMulti < 0.5);
					lCommandLine += "atempo=" + std::to_string(lMulti);
				}
				else
				{
					lCommandLine += "atempo=" + std::to_string(lMulti);
				}
				lCommandLine += "\" -vn \"" + lFullPath + "\\" + aNewFilename + "\"";
				STARTUPINFOA lSi;
				PROCESS_INFORMATION lPi;
				ZeroMemory(&lSi, sizeof(lSi));
				lSi.cb = sizeof(lSi);
				ZeroMemory(&lPi, sizeof(lPi));
				std::string lffPath = fs::current_path().string() + "\\ffmpeg.exe";
				if (!CreateProcessA(lffPath.c_str(), const_cast<char *>(lCommandLine.c_str()), NULL, NULL, false, 0, NULL, NULL, &lSi, &lPi))
					throw "Failed to create process: " + GetLastError();
			}

			try
			{
				/*aOsuFile.sBeatmapID = 0;
				aOsuFile.sCreator = "Bauxe";
				aOsuFile.sVersion = "Test";
				aOsuFile.sFileName = aOsuFile.sArtist + " - " + aOsuFile.sTitle + " (" + aOsuFile.sCreator + ") [" + aOsuFile.sVersion + "].osu";*/
				aOsuFile.sAudioFilename = aNewFilename;
				aOsuFile.sPreviewTime /= aMulti;

				for (auto& lTimingPoint : aOsuFile.sTimingPoints)
				{
					lTimingPoint.sOffset /= aMulti;
					if (lTimingPoint.sInherited)
						lTimingPoint.sMSPerBeat /= aMulti;
				}

				for (auto& lBreakPeriod : aOsuFile.sBreakPeriods)
				{
					lBreakPeriod.first /= aMulti;
					lBreakPeriod.second /= aMulti;
				}

				for (auto& lHitObject : aOsuFile.sHitObjects)
				{
					if (lHitObject->sType & (1 << 0))
					{
						hit_object_circle* lObject = static_cast<hit_object_circle*>(lHitObject);
						lObject->sTime /= aMulti;
					}
					else if (lHitObject->sType & (1 << 1))
					{
						hit_object_slider* lObject = static_cast<hit_object_slider*>(lHitObject);
						lObject->sTime /= aMulti;

						/*int lBeatDuration = 0, lBeatPercentage = -100;
						for(const auto& lTimingPoint : aOsuFile.sTimingPoints)
						{
						if (lTimingPoint.sOffset < lObject->sTime)
						{
						if (lTimingPoint.sMSPerBeat > 0)
						lBeatDuration = lTimingPoint.sMSPerBeat;
						else
						lBeatPercentage = lTimingPoint.sMSPerBeat;
						}
						else
						break;
						}
						lObject->sPixelLength = (lObject->sPixelLength / (100.0 * aOsuFile.sSliderMultiplier)) * (lBeatDuration * (abs(lBeatPercentage) / 100));*/
					}
					else if (lHitObject->sType & (1 << 3))
					{
						hit_object_spinner* lObject = static_cast<hit_object_spinner*>(lHitObject);
						lObject->sTime /= aMulti;
						lObject->sEndTime /= aMulti;
					}
				}
			}
			catch (std::exception& e)
			{
				throw "Error parsing .osu file: " + *e.what();
			}
			
		}

		void set_speed_percentage(float aMulti, osu_file& aOsuFile);
	}
}

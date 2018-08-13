#include "stdafx.h"
#include "file_writer.hpp"
#include "func.hpp"
#include "file_structs.hpp"
#include "file_changer.hpp"

using namespace std;
namespace fs = std::experimental::filesystem;

namespace osu_tools
{
	namespace file_writer
	{
		static inline void ReplaceAll(std::string &str, const std::string& from, const std::string& to)
		{
			size_t start_pos = 0;
			while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
				str.replace(start_pos, from.length(), to);
				start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
			}
		}

		bool write_file(osu_file& aOsuFile, std::string& aNewFilename, std::string& aNewAudioFilename, std::string& aNewVersion, const float& aMultiplier)
		{
			ReplaceAll(aNewVersion, "%VER%", aOsuFile.sVersion);
			ReplaceAll(aNewVersion, "%SPEED%", std::to_string(aMultiplier));
			ReplaceAll(aNewVersion, "%AR%", std::to_string(aOsuFile.sApproachRate));
			ReplaceAll(aNewVersion, "%CS%", std::to_string(aOsuFile.sCircleSize));
			ReplaceAll(aNewVersion, "%OD%", std::to_string(aOsuFile.sOverallDifficulty));
			ReplaceAll(aNewVersion, "%HP%", std::to_string(aOsuFile.sHPDrainRate));

			//ReplaceAll(aNewFilename, "%NAME%", aOsuFile.sFileName);
			ReplaceAll(aNewFilename, "%NAME%", aOsuFile.sArtist + " - " + aOsuFile.sTitle + " (" + aOsuFile.sCreator + ") [" + aNewVersion + "]");
			ReplaceAll(aNewFilename, "%VER%", aOsuFile.sVersion);
			ReplaceAll(aNewFilename, "%SPEED%", std::to_string(aMultiplier));
			ReplaceAll(aNewFilename, "%AR%", std::to_string(aOsuFile.sApproachRate));
			ReplaceAll(aNewFilename, "%CS%", std::to_string(aOsuFile.sCircleSize));
			ReplaceAll(aNewFilename, "%OD%", std::to_string(aOsuFile.sOverallDifficulty));
			ReplaceAll(aNewFilename, "%HP%", std::to_string(aOsuFile.sHPDrainRate));
			if (aNewFilename.find(".osu") == std::string::npos)
				aNewFilename += ".osu";
			char lIllegalChars[] = "\\/:*?\"<>|";
			for(uint32_t i = 0; i < strlen(lIllegalChars); ++i)
				aNewFilename.erase(std::remove(aNewFilename.begin(), aNewFilename.end(), lIllegalChars[i]));



			auto lSongPath = osu_tools::func::get_beatmap_directory();
			lSongPath += aOsuFile.sFolderName;
			lSongPath /= aNewFilename;
			aOsuFile.sVersion = aNewVersion;

			// if file already exists, we don't want to recreate it
			if (fs::exists(lSongPath))
				throw std::invalid_argument("Beatmap file name already exists! Please change file name");
			if (aMultiplier != 1.0)
				osu_tools::file_changer::set_speed_multiplier(aMultiplier, aOsuFile, aNewAudioFilename);

			aOsuFile.sBeatmapID = 0;

			ofstream lFileStream;
			lFileStream.open(lSongPath.string(), ofstream::out | ofstream::app);
			//	fs::create(lSongPath);
			lFileStream << "osu file format v" << static_cast<int>(aOsuFile.sBeatmapVersion) << std::endl << std::endl;

			lFileStream << "[General]" << std::endl;
			if (!aOsuFile.sAudioFilename.empty())
				lFileStream << "AudioFilename:" << aOsuFile.sAudioFilename << std::endl;
			if(aOsuFile.sAudioLeadIn != -1)
				lFileStream << "AudioLeadIn:" << aOsuFile.sAudioLeadIn << std::endl;
			if (aOsuFile.sPreviewTime != -1)
				lFileStream << "PreviewTime:" << aOsuFile.sPreviewTime << std::endl;
			if(aOsuFile.sCountdown != -1)
				lFileStream << "Countdown:" << static_cast<int>(aOsuFile.sCountdown) << std::endl;
			if (!aOsuFile.sSampleSet.empty())
				lFileStream << "SampleSet:" << aOsuFile.sSampleSet << std::endl;
			if (aOsuFile.sStackLeniency != -1.0)
				lFileStream << "StackLeniency:" << aOsuFile.sStackLeniency << std::endl; // float x.x
			if (aOsuFile.sMode != -1)
				lFileStream << "Mode:" << static_cast<int>(aOsuFile.sMode) << std::endl; // uint8_t
			if(aOsuFile.sLetterboxInBreaks != -1)
				lFileStream << "LetterboxInBreaks:" << static_cast<int>(aOsuFile.sLetterboxInBreaks) << std::endl;
			if (!aOsuFile.sSkinPreference.empty())
				lFileStream << "SkinPreference:" << aOsuFile.sSkinPreference << std::endl;
			if(static_cast<int>(aOsuFile.sEpilepsyWarning) != -1)
				lFileStream << "EpilepsyWarning:" << static_cast<int>(aOsuFile.sEpilepsyWarning) << std::endl;
			if(aOsuFile.sWidescreenStoryboard != -1)
				lFileStream << "WidescreenStoryboard:" << static_cast<int>(aOsuFile.sWidescreenStoryboard) << std::endl;
			lFileStream << std::endl;

			lFileStream << "[Editor]" << std::endl;
			if(!aOsuFile.sBookmarks.empty())
			{
				lFileStream << "Bookmarks:";
				for (uint32_t i = 0; i < aOsuFile.sBookmarks.size(); ++i)
				{
					lFileStream << aOsuFile.sBookmarks[i];
					if (i != aOsuFile.sBookmarks.size() - 1)
						lFileStream << ",";
				}
			}
			if(aOsuFile.sDistanceSpacing != -1.0)
				lFileStream << "DistanceSpacing:" << aOsuFile.sDistanceSpacing << std::endl;
			if(aOsuFile.sBeatDivisor != -1)
				lFileStream << "BeatDivisor:" << static_cast<int>(aOsuFile.sBeatDivisor) << std::endl;
			if(aOsuFile.sGridSize != -1)
				lFileStream << "GridSize:" << static_cast<int>(aOsuFile.sGridSize) << std::endl;
			if(aOsuFile.sTimelineZoom != -1.0)
				lFileStream << "TimelineZoom:" << aOsuFile.sTimelineZoom << std::endl; // float x.x
			lFileStream << std::endl;
			

			lFileStream << "[Metadata]" << std::endl;
			if(!aOsuFile.sTitle.empty())
				lFileStream << "Title:" << aOsuFile.sTitle << std::endl;
			if (!aOsuFile.sTitleUnicode.empty())
				lFileStream << "TitleUnicode:" << aOsuFile.sTitleUnicode << std::endl;
			if (!aOsuFile.sArtist.empty())
				lFileStream << "Artist:" << aOsuFile.sArtist << std::endl;
			if (!aOsuFile.sArtistUnicode.empty())
				lFileStream << "ArtistUnicode:" << aOsuFile.sArtistUnicode << std::endl;
			if (!aOsuFile.sCreator.empty())
				lFileStream << "Creator:" << aOsuFile.sCreator << std::endl;
			if (!aOsuFile.sVersion.empty())
				lFileStream << "Version:" << aOsuFile.sVersion << std::endl;
			if (!aOsuFile.sSource.empty())
				lFileStream << "Source:" << aOsuFile.sSource << std::endl;
			if (!aOsuFile.sTags.empty())
				lFileStream << "Tags:" << aOsuFile.sTags << std::endl;
			if(aOsuFile.sBeatmapID != -1)
				lFileStream << "BeatmapId:" << aOsuFile.sBeatmapID << std::endl;
			if(aOsuFile.sBeatmapSetID != -1)
				lFileStream << "BeatmapSetID:" << aOsuFile.sBeatmapSetID << std::endl << std::endl;
			lFileStream << std::endl;

			lFileStream << "[Difficulty]" << std::endl; // all float x.x
			if (aOsuFile.sHPDrainRate != -1.0)
				lFileStream << "HPDrainRate:" << aOsuFile.sHPDrainRate << std::endl;
			if (aOsuFile.sCircleSize != -1.0)
				lFileStream << "CircleSize:" << aOsuFile.sCircleSize << std::endl;
			if (aOsuFile.sOverallDifficulty != -1.0)
				lFileStream << "OverallDifficulty:" << aOsuFile.sOverallDifficulty << std::endl;
			if (aOsuFile.sApproachRate != -1.0)
				lFileStream << "ApproachRate:" << aOsuFile.sApproachRate << std::endl;
			if (aOsuFile.sSliderMultiplier != -1.0)
				lFileStream << "SliderMultiplier:" << aOsuFile.sSliderMultiplier << std::endl; // float x.xx
			if (aOsuFile.sSliderTickRate != -1.0)
				lFileStream << "SliderTickRate:" << aOsuFile.sSliderTickRate << std::endl << std::endl;

			lFileStream << "[Events]" << std::endl;
			lFileStream << "//Background and Video events" << std::endl;
			if (!aOsuFile.sBackground.empty())
			{
				lFileStream << "0,0,\"" << aOsuFile.sBackground << "\"";
				if (aOsuFile.sBeatmapVersion >= 10)
					lFileStream << ",0,0";
				lFileStream << std::endl;
			}
			lFileStream << "//Break Periods" << std::endl;
			for (const auto& lBreak : aOsuFile.sBreakPeriods)
				lFileStream << "2," << lBreak.first << "," << lBreak.second << std::endl;
			lFileStream << "//Storyboard Layer 0 (Background)" << std::endl;
			lFileStream << "//Storyboard Layer 1 (Fail)" << std::endl;
			lFileStream << "//Storyboard Layer 2 (Pass)" << std::endl;
			lFileStream << "//Storyboard Layer 3 (Foreground)" << std::endl;
			lFileStream << "//Storyboard Sound Samples" << std::endl;
			lFileStream << std::endl;

			lFileStream << "[TimingPoints]" << std::endl;
			for (const auto& lTimingPoint : aOsuFile.sTimingPoints)
				lFileStream << lTimingPoint.sOffset << "," << lTimingPoint.sMSPerBeat << "," << lTimingPoint.sMeter << "," << static_cast<int>(lTimingPoint.sSampleSet) << "," << static_cast<int>(lTimingPoint.sSampleIndex) << "," << static_cast<int>(lTimingPoint.sVolume) << "," << lTimingPoint.sInherited << "," << lTimingPoint.sKiai << std::endl;
			lFileStream << std::endl;

			lFileStream << "[Colours]" << std::endl;
			for (uint32_t i = 0; i < aOsuFile.sColours.size(); ++i)
				lFileStream << "Combo" << i + 1 << " : " << get<0>(aOsuFile.sColours[i]) << "," << get<1>(aOsuFile.sColours[i]) << "," << get<2>(aOsuFile.sColours[i]) << std::endl;
			lFileStream << std::endl;

			lFileStream << "[HitObjects]" << std::endl;
			for (const auto& lHitObject : aOsuFile.sHitObjects)
			{
				if (lHitObject->sType & (1 << 0))
				{
					hit_object_circle* lObject = static_cast<hit_object_circle*>(lHitObject);
					lFileStream << lObject->sX << "," << lObject->sY << "," << lObject->sTime << "," << static_cast<int>(lObject->sType) << "," << static_cast<int>(lObject->sHitSounds);
					if (lObject->sExtras.sSampleSet != -1)
					{
						lFileStream << "," << lObject->sExtras.sSampleSet << ":" << lObject->sExtras.sAdditionSet << ":" << lObject->sExtras.sCustomIndex << ":" << lObject->sExtras.sSampleVolume << ":" << lObject->sExtras.sFilename;
					}
				}
				else if (lHitObject->sType & (1 << 1))
				{
					hit_object_slider* lObject = static_cast<hit_object_slider*>(lHitObject);
					lFileStream << lObject->sX << "," << lObject->sY << "," << lObject->sTime << "," << static_cast<int>(lObject->sType) << "," << static_cast<int>(lObject->sHitSounds) << "," << lObject->sSliderType;
					for (const auto& lCurvePoint : lObject->sCurvePoints)
						lFileStream << "|" << lCurvePoint.first << ":" << lCurvePoint.second;
					lFileStream << "," << static_cast<int>(lObject->sRepeat) << "," << lObject->sPixelLength;
					
					if(!lObject->sEdgeHitsounds.empty())
					{
						lFileStream << ",";
						for (uint32_t i = 0; i < lObject->sEdgeHitsounds.size(); ++i)
						{
							lFileStream << static_cast<int>(lObject->sEdgeHitsounds[i]);
							if (i < lObject->sEdgeHitsounds.size() - 1)
								lFileStream << "|";
						}
					}
					if(!lObject->sEdgeAdditions.empty())
					{
						lFileStream << ",";
						for (uint32_t i = 0; i < lObject->sEdgeAdditions.size(); ++i)
						{
							lFileStream << static_cast<int>(lObject->sEdgeAdditions[i].first) << ":" << static_cast<int>(lObject->sEdgeAdditions[i].second);
							if (i < lObject->sEdgeAdditions.size() - 1)
								lFileStream << "|";
						}
					}
					if (lObject->sExtras.sSampleSet != -1)
					{
						lFileStream << "," << lObject->sExtras.sSampleSet << ":" << lObject->sExtras.sAdditionSet << ":" << lObject->sExtras.sCustomIndex << ":" << lObject->sExtras.sSampleVolume << ":" << lObject->sExtras.sFilename;
					}
				}
				else if (lHitObject->sType & (1 << 3))
				{
					hit_object_spinner* lObject = static_cast<hit_object_spinner*>(lHitObject);
					lFileStream << lObject->sX << "," << lObject->sY << "," << lObject->sTime << "," << static_cast<int>(lObject->sType) << "," << static_cast<int>(lObject->sHitSounds) << "," << lObject->sEndTime;
					if (lObject->sExtras.sSampleSet != -1)
					{
						lFileStream << "," << lObject->sExtras.sSampleSet << ":" << lObject->sExtras.sAdditionSet << ":" << lObject->sExtras.sCustomIndex << ":" << lObject->sExtras.sSampleVolume << ":" << lObject->sExtras.sFilename;
					}

				}
				lFileStream << std::endl;
			}

			lFileStream.close();
			return true;
		}

	}
}

#include "stdafx.h"
#include "file_writer.hpp"
#include "func.hpp"
#include "file_structs.hpp"

using namespace std;
namespace fs = std::experimental::filesystem;

namespace osu_tools
{
	namespace file_writer
	{
		bool write_file(const osu_file& aOsuFile)
		{
			auto lSongPath = osu_tools::func::get_beatmap_directory();
			lSongPath += aOsuFile.sFolderName;
			lSongPath /= aOsuFile.sFileName;

			// if file already exists, we don't want to recreate it
			if (fs::exists(lSongPath))
				return false;
			ofstream lFileStream;
			lFileStream.open(lSongPath.string(), ofstream::out | ofstream::app);
			//	fs::create(lSongPath);
			lFileStream << "osu file format v" << static_cast<int>(aOsuFile.sBeatmapVersion) << endl << endl;

			lFileStream << "[General]" << endl;
			if (!aOsuFile.sAudioFilename.empty())
				lFileStream << "AudioFilename:" << aOsuFile.sAudioFilename << endl;
			if(aOsuFile.sAudioLeadIn != -1)
				lFileStream << "AudioLeadIn:" << aOsuFile.sAudioLeadIn << endl;
			if (aOsuFile.sPreviewTime != -1)
				lFileStream << "PreviewTime:" << aOsuFile.sPreviewTime << endl;
			if(aOsuFile.sCountdown != -1)
				lFileStream << "Countdown:" << static_cast<int>(aOsuFile.sCountdown) << endl;
			if (!aOsuFile.sSampleSet.empty())
				lFileStream << "SampleSet:" << aOsuFile.sSampleSet << endl;
			if (aOsuFile.sStackLeniency != -1.0)
				lFileStream << "StackLeniency:" << aOsuFile.sStackLeniency << endl; // float x.x
			if (aOsuFile.sMode != -1)
				lFileStream << "Mode:" << static_cast<int>(aOsuFile.sMode) << endl; // uint8_t
			if(aOsuFile.sLetterboxInBreaks != -1)
				lFileStream << "LetterboxInBreaks:" << static_cast<int>(aOsuFile.sLetterboxInBreaks) << endl;
			if (!aOsuFile.sSkinPreference.empty())
				lFileStream << "SkinPreference:" << aOsuFile.sSkinPreference << endl;
			if(static_cast<int>(aOsuFile.sEpilepsyWarning) != -1)
				lFileStream << "EpilepsyWarning:" << static_cast<int>(aOsuFile.sEpilepsyWarning) << endl;
			if(aOsuFile.sWidescreenStoryboard != -1)
				lFileStream << "WidescreenStoryboard:" << static_cast<int>(aOsuFile.sWidescreenStoryboard) << endl;
			lFileStream << endl;

			lFileStream << "[Editor]" << endl;
			if(aOsuFile.sBookmarks.size() > 0)
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
				lFileStream << "DistanceSpacing:" << aOsuFile.sDistanceSpacing << endl;
			if(aOsuFile.sBeatDivisor != -1)
				lFileStream << "BeatDivisor:" << static_cast<int>(aOsuFile.sBeatDivisor) << endl;
			if(aOsuFile.sGridSize != -1)
				lFileStream << "GridSize:" << static_cast<int>(aOsuFile.sGridSize) << endl;
			if(aOsuFile.sTimelineZoom != -1.0)
				lFileStream << "TimelineZoom:" << aOsuFile.sTimelineZoom << endl; // float x.x
			lFileStream << endl;
			

			lFileStream << "[Metadata]" << endl;
			if(!aOsuFile.sTitle.empty())
				lFileStream << "Title:" << aOsuFile.sTitle << endl;
			if (!aOsuFile.sTitleUnicode.empty())
				lFileStream << "TitleUnicode:" << aOsuFile.sTitleUnicode << endl;
			if (!aOsuFile.sArtist.empty())
				lFileStream << "Artist:" << aOsuFile.sArtist << endl;
			if (!aOsuFile.sArtistUnicode.empty())
				lFileStream << "ArtistUnicode:" << aOsuFile.sArtistUnicode << endl;
			if (!aOsuFile.sCreator.empty())
				lFileStream << "Creator:" << aOsuFile.sCreator << endl;
			if (!aOsuFile.sVersion.empty())
				lFileStream << "Version:" << aOsuFile.sVersion << endl;
			if (!aOsuFile.sSource.empty())
				lFileStream << "Source:" << aOsuFile.sSource << endl;
			if (!aOsuFile.sTags.empty())
				lFileStream << "Tags:" << aOsuFile.sTags << endl;
			if(aOsuFile.sBeatmapID != -1)
				lFileStream << "BeatmapId:" << aOsuFile.sBeatmapID << endl;
			if(aOsuFile.sBeatmapSetID != -1)
				lFileStream << "BeatmapSetID:" << aOsuFile.sBeatmapSetID << endl << endl;
			lFileStream << endl;

			lFileStream << "[Difficulty]" << endl; // all float x.x
			if (aOsuFile.sHPDrainRate != -1.0)
				lFileStream << "HPDrainRate:" << aOsuFile.sHPDrainRate << endl;
			if (aOsuFile.sCircleSize != -1.0)
				lFileStream << "CircleSize:" << aOsuFile.sCircleSize << endl;
			if (aOsuFile.sOverallDifficulty != -1.0)
				lFileStream << "OverallDifficulty:" << aOsuFile.sOverallDifficulty << endl;
			if (aOsuFile.sApproachRate != -1.0)
				lFileStream << "ApproachRate:" << aOsuFile.sApproachRate << endl;
			if (aOsuFile.sSliderMultiplier != -1.0)
				lFileStream << "SliderMultiplier:" << aOsuFile.sSliderMultiplier << endl; // float x.xx
			if (aOsuFile.sSliderTickRate != -1.0)
				lFileStream << "SliderTickRate:" << aOsuFile.sSliderTickRate << endl << endl;

			lFileStream << "[Events]" << endl;
			lFileStream << "//Background and Video events" << endl;
			if (!aOsuFile.sBackground.empty())
			{
				lFileStream << "0,0,\"" << aOsuFile.sBackground << "\"";
				if (aOsuFile.sBeatmapVersion >= 10)
					lFileStream << ",0,0";
				lFileStream << endl;
			}
			lFileStream << "//Break Periods" << endl;
			for (const auto& lBreak : aOsuFile.sBreakPeriods)
				lFileStream << "2," << lBreak.first << "," << lBreak.second << endl;
			lFileStream << "//Storyboard Layer 0 (Background)" << endl;
			lFileStream << "//Storyboard Layer 1 (Fail)" << endl;
			lFileStream << "//Storyboard Layer 2 (Pass)" << endl;
			lFileStream << "//Storyboard Layer 3 (Foreground)" << endl;
			lFileStream << "//Storyboard Sound Samples" << endl;
			lFileStream << endl;

			lFileStream << "[TimingPoints]" << endl;
			for (const auto& lTimingPoint : aOsuFile.sTimingPoints)
				lFileStream << lTimingPoint.sOffset << "," << lTimingPoint.sMSPerBeat << "," << lTimingPoint.sMeter << "," << static_cast<int>(lTimingPoint.sSampleSet) << "," << static_cast<int>(lTimingPoint.sSampleIndex) << "," << static_cast<int>(lTimingPoint.sVolume) << "," << lTimingPoint.sInherited << "," << lTimingPoint.sKiai << endl;
			lFileStream << endl;

			lFileStream << "[Colours]" << endl;
			for (uint32_t i = 0; i < aOsuFile.sColours.size(); ++i)
				lFileStream << "Combo" << i + 1 << " : " << get<0>(aOsuFile.sColours[i]) << "," << get<1>(aOsuFile.sColours[i]) << "," << get<2>(aOsuFile.sColours[i]) << endl;
			lFileStream << endl;

			lFileStream << "[HitObjects]" << endl;
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
					
					if(lObject->sEdgeHitsounds.size() > 0)
					{
						lFileStream << ",";
						for (uint32_t i = 0; i < lObject->sEdgeHitsounds.size(); ++i)
						{
							lFileStream << static_cast<int>(lObject->sEdgeHitsounds[i]);
							if (i < lObject->sEdgeHitsounds.size() - 1)
								lFileStream << "|";
						}
					}
					if(lObject->sEdgeAdditions.size() > 0)
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
				lFileStream << endl;
			}

			lFileStream.close();
			return true;
		}

	}
}

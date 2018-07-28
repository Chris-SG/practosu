#include "stdafx.h"
#include "file_writer.hpp"
#include "func.hpp"
#include "file_structs.hpp"

using namespace std;
namespace fs = std::experimental::filesystem;

namespace OsuFileWriter
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
		lFileStream << "osu file format v" << aOsuFile.sVersion << endl << endl;

		lFileStream << "[General]" << endl;
		lFileStream << "AudioFilename: " << aOsuFile.sAudioFilename << endl;
		lFileStream << "AudioLeadIn: " << aOsuFile.sAudioLeadIn << endl;
		lFileStream << "PreviewTime: " << aOsuFile.sPreviewTime << endl;
		lFileStream << "Countdown: " << aOsuFile.sCountdown << endl;
		lFileStream << "SampleSet: " << aOsuFile.sSampleSet << endl;
		lFileStream << "StackLeniency: " << aOsuFile.sStackLeniency << endl; // float x.x
		lFileStream << "Mode: " << aOsuFile.sMode << endl; // uint8_t
		lFileStream << "LetterboxInBreaks: " << aOsuFile.sLetterboxInBreaks << endl;
		lFileStream << "EpilepsyWarning: " << aOsuFile.sEpilepsyWarning << endl;
		lFileStream << "WidescreenStoryboard: " << aOsuFile.sWidescreenStoryboard << endl << endl;

		lFileStream << "[Editor]" << endl;
		lFileStream << "Bookmarks: ";
		for (uint32_t i = 0; i < aOsuFile.sBookmarks.size(); ++i)
		{
			lFileStream << aOsuFile.sBookmarks[i];
			if (i != aOsuFile.sBookmarks.size() - 1)
				lFileStream << ",";
		}
		lFileStream << "DistanceSpacing: " << aOsuFile.sDistanceSpacing << endl;
		lFileStream << "BeatDivisor: " << aOsuFile.sBeatDivisor << endl;
		lFileStream << "GridSize: " << aOsuFile.sGridSize << endl;
		lFileStream << "TimelineZoom: " << aOsuFile.sTimelineZoom << endl << endl; // float x.x

		lFileStream << "[Metadata]" << endl;
		lFileStream << "Title:" << aOsuFile.sTitle << endl;
		lFileStream << "TitleUnicode:" << aOsuFile.sTitleUnicode << endl;
		lFileStream << "Artist:" << aOsuFile.sArtist << endl;
		lFileStream << "ArtistUnicode:" << aOsuFile.sArtistUnicode << endl;
		lFileStream << "Version:" << aOsuFile.sVersion << endl;
		lFileStream << "Source:" << aOsuFile.sSource << endl;
		lFileStream << "Tags:" << aOsuFile.sTags << endl;
		lFileStream << "BeatmapId:" << aOsuFile.sBeatmapID << endl;
		lFileStream << "BeatmapSetID:" << aOsuFile.sBeatmapSetID << endl << endl;

		lFileStream << "[Difficulty]" << endl; // all float x.x
		lFileStream << "HPDrainRate:" << aOsuFile.sHPDrainRate << endl;
		lFileStream << "CircleSize:" << aOsuFile.sCircleSize << endl;
		lFileStream << "OverallDifficulty:" << aOsuFile.sOverallDifficulty << endl;
		lFileStream << "ApproachRate:" << aOsuFile.sApproachRate << endl;
		lFileStream << "SliderMultiplier:" << aOsuFile.sSliderMultiplier << endl; // float x.xx
		lFileStream << "SliderTickRate:" << aOsuFile.sSliderTickRate << endl;

		lFileStream << "[Events]" << endl;
		lFileStream << "//Background and Video events" << endl;
		lFileStream << "0,0\"" << aOsuFile.sBackground << "\",0,0" << endl;
		lFileStream << "//Break Periods" << endl;
		for(const auto& lBreak : aOsuFile.sBreakPeriods)
			lFileStream << "2," << lBreak.first << "," << lBreak.second << endl;
		lFileStream << "//Storyboard Layer 0 (Background)" << endl;
		lFileStream << "//Storyboard Layer 1 (Fail)" << endl;
		lFileStream << "//Storyboard Layer 2 (Pass)" << endl;
		lFileStream << "//Storyboard Layer 3 (Foreground)" << endl;
		lFileStream << "//Storyboard Sound Samples" << endl;

		lFileStream << "[TimingPoints]" << endl;
		for(const auto& lTimingPoint : aOsuFile.sTimingPoints)
			lFileStream << lTimingPoint.sOffset << "," << lTimingPoint.sMSPerBeat << "," << lTimingPoint.sMeter << "," << lTimingPoint.sSampleSet << "," << lTimingPoint.sSampleIndex << "," << lTimingPoint.sVolume << "," << lTimingPoint.sInherited << "," << lTimingPoint.sKiai << endl;
		lFileStream << endl;

		lFileStream << "[Colours]" << endl;
		for (uint32_t i = 0; i < aOsuFile.sColours.size(); ++i)
			lFileStream << "Combo" << i + 1 << " : " << get<0>(aOsuFile.sColours[i]) << "," << get<1>(aOsuFile.sColours[i]) << "," << get<2>(aOsuFile.sColours[i]) << endl;
		lFileStream << endl;

		lFileStream << "HitObjects" << endl;
		for(const auto& lHitObject : aOsuFile.sHitObjects)
		{
			if (lHitObject->sType & (1 << 0))
			{
				hit_object_circle* lObject = static_cast<hit_object_circle*>(lHitObject);
				lFileStream << lObject->sX << "," << lObject->sY << "," << lObject->sTime << "," << lObject->sType << "," << lObject->sHitSounds << ",0:0:0:0:";
			}
			else if (lHitObject->sType & (1 << 1))
			{
				hit_object_slider* lObject = static_cast<hit_object_slider*>(lHitObject);
				lFileStream << lObject->sX << "," << lObject->sY << "," << lObject->sTime << "," << lObject->sType << "," << lObject->sHitSounds << "," << lObject->sSliderType;
				for(const auto& lCurvePoint : lObject->sCurvePoints)
					lFileStream << "|" << lCurvePoint.first << ":" << lCurvePoint.second;
				lFileStream << "," << lObject->sRepeat << "," << lObject->sPixelLength << ",";
				for (uint32_t i = 0; i < lObject->sEdgeHitsounds.size(); ++i)
				{
					lFileStream << lObject->sEdgeHitsounds[i];
					if (i < lObject->sEdgeHitsounds.size() - 1)
						lFileStream << "|";
				}
				lFileStream << ",";
				for (uint32_t i = 0; i < lObject->sEdgeAdditions.size(); ++i)
				{
					lFileStream << lObject->sEdgeAdditions[i].first << ":" << lObject->sEdgeAdditions[i].second;
					if (i < lObject->sEdgeAdditions.size() - 1)
						lFileStream << "|";
				}
				lFileStream << ",0:0:0:0:";
			}
			else if (lHitObject->sType & (1 << 3))
			{
				hit_object_spinner* lObject = static_cast<hit_object_spinner*>(lHitObject);
				lFileStream << lObject->sX << "," << lObject->sY << "," << lObject->sTime << "," << lObject->sType << "," << lObject->sHitSounds << "," << lObject->sEndTime << ",0:0:0:0:";
				
			}
			lFileStream << endl;
		}

		lFileStream << endl;

		lFileStream.close();
		return true;
	}

}
#pragma once

#include "stdafx.h"

struct beatmap
{
	uint32_t sEntrySz;
	std::string sArtist;
	std::string sArtistUnicode;
	std::string sTitle;
	std::string sTitleUnicode;
	std::string sCreator;
	std::string sDifficulty;
	std::string sAudioName;
	std::string sMD5;
	std::string sOsuFilename;
	uint8_t sRankedStatus;
	uint16_t sNumHitcircles;
	uint16_t sNumSliders;
	uint16_t sNumSpinners;
	uint64_t sLastModification;
	uint8_t sAR;
	uint8_t sCS;
	uint8_t sHP;
	uint8_t sOD;
	float sARFloat; //20140609 or later
	float sCSFloat; //20140609 or later
	float sHPFloat; //20140609 or later
	float sODFloat; //20140609 or later
	double sSliderVelocity;
	uint32_t sRatingsStd; //20140609 or later
	std::vector<std::pair<uint32_t, double>> sStarRatingStd; //20140609 or later
	uint32_t sRatingsTaiko; //20140609 or later
	std::vector<std::pair<uint32_t, double>> sStarRatingTaiko; //20140609 or later
	uint32_t sRatingsCTB; //20140609 or later
	std::vector<std::pair<uint32_t, double>> sStarRatingCTB; //20140609 or later
	uint32_t sRatingsMania; //20140609 or later
	std::vector<std::pair<uint32_t, double>> sStarRatingMania; //20140609 or later
	uint32_t sDraintime;
	uint32_t sTotalTime;
	uint32_t sPreviewTime;
	uint32_t sTimingPointCount;
	std::vector<std::tuple<double, double, bool>> sTimingPoints;
	uint32_t sBeatmapID;
	uint32_t sBeatmapSetID;
	uint32_t sThreadID;
	uint8_t sGradeStd;
	uint8_t sGradeTaiko;
	uint8_t sGradeCTB;
	uint8_t sGradeMania;
	uint16_t sLocalOffset;
	float sStackLeniency;
	uint8_t sGameplayMode;
	std::string sSource;
	std::string sTags;
	uint16_t sOnlineOffset;
	std::string sFont;
	bool sPlayed;
	uint64_t sLastPlaytime;
	bool sIsOSZ2;
	std::string sFolderName;
	uint64_t sLastCheckedOnline;
	bool sIgnoreBeatmapSound;
	bool sIgnoreBeatmapSkin;
	bool sDisableSB;
	bool sDisableVideo;
	bool sVisualOverride;
	uint16_t sUnknown; // only prior to 20140609
	uint32_t sLastModificationTime;
	uint8_t sManiaScrollSpeed;
};

struct timing_point
{
	uint32_t sOffset;
	float sMSPerBeat;
	uint32_t sMeter;
	uint8_t sSampleSet;
	uint8_t sSampleIndex;
	uint8_t sVolume;
	bool sInherited;
	bool sKiai;
};

struct hit_object
{
	uint16_t sX;
	uint16_t sY;
	uint32_t sTime;
	uint8_t sType;
	uint8_t sHitSounds;
	// other data goes here
	// todo later extra 0:0:0:0:
};

struct hit_object_circle : hit_object
{

};

struct hit_object_slider : hit_object
{
	char sSliderType;
	std::vector<std::pair<uint32_t, uint32_t>> sCurvePoints; // L uses 1, P uses 2, B uses many
	uint8_t sRepeat;
	float sPixelLength;
	std::vector<uint8_t> sEdgeHitsounds;
	std::vector<std::pair<uint8_t, uint8_t>> sEdgeAdditions;
};

struct hit_object_spinner : hit_object
{
	uint32_t sEndTime;
};

struct osu_file
{
	std::string sFolderName;
	std::string sFileName;

	uint8_t sBeatmapVersion;
	// general
	std::string sAudioFilename;
	uint32_t sAudioLeadIn;
	uint32_t sPreviewTime;
	bool sCountdown;
	std::string sSampleSet;
	float sStackLeniency;
	uint8_t sMode;
	bool sLetterboxInBreaks;
	bool sEpilepsyWarning;
	bool sWidescreenStoryboard;
	// editor
	std::vector<int> sBookmarks;
	float sDistanceSpacing;
	uint8_t sBeatDivisor;
	uint8_t sGridSize;
	float sTimelineZoom;
	// metadata
	std::string sTitle;
	std::string sTitleUnicode;
	std::string sArtist;
	std::string sArtistUnicode;
	std::string sCreator;
	std::string sVersion;
	std::string sSource;
	std::string sTags;
	uint32_t sBeatmapID;
	uint32_t sBeatmapSetID;
	// difficulty
	float sHPDrainRate;
	float sCircleSize;
	float sOverallDifficulty;
	float sApproachRate;
	float sSliderMultiplier;
	float sSliderTickRate;
	// events
	std::string sBackground;
	std::string sVideo;
	std::vector<std::pair<uint32_t, uint32_t>> sBreakPeriods;
	// TODO layer 0/1/2/3/sounds
	// timing points
	std::vector<timing_point> sTimingPoints;
	// colors
	std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> sColours;
	// hitobjects
	std::vector<hit_object*> sHitObjects;
};

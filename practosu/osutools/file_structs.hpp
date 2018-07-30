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
	int32_t sOffset;
	double sMSPerBeat;
	uint32_t sMeter;
	uint8_t sSampleSet;
	uint8_t sSampleIndex;
	uint8_t sVolume;
	bool sInherited;
	bool sKiai;
};

struct hit_object_extras
{
	uint32_t sSampleSet = -1;
	uint32_t sAdditionSet = -1;
	uint32_t sCustomIndex = -1;
	uint32_t sSampleVolume = -1;
	std::string sFilename;
};

struct hit_object
{
	uint16_t sX;
	uint16_t sY;
	uint32_t sTime;
	uint8_t sType;
	uint8_t sHitSounds;
	// other data goes here
	hit_object_extras sExtras;
};

struct hit_object_circle : hit_object
{

};

struct hit_object_slider : hit_object
{
	char sSliderType;
	std::vector<std::pair<int32_t, int32_t>> sCurvePoints; // L uses 1, P uses 2, B uses many
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

	uint8_t sBeatmapVersion = -1;
	// general
	std::string sAudioFilename;
	uint32_t sAudioLeadIn = -1;
	int32_t sPreviewTime = -1;
	int8_t sCountdown = -1;
	std::string sSampleSet;
	float sStackLeniency = -1.0;
	int8_t sMode = -1;
	int8_t sLetterboxInBreaks = -1;
	int8_t sEpilepsyWarning = -1;
	int8_t sWidescreenStoryboard = -1;
	std::string sSkinPreference;
	// editor
	std::vector<int> sBookmarks;
	float sDistanceSpacing = -1.0;
	uint8_t sBeatDivisor = -1;
	uint8_t sGridSize = -1;
	float sTimelineZoom = -1.0;
	// metadata
	std::string sTitle;
	std::string sTitleUnicode;
	std::string sArtist;
	std::string sArtistUnicode;
	std::string sCreator;
	std::string sVersion;
	std::string sSource;
	std::string sTags;
	uint32_t sBeatmapID = -1;
	uint32_t sBeatmapSetID = -1;
	// difficulty
	float sHPDrainRate = -1.0;
	float sCircleSize = -1.0;
	float sOverallDifficulty = -1.0;
	float sApproachRate = -1.0;
	float sSliderMultiplier = -1.0;
	float sSliderTickRate = -1.0;
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

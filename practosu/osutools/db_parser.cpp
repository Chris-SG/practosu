
#include "stdafx.h"
#include "db_parser.hpp"
#include "file_structs.hpp"

using namespace std;
namespace fs = std::experimental::filesystem;

namespace osu_tools
{
	namespace db_parser
	{
		void parse_osu_db(fs::path &pFilePath)
		{
			// confirm file path exists and db is not cached
			if (!aDbCached && fs::exists(pFilePath))
			{
				// open file as binary
				ifstream lFile(pFilePath.string(), ios::binary);
				// check file is good
				if (lFile.good())
				{
					// parse general information
					aCachedDb.sOsuVersion = get_int(lFile);
					aCachedDb.sFolderCount = get_int(lFile);
					aCachedDb.sAccountUnlocked = get_bool(lFile);
					aCachedDb.sUnlockTime = get_long_long(lFile);
					aCachedDb.sPlayerName = get_string(lFile);
					aCachedDb.sBeatmapCount = get_int(lFile);
					aCachedDb.sFileDetails = vector<tuple<uint32_t, string, string>>();
					//lDb.sBeatmaps = vector<beatmap>();
					// iterate over beatmap entries
					for (uint32_t i = 0; i < aCachedDb.sBeatmapCount; ++i)
					{
						if(i % 1000 == 0)
							std::cout << "Done " << i << " / " << aCachedDb.sBeatmapCount << endl;
						// parse beatmap information
						beatmap lBeatmap;
						lBeatmap.sEntrySz = get_int(lFile);
						lBeatmap.sArtist = get_string(lFile);
						lBeatmap.sArtistUnicode = get_string(lFile);
						lBeatmap.sTitle = get_string(lFile);
						lBeatmap.sTitleUnicode = get_string(lFile);
						lBeatmap.sCreator = get_string(lFile);
						lBeatmap.sDifficulty = get_string(lFile);
						lBeatmap.sAudioName = get_string(lFile);
						lBeatmap.sMD5 = get_string(lFile);
						lBeatmap.sOsuFilename = get_string(lFile);
						lBeatmap.sRankedStatus = get_char(lFile);
						lBeatmap.sNumHitcircles = get_short(lFile);
						lBeatmap.sNumSliders = get_short(lFile);
						lBeatmap.sNumSpinners = get_short(lFile);
						lBeatmap.sLastModification = get_long_long(lFile);
//						if (lBeatmap.sLastModification >= uint64_t(635378688000000000))
						if(1)
						{
							lBeatmap.sARFloat = get_float(lFile);
							lBeatmap.sCSFloat = get_float(lFile);
							lBeatmap.sHPFloat = get_float(lFile);
							lBeatmap.sODFloat = get_float(lFile);

						}
						else
						{
							lBeatmap.sAR = get_char(lFile);
							lBeatmap.sCS = get_char(lFile);
							lBeatmap.sHP = get_char(lFile);
							lBeatmap.sOD = get_char(lFile);
						}
						lBeatmap.sSliderVelocity = get_double(lFile);
						//if (lBeatmap.sLastModification >= uint64_t(635378688000000000))
						if(1)
						{
							lBeatmap.sRatingsStd = get_int(lFile);
							for (uint32_t j = 0; j < lBeatmap.sRatingsStd; ++j)
								lBeatmap.sStarRatingStd.push_back(get_int_double_pair(lFile));
							lBeatmap.sRatingsTaiko = get_int(lFile);
							for (uint32_t j = 0; j < lBeatmap.sRatingsTaiko; ++j)
								lBeatmap.sStarRatingTaiko.push_back(get_int_double_pair(lFile));
							lBeatmap.sRatingsCTB = get_int(lFile);
							for (uint32_t j = 0; j < lBeatmap.sRatingsCTB; ++j)
								lBeatmap.sStarRatingCTB.push_back(get_int_double_pair(lFile));
							lBeatmap.sRatingsMania = get_int(lFile);
							for (uint32_t j = 0; j < lBeatmap.sRatingsMania; ++j)
								lBeatmap.sStarRatingMania.push_back(get_int_double_pair(lFile));
						}
						lBeatmap.sDraintime = get_int(lFile);
						lBeatmap.sTotalTime = get_int(lFile);
						lBeatmap.sPreviewTime = get_int(lFile);
						lBeatmap.sTimingPointCount = get_int(lFile);
						for (uint32_t j = 0; j < lBeatmap.sTimingPointCount; ++j)
							lBeatmap.sTimingPoints.push_back(std::tuple<double, double, bool>(get_double(lFile), get_double(lFile), get_bool(lFile)));
						lBeatmap.sBeatmapID = get_int(lFile);
						lBeatmap.sBeatmapSetID = get_int(lFile);
						lBeatmap.sThreadID = get_int(lFile);
						lBeatmap.sGradeStd = get_char(lFile);
						lBeatmap.sGradeTaiko = get_char(lFile);
						lBeatmap.sGradeCTB = get_char(lFile);
						lBeatmap.sGradeMania = get_char(lFile);
						lBeatmap.sLocalOffset = get_short(lFile);
						lBeatmap.sStackLeniency = get_float(lFile);
						lBeatmap.sGameplayMode = get_char(lFile);
						lBeatmap.sSource = get_string(lFile);
						lBeatmap.sTags = get_string(lFile);
						lBeatmap.sOnlineOffset = get_short(lFile);
						lBeatmap.sFont = get_string(lFile);
						lBeatmap.sPlayed = get_bool(lFile);
						lBeatmap.sLastPlaytime = get_long_long(lFile);
						lBeatmap.sIsOSZ2 = get_bool(lFile);
						lBeatmap.sFolderName = get_string(lFile);
						lBeatmap.sLastCheckedOnline = get_long_long(lFile);
						lBeatmap.sIgnoreBeatmapSound = get_bool(lFile);
						lBeatmap.sIgnoreBeatmapSkin = get_bool(lFile);
						lBeatmap.sDisableSB = get_bool(lFile);
						lBeatmap.sDisableVideo = get_bool(lFile);
						lBeatmap.sVisualOverride = get_bool(lFile);
						//if (lBeatmap.sLastModification < uint64_t(635378688000000000))
						if(0)
							lBeatmap.sUnknown = get_short(lFile); // only prior to 20140609
						lBeatmap.sLastModificationTime = get_int(lFile);
						lBeatmap.sManiaScrollSpeed = get_char(lFile);

						//lDb.sBeatmaps.push_back(lBeatmap);
						aCachedDb.sFileDetails.push_back(std::tuple<uint32_t, string, string>(lBeatmap.sBeatmapID, lBeatmap.sFolderName, lBeatmap.sOsuFilename));
					}

					lFile.close();
					aDbCached = true;
				}
			}
		}

		float get_float(std::ifstream &pFs)
		{
			float lRet;
			pFs.read(reinterpret_cast<char*>(&lRet), sizeof(lRet));
			return lRet;
		}

		double get_double(std::ifstream &pFs)
		{
			double lRet;
			pFs.read(reinterpret_cast<char*>(&lRet), sizeof(lRet));
			return lRet;
		}

		uint8_t get_char(std::ifstream &pFs)
		{
			uint8_t lRet;
			pFs.read(reinterpret_cast<char*>(&lRet), sizeof(lRet));
			return lRet;
		}

		uint16_t get_short(std::ifstream &pFs)
		{
			uint16_t lRet;
			pFs.read(reinterpret_cast<char*>(&lRet), sizeof(lRet));
			return lRet;
		}

		uint32_t get_int(std::ifstream &pFs)
		{
			uint32_t lRet;
			pFs.read(reinterpret_cast<char*>(&lRet), sizeof(lRet));
			return lRet;
		}

		uint64_t get_long_long(std::ifstream &pFs)
		{
			uint64_t lRet;
			pFs.read(reinterpret_cast<char*>(&lRet), sizeof(lRet));
			return lRet;
		}

		bool get_bool(std::ifstream &pFs)
		{

			return (get_char(pFs) ? true : false);
		}

		std::string get_string(std::ifstream &pFs)
		{
			// all strings begin with 0x00 if empty, or 0x0b if otherwise
			std::string lRet;
			if (pFs.get() != '\x0b')
				return lRet;
			uint32_t lLength = 0;
			uint32_t lShift = 0;
			unsigned char lChar;

			// convert from ULEB128 to uint32_t
			while (lChar = pFs.get())
			{
				lLength |= ((lChar & 0x7F) << lShift);
				if ((lChar & (1 << 7)) == 0)
					break;
				lShift += 7;
			}
			// if length is greater than zero, read required characters to string
			if (lLength > 0)
			{
				lRet.resize(lLength);
				pFs.read(&lRet[0], lLength);
			}
			return lRet;
		}

		std::pair<int, double> get_int_double_pair(std::ifstream &pFs)
		{
			// int-double pair contains 0x08 int 0x0d double
			pFs.get();
			int lInt = get_int(pFs);
			pFs.get();
			double lDbl = get_double(pFs);
			return std::pair<int, double>(lInt, lDbl);
		}

	}
}

#include "stdafx.h"
#include "file_parser.hpp"
#include "file_structs.hpp"

#include <sstream>

using namespace std;
namespace fs = std::experimental::filesystem;

namespace osu_tools
{
	namespace file_parser
	{
		osu_file parse_osu_file(std::experimental::filesystem::path &pFilePath)
		{
			// create osu file
			osu_file lOsuFile;
			// confirm file exists
			if (fs::exists(pFilePath))
			{
				// open file
				ifstream lFile(pFilePath.string());
				if (lFile.good())
				{
					std::string lFilePath = pFilePath.string();
					uint32_t lPos = lFilePath.rfind("\\");
					lOsuFile.sFileName = lFilePath.substr(lPos + 1, lFilePath.size());
					lFilePath.erase(lPos, lFilePath.size());
					lPos = lFilePath.rfind("\\");
					lOsuFile.sFolderName = lFilePath.substr(lPos + 1, lFilePath.size());

					// first line is the file format // can files begin with an empty line?
					std::string lLine;
					getline(lFile, lLine);
					size_t lFound = lLine.find("osu file format v");
					if (lFound == string::npos)
					{
						// if no version, close file and return
						lFile.close();
						return lOsuFile;
					}

					// add file version to struct
					lLine.erase(0, lFound + 17);
					int sVersion = stoi(lLine);
					lOsuFile.sBeatmapVersion = static_cast<uint8_t>(sVersion);
					// go through all different sections
					while (!lFile.eof() && getline(lFile, lLine))
					{
						if (lLine.find("[General]") != string::npos)
							read_file_general(lFile, lOsuFile);
						else if (lLine.find("[Editor]") != string::npos)
							read_file_editor(lFile, lOsuFile);
						else if (lLine.find("[Metadata]") != string::npos)
							read_file_metadata(lFile, lOsuFile);
						else if (lLine.find("[Difficulty]") != string::npos)
							read_file_difficulty(lFile, lOsuFile);
						else if (lLine.find("[Events]") != string::npos)
							read_file_events(lFile, lOsuFile);
						else if (lLine.find("[TimingPoints]") != string::npos)
							read_file_timingpoints(lFile, lOsuFile);
						else if (lLine.find("[Colours]") != string::npos)
							read_file_colours(lFile, lOsuFile);
						else if (lLine.find("[HitObjects]") != string::npos)
							read_file_hitobjects(lFile, lOsuFile);
					}
					lFile.close();
				}
			}

			return lOsuFile;
		}

		void read_file_general(std::ifstream &lFs, osu_file &lOsuFile)
		{
			string lLine;
			size_t lPos;
			string lDilimiter = ":";
			string lToken;
			while (getline(lFs, lLine))
			{
				if (lLine.length() == 0)
					return;
				lPos = lLine.find(lDilimiter);
				lToken = lLine.substr(0, lPos);
				lLine.erase(0, lPos + lDilimiter.length());
				if (lToken == "AudioFilename")
					lOsuFile.sAudioFilename = lLine;
				else if (lToken == "SampleSet")
					lOsuFile.sSampleSet = lLine;
				else if (lToken == "StackLeniency")
					lOsuFile.sStackLeniency = stof(lLine);
				else if (lToken == "SkinPreference")
					lOsuFile.sSkinPreference = lLine;
				else
				{
					int sInt = stoi(lLine);
					if (lToken == "AudioLeadIn")
						lOsuFile.sAudioLeadIn = sInt;
					else if (lToken == "PreviewTime")
						lOsuFile.sPreviewTime = sInt;
					else if (lToken == "Mode")
						lOsuFile.sMode = static_cast<uint8_t>(sInt);
					else if (lToken == "Countdown")
						lOsuFile.sCountdown = static_cast<uint8_t>(sInt);
					else if (lToken == "LetterboxInBreaks")
						lOsuFile.sLetterboxInBreaks = static_cast<uint8_t>(sInt);
					else if (lToken == "WidescreenStoryboard")
						lOsuFile.sWidescreenStoryboard = static_cast<uint8_t>(sInt);
					else if (lToken == "EpilepsyWarning")
						lOsuFile.sEpilepsyWarning = static_cast<uint8_t>(sInt);
				}
			}
		}

		void read_file_editor(std::ifstream &lFs, osu_file &lOsuFile)
		{
			string lLine;
			size_t lPos;
			string lDilimiter = ":";
			string lToken;
			while (getline(lFs, lLine))
			{
				if (lLine.length() == 0)
					return;
				lPos = lLine.find(lDilimiter);
				lToken = lLine.substr(0, lPos);
				lLine.erase(0, lPos + lDilimiter.length());

				if (lToken == "Bookmarks")
				{
					stringstream lSs(lLine);
					int lNum;
					while (lSs >> lNum)
					{
						lOsuFile.sBookmarks.push_back(lNum);
						if (lSs.peek() == ',')
							lSs.ignore();
					}
				}
				else if (lToken == "DistanceSpacing")
					lOsuFile.sDistanceSpacing = stof(lLine);
				else if (lToken == "TimelineZoom")
					lOsuFile.sTimelineZoom = stof(lLine);
				else
				{
					int sInt = stoi(lLine);
					if (lToken == "BeatDivisor")
						lOsuFile.sBeatDivisor = static_cast<uint8_t>(sInt);
					else if (lToken == "GridSize")
						lOsuFile.sGridSize = static_cast<uint8_t>(sInt);
				}
			}
		}

		void read_file_metadata(std::ifstream &lFs, osu_file &lOsuFile)
		{
			string lLine;
			size_t lPos;
			string lDilimiter = ":";
			string lToken;
			while (getline(lFs, lLine))
			{
				if (lLine.length() == 0)
					return;
				lPos = lLine.find(lDilimiter);
				lToken = lLine.substr(0, lPos);
				lLine.erase(0, lPos + lDilimiter.length());
				if (lToken == "Title")
					lOsuFile.sTitle = lLine;
				else if (lToken == "TitleUnicode")
					lOsuFile.sTitleUnicode = lLine;
				else if (lToken == "Artist")
					lOsuFile.sArtist = lLine;
				else if (lToken == "ArtistUnicode")
					lOsuFile.sArtistUnicode = lLine;
				else if (lToken == "Creator")
					lOsuFile.sCreator = lLine;
				else if (lToken == "Version")
					lOsuFile.sVersion = lLine;
				else if (lToken == "Source")
					lOsuFile.sSource = lLine;
				else if (lToken == "Tags")
					lOsuFile.sTags = lLine;
				else if (lToken == "BeatmapID")
					lOsuFile.sBeatmapID = stoi(lLine);
				else if (lToken == "BeatmapSetID")
					lOsuFile.sBeatmapSetID = stoi(lLine);
			}
		}

		void read_file_difficulty(std::ifstream &lFs, osu_file &lOsuFile)
		{
			string lLine;
			size_t lPos;
			string lDilimiter = ":";
			string lToken;
			while (getline(lFs, lLine))
			{
				if (lLine.length() == 0)
					return;
				lPos = lLine.find(lDilimiter);
				lToken = lLine.substr(0, lPos);
				lLine.erase(0, lPos + lDilimiter.length());
				if (lToken == "HPDrainRate")
					lOsuFile.sHPDrainRate = stof(lLine);
				else if (lToken == "CircleSize")
					lOsuFile.sCircleSize = stof(lLine);
				else if (lToken == "OverallDifficulty")
					lOsuFile.sOverallDifficulty = stof(lLine);
				else if (lToken == "ApproachRate")
					lOsuFile.sApproachRate = stof(lLine);
				else if (lToken == "SliderMultiplier")
					lOsuFile.sSliderMultiplier = stof(lLine);
				else if (lToken == "SliderTickRate")
					lOsuFile.sSliderTickRate = stof(lLine);
			}
		}
		void read_file_events(std::ifstream &lFs, osu_file &lOsuFile)
		{
			string lLine;
			while (getline(lFs, lLine))
			{
				if (lLine.length() == 0)
					return;

				stringstream lSs(lLine);
				uint32_t lTemp;
				lSs >> lTemp;
				if (lTemp == 0)
				{
					lSs.ignore();
					lSs >> lTemp;
					lSs.ignore();
					// get lss between quotations
				}
				else if (lTemp == 2)
				{
					lSs.ignore();
					int lTemp2;
					lSs >> lTemp;
					lSs.ignore();
					lSs >> lTemp2;
					lOsuFile.sBreakPeriods.push_back(pair<uint32_t, uint32_t>(lTemp, lTemp2));
				}

			}
		}
		void read_file_timingpoints(std::ifstream &lFs, osu_file &lOsuFile)
		{
			string lLine;
			while (getline(lFs, lLine))
			{
				if (lLine.length() == 0)
					return;

				stringstream lSs(lLine);
				timing_point lTP;
				uint32_t lTemp;
				lSs >> lTP.sOffset;
				lSs.ignore();
				lSs >> lTP.sMSPerBeat;
				lSs.ignore();
				lSs >> lTP.sMeter;
				lSs.ignore();
				lSs >> lTemp;
				lTP.sSampleSet = static_cast<uint8_t>(lTemp);
				lSs.ignore();
				lSs >> lTemp;
				lTP.sSampleIndex = static_cast<uint8_t>(lTemp);
				lSs.ignore();
				lSs >> lTemp;
				lTP.sVolume = static_cast<uint8_t>(lTemp);
				lSs.ignore();
				lSs >> lTP.sInherited;
				lSs.ignore();
				lSs >> lTP.sKiai;

				lOsuFile.sTimingPoints.push_back(lTP);
			}

		}
		void read_file_colours(std::ifstream &lFs, osu_file &lOsuFile)
		{
			string lLine;
			size_t lPos;
			string lDilimiter = ":";
			string lToken;
			while (getline(lFs, lLine))
			{
				if (lLine.length() == 0)
					return;
				lPos = lLine.find(lDilimiter);
				lToken = lLine.substr(0, lPos);
				lLine.erase(0, lPos + lDilimiter.length());

				stringstream lSs(lLine);
				int lNum1, lNum2, lNum3;
				lSs >> lNum1;
				lSs.ignore();
				lSs >> lNum2;
				lSs.ignore();
				lSs >> lNum3;
				lOsuFile.sColours.push_back(tuple<int, int, int>(lNum1, lNum2, lNum3));
			}
		}
		void read_file_hitobjects(std::ifstream &lFs, osu_file &lOsuFile)
		{
			// hit objects are done as pointers, and can later be reaccessed using a static_cast
			string lLine;
			while (getline(lFs, lLine))
			{
				if (lLine.length() == 0)
					return;
				stringstream lSs(lLine);
				uint16_t lX, lY;
				uint32_t lTime, lTemp;
				uint8_t lType, lHitSounds;
				lSs >> lX;
				lSs.ignore();
				lSs >> lY;
				lSs.ignore();
				lSs >> lTime;
				lSs.ignore();
				lSs >> lTemp;
				lType = static_cast<uint8_t>(lTemp);
				lSs.ignore();
				lSs >> lTemp;
				lHitSounds = static_cast<uint8_t>(lTemp);
				if (lType & (1 << 0))
				{
					hit_object_circle* lObject = new hit_object_circle;
					lObject->sX = lX;
					lObject->sY = lY;
					lObject->sTime = lTime;
					lObject->sType = lType;
					lObject->sHitSounds = lHitSounds;

					if(lSs.peek() == ',')
					{
						lSs.ignore();
						lSs >> lObject->sExtras.sSampleSet;
						lSs.ignore();
						lSs >> lObject->sExtras.sAdditionSet;
						lSs.ignore();
						lSs >> lObject->sExtras.sCustomIndex;
						lSs.ignore();
						lSs >> lObject->sExtras.sSampleVolume;
						lSs.ignore();
						lSs >> lObject->sExtras.sFilename;
					}

					lOsuFile.sHitObjects.push_back(lObject);
				}
				else if (lType & (1 << 1))
				{
					hit_object_slider* lObject = new hit_object_slider;
					lObject->sX = lX;
					lObject->sY = lY;
					lObject->sTime = lTime;
					lObject->sType = lType;
					lObject->sHitSounds = lHitSounds;

					lSs.ignore();
					lSs >> lObject->sSliderType;
					while (lSs.peek() == '|')
					{
						lSs.ignore();
						uint32_t lFirst, lSecond;
						lSs >> lFirst;
						lSs.ignore();
						lSs >> lSecond;
						lObject->sCurvePoints.push_back(pair<uint32_t, uint32_t>(lFirst, lSecond));
					}
					lSs.ignore();
					lSs >> lTemp;
					lObject->sRepeat = static_cast<uint8_t>(lTemp);
					lSs.ignore();
					lSs >> lObject->sPixelLength;
					if(lSs.peek() == ',')
					{
						do
						{
							lSs.ignore();
							lSs >> lTemp;
							lObject->sEdgeHitsounds.push_back(static_cast<uint8_t>(lTemp));
						} while (lSs.peek() == '|');
					}
					if(lSs.peek() == ',')
					{
						do
						{
							lSs.ignore();
							uint8_t lTemp_u8;
							lSs >> lTemp;
							lTemp_u8 = static_cast<uint8_t>(lTemp);
							lSs.ignore();
							lSs >> lTemp;
							lObject->sEdgeAdditions.push_back(pair<uint8_t, uint8_t>(lTemp_u8, static_cast<uint8_t>(lTemp)));
						} while (lSs.peek() == '|');
					}

					if (lSs.peek() == ',')
					{
						lSs.ignore();
						lSs >> lObject->sExtras.sSampleSet;
						lSs.ignore();
						lSs >> lObject->sExtras.sAdditionSet;
						lSs.ignore();
						lSs >> lObject->sExtras.sCustomIndex;
						lSs.ignore();
						lSs >> lObject->sExtras.sSampleVolume;
						lSs.ignore();
						lSs >> lObject->sExtras.sFilename;
					}

					lOsuFile.sHitObjects.push_back(lObject);
				}
				else if (lType & (1 << 3))
				{
					hit_object_spinner* lObject = new hit_object_spinner;
					lObject->sX = lX;
					lObject->sY = lY;
					lObject->sTime = lTime;
					lObject->sType = lType;
					lObject->sHitSounds = lHitSounds;

					lSs.ignore();
					lSs >> lObject->sEndTime;

					if (lSs.peek() == ',')
					{
						lSs.ignore();
						lSs >> lObject->sExtras.sSampleSet;
						lSs.ignore();
						lSs >> lObject->sExtras.sAdditionSet;
						lSs.ignore();
						lSs >> lObject->sExtras.sCustomIndex;
						lSs.ignore();
						lSs >> lObject->sExtras.sSampleVolume;
						lSs.ignore();
						lSs >> lObject->sExtras.sFilename;
					}

					lOsuFile.sHitObjects.push_back(lObject);
				}

			}
		}

		bool is_circle(hit_object* lObject)
		{
			return (lObject->sType & (1 << 0));
		}

		bool is_slider(hit_object* lObject)
		{
			return (lObject->sType & (1 << 1));
		}

		bool is_spinner(hit_object* lObject)
		{
			return (lObject->sType & (1 << 3));
		}
	}
}

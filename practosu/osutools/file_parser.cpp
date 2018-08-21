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
					lOsuFile.s_file_name = lFilePath.substr(lPos + 1, lFilePath.size());
					lFilePath.erase(lPos, lFilePath.size());
					lPos = lFilePath.rfind("\\");
					lOsuFile.s_folder_name = lFilePath.substr(lPos + 1, lFilePath.size());

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
					lOsuFile.s_beatmap_version = static_cast<uint8_t>(sVersion);
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
			try
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
						lOsuFile.s_audio_filename = lLine;
					else if (lToken == "SampleSet")
						lOsuFile.s_sample_set = lLine;
					else if (lToken == "StackLeniency")
						lOsuFile.s_stack_leniency = stof(lLine);
					else if (lToken == "SkinPreference")
						lOsuFile.s_skin_preference = lLine;
					else
					{
						int sInt = stoi(lLine);
						if (lToken == "AudioLeadIn")
							lOsuFile.s_audio_lead_in = sInt;
						else if (lToken == "PreviewTime")
							lOsuFile.s_preview_time = sInt;
						else if (lToken == "Mode")
							lOsuFile.s_mode = static_cast<uint8_t>(sInt);
						else if (lToken == "Countdown")
							lOsuFile.s_countdown = static_cast<uint8_t>(sInt);
						else if (lToken == "LetterboxInBreaks")
							lOsuFile.s_letterbox_in_breaks = static_cast<uint8_t>(sInt);
						else if (lToken == "WidescreenStoryboard")
							lOsuFile.s_widescreen_storyboard = static_cast<uint8_t>(sInt);
						else if (lToken == "EpilepsyWarning")
							lOsuFile.s_epilepsy_warning = static_cast<uint8_t>(sInt);
					}
				}
			}
			catch (std::exception& e)
			{
				throw std::invalid_argument("Error parsing general section: " + *e.what());
			}
		}

		void read_file_editor(std::ifstream &lFs, osu_file &lOsuFile)
		{
			try
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
							lOsuFile.s_bookmarks.push_back(lNum);
							if (lSs.peek() == ',')
								lSs.ignore();
						}
					}
					else if (lToken == "DistanceSpacing")
						lOsuFile.s_distance_spacing = stof(lLine);
					else if (lToken == "TimelineZoom")
						lOsuFile.s_timeline_zoom = stof(lLine);
					else
					{
						int sInt = stoi(lLine);
						if (lToken == "BeatDivisor")
							lOsuFile.s_beat_divisor = static_cast<uint8_t>(sInt);
						else if (lToken == "GridSize")
							lOsuFile.s_grid_size = static_cast<uint8_t>(sInt);
					}
				}
			}
			catch(std::exception& e)
			{
				throw std::invalid_argument("Error parsing editor section: " + *e.what());
			}
		}

		void read_file_metadata(std::ifstream &lFs, osu_file &lOsuFile)
		{
			try
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
						lOsuFile.s_title = lLine;
					else if (lToken == "TitleUnicode")
						lOsuFile.s_title_unicode = lLine;
					else if (lToken == "Artist")
						lOsuFile.s_artist = lLine;
					else if (lToken == "ArtistUnicode")
						lOsuFile.s_artist_unicode = lLine;
					else if (lToken == "Creator")
						lOsuFile.s_creator = lLine;
					else if (lToken == "Version")
						lOsuFile.s_version = lLine;
					else if (lToken == "Source")
						lOsuFile.s_source = lLine;
					else if (lToken == "Tags")
						lOsuFile.s_tags = lLine;
					else if (lToken == "BeatmapID")
						lOsuFile.s_beatmap_id = stoi(lLine);
					else if (lToken == "BeatmapSetID")
						lOsuFile.s_beatmap_set_id = stoi(lLine);
				}
			}
			catch (std::exception& e)
			{
				throw std::invalid_argument("Error parsing metadata section: " + *e.what());
			}
		}

		void read_file_difficulty(std::ifstream &lFs, osu_file &lOsuFile)
		{
			try
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
						lOsuFile.s_hp_drain_rate = stof(lLine);
					else if (lToken == "CircleSize")
						lOsuFile.s_circle_size = stof(lLine);
					else if (lToken == "OverallDifficulty")
						lOsuFile.s_overall_difficulty = stof(lLine);
					else if (lToken == "ApproachRate")
						lOsuFile.s_approach_rate = stof(lLine);
					else if (lToken == "SliderMultiplier")
						lOsuFile.s_slider_multiplier = stof(lLine);
					else if (lToken == "SliderTickRate")
						lOsuFile.s_slider_tick_rate = stof(lLine);
				}
			}
			catch (std::exception& e)
			{
				throw std::invalid_argument("Error parsing difficulty section: " + *e.what());
			}
		}
		void read_file_events(std::ifstream &lFs, osu_file &lOsuFile)
		{
			try
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
						lOsuFile.s_break_periods.push_back(pair<uint32_t, uint32_t>(lTemp, lTemp2));
					}
				}
			}
			catch (std::exception& e)
			{
				throw std::invalid_argument("Error parsing events section: " + *e.what());
			}
		}
		void read_file_timingpoints(std::ifstream &lFs, osu_file &lOsuFile)
		{
			try
			{
				string lLine;
				while (getline(lFs, lLine))
				{
					if (lLine.length() == 0)
						return;

					stringstream lSs(lLine);
					timing_point lTP;
					uint32_t lTemp;
					lSs >> lTP.s_offset;
					lSs.ignore();
					lSs >> lTP.s_ms_per_beat;
					lSs.ignore();
					lSs >> lTP.s_meter;
					lSs.ignore();
					lSs >> lTemp;
					lTP.s_sample_set = static_cast<uint8_t>(lTemp);
					lSs.ignore();
					lSs >> lTemp;
					lTP.s_sample_index = static_cast<uint8_t>(lTemp);
					lSs.ignore();
					lSs >> lTemp;
					lTP.s_volume = static_cast<uint8_t>(lTemp);
					if (lSs.peek() == ',');
					{
						lSs.ignore();
						lSs >> lTemp;
						lTP.s_inherited = static_cast<uint8_t>(lTemp);
						lSs.ignore();
						lSs >> lTemp;
						lTP.s_kiai = static_cast<uint8_t>(lTemp);
					}

					lOsuFile.s_timing_points.push_back(lTP);
				}
			}
			catch (std::exception& e)
			{
				throw std::invalid_argument("Error parsing timingpoints section: " + *e.what());
			}
		}

		void read_file_colours(std::ifstream &lFs, osu_file &lOsuFile)
		{
			try
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
					lOsuFile.s_colours.push_back(tuple<int, int, int>(lNum1, lNum2, lNum3));
				}
			}
			catch (std::exception& e)
			{
				throw std::invalid_argument("Error parsing colours section: " + *e.what());
			}
		}

		void read_file_hitobjects(std::ifstream &lFs, osu_file &lOsuFile)
		{
			try
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
						lObject->s_x = lX;
						lObject->s_y = lY;
						lObject->s_time = lTime;
						lObject->s_type = lType;
						lObject->s_hit_sounds = lHitSounds;

						if (lSs.peek() == ',')
						{
							lSs.ignore();
							lSs >> lObject->s_extras.s_sample_set;
							lSs.ignore();
							lSs >> lObject->s_extras.s_addition_set;
							lSs.ignore();
							lSs >> lObject->s_extras.s_custom_index;
							lSs.ignore();
							lSs >> lObject->s_extras.s_sample_volume;
							lSs.ignore();
							lSs >> lObject->s_extras.s_filename;
						}

						lOsuFile.s_hit_objects.push_back(lObject);
					}
					else if (lType & (1 << 1))
					{
						hit_object_slider* lObject = new hit_object_slider;
						lObject->s_x = lX;
						lObject->s_y = lY;
						lObject->s_time = lTime;
						lObject->s_type = lType;
						lObject->s_hit_sounds = lHitSounds;

						lSs.ignore();
						lSs >> lObject->s_slider_type;
						while (lSs.peek() == '|')
						{
							lSs.ignore();
							uint32_t lFirst, lSecond;
							lSs >> lFirst;
							lSs.ignore();
							lSs >> lSecond;
							lObject->s_curve_points.push_back(pair<uint32_t, uint32_t>(lFirst, lSecond));
						}
						lSs.ignore();
						lSs >> lTemp;
						lObject->s_repeat = static_cast<uint8_t>(lTemp);
						lSs.ignore();
						lSs >> lObject->s_pixel_length;
						if (lSs.peek() == ',')
						{
							do
							{
								lSs.ignore();
								lSs >> lTemp;
								lObject->s_edge_hitsounds.push_back(static_cast<uint8_t>(lTemp));
							} while (lSs.peek() == '|');
						}
						if (lSs.peek() == ',')
						{
							do
							{
								lSs.ignore();
								uint8_t lTemp_u8;
								lSs >> lTemp;
								lTemp_u8 = static_cast<uint8_t>(lTemp);
								lSs.ignore();
								lSs >> lTemp;
								lObject->s_edge_additions.push_back(pair<uint8_t, uint8_t>(lTemp_u8, static_cast<uint8_t>(lTemp)));
							} while (lSs.peek() == '|');
						}

						if (lSs.peek() == ',')
						{
							lSs.ignore();
							lSs >> lObject->s_extras.s_sample_set;
							lSs.ignore();
							lSs >> lObject->s_extras.s_addition_set;
							lSs.ignore();
							lSs >> lObject->s_extras.s_custom_index;
							lSs.ignore();
							lSs >> lObject->s_extras.s_sample_volume;
							lSs.ignore();
							lSs >> lObject->s_extras.s_filename;
						}

						lOsuFile.s_hit_objects.push_back(lObject);
					}
					else if (lType & (1 << 3))
					{
						hit_object_spinner* lObject = new hit_object_spinner;
						lObject->s_x = lX;
						lObject->s_y = lY;
						lObject->s_time = lTime;
						lObject->s_type = lType;
						lObject->s_hit_sounds = lHitSounds;

						lSs.ignore();
						lSs >> lObject->s_end_time;

						if (lSs.peek() == ',')
						{
							lSs.ignore();
							lSs >> lObject->s_extras.s_sample_set;
							lSs.ignore();
							lSs >> lObject->s_extras.s_addition_set;
							lSs.ignore();
							lSs >> lObject->s_extras.s_custom_index;
							lSs.ignore();
							lSs >> lObject->s_extras.s_sample_volume;
							lSs.ignore();
							lSs >> lObject->s_extras.s_filename;
						}

						lOsuFile.s_hit_objects.push_back(lObject);
					}

				}
			}
			catch (std::exception& e)
			{
				throw std::invalid_argument("Error parsing hitobjects section: " + *e.what());
			}
		}

		bool is_circle(hit_object* lObject)
		{
			return (lObject->s_type & (1 << 0));
		}

		bool is_slider(hit_object* lObject)
		{
			return (lObject->s_type & (1 << 1));
		}

		bool is_spinner(hit_object* lObject)
		{
			return (lObject->s_type & (1 << 3));
		}
	}
}

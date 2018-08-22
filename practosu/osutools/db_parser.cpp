
#include "stdafx.h"
#include "db_parser.hpp"
#include "file_structs.hpp"

using namespace std;
namespace fs = std::experimental::filesystem;

namespace osu_tools
{
	namespace db_parser
	{
		bool parse_osu_db(fs::path &pFilePath)
		{
			// confirm file path exists and db is not cached
			if (!a_db_cached)
			{
				// Throw error if file cannot be found.
				if (!fs::exists(pFilePath))
					throw std::invalid_argument("osu.db file does not exist in path " + pFilePath.string());
				// open file as binary
				ifstream lFile(pFilePath.string(), ios::binary);
				// check file is good
				if (lFile.good())
				{
					try
					{
						// parse general information
						a_cached_db.s_osu_version = get_int(lFile);
						a_cached_db.s_folder_count = get_int(lFile);
						a_cached_db.s_account_unlocked = get_bool(lFile);
						a_cached_db.s_unlock_time = get_long_long(lFile);
						a_cached_db.s_player_name = get_string(lFile);
						a_cached_db.s_beatmap_count = get_int(lFile);
						a_cached_db.s_file_details = vector<tuple<uint32_t, string, string>>();
						//lDb.s_beatmaps = vector<beatmap>(); // can be used to store ALL beatmap information.
						// iterate over beatmap entries
						for (uint32_t i = 0; i < a_cached_db.s_beatmap_count; ++i)
						{
							// parse beatmap information
							beatmap lBeatmap;
							lBeatmap.s_entry_sz = get_int(lFile);
							lBeatmap.s_artist = get_string(lFile);
							lBeatmap.s_artist_unicode = get_string(lFile);
							lBeatmap.s_title = get_string(lFile);
							lBeatmap.s_title_unicode = get_string(lFile);
							lBeatmap.s_creator = get_string(lFile);
							lBeatmap.s_difficulty = get_string(lFile);
							lBeatmap.s_audio_name = get_string(lFile);
							lBeatmap.s_md5 = get_string(lFile);
							lBeatmap.s_osu_filename = get_string(lFile);
							lBeatmap.s_ranked_status = get_char(lFile);
							lBeatmap.s_num_hitcircles = get_short(lFile);
							lBeatmap.s_num_sliders = get_short(lFile);
							lBeatmap.s_num_spinners = get_short(lFile);
							lBeatmap.s_last_modification = get_long_long(lFile);

							// should use non-float if version is before 2014-06-09
							lBeatmap.s_ar_float = get_float(lFile);
							lBeatmap.s_cs_float = get_float(lFile);
							lBeatmap.s_hp_float = get_float(lFile);
							lBeatmap.s_od_float = get_float(lFile);

							lBeatmap.s_slider_velocity = get_double(lFile);

							// should be removed if version is before 2014-06-09
							lBeatmap.s_ratings_std = get_int(lFile);
							for (uint32_t j = 0; j < lBeatmap.s_ratings_std; ++j)
								lBeatmap.s_star_rating_std.push_back(get_int_double_pair(lFile));
							lBeatmap.s_ratings_taiko = get_int(lFile);
							for (uint32_t j = 0; j < lBeatmap.s_ratings_taiko; ++j)
								lBeatmap.s_star_rating_taiko.push_back(get_int_double_pair(lFile));
							lBeatmap.s_ratings_ctb = get_int(lFile);
							for (uint32_t j = 0; j < lBeatmap.s_ratings_ctb; ++j)
								lBeatmap.s_star_rating_ctb.push_back(get_int_double_pair(lFile));
							lBeatmap.s_ratings_mania = get_int(lFile);
							for (uint32_t j = 0; j < lBeatmap.s_ratings_mania; ++j)
								lBeatmap.s_star_rating_mania.push_back(get_int_double_pair(lFile));

							lBeatmap.s_draintime = get_int(lFile);
							lBeatmap.s_total_time = get_int(lFile);
							lBeatmap.s_preview_time = get_int(lFile);
							lBeatmap.s_timing_point_count = get_int(lFile);
							for (uint32_t j = 0; j < lBeatmap.s_timing_point_count; ++j)
								lBeatmap.s_timing_points.push_back(std::tuple<double, double, bool>(get_double(lFile), get_double(lFile), get_bool(lFile)));
							lBeatmap.s_beatmap_id = get_int(lFile);
							lBeatmap.s_beatmap_set_id = get_int(lFile);
							lBeatmap.s_thread_id = get_int(lFile);
							lBeatmap.s_grade_std = get_char(lFile);
							lBeatmap.s_grade_taiko = get_char(lFile);
							lBeatmap.s_grade_ctb = get_char(lFile);
							lBeatmap.s_grade_mania = get_char(lFile);
							lBeatmap.s_local_offset = get_short(lFile);
							lBeatmap.s_stack_leniency = get_float(lFile);
							lBeatmap.s_gameplay_mode = get_char(lFile);
							lBeatmap.s_source = get_string(lFile);
							lBeatmap.s_tags = get_string(lFile);
							lBeatmap.s_online_offset = get_short(lFile);
							lBeatmap.s_font = get_string(lFile);
							lBeatmap.s_played = get_bool(lFile);
							lBeatmap.s_last_playtime = get_long_long(lFile);
							lBeatmap.s_is_osz2 = get_bool(lFile);
							lBeatmap.s_folder_name = get_string(lFile);
							lBeatmap.s_last_checked_online = get_long_long(lFile);
							lBeatmap.s_ignore_beatmap_sound = get_bool(lFile);
							lBeatmap.s_ignore_beatmap_skin = get_bool(lFile);
							lBeatmap.s_disable_sb = get_bool(lFile);
							lBeatmap.s_disable_video = get_bool(lFile);
							lBeatmap.s_visual_override = get_bool(lFile);
							// should be uncommented if version is before 2014-06-09
							//lBeatmap.s_unknown = get_short(lFile);
							lBeatmap.s_last_modification_time = get_int(lFile);
							lBeatmap.s_mania_scroll_speed = get_char(lFile);

							//lDb.s_beatmaps.push_back(lBeatmap); // if using whole beatmap, uncomment this.
							a_cached_db.s_file_details.push_back(std::tuple<uint32_t, string, string>(lBeatmap.s_beatmap_id, lBeatmap.s_folder_name, lBeatmap.s_osu_filename));
						}
					}
					catch (std::exception& e)
					{
						// Clear cached data.
						a_cached_db.s_file_details.clear();
						a_cached_db = a_empty_struct;
						throw e;
					}

					// Success
					lFile.close();
					a_db_cached = true;
					return true;
				}
			}
			return false;
		}

		// Read a float
		float get_float(std::ifstream &pFs)
		{
			float lRet;
			pFs.read(reinterpret_cast<char*>(&lRet), sizeof(lRet));
			return lRet;
		}

		// Read a double
		double get_double(std::ifstream &pFs)
		{
			double lRet;
			pFs.read(reinterpret_cast<char*>(&lRet), sizeof(lRet));
			return lRet;
		}

		// Read a uint8_t
		uint8_t get_char(std::ifstream &pFs)
		{
			uint8_t lRet;
			pFs.read(reinterpret_cast<char*>(&lRet), sizeof(lRet));
			return lRet;
		}

		// Read a short
		uint16_t get_short(std::ifstream &pFs)
		{
			uint16_t lRet;
			pFs.read(reinterpret_cast<char*>(&lRet), sizeof(lRet));
			return lRet;
		}

		// Read a int
		uint32_t get_int(std::ifstream &pFs)
		{
			uint32_t lRet;
			pFs.read(reinterpret_cast<char*>(&lRet), sizeof(lRet));
			return lRet;
		}

		// Read a long long
		uint64_t get_long_long(std::ifstream &pFs)
		{
			uint64_t lRet;
			pFs.read(reinterpret_cast<char*>(&lRet), sizeof(lRet));
			return lRet;
		}

		// Read a bool
		bool get_bool(std::ifstream &pFs)
		{

			return (get_char(pFs) ? true : false);
		}

		// Read a string
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

		// Read an int-double pair
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

#pragma once

#include "stdafx.h"
#include "file_structs.hpp"

// This struct holds the information contained within osu!.db
struct osu_db
{
	uint32_t s_osu_version;
	uint32_t s_folder_count;
	bool s_account_unlocked;
	uint64_t s_unlock_time;
	std::string s_player_name;
	uint32_t s_beatmap_count = -1;
	std::vector < std::tuple<uint32_t, std::string, std::string>> s_file_details;
	//std::vector<beatmap> s_beatmaps; // This can hold detailed information about beatmaps as read from the db. Currently disabled to improve load time.
};

namespace osu_tools
{
	namespace db_parser
	{
		inline osu_db a_cached_db;
		inline bool a_db_cached = false;
		static const osu_db a_empty_struct;

		bool parse_osu_db(std::experimental::filesystem::path &p_file_path);

		float get_float(std::ifstream &p_fs);
		double get_double(std::ifstream &p_fs);
		uint8_t get_char(std::ifstream &p_fs);
		uint32_t get_int(std::ifstream &p_fs);
		uint16_t get_short(std::ifstream &p_fs);
		uint64_t get_long_long(std::ifstream &p_fs);
		bool get_bool(std::ifstream &p_fs);
		std::string get_string(std::ifstream &p_fs);
		std::pair<int, double> get_int_double_pair(std::ifstream &p_fs);
	}
}
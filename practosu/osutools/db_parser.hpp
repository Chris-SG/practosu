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

// General namespace for all osu_tools
namespace osu_tools
{
	// db_parser allows the user to parse the contents of the osu!.db file. The user may
	// do so by calling "osu_tools::db_parser::parse_osu_db(fs::path)" to parse the
	// given file. The result is stored in a_cached_db.
	namespace db_parser
	{
		inline osu_db a_cached_db;
		inline bool a_db_cached = false;
		static const osu_db a_empty_struct;

		// Will parse the osu!.db file passed. Does not do any checking, so ensure your path
		// is correct. Will return successful parsing of file. If a_force is set to true,
		// will recache db.
		bool parse_osu_db(std::experimental::filesystem::path &p_file_path, bool a_force = false);

		// Get float from file stream as per osu! reference.
		float get_float(std::ifstream &p_fs);
		// Get double from file stream as per osu! reference.
		double get_double(std::ifstream &p_fs);
		// Get char from file stream as per osu! reference.
		uint8_t get_char(std::ifstream &p_fs);
		// Get int from file stream as per osu! reference.
		uint32_t get_int(std::ifstream &p_fs);
		// Get short from file stream as per osu! reference.
		uint16_t get_short(std::ifstream &p_fs);
		// Get long long from file stream as per osu! reference.
		uint64_t get_long_long(std::ifstream &p_fs);
		// Get bool from file stream as per osu! reference.
		bool get_bool(std::ifstream &p_fs);
		// Get string from file stream as per osu! reference.
		std::string get_string(std::ifstream &p_fs);
		// Get int-double pair from file stream as per osu! reference.
		std::pair<int, double> get_int_double_pair(std::ifstream &p_fs);
	}
}
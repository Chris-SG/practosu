#pragma once

#include "stdafx.h"
#include "file_structs.hpp"

struct osu_db
{
	uint32_t sOsuVersion;
	uint32_t sFolderCount;
	bool sAccountUnlocked;
	uint64_t sUnlockTime;
	std::string sPlayerName;
	uint32_t sBeatmapCount;
	std::vector < std::tuple<uint32_t, std::string, std::string>> sFileDetails;
	//std::vector<beatmap> sBeatmaps;
};

namespace osu_tools
{
	namespace db_parser
	{
		osu_db parse_osu_db(std::experimental::filesystem::path &pFilePath);

		float get_float(std::ifstream &pFs);
		double get_double(std::ifstream &pFs);
		uint8_t get_char(std::ifstream &pFs);
		uint32_t get_int(std::ifstream &pFs);
		uint16_t get_short(std::ifstream &pFs);
		uint64_t get_long_long(std::ifstream &pFs);
		bool get_bool(std::ifstream &pFs);
		std::string get_string(std::ifstream &pFs);
		std::pair<int, double> get_int_double_pair(std::ifstream &pFs);
	}
}
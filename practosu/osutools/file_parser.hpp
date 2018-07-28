#pragma once

#include "stdafx.h"
#include "file_structs.hpp"

namespace osu_tools
{
	namespace file_parser
	{
		osu_file parse_osu_file(std::experimental::filesystem::path &pFilePath);

		void read_file_general(std::ifstream &lFs, osu_file &lOsuFile);
		void read_file_editor(std::ifstream &lFs, osu_file &lOsuFile);
		void read_file_metadata(std::ifstream &lFs, osu_file &lOsuFile);
		void read_file_difficulty(std::ifstream &lFs, osu_file &lOsuFile);
		void read_file_events(std::ifstream &lFs, osu_file &lOsuFile);
		void read_file_timingpoints(std::ifstream &lFs, osu_file &lOsuFile);
		void read_file_colours(std::ifstream &lFs, osu_file &lOsuFile);
		void read_file_hitobjects(std::ifstream &lFs, osu_file &lOsuFile);

		bool is_circle(hit_object* lObject);
		bool is_slider(hit_object* lObject);
		bool is_spinner(hit_object* lObject);
	}
}

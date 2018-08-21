#pragma once

#include "stdafx.h"
#include "file_structs.hpp"

namespace osu_tools
{
	namespace file_parser
	{
		osu_file parse_osu_file(std::experimental::filesystem::path &p_file_path);

		void read_file_general(std::ifstream &l_fs, osu_file &l_osu_file);
		void read_file_editor(std::ifstream &l_fs, osu_file &l_osu_file);
		void read_file_metadata(std::ifstream &l_fs, osu_file &l_osu_file);
		void read_file_difficulty(std::ifstream &l_fs, osu_file &l_osu_file);
		void read_file_events(std::ifstream &l_fs, osu_file &l_osu_file);
		void read_file_timingpoints(std::ifstream &l_fs, osu_file &l_osu_file);
		void read_file_colours(std::ifstream &l_fs, osu_file &l_osu_file);
		void read_file_hitobjects(std::ifstream &l_fs, osu_file &l_osu_file);

		bool is_circle(hit_object* l_object);
		bool is_slider(hit_object* l_object);
		bool is_spinner(hit_object* l_object);
	}
}

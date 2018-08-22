#pragma once

#include "stdafx.h"
#include "file_structs.hpp"

// General namespace for all osu_tools
namespace osu_tools
{
	// file_parser allows parsing of a given .osu file. It will parse all
	// information in the file using the parse_osu_file function.
	namespace file_parser
	{
		// Parses a given .osu file p_file_path and returns it as an
		// osu_file type.
		osu_file parse_osu_file(std::experimental::filesystem::path &p_file_path);

		// Read general section from file stream.
		void read_file_general(std::ifstream &l_fs, osu_file &l_osu_file);
		// Read editor section from file stream.
		void read_file_editor(std::ifstream &l_fs, osu_file &l_osu_file);
		// Read metadata section from file stream.
		void read_file_metadata(std::ifstream &l_fs, osu_file &l_osu_file);
		// Read difficulty section from file stream.
		void read_file_difficulty(std::ifstream &l_fs, osu_file &l_osu_file);
		// Read events section from file stream.
		void read_file_events(std::ifstream &l_fs, osu_file &l_osu_file);
		// Read timingpoints section from file stream.
		void read_file_timingpoints(std::ifstream &l_fs, osu_file &l_osu_file);
		// Read colours section from file stream.
		void read_file_colours(std::ifstream &l_fs, osu_file &l_osu_file);
		// Read hitobjects section from file stream.
		void read_file_hitobjects(std::ifstream &l_fs, osu_file &l_osu_file);

		// Determines if hit_object is of type hit_object_circle.
		bool is_circle(hit_object* l_object);
		// Determines if hit_object is of type hit_object_slider.
		bool is_slider(hit_object* l_object);
		// Determines if hit_object is of type hit_object_slider.
		bool is_spinner(hit_object* l_object);
		// Determines if hit_object is of type hit_object_mania_hold.
		bool is_mania_hold(hit_object* l_object);
	}
}

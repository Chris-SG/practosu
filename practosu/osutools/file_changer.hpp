#pragma once

#include "stdafx.h"
#include "file_structs.hpp"

// General namespace for all osu_tools
namespace osu_tools
{
	// file_changer is used to modify the timing of a given beatmap passed to
	// the function. This allows for changing by a multiplier, percentage
	// or set numerical value.
	namespace file_changer
	{
		// Will set the speed based on a multiplier value. This value can be any
		// float value, however should be positive. The a_new_filename parameter
		// is the name of the new audio file created.
		void set_speed_multiplier(float a_multi, osu_file& a_osu_file, std::string a_new_filename);
		// Will set the speed based on a percentage value. This value can be any
		// float value, however should be positive. The a_new_filename parameter
		// is the name of the new audio file created.
		void set_speed_percentage(float a_multi, osu_file& a_osu_file, string aNewFilename);
		// Converts sliders into a single hitcircle located at the initial point
		// of each slider.
		void convert_sliders_to_circle(osu_file& a_osu_file);

		double get_bpm_max(std::vector<timing_point> timing_point);

		double ms_per_beat_to_bpm(double ms);

		double bpm_to_ms_per_beat(double bpm);

		void flip_play_area(osu_file& a_osu_file, bool flip_x, bool flip_y);

		void flip_play_area_quadrants(osu_file& a_osu_file, bool flip_x, bool flip_y);
	}
}
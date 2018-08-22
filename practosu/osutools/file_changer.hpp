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
		// to be implemented
		void set_speed_percentage(float a_multi, osu_file& a_osu_file);
	}
}
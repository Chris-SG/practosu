#pragma once

#include "stdafx.h"
#include "file_structs.hpp"

namespace osu_tools
{
	namespace file_changer
	{
		void set_speed_multiplier(float a_multi, osu_file& a_osu_file, std::string a_new_filename);
		void set_speed_percentage(float a_multi, osu_file& a_osu_file);
	}
}
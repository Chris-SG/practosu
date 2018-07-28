#pragma once

#include "stdafx.h"
#include "file_structs.hpp"

namespace osu_tools
{
	namespace file_changer
	{
		void set_speed_multiplier(float aMulti, osu_file& aOsuFile);
		void set_speed_percentage(float aMulti, osu_file& aOsuFile);
	}
}
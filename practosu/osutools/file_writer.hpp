#pragma once

#include "stdafx.h"
#include "file_structs.hpp"

namespace osu_tools
{
	namespace file_writer
	{
		bool write_file(osu_file& a_osu_file, std::string& a_new_filename, std::string& a_new_audio_filename, std::string& a_new_version, const float& a_multiplier = 1.0);
	}
	
}
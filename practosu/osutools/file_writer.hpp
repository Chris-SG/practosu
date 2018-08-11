#pragma once

#include "stdafx.h"
#include "file_structs.hpp"

namespace osu_tools
{
	namespace file_writer
	{
		bool write_file(osu_file& aOsuFile, std::string& aNewFilename, std::string& aNewAudioFilename, const float& aMultiplier = 1.0);
	}
	
}
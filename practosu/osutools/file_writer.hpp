#pragma once

#include "stdafx.h"
#include "file_structs.hpp"

// General namespace for all osu_tools
namespace osu_tools
{
	// file_writer is used to output a modified osu_file to a file.
	namespace file_writer
	{
		// write_file will take in the osu_file to use a_osu_file, a new
		// filename to use a_new_filename, the new audio filename to use
		// a_new_audio_filename, the new version name to use a_new_version
		// and an optional multiplier to apply to the beatmap. Will return
		// true if successful.
		bool write_file(osu_file& a_osu_file, std::string& a_new_filename, std::string& a_new_audio_filename, std::string& a_new_version, const float& a_multiplier = 1.0);
	}
}
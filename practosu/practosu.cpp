// practosu.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "func.hpp"
#include "file_parser.hpp"
#include "file_changer.hpp"
#include "file_writer.hpp"


int main()
{
	//std::cout << osu_tools::func::get_osu_base_address() << std::endl;
	std::cout << osu_tools::func::get_current_beatmap() << std::endl;
	auto lPath = osu_tools::func::get_current_beatmap();
	auto lOsuFile = osu_tools::file_parser::parse_osu_file(lPath);
	osu_tools::file_changer::set_speed_multiplier(2.3, lOsuFile);
	if (osu_tools::file_writer::write_file(lOsuFile))
		std::cout << "SUCCESS" << std::endl;
	else
		std::cout << "FAIL" << std::endl;
	
	Sleep(10000);
    return 0;
}


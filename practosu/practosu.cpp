// practosu.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "func.hpp"


int main()
{
	std::cout << osu_tools::func::get_osu_base_address() << std::endl;
	std::cout << osu_tools::func::get_current_beatmap() << std::endl;
	Sleep(50000);
    return 0;
}


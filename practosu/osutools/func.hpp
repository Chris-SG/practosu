#pragma once

#include "stdafx.h"


namespace osu_tools
{
	class func
	{
	public:
		static bool osu_running();

		static std::experimental::filesystem::path get_beatmap_directory();
		static std::experimental::filesystem::path get_osu_directory();

		static std::experimental::filesystem::path get_current_beatmap();

		//const static inline unsigned char mBeatmapMask[] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x28\x12\x8B\x03\x28\x12\x8B\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
		//const static inline char mBeatmapMaskSz[] = "xxxxxxxxxxxxxxxx????xx??xx??xxxxxxxxxxxxxxxx";
		const static inline unsigned char m_beatmap_mask[] = "\xC0\x0F\x8E\x8F\x01\x00\x00\xA1\x00\x00\x00\x00\x89\x45\xC0\x85";
		const static inline char m_beatmap_mask_sz[] = "xxxxxxxx????xxxx";

		static HWND get_osu_wind_handle(const int& p_retry = 0);
		static DWORD get_osu_proc_id();
		static void get_osu_proc_handle(HANDLE &f_hdl);
		static LPVOID get_osu_base_address();
		static DWORD get_osu_beatmap_pointer();

		static DWORD __stdcall FindPattern(std::vector<unsigned char> data, unsigned int base_address, const unsigned char* lp_pattern, const char* psz_mask, int offset, int result_usage);
	};
}
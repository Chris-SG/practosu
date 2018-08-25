// TODO convert from class into namespace

#pragma once

#include "stdafx.h"

// General namespace for all osu_tools
namespace osu_tools
{
	// Contains general functions used by the program for various
	// reasons.
	class func
	{
	public:
		// osu_running checks if the osu! process is currently running.
		static bool osu_running();

		// get_beatmap_directory retrieves the directory beatmaps are stored.
		static std::experimental::filesystem::path get_beatmap_directory();
		// get_osu_directory retrieves osu!'s running directory.
		static std::experimental::filesystem::path get_osu_directory();
		// get_current_beatmap gets the beatmap currently selected ingame.
		static std::experimental::filesystem::path get_current_beatmap();

		// pattern mask to scan for in memory
		const static inline unsigned char m_beatmap_mask[] = "\xC0\x0F\x8E\x8F\x01\x00\x00\xA1\x00\x00\x00\x00\x89\x45\xC0\x85";
		// apply size to mask
		const static inline char m_beatmap_mask_sz[] = "xxxxxxxx????xxxx";
		// pointer to beatmap
		static inline DWORD m_beatmap_pointer = 0;

		// Manual pointer search values
		const static inline unsigned char m_my_love_easy[] = "\xE0\x10\x06\x00\x28\x7D\x02\x00";
		const static inline unsigned char m_my_love_medium[] = "\xDE\x10\x06\x00\x28\x7D\x02\x00";
		const static inline unsigned char m_my_love_hard[] = "\xDF\x10\x06\x00\x28\x7D\x02\x00";
		const static inline char m_my_love_easy_sz[] = "xxxxxxxx";
		const static inline char m_my_love_medium_sz[] = "xxxxxxxx";
		const static inline char m_my_love_hard_sz[] = "xxxxxxxx";

		// Options used in manual pointer search
		static inline std::vector<DWORD> m_options;
		static inline uint8_t m_progress = 0;

		// get_osu_wind_handle gets the handle for the osu! window.
		static HWND get_osu_wind_handle(const int& p_retry = 0);
		// get_osu_proc_id gets the process id for osu!
		static DWORD get_osu_proc_id();
		// get_osu_proc_handle gets the handle for osu!
		static void get_osu_proc_handle(HANDLE &f_hdl);
		// get_osu_base_address gets the base address of the osu! process
		static LPVOID get_osu_base_address();
		// get_osu_beatmap_pointer gets the pointer to the current osu! beatmap pointer.
		static DWORD get_osu_beatmap_pointer(bool a_force = false);
		// backup method of searching for beatmap pointer
		static void backup_pointer_search();

		// Finds a given pattern based on the passed mask within a block of data
		static DWORD __stdcall FindPattern(std::vector<unsigned char> data, unsigned int base_address, const unsigned char* lp_pattern, const char* psz_mask, int offset, int result_usage);
	};
}
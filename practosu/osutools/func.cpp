#include "stdafx.h"
#include "func.hpp"
#include "db_parser.hpp"

#include <iostream>
#include <string>

namespace fs = std::experimental::filesystem;
using namespace std;

namespace osu_tools
{
	bool func::osu_running()
	{
		// If we can get the osu! window handle, the game is running
		return (get_osu_wind_handle() == nullptr ? false : true);
	}

	fs::path func::get_beatmap_directory()
	{
		// Create base path to return
		fs::path lRet = fs::path();
		// Get running user's username
		char lUser[UNLEN + 1] = { 0 };
		DWORD lUser_len = UNLEN + 1;
		GetUserNameA(lUser, &lUser_len);
		// Convert char[] to string
		string lUserStr;
		for (unsigned int i = 0; i < lUser_len; ++i)
		{
			if (lUser[i] != 0)
				lUserStr += lUser[i];
		}

		// Create config file name using user string
		string lCfg = "osu!." + lUserStr + ".cfg";
		// Create full config path including osu! directory
		fs::path lPath = fs::path(get_osu_directory());
		fs::path lCfgPath = fs::path(lPath);
		lCfgPath += lCfg;
		// Confirm file exists
		if (fs::exists(lCfgPath))
		{
			// Open config file
			fstream lFs(lCfgPath.string(), fstream::in);
			if (lFs.good())
			{
				// Iterate over all lines
				string lLine;
				while (getline(lFs, lLine))
				{
					// Remove whitespace from string
					lLine.erase(std::remove_if(lLine.begin(), lLine.end(), ::isspace), lLine.end());

					// Search for BeatmapDirectory starting from the beginning of the string
					if (lLine.find("BeatmapDirectory") == 0)
					{
						// If found, trim the beginning
						auto lPos = lLine.find("=");
						if (lPos != string::npos)
							lLine.erase(0, lPos+1);

						// If we find :\, path is absolute. If not, path is relative
						lPos = lLine.rfind(":\\");
						if (lPos == string::npos)
						{
							lRet += lPath;
							lRet += lLine;
							lRet += "\\";
						}
						else
						{
							lRet += lLine;
							lRet += "\\";
						}
						break;
					}
					// Clear line
					lLine.clear();
				}
			}
		}
		return lRet;
	}

	fs::path func::get_osu_directory()
	{
		// Get process handle of osu!
		HANDLE lProcHdl;
		get_osu_proc_handle(lProcHdl);
		// Empty path
		fs::path lRet = fs::path();
		// Confirm we have a valid handle
		if (lProcHdl != nullptr)
		{
			// Create buffer and obtain filename of module
			char lBuf[MAX_PATH] = { 0 };
			if (GetModuleFileNameExA(lProcHdl, 0, lBuf, MAX_PATH - 1) == 0)
			{
				throw std::invalid_argument("Invalid module filename");
			}
			else
			{
				// Move buffer into string
				string lPath;
				for (unsigned int i = 0; i < MAX_PATH; ++i)
				{
					if(lBuf[i] != 0)
						lPath += lBuf[i];
				}

				// Strip past final '\' character
				auto lPos = lPath.rfind("\\");
				if (lPos != string::npos)
				{
					lPath.erase(lPos + 1);
				}

				lRet = lPath;
			}

			// Close process handle
			CloseHandle(lProcHdl);
		}
		return lRet;
	}

	fs::path func::get_current_beatmap()
	{
		// create new path
		fs::path lRet = fs::path();
		// get pointer to beatmap
		auto lPtr = get_osu_beatmap_pointer();
		HANDLE lHdl;
		get_osu_proc_handle(lHdl);

		// read beatmap from memory
		unsigned char lBuf[4] = { 0 };

		// read pointer for value
		ReadProcessMemory(lHdl, (void*)lPtr, &lBuf, 4, NULL);
		// little endian
		int lVal = (lBuf[0] << 0 | lBuf[1] << 8 | lBuf[2] << 16 | lBuf[3] << 24);
		if (lVal == 0)
			throw std::out_of_range("Try using Tools -> Manual Pointer Search. Invalid beatmap pointer found: " + lVal);
		// unsure if this is always correct?
		lVal += 0xC0;

		// Read beatmap from memory location
		ReadProcessMemory(lHdl, (void*)lVal, &lBuf, 4, NULL);

		// sort beatmap ID correctly
		int lBmId = (lBuf[0] << 0 | lBuf[1] << 8 | lBuf[2] << 16 | lBuf[3] << 24);
#ifdef _DEBUG
		std::cout << "Beatmap ID seems to be: " << lBmId << std::endl;
		// get osu! directory and append db name
#endif
		if (lBmId <= 0)
			throw std::invalid_argument("Invalid beatmap ID found: " + lBmId);
		fs::path lPath = func::func::get_osu_directory();
		lPath += "osu!.db";
		// parse db
		db_parser::parse_osu_db(lPath);
		// iterate over db
		for (uint32_t i = 0; i < db_parser::a_cached_db.s_file_details.size(); ++i)
		{
			if (get<0>(db_parser::a_cached_db.s_file_details[i]) == lBmId)
			{
				// create file path
				lRet = func::func::get_beatmap_directory();
				lRet += get<1>(db_parser::a_cached_db.s_file_details[i]);
				lRet /= get<2>(db_parser::a_cached_db.s_file_details[i]);
				break;
			}
		}

		// return path
		return lRet;
	}

	BOOL CALLBACK enum_windows(HWND hwnd, LPARAM lParam)
	{
		char lBuf[128];
		const auto lWritten = GetWindowTextA(hwnd, lBuf, 128);
		if(lWritten && strstr(lBuf, "osu!") != nullptr)
		{
			*reinterpret_cast<HWND*>(lParam) = hwnd;
			return FALSE;
		}
		return TRUE;
	}

	HWND func::get_osu_wind_handle(const int& pRetry)
	{
		// null HWND
		HWND lRet = nullptr;

		EnumWindows(enum_windows, reinterpret_cast<LPARAM>(&lRet));

		return lRet;
	}

	DWORD func::get_osu_proc_id()
	{
		// Get process ID based on osu! window handle
		DWORD lRet;
		GetWindowThreadProcessId(get_osu_wind_handle(), &lRet);

		return lRet;
	}

	void func::get_osu_proc_handle(HANDLE &fHdl)
	{
		// Get process handle based on process ID
		fHdl = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, get_osu_proc_id());
	}

	LPVOID func::get_osu_base_address()
	{
		// get process base address
		TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
		TCHAR lProcName[MAX_PATH] = TEXT("osu!.exe");
		HMODULE hMod;
		DWORD cbNeeded;

		HANDLE lHdl;
		get_osu_proc_handle(lHdl);

		if (EnumProcessModulesEx(lHdl, &hMod, sizeof(hMod), &cbNeeded, LIST_MODULES_32BIT | LIST_MODULES_64BIT))
		{
			GetModuleBaseName(lHdl, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
		}

		return (void*)hMod;
	}

	DWORD func::get_osu_beatmap_pointer(bool a_force)
	{
		if (m_beatmap_pointer != 0 && !a_force)
			return m_beatmap_pointer;
		// Get pagesize
		SYSTEM_INFO lInfo;
		GetSystemInfo(&lInfo);
		DWORD lPageSize = lInfo.dwPageSize;

		// Prepare variables for process iteration
		LPVOID lBaseAddr = (void*)0; // base offset
		vector<unsigned char> lBuf; // buffer for pattern search
		unsigned char lBufChar[4096]; // buffer for memory search
		SIZE_T lRead = 0; //amount of characters read

		lBaseAddr = get_osu_base_address();

		// Get handle
		HANDLE lHdl;
		get_osu_proc_handle(lHdl);

		// arbitrary value, should suffice
		while ((int)lBaseAddr < 0x70FFFFFF)
		{
			// Read process for a full page size
			ReadProcessMemory(lHdl, lBaseAddr, &lBufChar, lPageSize, &lRead);
			// Add to vector
			lBuf.resize(lRead);
			lBuf.assign(lBufChar, lBufChar + lRead);
			
			// Pattern search against value
			auto lVal = FindPattern(lBuf, (int)lBaseAddr, m_beatmap_mask, m_beatmap_mask_sz, 0, 0);
			// if we have found the pattern
			if (lVal != 0)
			{
				// actual value we want is 0x8 behind the pattern
				lVal -= 0x8;
				unsigned char lPtrPtrBuf[4] = { 0 };
				unsigned char lPtrBuf[4] = { 0 };

				ReadProcessMemory(lHdl, (void*)lVal, &lPtrPtrBuf, 4, NULL);
				lVal = (lPtrPtrBuf[0] << 0 | lPtrPtrBuf[1] << 8 | lPtrPtrBuf[2] << 16 | lPtrPtrBuf[3] << 24);
				if(lVal == 0)
				{
					// MUST DO BACKUP PLAN
					throw std::out_of_range("Beatmap pointer invalid!");
				}

				m_beatmap_pointer = (DWORD)lVal;
				// Close handle
				CloseHandle(lHdl);

				// return pointer to song
				return m_beatmap_pointer;

			}

			lBaseAddr = (void*)((int)lBaseAddr + lPageSize - sizeof(m_beatmap_mask) / sizeof(m_beatmap_mask[0]) + 1);
		}
		// failed to find pointer
		std::cout << GetLastError() << std::endl;
		CloseHandle(lHdl);
		return 0;
	}

	void func::backup_pointer_search()
	{
		// Get pagesize
		SYSTEM_INFO lInfo;
		GetSystemInfo(&lInfo);
		DWORD lPageSize = lInfo.dwPageSize;

		vector<unsigned char> lBuf; // buffer for pattern search
		unsigned char lBufChar[4096]; // buffer for memory search
		SIZE_T lRead = 0; //amount of characters read
		int lCount = 0;

		// Get handle
		HANDLE lHdl;
		get_osu_proc_handle(lHdl);

		auto lBaseAddr = get_osu_base_address();
		LPVOID lAddrMax = (void*)((int)lBaseAddr + 0x1FFFFFFF);

		// Do full scan for first beatmap
		if(m_progress == 0)
		{
			DWORD l_val = 0;
			while((int)lBaseAddr < (int)lAddrMax)
			{
				// Read process for a full page size
				ReadProcessMemory(lHdl, lBaseAddr, &lBufChar, lPageSize, &lRead);
				// Add to vector
				lBuf.resize(lRead);
				lBuf.assign(lBufChar, lBufChar + lRead);

				// Pattern search against value
				l_val = FindPattern(lBuf, (int)lBaseAddr, m_my_love_easy, m_my_love_easy_sz, 0, 0);
				// if we have found the pattern
				if (l_val != 0)
				{
					break;
				}
				lBaseAddr = (void*)((int)lBaseAddr + lPageSize - sizeof(m_my_love_easy) / sizeof(m_my_love_easy[0]) + 1);
			}
			l_val -= 0xC0;
			lCount = 0;
			lRead = 0;
			lBaseAddr = get_osu_base_address();
			unsigned char pat[] = { l_val >> 0, l_val >> 8, l_val >> 16, l_val >> 24 };
			while ((int)lBaseAddr < (int)lAddrMax)
			{
				// Read process for a full page size
				ReadProcessMemory(lHdl, lBaseAddr, &lBufChar, lPageSize, &lRead);
				// Add to vector
				lBuf.resize(lRead);
				lBuf.assign(lBufChar, lBufChar + lRead);

				// Pattern search against value
				l_val = FindPattern(lBuf, (int)lBaseAddr, pat, "xxxx", 0, 0);
				// if we have found the pattern
				if (l_val != 0)
				{
					m_options.push_back(l_val);
				}
				lBaseAddr = (void*)((int)lBaseAddr + lPageSize - 4 / 4 + 1);
			}
			++m_progress;
			CloseHandle(lHdl);
			return;
		}
		else
		{
			DWORD l_val = 0;
			while ((int)lBaseAddr < (int)lAddrMax)
			{
				// Read process for a full page size
				ReadProcessMemory(lHdl, lBaseAddr, &lBufChar, lPageSize, &lRead);
				// Add to vector
				lBuf.resize(lRead);
				lBuf.assign(lBufChar, lBufChar + lRead);

				// Pattern search against value
				if(m_progress == 1)
					l_val = FindPattern(lBuf, (int)lBaseAddr, m_my_love_medium, m_my_love_medium_sz, 0, 0);
				else
					l_val = FindPattern(lBuf, (int)lBaseAddr, m_my_love_hard, m_my_love_hard_sz, 0, 0);
				// if we have found the pattern
				if (l_val != 0)
				{
					break;
				}
				lBaseAddr = (void*)((int)lBaseAddr + lPageSize - sizeof(m_my_love_easy) / sizeof(m_my_love_easy[0]) + 1);
			}
			l_val -= 0xC0;
			unsigned char pat[4] = { l_val >> 0, l_val >> 8, l_val >> 16, l_val >> 24 };
			std::vector<DWORD> l_options;
			// Loop found values
			for(int i = 0; i < m_options.size(); ++i)
			{
				unsigned char l_ptr_buf[4] = { 0 };
				// Read memory of length 4
				ReadProcessMemory(lHdl, (void*)m_options[i], &l_ptr_buf, 4, NULL);
				// Confirm data is equal between expected and new
				if(memcpy(pat, l_ptr_buf, 4))
				{
					l_options.push_back(m_options[i]);
				}
			}
			m_options = l_options;
			++m_progress;
			if(m_progress == 3)
			{
				if (m_options.size() == 0)
					throw std::invalid_argument("No value found!");
				m_beatmap_pointer = m_options[0];
				m_progress = 0;
			}
			CloseHandle(lHdl);
			return;
		}
	}

	DWORD __stdcall func::FindPattern(std::vector<unsigned char> data, unsigned int baseAddress, const unsigned char* lpPattern, const char* pszMask, int offset, int resultUsage)
	{
		// Build vectored pattern..
		std::vector<std::pair<unsigned char, bool>> pattern;
		for (size_t x = 0; x < strlen(pszMask); x++)
			pattern.push_back(std::make_pair(lpPattern[x], pszMask[x] == 'x'));

		// The result count for multiple results..
		auto resultCount = 0;
		auto scanStart = data.begin();

		while (true)
		{
			// Search for the pattern..
			auto ret = std::search(scanStart, data.end(), pattern.begin(), pattern.end(),
				[&](unsigned char curr, std::pair<unsigned char, bool> currPattern)
			{
				return (!currPattern.second) || curr == currPattern.first;
			});

			// Did we find a match..
			if (ret != data.end())
			{
				// If we hit the usage count, return the result..
				if (resultCount == resultUsage || resultUsage == 0)
					return (std::distance(data.begin(), ret) + baseAddress) + offset;

				// Increment the found count and scan again..
				++resultCount;
				scanStart = ++ret;
			}
			else
				break;
		}

		return 0;
	}
}


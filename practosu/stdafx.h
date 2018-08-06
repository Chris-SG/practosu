#pragma once

#include <QtWidgets>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <Lmcons.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <psapi.h>
#include <tchar.h>

#pragma comment(lib, "psapi.lib")
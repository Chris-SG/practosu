#include "stdafx.h"
#include "practosu.h"
#include <QtWidgets/QApplication>
#include <chrono>
#include "func.hpp"

bool gQuit = false;

void pollKeyboard(practosu& aWindow)
{
	using namespace std::chrono_literals;
	bool lPressed = false;

	INPUT lInput;
	lInput.type = INPUT_KEYBOARD;
	lInput.ki.wScan = 0;
	lInput.ki.time = 0;
	lInput.ki.dwExtraInfo = 0;
	lInput.ki.wVk = VK_MENU;

	while(1)
	{
		if (gQuit)
			return;
		try
		{
			if (GetAsyncKeyState(VK_F11) && !lPressed)
			{
				HWND lWnd = reinterpret_cast<HWND>(aWindow.winId());
				lPressed = true;
				aWindow.pressLoadMap();
				lInput.ki.dwFlags = 0;
				SendInput(1, &lInput, sizeof(INPUT));
				Sleep(50);

				SetActiveWindow(lWnd);
				SetForegroundWindow(lWnd);

				lInput.ki.dwFlags = KEYEVENTF_KEYUP;
				SendInput(1, &lInput, sizeof(INPUT));

				Sleep(1000);
			}
			else
			{
				lPressed = false;
				//std::this_thread::yield();
				std::this_thread::sleep_for(50ms);
			}
		}
		catch (std::exception& e)
		{
			return;
		}
	}
}

int main(int argc, char *argv[])
{
	if (!osu_tools::func::osu_running())
		return 2;
	QApplication a(argc, argv);
	practosu w;
	w.hide();
	std::thread lThread = std::thread(pollKeyboard, std::ref(w));
	w.show();
	/*
	int lRet = a.exec();
	gQuit = true;
	return lRet;*/
	return a.exec();
}

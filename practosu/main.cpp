#include "stdafx.h"
#include "practosu.h"
#include <QtWidgets/QApplication>
#include "func.hpp"

void pollKeyboard(practosu& aWindow)
{
	bool lPressed = false;
	while(1)
	{
		if(GetAsyncKeyState(VK_F11) && !lPressed)
		{
			HWND lWnd = reinterpret_cast<HWND>(aWindow.winId());
			lPressed = true;
			aWindow.pressLoadMap();
			SetForegroundWindow(lWnd);
			Sleep(1000);
		}
		else
		{
			lPressed =false;
			std::this_thread::yield();
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
	return a.exec();
}

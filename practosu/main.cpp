#include "stdafx.h"
#include "practosu.h"
#include <QtWidgets/QApplication>
#include <QKeyEvent>
#include "func.hpp"

void pollKeyboard(practosu& aWindow)
{
	bool lPressed = false;
	while(1)
	{
		if(GetAsyncKeyState(VK_F11) == 1 && !lPressed)
		{
			lPressed = true;
			aWindow.loadMap();
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

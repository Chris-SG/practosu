#include "stdafx.h"
#include "practosu.h"
#include <QtWidgets/QApplication>
#include <QKeyEvent>
#include "func.hpp"

int main(int argc, char *argv[])
{
	if (!osu_tools::func::osu_running())
		return 2;
	QApplication a(argc, argv);
	practosu w;
	w.hide();

	w.show();
	return a.exec();
}

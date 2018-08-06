#include "stdafx.h"
#include "dbprogress.h"
#include "func.hpp"

dbprogress::dbprogress(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(((windowFlags() | Qt::CustomizeWindowHint)	& ~Qt::WindowCloseButtonHint));
	this->setVisible(false);

	mProgressThread = new std::thread(&dbprogress::perform, this);
	//Sleep(50);

	this->setVisible(true);
	while (!mComplete)
	{
		try
		{

			int lTotal = osu_tools::db_parser::aCachedDb.sBeatmapCount;
			int lDone = osu_tools::db_parser::aCachedDb.sFileDetails.size();
			ui.label->setText(QString::fromStdString("Currently Processed " + std::to_string(lDone) + " / " + std::to_string(lTotal) + " Beatmaps"));
			if (lTotal>0 && lDone>0)
				ui.progressBar->setValue(lDone * 100 / lTotal);
			QCoreApplication::processEvents();
			Sleep(10);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	//delete mProgressThread;
}

dbprogress::~dbprogress()
{
}

void dbprogress::perform()
{
	auto lPath = osu_tools::func::get_osu_directory();
	lPath += "osu!.db";
	mDbThread = new std::thread(osu_tools::db_parser::parse_osu_db, lPath);
	mDbThread->join();
	Sleep(1000);
	mComplete = true;
}
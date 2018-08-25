#include "stdafx.h"
#include "dbprogress.h"
#include "func.hpp"

// Performs setup of UI and begins DB processing
// thread.
dbprogress::dbprogress(QWidget *parent)
	: QWidget(parent)
{
	// Setup window, disable close button.
	ui.setupUi(this);
	this->setWindowFlags(((windowFlags() | Qt::CustomizeWindowHint)	& ~Qt::WindowCloseButtonHint));
	this->setVisible(false);

	// Create processing thread.
	mProgressThread = new std::thread(&dbprogress::perform, this);

	this->setVisible(true);
	// Loop until listed as complete.
	while (!mComplete)
	{
		try
		{
			// Get values of both completed and total size.
			int lTotal = osu_tools::db_parser::a_cached_db.s_beatmap_count;
			int lDone = osu_tools::db_parser::a_cached_db.s_file_details.size();
			// Update text and progress bar.
			ui.label->setText(QString::fromStdString("Currently Processed " + std::to_string(lDone) + " / " + std::to_string(lTotal) + " Beatmaps"));
			if (lTotal>0 && lDone>0)
				ui.progressBar->setValue(lDone * 100 / lTotal);
			// Force process UI events
			QCoreApplication::processEvents();
			Sleep(10);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}

// Destructor for UI.
dbprogress::~dbprogress()
{
}

// Begin processing of DB. osu! must be open
// for this function to successfully run.
void dbprogress::perform()
{
	// Get directory of osu! and append db name.
	auto lPath = osu_tools::func::get_osu_directory();
	lPath += "osu!.db";
	// Create a new thread to parse the DB.
	mDbThread = new std::thread(osu_tools::db_parser::parse_osu_db, lPath, true);
	// Join thread.
	mDbThread->join();
	Sleep(250);
	// Complete and exit function.
	mComplete = true;
}
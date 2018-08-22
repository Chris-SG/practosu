#pragma once

#include <QWidget>
#include "ui_dbprogress.h"
#include "db_parser.hpp"

// Progress bar display for loading all beatmaps the user has.
class dbprogress : public QWidget
{
	Q_OBJECT

public:
	// Constructor for widget.
	dbprogress(QWidget *parent = Q_NULLPTR);
	// Destructor for widget.
	~dbprogress();

	// bool used for when beatmap processing is complete.
	bool mComplete = false;

private:
	// ui for widget.
	Ui::dbprogress ui;
	// Thread for db processing.
	std::thread * mDbThread;
	// Thread for progress bar.
	std::thread * mProgressThread;

	// Perform processing of db.
	void perform();
};

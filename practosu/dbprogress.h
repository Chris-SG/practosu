#pragma once

#include <QWidget>
#include "ui_dbprogress.h"
#include "db_parser.hpp"

class dbprogress : public QWidget
{
	Q_OBJECT

public:
	dbprogress(QWidget *parent = Q_NULLPTR);
	~dbprogress();

	bool mComplete = false;

private:
	Ui::dbprogress ui;
	std::thread * mDbThread;
	std::thread * mProgressThread;

	void perform();
};

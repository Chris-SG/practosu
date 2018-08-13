#pragma once

#include <QWidget>
#include "ui_presetsmanager.h"

class presetsmanager : public QWidget
{
	Q_OBJECT

public:
	presetsmanager(QWidget *parent = Q_NULLPTR);
	~presetsmanager();

private:
	Ui::presetsmanager ui;
	void savePreset();
};

#pragma once

#include <QWidget>
#include "ui_presetsmanager.h"

// presetsmanager handles the UI for preset handling.
class presetsmanager : public QWidget
{
	Q_OBJECT

public:
	// Constructor for UI
	presetsmanager(QWidget *parent = Q_NULLPTR);
	// Destructor.
	~presetsmanager();

private:
	// UI type.
	Ui::presetsmanager ui;
	// Save preset created.
	void savePreset();
};

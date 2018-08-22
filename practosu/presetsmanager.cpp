#include "stdafx.h"
#include "presets.hpp"
#include "presetsmanager.h"

presetsmanager::presetsmanager(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	// Connect save preset button.
	connect(ui.savePreset, &QPushButton::clicked, this, &presetsmanager::savePreset);
}

presetsmanager::~presetsmanager()
{
}

void presetsmanager::savePreset()
{
	if(ui.lineEdit->text().isEmpty())
	{
		// Every preset must contain a name.
		QMessageBox lMsg;
		lMsg.setText("Please enter a preset name!");
		lMsg.setStandardButtons(QMessageBox::Ok);
		lMsg.exec();
		return;
	}
	else
	{
		// No proper checking on values. May cause errors!
		preset lPreset;
		lPreset.sPresetName = ui.lineEdit->text().toStdString();
		if (ui.hpCheck->isChecked())
			lPreset.sHP = static_cast<int>(round(std::stof(ui.hpText->text().toStdString()) * 10));
		if (ui.csCheck->isChecked())
			lPreset.sCS = static_cast<int>(round(std::stof(ui.csText->text().toStdString()) * 10));
		if (ui.odCheck->isChecked())
			lPreset.sOD = static_cast<int>(round(std::stof(ui.odText->text().toStdString()) * 10));
		if (ui.arCheck->isChecked())
			lPreset.sAR = static_cast<int>(round(std::stof(ui.arText->text().toStdString()) * 10));
		if (ui.creatorCheck->isChecked())
			lPreset.sCreator = ui.creatorName->text().toStdString();
		if (ui.fileNameCheck->isChecked())
			lPreset.sFilename = ui.fileName->text().toStdString();
		if (ui.versionCheck->isChecked())
			lPreset.sVersion = ui.versionName->text().toStdString();
		if (ui.speedCheck->isChecked())
			lPreset.sSpeed = std::stof(ui.multiplier->text().toStdString());
		presets::writePreset(lPreset);
		close();
	}
}
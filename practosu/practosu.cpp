#include "stdafx.h"
#include "practosu.h"
#include "func.hpp"
#include "db_parser.hpp"
#include "dbprogress.h"
#include "presets.hpp"

#include <sstream>
#include "file_writer.hpp"

namespace fs = std::experimental::filesystem;

practosu::practosu(QWidget *parent)
	: QMainWindow(parent)
{
	dbprogress * lWdg = new dbprogress;
	while (!lWdg->mComplete)
		Sleep(1000);
	delete lWdg;
	ui.setupUi(this);
	setFixedSize(700, 460);

	//auto lDv = new QDoubleValidator(0.1, 10.0, 3);
	//ui.speedText->setValidator(lDv);

	mPolling = new QThread(this);
	connect(mPolling, SIGNAL(keyPolled()), this, SLOT(loadMap()));

	connect(ui.loadMap, &QPushButton::clicked, this, &practosu::selectFile);
	connect(ui.reloadMap, &QPushButton::clicked, this, &practosu::loadSelectedMap);

	connect(ui.hpSlider, &QAbstractSlider::valueChanged, this, &practosu::updateHP);
	connect(ui.csSlider, &QAbstractSlider::valueChanged, this, &practosu::updateCS);
	connect(ui.odSlider, &QAbstractSlider::valueChanged, this, &practosu::updateOD);
	connect(ui.arSlider, &QAbstractSlider::valueChanged, this, &practosu::updateAR);
	connect(ui.writeFile, &QPushButton::clicked, this, &practosu::writeFile);
	connect(ui.speedText, &QLineEdit::textChanged, this, &practosu::updateAudio);

	connect(ui.presetsList, SIGNAL(activated(int)), this, SLOT(loadPreset()));
}

void practosu::loadSelectedMap()
{
	try
	{
		loadMap(osu_tools::func::get_current_beatmap());
	}
	catch(std::exception& e)
	{
		QMessageBox lMsg;
		lMsg.setText("Error loading map");
		lMsg.setInformativeText(e.what());
		lMsg.setStandardButtons(QMessageBox::Ok);
		lMsg.exec();
	}
}

void practosu::pressLoadMap()
{
	ui.reloadMap->click();
}

void practosu::loadPresetList()
{
	auto lPresets = presets::presetNames();
	ui.presetsList->clear();
	for (auto lPreset : lPresets)
		ui.presetsList->addItem(QString::fromStdString(lPreset));
}

void practosu::loadPreset()
{
	preset lPreset = presets::getPresetByName(ui.presetsList->currentText().toStdString());
	if (lPreset.sPresetName.empty())
		return;
	if (!lPreset.sFilename.empty())
		ui.fileText->setText(QString::fromStdString(lPreset.sFilename));
	if (!lPreset.sVersion.empty())
		ui.versionText->setText(QString::fromStdString(lPreset.sVersion));
	if (!lPreset.sCreator.empty())
		ui.creatorText->setText(QString::fromStdString(lPreset.sCreator));
	if(mCurrentMap.sBeatmapVersion>=13)
	{
		if (lPreset.sAR != -1)
			ui.arSlider->setValue(std::round(lPreset.sAR / 10));
		if (lPreset.sCS != -1)
			ui.csSlider->setValue(std::round(lPreset.sAR / 10));
		if (lPreset.sOD != -1)
			ui.odSlider->setValue(std::round(lPreset.sAR / 10));
		if (lPreset.sHP != -1)
			ui.hpSlider->setValue(std::round(lPreset.sAR / 10));
	}
	else
	{
		if (lPreset.sAR != -1)
			ui.arSlider->setValue(lPreset.sAR);
		if (lPreset.sCS != -1)
			ui.csSlider->setValue(lPreset.sAR);
		if (lPreset.sOD != -1)
			ui.odSlider->setValue(lPreset.sAR);
		if (lPreset.sHP != -1)
			ui.hpSlider->setValue(lPreset.sAR);
	}
	if (lPreset.sSpeed != -1.0)
		ui.speedText->setText(QString::number(lPreset.sSpeed));
}

void practosu::loadMap(fs::path aPath)
{
	mCurrentMap = osu_tools::file_parser::parse_osu_file(aPath);
	ui.currentMap->setText(QString::fromStdString(mCurrentMap.sFileName));
	ui.audioText->setText(QString::fromStdString(mCurrentMap.sAudioFilename));
	ui.audioText->setReadOnly(true);
	
	ui.fileText->setText(QString::fromStdString("%NAME%"));
	ui.creatorText->setText(QString::fromStdString("practosu"));
	ui.versionText->setText(QString::fromStdString(mCurrentMap.sVersion));
	if(mCurrentMap.sBeatmapVersion >= 13)
	{
		ui.hpSlider->setMaximum(100);
		ui.csSlider->setMaximum(100);
		ui.odSlider->setMaximum(100);
		ui.arSlider->setMaximum(100);
		ui.hpSlider->setValue(mCurrentMap.sHPDrainRate * 10);
		ui.csSlider->setValue(mCurrentMap.sCircleSize * 10);
		ui.odSlider->setValue(mCurrentMap.sOverallDifficulty * 10);
		if (mCurrentMap.sApproachRate == -1.0)
			ui.arSlider->setDisabled(true);
		else
		{
			ui.arSlider->setDisabled(false);
			ui.arSlider->setValue(mCurrentMap.sApproachRate * 10);
		}
	}
	else
	{
		ui.hpSlider->setMaximum(10);
		ui.csSlider->setMaximum(10);
		ui.odSlider->setMaximum(10);
		ui.arSlider->setMaximum(10);
		ui.hpSlider->setValue(mCurrentMap.sHPDrainRate);
		ui.csSlider->setValue(mCurrentMap.sCircleSize);
		ui.odSlider->setValue(mCurrentMap.sOverallDifficulty);
		if (mCurrentMap.sApproachRate == -1.0)
			ui.arSlider->setDisabled(true);
		else
		{
			ui.arSlider->setDisabled(false);
			ui.arSlider->setValue(mCurrentMap.sApproachRate);
		}
	}

	ui.speedText->setText("1.0");
}

void practosu::writeFile()
{
	try
	{
		//mCurrentMap.sFileName = ui.fileText->text().toStdString();
		mCurrentMap.sCreator = ui.creatorText->text().toStdString();
		mCurrentMap.sVersion = ui.versionText->text().toStdString();

		if (std::stof(ui.speedText->text().toStdString()) == 1.0)
			osu_tools::file_writer::write_file(mCurrentMap, ui.fileText->text().toStdString(), ui.audioText->text().toStdString());
		else
			osu_tools::file_writer::write_file(mCurrentMap, ui.fileText->text().toStdString(), ui.audioText->text().toStdString(), std::stof(ui.speedText->text().toStdString()));
	}
	catch (std::exception& e)
	{
		QMessageBox lMsg;
		lMsg.setText("Error saving map");
		lMsg.setInformativeText(e.what());
		lMsg.setStandardButtons(QMessageBox::Ok);
		lMsg.exec();
	}
}

void practosu::updateAudio()
{
	if (std::stof(ui.speedText->text().toStdString()) == 1.0)
		ui.audioText->setText(QString::fromStdString(mCurrentMap.sAudioFilename));
	else
	{
		std::string lAudioName = "[practosu speed " + ui.speedText->text().toStdString() + "] - " + mCurrentMap.sAudioFilename;
		ui.audioText->setText(QString::fromStdString(lAudioName));
	}
}

void practosu::selectFile()
{
	auto lPath = osu_tools::func::get_beatmap_directory();
	QString lDirectory = QFileDialog::getOpenFileName(this, tr("Select .osu file"), QString::fromStdString(lPath.string()), (".osu Files (*.osu)"));
	if(!lDirectory.isEmpty())
	{
		loadMap(fs::path(lDirectory.toStdString()));
	}
}

void practosu::updateHP()
{
	auto lPosition = ui.hpSlider->value();

	std::stringstream lStream;
	lStream << std::fixed << std::setprecision(1) << static_cast<double>(lPosition) / (ui.hpSlider->maximum() / 10.0);
	ui.hpLabel->setText(QString::fromStdString(lStream.str()));
	mCurrentMap.sHPDrainRate = stof(lStream.str());
}

void practosu::updateCS()
{
	auto lPosition = ui.csSlider->value();

	std::stringstream lStream;
	lStream << std::fixed << std::setprecision(1) << static_cast<double>(lPosition) / (ui.csSlider->maximum() / 10.0);
	ui.csLabel->setText(QString::fromStdString(lStream.str()));
	mCurrentMap.sCircleSize = stof(lStream.str());
}

void practosu::updateOD()
{
	auto lPosition = ui.odSlider->value();

	std::stringstream lStream;
	lStream << std::fixed << std::setprecision(1) << static_cast<double>(lPosition) / (ui.odSlider->maximum() / 10.0);
	ui.odLabel->setText(QString::fromStdString(lStream.str()));
	mCurrentMap.sOverallDifficulty = stof(lStream.str());
}

void practosu::updateAR()
{
	auto lPosition = ui.arSlider->value();

	std::stringstream lStream;
	lStream << std::fixed << std::setprecision(1) << static_cast<double>(lPosition) / (ui.arSlider->maximum() / 10.0);
	ui.arLabel->setText(QString::fromStdString(lStream.str()));
	mCurrentMap.sApproachRate = stof(lStream.str());
}

void poll();
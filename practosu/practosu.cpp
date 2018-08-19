#include "stdafx.h"
#include "practosu.h"
#include "func.hpp"
#include "db_parser.hpp"
#include "dbprogress.h"
#include "presets.hpp"
#include "presetsmanager.h"

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

	connect(ui.loadMap, &QPushButton::clicked, this, &practosu::selectFile);
	connect(ui.reloadMap, &QPushButton::clicked, this, &practosu::loadSelectedMap);
	connect(ui.hpSlider, &QAbstractSlider::valueChanged, this, &practosu::updateHP);
	connect(ui.csSlider, &QAbstractSlider::valueChanged, this, &practosu::updateCS);
	connect(ui.odSlider, &QAbstractSlider::valueChanged, this, &practosu::updateOD);
	connect(ui.arSlider, &QAbstractSlider::valueChanged, this, &practosu::updateAR);

	connect(ui.writeFile, &QPushButton::clicked, this, &practosu::writeFile);
	connect(ui.speedText, &QLineEdit::textChanged, this, &practosu::updateAudio);
	connect(ui.editPresets, &QPushButton::clicked, this, &practosu::editPresets);

	connect(ui.actionQuit, &QAction::triggered, this, &practosu::close);

	connect(ui.presetsList, &QComboBox::currentTextChanged, this, &practosu::loadPreset);

	ui.label_11->setTextInteractionFlags(Qt::TextBrowserInteraction);
	ui.label_11->setOpenExternalLinks(true);
	ui.success->setVisible(false);
	
	loadPresetList();
}

void practosu::loadSelectedMap()
{
	try
	{
		loadMap(osu_tools::func::get_current_beatmap());
		loadPresetList();
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
	ui.presetsList->addItem("");
	for (auto lPreset : lPresets)
		ui.presetsList->addItem(QString::fromStdString(lPreset));
	ui.presetsList->setCurrentIndex(0);
}

void practosu::resetUI()
{
	clearCurrentMap();

	ui.currentMap->setText("");
	ui.audioText->setText("");
	ui.fileText->setText("");
	ui.creatorText->setText("");
	ui.versionText->setText("");

	ui.presetsList->setCurrentIndex(0);

	ui.hpSlider->setValue(0);
	ui.csSlider->setValue(0);
	ui.odSlider->setValue(0);
	ui.arSlider->setValue(0);
	ui.speedText->setText("");
}

void practosu::clearCurrentMap()
{
	for (std::vector<hit_object*>::iterator it = mCurrentMap.sHitObjects.begin(); it != mCurrentMap.sHitObjects.end(); ++it)
		delete(*it);
	mCurrentMap.sHitObjects.clear();
	mCurrentMap = mEmptyMap;
}

void practosu::loadPreset()
{
	if (ui.presetsList->currentIndex() == 0)
		return;
	preset lPreset = presets::getPresetByName(ui.presetsList->currentText().toStdString());
	if (lPreset.sPresetName.empty())
		return;
	if (!lPreset.sFilename.empty())
		ui.fileText->setText(QString::fromStdString(lPreset.sFilename));
	if (!lPreset.sVersion.empty())
		ui.versionText->setText(QString::fromStdString(lPreset.sVersion));
	if (!lPreset.sCreator.empty())
		ui.creatorText->setText(QString::fromStdString(lPreset.sCreator));
	if(mCurrentMap.sBeatmapVersion<13)
	{
		if (lPreset.sAR != -1)
			ui.arSlider->setValue(std::round(lPreset.sAR / 10));
		if (lPreset.sCS != -1)
			ui.csSlider->setValue(std::round(lPreset.sCS / 10));
		if (lPreset.sOD != -1)
			ui.odSlider->setValue(std::round(lPreset.sOD / 10));
		if (lPreset.sHP != -1)
			ui.hpSlider->setValue(std::round(lPreset.sHP / 10));
	}
	else
	{
		if (lPreset.sAR != -1)
			ui.arSlider->setValue(lPreset.sAR);
		if (lPreset.sCS != -1)
			ui.csSlider->setValue(lPreset.sCS);
		if (lPreset.sOD != -1)
			ui.odSlider->setValue(lPreset.sOD);
		if (lPreset.sHP != -1)
			ui.hpSlider->setValue(lPreset.sHP);
	}
	if (lPreset.sSpeed != -1.0)
		ui.speedText->setText(QString::number(lPreset.sSpeed));
}

void practosu::editPresets()
{
	presetsmanager * lMgr = new presetsmanager;
	lMgr->show();
	lMgr->setAttribute(Qt::WA_DeleteOnClose);
	connect(lMgr, &QWidget::destroyed, this, &practosu::loadPresetList);
}

void practosu::loadMap(fs::path aPath)
{
	clearCurrentMap();
	ui.success->setVisible(false);

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
		ui.hpSlider->setValue(static_cast<int>(mCurrentMap.sHPDrainRate));
		ui.csSlider->setValue(static_cast<int>(mCurrentMap.sCircleSize));
		ui.odSlider->setValue(static_cast<int>(mCurrentMap.sOverallDifficulty));
		if (mCurrentMap.sApproachRate == -1.0)
			ui.arSlider->setDisabled(true);
		else
		{
			ui.arSlider->setDisabled(false);
			ui.arSlider->setValue(static_cast<int>(mCurrentMap.sApproachRate));
		}
	}

	ui.speedText->setText("1.0");
}

void practosu::writeFile()
{
	try
	{
		mCurrentMap.sApproachRate = std::stof(ui.arLabel->text().toStdString());
		mCurrentMap.sOverallDifficulty = std::stof(ui.odLabel->text().toStdString());
		mCurrentMap.sHPDrainRate = std::stof(ui.hpLabel->text().toStdString());
		mCurrentMap.sCircleSize = std::stof(ui.csLabel->text().toStdString());
		//mCurrentMap.sFileName = ui.fileText->text().toStdString();
		mCurrentMap.sCreator = ui.creatorText->text().toStdString();

		if (std::stof(ui.speedText->text().toStdString()) == 1.0)
			osu_tools::file_writer::write_file(mCurrentMap, ui.fileText->text().toStdString(), ui.audioText->text().toStdString(), ui.versionText->text().toStdString());
		else
			osu_tools::file_writer::write_file(mCurrentMap, ui.fileText->text().toStdString(), ui.audioText->text().toStdString(), ui.versionText->text().toStdString(), std::stof(ui.speedText->text().toStdString()));
		resetUI();
		ui.success->setVisible(true);
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
	if(ui.speedText->text().isEmpty())
		ui.audioText->setText(QString::fromStdString(mCurrentMap.sAudioFilename));
	else if (std::stof(ui.speedText->text().toStdString()) == 1.0)
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
}

void practosu::updateCS()
{
	auto lPosition = ui.csSlider->value();

	std::stringstream lStream;
	lStream << std::fixed << std::setprecision(1) << static_cast<double>(lPosition) / (ui.csSlider->maximum() / 10.0);
	ui.csLabel->setText(QString::fromStdString(lStream.str()));
}

void practosu::updateOD()
{
	auto lPosition = ui.odSlider->value();

	std::stringstream lStream;
	lStream << std::fixed << std::setprecision(1) << static_cast<double>(lPosition) / (ui.odSlider->maximum() / 10.0);
	ui.odLabel->setText(QString::fromStdString(lStream.str()));
}

void practosu::updateAR()
{
	auto lPosition = ui.arSlider->value();

	std::stringstream lStream;
	lStream << std::fixed << std::setprecision(1) << static_cast<double>(lPosition) / (ui.arSlider->maximum() / 10.0);
	ui.arLabel->setText(QString::fromStdString(lStream.str()));
}

void poll();
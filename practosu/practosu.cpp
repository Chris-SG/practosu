#include "stdafx.h"
#include "practosu.h"
#include "func.hpp"
#include "db_parser.hpp"
#include "dbprogress.h"
#include "presets.hpp"
#include "presetsmanager.h"
#include "audiocleaner.h"

#include <sstream>
#include "file_writer.hpp"

namespace fs = std::experimental::filesystem;

practosu::practosu(QWidget *parent)
	: QMainWindow(parent)
{
	reloadDb();
	ui.setupUi(this);
	setFixedSize(700, 460);

	//auto lDv = new QDoubleValidator(0.1, 10.0, 3);
	//ui.speedText->setValidator(lDv);

	// Create all object connections.
	connect(ui.loadMap, &QPushButton::clicked, this, &practosu::selectFile);
	connect(ui.reloadMap, &QPushButton::clicked, this, &practosu::loadSelectedMap);
	connect(ui.hpSlider, &QAbstractSlider::valueChanged, this, &practosu::updateHP);
	connect(ui.csSlider, &QAbstractSlider::valueChanged, this, &practosu::updateCS);
	connect(ui.odSlider, &QAbstractSlider::valueChanged, this, &practosu::updateOD);
	connect(ui.arSlider, &QAbstractSlider::valueChanged, this, &practosu::updateAR);
	connect(ui.reloadDb, &QPushButton::clicked, this, &practosu::reloadDb);

	connect(ui.writeFile, &QPushButton::clicked, this, &practosu::writeFile);
	connect(ui.speedText, &QLineEdit::textChanged, this, &practosu::updateAudio);
	connect(ui.editPresets, &QPushButton::clicked, this, &practosu::editPresets);

	connect(ui.actionQuit, &QAction::triggered, this, &practosu::close);
	connect(ui.audioFileCleaner, &QAction::triggered, this, &practosu::audioCleaner);
	connect(ui.manualPointerSearch, &QAction::triggered, this, &practosu::manualPointerSearch);

	connect(ui.presetsList, &QComboBox::currentTextChanged, this, &practosu::loadPreset);
	connect(ui.versionChange, &QCheckBox::stateChanged, this, &practosu::updateSliders);

	ui.label_11->setTextInteractionFlags(Qt::TextBrowserInteraction);
	ui.label_11->setOpenExternalLinks(true);
	ui.success->setVisible(false);
	
	loadPresetList();
}

void practosu::loadSelectedMap()
{
	try
	{
		// Load currently selected beatmap.
		loadMap(osu_tools::func::get_current_beatmap());
		// Load presets.
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
	// Simulate click.
	ui.reloadMap->click();
}

void practosu::loadPresetList()
{
	// Load all presets and get names.
	auto lPresets = presets::presetNames();
	ui.presetsList->clear();
	// Add blank item.
	ui.presetsList->addItem("");
	// Add all presets to list.
	for (auto lPreset : lPresets)
		ui.presetsList->addItem(QString::fromStdString(lPreset));
	ui.presetsList->setCurrentIndex(0);
}

void practosu::resetUI()
{
	// Reset UI to empty.
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

	ui.versionChange->setChecked(false);
}

void practosu::clearCurrentMap()
{
	// Clear currently loaded beatmap.
	for (std::vector<hit_object*>::iterator it = mCurrentMap.s_hit_objects.begin(); it != mCurrentMap.s_hit_objects.end(); ++it)
		delete(*it);
	mCurrentMap.s_hit_objects.clear();
	mCurrentMap = mEmptyMap;
}

void practosu::loadPreset()
{
	try
	{
		// First index is always empty, so ignore.
		if (ui.presetsList->currentIndex() == 0)
			return;
		// Load preset by name.
		preset lPreset = presets::getPresetByName(ui.presetsList->currentText().toStdString());
		if (lPreset.sPresetName.empty())
			return;
		if (!lPreset.sFilename.empty())
			ui.fileText->setText(QString::fromStdString(lPreset.sFilename));
		if (!lPreset.sVersion.empty())
			ui.versionText->setText(QString::fromStdString(lPreset.sVersion));
		if (!lPreset.sCreator.empty())
			ui.creatorText->setText(QString::fromStdString(lPreset.sCreator));
		// Version before 13 doesn't support decimal values.
		if (mCurrentMap.s_beatmap_version<13)
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
	catch(std::exception& e)
	{
		QMessageBox lMsg;
		lMsg.setText("Error loading preset");
		lMsg.setInformativeText(e.what());
		lMsg.setStandardButtons(QMessageBox::Ok);
		lMsg.exec();
	}
}

void practosu::editPresets()
{
	// Open new preset menu.
	presetsmanager * lMgr = new presetsmanager;
	lMgr->show();
	lMgr->setAttribute(Qt::WA_DeleteOnClose);
	connect(lMgr, &QWidget::destroyed, this, &practosu::loadPresetList);
}

void practosu::audioCleaner()
{
	// Confirm user wants to run audio cleaner. It is an expensive
	// operation.
	QMessageBox l_box;
	l_box.setText("Are you sure?");
	l_box.setInformativeText("This is an expensive operation and may take several minutes if you have many beatmaps. In addition, the program will become unresponsive while loading. Use with caution!");
	l_box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	l_box.setDefaultButton(QMessageBox::Ok);
	auto l_result = l_box.exec();

	if(l_result == QMessageBox::Ok)
	{
		audiocleaner * lCleaner = new audiocleaner;
		lCleaner->show();
		lCleaner->setAttribute(Qt::WA_DeleteOnClose);
	}
}

void practosu::loadMap(fs::path aPath)
{
	try
	{	
		clearCurrentMap();
		ui.success->setVisible(false);

		mCurrentMap = osu_tools::file_parser::parse_osu_file(aPath);
		ui.currentMap->setText(QString::fromStdString(mCurrentMap.s_file_name));
		ui.audioText->setText(QString::fromStdString(mCurrentMap.s_audio_filename));
		ui.audioText->setReadOnly(true);
		
		ui.fileText->setText(QString::fromStdString("%NAME%"));
		ui.creatorText->setText(QString::fromStdString("practosu"));
		ui.versionText->setText(QString::fromStdString(mCurrentMap.s_version));
		setVersion(mCurrentMap.s_beatmap_version);

		ui.speedText->setText("1.0");
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

void practosu::updateSliders()
{
	// Update beatmap version and sliders.
	if(ui.versionChange->isChecked())
	{
		setVersion(14);
	}
}

void practosu::writeFile()
{
	try
	{
		// Prepare writing of file.
		mCurrentMap.s_approach_rate = std::stof(ui.arLabel->text().toStdString());
		mCurrentMap.s_overall_difficulty = std::stof(ui.odLabel->text().toStdString());
		mCurrentMap.s_hp_drain_rate = std::stof(ui.hpLabel->text().toStdString());
		mCurrentMap.s_circle_size = std::stof(ui.csLabel->text().toStdString());
		mCurrentMap.s_creator = ui.creatorText->text().toStdString();
		if (ui.versionChange->isChecked())
			mCurrentMap.s_beatmap_version = 14;

		if (std::stof(ui.speedText->text().toStdString()) == 1.0)
			osu_tools::file_writer::write_file(mCurrentMap, ui.fileText->text().toStdString(), ui.audioText->text().toStdString(), ui.versionText->text().toStdString());
		else
			osu_tools::file_writer::write_file(mCurrentMap, ui.fileText->text().toStdString(), ui.audioText->text().toStdString(), ui.versionText->text().toStdString(), std::stof(ui.speedText->text().toStdString()));
		// Reset UI after writing file.
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
	// Update audio filename.
	if(ui.speedText->text().isEmpty())
		ui.audioText->setText(QString::fromStdString(mCurrentMap.s_audio_filename));
	else if (std::stof(ui.speedText->text().toStdString()) == 1.0)
		ui.audioText->setText(QString::fromStdString(mCurrentMap.s_audio_filename));
	else
	{
		std::string lAudioName = "[practosu speed " + ui.speedText->text().toStdString() + "] - " + mCurrentMap.s_audio_filename;
		ui.audioText->setText(QString::fromStdString(lAudioName));
	}
}

void practosu::selectFile()
{
	// Create selection dialog.
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

void practosu::setVersion(uint8_t a_version)
{
	mCurrentMap.s_beatmap_version = a_version;
	// Version 13 and later support decimal values.
	if (mCurrentMap.s_beatmap_version >= 13)
	{
		ui.hpSlider->setMaximum(100);
		ui.csSlider->setMaximum(100);
		ui.odSlider->setMaximum(100);
		ui.arSlider->setMaximum(100);
		ui.hpSlider->setValue(mCurrentMap.s_hp_drain_rate * 10);
		ui.csSlider->setValue(mCurrentMap.s_circle_size * 10);
		ui.odSlider->setValue(mCurrentMap.s_overall_difficulty * 10);
		if (mCurrentMap.s_approach_rate == -1.0)
			mCurrentMap.s_approach_rate = mCurrentMap.s_overall_difficulty;
		ui.arSlider->setDisabled(false);
		ui.arSlider->setValue(mCurrentMap.s_approach_rate * 10);
	}
	else
	{
		ui.hpSlider->setMaximum(10);
		ui.csSlider->setMaximum(10);
		ui.odSlider->setMaximum(10);
		ui.arSlider->setMaximum(10);
		ui.hpSlider->setValue(static_cast<int>(mCurrentMap.s_hp_drain_rate));
		ui.csSlider->setValue(static_cast<int>(mCurrentMap.s_circle_size));
		ui.odSlider->setValue(static_cast<int>(mCurrentMap.s_overall_difficulty));
		// Approach Rate was originally tied to OD.
		if (mCurrentMap.s_approach_rate == -1.0)
			ui.arSlider->setDisabled(true);
		else
		{
			ui.arSlider->setDisabled(false);
			ui.arSlider->setValue(static_cast<int>(mCurrentMap.s_approach_rate));
		}
	}
}

void practosu::manualPointerSearch()
{
	// Confirm user wants to run pointer search.
	QMessageBox l_box;
	l_box.setText("Are you sure?");
	l_box.setInformativeText("Only use this if automatic beatmap searching (F11) is not working!");
	l_box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	l_box.setDefaultButton(QMessageBox::Ok);
	auto l_result = l_box.exec();

	if (l_result == QMessageBox::Ok)
	{
		QMessageBox l_box_2;
		// Perform first part of manual search.
		l_box_2.setText("Part One");
		l_box_2.setInformativeText("Please ensure you have <a href=\"osu://dl/163112\">Kuba Oms - My Love (W h i t e)</a> downloaded.</br>When ready, select the 'Normal' difficulty and click Ok.");
		l_box_2.setStandardButtons(QMessageBox::Ok);
		l_box_2.setDefaultButton(QMessageBox::Ok);
		auto l_result = l_box_2.exec();
		osu_tools::func::backup_pointer_search();
		// Perform second part of manual search.
		l_box_2.setText("Part Two");
		l_box_2.setInformativeText("Thanks! Now change to the 'Hard' difficulty and click Ok again.");
		l_result = l_box_2.exec();
		osu_tools::func::backup_pointer_search();
		// Perform third part of manual search.
		l_box_2.setText("Part Three");
		l_box_2.setInformativeText("Just one more! Change to the 'Insane' difficulty and click Ok!");
		l_result = l_box_2.exec();
		osu_tools::func::backup_pointer_search();
	}
}

void practosu::reloadDb()
{
	dbprogress * lWdg = new dbprogress;
	while (!lWdg->mComplete)
		Sleep(1000);
	delete lWdg;
}
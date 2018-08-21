#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_practosu.h"

#include "file_parser.hpp"

class practosu : public QMainWindow
{
	Q_OBJECT

public:
	practosu(QWidget *parent = Q_NULLPTR);
	void loadSelectedMap();
	void loadMap(std::experimental::filesystem::path aPath);
	void pressLoadMap();


private:
	osu_file mCurrentMap;
	const struct osu_file mEmptyMap;

	Ui::practosuClass ui;
private slots:
	void selectFile();
	void writeFile();
	void updateAudio();

	void updateHP();
	void updateCS();
	void updateOD();
	void updateAR();

	void loadPresetList();
	void loadPreset();
	void editPresets();

	void resetUI();
	void clearCurrentMap();

	void audioCleaner();
};


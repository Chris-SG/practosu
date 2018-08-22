#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_practosu.h"

#include "file_parser.hpp"

// Main window used by practosu.
class practosu : public QMainWindow
{
	Q_OBJECT

public:
	// Constructor
	practosu(QWidget *parent = Q_NULLPTR);
	// Load currently selected beatmap.
	void loadSelectedMap();
	// Load beatmap using a file path.
	void loadMap(std::experimental::filesystem::path aPath);
	// Simulate a press on the LoadMap button.
	void pressLoadMap();


private:
	// Current loaded beatmap.
	osu_file mCurrentMap;
	// Empty beatmap struct.
	const struct osu_file mEmptyMap;

	// ui for main window.
	Ui::practosuClass ui;
private slots:
	// Used to create a select file dialogue for manual
	// beatmap selection.
	void selectFile();

	// Write the beatmap file based on the values changed
	// in the UI.
	void writeFile();
	// Update the audio filename label based on chosen beatmap speed.
	void updateAudio();

	// Update label for HP.
	void updateHP();
	// Update label for CS.
	void updateCS();
	// Update label for OD.
	void updateOD();
	// Update label for AR.
	void updateAR();

	// Load presets from file to display in dropdown box.
	void loadPresetList();
	// Load specific preset from list.
	void loadPreset();
	// Used to add new presets to the list.
	void editPresets();

	// Reset UI to empty state.
	void resetUI();
	// Clear current osu_file struct.
	void clearCurrentMap();

	// Run audio cleaner widget.
	void audioCleaner();
	// Update HP/CS/OD/AR sliders.
	void updateSliders();
	// Set beatmap version.
	void setVersion(uint8_t a_version);
};


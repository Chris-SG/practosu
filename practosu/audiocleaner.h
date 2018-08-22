#pragma once

#include <QWidget>
#include "ui_audiocleaner.h"

// audiocleaner is used to clear any leftover audio files from
// previously deleted beatmaps that utilized speed changes. This
// is done by applying a prefix to the created audio files. These
// values are hardcoded and non-transferable.
class audiocleaner : public QWidget
{
	Q_OBJECT

public:
	// Constructor for widget.
	audiocleaner(QWidget *parent = Q_NULLPTR);
	// Destructor for widget.
	~audiocleaner();

private:
	// ui object to use.
	Ui::audiocleaner ui;
	// std::vector of all name-matching audio files found.
	std::vector<std::pair<std::experimental::filesystem::path, std::string>> m_audio_files;

	// Load all matching audio files into the m_audio_files std::vector.
	void loadAudioFiles();
	// Clear the m_audio_files std::vector.
	void clearAudioFiles();

	// Update currently selected audio file in table view.
	void updateSelection();
	// Clear any inactive audio files, deleting them permanantly from the harddrive.
	void clearInactive();
};

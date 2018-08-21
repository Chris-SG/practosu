#pragma once

#include <QWidget>
#include "ui_audiocleaner.h"

class audiocleaner : public QWidget
{
	Q_OBJECT

public:
	audiocleaner(QWidget *parent = Q_NULLPTR);
	~audiocleaner();

private:
	Ui::audiocleaner ui;
	std::vector<std::pair<std::experimental::filesystem::path, std::string>> m_audio_files;

	void loadAudioFiles();
	void clearAudioFiles();

	void updateSelection();
	void clearInactive();
};

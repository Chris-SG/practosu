#include "stdafx.h"
#include "audiocleaner.h"
#include "func.hpp"
#include "file_parser.hpp"

namespace fs = std::experimental::filesystem;

audiocleaner::audiocleaner(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	loadAudioFiles();
	for (std::pair<fs::path, std::string> l_item : m_audio_files)
		ui.songNames->addItem(QString::fromStdString(l_item.second));

	connect(ui.songNames, &QListWidget::itemSelectionChanged, this, &audiocleaner::updateSelection);
	connect(ui.deleteInactive, &QPushButton::clicked, this, &audiocleaner::clearInactive);
	connect(ui.reload, &QPushButton::clicked, this, &audiocleaner::loadAudioFiles);
}

audiocleaner::~audiocleaner()
{
}

void audiocleaner::loadAudioFiles()
{
	m_audio_files.clear();
	ui.songNames->clear();

	fs::path l_path = osu_tools::func::get_beatmap_directory();
	if (!fs::exists(l_path) || !fs::is_directory(l_path))
		return;
	fs::recursive_directory_iterator l_it(l_path);
	fs::recursive_directory_iterator l_endit;

	while(l_it != l_endit)
	{
		if(fs::is_regular_file(*l_it) && l_it->path().extension() == ".mp3")
		{
			std::string l_filename = l_it->path().string().substr(l_it->path().string().find_last_of("\\") + 1);
			if (l_filename.find("[practosu speed") == 0)
				m_audio_files.push_back(std::pair<fs::path, std::string>(l_it->path(), l_filename));
		}
		++l_it;
	}
}

void audiocleaner::clearAudioFiles()
{
	m_audio_files.clear();
	ui.songNames->clear();
}

void audiocleaner::updateSelection()
{
	ui.audioName->setText(ui.songNames->currentItem()->text());
	for(auto l_it = m_audio_files.begin(); l_it != m_audio_files.end();)
	{
		if(l_it->second == ui.songNames->currentItem()->text().toStdString())
		{
			ui.filePath->setText(QString::fromStdString(l_it->first.string()));
			break;
		}

		++l_it;
	}
}

void audiocleaner::clearInactive()
{
	uint32_t l_count = 0;

	for (auto l_it = m_audio_files.begin(); l_it != m_audio_files.end();)
	{
		std::string l_folder_path = l_it->first.string().substr(0, l_it->first.string().find_last_of("\\"));

		fs::directory_iterator l_folder_it(l_folder_path);
		fs::directory_iterator l_endit;
		bool l_use = false;

		while (l_folder_it != l_endit)
		{
			if (fs::is_regular_file(*l_folder_it) && l_folder_it->path().extension() == ".osu")
			{
				auto l_path = l_folder_it->path();
				auto l_file = osu_tools::file_parser::parse_osu_file(l_path);
				if(l_file.s_audio_filename == l_it->second)
				{
					l_use = true;
					break;
				}
			}

			++l_folder_it;
		}
		if(!l_use)
		{
			fs::remove(l_it->first);
			++l_count;
		}

		++l_it;
	}
	QMessageBox l_box;
	l_box.setText("Complete!");
	l_box.setInformativeText(QString::fromStdString(std::string("Cleared " + std::to_string(l_count) + " audio files.")));
	l_box.setStandardButtons(QMessageBox::Ok);
	l_box.setDefaultButton(QMessageBox::Ok);
	l_box.exec();

	clearAudioFiles();
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
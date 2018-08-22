#include "stdafx.h"
#include "audiocleaner.h"
#include "func.hpp"
#include "file_parser.hpp"

namespace fs = std::experimental::filesystem;

audiocleaner::audiocleaner(QWidget *parent)
	: QWidget(parent)
{
	// Load UI and begin loading of audio files.
	ui.setupUi(this);
	loadAudioFiles();
	// Add audio file names to songNames list.
	for (std::pair<fs::path, std::string> l_item : m_audio_files)
		ui.songNames->addItem(QString::fromStdString(l_item.second));

	// Connect change in selected songName.
	connect(ui.songNames, &QListWidget::itemSelectionChanged, this, &audiocleaner::updateSelection);
	// Connect clicking deleteInactive button.
	connect(ui.deleteInactive, &QPushButton::clicked, this, &audiocleaner::clearInactive);
	// Connect clicking reload button.
	connect(ui.reload, &QPushButton::clicked, this, &audiocleaner::loadAudioFiles);
}

audiocleaner::~audiocleaner()
{
}

void audiocleaner::loadAudioFiles()
{
	// Clear current vector m_audio_files as well as list.
	clearAudioFiles();

	// Get current beatmap directory.
	fs::path l_path = osu_tools::func::get_beatmap_directory();
	if (!fs::exists(l_path) || !fs::is_directory(l_path))
		return;
	// Create iterators.
	fs::recursive_directory_iterator l_it(l_path);
	fs::recursive_directory_iterator l_endit;

	// Iterate through entire directory.
	while(l_it != l_endit)
	{
		// If file is an mp3, continue
		if(fs::is_regular_file(*l_it) && l_it->path().extension() == ".mp3")
		{
			// Break off filename.
			std::string l_filename = l_it->path().string().substr(l_it->path().string().find_last_of("\\") + 1);
			// All practosu audio files begin with [practosu speed
			if (l_filename.find("[practosu speed") == 0)
				m_audio_files.push_back(std::pair<fs::path, std::string>(l_it->path(), l_filename));
		}
		++l_it;
	}
}

void audiocleaner::clearAudioFiles()
{
	// Clearing objects.
	m_audio_files.clear();
	ui.songNames->clear();
}

void audiocleaner::updateSelection()
{
	// Occurs when a list item is clicked.
	ui.audioName->setText(ui.songNames->currentItem()->text());
	// Find and display audio name and full path.
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
	// Loop through all audio file paths.
	for (auto l_it = m_audio_files.begin(); l_it != m_audio_files.end();)
	{
		// Get just the folder path.
		std::string l_folder_path = l_it->first.string().substr(0, l_it->first.string().find_last_of("\\"));

		// Iterate through directory, not looking in sub-directories.
		fs::directory_iterator l_folder_it(l_folder_path);
		fs::directory_iterator l_endit;
		bool l_use = false;

		while (l_folder_it != l_endit)
		{
			// Confirm is .osu file
			if (fs::is_regular_file(*l_folder_it) && l_folder_it->path().extension() == ".osu")
			{
				// parse file and check audio_filename.
				auto l_path = l_folder_it->path();
				auto l_file = osu_tools::file_parser::parse_osu_file(l_path);
				// prevent memory leak.
				if(l_file.s_audio_filename == l_it->second)
				{
					l_use = true;
					for (std::vector<hit_object*>::iterator it = l_file.s_hit_objects.begin(); it != l_file.s_hit_objects.end(); ++it)
						delete(*it);
					break;
				}
				for (std::vector<hit_object*>::iterator it = l_file.s_hit_objects.begin(); it != l_file.s_hit_objects.end(); ++it)
					delete(*it);
			}

			++l_folder_it;
		}
		// If file is not used, delete.
		if(!l_use)
		{
			fs::remove(l_it->first);
			++l_count;
		}

		++l_it;
	}

	// Messagebox showing deletion stats.
	QMessageBox l_box;
	l_box.setText("Complete!");
	l_box.setInformativeText(QString::fromStdString(std::string("Cleared " + std::to_string(l_count) + " audio files.")));
	l_box.setStandardButtons(QMessageBox::Ok);
	l_box.setDefaultButton(QMessageBox::Ok);
	l_box.exec();

	clearAudioFiles();
}
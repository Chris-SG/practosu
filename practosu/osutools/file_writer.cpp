#include "stdafx.h"
#include "file_writer.hpp"
#include "func.hpp"
#include "file_structs.hpp"
#include "file_changer.hpp"

#include <sstream>

using namespace std;
namespace fs = std::experimental::filesystem;

namespace osu_tools
{
	namespace file_writer
	{
		static inline void ReplaceAll(std::string &str, const std::string& from, const std::string& to)
		{
			size_t start_pos = 0;
			while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
				str.replace(start_pos, from.length(), to);
				start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
			}
		}

		bool write_file(osu_file& aOsuFile, std::string& aNewFilename, std::string& aNewAudioFilename, std::string& aNewVersion, const float& aMultiplier)
		{
			std::stringstream lSs;

			lSs << std::fixed << std::setprecision(1) << aOsuFile.s_approach_rate;
			ReplaceAll(aNewVersion, "%AR%", lSs.str());
			ReplaceAll(aNewFilename, "%AR%", lSs.str());
			std::stringstream().swap(lSs);

			lSs << std::fixed << std::setprecision(1) << aOsuFile.s_circle_size;
			ReplaceAll(aNewVersion, "%CS%", lSs.str());
			ReplaceAll(aNewFilename, "%CS%", lSs.str());
			std::stringstream().swap(lSs);

			lSs << std::fixed << std::setprecision(1) << aOsuFile.s_overall_difficulty;
			ReplaceAll(aNewVersion, "%OD%", lSs.str());
			ReplaceAll(aNewFilename, "%OD%", lSs.str());
			std::stringstream().swap(lSs);

			lSs << std::fixed << std::setprecision(1) << aOsuFile.s_hp_drain_rate;
			ReplaceAll(aNewVersion, "%HP%", lSs.str());
			ReplaceAll(aNewFilename, "%HP%", lSs.str());
			std::stringstream().swap(lSs);

			string lSpeed = std::to_string(aMultiplier);
			lSpeed.erase(lSpeed.find_last_not_of('0') + 1, std::string::npos);
			ReplaceAll(aNewVersion, "%SPEED%", lSs.str());
			ReplaceAll(aNewFilename, "%SPEED%", lSs.str());

			ReplaceAll(aNewVersion, "%VER%", aOsuFile.s_version);

			ReplaceAll(aNewFilename, "%NAME%", aOsuFile.s_artist + " - " + aOsuFile.s_title + " (" + aOsuFile.s_creator + ") [" + aNewVersion + "]");
			ReplaceAll(aNewFilename, "%VER%", aOsuFile.s_version);
			if (aNewFilename.find(".osu") == std::string::npos)
				aNewFilename += ".osu";
			char lIllegalChars[] = "\\/:*?\"<>|";
			for(uint32_t i = 0; i < strlen(lIllegalChars); ++i)
				aNewFilename.erase(std::remove(aNewFilename.begin(), aNewFilename.end(), lIllegalChars[i]));



			auto lSongPath = osu_tools::func::get_beatmap_directory();
			lSongPath += aOsuFile.s_folder_name;
			lSongPath /= aNewFilename;
			aOsuFile.s_version = aNewVersion;

			// if file already exists, we don't want to recreate it
			if (fs::exists(lSongPath))
				throw std::invalid_argument("Beatmap file name already exists! Please change file name");
			if (aMultiplier != 1.0)
				osu_tools::file_changer::set_speed_multiplier(aMultiplier, aOsuFile, aNewAudioFilename);

			aOsuFile.s_beatmap_id = 0;

			ofstream lFileStream;
			lFileStream.open(lSongPath.string(), ofstream::out | ofstream::app);
			//	fs::create(lSongPath);
			lFileStream << "osu file format v" << static_cast<int>(aOsuFile.s_beatmap_version) << std::endl << std::endl;

			lFileStream << "[General]" << std::endl;
			if (!aOsuFile.s_audio_filename.empty())
				lFileStream << "AudioFilename:" << aOsuFile.s_audio_filename << std::endl;
			if(aOsuFile.s_audio_lead_in != -1)
				lFileStream << "AudioLeadIn:" << aOsuFile.s_audio_lead_in << std::endl;
			if (aOsuFile.s_preview_time != -1)
				lFileStream << "PreviewTime:" << aOsuFile.s_preview_time << std::endl;
			if(aOsuFile.s_countdown != -1)
				lFileStream << "Countdown:" << static_cast<int>(aOsuFile.s_countdown) << std::endl;
			if (!aOsuFile.s_sample_set.empty())
				lFileStream << "SampleSet:" << aOsuFile.s_sample_set << std::endl;
			if (aOsuFile.s_stack_leniency != -1.0)
				lFileStream << "StackLeniency:" << aOsuFile.s_stack_leniency << std::endl; // float x.x
			if (aOsuFile.s_mode != -1)
				lFileStream << "Mode:" << static_cast<int>(aOsuFile.s_mode) << std::endl; // uint8_t
			if(aOsuFile.s_letterbox_in_breaks != -1)
				lFileStream << "LetterboxInBreaks:" << static_cast<int>(aOsuFile.s_letterbox_in_breaks) << std::endl;
			if (!aOsuFile.s_skin_preference.empty())
				lFileStream << "SkinPreference:" << aOsuFile.s_skin_preference << std::endl;
			if(static_cast<int>(aOsuFile.s_epilepsy_warning) != -1)
				lFileStream << "EpilepsyWarning:" << static_cast<int>(aOsuFile.s_epilepsy_warning) << std::endl;
			if(aOsuFile.s_widescreen_storyboard != -1)
				lFileStream << "WidescreenStoryboard:" << static_cast<int>(aOsuFile.s_widescreen_storyboard) << std::endl;
			lFileStream << std::endl;

			lFileStream << "[Editor]" << std::endl;
			if(!aOsuFile.s_bookmarks.empty())
			{
				lFileStream << "Bookmarks:";
				for (uint32_t i = 0; i < aOsuFile.s_bookmarks.size(); ++i)
				{
					lFileStream << aOsuFile.s_bookmarks[i];
					if (i != aOsuFile.s_bookmarks.size() - 1)
						lFileStream << ",";
				}
			}
			if(aOsuFile.s_distance_spacing != -1.0)
				lFileStream << "DistanceSpacing:" << aOsuFile.s_distance_spacing << std::endl;
			if(aOsuFile.s_beat_divisor != -1)
				lFileStream << "BeatDivisor:" << static_cast<int>(aOsuFile.s_beat_divisor) << std::endl;
			if(aOsuFile.s_grid_size != -1)
				lFileStream << "GridSize:" << static_cast<int>(aOsuFile.s_grid_size) << std::endl;
			if(aOsuFile.s_timeline_zoom != -1.0)
				lFileStream << "TimelineZoom:" << aOsuFile.s_timeline_zoom << std::endl; // float x.x
			lFileStream << std::endl;
			

			lFileStream << "[Metadata]" << std::endl;
			if(!aOsuFile.s_title.empty())
				lFileStream << "Title:" << aOsuFile.s_title << std::endl;
			if (!aOsuFile.s_title_unicode.empty())
				lFileStream << "TitleUnicode:" << aOsuFile.s_title_unicode << std::endl;
			if (!aOsuFile.s_artist.empty())
				lFileStream << "Artist:" << aOsuFile.s_artist << std::endl;
			if (!aOsuFile.s_artist_unicode.empty())
				lFileStream << "ArtistUnicode:" << aOsuFile.s_artist_unicode << std::endl;
			if (!aOsuFile.s_creator.empty())
				lFileStream << "Creator:" << aOsuFile.s_creator << std::endl;
			if (!aOsuFile.s_version.empty())
				lFileStream << "Version:" << aOsuFile.s_version << std::endl;
			if (!aOsuFile.s_source.empty())
				lFileStream << "Source:" << aOsuFile.s_source << std::endl;
			if (!aOsuFile.s_tags.empty())
				lFileStream << "Tags:" << aOsuFile.s_tags << std::endl;
			if(aOsuFile.s_beatmap_id != -1)
				lFileStream << "BeatmapId:" << aOsuFile.s_beatmap_id << std::endl;
			if(aOsuFile.s_beatmap_set_id != -1)
				lFileStream << "BeatmapSetID:" << aOsuFile.s_beatmap_set_id << std::endl << std::endl;
			lFileStream << std::endl;

			lFileStream << "[Difficulty]" << std::endl; // all float x.x
			if (aOsuFile.s_hp_drain_rate != -1.0)
				lFileStream << "HPDrainRate:" << aOsuFile.s_hp_drain_rate << std::endl;
			if (aOsuFile.s_circle_size != -1.0)
				lFileStream << "CircleSize:" << aOsuFile.s_circle_size << std::endl;
			if (aOsuFile.s_overall_difficulty != -1.0)
				lFileStream << "OverallDifficulty:" << aOsuFile.s_overall_difficulty << std::endl;
			if (aOsuFile.s_approach_rate != -1.0)
				lFileStream << "ApproachRate:" << aOsuFile.s_approach_rate << std::endl;
			if (aOsuFile.s_slider_multiplier != -1.0)
				lFileStream << "SliderMultiplier:" << aOsuFile.s_slider_multiplier << std::endl; // float x.xx
			if (aOsuFile.s_slider_tick_rate != -1.0)
				lFileStream << "SliderTickRate:" << aOsuFile.s_slider_tick_rate << std::endl << std::endl;

			lFileStream << "[Events]" << std::endl;
			lFileStream << "//Background and Video events" << std::endl;
			if (!aOsuFile.s_background.empty())
			{
				lFileStream << "0,0,\"" << aOsuFile.s_background << "\"";
				if (aOsuFile.s_beatmap_version >= 10)
					lFileStream << ",0,0";
				lFileStream << std::endl;
			}
			lFileStream << "//Break Periods" << std::endl;
			for (const auto& lBreak : aOsuFile.s_break_periods)
				lFileStream << "2," << lBreak.first << "," << lBreak.second << std::endl;
			lFileStream << "//Storyboard Layer 0 (Background)" << std::endl;
			lFileStream << "//Storyboard Layer 1 (Fail)" << std::endl;
			lFileStream << "//Storyboard Layer 2 (Pass)" << std::endl;
			lFileStream << "//Storyboard Layer 3 (Foreground)" << std::endl;
			lFileStream << "//Storyboard Sound Samples" << std::endl;
			lFileStream << std::endl;

			lFileStream << "[TimingPoints]" << std::endl;
			for (const auto& lTimingPoint : aOsuFile.s_timing_points)
				lFileStream << lTimingPoint.s_offset << "," << lTimingPoint.s_ms_per_beat << "," << lTimingPoint.s_meter << "," << static_cast<int>(lTimingPoint.s_sample_set) << "," << static_cast<int>(lTimingPoint.s_sample_index) << "," << static_cast<int>(lTimingPoint.s_volume) << "," << lTimingPoint.s_inherited << "," << lTimingPoint.s_kiai << std::endl;
			lFileStream << std::endl;

			lFileStream << "[Colours]" << std::endl;
			for (uint32_t i = 0; i < aOsuFile.s_colours.size(); ++i)
				lFileStream << "Combo" << i + 1 << " : " << get<0>(aOsuFile.s_colours[i]) << "," << get<1>(aOsuFile.s_colours[i]) << "," << get<2>(aOsuFile.s_colours[i]) << std::endl;
			lFileStream << std::endl;

			lFileStream << "[HitObjects]" << std::endl;
			for (const auto& lHitObject : aOsuFile.s_hit_objects)
			{
				if (lHitObject->s_type & (1 << 0))
				{
					hit_object_circle* lObject = static_cast<hit_object_circle*>(lHitObject);
					lFileStream << lObject->s_x << "," << lObject->s_y << "," << lObject->s_time << "," << static_cast<int>(lObject->s_type) << "," << static_cast<int>(lObject->s_hit_sounds);
					if (lObject->s_extras.s_sample_set != -1)
					{
						lFileStream << "," << lObject->s_extras.s_sample_set << ":" << lObject->s_extras.s_addition_set << ":" << lObject->s_extras.s_custom_index << ":" << lObject->s_extras.s_sample_volume << ":" << lObject->s_extras.s_filename;
					}
				}
				else if (lHitObject->s_type & (1 << 1))
				{
					hit_object_slider* lObject = static_cast<hit_object_slider*>(lHitObject);
					lFileStream << lObject->s_x << "," << lObject->s_y << "," << lObject->s_time << "," << static_cast<int>(lObject->s_type) << "," << static_cast<int>(lObject->s_hit_sounds) << "," << lObject->s_slider_type;
					for (const auto& lCurvePoint : lObject->s_curve_points)
						lFileStream << "|" << lCurvePoint.first << ":" << lCurvePoint.second;
					lFileStream << "," << static_cast<int>(lObject->s_repeat) << "," << lObject->s_pixel_length;
					
					if(!lObject->s_edge_hitsounds.empty())
					{
						lFileStream << ",";
						for (uint32_t i = 0; i < lObject->s_edge_hitsounds.size(); ++i)
						{
							lFileStream << static_cast<int>(lObject->s_edge_hitsounds[i]);
							if (i < lObject->s_edge_hitsounds.size() - 1)
								lFileStream << "|";
						}
					}
					if(!lObject->s_edge_additions.empty())
					{
						lFileStream << ",";
						for (uint32_t i = 0; i < lObject->s_edge_additions.size(); ++i)
						{
							lFileStream << static_cast<int>(lObject->s_edge_additions[i].first) << ":" << static_cast<int>(lObject->s_edge_additions[i].second);
							if (i < lObject->s_edge_additions.size() - 1)
								lFileStream << "|";
						}
					}
					if (lObject->s_extras.s_sample_set != -1)
					{
						lFileStream << "," << lObject->s_extras.s_sample_set << ":" << lObject->s_extras.s_addition_set << ":" << lObject->s_extras.s_custom_index << ":" << lObject->s_extras.s_sample_volume << ":" << lObject->s_extras.s_filename;
					}
				}
				else if (lHitObject->s_type & (1 << 3))
				{
					hit_object_spinner* lObject = static_cast<hit_object_spinner*>(lHitObject);
					lFileStream << lObject->s_x << "," << lObject->s_y << "," << lObject->s_time << "," << static_cast<int>(lObject->s_type) << "," << static_cast<int>(lObject->s_hit_sounds) << "," << lObject->s_end_time;
					if (lObject->s_extras.s_sample_set != -1)
					{
						lFileStream << "," << lObject->s_extras.s_sample_set << ":" << lObject->s_extras.s_addition_set << ":" << lObject->s_extras.s_custom_index << ":" << lObject->s_extras.s_sample_volume << ":" << lObject->s_extras.s_filename;
					}

				}
				lFileStream << std::endl;
			}

			lFileStream.close();
			return true;
		}

	}
}

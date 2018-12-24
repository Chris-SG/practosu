
#include "stdafx.h"
#include "file_changer.hpp"
#include "file_structs.hpp"
#include "func.hpp"

using namespace std;
namespace fs = std::experimental::filesystem;

namespace osu_tools
{
	namespace file_changer
	{
		// Set the speed to a multiplier value
		void set_speed_multiplier(float aMulti, osu_file& aOsuFile, string aNewFilename)
		{
			// Get the song folder path
			std::string lFullPath = osu_tools::func::get_beatmap_directory().string() + aOsuFile.s_folder_name;
			// Only continue if new audio file does not yet exist
			if(!fs::exists(lFullPath + "\\" + aNewFilename))
			{
				// If original audio file cannot be found, try trimming spaces.
				// If still not found, throw error.
				if(!fs::exists(lFullPath + "\\" + aOsuFile.s_audio_filename))
				{
					if (aOsuFile.s_audio_filename.find_first_not_of(" ") == 1)
						aOsuFile.s_audio_filename = aOsuFile.s_audio_filename.substr(1, aOsuFile.s_audio_filename.find_last_not_of(" "));
					else
						throw std::invalid_argument("Audio file cannot be found!");
				}
				// Setup commandline params to use for ffmpeg
				std::string lCommandLine = "-nostats -loglevel 0 -i \"" + lFullPath + "\\" + aOsuFile.s_audio_filename + "\" -filter:a \"";
				auto lMulti = aMulti;
				// ffmpeg only allows atempo up to 2.0 max.
				if (lMulti > 2.0)
				{
					do
					{
						lCommandLine += "atempo=2.0,";
						lMulti /= 2.0;
					} while (lMulti > 2.0);
					lCommandLine += "atempo=" + std::to_string(lMulti);
				}
				// ffmpeg only allows atempo down to 0.5 min.
				else if (aMulti < 0.5)
				{
					do
					{
						lCommandLine += "atempo=0.5,";
						lMulti /= 0.5;
					} while (lMulti < 0.5);
					lCommandLine += "atempo=" + std::to_string(lMulti);
				}
				else
				{
					lCommandLine += "atempo=" + std::to_string(lMulti);
				}
				// Append final details to commandline
				lCommandLine += "\" -vn \"" + lFullPath + "\\" + aNewFilename + "\"";
				// Create process information, prepare for launch
				STARTUPINFOA lSi;
				PROCESS_INFORMATION lPi;
				ZeroMemory(&lSi, sizeof(lSi));
				lSi.cb = sizeof(lSi);
				ZeroMemory(&lPi, sizeof(lPi));
				std::string lffPath = fs::current_path().string() + "\\ffmpeg.exe";
				// Attempt to create ffmpeg process
				if (!CreateProcessA(lffPath.c_str(), const_cast<char *>(lCommandLine.c_str()), NULL, NULL, false, 0, NULL, NULL, &lSi, &lPi))
					throw std::invalid_argument("Failed to create process: " + GetLastError());
			}

			// try block for modifying the timing points of the beatmap
			try
			{
				// Move across audio filename
				aOsuFile.s_audio_filename = aNewFilename;
				// Set preview time to same point
				aOsuFile.s_preview_time /= aMulti;

				// Loop over all timing points, fixing the offset and, if
				// applicable, the ms per beat.
				for (auto& lTimingPoint : aOsuFile.s_timing_points)
				{
					if (floor(lTimingPoint.s_offset) == lTimingPoint.s_offset)
						lTimingPoint.s_offset = floor(lTimingPoint.s_offset /= aMulti);
					else
						lTimingPoint.s_offset /= aMulti;
					if (lTimingPoint.s_inherited)
						lTimingPoint.s_ms_per_beat /= aMulti;
				}

				// Loop over all break periods and set new timing points.
				for (auto& lBreakPeriod : aOsuFile.s_break_periods)
				{
					lBreakPeriod.first /= aMulti;
					lBreakPeriod.second /= aMulti;
				}

				// Loop over all hit objects.
				for (auto& lHitObject : aOsuFile.s_hit_objects)
				{
					// Check if normal hit circle.
					if (lHitObject->s_type & (1 << 0))
					{
						hit_object_circle* lObject = static_cast<hit_object_circle*>(lHitObject);
						lObject->s_time /= aMulti;
					}
					// Check if normal slider.
					else if (lHitObject->s_type & (1 << 1))
					{
						hit_object_slider* lObject = static_cast<hit_object_slider*>(lHitObject);
						lObject->s_time /= aMulti;
					}
					// Check if normal spinner.
					else if (lHitObject->s_type & (1 << 3))
					{
						hit_object_spinner* lObject = static_cast<hit_object_spinner*>(lHitObject);
						lObject->s_time /= aMulti;
						lObject->s_end_time /= aMulti;
					}
					// Check if normal mania hold.
					else if(lHitObject->s_type & (1 << 7))
					{
						hit_object_mania_hold* lObject = static_cast<hit_object_mania_hold*>(lHitObject);
						lObject->s_time /= aMulti;
						lObject->s_end_time /= aMulti;
					}
				}
			}
			catch (std::exception& e)
			{
				// Failed to modify correctly.
				throw invalid_argument("Error modifying timing points: " + *e.what());
			}
			
		}

		void set_speed_percentage(float aMulti, osu_file& aOsuFile, string aNewFilename)
		{
			set_speed_multiplier(aMulti / 100, aOsuFile, aNewFilename);
		}
		
		double ms_per_beat_to_bpm(double ms)
		{
			const double ms_per_second = 1000.0;

			return (ms_per_second / ms) * 60.0;
		}

		double bpm_to_ms_per_beat(double bpm)
		{
			const double ms_per_second = 1000.0;

			return (ms_per_second / bpm) * 60.0;
		}

		void flip_play_area(osu_file & a_osu_file, bool flip_x, bool flip_y)
		{
			if (!flip_x && !flip_y)
				return;

			for (auto& hit_obj : a_osu_file.s_hit_objects)
			{
				if(flip_x)
					hit_obj->s_x *= -1;
				if(flip_y)
					hit_obj->s_y *= -1;
			}
		}


		// todo : allow arbitrary quadrant size via var
		void flip_play_area_quadrants(osu_file & a_osu_file, bool flip_x, bool flip_y)
		{
			if (!flip_x && !flip_y)
				return;

			const uint16_t max_osu_pixels_quadrant_x = 0;
			const uint16_t max_osu_pixels_quadrant_y = 0;

			for (auto& hit_obj : a_osu_file.s_hit_objects)
			{
				if (flip_x)
				{

				}
			}
		}


		void convert_sliders_to_circles(osu_file & a_osu_file)
		{
			for (auto& hit_obj : a_osu_file.s_hit_objects)
			{
				if (hit_obj->s_type & 1 << 1)
				{
					hit_obj->s_type = 0;
				}
			}
		}
	}
}

#pragma once

#include "stdafx.h"

struct beatmap
{
	uint32_t s_entry_sz;
	std::string s_artist;
	std::string s_artist_unicode;
	std::string s_title;
	std::string s_title_unicode;
	std::string s_creator;
	std::string s_difficulty;
	std::string s_audio_name;
	std::string s_md5;
	std::string s_osu_filename;
	uint8_t s_ranked_status;
	uint16_t s_num_hitcircles;
	uint16_t s_num_sliders;
	uint16_t s_num_spinners;
	uint64_t s_last_modification;
	uint8_t s_ar;
	uint8_t s_cs;
	uint8_t s_hp;
	uint8_t s_od;
	float s_ar_float; //20140609 or later
	float s_cs_float; //20140609 or later
	float s_hp_float; //20140609 or later
	float s_od_float; //20140609 or later
	double s_slider_velocity;
	uint32_t s_ratings_std; //20140609 or later
	std::vector<std::pair<uint32_t, double>> s_star_rating_std; //20140609 or later
	uint32_t s_ratings_taiko; //20140609 or later
	std::vector<std::pair<uint32_t, double>> s_star_rating_taiko; //20140609 or later
	uint32_t s_ratings_ctb; //20140609 or later
	std::vector<std::pair<uint32_t, double>> s_star_rating_ctb; //20140609 or later
	uint32_t s_ratings_mania; //20140609 or later
	std::vector<std::pair<uint32_t, double>> s_star_rating_mania; //20140609 or later
	uint32_t s_draintime;
	uint32_t s_total_time;
	uint32_t s_preview_time;
	uint32_t s_timing_point_count;
	std::vector<std::tuple<double, double, bool>> s_timing_points;
	uint32_t s_beatmap_id;
	uint32_t s_beatmap_set_id;
	uint32_t s_thread_id;
	uint8_t s_grade_std;
	uint8_t s_grade_taiko;
	uint8_t s_grade_ctb;
	uint8_t s_grade_mania;
	uint16_t s_local_offset;
	float s_stack_leniency;
	uint8_t s_gameplay_mode;
	std::string s_source;
	std::string s_tags;
	uint16_t s_online_offset;
	std::string s_font;
	bool s_played;
	uint64_t s_last_playtime;
	bool s_is_osz2;
	std::string s_folder_name;
	uint64_t s_last_checked_online;
	bool s_ignore_beatmap_sound;
	bool s_ignore_beatmap_skin;
	bool s_disable_sb;
	bool s_disable_video;
	bool s_visual_override;
	uint16_t s_unknown; // only prior to 20140609
	uint32_t s_last_modification_time;
	uint8_t s_mania_scroll_speed;
};

struct timing_point
{
	double s_offset;
	double s_ms_per_beat;
	uint32_t s_meter;
	uint8_t s_sample_set;
	uint8_t s_sample_index;
	uint8_t s_volume;
	bool s_inherited = false;
	bool s_kiai = false;
};

struct hit_object_extras
{
	uint32_t s_sample_set = 0;
	uint32_t s_addition_set = 0;
	uint32_t s_custom_index = 0;
	uint32_t s_sample_volume = 0;
	std::string s_filename;
};

struct hit_object
{
	uint16_t s_x;
	uint16_t s_y;
	uint32_t s_time;
	uint8_t s_type;
	uint8_t s_hit_sounds;
	// other data goes here
	hit_object_extras s_extras;
};

struct hit_object_circle : hit_object
{

};

struct hit_object_slider : hit_object
{
	char s_slider_type;
	std::vector<std::pair<int32_t, int32_t>> s_curve_points; // L uses 1, P uses 2, B uses many
	uint8_t s_repeat;
	float s_pixel_length;
	std::vector<uint8_t> s_edge_hitsounds;
	std::vector<std::pair<uint8_t, uint8_t>> s_edge_additions;
};

struct hit_object_spinner : hit_object
{
	uint32_t s_end_time;
};

struct osu_file
{
	std::string s_folder_name;
	std::string s_file_name;

	uint8_t s_beatmap_version = -1;
	// general
	std::string s_audio_filename;
	uint32_t s_audio_lead_in = -1;
	int32_t s_preview_time = -1;
	int8_t s_countdown = -1;
	std::string s_sample_set;
	float s_stack_leniency = -1.0;
	int8_t s_mode = -1;
	int8_t s_letterbox_in_breaks = -1;
	int8_t s_epilepsy_warning = -1;
	int8_t s_widescreen_storyboard = -1;
	std::string s_skin_preference;
	// editor
	std::vector<int> s_bookmarks;
	float s_distance_spacing = -1.0;
	uint8_t s_beat_divisor = -1;
	uint8_t s_grid_size = -1;
	float s_timeline_zoom = -1.0;
	// metadata
	std::string s_title;
	std::string s_title_unicode;
	std::string s_artist;
	std::string s_artist_unicode;
	std::string s_creator;
	std::string s_version;
	std::string s_source;
	std::string s_tags;
	uint32_t s_beatmap_id = -1;
	uint32_t s_beatmap_set_id = -1;
	// difficulty
	float s_hp_drain_rate = -1.0;
	float s_circle_size = -1.0;
	float s_overall_difficulty = -1.0;
	float s_approach_rate = -1.0;
	float s_slider_multiplier = -1.0;
	float s_slider_tick_rate = -1.0;
	// events
	std::string s_background;
	std::string s_video;
	std::vector<std::pair<uint32_t, uint32_t>> s_break_periods;
	// TODO layer 0/1/2/3/sounds
	// timing points
	std::vector<timing_point> s_timing_points;
	// colors
	std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> s_colours;
	// hitobjects
	std::vector<hit_object*> s_hit_objects;
};

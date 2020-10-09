#include "uri.h"
#include "method.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string.h>

namespace uri {

	TvSettingsGroupURI tvSettingsPictureUri[] = {
		{ "picture_mode" },
		{ "auto_brightness_control" },
		{ "backlight" },
		{ "brightness" },
		{ "contrast" },
		{ "color" },
		{ "tint" },
		{ "sharpness" },
		{ "color_calibration" , "picture_mode" },
		{ "color_calibration" , "color_temperature" },
		{ "color_calibration" , "calibration_tests" , "smpte_test_pattern" },
		{ "color_calibration" , "calibration_tests" , "flat_test_pattern" },
		{ "color_calibration" , "calibration_tests" , "ramp_test_pattern" },
		{ "color_calibration" , "calibration_tests" , "uniformity_analyzer_test_pattern" },
		{ "color_calibration" , "calibration_tests" , "dolby_vision_test_pattern" },
		{ "color_calibration" , "reset_color_tuner" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "header" , "red" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "header" , "green" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "header" , "blue" },
      /*{ "color_calibration" , "color_tuner" , "color_tuner_table" , "header" , "cyan" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "header" , "magenta" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "header" , "yellow"
		},*/
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "hue" , "red" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "hue" , "green" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "hue" , "blue" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "hue" , "cyan" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "hue" , "magenta" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "hue" , "yellow" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "saturation" , "red" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "saturation" , "green" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "saturation" , "blue" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "saturation" , "cyan" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "saturation" , "magenta" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "saturation" , "yellow" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "brightness" , "red" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "brightness" , "green" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "brightness" , "blue" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "brightness" , "cyan" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "brightness" , "magenta" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "brightness" , "yellow" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "offset" , "red" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "offset" , "green" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "offset" , "blue" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "gain" , "red" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "gain" , "green" },
		{ "color_calibration" , "color_tuner" , "color_tuner_table" , "gain" , "blue" },
		{ "more_picture" , "color_temperature" },
		{ "more_picture" , "black_detail" },
		{ "more_picture" , "active_led_zones" },
		{ "more_picture" , "backlight_control" },
		{ "more_picture" , "reduce_judder" },
		{ "more_picture" , "reduce_motion_blur" },
		{ "more_picture" , "clear_action" },
		{ "more_picture" , "reduce_signal_noise" },
		{ "more_picture" , "reduce_block_noise" },
		{ "more_picture" , "game_low_latency" },
		{ "more_picture" , "film_mode" },
		{ "more_picture" , "color_space" },
		{ "more_picture" , "gamma" },
		{ "picture_mode_edit" , "save_picture_mode" },
		{ "picture_mode_edit" , "lock_unlock_picture_mode" },
		{ "picture_mode_edit" , "delete_picture_mode" },
		{ "picture_mode_edit" , "reset_picture_mode" },
      /*{ "color_calibration" , "color_tuner" , "11_point_balance" , "gain" },
		{ "color_calibration" , "color_tuner" , "11_point_balance" , "red" },
		{ "color_calibration" , "color_tuner" , "11_point_balance" , "green" },
		{ "color_calibration" , "color_tuner" , "11_point_balance" , "blue" },*/

        { "color_calibration" , "color_tuner" , "11_point_balance" , "5_ire", "red" },
        { "color_calibration" , "color_tuner" , "11_point_balance" , "5_ire", "green" },
        { "color_calibration" , "color_tuner" , "11_point_balance" , "5_ire", "blue" },

        { "color_calibration" , "color_tuner" , "11_point_balance" , "10_ire", "red" },
        { "color_calibration" , "color_tuner" , "11_point_balance" , "10_ire", "green" },
        { "color_calibration" , "color_tuner" , "11_point_balance" , "10_ire", "blue" },

        { "color_calibration" , "color_tuner" , "11_point_balance" , "20_ire", "red" },
        { "color_calibration" , "color_tuner" , "11_point_balance" , "20_ire", "green" },
        { "color_calibration" , "color_tuner" , "11_point_balance" , "20_ire", "blue" },

        { "color_calibration" , "color_tuner" , "11_point_balance" , "30_ire", "red" },
        { "color_calibration" , "color_tuner" , "11_point_balance" , "30_ire", "green" },
        { "color_calibration" , "color_tuner" , "11_point_balance" , "30_ire", "blue" },

        { "color_calibration" , "color_tuner" , "11_point_balance" , "40_ire", "red" },
        { "color_calibration" , "color_tuner" , "11_point_balance" , "40_ire", "green" },
        { "color_calibration" , "color_tuner" , "11_point_balance" , "40_ire", "blue" },

        { "color_calibration" , "color_tuner" , "11_point_balance" , "50_ire", "red" },
        { "color_calibration" , "color_tuner" , "11_point_balance" , "50_ire", "green" },
        { "color_calibration" , "color_tuner" , "11_point_balance" , "50_ire", "blue" },

        { "color_calibration" , "color_tuner" , "11_point_balance" , "60_ire", "red" },
        { "color_calibration" , "color_tuner" , "11_point_balance" , "60_ire", "green" },
        { "color_calibration" , "color_tuner" , "11_point_balance" , "60_ire", "blue" },

        { "color_calibration" , "color_tuner" , "11_point_balance" , "70_ire", "red" },
        { "color_calibration" , "color_tuner" , "11_point_balance" , "70_ire", "green" },
        { "color_calibration" , "color_tuner" , "11_point_balance" , "70_ire", "blue" },

        { "color_calibration" , "color_tuner" , "11_point_balance" , "80_ire", "red" },
        { "color_calibration" , "color_tuner" , "11_point_balance" , "80_ire", "green" },
        { "color_calibration" , "color_tuner" , "11_point_balance" , "80_ire", "blue" },

        { "color_calibration" , "color_tuner" , "11_point_balance" , "90_ire", "red" },
        { "color_calibration" , "color_tuner" , "11_point_balance" , "90_ire", "green" },
        { "color_calibration" , "color_tuner" , "11_point_balance" , "90_ire", "blue" },

        { "color_calibration" , "color_tuner" , "11_point_balance" , "100_ire", "red" },
        { "color_calibration" , "color_tuner" , "11_point_balance" , "100_ire", "green" },
        { "color_calibration" , "color_tuner" , "11_point_balance" , "100_ire", "blue" },

        { "hdr_backlight"},
        { "more_picture" , "ultra_color_spectrum"},
        { "color_calibration" , "lut_selection_normal"},
        { "color_calibration" , "lut_selection_computer"},
        { "color_calibration" , "lut_selection_cool"},
        { "" },
    };

	TvSettingsGroupURI tvSettingsAudioUri[] = {

		/*
		###########################################
		# AUDIO
		*/

		{ "tv_speakers" },
        { "volume_control_display" },
		{ "surround_sound" },
		{ "volume_leveling" },
		{ "balance" },
		{ "lip_sync" },
		{ "digital_audio_out" },
		{ "analog_audio_out" },
		{ "volume" },
		{ "mute" },
		{ "equalizer" , "audio_mode" },
		{ "equalizer" , "100_hz" },
		{ "equalizer" , "300_hz" },
		{ "equalizer" , "1k_hz" },
		{ "equalizer" , "3k_hz" },
		{ "equalizer" , "10k_hz" },
		{ "delete_audio_mode" },
		{ "bluetooth" },
		{ "bluetooth_pair" },
		{ "" },
	};

	TvSettingsGroupURI tvSettingsTimerUri[] = {
		/*
		# TIMER
		*/

		{ "sleep_timer" },
		{ "auto_power_off_timer" },
		{"blank_screen"},
		{ "" },
	};

	TvSettingsGroupURI tvSettingsNetworkUri[] = {
        /*
        # NETWORK
        */

        { "connection_type" },
        { "connection_status" },
        { "current_ssid_name" },
        { "dhcp_mode" },
        { "ip_address" },
        { "subnet_mask" },
        { "default_gateway" },
        { "wireless_access_points" },
        { "current_access_point" },
        { "manual_setup" },
        { "manual_setup_info" , "dhcp" },
        { "manual_setup_info" , "ip_address" },
        { "manual_setup_info" , "subnet_mask" },
        { "manual_setup_info" , "default_gateway" },
        { "manual_setup_info" , "pref_dns_server" },
        { "manual_setup_info" , "alt_dns_server" },
        { "manual_setup_info" , "rj45_mac_address" },
        { "manual_setup_info" , "wireless_mac_address" },
        { "start_ap_search" },
        { "stop_ap_search" },
        { "set_wifi_password" },
        { "hidden_network" },
        { "hidden_network_info" , "name" },
        { "hidden_network_info" , "password" },
        { "test_connection" },
        { "test_connection" , "test_connection" },
        { "test_connection" , "test_connection_results" },
        { "test_connection" , "retest" },
        { "test_connection" , "manual_setup" , "dhcp" },
        { "test_connection" , "manual_setup" , "ip_address" },
        { "test_connection" , "manual_setup" , "subnet_mask" },
        { "test_connection" , "manual_setup" , "default_gateway" },
        { "test_connection" , "manual_setup" , "pref_dns_server" },
        { "test_connection" , "manual_setup" , "alt_dns_server" },
        { "test_connection" , "manual_setup" , "rj45_mac_address" },
        { "test_connection" , "manual_setup" , "wireless_mac_address" },
        { "" },

};

	TvSettingsGroupURI tvSettingsDevicesUri[] = {

	/*
	# DEVICES
	*/
	{ "name_input" , "cast" },
	{ "name_input" , "hdmi1" },
	{ "name_input" , "hdmi2" },
	{ "name_input" , "hdmi3" },
	{ "name_input" , "hdmi4" },
	{ "name_input" , "hdmi5" },
	{ "name_input" , "comp" },
	{ "current_input" },
        { "hdmi1" , "hdmi_color" },
        { "hdmi2" , "hdmi_color" },
        { "hdmi3" , "hdmi_color" },
        { "hdmi4" , "hdmi_color" },
        { "hdmi5" , "hdmi_color" },
	{ "full_chrome" , "hdmi1" },
	{ "full_chrome" , "hdmi2" },
	{ "full_chrome" , "hdmi3" },
	{ "full_chrome" , "hdmi4" },
	{ "full_chrome" , "hdmi5" },
        { "name_input" , "tuner" },
        { "name_input" , "vga" },
        { "auto_size_position" },
        { "picture_size" , "horizontal" },
        { "picture_size" , "vertical" },
        { "picture_position" , "horizontal" },
        { "picture_position" , "vertical" },

        { "cast" , "name_input" },
        { "hdmi1" , "name_input" },
        { "hdmi2" , "name_input" },
        { "hdmi3" , "name_input" },
        { "hdmi4" , "name_input" },
        { "hdmi5" , "name_input" },
        { "comp" , "name_input" },

        { "hdmi1" , "hide_input" },
        { "hdmi2" , "hide_input" },
        { "hdmi3" , "hide_input" },
        { "hdmi4" , "hide_input" },
        { "hdmi5" , "hide_input" },
        { "comp" , "hide_input" },

        { "tuner" , "picture_size","horizontal"},
        { "tuner" , "picture_size","vertical" },
        { "tuner" , "picture_position","horizontal"},
        { "tuner" , "picture_position","vertical" },
        { "tuner" , "hide_input"},

        { "current_inputs" },
        
        { "hdmi1","picture_size" , "horizontal" },
        { "hdmi1","picture_size" , "vertical" },
        { "hdmi1","picture_position" , "horizontal" },
        { "hdmi1","picture_position" , "vertical" },

        { "hdmi2","picture_size" , "horizontal" },
        { "hdmi2","picture_size" , "vertical" },
        { "hdmi2","picture_position" , "horizontal" },
        { "hdmi2","picture_position" , "vertical" },

        { "hdmi3","picture_size" , "horizontal" },
        { "hdmi3","picture_size" , "vertical" },
        { "hdmi3","picture_position" , "horizontal" },
        { "hdmi3","picture_position" , "vertical" },

        { "hdmi4","picture_size" , "horizontal" },
        { "hdmi4","picture_size" , "vertical" },
        { "hdmi4","picture_position" , "horizontal" },
        { "hdmi4","picture_position" , "vertical" },

        { "hdmi5","picture_size" , "horizontal" },
        { "hdmi5","picture_size" , "vertical" },
        { "hdmi5","picture_position" , "horizontal" },
        { "hdmi5","picture_position" , "vertical" },

        { "comp","picture_size" , "horizontal" },
        { "comp","picture_size" , "vertical" },
        { "comp","picture_position" , "horizontal" },
        { "comp","picture_position" , "vertical" },

        { "" },
};

	TvSettingsGroupURI tvSettingsSystemUri[] = {
		/*
		# SYSTEM
		*/

		{ "system_information" , "tv_information" , "cast_name" },
		{ "system_information" , "tv_information" , "serial_number" },
		{ "system_information" , "tv_information" , "model_name" },
		{ "system_information" , "tv_information" , "version" },
		{ "system_information" , "tv_information" , "cast_version" },
		{ "system_information" , "tv_information" , "source_type" },
		{ "system_information" , "tv_information" , "resolution" },
		{ "system_information" , "tv_information" , "vertical_frequency" },
		{ "system_information" , "tv_information" , "hdr" },
		{ "system_information" , "tv_information" , "audio_type" },
		{ "system_information" , "tv_information" , "frc_version" },
		{ "system_information" , "tv_information" , "netflix_esn" },
        { "system_information" , "tv_information" , "netflix_esn2" },
		{ "system_information" , "network_information" , "connection_type" },
		{ "system_information" , "network_information" , "ssid" },
		{ "system_information" , "network_information" , "security_type" },
		{ "system_information" , "network_information" , "ip_address" },
		{ "system_information" , "network_information" , "gateway" },
		{ "system_information" , "network_information" , "subnet_mask" },
		{ "system_information" , "network_information" , "preferred_dns_server" },
		{ "system_information" , "network_information" , "alternate_dns_server" },
		{ "system_information" , "network_information" , "download_speed" },
		{ "system_information" , "network_information" , "wifi_signal_strength" },
		{ "system_information" , "network_information" , "rssi" },
		{ "system_information" , "network_information" , "wifi_channel" },
		{ "system_information" , "network_information" , "rj45_mac_address" },
		{ "system_information" , "network_information" , "wlan_mac_address" },
		{ "system_information" , "network_information" , "bt_mac_address" },
		{ "system_information" , "uli_information" , "esn" },
		{ "system_information" , "uli_information" , "uid" },
		{ "system_information" , "uli_information" , "mv" },
		{ "system_information" , "uli_information" , "sv" },
		{ "system_information" , "uli_information" , "reg" },
		{ "system_information" , "uli_information" , "fet" },
		{ "system_information" , "uli_information" , "lt" },
		{ "system_information" , "uli_information" , "np" },
		{ "system_information" , "uli_information" , "ldt" },
		{ "system_information" , "uli_information" , "ldm" },
		{ "system_information" , "uli_information" , "ec" },
		{ "system_information" , "uli_information" , "le" },
		{ "system_information" , "uli_information" , "let" },
		{ "system_information" , "uli_information" , "rd" },
		{ "reset_and_admin" , "service_reset" },
		{ "service_check" },
		{ "local_time_settings" , "use_internet_time" },
		{ "local_time_settings" , "date_time" },
		{ "local_time_settings" , "timezone" },
		{ "time" , "automatic_time" },
		{ "local_time_settings" , "daylight_savings_time" },
		{ "local_time_settings" , "time_format" },
		{ "local_time_settings" , "country" },
		{ "menu_language" },
		{ "cec" , "cec_function" },
		{ "cec" , "device_discovery" },
		{ "reset_and_admin" , "power_indicator" },
		{ "reset_and_admin" , "usb_power" },
		{ "reset_and_admin" , "factory_reset" },
		{ "reset_and_admin" , "vizio_privacy_policy" },
		{ "reset_and_admin" , "smart_interactivity" },
		{ "reset_and_admin" , "store_demo" },
		{ "reset_and_admin" , "advertising" , "limit_ad_tracking" },
		{ "reset_and_admin" , "advertising" , "tvad_id_reset" },
		{ "reset_and_admin" , "advertising" , "ad_replacement" },
		{ "system_information" , "tuner_information" , "physical_channel" },
        { "system_information" , "tuner_information" , "frequency" },
        { "system_information" , "tuner_information" , "modulation" },
        { "system_information" , "tuner_information" , "status" },
        { "system_information" , "tuner_information" , "signal_strength" },
        { "system_information" , "tuner_information" , "name" },
        { "country" },
        { "power_mode" },
        { "aspect_ratio" },
        { "cast_name" },
        { "reset_and_admin" , "system_pin" },
        { "system_information" , "tv_information" , "series" },
        { "system_information" , "tv_information" , "size" },
        { "accessibility","talkback" },
        { "accessibility","speech_rate" },
        { "accessibility","zoom_mode" },
        { "system_information" , "tv_information" , "audio_type_out" },
        { "system_information" , "network_information" , "wifi_module" },
        { "system_information" , "tv_information" , "airplay_version" },
        { "" },

	};

	TvSettingsGroupURI tvSettingsCastUri[] = {
		/*
		# CAST
		*/

		{ "device_name" },
		{ "serial_device_id" },
		{ "google_privacy_policy" },
		{ "tos_accepted" },
		{ "soft_ap_config" },
		{ "start_soft_ap" },
		{ "stop_soft_ap" },
		{ "soft_ap_status" },
		{ "soft_ap_mode" },
		{ "state" },
		{ "session_device_id" },
		{ "" },
	};

	TvSettingsGroupURI tvSettingsChannelsUri[] = {
		/*
		# CHANNELS
		*/

		{"auto_channel_scan"},
		{"analog"},
		{"digital"},
		{"percent_complete"},
		{"skip_channel"},
		{"analog_audio"},
		{"digital_language"},
		{"parental_controls"},
		{"parental_controls" , "locks"},
		{"parental_controls" , "channel_locks"},
		{"parental_controls" , "usa_tv"},
		{"parental_controls" , "usa_tv" , "tv_y"},
		{"parental_controls" , "usa_tv" , "tv_y" , "all"},
		{"parental_controls" , "usa_tv" , "tv_y7"},
		{"parental_controls" , "usa_tv" , "tv_y7" , "all"},
		{"parental_controls" , "usa_tv" , "tv_y7" , "fv"},
		{"parental_controls" , "usa_tv" , "tv_g"},
		{"parental_controls" , "usa_tv" , "tv_g" , "all"},
		{"parental_controls" , "usa_tv" , "tv_pg"},
		{"parental_controls" , "usa_tv" , "tv_pg" , "all"},
		{"parental_controls" , "usa_tv" , "tv_pg" , "d"},
		{"parental_controls" , "usa_tv" , "tv_pg" , "s"},
		{"parental_controls" , "usa_tv" , "tv_pg" , "l"},
		{"parental_controls" , "usa_tv" , "tv_pg" , "v"},
		{"parental_controls" , "usa_tv" , "tv_14"},
		{"parental_controls" , "usa_tv" , "tv_14" , "all"},
		{"parental_controls" , "usa_tv" , "tv_14" , "d"},
		{"parental_controls" , "usa_tv" , "tv_14" , "s"},
		{"parental_controls" , "usa_tv" , "tv_14" , "l"},
		{"parental_controls" , "usa_tv" , "tv_14" , "v"},
		{"parental_controls" , "usa_tv" , "tv_ma"},
		{"parental_controls" , "usa_tv" , "tv_ma" , "all"},
		{"parental_controls" , "usa_tv" , "tv_ma" , "s"},
		{"parental_controls" , "usa_tv" , "tv_ma" , "l"},
		{"parental_controls" , "usa_tv" , "tv_ma" , "v"},
		{"parental_controls" , "usa_movie"},
		{"parental_controls" , "usa_movie" , "all"},
		{"parental_controls" , "usa_movie" , "g"},
		{"parental_controls" , "usa_movie" , "pg"},
		{"parental_controls" , "usa_movie" , "pg_13"},
		{"parental_controls" , "usa_movie" , "r"},
		{"parental_controls" , "usa_movie" , "nc_17"},
		{"parental_controls" , "canadian_english"},
		{"parental_controls" , "canadian_english" , "c"},
		{"parental_controls" , "canadian_english" , "c_plus"},
		{"parental_controls" , "canadian_english" , "g"},
		{"parental_controls" , "canadian_english" , "pg"},
		{"parental_controls" , "canadian_english" , "14_plus"},
		{"parental_controls" , "canadian_english" , "18_plus"},
		{"parental_controls" , "canadian_french"},
		{"parental_controls" , "canadian_french" , "g"},
		{"parental_controls" , "canadian_french" , "8_ans_plus"},
		{"parental_controls" , "canadian_french" , "13_ans_plus"},
		{"parental_controls" , "canadian_french" , "16_ans_plus"},
		{"parental_controls" , "canadian_french" , "18_ans_plus"},
		{"parental_controls" , "drrt"},
		{"parental_controls" , "drrt" , "*"},//parental_controls_drrt_dimension_M
	    {"parental_controls" , "drrt" , "*" , "*"},//parental_controls_drrt_dimension_M_ratingN
		{"parental_controls" , "block_unrated_shows"},
		{"parental_controls" , "reset"},
		{"current_channel"},
        {"skip_channel" , "*"}, //skip_channel_channel_N
        {"skip_channel" , "*" , "name"},//skip_channel_channel_N_name
        {"skip_channel" , "*" , "state"},//skip_channel_channel_N_state
        {"parental_controls" , "channel_locks" ,  "*"},//parental_controls_channel_locks_channel_N
        {"parental_controls" , "channel_locks" , "*" , "name"},//parental_controls_channel_locks_channel_N_name
        {"parental_controls" , "channel_locks" , "*" , "state"},//parental_controls_channel_locks_channel_N_state
	{"cancel_channel_scan"},
        { "" },
	};

	TvSettingsGroupURI tvSettingsClosedCaptionsUri[] = {
		/*
		# CLOSED CAPTIONS
		*/

		{ "closed_captions_enabled"} ,
		{ "analog_closed_captions"} ,
		{ "digital_closed_captions"} ,
		{ "digital_style"} ,
		{ "digital_style","text_style"} ,
		{ "digital_style","text_size"} ,
		{ "digital_style","text_color"} ,
		{ "digital_style","text_opacity"} ,
		{ "digital_style","text_edges"} ,
		{ "digital_style","text_edges_color"} ,
		{ "digital_style","background_color"} ,
		{ "digital_style","background_opacity"} ,
		{ "digital_style","window_color"} ,
		{ "digital_style","window_opacity"} ,
      /*{ "caption_style"} ,*/
		{ "" },
	};

	std::string tvSettingsGroupURIs[] = {
		URI_TV_SETTINGS_PICTURE	   ,
		URI_TV_SETTINGS_AUDIO 	   ,
		URI_TV_SETTINGS_TIMERS	   ,
		URI_TV_SETTINGS_NETWORK	   ,
		URI_TV_SETTINGS_DEVICES	   ,
		URI_TV_SETTINGS_SYSTEM	   ,
		URI_TV_SETTINGS_CAST 	   ,
		URI_TV_SETTINGS_CHANNELS   ,
		URI_TV_SETTINGS_CLOSED_CAPTIONS ,
	};

	TvSettingsGroupURI * tvSettingsUris[] = {
		tvSettingsPictureUri,
		tvSettingsAudioUri,
		tvSettingsTimerUri,
		tvSettingsNetworkUri,
		tvSettingsDevicesUri,
		tvSettingsSystemUri,
		tvSettingsCastUri,
		tvSettingsChannelsUri,
		tvSettingsClosedCaptionsUri,
	};

	StateGroupURI  stateGroupUris[] = {
		/*
		# STATE
		*/
		{ "device" , "deviceinfo" },
		{ "devices" , "name_input" , "cast" },
		{ "devices" , "name_input" , "hdmi1" },
		{ "devices" , "name_input" , "hdmi2" },
		{ "devices" , "name_input" , "hdmi3" },
		{ "devices" , "name_input" , "hdmi4" },
		{ "devices" , "name_input" , "hdmi5" },
		{ "devices" , "name_input" , "comp" },
		{ "devices" , "name_input" , "vga" },
		{ "audio" , "is_sound_bar_connected" },
		{ "acr" , "whatson" },
		{ "cec" , "hdmi1" },
		{ "cec" , "hdmi2" },
		{ "cec" , "hdmi3" },
		{ "cec" , "hdmi4" },
		{ "cec" , "hdmi5" },
		{ "device" , "currentstate" },
		{ "device" , "power_mode" },
		{ "devices" , "cast" },
        { "devices" , "hdmi1" },
        { "devices" , "hdmi2" },
        { "devices" , "hdmi3" },
        { "devices" , "hdmi4" },
        { "devices" , "hdmi5" },
        { "devices" , "comp" },
        { "devices" , "vga" },
        { "tuner" , "channel_settings" },
        { "tuner" , "all_channel_settings" },
        { "tuner" , "drrt_name" },
        { "tuner" , "drrt_all_settings" },
        { "tuner" , "drrt_settings" },
		{ "" },
	};

	PairingGroupURI  pairingGroupURIs[] = {

		/*
		# PAIRING
		# pairing" , "start
		# pairing" , "pair
		# pairing" , "unpair
		*/

		{ "bt" , "getauthtoken" },
		{ "bt" , "status" },
		{ "bt" , "rssimeter" },

        { "start" },
        { "pair" },
        { "unpair" },
        { "correct_challenge_response" },
        { "cancel" },
        { "bt" , "dev_info" },
        { "stop" },
        { "" },
	};

	OobeGroupURI  oobeGroupURIs[] = {

		/*
		# OOBE
		*/

		{ "currentstate" },
		{ "is_complete" },
		{ "" },
	};

    AppGroupURI appGroupURIs[] = {

        /*
        # ...
        */

        { "launch" },
        { "current" },
        { "is_in_ambient" },
        { "exit" },
        { "tts" , "enabled"},
        { "tts" , "play"},
        { "zoom" , "enabled"},
        { "zoom" , "focus"},
        { "splash" , "show"},
        { "splash" , "hide"},
        { "key" , "delegate"},
        { "menu" , "show" },
        { "menu" , "hide" },
        { "input" , "create" },
        { "input" , "metadata" },
        { "input" , "update" },
        { "input" , "delete" },
        { "tts" , "cancel"},
        { "" },
    };
    
    ULIGroupURI  uLIGroupURIs[] = {
    
        /*
        # ULI
        */
        
        { "register" },
        { "is_device_registered" },
        { "is_owner_registered" },
        { "is_updating" },
        { "available_update_info" },
        { "start_update" },
        { "update_status" },
        { "update_time" },
        { "" },
    };
    
    PINGroupURI  pINGroupURIs[] = {
    
        /*
        # PIN
        */
        
        {"confirm_pin"},
        {"set_pin"},
        {"is_pin_default"},
        {"is_pin_confirmed"},
        { "" },
    };

    SystemGroupURI  systemGroupURIs[] = {
        { "displayInfo" },
        { "restVersion" },
        { "config", "update" },
        { "decrypt" },
        { "assets", "stage" },
        { "assets", "update" },
        { "acr", "state" },
        { "ready" },
        { "pong" },
        { "systemInfo" },
        { "capabilities" },
        { "deviceInputs" },
        { "stage" } ,
        { "network_time" },
        { "binaries", "version" },
        { "native_power_function" },
        { "current_inputs" },
        { "is_composite" },
        { "input", "list" },
        { "input", "current_input" },
        { "identify" },
        { "input", "name" },
        { "input", "visible" },
        { "airplay", "user_settings_path" },
        { "binaries", "stage" },
        { "update" },
        { "uninstall" },
        { "uninstall_name" },
        { "remote_logging" },
        { "test", "check_for_update" },
        { "platform","ad_replacement" },
        { "picture","picture_mode" },
        { "" },
    };

    KeyCommandGroupURI  keycommandGroupURIs[] = {
        { "" },
        { "" },
    };

    ServerGroupURI serverGroupURIs[] = {
        /*
	# ...
        */
        { "ready" },
        { "" },
    };

    AirplayGroupURI airplayGroupURIs[] = {
        /*
	# ...
        */
        { "property" },
        { "action" },
        { "" },
    };

    HomekitGroupURI homekitGroupURIs[] = {
        /*
	# ...
        */
        { "pairing_status" },
        { "" },
    };

    System_menuGroupURI system_menuGroupURIs[] = {
    /*
    # ...
    */
    { "enabled" },
    { "" },

    };

    Conjure_GroupURI conjure_GroupURIs[] = {
    /*
# ...
    */
    { "state" },
    { "" },
    
    };

    Audio_GroupURI audio_GroupURIs[] = {
    /*
    # ...
    */
    { "volume","level" },
    { "volume","mute" },
    { "" },


    };

};

/*
struct  URI_RELATION_MATRIX
{
    int id;
    std::string module;
    std::string method;
    URIS*uris;
};

handler tv_handlers [] = {
        tv_settings_handler,
        state_handler,
        pairing_handler,
        oobe_handler,
        uli_handler,
        pin_handler,
        tv_system_handler,
        key_command_handler,
        app_handler,
        events_handler
};
*/
namespace matrix {
    URI_RELATION_MATRIX uriRelationMatrix[] = {
        { id::ID_MODULE_STATE       , uri::URI_STATE        ,  method::METHOD_STATE         , (URIS *)uri::stateGroupUris },
        { id::ID_MODULE_PAIRING     , uri::URI_PAIRING      ,  method::METHOD_PAIRING       , (URIS *)uri::pairingGroupURIs },
        { id::ID_MODULE_OOBE        , uri::URI_OOBE         ,  method::METHOD_OOBE          , (URIS *)uri::oobeGroupURIs },
        { id::ID_MODULE_ULI         , uri::URI_ULI          ,  method::METHOD_ULI           , (URIS *)uri::uLIGroupURIs },
        { id::ID_MODULE_PIN         , uri::URI_PIN          ,  method::METHOD_PIN           , (URIS *)uri::pINGroupURIs },
        { id::ID_MODULE_SYSTEM      , uri::URI_SYSTEM       ,  method::METHOD_SYSTEM        , (URIS *)uri::systemGroupURIs },
        { id::ID_MODULE_KEY_COMMAND , uri::URI_KEY_COMMAND  ,  method::METHOD_KEY_COMMAND   , (URIS *)uri::keycommandGroupURIs },
        { id::ID_MODULE_APP         , uri::URI_APP          ,  method::METHOD_APP           , (URIS *)uri::appGroupURIs },
        { id::ID_MODULE_SERVER      , uri::URI_SERVER       ,  method::METHOD_SERVER        , (URIS *)uri::serverGroupURIs },
        { id::ID_MODULE_AIRPLAY     , uri::URI_AIRPLAY      ,  method::METHOD_AIRPLAY       , (URIS *)uri::airplayGroupURIs },
        { id::ID_MODULE_HOMEKIT     , uri::URI_HOMEKIT      ,  method::METHOD_HOMEKIT       , (URIS *)uri::homekitGroupURIs },
        { id::ID_MODULE_SYSTEM_MENU     , uri::URI_SYSTEM_MENU      ,  method::METHOD_SYSTEM_MENU       , (URIS *)uri::system_menuGroupURIs },
        { id::ID_MODULE_CONJURE, uri::URI_CONJURE      ,  method::METHOD_CONJURE       , (URIS *)uri::conjure_GroupURIs },
        { id::ID_MODULE_AUDIO, uri::URI_AUDIO      ,  method::METHOD_AUDIO       , (URIS *)uri::audio_GroupURIs },
    };
};


static std::vector<std::string> splitString(const std::string& inputStr, char splitChar) {
	std::vector<std::string> outStrVec;

	std::string::size_type loc_start = 0;
	std::string::size_type loc_stop = inputStr.find_first_of(splitChar);

	while (1) {
		if (loc_stop == std::string::npos)
		{
			outStrVec.push_back(inputStr.substr(loc_start));
			break;
		}
		if (loc_start != loc_stop)
		{
			if (loc_stop != std::string::npos)
				outStrVec.push_back(inputStr.substr(loc_start, loc_stop - loc_start));
			else
				outStrVec.push_back(inputStr.substr(loc_start));
		}

		loc_start = loc_stop + 1;
		loc_stop = inputStr.find_first_of(splitChar, loc_start);
	}

	return outStrVec;
}


/*indexN:indicate dynamic value in uri,for example:
N in uri "tv_settings/channels/skip_channel/channel-N"
,or the high 16 bit indicate 255 and the low 16 bit indicate 1 in
uri "/menu_native/static/tv_settings/channels/parental_controls/drrt/dimension_255/rating1"

|--------------------------------|
|             channel            |
|--------------------------------|
or
|----------------|----------------|
|   dimension    |     rating     |
|----------------|----------------|

*/
template<class T> int find_index(T* array, std::vector<std::string> uris, int offset, int& indexN)
{
// appGroupURIs[], ["app"]["current"], 1, 0
//std::cout << "array:" << array << std::endl;
//std::cout << "uris:" << uris << std::endl;
//std::cout << "offset:" << offset << std::endl;
//std::cout << "indexN:" << indexN << std::endl;

    std::string channel_N;
    std::string dimension_N;
    std::string rating_N;

/*
AppGroupURI appGroupURIs[] = {
    { "launch" },
    { "current" },
    { "is_in_ambient" },
    { "exit" },
    { "tts" , "enable"},
    { "tts" , "play"},
    { "zoom" , "enable"},
    { "zoom" , "focus"},
    { "splash" , "show"},
    { "splash" , "hide"},
    { "" },
};
*/

    for (int i = 0; ; i++)
    {
        //std::cout << "array[i].index[0]:" << array[i].index[0] << std::endl;
        unsigned int j;
        if (array[i].index[0] == "")
        {
            if(uris.size() == offset)
                return 0;
            break;
        }
        for (j = 0; j < uris.size()-offset; j++)
        {
            //std::cout << "uris[j + offset]:" << uris[j + offset] << " array[i].index[j]" << array[i].index[j] << std::endl;
            if (uris[j + offset] != array[i].index[j])
            {
                if(array[i].index[j] == "*")
                {
                    if(uris[j + offset].compare(0, 7,"channel") == 0)
                    {
                        channel_N = uris[j + offset].substr(8);
                        indexN = atoi((const char*)channel_N.c_str());//indexN indicate the"N" value in channel-N
                    }
                    else if(uris[j + offset].compare(0, 9,"dimension") == 0)
                    {
                        dimension_N = uris[j + offset].substr(10);
                        indexN = (atoi((const char*)dimension_N.c_str()))<<16;//indexN indicate the"N" value in dimension_N
                    }
                    else if(uris[j + offset].compare(0, 6,"rating") == 0)
                    {
                        rating_N = uris[j + offset].substr(6);
                        indexN |= atoi((const char*)rating_N.c_str());//indexN indicate the"N" value in ratingN
                    }
                }
                else
                {
                    break;
				}
            }
            if (j == uris.size() - offset - 1)
            {
                return i;
            }
        }
    }
    return -1;
}

int uri::uritoid(const std::string uri, int & N)
{
	std::cout << "Enter uri::uritoid" << std::endl;
	std::vector<std::string> uris = splitString(uri, '/');

	for (unsigned int i = 0; i < uris.size(); i++)
	{
		std::cout << "item : " << uris[i] << std::endl;
	}

	int id = -1;

    if (uris[0] == uri::URI_TV_SETTINGS)
    {
        id = id::ID_MODULE_TV_SETTINGS << id::ID_MODULE_OFFSET;
        for (int group = 0; group < uri::TV_SETTINGS_GROUP_URI_SIZE; group++)
        {
            if (uris[1] == tvSettingsGroupURIs[group])
            {
                id |= group << id::ID_GROUP_OFFSET;
                int index = find_index(uri::tvSettingsUris[group], uris, 2, N);
                if (index != -1)
                {
                    id |= index;
                    //std::cout << "N="<<N<<",wff id : " << id <<"  index"<<index<<",wff id%="<<((id%0xff0000)>>16)<<",wff id%2="<<(id&0xffff)<< std::endl;
                    return id;
                }
                return -1;
            }
        }
        return -1;
    }

	/* skip tv settings */
    for (int module = 0; module < matrix::URI_RELATION_MATRIX_SIZE; module++)
    {
        //std::cout << "module: " << module << "module_name: " << matrix::uriRelationMatrix[module].module << std::endl;
        if (uris[0] == matrix::uriRelationMatrix[module].module)
        {
            id = matrix::uriRelationMatrix[module].id << id::ID_MODULE_OFFSET;
            std::cout << "id: " << id << std::endl;
            int index = find_index(matrix::uriRelationMatrix[module].uris, uris, 1, N);
            if (index != -1)
            {
                id |= index;
                return id;
            }
            return -1;
        }
    }

    return -1;
}

std::string uri::uritomethod(const std::string uri)
{
	std::vector<std::string> uris = splitString(uri, '/');
	if (uris.size() <= 2)
	{
		return "";
	}

	for (int module = 0; module < matrix::URI_RELATION_MATRIX_SIZE; module++)
	{
		if (uris[0] == matrix::uriRelationMatrix[module].module)
		{
			return matrix::uriRelationMatrix[module].method;
		}
	}

	return "";
}

std::string uri::idtomethod(int id)
{
	int index = id >> id::ID_MODULE_OFFSET;
	switch (index)
	{
	case 0:
		return method::METHOD_TV_SETTINGS;
	case 1:
		return method::METHOD_STATE;
	case 2:
		return method::METHOD_PAIRING;
	case 3:
		return method::METHOD_OOBE;
	case 4:
		return method::METHOD_ULI;
  	case 5:
		return method::METHOD_APP;
	}
	return "";
}


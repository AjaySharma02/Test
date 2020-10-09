#ifndef URI_H
#define URI_H

#include <string>
#include "method.h"

/* ID */

/*

|--------|--------|--------|--------|
| module | group  |     index       |
|--------|--------|--------|--------|

*/

namespace id {
    static const int ID_MODULE_TV_SETTINGS = 0x00;
    static const int ID_MODULE_STATE = 0x01;
    static const int ID_MODULE_PAIRING = 0x02;
    static const int ID_MODULE_OOBE = 0x03;
    static const int ID_MODULE_ULI = 0x04;
    static const int ID_MODULE_PIN = 0x05;
    static const int ID_MODULE_SYSTEM = 0x06;
    static const int ID_MODULE_KEY_COMMAND = 0x07;
    static const int ID_MODULE_APP = 0x08;
    static const int ID_MODULE_SERVER = 0x09;
    static const int ID_MODULE_AIRPLAY = 0x0a;
    static const int ID_MODULE_HOMEKIT = 0x0b;
    static const int ID_MODULE_SYSTEM_MENU = 0x0c;
    static const int ID_MODULE_CONJURE = 0x0d;
    static const int ID_MODULE_AUDIO = 0x0e;
	 
    static const int ID_MODULE_MASK = 0xff000000;
    static const int ID_MODULE_OFFSET = 24;
    
    static const int ID_GROUP_TV_SETTINGS_PICTURE = 0x00;
    static const int ID_GROUP_TV_SETTINGS_AUDIO = 0x01;
    static const int ID_GROUP_TV_SETTINGS_TIMERS = 0x02;
    static const int ID_GROUP_TV_SETTINGS_NETWORK = 0x03;
    static const int ID_GROUP_TV_SETTINGS_DEVICES = 0x04;
    static const int ID_GROUP_TV_SETTINGS_SYSTEM = 0x05;
    static const int ID_GROUP_TV_SETTINGS_CAST = 0x06;
    
    static const int ID_GROUP_MASK = 0x00ff0000;
    static const int ID_GROUP_OFFSET = 16;
    
    static const int ID_INDEX_MASK = 0x0000ffff;
};

namespace uri {

	struct URI {
		std::string module;
		std::string group;
		std::string index[];
	};

	static const int URI_INDEX_SIZE = 8;

	struct TvSettingsGroupURI {
		std::string index[URI_INDEX_SIZE];
	};

	/*
	# PICTURE
	*/

	static const std::string URI_TV_SETTINGS = "tv_settings";

	static const std::string URI_TV_SETTINGS_PICTURE = "picture";
	static const std::string URI_TV_SETTINGS_AUDIO = "audio";
	static const std::string URI_TV_SETTINGS_TIMERS = "timers";
	static const std::string URI_TV_SETTINGS_NETWORK = "network";
	static const std::string URI_TV_SETTINGS_DEVICES = "devices";
	static const std::string URI_TV_SETTINGS_SYSTEM = "system";
	static const std::string URI_TV_SETTINGS_CAST = "cast";
	static const std::string URI_TV_SETTINGS_CHANNELS = "channels";
	static const std::string URI_TV_SETTINGS_CLOSED_CAPTIONS = "closed_captions";

	static const int TV_SETTINGS_GROUP_URI_SIZE = 9;

	static const std::string URI_STATE = "state";

	struct StateGroupURI {
		std::string index[URI_INDEX_SIZE];
	};

	static const std::string URI_PAIRING = "pairing";

	struct PairingGroupURI {
		std::string index[URI_INDEX_SIZE];
	};


	static const std::string URI_OOBE = "oobe";

	struct OobeGroupURI {
		std::string index[URI_INDEX_SIZE];
	};


    static const std::string URI_APP = "app";

	struct AppGroupURI {
		std::string index[URI_INDEX_SIZE];
	};


	static const std::string URI_ULI = "uli";

	struct ULIGroupURI {
		std::string index[URI_INDEX_SIZE];
	};

	static const std::string URI_PIN = "pin";

	struct PINGroupURI {
		std::string index[URI_INDEX_SIZE];
	};

    static const std::string URI_SYSTEM = "system";

	struct SystemGroupURI {
		std::string index[URI_INDEX_SIZE];
	};

    static const std::string URI_KEY_COMMAND = "key_command";

	struct KeyCommandGroupURI {
		std::string index[URI_INDEX_SIZE];
	};

	static const std::string URI_SERVER = "server";

	struct ServerGroupURI {
		std::string index[URI_INDEX_SIZE];
	};

    static const std::string URI_AIRPLAY = "airplay";

	struct AirplayGroupURI {
		std::string index[URI_INDEX_SIZE];
	};

    static const std::string URI_HOMEKIT = "homekit";

	struct HomekitGroupURI {
		std::string index[URI_INDEX_SIZE];
	};

    static const std::string URI_SYSTEM_MENU = "system_menu";

	struct System_menuGroupURI {
		std::string index[URI_INDEX_SIZE];
	};

    static const std::string URI_CONJURE = "conjure";

	struct Conjure_GroupURI {
		std::string index[URI_INDEX_SIZE];
	};

    static const std::string URI_AUDIO = "audio";

	struct Audio_GroupURI {
		std::string index[URI_INDEX_SIZE];
	};

	int uritoid(const std::string uri, int & N);
	std::string uritomethod(const std::string uri);
	std::string idtomethod(int id);

};


namespace matrix {
	struct URIS {
		std::string index[uri::URI_INDEX_SIZE];
	};

	struct  URI_RELATION_MATRIX
	{
		int id;
		std::string module;
		std::string method;
		URIS*uris;
	};

	static  const  int URI_RELATION_MATRIX_SIZE = 14;
};



#if 0  /* just for debug usage */
URI  uris[] = {

	{ "tv_settings" , "picture" , "picture_mode" },
	{ "tv_settings" , "picture" , "auto_brightness_control" },
	{ "tv_settings" , "picture" , "backlight" },
	{ "tv_settings" , "picture" , "brightness" },
	{ "tv_settings" , "picture" , "contrast" },
	{ "tv_settings" , "picture" , "color" },
	{ "tv_settings" , "picture" , "tint" },
	{ "tv_settings" , "picture" , "sharpness" },
	{ "tv_settings" , "picture" , "color_calibration" , "picture_mode" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_temperature" },
	{ "tv_settings" , "picture" , "color_calibration" , "calibration_tests" , "smpte_test_pattern" },
	{ "tv_settings" , "picture" , "color_calibration" , "calibration_tests" , "flat_test_pattern" },
	{ "tv_settings" , "picture" , "color_calibration" , "calibration_tests" , "ramp_test_pattern" },
	{ "tv_settings" , "picture" , "color_calibration" , "calibration_tests" , "uniformity_analyzer_test_pattern" },
	{ "tv_settings" , "picture" , "color_calibration" , "calibration_tests" , "dolby_vision_test_pattern" },
	{ "tv_settings" , "picture" , "color_calibration" , "reset_color_tuner" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "header" , "red" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "header" , "green" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "header" , "blue" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "header" , "cyan" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "header" , "magenta" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "header" , "yellow" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "hue" , "red" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "hue" , "green" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "hue" , "blue" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "hue" , "cyan" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "hue" , "magenta" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "hue" , "yellow" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "saturation" , "red" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "saturation" , "green" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "saturation" , "blue" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "saturation" , "cyan" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "saturation" , "magenta" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "saturation" , "yellow" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "brightness" , "red" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "brightness" , "green" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "brightness" , "blue" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "brightness" , "cyan" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "brightness" , "magenta" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "brightness" , "yellow" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "offset" , "red" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "offset" , "green" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "offset" , "blue" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "gain" , "red" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "gain" , "green" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "color_tuner_table" , "gain" , "blue" },
	{ "tv_settings" , "picture" , "color_temperature" },
	{ "tv_settings" , "picture" , "black_detail" },
	{ "tv_settings" , "picture" , "active_led_zones" },
	{ "tv_settings" , "picture" , "backlight_control" },
	{ "tv_settings" , "picture" , "reduce_judder" },
	{ "tv_settings" , "picture" , "reduce_motion_blur" },
	{ "tv_settings" , "picture" , "clear_action" },
	{ "tv_settings" , "picture" , "reduce_signal_noise" },
	{ "tv_settings" , "picture" , "reduce_block_noise" },
	{ "tv_settings" , "picture" , "game_low_latency" },
	{ "tv_settings" , "picture" , "film_mode" },
	{ "tv_settings" , "picture" , "color_space" },
	{ "tv_settings" , "picture" , "gamma" },
	{ "tv_settings" , "picture" , "picture_mode_edit" , "save_picture_mode" },
	{ "tv_settings" , "picture" , "picture_mode_edit" , "lock_unlock_picture_mode" },
	{ "tv_settings" , "picture" , "picture_mode_edit" , "delete_picture_mode" },
	{ "tv_settings" , "picture" , "picture_mode_edit" , "reset_picture_mode" },
	{ "tv_settings" , "picture" , "picture_size" , "horizontal" },
	{ "tv_settings" , "picture" , "picture_size" , "vertical" },
	{ "tv_settings" , "picture" , "picture_position" , "horizontal" },
	{ "tv_settings" , "picture" , "picture_position" , "vertical" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "11_point_balance" , "gain" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "11_point_balance" , "red" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "11_point_balance" , "green" },
	{ "tv_settings" , "picture" , "color_calibration" , "color_tuner" , "11_point_balance" , "blue" },
	{ "#tv_settings" , "picture" , "ultra_color_spectrum" },
	{ "#tv_settings" , "picture" , "hdmi_color" },

	/*
	###########################################
	# AUDIO
	*/

	{ "tv_settings" , "audio" , "tv_speakers" },
	{ "tv_settings" , "audio" , "surround_sound" },
	{ "tv_settings" , "audio" , "volume_leveling" },
	{ "tv_settings" , "audio" , "balance" },
	{ "tv_settings" , "audio" , "lip_sync" },
	{ "tv_settings" , "audio" , "digital_audio_out" },
	{ "tv_settings" , "audio" , "analog_audio_out" },
	{ "tv_settings" , "audio" , "volume" },
	{ "tv_settings" , "audio" , "mute" },
	{ "tv_settings" , "audio" , "equalizer" , "audio_mode" },
	{ "tv_settings" , "audio" , "equalizer" , "100_hz" },
	{ "tv_settings" , "audio" , "equalizer" , "300_hz" },
	{ "tv_settings" , "audio" , "equalizer" , "1k_hz" },
	{ "tv_settings" , "audio" , "equalizer" , "3k_hz" },
	{ "tv_settings" , "audio" , "equalizer" , "10k_hz" },
	{ "tv_settings" , "audio" , "delete_audio_mode" },
	{ "tv_settings" , "audio" , "bluetooth" },
	{ "tv_settings" , "audio" , "bluetooth_pair" },

	/*
	# TIMER
	*/

	{ "tv_settings" , "timers" , "sleep_timer" },
	{ "tv_settings" , "timers" , "auto_power_off_timer" },

	/*
	# NETWORK
	*/

	{ "tv_settings" , "network", "connection_type" },
	{ "tv_settings" , "network", "connection_status" },
	{ "tv_settings" , "network", "current_ssid_name" },
	{ "tv_settings" , "network" , "dhcp_mode" },
	{ "tv_settings" , "network" , "ip_address" },
	{ "tv_settings" , "network" , "subnet_mask" },
	{ "tv_settings" , "network" , "default_gateway" },
	{ "tv_settings" , "network" , "wireless_access_points" },
	{ "tv_settings" , "network" , "current_access_point" },
	{ "#tv_settings", "network", "current_ssid_password" },
	{ "#tv_settings", "network", "rj45_mac_address" },
	{ "tv_settings" , "network" , "manual_setup" , "dhcp" },
	{ "tv_settings" , "network" , "manual_setup" , "ip_address" },
	{ "tv_settings" , "network" , "manual_setup" , "subnet_mask" },
	{ "tv_settings" , "network" , "manual_setup" , "default_gateway" },
	{ "tv_settings" , "network" , "manual_setup" , "pref_dns_server" },
	{ "tv_settings" , "network" , "manual_setup" , "alt_dns_server" },
	{ "tv_settings" , "network" , "manual_setup" , "rj45_mac_address" },
	{ "tv_settings" , "network" , "manual_setup" , "wireless_mac_address" },
	{ "tv_settings" , "network" , "start_ap_search" },
	{ "tv_settings" , "network" , "stop_ap_search" },
	{ "tv_settings" , "network" , "set_wifi_password" },
	{ "tv_settings" , "network" , "hidden_network" , "name" },
	{ "tv_settings" , "network" , "hidden_network" , "password" },
	{ "tv_settings" , "network" , "test_connection" , "test_connection" },
	{ "tv_settings" , "network" , "test_connection" , "test_connection_results" },
	{ "tv_settings" , "network" , "test_connection" , "retest" },
	{ "tv_settings" , "network" , "test_connection" , "manual_setup" , "dhcp" },
	{ "tv_settings" , "network" , "test_connection" , "manual_setup" , "ip_address" },
	{ "tv_settings" , "network" , "test_connection" , "manual_setup" , "subnet_mask" },
	{ "tv_settings" , "network" , "test_connection" , "manual_setup" , "default_gateway" },
	{ "tv_settings" , "network" , "test_connection" , "manual_setup" , "pref_dns_server" },
	{ "tv_settings" , "network" , "test_connection" , "manual_setup" , "alt_dns_server" },
	{ "tv_settings" , "network" , "test_connection" , "manual_setup" , "rj45_mac_address" },
	{ "tv_settings" , "network" , "test_connection" , "manual_setup" , "wireless_mac_address" },



	/*
	# DEVICES
	*/
	{ "tv_settings" , "devices" , "name_input" , "cast" },
	{ "tv_settings" , "devices" , "name_input" , "hdmi1" },
	{ "tv_settings" , "devices" , "name_input" , "hdmi2" },
	{ "tv_settings" , "devices" , "name_input" , "hdmi3" },
	{ "tv_settings" , "devices" , "name_input" , "hdmi4" },
	{ "tv_settings" , "devices" , "name_input" , "hdmi5" },
	{ "tv_settings" , "devices" , "name_input" , "comp" },
	{ "tv_settings" , "devices" , "current_input" },
	{ "tv_settings" , "devices" , "hdmi_color" , "hdmi1" },
	{ "tv_settings" , "devices" , "hdmi_color" , "hdmi2" },
	{ "tv_settings" , "devices" , "hdmi_color" , "hdmi3" },
	{ "tv_settings" , "devices" , "hdmi_color" , "hdmi4" },

	/*
	# SYSTEM
	*/

	{ "tv_settings" , "system" , "system_information" , "tv_information" , "cast_name" },
	{ "tv_settings" , "system" , "system_information" , "tv_information" , "serial_number" },
	{ "tv_settings" , "system" , "system_information" , "tv_information" , "model_name" },
	{ "tv_settings" , "system" , "system_information" , "tv_information" , "version" },
	{ "tv_settings" , "system" , "system_information" , "tv_information" , "cast_version" },
	{ "tv_settings" , "system" , "system_information" , "tv_information" , "source_type" },
	{ "tv_settings" , "system" , "system_information" , "tv_information" , "resolution" },
	{ "tv_settings" , "system" , "system_information" , "tv_information" , "vertical_frequency" },
	{ "tv_settings" , "system" , "system_information" , "tv_information" , "hdr" },
	{ "tv_settings" , "system" , "system_information" , "tv_information" , "audio_type" },
	{ "tv_settings" , "system" , "system_information" , "tv_information" , "frc_version" },
	{ "tv_settings" , "system" , "system_information" , "tv_information" , "netflix_esn" },
	{ "tv_settings" , "system" , "system_information" , "network_information" , "connection_type" },
	{ "tv_settings" , "system" , "system_information" , "network_information" , "ssid" },
	{ "tv_settings" , "system" , "system_information" , "network_information" , "security_type" },
	{ "tv_settings" , "system" , "system_information" , "network_information" , "ip_address" },
	{ "tv_settings" , "system" , "system_information" , "network_information" , "gateway" },
	{ "tv_settings" , "system" , "system_information" , "network_information" , "subnet_mask" },
	{ "tv_settings" , "system" , "system_information" , "network_information" , "preferred_dns_server" },
	{ "tv_settings" , "system" , "system_information" , "network_information" , "alternate_dns_server" },
	{ "tv_settings" , "system" , "system_information" , "network_information" , "download_speed" },
	{ "tv_settings" , "system" , "system_information" , "network_information" , "wifi_signal_strength" },
	{ "tv_settings" , "system" , "system_information" , "network_information" , "rssi" },
	{ "tv_settings" , "system" , "system_information" , "network_information" , "wifi_channel" },
	{ "tv_settings" , "system" , "system_information" , "network_information" , "rj45_mac_address" },
	{ "tv_settings" , "system" , "system_information" , "network_information" , "wlan_mac_address" },
	{ "tv_settings" , "system" , "system_information" , "network_information" , "bt_mac_address" },
	{ "tv_settings" , "system" , "system_information" , "uli_information" , "esn" },
	{ "tv_settings" , "system" , "system_information" , "uli_information" , "uid" },
	{ "tv_settings" , "system" , "system_information" , "uli_information" , "mv" },
	{ "tv_settings" , "system" , "system_information" , "uli_information" , "sv" },
	{ "tv_settings" , "system" , "system_information" , "uli_information" , "reg" },
	{ "tv_settings" , "system" , "system_information" , "uli_information" , "fet" },
	{ "tv_settings" , "system" , "system_information" , "uli_information" , "lt" },
	{ "tv_settings" , "system" , "system_information" , "uli_information" , "np" },
	{ "tv_settings" , "system" , "system_information" , "uli_information" , "ldt" },
	{ "tv_settings" , "system" , "system_information" , "uli_information" , "ldm" },
	{ "tv_settings" , "system" , "system_information" , "uli_information" , "ec" },
	{ "tv_settings" , "system" , "system_information" , "uli_information" , "le" },
	{ "tv_settings" , "system" , "system_information" , "uli_information" , "let" },
	{ "tv_settings" , "system" , "system_information" , "uli_information" , "rd" },
	{ "tv_settings" , "system" , "system_information" , "service_reset" },
	{ "tv_settings" , "system" , "system_information" , "service_check" },
	{ "tv_settings" , "system" , "local_time_settings" , "use_internet_time" },
	{ "tv_settings" , "system" , "local_time_settings" , "date_time" },
	{ "tv_settings" , "system" , "local_time_settings" , "timezone" },
	{ "tv_settings" , "system" , "local_time_settings" , "daylight_savings_time" },
	{ "tv_settings" , "system" , "local_time_settings" , "time_format" },
	{ "tv_settings" , "system" , "menu_language" },
	{ "tv_settings" , "system" , "cec" , "cec_function" },
	{ "tv_settings" , "system" , "cec" , "device_discovery" },
	{ "tv_settings" , "system" , "power_indicator" },
	{ "tv_settings" , "system" , "reset_and_admin" , "factory_reset" },
	{ "tv_settings" , "system" , "reset_and_admin" , "vizio_privacy_policy" },
	{ "tv_settings" , "system" , "reset_and_admin" , "smart_interactivity" },
	{ "tv_settings" , "system" , "reset_and_admin" , "store_demo" },
	{ "#tv_settings" , "system" , "system_information" , "tv_information" , "progressive" },
	{ "#tv_settings" , "system" , "system_information" , "uli_information" , "ml" },
	{ "#tv_settings" , "system" , "system_information" , "uli_information" , "mc" },
	{ "#tv_settings" , "system" , "system_information" , "uli_information" , "device_class" },
	{ "#tv_settings" , "system" , "cec" , "device_discovery" , "cec_device_1" },
	{ "#tv_settings" , "system" , "cec" , "device_discovery" , "cec_device_2" },


	/*
	# CAST
	*/

	{ "tv_settings" , "cast" , "device_name" },
	{ "#tv_settings" , "cast", "receiver_name" },
	{ "tv_settings" , "cast" , "serial_device_id" },
	{ "tv_settings" , "cast" , "google_privacy_policy" },
	{ "tv_settings" , "cast" , "tos_accepted" },
	{ "tv_settings" , "cast" , "soft_ap_config" },
	{ "tv_settings" , "cast" , "start_soft_ap" },
	{ "tv_settings" , "cast" , "stop_soft_ap" },
	{ "tv_settings" , "cast" , "soft_ap_status" },
	{ "tv_settings" , "cast" , "soft_ap_mode" },

	/*
	# STATE
	*/

	{ "state" , "devices" , "name_input" , "cast" },
	{ "state" , "devices" , "name_input" , "hdmi1" },
	{ "state" , "devices" , "name_input" , "hdmi2" },
	{ "state" , "devices" , "name_input" , "hdmi3" },
	{ "state" , "devices" , "name_input" , "hdmi4" },
	{ "state" , "devices" , "name_input" , "hdmi5" },
	{ "state" , "devices" , "name_input" , "comp" },
	{ "state" , "audio" , "is_sound_bar_connected" },
	{ "state" , "acr" , "whatson" },
	{ "state" , "cec" , "hdmi1" },
	{ "state" , "cec" , "hdmi2" },
	{ "state" , "cec" , "hdmi3" },
	{ "state" , "cec" , "hdmi4" },
	{ "state" , "cec" , "hdmi5" },



	/*
	# PAIRING
	# pairing" , "start
	# pairing" , "pair
	# pairing" , "unpair
	*/

	{ "pairing" , "bt" , "getauthtoken" },
	{ "pairing" , "bt" , "status" },
	{ "pairing" , "bt" , "rssimeter" },

	/*
	# ULI
	*/

	{ "uli" , "register" },
	{ "uli" , "is_device_registered" },


	/*
	# OOBE
	*/

	{ "oobe" , "currentstate" },
	{ "oobe" , "is_complete" },


};

#endif

#endif /* URI_H */


# -*- coding: utf-8 -*-

import logging
from REST.resources import LanguageLists
from REST.resources import LanguageStrings
from django.conf import settings

import sys
print(sys.path)

#from REST.resources import Global
from REST.resources import *
#from REST.resources import Global


LANGUAGE_COUNT = 3
debuglevel = 0

logger = logging.getLogger(__name__)

def debug(message):
    if debuglevel > 0:
        print message


def tr(object, lang, ret, value):
    for name in dir(object):
        if not name.startswith('__'):
            l = vars(object).get(name)
            debug(l)
            if isinstance(l, list):
                debug(l)
                debug(len(l))
                assert len(l) == LANGUAGE_COUNT
                if isinstance(l[lang], list):
                    debug(l[lang])
                    if value in l[lang]:
                        p = l[lang].index(value)
                        debug(p)
                        if p >= 0:
                            return l[ret][p]
                else:
                    debug(type(l[lang]))
                    if l[lang] == value:
                        debug(l[lang])
                        return l[ret]
    return None



# def stringstr(lang, ret, value):
#     return tr(LanguageStrings, lang, ret, value)
#
# def gettype( uri):
#     obj = Global.resourceTree.get(uri)
#     print obj
#     return obj.type
#
# def current():
#     return Global.lang
#
# def isMulti( uri):
#     obj = Global.resourceTree.get(uri)
#     if vars(obj).get("elements") ==  None :
#         return False
#     else :
#         return True


"""

# use this code in Global.py to get all translation uri


print '----------------------------------------------------------'


def rename(object):
    for name in dir(object):
        if not name.startswith('__'):
            l = vars(object).get(name)
            if isinstance(l, list):
                if isinstance(l[0], list):
                    l[0].append(name)
    return None


from REST.resources import LanguageLists


rename(LanguageLists)

print '-----------------------------------------------------------------'




...

#init tv resource tree

...


print '---------------------------------------------'


trees = [resourceTree, autoCalTree, systemPINTree, factoryTree, pairingTree, stateTree, oobeTree, uliTree]

for tree in trees:
    try :
        object = tree.resourceMap.items()
        for obj in object:
            data = obj[1]
            e = vars(data).get("elements")
            if e !=  None :
                if isinstance (e, list) and len(e) > 0:
                    out = []
                    out.append("[\"")
                    out.append(obj[0])
                    out.append("\", ")
                    out.append('\"')
                    out.append(e[len(e) -1])
                    out.append('\"],')
                    print "".join(out)

    except Exception as e:
        print e

print '---------------------------------------------'

"""

translation_wildcard = [
["tv_settings/channels/skip_channel/channel", "L_NO_YES"],
["tv_settings/channels/parental_controls/channel", "L_LOCKS"],
]

translation = [
["tv_settings/channels/parental_controls/usa_movie/nc-17", "L_LOCKS"],
["tv_settings/picture/color_calibration/color_tuner/color_tuner_table/header/blue", "L_OFF_ON"],
["tv_settings/channels/parental_controls/usa_tv/tv_pg/d", "L_LOCKS"],
["tv_settings/system/local_time_settings/timezone", "L_TIMEZONES"],
["tv_settings/channels/parental_controls/usa_tv/tv_pg/l", "L_LOCKS"],
["tv_settings/channels/parental_controls/canadian_french/16_ans_plus", "L_LOCKS"],
["tv_settings/channels/parental_controls/usa_tv/tv_pg/v", "L_LOCKS"],
["tv_settings/channels/parental_controls/canadian_english/c", "L_LOCKS"],
["tv_settings/channels/parental_controls/block_unrated_shows", "L_OFF_ON"],
["tv_settings/picture/more_picture/black_detail", "L_OFF_LOW_MED_HI"],
["tv_settings/channels/parental_controls/canadian_french/18_ans_plus", "L_LOCKS"],
["tv_settings/system/menu_language", "L_LANGUAGES"],
["tv_settings/channels/parental_controls/canadian_english/c_plus", "L_LOCKS"],
["tv_settings/closed_captions/analog_closed_captions", "L_OFF_ON"],
["tv_settings/channels/parental_controls/usa_movie/pg-13", "L_LOCKS"],
["tv_settings/channels/parental_controls/canadian_english/g", "L_LOCKS"],
["tv_settings/cast/soft_ap_status", "L_OFF_ON"],
["tv_settings/channels/digital_language", "L_DIGITAL_LANGUAGES"],
["tv_settings/picture/color_calibration/color_tuner/color_tuner_table/header/green", "L_OFF_ON"],
["tv_settings/channels/parental_controls/usa_tv/tv_g/all", "L_LOCKS"],
["tv_settings/audio/tv_speakers", "L_AUTO_ON_OFF"],
["tv_settings/audio/volume_control_display", "L_OFF_ON"],
["tv_settings/timers/auto_power_off_timer", "L_AUTO_POWER_OFF_TIMER"],
["tv_settings/picture/color_calibration/calibration_tests/flat_test_pattern", "L_TEST_PATTERN"],
["tv_settings/channels/parental_controls/usa_tv/tv_y/all", "L_LOCKS"],
["tv_settings/channels/parental_controls/usa_movie/all", "L_LOCKS"],
["tv_settings/channels/parental_controls/usa_tv/tv_y7/all", "L_LOCKS"],
["tv_settings/picture/more_picture/gamma", "2.4"],
["tv_settings/picture/more_picture/active_led_zones", "L_OFF_ON"],
["tv_settings/system/local_time_settings/daylight_savings_time", "L_AUTO_ON_OFF"],
["tv_settings/channels/parental_controls/usa_tv/tv_14/all", "L_LOCKS"],
["tv_settings/system/power_indicator", "L_OFF_ON"],
["tv_settings/channels/parental_controls/canadian_french/8_ans_plus", "L_LOCKS"],
["tv_settings/picture/more_picture/game_low_latency", "L_OFF_ON"],
["tv_settings/audio/surround_sound", "L_OFF_ON"],
["tv_settings/channels/parental_controls/usa_tv/tv_pg/all", "L_LOCKS"],
["tv_settings/system/reset_and_admin/store_demo", "L_DISABLED_ENABLED"],
["tv_settings/system/local_time_settings/time_format", "L_12H_24H_FORMAT"],
["tv_settings/network/manual_setup/dhcp", "L_DHCP_MODES"],
["tv_settings/channels/parental_controls/canadian_english/14_plus", "L_LOCKS"],
["tv_settings/system/reset_and_admin/smart_interactivity", "L_OFF_ON"],
["tv_settings/picture/more_picture/film_mode", "L_OFF_ON"],
["tv_settings/picture/color_calibration/calibration_tests/ramp_test_pattern", "L_COLORS"],
["tv_settings/system/local_time_settings/use_internet_time", "L_OFF_ON"],

["tv_settings/closed_captions/digital_style/text_style", "L_CLOSED_CAPTION_TEXT_STYLE"],
["tv_settings/closed_captions/digital_style/text_size", "L_CLOSED_CAPTION_TEXT_SIZE"],
["tv_settings/closed_captions/digital_style/text_color", "L_CLOSED_CAPTION_COLORS"],
["tv_settings/closed_captions/digital_style/text_opacity", "L_CLOSED_CAPTION_OPACITY"],
["tv_settings/closed_captions/digital_style/text_edges", "L_CLOSED_CAPTION_TEXT_EDGES"],
["tv_settings/closed_captions/digital_style/text_edges_color", "L_CLOSED_CAPTION_COLORS"],
["tv_settings/closed_captions/digital_style/background_color", "L_CLOSED_CAPTION_COLORS"],
["tv_settings/closed_captions/digital_style/background_opacity", "L_CLOSED_CAPTION_OPACITY"],
["tv_settings/closed_captions/digital_style/window_color", "L_CLOSED_CAPTION_COLORS"],
["tv_settings/closed_captions/digital_style/window_opacity", "L_CLOSED_CAPTION_OPACITY"],


["tv_settings/channels/parental_controls/usa_movie/r", "L_LOCKS"],
["tv_settings/channels/parental_controls/canadian_french/g", "L_LOCKS"],
["tv_settings/picture/more_picture/clear_action", "L_OFF_ON"],
["tv_settings/channels/parental_controls/usa_movie/g", "L_LOCKS"],
["tv_settings/channels/parental_controls/usa_tv/tv_y7/fv", "L_LOCKS"],
["tv_settings/channels/parental_controls/usa_tv/tv_14/s", "L_LOCKS"],
["tv_settings/channels/parental_controls/usa_tv/tv_14/v", "L_LOCKS"],
["tv_settings/channels/parental_controls/canadian_french/13_ans_plus", "L_LOCKS"],
["tv_settings/channels/parental_controls/usa_tv/tv_14/d", "L_LOCKS"],
["tv_settings/channels/parental_controls/usa_tv/tv_14/l", "L_LOCKS"],
["tv_settings/channels/parental_controls/canadian_english/18_plus", "L_LOCKS"],
["tv_settings/system/power_mode", "L_POWER_MODE"],
["tv_settings/system/aspect_ratio", "L_ASPECT_RATIOS"],
["tv_settings/audio/volume_leveling", "L_OFF_ON"],
["tv_settings/system/cec/cec_function", "L_CEC_MODES"],
["tv_settings/picture/more_picture/reduce_signal_noise", "L_OFF_LOW_MED_HI"],
["tv_settings/channels/parental_controls/usa_movie/pg", "L_LOCKS"],
["tv_settings/picture/color_calibration/color_tuner/color_tuner_table/header/red", "L_OFF_ON"],
["tv_settings/picture/color_calibration/calibration_tests/smpte_test_pattern", "L_OFF_ON"],
["tv_settings/channels/analog_audio", "L_TUNER_ANALOG_AUDIO"],
["tv_settings/audio/digital_audio_out", "L_DIGITAL_AUDIO"],
["tv_settings/channels/parental_controls/usa_tv/tv_pg/s", "L_LOCKS"],
["tv_settings/channels/parental_controls/canadian_english/pg", "L_LOCKS"],
["tv_settings/audio/analog_audio_out", "L_ANALOG_AUDIO"],
["tv_settings/timers/sleep_timer", "L_SLEEP_TIMER"],
["tv_settings/channels/parental_controls/usa_tv/tv_ma/l", "L_LOCKS"],
["tv_settings/picture/auto_brightness_control", "L_OFF_LOW_MED_HI"],
["tv_settings/closed_captions/digital_closed_captions", "L_OFF_ON"],
["tv_settings/picture/picture_mode_edit/lock_unlock_picture_mode", "L_OFF_ON"],
["tv_settings/picture/color_calibration/calibration_tests/uniformity_analyzer_test_pattern", "L_OFF_ON"],
["tv_settings/system/local_time_settings/country", "L_COUNTRIES"],
["tv_settings/picture/more_picture/reduce_block_noise", "L_OFF_LOW_MED_HI"],
["tv_settings/closed_captions/closed_captions_enabled", "L_OFF_ON"],
["tv_settings/audio/mute", "L_OFF_ON"],
["tv_settings/channels/parental_controls/usa_tv/tv_ma/all", "L_LOCKS"],
["tv_settings/channels/parental_controls/usa_tv/tv_ma/s", "L_LOCKS"],
["tv_settings/picture/more_picture/color_space", "L_COLOR_SPACE"],
["tv_settings/picture/more_picture/backlight_control","L_OFF_ON"],
["tv_settings/channels/parental_controls/usa_tv/tv_ma/v", "L_LOCKS"],
["tv_settings/channels/parental_controls/locks", "L_OFF_ON"],
["dolby/metadata/enable", "L_OFF_ON"],
["native/lut/3d/bypass", "L_OFF_ON"],
["pattern/enable", "L_OFF_ON"],
["native/lut/gamma/bypass", "L_OFF_ON"],
["state/audio/is_sound_bar_connected", "L_OFF_ON_UNKNOWN"],

["tv_settings/system/accessibility/talkback", "L_OFF_ON"],
["tv_settings/system/accessibility/speach_rate", "L_SPEECH_RATE"],
["tv_settings/system/accessibility/zoom_mode", "L_OFF_ON"],

]


def trlisttype(object, type, lang, ret, value):
    l = vars(object).get(type)
    if l != None:
        debug(l)
        if isinstance(l, list):
            debug(l)
            debug(len(l))
            assert len(l) == LANGUAGE_COUNT
            if isinstance(l[lang], list):
                debug(l[lang])
                if value in l[lang]:
                    p = l[lang].index(value)
                    debug(p)
                    if p >= 0:
                        return l[ret][p]
            else:
                debug(type(l[lang]))
                if l[lang] == value:
                    debug(l[lang])
                    return l[ret]
    return None

def trlist(object, type, lang, ret, value):
    if isinstance(type, list) :
        for t in type :
            r = trlisttype(object, t, lang, ret, value)
            if r != None :
                return r
    else :
        return trlisttype(object, type, lang, ret, value)


def findtype(uri):
    for t in translation:
        if t[0] == uri :
            return t[1]
    for t in translation_wildcard:
        if uri.startswith(t[0]) :
            return t[1]
    return None

def listtr(uri, lang, ret, value):
    type = findtype(uri)
    if type == None :
        return None
    logger.info('enter python listtr lang =%d ,ret = %d, value =%s ', lang,ret,value)
    return trlist(LanguageLists, type, lang, ret, value)


def current():
    return settings.LANG


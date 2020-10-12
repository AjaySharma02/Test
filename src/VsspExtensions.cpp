//
//  VsspExt.c
//  VIZIO Second Screen Protocol Python Extensions
//
//  Created by Bill Baxter on 3/30/15.
//  Copyright (c) 2015 Bill Baxter. All rights reserved.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <vector>
#include <memory>
#include <iostream>

#include <math.h>
#include <float.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>

#include <Python.h>
#include <Vssp_ipc.h>
#include <json/json.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "method.h"
#include "errorcodes.h"
#include "uri.h"
#include "uri_to_msgid.h"

#define MTK_KEY_SEND "ir.rx.send "
#define MAX_CMD_LEN  32
#define STORAGE_LENGTH 40

#define LANGUAGE_COUNT 3
#define COUNTRY_COUNT 3

/**************************     SCPL define     **************************/

/**************************** Socket ****************************/
typedef struct sockaddr* saddrp;
static    char *p_test_header = "vssp";
/**************************     SCPL define     **************************/

#define MAX_SCPL_URI_NUMBER 200       //we assume max uri number is 200 ,you can modify it according your fact
#define MAX_SCPL_PO_FILE_ONE_LINE_LEN 90        // assume in po file max line length is 90
#define MAX_PO_FILE_SIZE   60 * 1024      //allocate 60Kb for po file ,do not use malloc for avoid memory fragments
#define MAX_PO_FILE_MSGID_NUM 800   //we assume max msgid number is 800 ,you can modify it according your fact

typedef struct 
{
    char *p_msgid;
    char *p_msgstr;
}PO_MSG_ST;

typedef struct 
{
    int    i4_language_msgid[LANGUAGE_COUNT];  //each language msgid number
    PO_MSG_ST pt_po_msgid_and_msgstr[LANGUAGE_COUNT][MAX_PO_FILE_MSGID_NUM];   //.po file symbols 
}PO_ST;

PO_ST t_po_file;

static    char p_arr_po_file_buffer[MAX_PO_FILE_SIZE];
static    int i4_is_scpl_init_succeed = 0; //init succeed :1 ,faild -1 ,default 0

/**************************     SCPL define     **************************/



static Py_UNICODE Languages[LANGUAGE_COUNT][STORAGE_LENGTH]={0};
static Py_UNICODE CountriesUSA[COUNTRY_COUNT][STORAGE_LENGTH]={0};
static Py_UNICODE CountriesESP[COUNTRY_COUNT][STORAGE_LENGTH]={0};
static Py_UNICODE CountriesFRA[COUNTRY_COUNT][STORAGE_LENGTH]={0};

enum
{
    REST_INVALID = 0,
    REST_REQUEST_GET,
    REST_REQUEST_SET,
    REST_REQUEST_RESET,
    REST_REQUEST_ACTION,
    REST_RESPONSE_FROM_TV
} REST_TYPE;

static REST_MSG_BUF msg = { 0 };

enum _codeset
{
    ASCII = 0,
    KEY_MODIFIER,
    TRANSPORT,
    DPAD,
    NAV,
    AUDIO,
    VIDEO,
    INPUT,
    CH,
    COLOR,
    LAUNCH,
    PWR,
    THREED,
    CLOSED_CAPTION,
    FACTORY
} key_codeset;

// Must maintain consistency with Python handlers/Pairing.py challenge class
enum CHALLENGE_TYPES {
    JUST_WORKS = 0
    ,FOUR_DIGIT_BASE10_PIN = 1
    ,FOUR_DIGIT_BASE16_PIN = 2
    ,EIGHT_DIGIT_BASE10_PIN = 3
    ,EIGHT_DIGIT_BASE16_PIN = 4
    ,SUB_AUDIBLE_TONE = 5
    ,VOLUME_UP_BUTTON = 6
    ,PLAY_BUTTON = 7
};

typedef struct _CLI_KEY_MAP
{
    char key_name[15];
    char key_value[8];
} CLI_KEY_MAP;

typedef struct _CLI_KB_KEY_MAP
{
    int code;
    char key_name[10];
    char key_value[8];
} CLI_KB_KEY_MAP;

static CLI_KB_KEY_MAP t_cli_kb_key_map[] =
{
	{48,             "0",               "0x10030"},
	{49,             "1",               "0x10031"},
	{50,             "2",               "0x10032"},
	{51,             "3",               "0x10033"},
	{52,             "4",               "0x10034"},
	{53,             "5",               "0x10035"},
	{54,             "6",               "0x10036"},
	{55,             "7",               "0x10037"},
	{56,             "8",               "0x10038"},
	{57,             "9",               "0x10039"},

	{97,             "a",                "0xc0061"},
	{98,             "b",                "0xc0062"},
	{99,             "c",                "0xc0063"},
	{100,           "d",                "0xc0064"},
	{101,	    "e",		  "0xc0065"},
	{102,	    "f",		  "0xc0066"},
	{103,	    "g",		  "0xc0067"},
	{104,	    "h",	          "0xc0068"},
	{105,           "i",                 "0xc0069"},
	{106,           "j",                 "0xc006A"},
	{107,           "k",                "0xc006B"},
	{108,           "l",                 "0xc006C"},
	{109,           "m",               "0xc006D"},
	{110,           "n",                "0xc006E"},
	{111,           "o",                "0xc006F"},
	{112,           "p",                "0xc0070"},
	{113,           "q",                "0xc0071"},
	{114,           "r",                "0xc0072"},
	{115,           "s",                "0xc0073"},
	{116,           "t",                "0xc0074"},
	{117,           "u",                "0xc0075"},
	{118,           "v",                "0xc0076"},
	{119,           "w",               "0xc0077"},
	{120,           "x",                "0xc0078"},
	{121,           "y",                "0xc0079"},
	{122,           "z",                "0xc007A"},

	{65,             "A",                "0xc0041"},
	{66,             "B",                "0xc0042"},
	{67,             "C",                "0xc0043"},
	{68,             "D",                "0xc0044"},
	{69,	    	    "E",		  "0xc0045"},
	{70,	            "F",		  "0xc0046"},
	{71,	            "G",		  "0xc0047"},
	{72,	            "H",	          "0xc0048"},
	{73,             "I",                 "0xc0049"},
	{74,             "J",                 "0xc004A"},
	{75,             "K",                "0xc004B"},
	{76,             "L",                 "0xc004C"},
	{77,             "M",               "0xc004D"},
	{78,             "N",                "0xc004E"},
	{79,             "O",                "0xc004F"},
	{80,             "P",                "0xc0050"},
	{81,             "Q",                "0xc0051"},
	{82,             "R",                "0xc0052"},
	{83,             "S",                "0xc0053"},
	{84,             "T",                "0xc0054"},
	{85,             "U",                "0xc0055"},
	{86,             "V",                "0xc0056"},
	{87,             "W",               "0xc0057"},
	{88,             "X",                "0xc0058"},
	{89,             "Y",                "0xc0059"},
	{90,             "Z",                "0xc005A"},

	{8,             "DEL",                "0xc0008"},
	{64,             "@",                "0xc0040"},
	{35,             "#",                "0xc0023"},
	{36,             "$",                "0xc0024"},
	{95,             "_",               "0xc005F"},
	{38,             "&",                "0xc0026"},
	{45,             "-",                "0xc002D"},
	{43,             "+",                "0xc002B"},
	{40,             "(",		   "0xc0028"},
	{41,	  	    ")",		  "0xc0029"},
	{47,		     "/",		 "0xc002F"},
	{34,		      "QN",		 "0xc0022"},
	{39,		      "'",		  "0xc0027"},
	{58,		      ":",		 "0xc003A"},
	{59,		      ";",		 "0xc003B"},
	{33,			"!",		"0xc0021"},
	{63,			 "?",		   "0xc003F"},
	{126,		"~",		  "0xc007E"},
	{124,	       "|",		 "0xc007C"},
	{61,			 "=",		 "0xc003D"},
	{92,			  "", 	 	"0xc005C"},
	{123,		"{",		 "0xc007B"},
	{125,		"}",		 "0xc007D"},
	{91,			"[",		 "0xc005B"},
	{93,			"]",		 "0xc005D"},
	{62,			 ">",	 	"0xc003E"},
	{60,			 "<",	  	"0xc003C"},
	{37,			 "%",	  "0xc0025"},
	{94,			 "^",	  	"0xc005E"},
	{42,			 "*",		"0xc002A"},
	{44,			 ",",		"0xc002C"},
	{46,			 ".",		"0xc002E"},
//{46,			 "",		"0xc0060"},'
	{0,       "CUSTOMKEY",		   "CUSTOM"}
};
static CLI_KEY_MAP t_cli_key_map[] =
{
    {"0",               "0x10030"},
    {"1",               "0x10031"},
    {"2",               "0x10032"},
    {"3",               "0x10033"},
    {"4",               "0x10034"},
    {"5",               "0x10035"},
    {"6",               "0x10036"},
    {"7",               "0x10037"},
    {"8",               "0x10038"},
    {"9",               "0x10039"},
    {".",               "0x1f004"},
    {"MINUS",           "0xc002d"},

    {"FAST_FORWARD",    "0x9f007"},
    {"FAST_REVERSE",    "0x9f008"},
    {"PAUSE",           "0x9f001"},
    {"PLAY",            "0x9f000"},
    {"RECORD",          "0x9f004"},
    {"SLOW_FORWARD",    "0x9f007"},
    {"SLOW_REVERSE",    "0x9f008"},
    {"STEP_BACK",       "0x9f008"},
    {"STEP_FORWARD",    "0x9f007"},
    {"STOP",            "0x9f002"},
    {"TRACK_FORWARD",   "0x9f005"},
    {"TRACK_REVERSE",   "0x9f006"},

    {"LEFT",            "0x3f000"},
    {"RIGHT",           "0x3f001"},
    {"UP",              "0x3f002"},
    {"DOWN",            "0x3f003"},

    {"MENU",            "0x8f001"},
    {"INFO",            "0x5f017"},
    {"GUIDE",           "0x8f002"},
    {"ENTER",           "0x4f000"},
    {"EXIT",            "0x4f001"},
    {"BACK",            "0x4f004"},

    {"VOL-",            "0x6f001"},
    {"VOL+",            "0x6f000"},
    {"MUTE",            "0x6f002"},

    {"P_EFFECT",        "0x5f016"},
    {"WIDE_MODE",       "0x5f015"},

    {"INPUT",           "0x5f01f"},

    {"CH+",             "0x5f000"},
    {"CH-",             "0x5f001"},
    {"PRE-CH",          "0x5f002"},

    {"RED",             "0xbf000"},
    {"GREEN",           "0xbf001"},
    {"YELLOW",          "0xbf002"},
    {"BLUE",            "0xbf003"},

    {"POWER",           "0x8f000"},

    {"CC",              "0x5f01a"},

    {"WIDGET",          "0xef000"},
    {"LINK",            "0xef000"},

    {"CUSTOMKEY",       "CUSTOM"}
};

static int ir_rx_send(char* key_value)
{
    int  handle  = 0;
    char command[MAX_CMD_LEN];
    char c_key;
    int  i   = 0;
    int  key_len = 0;
    int  b_is_key = 0;

    if( !key_value )
    {
        return -1;
    }

    if( (handle = open("/dev/cli",O_RDWR)) < 0 )
    {
        printf("[ReST Glue] Can not open the cli device \n");
        return -1;
    }

    memset(command,'\0', MAX_CMD_LEN);

    for(i=0;( strcmp(t_cli_key_map[i].key_name,"CUSTOMKEY") != 0 );i++)
    {
        if( ( strcmp(t_cli_key_map[i].key_name, key_value)==0 ) )
        {
            strcpy(command, MTK_KEY_SEND);
            strcat(command, t_cli_key_map[i].key_value);
            b_is_key = 1;
            break;
        }
    }

    if( !b_is_key )
    {
        return -1;
    }
    else
    {
        key_len = strlen(command);

        for(i=0; i<key_len; i++)
        {
            c_key = command[i];
            ioctl(handle, 0, &c_key);
        }
    }

    c_key = '\r';
    ioctl(handle, 0, &c_key);
    c_key = '\n';
    ioctl(handle, 0, &c_key);
    close(handle);
    return 0;
}

static int ir_rx_kb_send(int code)
{
    int  handle  = 0;
    char command[MAX_CMD_LEN];
    char c_key;
    int  i   = 0;
    int  key_len = 0;
    int  b_is_key = 0;

    if( (handle = open("/dev/cli",O_RDWR)) < 0 )
    {
        printf("[ReST Glue] Can not open the cli device \n");
        return -1;
    }
    memset(command,'\0', MAX_CMD_LEN);

    for(i=0;( strcmp(t_cli_kb_key_map[i].key_name,"CUSTOMKEY") != 0 );i++)
    {
        if( t_cli_kb_key_map[i].code== code )
        {
	        printf("[ReST Glue] %s,%d,key_name: %s,key:%s\n", __FUNCTION__, __LINE__,t_cli_kb_key_map[i].key_name,t_cli_kb_key_map[i].key_value);
            strcpy(command, MTK_KEY_SEND);
            strcat(command, t_cli_kb_key_map[i].key_value);
            b_is_key = 1;
            break;
        }
    }

    if( !b_is_key )
    {
        return -1;
    }
    else
    {
        key_len = strlen(command);

        for(i=0; i<key_len; i++)
        {
            c_key = command[i];
            ioctl(handle, 0, &c_key);
        }
    }

    c_key = '\r';
    ioctl(handle, 0, &c_key);
    c_key = '\n';
    ioctl(handle, 0, &c_key);
    close(handle);
    return 0;
}

int key_set(int codeset, int code)
{
    printf("[ReST Glue] %s,%d\n", __FUNCTION__, __LINE__);
    usleep(20*1000);   //dolphin 20200611 add this for driver could not handle the key event so rapidly and would ignore some of them ,so we wait some time to slow down key send speed

    int errCode = API_ERR_SUCCESS;

    switch (codeset)
    {
        case ASCII:
	    ir_rx_kb_send(code);
	    break;

        case KEY_MODIFIER:
            break;

        case TRANSPORT:
        {
            switch(code)
            {
                case 0:
                    printf("[ReST Glue] FAST_FORWARD\n");
                    ir_rx_send((char*)"FAST_FORWARD");
                    break;
                case 1:
                    printf("[ReST Glue] FAST_REVERSE\n");
                    ir_rx_send((char*)"FAST_REVERSE");
                    break;
                case 2:
                    printf("[ReST Glue] PAUSE\n");
                    ir_rx_send((char*)"PAUSE");
                    break;
                case 3:
                    printf("[ReST Glue] PLAY\n");
                    ir_rx_send((char*)"PLAY");
                    break;
                case 4:
                    ir_rx_send((char*)"RECORD");
                    break;
                case 5:
                    printf("[ReST Glue] SLOW_FORWARD\n");
                    ir_rx_send((char*)"SLOW_FORWARD");
                    break;
                case 6:
                    printf("[ReST Glue] SLOW_REVERSE\n");
                    ir_rx_send((char*)"SLOW_REVERSE");
                    break;
                case 7:
                    printf("[ReST Glue] STEP_BACK\n");
                    ir_rx_send((char*)"STEP_BACK");
                    break;
                case 8:
                    printf("[ReST Glue] STEP_FORWARD\n");
                    ir_rx_send((char*)"STEP_FORWARD");
                    break;
                case 9:
                    printf("[ReST Glue] STOP\n");
                    ir_rx_send((char*)"STOP");
                    break;
                case 10:
                    printf("[ReST Glue] TRACK_FORWARD\n");
                    ir_rx_send((char*)"TRACK_FORWARD");
                    break;
                case 11:
                    printf("[ReST Glue] TRACK_REVERSE\n");
                    ir_rx_send((char*)"TRACK_REVERSE");
                    break;
                default:
                    errCode = API_ERR_BLOCKED;
                    break;
            }
            break;
        }

        case DPAD:
        {
            switch(code)
            {
                case 0:
                {
                    ir_rx_send((char*)"DOWN");
                    break;
                }
                case 1:
                {
                    ir_rx_send((char*)"LEFT");
                    break;
                }
                case 2:
                {
                    ir_rx_send((char*)"ENTER");
                    break;
                }
                case 7:
                {
                    ir_rx_send((char*)"RIGHT");
                    break;
                }
                case 8:
                {
                    ir_rx_send((char*)"UP");
                    break;
                }
                default:
                {
                    errCode = API_ERR_BLOCKED;
                    break;
                }
            }
            break;
        }

        case NAV:
        {
            switch(code)
            {
                case 0:
                {
                    ir_rx_send((char*)"BACK");
                    break;
                }
                case 3:
                {
                    ir_rx_send((char*)"EXIT");
                    break;
                }
                case 6:
                {
                    ir_rx_send((char*)"INFO");
                    break;
                }
                case 8:
                {
                    ir_rx_send((char*)"MENU");
                    break;
                }
                case 15:
                {
                    ir_rx_send((char*)"WIDGET");
                    break;
                }
                case 16:
                {
                    ir_rx_send((char*)"LINK");
                    break;
                }
                default:
                {
                    errCode = API_ERR_BLOCKED;
                    break;
                }
            }
            break;
        }

        case AUDIO:
        {
            switch(code)
            {
                case 0:
                {
                    //VOL-
                    ir_rx_send((char*)"VOL-");
                    break;
                }
                case 1:
                {
                    //VOL+
                    ir_rx_send((char*)"VOL+");
                    break;
                }
                case 2:
                case 3:
                case 4:
                {
                    //MUTE_OFF, MUTE_ON, MUTE_TOGGLE
                    ir_rx_send((char*)"MUTE");
                    break;
                }
                default:
                {
                    errCode = API_ERR_BLOCKED;
                    break;
                }
            }
            break;
        }

        case VIDEO:
        {
            switch(code)
            {
                case 0:
                {
                    //PIC
                    ir_rx_send("P_EFFECT");
                    break;
                }
                case 1:
                case 2:
                {
                    //WIDE
                    ir_rx_send((char*)"WIDE_MODE");
                    break;
                }
                default:
                {
                    errCode = API_ERR_BLOCKED;
                    break;
                }
            }
            break;
        }

        case INPUT:
            switch(code)
            {
                case 1:
                    ir_rx_send((char*)"INPUT");
            break;
                default:
                {
                    break;
                }
            }
            break;
        case CH:
        {
            switch(code)
            {
                case 0:
                {
                    //CH-
                    ir_rx_send((char*)"CH-");
                    break;
                }
                case 1:
                {
                    //CH+
                    ir_rx_send((char*)"CH+");
                    break;
                }
                case 2:
                {
                    //PRE
                    ir_rx_send((char*)"PRE-CH");
                    break;
                }
                default:
                {
                    errCode = API_ERR_BLOCKED;
                    break;
                }
            }
            break;
        }

        case COLOR:
            break;

        case LAUNCH:
            break;

        case PWR:
        {
            switch(code)
            {
                case 2:
                {
                    //POWER
                    ir_rx_send((char*)"POWER");
                    break;
                }
                default:
                {
                    errCode = API_ERR_BLOCKED;
                    break;
                }
            }
            break;
        }

        case THREED:
            break;

        case CLOSED_CAPTION:
        {
            switch(code)
            {
                case 1:
                {
                    ir_rx_send((char*)"CC");
                    break;
                }
                default:
                {
                    errCode = API_ERR_BLOCKED;
                    break;
                }
            }
            break;
        }

        case FACTORY:
            break;

        default:
            errCode = API_ERR_BLOCKED;
            break;
    }

    return errCode;
}

int josnRpcCppSend( int type, std::string uri, Json::Value & message)
{
    Json::Value query;
    Json::FastWriter writer;
    std::string queryStr;

    std::cout << "josnRpcCppSend uri: " << uri << std::endl;

    int N = 0;

    int id = uri::uritoid(uri, N);


    std::cout << "josnRpcCppSend id: " << id << std::endl;

    /* build JSON-RPC query */
    query["jsonrpc"] = "2.0";
    query["id"] = id;
    query["type"] = type;
    query["method"] = "method";
    query["uri"] = uri;
    query["message"] = message;
    query["N"] = N;


    queryStr = writer.write(query);

    std::cout << "[ReST Glue] Query is: " << queryStr << std::endl;

	int size = queryStr.length();
	int index = 0;
	char * p = (char *)queryStr.c_str();

	do {
		memset(&msg, 0, sizeof(REST_MSG_BUF));


		msg.total_size = queryStr.length();

		if(size > REST_BUFFER_DATA_SIZE) {
			memcpy(msg.data_str, p+index, REST_BUFFER_DATA_SIZE);
			index += REST_BUFFER_DATA_SIZE;
			size -= REST_BUFFER_DATA_SIZE;
		} else {
			memcpy(msg.data_str, p+index, size);
			index += size;
			size = 0;
		}

		msg.seq = index;

		if (Vssp_send_msg_2_rest(&msg) == -1)
		{
			std::cout << "[ReST Glue] Send fail" << std::endl;
			std::cerr << "[ReST Glue] Error while sending data!" << std::endl;
			return EXIT_FAILURE;
		}

	} while (size != 0);
    std::cout << "[ReST Glue] Send OK" << std::endl;

    return EXIT_SUCCESS;
}

int josnRpcCppSend_ex( int type, std::string uri, Json::Value & message, std::string prop)
{
    Json::Value         query;
    Json::FastWriter    writer;
    std::string         queryStr;

    std::cout << "josnRpcCppSend_ex uri: " << uri << std::endl;

    int                 N = 0;
    int                 id = uri::uritoid(uri, N);
    std::cout << "josnRpcCppSend_ex id: " << id << std::endl;

    /* build JSON-RPC query */
    query["jsonrpc"] = "2.0";
    query["id"] = id;
    query["type"] = type;
    query["method"] = "method";
    query["uri"] = uri;
    query["message"] = message;
    query["N"] = N;
    query["airplay_prop"] = prop;
    queryStr = writer.write(query);

    std::cout << "[ReST Glue] Query is: " << queryStr << std::endl;

	int     size = queryStr.length();
	int     index = 0;
	char    *p = (char*)queryStr.c_str();

	do {
		memset(&msg, 0, sizeof(REST_MSG_BUF));
		msg.total_size = queryStr.length();
		if (size > REST_BUFFER_DATA_SIZE) {
			memcpy(msg.data_str, p+index, REST_BUFFER_DATA_SIZE);
			index += REST_BUFFER_DATA_SIZE;
			size -= REST_BUFFER_DATA_SIZE;
		} else {
			memcpy(msg.data_str, p+index, size);
			index += size;
			size = 0;
		}

		msg.seq = index;
		if (Vssp_send_msg_2_rest(&msg) == -1)
		{
			std::cout << "[ReST Glue] Send fail" << std::endl;
			std::cerr << "[ReST Glue] Error while sending data!" << std::endl;
			return EXIT_FAILURE;
		}
	} while (size != 0);

    std::cout << "[ReST Glue] Send OK" << std::endl;

    return EXIT_SUCCESS;
}

Json::Value josnRpcCppRecv(void) {

    
    Json::Value root;

	std::string responseStr;

	while (true) {
		memset(&msg, 0, sizeof(REST_MSG_BUF));
		int ret = Vssp_get_msg_from_rest(&msg);
		printf("[ReST Glue] Vssp_get_msg_from_rest : %d %d \n", msg.seq, msg.total_size);
		if (VSSP_IPC_R_OK != ret) {
			//std::cerr << "Error while receiving data!" << std::endl;
			root["SUCCESS"] = 1;
			return root;
		}
		responseStr.append(msg.data_str);
		if (msg.seq == msg.total_size) {
			break;
		}
	}

    
    Json::Reader reader;
    if (reader.parse(responseStr, root))
    {

        std::string uri = root["uri"].asString();       
        int id = root["id"].asInt();
        Json::Value result = root["result"];

        std::cout << "[ReST Glue] uri : " << uri << std::endl;
        std::cout << "[ReST Glue] id : " << id << std::endl;

        Json::FastWriter writer;
        std::cout << "[ReST Glue] result : " << writer.write(result) << std::endl;
        return result;
    }
    root["SUCCESS"] = 1;
    return root;
}

static int VsspExt_GetIndex(PyObject* obj)
{
    Py_ssize_t size = PyUnicode_GET_SIZE(obj);
    Py_UNICODE* valueStr = PyUnicode_AS_UNICODE(obj);

    /* TODO : not check Languages or Countries size*/

    for(int i = 0 ; i < 3; i++)
    {
        if(!(memcmp( (const Py_UNICODE*)valueStr, (const Py_UNICODE*)Languages[i],
            size*sizeof(Py_UNICODE))))
        {
            return i;
        }

        if(!(memcmp( (const Py_UNICODE*)valueStr, (const Py_UNICODE*)CountriesUSA[i], 
            size*sizeof(Py_UNICODE))))
        {
            return i;
        }        
        
        if(!(memcmp( (const Py_UNICODE*)valueStr, (const Py_UNICODE*)CountriesESP[i], 
            size*sizeof(Py_UNICODE))))
        {
            return i;
        }        

        if(!(memcmp( (const Py_UNICODE*)valueStr, (const Py_UNICODE*)CountriesFRA[i], 
            size*sizeof(Py_UNICODE))))
        {
            return i;
        } 
               
    }
    return -1;
}

static Json::Value pythonToJson(PyObject* obj)
{
    Json::Value data;
    PyObject* subObj = NULL;
    Json::Value subItem;
    Py_ssize_t size = 0;
    PyObject * key = NULL;
    Py_ssize_t pos = 0;


    if (Py_None == obj)
    {
        printf("[ReST Glue] obj is py_none\n");
    }
    else if (PyBool_Check(obj))
    {
        //printf("[ReST Glue] obj is bool\n");//Py_False,Py_True
        data = (obj == Py_True) ? true : false;
    }
    else if (PyInt_CheckExact(obj))
    {
        //printf("[ReST Glue] obj is int \n");
        data = (int)PyInt_AsLong(obj);
    }
    else if (PyFloat_CheckExact(obj))
    {
        //printf("[ReST Glue] obj is float \n");
        data = PyFloat_AsDouble(obj);
    }
    else if (PyUnicode_CheckExact(obj))
    {
        //printf("[ReST Glue] obj is Unicode string\n");

        /*int index = VsspExt_GetIndex(obj);
        if(index != -1) {
            data = index;
        }else {*/
            PyObject* uf8Obj = PyUnicode_AsUTF8String(obj);
            data = (PyString_AsString(uf8Obj));
        //}        
    }
    else if (PyString_CheckExact(obj))
    {
        //printf("[ReST Glue] obj is string\n");
        data = (PyString_AsString(obj));
    }
    else if (PyList_CheckExact(obj))
    {
        //printf("[ReST Glue] obj is list\n");
        size = PyList_Size(obj);
        for (int i = 0;i < size;i++)
        {
            subObj = PyList_GetItem(obj, i);
            data[i] = pythonToJson(subObj);
        }       
    }
    else if (PyDict_CheckExact(obj))
    {
        //printf("[ReST Glue] obj is dictionary\n");

        while (PyDict_Next(obj, &pos, &key, &subObj))
        {
            subItem = pythonToJson(subObj);
            data[PyString_AsString(key)] = subItem;
        }
    }
    else
    {
        //printf("[ReST Glue] obj is unknown\n");
    }
    return data;
}

static PyObject * jsonToPython(Json::Value data)
{
    PyObject * py = NULL;
    Json::Value child;
    PyObject * child_py = NULL;

    //printf("[ReST Glue] enter jsonToPython\n");

    if (data.isBool())
    {
        //printf("[ReST Glue] ....create bool\n");
        py =  (data.asBool() == true) ? Py_True : Py_False;
    } 
    else if (data.isNull())
    {
        //printf("[ReST Glue] ....create null\n");
        py = Py_None;
    }
    else if (data.isInt())
    {
        //printf("[ReST Glue] ....create int\n");
        py = Py_BuildValue("i", data.asInt());
    }
    else if (data.isDouble())
    {
        //printf("[ReST Glue] ....create double\n");
        py = Py_BuildValue("d", data.asDouble());
    }
    else if (data.isString())
    {
        //printf("[ReST Glue] ....create string\n");
        py = Py_BuildValue("s", data.asCString());
        py = PyUnicode_FromString(data.asCString());
        if (py == NULL)
        {
            //printf("[ReST Glue] ....create string fail\n");
            py = PyUnicode_FromString("Unknown name");
        }
    }
    else if (data.isArray())
    {
        //printf("[ReST Glue] ....create list\n");
        py = PyList_New(0);
        //printf("[ReST Glue] ....list size %d\n", data.size());
        for (int i = 0; i<data.size(); i++) {
            //std::cout << "\t\t\tarr:" << data[i] << std::endl;
            child_py = jsonToPython(data[i]);
            if (child_py == NULL) {
                break;
            }
            //printf("[ReST Glue] append a child into list\n");
            PyList_Append(py, child_py);
            //Py_DECREF(child_py);
        }       
    }
    else if (data.isObject())
    {
        //printf("[ReST Glue] ....create dict\n");
        py = PyDict_New();

        Json::Value::Members mems = data.getMemberNames();
        for (Json::Value::Members::iterator it = mems.begin(); it != mems.end(); it++)
        {
            std::string name = *it;
            //std::cout << "key:" << name << "\tvalue:" << data[name] << std::endl;
            Json::Value arrJson = data[name];
            child_py = jsonToPython(arrJson);
            if (child_py != NULL) {
                //printf("[ReST Glue] set a pair into dict\n");
                PyDict_SetItemString(py, name.c_str(), child_py);
            } else {
                //printf("[ReST Glue] ....child_py is null\n");
            }
            //Py_DECREF(child_py);
        }
    }

    return py;
}

#ifdef __cplusplus
extern "C" {
#endif

static PyObject* pyModule = NULL;

PyObject* callPython( const char * function, const char *format, ... )
{
	PyObject* pyMethod = NULL;
	PyObject* PyArgument = NULL;
	PyObject* PyReturn = NULL;

	if (pyModule == NULL) {
		printf("[ReST Glue] callPython fail, module is null\n");
		return NULL;
	}

	if (!(pyMethod = PyObject_GetAttrString(pyModule, function))) {
		printf("[ReST Glue] PyObject_GetAttrString fail\n");
		return NULL;
	}

	printf("[ReST Glue] callPython start\n");

	va_list vargs;
	va_start(vargs, format);
	PyArgument = Py_VaBuildValue(format, vargs);
	va_end(vargs);

	PyReturn = PyEval_CallObject( pyMethod, PyArgument);

	printf("[ReST Glue] callPython return\n");
	return PyReturn;
}

PyObject* transtion(const char *uri,int source, int dest, PyObject * srcLanguage)
{
	PyObject* destLanguage = callPython("listtr", "(siiO)", uri,source, dest, srcLanguage);
	return  destLanguage;
}

static int _current_country_index = 0; //default language index 0 -> 0(English) 1(Spanish) 2(Franch)

int get_current()
{
    printf("[ReST Glue] get current language \n");

    PyObject * ret = callPython("current", "()");

    int cur = PyInt_AsLong(ret);

    if(cur != _current_country_index)
        printf("[ReST Glue] current language not synced \n");

    return _current_country_index;
}

static wchar_t curLang[40] = L"";
static wchar_t curCountry[40] = L"";

static int get_language_index(const char * lang) {


		const char* Languages[] = {"English", "Español","Français"};
		for (int i = 0; i < 3; i++) {
			PyObject * pyObjLang = PyUnicode_FromString(Languages[i]);

			printf("[ReST Glue] pythonToJson: string = %s \n",Languages[i]);

			PyObject * utf8Obj = PyUnicode_AsUTF8String(pyObjLang);

			const char * data = (PyString_AsString(utf8Obj));

			if(strcmp(lang, data) == 0)
				return i;

		}

	return 0;
}

static const char * countries[][3] = {{"USA", "Canada", "Mexico"},
        {"EE.UU.", "Canadá", "México"},
        {"USA", "Canada", "Mexique"}};
static int get_country_index(const char * country) {
    for (int i = 0; i < 3; i++) {
        PyObject * pyObjLang = PyUnicode_FromString(countries[_current_country_index][i]);

        printf("[ReST Glue] pythonToJson: string = %s \n",countries[i]);

        PyObject * utf8Obj = PyUnicode_AsUTF8String(pyObjLang);

        const char * data = (PyString_AsString(utf8Obj));

        if(strcmp(country, data) == 0)
            return i;

    }

	return 0;

}

/**************************************************************************************************
* Function: 
*    int _vssp_is_should_be_translated(const char *p_uri ,char *p_fuzzy_msgid)
*    check whether uri need to be translate ,
* Input:
*   p_uri: user set/get uri for some uri need not be translated
*   p_fuzzy_msgid: in po file ,original python file will to converted to multi-msgid with "_0" "_1" "_2" ...pending
* Output:
*   s_output_json ,if succeed then will output 
* Return: 
*    faild: -1 succeed:n
* Author: 
*    haoyuan.li@mediatek.com
* Date: 
*    2019/10/14
**************************************************************************************************/
int _vssp_is_should_be_translated(const char *p_uri ,char *p_fuzzy_msgid)
{
    int i_count=0;
    int i_uri_num= sizeof(translation_text)/sizeof(char*);
    //printf("[ReST Glue] i_uri_num is %d\n",i_uri_num);

    //[uri][msgid] is double size of [msgid] so need to take twice
    while(i_count < i_uri_num && i_count < MAX_SCPL_URI_NUMBER*2 && i_uri_num > 0) 
    {
        if(p_uri==NULL || translation_text[i_count]==NULL)
        {
            continue;
        }

        int i4_dst_len=strlen(p_uri);
	 int i4_src_len=strlen(translation_text[i_count]);
        int i4_min_len = 0;
	 i4_min_len = i4_dst_len>i4_src_len?i4_src_len:i4_dst_len;

        if(0 == memcmp(p_uri,translation_text[i_count],i4_min_len))
        {
            strcpy(p_fuzzy_msgid,translation_text[i_count+1]);
            //printf("[ReST Glue]meets condition %s %s msgid %s to be translate\n",p_uri,translation_text[i_count] ,p_fuzzy_msgid);
            return i_count;
        }
        i_count +=2;
    }
    return -1;
}

/****************************************************************************************************************************************
* Function: 
*    int _vssp_look_for_translate(Json::Value s_input_json, char* p_fuzzy_msgid, int i4_src_language_index, int i4_dst_language_index, Json::Value *s_output_json)
*    use for translate value according to language index ,if faild just return -1 ,succeed return 0
* Input:
*   s_input_json:  user input json value waitting for translating
*   p_uri: user set/get uri for some uri need not be translated
*   i4_language_index: language index ->0:en 1:es 2:fr
*   p_fuzzy_msgid: in po file ,original python file will to converted to multi-msgid with "_0" "_1" "_2" ...pending
* Output:
*   s_output_json ,if succeed then will output 
* Return: 
*    faild: -1 succeed:0
* Author: 
*    haoyuan.li@mediatek.com
* Date: 
*    2019/10/14
******************************************************************************************************************************************/
int _vssp_look_for_translate(Json::Value s_input_json, char* p_fuzzy_msgid, int i4_src_language_index, int i4_dst_language_index, Json::Value *s_output_json)
{
    int i4_ret = -1;
    char p_msgid[MAX_SCPL_PO_FILE_ONE_LINE_LEN]={0};

    do
    {
         /****  dolphin firstly get src language msgid  ****/
        std::string s_value =  s_input_json["VALUE"].asString();
        //printf("[ReST Glue] _vssp_look_for_translate input value [%s] msgid num %d\n",s_value.c_str(),t_po_file.i4_language_msgid[0]);
        printf("_vssp_look_for_translate src value [%s]\n",s_value.c_str());

        for(int i4_count=0;i4_count<t_po_file.i4_language_msgid[i4_src_language_index];i4_count++)
        {
            int i4_dst_len=strlen(p_fuzzy_msgid);
            int i4_src_len=strlen(t_po_file.pt_po_msgid_and_msgstr[i4_src_language_index][i4_count].p_msgid);
            int i4_min_len = 0;
            i4_min_len = i4_dst_len>i4_src_len?i4_src_len:i4_dst_len;
            if(0 == memcmp(p_fuzzy_msgid,t_po_file.pt_po_msgid_and_msgstr[i4_src_language_index][i4_count].p_msgid,i4_min_len) && i4_min_len!=0)
            {
                //printf("[ReST Glue] _vssp_look_for_translate fuzzy&specific [%s][%s]\n",p_fuzzy_msgid,t_po_file.pt_po_msgid_and_msgstr[i4_src_language_index][i4_count].p_msgid);
                int i4_msgid_dst_len=strlen(s_value.c_str());
                int i4_msgid_src_len=strlen(t_po_file.pt_po_msgid_and_msgstr[i4_src_language_index][i4_count].p_msgstr);
                int i4_msgid_min_len = 0;
                i4_msgid_min_len = i4_msgid_dst_len>i4_msgid_src_len?i4_msgid_src_len:i4_msgid_dst_len;
                if(0 ==memcmp(s_value.c_str(),t_po_file.pt_po_msgid_and_msgstr[i4_src_language_index][i4_count].p_msgstr,i4_msgid_min_len) && i4_msgid_min_len!=0)
                {
                    //printf("[ReST Glue] _vssp_look_for_translate specific msgstr [%s] msgid[%s]\n",s_value.c_str(),t_po_file.pt_po_msgid_and_msgstr[i4_src_language_index][i4_count].p_msgid);
                    strcpy(p_msgid,t_po_file.pt_po_msgid_and_msgstr[i4_src_language_index][i4_count].p_msgid);
                    i4_ret = 0;
                    break;
                }
            }
        }
    }
    while(0);

    if(i4_ret!=0)
    {
        printf("[ReST Glue] get language index%d value [%s] Language faild\n",i4_src_language_index,s_input_json["VALUE"].asString().c_str());
        return -1;
    }
    else
    {
        /****  dolphin then change to destination language msgid  ****/
        i4_ret = -1;
        //printf("[ReST Glue] _vssp_look_for_translate msgid[%d] [%s] ,msgid num %d\n",i4_dst_language_index,p_msgid,t_po_file.i4_language_msgid[i4_dst_language_index]);
        for(int i4_count=0;i4_count<t_po_file.i4_language_msgid[i4_dst_language_index];i4_count++)
        {
            int i4_dst_len=strlen(p_msgid);
            int i4_src_len=strlen(t_po_file.pt_po_msgid_and_msgstr[i4_dst_language_index][i4_count].p_msgid);
            int i4_min_len = 0;
            i4_min_len = i4_dst_len>i4_src_len?i4_src_len:i4_dst_len;
            if(0 ==memcmp(p_msgid,t_po_file.pt_po_msgid_and_msgstr[i4_dst_language_index][i4_count].p_msgid,i4_min_len) && i4_min_len!=0)
            {
                (*s_output_json) = s_input_json;
                (*s_output_json)["VALUE"] = t_po_file.pt_po_msgid_and_msgstr[i4_dst_language_index][i4_count].p_msgstr ;
                i4_ret = 0;
                //printf("[ReST Glue] dest msgid[%d][%s] src msgid [%s] match msgstr [%s]\n",i4_dst_language_index,\
                  //                  t_po_file.pt_po_msgid_and_msgstr[i4_dst_language_index][i4_count].p_msgid,\
                   //                 p_msgid,\
                      //              t_po_file.pt_po_msgid_and_msgstr[i4_dst_language_index][i4_count].p_msgstr);
                printf("_vssp_look_for_translate dest value [%s]\n",(*s_output_json)["VALUE"].asString().c_str());
                break;
            }
        }
    }
    return i4_ret;
}

/************************************************************************************************************************************
* Function: 
*    Json::Value _vssp_translate_json(Json::Value s_input_json ,int i4_src_language_index, int i4_dst_language_index, const char *p_uri ,char *p_fuzzy_msgid)
*    use for translate value according to language index ,if faild just return input itself
* Input:
*   s_input_json:  user input json value waitting for translating
*   p_uri: user set/get uri for some uri need not be translated
*   i4_src_language_index: language index ->0:en 1:es 2:fr ,the source kind of language waitted for translate
*   i4_dst_language_index: language index ->0:en 1:es 2:fr ,the kind of language you want to translate to
*   p_fuzzy_msgid: in po file ,original python file will to converted to multi-msgid with "_0" "_1" "_2" ...pending
* Return: 
*    json: if succeed return translate json ,fail then return original input json
* Author: 
*    haoyuan.li@mediatek.com
* Date: 
*    2019/10/14
************************************************************************************************************************************/
Json::Value _vssp_translate_json(Json::Value s_input_json ,int i4_src_language_index, int i4_dst_language_index, const char *p_uri ,char *p_fuzzy_msgid)
{
    int i4_ret = -1;
    Json::Value j_translate;

    i4_ret =  _vssp_look_for_translate(s_input_json,p_fuzzy_msgid,i4_src_language_index,i4_dst_language_index,&j_translate);
    if(i4_ret!=0){
        printf("[ReST Glue] _vssp_translate_json faild\n");
        return s_input_json;
    }
    else{
        //printf("[ReST Glue] _vssp_translate_json succeed\n");
        return j_translate;
    }
}

/*******************************************************************************************************************
* Function: 
*    static Json::Value _vssp_translate(Json::Value s_input_json, const char *p_uri, int i4_src_language_index ,int i4_dst_language_index)
*    use for translate value according to language index ,if faild just return input itself
* Input:
*   s_input_json:  user input json value waitting for translating
*   p_uri: user set/get uri for some uri need not be translated
*   i4_src_language_index: language index ->0:en 1:es 2:fr ,the source kind of language waitted for translate
*   i4_dst_language_index: language index ->0:en 1:es 2:fr ,the kind of language you want to translate to
* Return: 
*    json: if succeed return translate json ,fail then return original input json
* Author: 
*    haoyuan.li@mediatek.com
* Date: 
*    2019/10/14
*******************************************************************************************************************/
static Json::Value _vssp_translate(Json::Value s_input_json, const char *p_uri, int i4_src_language_index ,int i4_dst_language_index)
{
    char p_fuzzy_msgid[MAX_SCPL_PO_FILE_ONE_LINE_LEN]={0};
    do
    {
        if(i4_src_language_index>2 || i4_src_language_index<0 ||i4_dst_language_index>2 ||i4_dst_language_index<0)  //unsupport language index
        {
            printf("[ReST Glue]vssp unrecognize language index\n");
            break;
        }
        if(i4_src_language_index==0 && i4_dst_language_index==0)  //default English ,need not to modify
        {
            //printf("[ReST Glue]no need to translate\n");
            break;
        }
        if(-1 != _vssp_is_should_be_translated(p_uri,p_fuzzy_msgid))
        {
            return _vssp_translate_json(s_input_json, i4_src_language_index,i4_dst_language_index, p_uri, p_fuzzy_msgid);
        }
	 else
	 {
            //printf("[ReST Glue] not in trante uri group, no need to translate");
            break;
	 }
    }
    while(0);
    return s_input_json;
}

/***************************************************************************
* Function: 
*    int _vssp_po_init(void)
*    use for init scpl lib with no py file dependent we use scpl.po replace LanguageLists.py 
*    and LanguageStrings.py to create a pure lib.so
* Return: 
*    fail:-1 succeed: 1
* Author: 
*    haoyuan.li@mediatek.com
* Date: 
*    2019/10/14
***************************************************************************/
int _vssp_po_init(void)
{
    FILE *fp_po=NULL;
    int i4_ret = -1;
    long i4_filesize = -1;
    const char* p_po_file_dir_path = "/3rd/scpl/locale/";
    const char* p_po_rw_file_dir_path = "/3rd_rw/scpl/locale/";
    const char* p_language_index_name[LANGUAGE_COUNT] = {"en","es","fr"};
    const char* p_po_file_base_path= "/LC_MESSAGES/scpl.po";
    char p_full_path[100]={0};
    char p_rw_full_path[100]={0};
    const char* p_msgid_flag  = "msgid"; 
    const char* p_msgstr_flag = "msgstr";
    int i4_language_count = 0;
    int i4_need_run = 1;

    while(i4_language_count < LANGUAGE_COUNT && i4_need_run)
    {
        //memset
        memset(p_full_path,0x0,sizeof(p_full_path));
        memset(p_rw_full_path,0x0,sizeof(p_rw_full_path));

        //assemble full file path
        sprintf(p_full_path,"%s%s%s"       ,p_po_file_dir_path, p_language_index_name[i4_language_count], p_po_file_base_path);
        sprintf(p_rw_full_path,"%s%s%s" ,p_po_rw_file_dir_path, p_language_index_name[i4_language_count], p_po_file_base_path);

        //check whether po file exist
        if(access(p_full_path,F_OK)==0)
        {
            if((fp_po=fopen(p_full_path,"r"))==NULL)
	    {
	        printf("[ReST Glue]open po file %s error\n",p_full_path);
               break;
	    }
        }
        else if(access(p_rw_full_path,F_OK)==0)
        {
            if((fp_po=fopen(p_rw_full_path,"r"))==NULL)
	    {
	        printf("[ReST Glue]open po file %s error\n",p_rw_full_path);
               break;
	    }        
        }
        else
        {
            printf("[ReST Glue]can not find scpl.po\n");
            break;
        }

        //reset buffer
        memset(p_arr_po_file_buffer, 0x0, sizeof(p_arr_po_file_buffer));
        memset(t_po_file.pt_po_msgid_and_msgstr[i4_language_count], 0x0, sizeof(PO_MSG_ST)*MAX_PO_FILE_MSGID_NUM);

        //read the total po file
        i4_filesize = fread(p_arr_po_file_buffer,1,sizeof(p_arr_po_file_buffer),fp_po);
        fclose(fp_po);

        if(i4_filesize!=-1 && i4_filesize!=0)
        {
            //printf("[ReST Glue]%s scpl.po file size is %ld\n", p_language_index_name[i4_language_count], i4_filesize);
        }
        else
        {
            printf("[ReST Glue]read scpl.po error \n");
            break;
        }

        char *p_file_start_addr=p_arr_po_file_buffer;

         //phrase all MSGID&MSGSTR in po file
        for(int i4_msgid_count=0;i4_msgid_count<MAX_PO_FILE_MSGID_NUM;i4_msgid_count++)
        {
             char *p_file_read_addr=NULL;
             char   p_max_cache_buff[100]={0};
             int i4_readstr_len = 0;
             int i4_readstr_line_len = 0;
             if(p_file_start_addr - p_arr_po_file_buffer >=i4_filesize || NULL==strstr(p_file_start_addr, p_msgid_flag))
             {
                  //printf("[ReST Glue]scanf po file ending ,total msgid num %d\n",i4_msgid_count);
                  t_po_file.i4_language_msgid[i4_language_count] = i4_msgid_count;
                  if((i4_language_count+1) == LANGUAGE_COUNT) //dolphin its range is 0-2 so need to +1
                  {
                      i4_ret = 1;
                      i4_need_run=0;
                      //printf("[ReST Glue]scanf three po file succeed\n");
                  }
                  break;
             }
             else
             {
                  /*
                  format:
                  #: LanguageStrings.py
                  msgid "BASS"
                  msgstr "bass"

                  #LanguageStrings.py
                  msgid ***
                  */
                  memset(p_max_cache_buff,0x0,sizeof(p_max_cache_buff));
                  p_file_read_addr=strstr(p_file_start_addr,p_msgid_flag);
                  sscanf(p_file_read_addr,"msgid \"%[^\"]\"",p_max_cache_buff);
                  i4_readstr_len = strlen(p_max_cache_buff);
                  if(strlen(p_max_cache_buff)>MAX_SCPL_PO_FILE_ONE_LINE_LEN)
		   {
		       printf("[ReST Glue]detect max msgid [%s] len %d is max than define %d\n",p_max_cache_buff,i4_readstr_len,MAX_SCPL_PO_FILE_ONE_LINE_LEN);
		       i4_need_run=0;
		       break;
		   }
                  //dolphin: one line have many symbol so add they all
                  t_po_file.pt_po_msgid_and_msgstr[i4_language_count][i4_msgid_count].p_msgid = (char*)malloc(i4_readstr_len + 1); //string + '\0' ,malloc will never release when process
                  memcpy(t_po_file.pt_po_msgid_and_msgstr[i4_language_count][i4_msgid_count].p_msgid, p_max_cache_buff, i4_readstr_len + 1);
                  i4_readstr_line_len = strlen(p_msgid_flag) + strlen(" ") + strlen("\"") + i4_readstr_len + strlen("\"") + strlen("\n"); //msgid "BASS"
                  p_file_start_addr = p_file_read_addr + i4_readstr_line_len;  //msgid "BASS"

                  memset(p_max_cache_buff,0x0,sizeof(p_max_cache_buff));
                  p_file_read_addr=strstr(p_file_start_addr, p_msgstr_flag);
                  sscanf(p_file_read_addr,"msgstr \"%[^\"]\"",p_max_cache_buff);
                  i4_readstr_len = strlen(p_max_cache_buff);
                  if(strlen(p_max_cache_buff)>MAX_SCPL_PO_FILE_ONE_LINE_LEN)
		   {
		       printf("[ReST Glue]detect max msgstr [%s] len %d is max than define %d\n",p_max_cache_buff,i4_readstr_len,MAX_SCPL_PO_FILE_ONE_LINE_LEN);
		       i4_need_run=0;
		       break;
		   }
                  t_po_file.pt_po_msgid_and_msgstr[i4_language_count][i4_msgid_count].p_msgstr = (char*)malloc(i4_readstr_len + 1); //string + '\0'   ,malloc will never release when process 
                  memcpy(t_po_file.pt_po_msgid_and_msgstr[i4_language_count][i4_msgid_count].p_msgstr, p_max_cache_buff, i4_readstr_len + 1);
                  i4_readstr_line_len = strlen(p_msgstr_flag) + strlen(" ") + strlen("\"") + i4_readstr_len + strlen("\"") + strlen("\n");
                  p_file_start_addr = p_file_read_addr + i4_readstr_line_len;  //msgstr "bass"
             }
        }
        i4_language_count++;
        if(i4_language_count==LANGUAGE_COUNT && i4_ret!=1)  //scanf all but faild ,maybe po file had changed 
        {
            printf("scanf three po file end,but not succeed ,please check!\n");
        }
    }
    return i4_ret;
}

static std::vector<std::string> splitString(const std::string& inputStr, char splitChar) 
{
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

// VsspExt.Set
// Sets the value of the resource referenced by parameter 'uri' to the parameter 'newValue'
static PyObject * VsspExt_Set(PyObject *self, PyObject *args)
{
    const char *uri;
    PyObject* newValueObj;
    int errCode = API_ERR_INVALID_PARAMETER;
    int uri_len, key_list_len;

    // Get the URI, URI Len, and the Python Dictionary that represents the new value
    if (!PyArg_ParseTuple(args, "sO!", &uri, &PyDict_Type, &newValueObj))
    {
        //printf("[ReST Glue] %s,%d possible API_ERR_INVALID_PARAMETER\n", __FUNCTION__, __LINE__);
        if (!PyArg_ParseTuple(args, "siOi", &uri, &uri_len, &newValueObj, &key_list_len)) {
            printf("[ReST Glue] %s,%d API_ERR_INVALID_PARAMETER\n", __FUNCTION__, __LINE__);
            return Py_BuildValue("i", API_ERR_INVALID_PARAMETER);
        }
    }
    printf("[ReST Glue] %s,%s,%d, URI = %s\n", __FILE__, __FUNCTION__, __LINE__, uri);
    //printf("[ReST Glue] %s,%s,%d, is Dict = %d\n", __FILE__, __FUNCTION__, __LINE__, PyDict_Check(newValueObj));
    //printf("[ReST Glue] %s,%s,%d, is List = %d\n", __FILE__, __FUNCTION__, __LINE__, PyList_Check(newValueObj));
    if (!newValueObj || (!PyDict_Check(newValueObj) && !PyList_Check(newValueObj)))
    {
        if (strcmp(uri, "app/key/delegate") != 0 && strcmp(uri, "key_command") != 0) {
            printf("[ReST Glue] %s,%d API_ERR_INVALID_PARAMETER\n", __FUNCTION__, __LINE__);
            return Py_BuildValue("i", API_ERR_INVALID_PARAMETER);
        } else {
            printf("[ReST Glue] %s,%d whitelist %s\n", __FUNCTION__, __LINE__, uri);
        }
    }

    std::vector<std::string> uris = splitString(uri, '/');

    //printf("[ReST Glue] %s,%s,%d,uris[0] = %s\n", __FILE__, __FUNCTION__, __LINE__, uris[0].c_str());

    if (uris.size() > 2
        && uris[0] != std::string("app")) {
        PyObject* PyObject_value = PyDict_GetItemString(newValueObj, "VALUE");

        if (!PyObject_value) {
            printf("[ReST Glue] AUDIO_System_Set: no VALUE property\n");
            return Py_BuildValue("i", API_ERR_MISSING_PARAMETER);
        }
        if (uris[0] == std::string("tv_settings")) {
            if(uris[1] == std::string("system")){
                if(uris[2] == std::string("menu_language")){
                    printf( "[ReST Glue] VsspExt_Set: menu_language\n");
                    Py_ssize_t size = PyUnicode_GET_SIZE(PyObject_value);

                    printf("[ReST Glue] VsspExt_Set: size of string = %zd\n",size);

                    Py_UNICODE* valueStr = PyUnicode_AS_UNICODE(PyObject_value);

                    printf("[ReST Glue] VsspExt_Set: as unicode \n");

                    if (!valueStr) {
                        printf("[ReST Glue] VsspExt_Set: VALUE not a unicode string\n");
                        return Py_BuildValue("i", API_ERR_MISSING_PARAMETER);
                    }


                    memset(curLang, 0, STORAGE_LENGTH*sizeof(Py_UNICODE));
                    memcpy(curLang, valueStr, size*sizeof(Py_UNICODE));


                    PyObject * utf8Obj = PyUnicode_AsUTF8String(PyObject_value);
                    const char * data = (PyString_AsString(utf8Obj));


                    int index = get_language_index(data);

                    PyDict_SetItemString(newValueObj, "VALUE", Py_BuildValue("i", index));

                    _current_country_index = index;


                }else if (uris[2] == std::string("country")) {
                    printf("[ReST Glue] VsspExt_Set: country\n");
                    Py_ssize_t size = PyUnicode_GET_SIZE(PyObject_value);

                    fprintf(stderr, "VsspExt_Set: size of string = %zd\n",size);

                    Py_UNICODE* valueStr = PyUnicode_AS_UNICODE(PyObject_value);

                    if (!valueStr) {
                        fprintf(stderr, "VsspExt_Set: VALUE not a unicode string\n");
                        return Py_BuildValue("i", API_ERR_MISSING_PARAMETER);
                    }

                    //memset(curCountry, 0, STORAGE_LENGTH*sizeof(Py_UNICODE));
                    //memcpy(curCountry, valueStr, size*sizeof(Py_UNICODE));

                    PyObject * utf8Obj = PyUnicode_AsUTF8String(PyObject_value);
                    const char * data = (PyString_AsString(utf8Obj));

                    int index = get_country_index(data);

                    PyDict_SetItemString(newValueObj, "VALUE", Py_BuildValue("i", index));
                }
            }
        }
    }
    
    printf("[ReST Glue] VsspExt_Set: start to json \n");

    {

#if 0
        PyObject* pyObject_value = PyDict_GetItemString(newValueObj, "VALUE");
        if (pyObject_value) {  //for key_command do not transtion because it has no VALUE element
            PyObject* newPyObject = transtion(uri,get_current(), 0, pyObject_value);
            if (newPyObject != Py_None) {
                PyDict_SetItemString(newValueObj, "VALUE", newPyObject);
            }
        }
#else
#endif
    }

    Json::Value root;
    Json::Value value = pythonToJson(newValueObj);
    if(i4_is_scpl_init_succeed==1) //had been inited
    {
        value = _vssp_translate(value,uri,_current_country_index,0); //dolphin translate other language to English
        printf("[ReST Glue] set uri should translation \n");
    }

    if (uris[0] == std::string("key_command"))
    {
        printf("[ReST Glue] %s,%s,%d,key_command v1\n", __FILE__, __FUNCTION__, __LINE__);

        Json::Value raw;
        int codeset;
        int code;
        char p_codeset[16]={0};
        char p_code[16]={0};	
        char s_key_list[1024] = {0};
        char* ps_key_list = NULL;
        char* pc_cur = NULL;
        char* pc_cur_end = NULL;
        const char *p_CODESET = "CODESET";
        const char *p_CODE = "CODE";

        //Json::Reader reader;
        //Json::Value json_data; //no use json for uncertain structure

        printf("value: %s\n", value.toStyledString().c_str());

        memset(s_key_list, '\0', 1024);
        strncpy(s_key_list, value.toStyledString().c_str(), 1024);

        ps_key_list = s_key_list;

        while (ps_key_list != NULL && strlen(ps_key_list) > 0)
        {
            code = -1;
            codeset = -1;

            if ((pc_cur = strstr(ps_key_list, "CODESET")) != NULL) {
                sscanf(pc_cur,"%*[^0-9]%[0-9]",p_codeset);
                sscanf(p_codeset,"%d",&codeset);
                printf("%s >%s< codeset = %d\n",__FUNCTION__, pc_cur,codeset);
                pc_cur[1]='M'; //modify the codeset for next search
            } else {
                break;
            }
            if ((pc_cur = strstr(ps_key_list, "CODE")) != NULL) {
                sscanf(pc_cur,"%*[^0-9]%[0-9]",p_code);
                sscanf(p_code,"%d",&code);
                printf("%s >%s< code = %d\n",__FUNCTION__, pc_cur,code);
                pc_cur[1]='M'; //modify the codeset for next search
                } else {
                    break;
                }

            printf("code = %d & codeset = %d\n", code, codeset);

            if (codeset == PWR
                && (code == 0 || code == 1)) { // 0-OFF  1-ON
                Json::Value power_value;
                value = code;
                errCode = josnRpcCppSend(REST_REQUEST_SET, "system/native_power_function", value);
                if (errCode != EXIT_SUCCESS) {
                    return Py_BuildValue("i", errCode);
                }
                printf("[ReST Glue] VsspExt_Set send end for power key\n");

                root = josnRpcCppRecv();
                errCode = root["SUCCESS"].asInt();

                return Py_BuildValue("i", errCode);
            } else if (code != -1 && codeset != -1) {
                errCode = key_set(codeset, code);
            }
            ps_key_list = pc_cur;
        }

        return Py_BuildValue("i", errCode);
    }
    errCode = josnRpcCppSend(REST_REQUEST_SET, uri, value);

    if (errCode != EXIT_SUCCESS)
    {
        return Py_BuildValue("i", errCode);
    }

    printf("[ReST Glue] VsspExt_Set send end\n");

    root = josnRpcCppRecv();
    errCode = root["SUCCESS"].asInt();

    return Py_BuildValue("i", errCode);
}

// VsspExt.Action
// Resets the value of the resource referenced by parameter 'uri'
static PyObject * VsspExt_Action(PyObject *self, PyObject *args)
{
    const char *uri;
    PyObject* newValueObj;
    int errCode = API_ERR_INVALID_PARAMETER;

    if (!PyArg_ParseTuple(args, "sO!", &uri, &PyDict_Type, &newValueObj))
    {
        printf("[ReST Glue] %s,%d\n", __FUNCTION__, __LINE__);
        return Py_BuildValue("i", API_ERR_INVALID_PARAMETER);
    }

    if (!newValueObj || !PyDict_Check(newValueObj))
    {
        printf("[ReST Glue] %s,%d\n", __FUNCTION__, __LINE__);
        return Py_BuildValue("i", API_ERR_INVALID_PARAMETER);
    }  

    Json::Value value = pythonToJson(newValueObj);

    errCode = josnRpcCppSend(REST_REQUEST_ACTION, uri, value);

    printf("[ReST Glue] VsspExt_Action send ok\n");

    if (errCode != EXIT_SUCCESS)
    {
        return Py_BuildValue("i", errCode);
    }


    Json::Value root = josnRpcCppRecv();
    errCode = root["SUCCESS"].asInt();

    // Return the error code
    return Py_BuildValue("i", errCode);
}

// VsspExt.Get
// Gets the value of the resource referenced by parameter 'uri'
static PyObject * VsspExt_Get(PyObject *self, PyObject *args)
{
    const char *uri;
    int errCode = API_ERR_INVALID_PARAMETER;

    if (!PyArg_ParseTuple(args, "s", &uri))
    {
        printf("[ReST Glue] %s, %d\n", __FUNCTION__, __LINE__);
        return Py_BuildValue("i", API_ERR_INVALID_PARAMETER);
    }

    printf("[ReST Glue] VsspExt_Get %s.\n", uri);

    std::vector<std::string> uris = splitString(uri, '/');
    // Invoke the proper handler
    if (uris.size() > 2)
    {
        PyObject* result = NULL;
        if (uris[0] == std::string("tv_settings"))
        {
            if (uris[1] == std::string("system"))
            {
                if (uris[2] == std::string("menu_language"))
                {
                    printf("[ReST Glue] VsspExt_Get menu_language\n");

                    // TODO: shoule get language index fisrt

                    Json::Value none;

                    errCode = josnRpcCppSend(REST_REQUEST_GET, uri, none);

                    printf("[ReST Glue] VsspExt_Get ok\n");

                    if (errCode != EXIT_SUCCESS) {
                        return Py_BuildValue("i", errCode);
                    }

                    Json::Value root = josnRpcCppRecv();

                    PyObject* result = jsonToPython(root);
                    if (NULL == result) {
                        printf("[ReST Glue] VsspExt_Get:jsonToPython fail!\n");
                    }

                    PyObject* PyObject_value = PyDict_GetItemString(result, "VALUE");

                    int index = PyInt_AsLong(PyObject_value);
                    if (index > 2) {
                        printf("[ReST Glue] wrong index \n");
                        index = 0;
                    }
                    _current_country_index = index;

                    result = PyDict_New();
                    // if (wcslen(curLang) == 0) 

                    const char* Languages[] = {"English", "Español","Français"};
                    PyObject * pyObjLang = PyUnicode_FromString(Languages[index]);
                    /*
                    Py_ssize_t size = PyUnicode_GET_SIZE(pyObjLang);
                    printf("VsspExt_Get: size of string = %zd\n",size);
                    Py_UNICODE* valueStr = PyUnicode_AS_UNICODE(pyObjLang);
                    if (!valueStr) {
                       printf("VsspExt_Get: VALUE not a unicode string\n");
                       return result;
                    }
                    memset(curLang, 0, STORAGE_LENGTH*sizeof(Py_UNICODE));
                    memcpy(curLang, valueStr, size*sizeof(Py_UNICODE));
                    */

                    printf("[ReST Glue] VsspExt_Get menu_language cur not null\n");

                    PyObject * value = Py_BuildValue("u", PyUnicode_AS_UNICODE(pyObjLang));

                    PyDict_SetItemString(result,"VALUE", value);
                    PyDict_SetItemString(result,"SUCCESS", Py_BuildValue("i", 0));

                    printf("[ReST Glue] VsspExt_Get menu_language success\n");

                    return result;  
                }
                else if (uris[2] == std::string("country"))
                {
                    printf("[ReST Glue] VsspExt_Get country\n");

                    // TODO: shoule get language index fisrt

                    Json::Value none;

                    errCode = josnRpcCppSend(REST_REQUEST_GET, uri, none);

                    printf("[ReST Glue] VsspExt_Get ok\n");

                    if (errCode != EXIT_SUCCESS) {
                        return Py_BuildValue("i", errCode);
                    }

                    Json::Value root = josnRpcCppRecv();

                    PyObject* result = jsonToPython(root);
                    if (NULL == result) {
                        printf("[ReST Glue] VsspExt_Get:jsonToPython fail!\n");
                    }

                    PyObject* PyObject_value = PyDict_GetItemString(result, "VALUE");

                    int index = PyInt_AsLong(PyObject_value);
                    printf("[ReST Glue] VsspExt_Get country langIdx=%d countryIdx=%d\n",_current_country_index,index);
                    if (index > 2) {
                        printf("[ReST Glue] wrong country index \n");
                        index = 0;
                    }

                    PyDict_SetItemString(result,"VALUE", PyUnicode_FromString(countries[_current_country_index][index]));
                    PyDict_SetItemString(result,"SUCCESS", Py_BuildValue("i", 0));

                    printf("[ReST Glue] VsspExt_Get country success\n");

                    return result;  
                }
                else {
                    printf("[ReST Glue] VsspExt_Get not language & country\n");
                }
            }
        }  
    }

    Json::Value value;
    if (uris[0] == std::string("airplay") && strstr(std::string(uris[1]).c_str(), "property") != NULL) {
        char* p = NULL;
        char temp_str[128];
        strncpy(temp_str, std::string(uris[1]).c_str(), 127);
        for (char* q=temp_str; ;q++) {
            if (*q == 0 || strncmp(q, "?", 1) == 0) {
                p = q;
                break;
            }
        }
        if (p != NULL && *p != 0) {
            p++;
            printf("[ReST Glue] VsspExt_Get airplay/property %s.\n\r", p);
        }
        errCode = josnRpcCppSend_ex(REST_REQUEST_GET, std::string("airplay/property"), value, std::string(p));
    } else {
        errCode = josnRpcCppSend(REST_REQUEST_GET, uri, value);
    }

    printf("[ReST Glue] VsspExt_Get ok\n");

    if (errCode != EXIT_SUCCESS) {
        return Py_BuildValue("i", errCode);
    }

    Json::Value root = josnRpcCppRecv();
    printf("[ReST Glue]get methid i4_is_scpl_init_succeed %d\n",i4_is_scpl_init_succeed);
    if(i4_is_scpl_init_succeed==1) //if scpl phrase struct had init succeed then enter translate
    {
        Json::Value j_copyroot=root;
        root = _vssp_translate(j_copyroot,uri,0,_current_country_index); //dolphin get English and translate to dest language
    }

    PyObject* result = jsonToPython(root);
    if (NULL == result) {
        printf("[ReST Glue] VsspExt_Get:jsonToPython fail!\n");
    }

#if 0
    PyObject* pyObject_value = PyDict_GetItemString(result, "VALUE");
    if (pyObject_value != NULL)
    {
        PyObject* newPyObject = transtion(uri,0, get_current(), pyObject_value);
        if (newPyObject != Py_None) {
            PyDict_SetItemString(result, "VALUE", newPyObject);
        }
    }

    ;

#endif
    return result;
}

// VsspExt.AcrGet
// Gets the value of the resource referenced by parameter 'uri'
static PyObject * VsspExt_AcrGet(PyObject *self, PyObject *args)
{
    int i;
    const char *uri;

    if (!PyArg_ParseTuple(args, "s", &uri))
        return Py_BuildValue("i", API_ERR_INVALID_PARAMETER);

    PyObject* result = NULL;

    //TODO


    return result;
}

// VsspExt.Reset
// Resets the value of the resource referenced by parameter 'uri'
static PyObject * VsspExt_Reset(PyObject *self, PyObject *args)
{
    const char *uri;
    int errCode = API_ERR_INVALID_PARAMETER;

    if (!PyArg_ParseTuple(args, "s", &uri))
        return Py_BuildValue("i", API_ERR_INVALID_PARAMETER);


    printf("[ReST Glue] VsspExt_Reset \n");

    Json::Value value;

    errCode = josnRpcCppSend(REST_REQUEST_RESET, uri, value);

    if (errCode != EXIT_SUCCESS)
    {
        return Py_BuildValue("i", errCode);
    }


    printf("[ReST Glue] VsspExt_Reset ok\n");

    Json::Value root = josnRpcCppRecv();
    errCode = root["SUCCESS"].asInt();


    // Return the error code
    return Py_BuildValue("i", errCode);
}

// VsspExt.Pairing_Start
// Indicate pairing started and return current challenge type
/*static PyObject *
VsspExt_Pairing_Start(PyObject *self, PyObject *args)
{
    return VsspExt_Get(self,args);
}*/

static PyObject * VsspExt_Pairing_Start(PyObject *self, PyObject *args)
{
    fprintf(stderr,"VsspExt.Pairing_Start called\n");

    const char *pin;

    // Get the URI and the Python Dictionary that represents the new value
    if (!PyArg_ParseTuple(args, "s", &pin)){
        return Py_BuildValue("i", API_ERR_INVALID_PARAMETER);
    }

    PyObject* message = PyDict_New();
    PyDict_SetItemString(message,"VALUE", Py_BuildValue("s", pin));
    
    
    Json::Value value = pythonToJson(message);
    int errCode = josnRpcCppSend(REST_REQUEST_SET, "pairing/start", value);

    if (errCode != EXIT_SUCCESS)
    {
        return Py_BuildValue("i", errCode);
    }
    
    Json::Value root = josnRpcCppRecv();
    errCode = root["SUCCESS"].asInt();

    PyObject* result = PyDict_New();
    if (!result){
        return NULL;
    }

    int err = PyDict_SetItemString(result,"SUCCESS", Py_BuildValue("i", errCode));
    if (-1 == err){
        fprintf(stderr, "apiResultDict fail!\n");
        return NULL;
    }
    
    // Indicate that pairing has started and return the challenge type required for this platform
    if (!result)
        return NULL;
    
    
    err = PyDict_SetItemString(result,"VALUE", Py_BuildValue("i", FOUR_DIGIT_BASE10_PIN));
    if (err == -1)
        return NULL;

    return result;

}

// VsspExt.Pairing_End
// Indicate pairing has ended
static PyObject * VsspExt_Pairing_End(PyObject *self, PyObject *args)
{
     Json::Value value;
     int errCode = josnRpcCppSend(REST_REQUEST_SET, "pairing/stop", value);

     if (errCode != EXIT_SUCCESS)
     {
         return Py_BuildValue("i", errCode);
     }
    
     Json::Value root = josnRpcCppRecv();
     errCode = root["SUCCESS"].asInt();
     return Py_BuildValue("i", errCode);
}

// VsspExt.Pairing_Has_Executed_Challenge
// Return indication of whether the platform-secific pairing challenge was executed
static PyObject * VsspExt_Pairing_Has_Executed_Challenge(PyObject *self, PyObject *args)
{
    return VsspExt_Get(self,args);
}

static PyObject* g_callback_event = NULL;
static PyObject* g_callback_watchdog = NULL;

/* ASSIGN THE PYTHON OBJECT (CALLBACK FUNCTION) */
PyObject* VsspExt_SetCB(PyObject* dummy, PyObject* args)
{
    PyObject* result = NULL;
    PyObject* obj_temp;
    int type;

    printf("[ReST Glue] dslRestSetCB, args = %p.\n", args);
    if (PyArg_ParseTuple(args, "Oi:RestSetCB", &obj_temp, &type))
    {
        if (!PyCallable_Check(obj_temp))
        {
            PyErr_SetString(PyExc_TypeError, "parameter 0 must be callable");
            return NULL;
        }

        // CALLBACK = Utils.enum('EVENT', 'WATCHDOG')
        printf("[ReST Glue] dslRestSetCB argument, type:%d, callback:%p.\n", type, obj_temp);

        switch (type)
        {
            case 0:
                printf("[ReST Glue] dslRestSetCB argument, type:EVENT (old, new) = (%p, %p).\n", g_callback_event, obj_temp);
                Py_XINCREF(obj_temp);       /* Add a reference to new callback to avoid grabage collection*/
                Py_XDECREF(g_callback_event); /* Dispose of previous callback. acceptable if it's null*/
                g_callback_event = obj_temp;      /* Remember new callback */
                break;
            case 1:
                printf("[ReST Glue] dslRestSetCB argument, type:WATCHDOG (old, new) = (%p, %p).\n", g_callback_watchdog, obj_temp);
                Py_XINCREF(obj_temp);       /* Add a reference to new callback to avoid grabage collection*/
                Py_XDECREF(g_callback_watchdog); /* Dispose of previous callback. acceptable if it's null*/
                g_callback_watchdog = obj_temp;      /* Remember new callback */
                break;
            default:
                PyErr_SetString(PyExc_TypeError, "No such type");
                return NULL;
        }

        /* Boilerplate to return "None" */
        Py_INCREF(Py_None);
        result = Py_None;
    }

    return result;
}

PyObject * VsspExt_Notify(PyObject *self, PyObject *args)
{
    printf("[ReST Glue] dslRestNotify\n");

    if (PyCallable_Check(g_callback_event))
    {
        printf("[ReST Glue] dslRestNotify, g_callback_event = %p.\n", g_callback_event);

        PyGILState_STATE gstate;
        gstate = PyGILState_Ensure();

        // HARD CODE THE REASON FOR TESTING
        int reason = 4;

        switch (reason)
        {
            case 1:
            case 2:
                {
                    /********************************************************/
                    /* TEST USING TUPLE RETURN                              */
                    /********************************************************/

                    printf("[ReST Glue] dslRestNotify x1\n");

                    // PyObject* py_args_tuple;

                    // URI AND VALUE GO INTO TUPLE
                    //py_args_tuple = PyTuple_New(2);

                    //PyTuple_SetItem(py_args_tuple, 0, PyString_FromString("state/device/power_mode")); //URI
                    //PyTuple_SetItem(py_args_tuple, 1, PyInt_FromLong(0));      //VALUE

                    PyObject_CallFunction(g_callback_event, "{s:s, s:i}", "URI", "state/device/power_mode", "VALUE", 0);
                    //PyObject_CallObject(g_callback, py_args_tuple);

                    //Py_DECREF(py_args_tuple);

                    printf("[ReST Glue] dslRestNotify x2\n");
                }
                break;

            case 3: // KEY COMMAND
                {
                    /********************************************************/
                    /* TEST USING PY_BUILDVALUE FORMATTING                  */
                    /********************************************************/
                    PyObject_CallFunction(g_callback_event, "{s:s, s:{s:[{s:i, s:i, s:s}]}}", "URI", "key_command", "VALUE", "KEYLIST", "CODESET", 0,
                                            "CODE", 65, "ACTION", "KEYPRESS");
                }
                break;
            case 4: // CURRENT INPUT
                {
                    PyObject_CallFunction(g_callback_event, "{s:s, s:s}", "URI", "tv_settings/devices/current_input", "VALUE", "HDMI-1");
                }
        }

        PyGILState_Release(gstate);
    }
    else
    {
        printf("[ReST Glue] dslRestNotify, INVALID g_callback_event\n");
    }

    // ACTUAL DSL LIBRARY DOES NOT RETURN ANYTHING AND IS ONLY CALLED FROM NATIVE, NOT PYTHON
    // LIKE WE ARE DOING HERE.. ONLY FOR TESTING
    return Py_None;
}

// Module's method table and initialization function
// See: http://docs.python.org/extending/extending.html#the-module-s-method-table-and-initialization-function
static PyMethodDef VsspExtMethods[] = {
    {"Set", VsspExt_Set, METH_VARARGS, "Perform Set Operation."},
    {"Get", VsspExt_Get, METH_VARARGS, "Perform Get Operation."},
    {"AcrGet", VsspExt_AcrGet, METH_VARARGS, "Perform AcrGet Operation."},
    {"Pairing_Start", VsspExt_Pairing_Start, METH_VARARGS, "Perform Pairing Start Operation."},
    {"Pairing_End", VsspExt_Pairing_End, METH_VARARGS, "Perform Pairing End Operation."},
    {"Pairing_Has_Executed_Challenge", VsspExt_Pairing_Has_Executed_Challenge, METH_VARARGS, "Perform Pairing Has Executed Operation."},
    {"Reset", VsspExt_Reset, METH_VARARGS, "Perform Reset Operation."},
    {"Action", VsspExt_Action, METH_VARARGS, "Perform Action Operation."},
    {"Notify", VsspExt_Notify, METH_VARARGS, "Rest Notify"},
    {"SetCB", VsspExt_SetCB, METH_VARARGS, "Rest Callback"},
    {NULL, NULL, 0, NULL}
};

static bool b_thread_running = true;

static void* VsspExt_Event_Ipc_Therad(void* arg)
{
    (void)arg;
    REST_MSG_BUF msg = {0};
    VSSP_IPC_RET_T ret = VSSP_IPC_R_FAIL;

    printf("[ReST Glue] %s,%d, enter\n", __FUNCTION__, __LINE__);

    while (b_thread_running) {
        memset(&msg, 0, sizeof(REST_MSG_BUF));
        ret = Vssp_get_msg_from_rest_event(&msg);
        if (VSSP_IPC_R_OK != ret) {
            printf("[ReST Glue] %s,%d,Vssp_get_msg_from_rest_event error,ret=%d\n", __FUNCTION__, __LINE__, ret);
        } else {
            printf("[ReST Glue] %s,%d,msg data:%s\n" , __FUNCTION__, __LINE__, msg.data_str);
            Json::Value root;
            Json::Reader reader;
            std::string responseStr;

            responseStr.append(msg.data_str);
            if (reader.parse(responseStr, root))
            {
                std::string uri = root["uri"].asString();
                Json::Value result = root["result"];

                std::cout << "[ReST Glue] VsspExt_Event_Ipc_Therad: uri: " << uri << std::endl;

                Json::FastWriter writer;
                std::cout << "[ReST Glue] VsspExt_Event_Ipc_Therad: result: " << writer.write(result) << std::endl;

                // notify rest server via Vizio's API
                if (PyCallable_Check(g_callback_event))
                {
                    printf("[ReST Glue] dslRestNotify, g_callback_event = %p.\n", g_callback_event);

                    PyGILState_STATE gstate;
                    gstate = PyGILState_Ensure();

                    if (uri == "oobe/currentstate") {
                        if (result["TYPE"].compare("PUT") == 0) {
                            printf("[ReST Glue][10FAPPCHECKPOINT] VsspExt_Event_Ipc_Therad: oobe/currentstate- PUT [WORKAROUNT] send a key_command\n");
                            //PyObject_CallFunction(g_callback, "{s:s, s:{s:s}, s:s}", "URI", uri.c_str(), "VALUE", "CS", result["CS"].asString().c_str(), "TYPE", "PUT");
                            PyObject_CallFunction(g_callback_event, "{s:s, s:{s:[{s:i, s:i, s:s}]}}", "URI", "key_command", "VALUE", "KEYLIST", "CODESET", 7,
                                                "CODE", 4, "ACTION", "KEYPRESS");
                        } else {
                            printf("[ReST Glue][10FAPPCHECKPOINT] VsspExt_Event_Ipc_Therad: oobe/currentstate- EVENT\n");
                            PyObject_CallFunction(g_callback_event, "{s:s, s:{s:s}}", "URI", uri.c_str(), "VALUE", "CS", result["CS"].asString().c_str());
                        }
                    } else if (uri == "key_command") {
                        PyObject_CallFunction(g_callback_event, "{s:s, s:{s:[{s:i, s:i, s:s}]}}", "URI", uri.c_str(), "VALUE", "KEYLIST", "CODESET", result["KEYLIST"][0]["CODESET"].asInt(),
                                                "CODE", result["KEYLIST"][0]["CODE"].asInt(), "ACTION", result["KEYLIST"][0]["ACTION"].asString().c_str());
                        printf("[ReST Glue][10FAPPCHECKPOINT] VsspExt_Event_Ipc_Therad: key_command\n");
                    } else if (uri == "ir_command")  {
                        if (result["HAS_KEY"].asInt() == 0)
                        {
                            PyObject_CallFunction(g_callback_event, "{s:s, s:{s:s, s:s}}", "URI", uri.c_str(), "VALUE", "IR", result["IR"].asString().c_str(), "KEY", NULL);
                        }
                        else
                        {
                            PyObject_CallFunction(g_callback_event, "{s:s, s:{s:s, s:{s:i, s:i, s:i}}}", "URI", uri.c_str(), "VALUE", "IR", result["IR"].asString().c_str(), "KEY", 
                                                    "ACTION", result["KEY"]["ACTION"].asInt(), "CODESET", result["KEY"]["CODESET"].asInt(), "CODE", result["KEY"]["CODE"].asInt());
                        }
                        printf("[ReST Glue][10FAPPCHECKPOINT] VsspExt_Event_Ipc_Therad: ir_command\n");
                    } else if (uri == "system/input/list"
                               || uri == "system/network_time"
                               || uri == "app/zoom/enabled"
                               || uri == "app/tts/enabled"
                               || uri == "tv_settings/system/menu_language"
                               || uri == "tv_settings/channels/percent_complete"
                               || uri == "tv_settings/channels/analog"
                               || uri == "tv_settings/channels/digital"
                               || uri == "tv_settings/channels/channel_scan_state"
                               || uri == "tv_settings/cast/state"
                               || uri == "tv_settings/cast/tos_accepted"
                               || uri == "tv_settings/closed_captions/closed_captions_enabled"
                               || uri == "tv_settings/network/connection_status"
                               || uri == "tv_settings/audio/volume_selector"
                               || uri == "tv_settings/audio/volume"
                               || uri == "tv_settings/picture/brightness"
                               || uri == "state/device/power_mode"
                               || uri == "system/picture/picture_mode") {
                        PyObject_CallFunction(g_callback_event, "{s:s, s:i}", "URI", uri.c_str(), "VALUE", result["VALUE"].asInt());
                    } else if (uri == "app/current") {
                        if (result["TYPE"].compare("PUT") == 0) {
                            printf("[ReST Glue][10FAPPCHECKPOINT] VsspExt_Event_Ipc_Therad: app/current- PUT\n");
                            if (result["MESSAGE"].asString().length() == 0) {
                                PyObject_CallFunction(g_callback_event, "{s:s, s:{s:i, s:s, s:s}, s:s}", "URI", uri.c_str(), "VALUE", "NAME_SPACE", result["NAME_SPACE"].asInt(), "APP_ID", result["APP_ID"].asString().c_str(), "MESSAGE", NULL, "TYPE", "PUT");
                            } else {
                                PyObject_CallFunction(g_callback_event, "{s:s, s:{s:i, s:s, s:s}, s:s}", "URI", uri.c_str(), "VALUE", "NAME_SPACE", result["NAME_SPACE"].asInt(), "APP_ID", result["APP_ID"].asString().c_str(), "MESSAGE", result["MESSAGE"].asString().c_str(), "TYPE", "PUT");
                            }
                        } else {
                            printf("[ReST Glue] VsspExt_Event_Ipc_Therad: app/current- EVENT\n");
                            if (result["APP_ID"].asString().length() == 0) {
                                PyObject_CallFunction(g_callback_event, "{s:s, s:s}", "URI", uri.c_str(), "VALUE", NULL);
                            } else {
                                if  (result["MESSAGE"].asString().length() == 0) {
                                    PyObject_CallFunction(g_callback_event, "{s:s, s:{s:i, s:s, s:s}}", "URI", uri.c_str(), "VALUE", "NAME_SPACE", result["NAME_SPACE"].asInt(), "APP_ID", result["APP_ID"].asString().c_str(), "MESSAGE", NULL);
                                } else {
                                    PyObject_CallFunction(g_callback_event, "{s:s, s:{s:i, s:s, s:s}}", "URI", uri.c_str(), "VALUE", "NAME_SPACE", result["NAME_SPACE"].asInt(), "APP_ID", result["APP_ID"].asString().c_str(), "MESSAGE", result["MESSAGE"].asString().c_str());
                                }
                            }
                        }
                    } else if (uri == "tv_settings/cast/device_name"
                               || uri == "system/input/name"
                               || uri == "system/input/visible"
                               || uri == "system/input/enable"
                               || uri == "airplay/property"
                               || uri == "tv_settings/system/power_mode"
                               || uri == "tv_settings/picture/picture_mode"
                               || uri == "tv_settings/audio/tv_speakers"
                               || uri == "tv_settings/audio/mute") {
                        PyObject_CallFunction(g_callback_event, "{s:s, s:s}", "URI", uri.c_str(), "VALUE", result["VALUE"].asString().c_str());
                    } else if (uri == "app/zoom/enabled"
                               || uri == "state/device/power_mode"
                               || uri == "tv_settings/cast/state"
                               || uri == "app/tts/enabled"
                               || uri == "tv_settings/closed_captions/closed_captions_enabled"
                               || uri == "app/zoom/enabled"
                               || uri == "tv_settings/cast/tos_accepted"
                               || uri == "tv_settings/network/connection_status"
                               || uri == "tv_settings/channels/channel_scan_state") {
                        PyObject_CallFunction(g_callback_event, "{s:s, s:i}", "URI", uri.c_str(), "VALUE", result["VALUE"].asInt());
                    } else if (uri == "uli/update_status") {
                        PyObject_CallFunction(g_callback_event, "{s:s, s:{s:i, s:i}}", "URI", uri.c_str(), "VALUE", "STATUS", result["STATUS"].asInt(), "PERCENT_COMPLETE", result["PERCENT_COMPLETE"].asInt());
                    } else if (uri == "uli/available_update_info") {
                        PyObject_CallFunction(g_callback_event, "{s:s, s:{s:i, s:s, s:i}}", "URI", uri.c_str(), "VALUE", "UPDATE_LEVEL", result["UPDATE_LEVEL"].asInt(), "VERSION", result["VERSION"].asString().c_str(), "IS_AVAILABLE", result["IS_AVAILABLE"].asInt());
                    } else if (uri == "system/input/current_input") {
                        std::cout << "[ReST Glue] VsspExt_Event_Ipc_Therad: current_input" << std::endl;
                        //std::cout << "[ReST Glue] VsspExt_Event_Ipc_Therad: isArray: " << result["VALUE"].isArray() << std::endl;
                        //std::cout << "[ReST Glue] VsspExt_Event_Ipc_Therad: isObject: " << result["VALUE"].isObject() << std::endl;
                        PyObject* pyobj = jsonToPython(result["VALUE"]);
                        PyObject_CallFunction(g_callback_event, "{s:s, s:O}", "URI", uri.c_str(), "VALUE", pyobj);
                    } else if (uri == "airplay/property") {
                        if (result["TYPE"].asInt() == 1) {
                            PyObject_CallFunction(g_callback_event, "{s:s, s:{s:i, s:s, s:i}}", "URI", uri.c_str(), "VALUE", "PROPERTY", result["PROPERTY"].asString().c_str(), "TYPE", result["TYPE"].asInt(), "VALUE", result["VALUE"].asInt());
                        } else if (result["TYPE"].asInt() == 0) {
                            PyObject_CallFunction(g_callback_event, "{s:s, s:{s:i, s:s, s:s}}", "URI", uri.c_str(), "VALUE", "PROPERTY", result["PROPERTY"].asString().c_str(), "TYPE", result["TYPE"].asInt(), "VALUE", result["VALUE"].asString().c_str());
                        }
                    } else if (uri == "app/metadata") {
                        std::cout << "[ReST Glue] VsspExt_Event_Ipc_Therad: app/metadata" << std::endl;
                        PyObject* pyobj = jsonToPython(result["METADATA"]);
                        PyObject_CallFunction(g_callback_event, "{s:s, s:s, s:i, s:s, s:O}", "URI", uri.c_str(), "REGISTERED", result["REGISTERED"].asString().c_str(), "NAME_SPACE", result["NAME_SPACE"].asInt(), "APP_ID", result["APP_ID"].asString().c_str(), "METADATA", pyobj);
                    }else if (uri == "tv_settings/closed_captions/digital_style") {
                        std::cout << "[ReST Glue] VsspExt_Event_Ipc_Therad: tv_settings/closed_captions/digital_style" << std::endl;
                        PyObject* pyobj = jsonToPython(result);
                        PyObject_CallFunction(g_callback_event, "{s:s, s:O}", "URI", uri.c_str(), "VALUE", pyobj);
                    } else if (uri == "tv_settings/system/reset_and_admin/tvad_id_reset") {
                        PyObject_CallFunction(g_callback_event, "{s:s, s:s}", "URI", uri.c_str(), "TYPE", result["TYPE"].asString().c_str());
                    } else if (uri == "audio/volume/level") {
                        std::cout << "[ReST Glue] VsspExt_Event_Ipc_Therad: audio/volume/level" << std::endl;
                        PyObject* pyobj = jsonToPython(result);
                        PyObject_CallFunction(g_callback_event, "{s:s, s:O}", "URI", uri.c_str(), "VALUE", pyobj);
                    }else if (uri == "audio/volume/mute") {
                        std::cout << "[ReST Glue] VsspExt_Event_Ipc_Therad: audio/volume/mute" << std::endl;
                        PyObject_CallFunction(g_callback_event, "{s:s, s:i}", "URI", uri.c_str(), "VALUE",result["VALUE"].asInt());
                    }else {
                        printf("[ReST Glue] VsspExt_Event_Ipc_Therad: unknown uri=%s\n", uri.c_str());
                    }

                    PyGILState_Release(gstate);
                }
                else
                {
                    printf("[ReST Glue] dslRestNotify, INVALID g_callback_event\n");
                }
            }
        }
    }

   //dolphin free
   for(int i4_lang_num=0;i4_lang_num<LANGUAGE_COUNT;i4_lang_num++)
   {
       for(int i4_msgid_count=0;i4_msgid_count<t_po_file.i4_language_msgid[i4_lang_num];i4_msgid_count++)
	{
	    free(t_po_file.pt_po_msgid_and_msgstr[i4_lang_num][i4_msgid_count].p_msgid);
	    free(t_po_file.pt_po_msgid_and_msgstr[i4_lang_num][i4_msgid_count].p_msgstr);
	}
   }

    return (void*)0;
}

//dplphin test for testing those endpoints
static void* VsspExt_Event_Test_Ipc_Thread(void* arg)
{
    int ret = -1;
    int opt_val =1;
    char buf[512] = {0};

    if(0!=access("/data/vsspdebug",F_OK))
    {
        printf("[ReST Glue] No debug ,just return\n");
        return (void*)0;
    }

    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if (0 > sockfd)
    {
        perror("[ReST Glue] sockfd");
        return (void*)0;
    }
    
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(52013);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,(void*)&opt_val,sizeof(opt_val));
    if(ret < 0)
    {
        perror("set socket opt fail\n");
        close(sockfd);
        return (void*)0;
    }  

    ret = bind(sockfd,(saddrp)&addr,sizeof(addr));
    if (0 > ret)
    {
        perror("[ReST Glue] bind");
        return (void*)0;
    }
    
    struct sockaddr_in src_addr ={};
    socklen_t addr_len = sizeof(struct sockaddr_in);

    printf("[ReST Glue] Create socket finish\n");
    while(1)
    {
       char *p_rec_buf =NULL;
       std::string s_rec_buf;
       Json::Value json_data;
       std::string s_method; 
       PyObject *self=NULL;
       PyObject *args=NULL;
       PyObject *retobj =NULL;
       Json::Value retjson;
	Json::Reader reader;
       std::string s_response_buf;
	Json::FastWriter response;

       memset(buf,0x0,sizeof(buf));
       recvfrom(sockfd,buf,sizeof(buf),0,(saddrp)&src_addr,&addr_len);

       printf("[ReST Glue] Recv:%s Len %d\n",buf,addr_len);
       if (0 != strncmp(buf,p_test_header,strlen(p_test_header)))
       {
           printf("[ReST Glue] Unmatch header\n");
           continue;
       }

       p_rec_buf = buf+strlen(p_test_header);//skip header
       s_rec_buf = std::string(p_rec_buf);
	sendto(sockfd,p_rec_buf,strlen(p_rec_buf),0,(saddrp)&src_addr,addr_len);

       if (reader.parse(s_rec_buf, json_data)){
           printf("[ReST Glue] parse json file succeed\n");
       }
       else{
           printf("[ReST Glue] parse json file failed :[%s] \n",s_rec_buf.c_str());
           return 0;
       }

       s_method = json_data["METHOD"].asString();
       //{"METHOD":"SET","URI": "tv_settings/closed_captions/closed_captions_enabled", "MESSAGE":{"VALUE": "On"}}}
       if(strcmp(s_method.c_str(),"SET")==0){ //set

           PyObject* pTuple = PyTuple_New(2);
           PyTuple_SetItem(pTuple,0,Py_BuildValue("s", json_data["URI"].asCString()));

          Json::Value msg = json_data["MESSAGE"];
	   PyObject *pymsg = jsonToPython(msg);
	   PyTuple_SetItem(pTuple,1,pymsg);

	   args = pTuple;
          retobj = VsspExt_Set(self,args);
          Py_DECREF(pTuple);
       }
	//{"METHOD":"GET","URI":"system_menu/enabled"}
       else if(strcmp(s_method.c_str(),"GET")==0){ //1: get
           PyObject* pTuple = PyTuple_New(1);
           PyTuple_SetItem(pTuple,0,Py_BuildValue("s", json_data["URI"].asCString()));
           args = pTuple;
           retobj = VsspExt_Get(self,args);
           Py_DECREF(pTuple);
       }
       else{ //3:reset & :action
           printf("[ReST Glue] Invaild method type\n");
       }

       memset(buf,0x0,sizeof(buf));
       retjson = pythonToJson(retobj);
	std::cout << response.write(retjson) << std::endl;
       s_response_buf = response.write(retjson);

       sendto(sockfd,s_response_buf.c_str(),s_response_buf.length(),0,(saddrp)&src_addr,addr_len);
    }  
    
    close(sockfd);
    return (void*)0;
}
static pthread_t VsspExt_Event_Ipc_Therad_Id;
static pthread_t VsspExt_Event_Ipc_Test_Thread_Id; //dolphin add 20200416 for test

void initlibSCPL(void) {
    // Module's initialization function
    // Will be called again if you use Python's reload()

#if 0  //dolphin 20191014 no more use Tools.py
	if( !(pyModule = PyImport_ImportModule("Tools") ) ){
		printf("PyImport_ImportModule fail\n");
	}
#endif
    printf("[ReST Glue] dolphin do not use Tools.py\n");
    i4_is_scpl_init_succeed = _vssp_po_init(); //dolphin init
/*
    if(i4_is_scpl_init_succeed) //dolphin print data
    {
        for(int i4_lang_num=0;i4_lang_num<LANGUAGE_COUNT;i4_lang_num++)
        {
            for(int i4_msgid_count=0;i4_msgid_count<t_po_file.i4_language_msgid[i4_lang_num];i4_msgid_count++)
            {
                printf("[%d][%d][%s][%s]\n",i4_lang_num,i4_msgid_count,t_po_file.pt_po_msgid_and_msgstr[i4_lang_num][i4_msgid_count].p_msgid,t_po_file.pt_po_msgid_and_msgstr[i4_lang_num][i4_msgid_count].p_msgstr);
            }
        }
    }
*/
    PyImport_AddModule("libSCPL");
    Py_InitModule("libSCPL", VsspExtMethods);
    
    printf("[ReST Glue] enter initVsspExt \r\n");
    printf("[ReST Glue] VSSP BUFFER_SIZE=%d \r\n",REST_BUFFER_SIZE);


    const char * LanguageList[] = {"English", "Español", "Français"};
    const char * CountryListUSA[] = {"USA", "Canada", "Mexico"};
    
    const char * CountryListESP[] = {"EE.UU.", "Canadá", "México"};
    const char * CountryListFRA[] = {"USA", "Canada", "Mexique"};
   

    for(int i = 0 ; i < LANGUAGE_COUNT; i++)
    {
        PyObject * pyObjLang_Dst = 
        PyUnicode_FromString(LanguageList[i]);
        Py_UNICODE* valueStr_Dst = PyUnicode_AS_UNICODE(pyObjLang_Dst);
        Py_ssize_t size_Dst = PyUnicode_GET_SIZE(pyObjLang_Dst); 
        memcpy(Languages[i], valueStr_Dst, size_Dst*sizeof(Py_UNICODE));  
    }    

    for(int i = 0 ; i < 3; i++)
    {
        PyObject * pyObjCountry_Dst = 
        PyUnicode_FromString(CountryListUSA[i]);
        Py_UNICODE* valueStr_Dst = PyUnicode_AS_UNICODE(pyObjCountry_Dst);
        Py_ssize_t size_Dst = PyUnicode_GET_SIZE(pyObjCountry_Dst); 
        memcpy(CountriesUSA[i], valueStr_Dst, size_Dst*sizeof(Py_UNICODE));        
    }    
    
    for(int i = 0 ; i < 3; i++)
    {
        PyObject * pyObjCountry_Dst = 
        PyUnicode_FromString(CountryListESP[i]);
        Py_UNICODE* valueStr_Dst = PyUnicode_AS_UNICODE(pyObjCountry_Dst);
        Py_ssize_t size_Dst = PyUnicode_GET_SIZE(pyObjCountry_Dst); 
        memcpy(CountriesESP[i], valueStr_Dst, size_Dst*sizeof(Py_UNICODE));        
    }    

    for(int i = 0 ; i < 3; i++)
    {
        PyObject * pyObjCountry_Dst = 
        PyUnicode_FromString(CountryListFRA[i]);
        Py_UNICODE* valueStr_Dst = PyUnicode_AS_UNICODE(pyObjCountry_Dst);
        Py_ssize_t size_Dst = PyUnicode_GET_SIZE(pyObjCountry_Dst); 
        memcpy(CountriesFRA[i], valueStr_Dst, size_Dst*sizeof(Py_UNICODE));        
    } 
    

    Vssp_ipc_init();
    pthread_create(&VsspExt_Event_Ipc_Therad_Id, NULL, VsspExt_Event_Ipc_Therad, NULL);
    pthread_create(&VsspExt_Event_Ipc_Test_Thread_Id, NULL, VsspExt_Event_Test_Ipc_Thread, NULL);

    printf("[ReST Glue] end initVsspExt josnRpcCppInit\r\n"); 
}

int main(int argc, char *argv[])
{
    printf("[ReST Glue] enter main \r\n");

    Py_SetProgramName(argv[0]);
    Py_Initialize();

    printf("[ReST Glue] enter main initlibSCPL \r\n");
    initlibSCPL();

    return 0;
}

#ifdef __cplusplus
}
#endif


/********************************************************************************************
 *     LEGAL DISCLAIMER 
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES 
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED 
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS 
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, 
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, 
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK 
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION 
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *     
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH 
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, 
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE 
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
 *     
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS 
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.  
 ************************************************************************************************/

#ifndef __REST_IPC_H__
#define __REST_IPC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#define TV_IPC_REST_KEY (128 - 5)
#define APP_IPC_REST_KEY (64 - 5)
#define REST_EVENT_IPC_KEY (32 - 5)
#define REST_BUFFER_SIZE 8*1024
#define REST_BUFFER_DATA_SIZE (8*1024 - 2* sizeof(long) - 1)
#define REST_URI_LEN 256

typedef unsigned int  uint32_t;


#define return_if_fail(p) if(!(p))          \
{printf("%s:%d Warning: "#p" failed. \n",   \
    __func__,__LINE__); return;}

#define return_val_if_fail(p, ret) if(!(p)) \
{printf("%s:%d Warning: "#p" failed. \n",   \
__func__,__LINE__); return(ret);}

#define REST_IPC_DEBUG(fmt...) do {printf("[MTK REST_IPC][%s]",__func__);printf(fmt);}while(0)
#define REST_IPC_ERROR(fmt...) do {printf("[MTK REST_IPC Error][%s][%d]",__func__,__LINE__);printf(fmt);}while(0)

typedef enum
{
    VSSP_IPC_R_OK =0,
    VSSP_IPC_R_FAIL =-1,
    VSSP_IPC_R_NO_MSG =-2,
}VSSP_IPC_RET_T;

typedef struct _REST_MSG_BUF{
    long total_size;
    long seq;
    char data_str[REST_BUFFER_SIZE - 2* sizeof(long)];
}REST_MSG_BUF;


typedef  enum{
    REST_KEYCODE_NEXT,
    REST_KEYCODE_PREV,
    REST_KEYCODE_PLAY,
    REST_KEYCODE_PAUSE,
    REST_KEYCODE_STOP,
    REST_KEYCODE_REPEAT_TOGGLE,         // repeat toggle, -> REST
    REST_KEYCODE_SHUFFLE_TOGGLE,        // shuffle toggle, ->REST
    REST_KEYCODE_MUTE_TOGGLE,           // mute toggle,  ->REST
    REST_KEYCODE_VOLUME_UP,             // volume up,-> REST
    REST_KEYCODE_VOLUME_DOWN
}REST_KEYEVENT;

typedef struct
{
    size_t size;
    char data[0];
} RESTShareMemoryData;

VSSP_IPC_RET_T Vssp_ipc_init(void);
VSSP_IPC_RET_T Vssp_ipc_destroy(void);
VSSP_IPC_RET_T Vssp_send_msg_2_rest(REST_MSG_BUF* pSendMsg);
VSSP_IPC_RET_T Vssp_get_msg_from_rest(REST_MSG_BUF* pRecvMsg);
VSSP_IPC_RET_T Vssp_get_msg_from_rest_event(REST_MSG_BUF* pRecvMsg);


#ifdef __cplusplus
}
#endif
#endif



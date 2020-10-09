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
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "Vssp_ipc.h"



typedef struct _VSSP_IPC_MSG_T
{
    int rest_Msgq_Id;
    int Vssp_Msgq_Id;
}VSSP_IPC_MSG_T;


static VSSP_IPC_MSG_T g_Vssp_ipc_msgq_t = {0};
static VSSP_IPC_RET_T _Vssp_q_create(int* p_msg_handle,int key_value);
static VSSP_IPC_RET_T _Vssp_q_destroy(int handle);
static VSSP_IPC_RET_T _Vssp_q_send_msg(int handle, REST_MSG_BUF* pv_msg);
static VSSP_IPC_RET_T _Vssp_q_receive_msg(int handle,REST_MSG_BUF* pv_msg);

static VSSP_IPC_RET_T _Vssp_q_create(int* p_msg_handle,int key_value)
{
    key_t key;
    int flags = IPC_CREAT;
    int msgid = -1;

    key = ftok("/basic/dtv_svc", key_value);
   
    msgid = msgget(key, flags|00666);
    if(-1 == msgid)
    {
        REST_IPC_ERROR("msgget failed!\n");
        *p_msg_handle = 0;
        return VSSP_IPC_R_FAIL;
    }

    fprintf(stderr, "\n%s,%d,key=%d,msgid=%d\n", __FUNCTION__, __LINE__, key, msgid);

    *p_msg_handle = msgid;
    return VSSP_IPC_R_OK;
}

static VSSP_IPC_RET_T _Vssp_q_destroy(int handle)
{
    msgctl(handle, IPC_RMID, NULL);

    return VSSP_IPC_R_OK;
}

static VSSP_IPC_RET_T _Vssp_q_send_msg(int handle, REST_MSG_BUF* pv_msg)
{
    int ret_val = -1;
     
    ret_val = msgsnd(handle, pv_msg, REST_BUFFER_SIZE + REST_URI_LEN, IPC_NOWAIT);
    if(-1 == ret_val)
    {
        REST_IPC_ERROR("\nmsg queue send failed\n");
        if(errno == EAGAIN)
        {
            REST_IPC_ERROR("\nmsg queue is full!\n");
        }

        return VSSP_IPC_R_FAIL;
    }

    return VSSP_IPC_R_OK;
}

static VSSP_IPC_RET_T _Vssp_q_receive_msg(int handle,REST_MSG_BUF* pv_msg)
{
    int ret_val = -1;

    ret_val = msgrcv(handle, (void*)pv_msg, REST_BUFFER_SIZE + REST_URI_LEN, 0, 0);
    if(ret_val < 0)
    {
        if(errno == E2BIG)
        {
            REST_IPC_ERROR("\ncurrent msg is too big!\n");
        }
        else if(errno == ENOMSG)
        {
            return VSSP_IPC_R_NO_MSG;
        }
        return VSSP_IPC_R_FAIL;
    }

    return VSSP_IPC_R_OK;
}

VSSP_IPC_RET_T Vssp_ipc_init(void)
{
    REST_IPC_DEBUG("Enter!\n");

    VSSP_IPC_RET_T ret_val;
    ret_val = _Vssp_q_create(&(g_Vssp_ipc_msgq_t.rest_Msgq_Id),TV_IPC_REST_KEY);
    if(ret_val != VSSP_IPC_R_OK)
    {
        REST_IPC_ERROR("\ncreate TV msg queue failed!\n");
        return VSSP_IPC_R_FAIL;
    }

    REST_IPC_DEBUG("\ncreate TV msg queue finished!\n");
    
    ret_val = _Vssp_q_create(&(g_Vssp_ipc_msgq_t.Vssp_Msgq_Id),APP_IPC_REST_KEY);
    if(ret_val != VSSP_IPC_R_OK)
    {
        REST_IPC_ERROR("\ncreate app msg queue failed!\n");
        return VSSP_IPC_R_FAIL;
    }

    REST_IPC_DEBUG("\ncreate app msg queue finished!\n");

    REST_IPC_DEBUG("Leave!\n");

    return VSSP_IPC_R_OK;    
}

VSSP_IPC_RET_T Vssp_ipc_destroy(void)
{
    _Vssp_q_destroy(g_Vssp_ipc_msgq_t.Vssp_Msgq_Id);
    _Vssp_q_destroy(g_Vssp_ipc_msgq_t.rest_Msgq_Id);

    memset(&g_Vssp_ipc_msgq_t, 0, sizeof(VSSP_IPC_MSG_T));

    return VSSP_IPC_R_OK;
}
VSSP_IPC_RET_T Vssp_send_msg_2_rest(REST_MSG_BUF* pSendMsg)
{
    return_val_if_fail((pSendMsg!=NULL),VSSP_IPC_R_FAIL);

    VSSP_IPC_RET_T ret_val;
    ret_val = _Vssp_q_send_msg(g_Vssp_ipc_msgq_t.rest_Msgq_Id,pSendMsg);
    if(ret_val != VSSP_IPC_R_OK)
    {
        REST_IPC_ERROR("\n%s,%dapp send message to TV failed!\n", __FUNCTION__, __LINE__);
        return VSSP_IPC_R_FAIL;
    }

    return VSSP_IPC_R_OK;
}

VSSP_IPC_RET_T Vssp_get_msg_from_rest(REST_MSG_BUF* pRecvMsg)
{
    return_val_if_fail((pRecvMsg!=NULL),VSSP_IPC_R_FAIL);

    VSSP_IPC_RET_T ret_val;
    ret_val = _Vssp_q_receive_msg(g_Vssp_ipc_msgq_t.Vssp_Msgq_Id,pRecvMsg);
    if(ret_val != VSSP_IPC_R_OK)
    {
        REST_IPC_ERROR("\n%s,%d,app get message to TV failed!\n", __FUNCTION__, __LINE__);
        return VSSP_IPC_R_FAIL;
    }
    
    return VSSP_IPC_R_OK;
}










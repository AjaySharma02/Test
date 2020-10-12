//
//  VsspExt.c
//  VIZIO Second Screen Protocol Python Extensions
//
//  Created by Bill Baxter on 3/30/15.
//  Copyright (c) 2015 Bill Baxter. All rights reserved.
//

#include <Python.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <pthread.h>
#include "helpers.h"
#include "errorcodes.h"
#include "Vssp_ipc.h"
#include <cJSON.h>
#include <sys/types.h>
#include <unistd.h>


VSSP_IPC_RET_T Vssp_send_notify_msg(cJSON* pv_msg,long mtype,const char *uri);

static cJSON * 
create_bool_json(PyObject* obj)
{
    if(obj == Py_False)
    {
        return cJSON_CreateFalse();
    }
    else
    {
        return cJSON_CreateTrue();
    }
}

static cJSON * 
create_json_from_python(PyObject* obj)
{
    cJSON * data = NULL;
    PyObject* subObj = NULL;
    cJSON * subItem = NULL;
    Py_ssize_t size = 0;
    PyObject * key = NULL;
    Py_ssize_t pos = 0;
    int i = 0;
    
    if(Py_None == obj)
    {
        printf("obj is py_none\n");
        data = cJSON_CreateNull();
    }
    else if(PyBool_Check(obj))
    {
        printf("obj is bool\n");//Py_False,Py_True
        data = create_bool_json(obj);
    }
    else if(PyInt_CheckExact(obj))
    {
        printf("obj is int \n");
        data = cJSON_CreateNumber(PyInt_AsLong(obj));
    }
    else if(PyFloat_CheckExact(obj))
    {
        printf("obj is float \n");
        data = cJSON_CreateNumber(PyFloat_AsDouble(obj));
    }
    else if(PyUnicode_CheckExact(obj))
    {
        printf("obj is Unicode string\n");
        data = cJSON_CreateString(PyString_AsString(obj));
    }
    else if(PyString_CheckExact(obj))
    {
        printf("obj is string\n");
        data = cJSON_CreateString(PyString_AsString(obj));
    }
    else if(PyList_CheckExact(obj))
    {
        printf("obj is list\n");
        data = cJSON_CreateArray();
        size = PyList_Size(obj);
        for(i = 0;i < size;i++)
        {
            subObj = PyList_GetItem(obj,i);
            subItem = create_json_from_python(subObj);
            cJSON_AddItemToArray(data,subItem);
        }
    }
    else if(PyDict_CheckExact(obj))
    {
        printf("obj is dictionary\n");
        data = cJSON_CreateObject();
        while(PyDict_Next(obj,&pos,&key,&subObj))
        {
            subItem = create_json_from_python(subObj);
            cJSON_AddItemToObject(data,PyString_AsString(key),subItem);
        }
    }
    else
    {
        printf("obj is unknown\n");
    }
    return data;
}

static PyObject * 
create_python_from_json(cJSON* data,char** key)
{
    PyObject * py = NULL;
    cJSON * child = NULL;
    char * child_key = NULL;
    PyObject * child_py = NULL;
    double d;
    *key = NULL;
    if(data->string)
    {
        printf("key:%s\n",data->string);
        *key = strdup(data->string);
    }
    switch(data->type)
    {
        case cJSON_False:
            printf("....create False \n");
            py = Py_False;
            break;
        case cJSON_True :
            printf("....create True \n");
            py = Py_True;
            
            break;
        case cJSON_NULL :
            printf("....create null \n");
            py = Py_None;
            break;
        case cJSON_Number :
            printf("...create number:");
            if(data->valueint)
                printf("....int number:%d\n",data->valueint);
            if(data->valuedouble)
                printf("....double number:double:%f\n",data->valuedouble);
            //py = Py_BuildValue("d",data->valuedouble);
            d =data->valuedouble;
            if (fabs(((double)data->valueint)-d)<=DBL_EPSILON && d<=INT_MAX && d>=INT_MIN)
            {
                py = Py_BuildValue("i",data->valueint);
            }
            else
            {
                py = Py_BuildValue("d",data->valuedouble);
            }
            //py = PyInt_FromLong((long)data->valueint);
            printf("after build value\n");
            
            break;
        case cJSON_String:
            printf("....create string:%s\n",data->valuestring);
            py = Py_BuildValue("s",data->valuestring);
            
            break;
        case cJSON_Array:
        {
            printf("....create list\n");
            py = PyList_New(0);
            child = data->child;
            while(child)
            {
                child_py = create_python_from_json(child,&child_key);
                if(child_py == NULL)
                    break;
                printf("append a child\n");
                PyList_Append(py,child_py);
                Py_DECREF(child_py);
                if(child_key)
                {
                    printf("list item should have no key\n");
                    free(child_key);
                }       
                child = child->next;
            }
        }    
            break;
        case cJSON_Object:
        {
            printf("....create dict\n");
            py = PyDict_New();
            child = data->child;
            while(child)
            {
                child_py = create_python_from_json(child,&child_key);
                if(child_py == NULL)
                    break;
                printf("append a child\n");
                if(!child_key)
                {
                    printf("dict item should have a key\n");
                }
                PyDict_SetItemString(py,child_key,child_py);
                Py_DECREF(child_py);
                if(child_key)
                {
                    free(child_key);
                }
                child = child->next;
            }
        }
            break;
        default:
        {
            printf("....default create_python_from_json\n");
        }
            break;
    }
    return py;
}

//
// VsspExt.Set
// Sets the value of the resource referenced by parameter 'uri' to the parameter 'newValue'
static PyObject *
VsspExt_Set(PyObject *self, PyObject *args)
{
    const char *uri;
    PyObject* newValueObj;
    int errCode = API_ERR_INVALID_PARAMETER;
    cJSON * jsonData = NULL;
    char * output = NULL;
    
    // Get the URI, URI Len, and the Python Dictionary that represents the new value
    if (!PyArg_ParseTuple(args, "sO!", &uri, &PyDict_Type, &newValueObj))
    {
        printf("%s,%d\n", __FUNCTION__, __LINE__);
        return Py_BuildValue("i", API_ERR_INVALID_PARAMETER);
    }
    printf("%s,%s,%d,URI = %s\n", __FILE__, __FUNCTION__, __LINE__, uri);
    if (!newValueObj || !PyDict_Check(newValueObj))
    {
        printf("%s,%d\n", __FUNCTION__, __LINE__);
        return Py_BuildValue("i", API_ERR_INVALID_PARAMETER);
    }
    // Handle processing the path components
    char * components[10];
    size_t count = splitStr(uri, components, L'/', 10);
    if (count <= 0)
    {
        printf("%s,%d\n", __FUNCTION__, __LINE__);
        return Py_BuildValue("i", API_ERR_INVALID_PARAMETER);
    }

    /*transtor python data to cjson data and send to dtv_svc */
    jsonData = create_json_from_python(newValueObj);
    output = cJSON_Print(jsonData);
    /*Ipc send cjson data*/
    Vssp_send_notify_msg(jsonData,REST_MSG_REQUEST_SET,uri);
    
    /*Wait for response for restipc*/
    rest_mutex_lock();
    while(!is_has_response()){
        rest_cond_wait();
    }
    REST_MSG_BUF * pMsg = get_response_data();
    set_has_response(FALSE);
    rest_mutex_unlock();

    /*Send response data to VSSPAIL*/
    jsonData = cJSON_Parse(pMsg->data_str);
    errCode = cJSON_GetObjectItem(jsonData,"SUCCESS")->valueint;

    /*Release the memory allocated by splitStr*/
    int i =0 ;
    for (i = 0; i < count; ++i)
    {
        if(NULL != components[i])
        {
            free(components[i]);
        }
    }
    if(NULL != output)
    {
        free(output);
    }
    if(NULL != jsonData)
    {
        cJSON_Delete(jsonData);
    }
    return Py_BuildValue("i", errCode);
}
// VsspExt.Action
// Resets the value of the resource referenced by parameter 'uri'
static PyObject *
VsspExt_Action(PyObject *self, PyObject *args)
{
    const char *uri;
    PyObject* newValueObj;
    //int errCode = API_ERR_INVALID_PARAMETER;
    cJSON * jsonData = NULL;
    char * output = NULL;

    if (!PyArg_ParseTuple(args, "sO!", &uri, &PyDict_Type, &newValueObj))
    {
        printf("%s,%d\n", __FUNCTION__, __LINE__);
        return Py_BuildValue("i", API_ERR_INVALID_PARAMETER);
    }

    if (!newValueObj || !PyDict_Check(newValueObj))
    {
        printf("%s,%d\n", __FUNCTION__, __LINE__);
        return Py_BuildValue("i", API_ERR_INVALID_PARAMETER);
    }
    
    /*transtor python data to cjson data and send to dtv_svc */
    jsonData = create_json_from_python(newValueObj);
    output = cJSON_Print(jsonData);

    // Handle processing the path components
    char * components[10];

    size_t count = splitStr(uri, components, L'/', 10);
    if (count <= 0)
    {
        printf("%s,%d\n", __FUNCTION__, __LINE__);
        return Py_BuildValue("i", API_ERR_INVALID_PARAMETER);
    }

    Vssp_send_notify_msg(NULL,REST_MSG_REQUEST_ACTION,uri);

    /* Wait response from restipc */
    rest_mutex_lock();
    while(!is_has_response())
    {
        rest_cond_wait();
    }
    REST_MSG_BUF * pMsg = get_response_data();
    set_has_response(FALSE);
    rest_mutex_unlock();

    // Release the memory allocated by splitStr
    int i = 0;
    for (i = 0; i < count; ++i)
    {
        free(components[i]);
    }
    // Return the error code
    return Py_BuildValue("i", 0);
}

// VsspExt.Get
// Gets the value of the resource referenced by parameter 'uri'
static PyObject *
VsspExt_Get(PyObject *self, PyObject *args)
{
    const char *uri;
    if (!PyArg_ParseTuple(args, "s", &uri))
    {
        printf("%s,%d\n", __FUNCTION__, __LINE__);
        return Py_BuildValue("i", API_ERR_INVALID_PARAMETER);
    }

    /* Handle processing the path components */
    char * components[10];
    size_t count = splitStr(uri, components, L'/', 10);
    if (count <= 0)
    {
        printf("%s,%d\n", __FUNCTION__, __LINE__);
        return Py_BuildValue("i", API_ERR_INVALID_PARAMETER);
    }

    Vssp_send_notify_msg(NULL,REST_MSG_REQUEST_GET,uri);

    /* Wait response from restipc */
    rest_mutex_lock();
    while(!is_has_response())
    {
        rest_cond_wait();
    }
    REST_MSG_BUF * pMsg = get_response_data();
    set_has_response(FALSE);
    rest_mutex_unlock();

    /* Send response data to VSSPAIL */
    cJSON *root;
    root = cJSON_Parse(pMsg->data_str);
    if (NULL == root)
    {
        printf("VsspExt_Get:cJSON_Parse fail!\n");
        return NULL;
    }

    char * key = NULL;
    PyObject* result = create_python_from_json(root,&key);
    if(NULL == result)
    {
        printf("VsspExt_Get:create_python_from_json fail!\n");
    }
    
    // Release the memory allocated by splitStr
    int i = 0;
    for (i = 0; i < count; ++i)
    {
        free(components[i]);
    }

    return result;
}
// VsspExt.AcrGet
// Gets the value of the resource referenced by parameter 'uri'
static PyObject *
VsspExt_AcrGet(PyObject *self, PyObject *args)
{
    int i;
    const char *uri;

    if (!PyArg_ParseTuple(args, "s", &uri))
        return Py_BuildValue("i", API_ERR_INVALID_PARAMETER);

    // Handle processing the path components
    char * components[10];

    size_t count = splitStr(uri, components, L'/', 10);
    if (count <= 0)
        return Py_BuildValue("i", API_ERR_INVALID_PARAMETER);

    PyObject* result = NULL;

    // Invoke the proper handler
    if (strcasecmp(components[0],"state") == 0) {
        // If this is TV product, invoke the TV state
 //       result = TV_State_Settings_Get(&(components[1]), count-1);
    }

    // Release the memory allocated by splitStr
    for (i = 0; i < count; ++i)
    {
        free(components[i]);
    }

    return result;
}


// VsspExt.Reset
// Resets the value of the resource referenced by parameter 'uri'
static PyObject *
VsspExt_Reset(PyObject *self, PyObject *args)
{
    const char *uri;

    if (!PyArg_ParseTuple(args, "s", &uri))
        return Py_BuildValue("i", API_ERR_INVALID_PARAMETER);

    // Handle processing the path components
    char * components[10];

    size_t count = splitStr(uri, components, L'/', 10);

    printf("found %zu path components\n",count);

    if (count <= 0)
        return Py_BuildValue("i", API_ERR_INVALID_PARAMETER);

    printf("Settings for model %s\n", components[0]);

    Vssp_send_notify_msg(NULL,REST_MSG_REQUEST_RESET,uri);

    /* Wait response from restipc */
    rest_mutex_lock();
    while(!is_has_response()){
        rest_cond_wait();
    }
    REST_MSG_BUF * pMsg = get_response_data();
    printf("msg id:%ld \n, msg:%s\n",pMsg->mtype,pMsg->data_str);
    set_has_response(FALSE);
    rest_mutex_unlock();

    // Release the memory allocated by splitStr
    int i = 0;for (i = 0; i < count; ++i) {
        free(components[i]);
    }

    // Return the error code
    return Py_BuildValue("i", 0);
}


// VsspExt.Pairing_Start
// Indicate pairing started and return current challenge type
static PyObject *
VsspExt_Pairing_Start(PyObject *self, PyObject *args)
{
    return VsspExt_Get(self,args);
}

// VsspExt.Pairing_End
// Indicate pairing has ended
static PyObject *
VsspExt_Pairing_End(PyObject *self, PyObject *args)
{
     return VsspExt_Action(self,args);
}

// VsspExt.Pairing_Has_Executed_Challenge
// Return indication of whether the platform-secific pairing challenge was executed
static PyObject *
VsspExt_Pairing_Has_Executed_Challenge(PyObject *self, PyObject *args)
{
    return VsspExt_Get(self,args);
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
    {NULL, NULL, 0, NULL}
};



static bool b_thread_running = TRUE;

static void* VsspExt_IpcTherad(void* arg)
{
    (void)arg;
    REST_MSG_BUF msg = {0};
    VSSP_IPC_RET_T ret = VSSP_IPC_R_FAIL;
    while(b_thread_running)
    { 
        memset(&msg, 0, sizeof(REST_MSG_BUF));
        ret = Vssp_get_msg_from_rest(&msg);
        if(VSSP_IPC_R_OK != ret)
        {
            printf("%s,%d,Vssp_get_msg_from_rest error,ret=%d\n",
                    __FUNCTION__, __LINE__, ret);
        }
        else
        {
            printf("%s,%d,msg type:%ld\n" ,
                    __FUNCTION__, __LINE__, msg.mtype);
            switch(msg.mtype)
            {
                case REST_MSG_RESPONSE_FROM_TV:
                {
                    rest_mutex_lock();
                    //post signal
                    set_response_data(&msg);
                    set_has_response(TRUE);
                    rest_cond_signal();
                    rest_mutex_unlock();
                    break;
                }
                    
                default:
                    printf("msg type unknown\n");
                    break;
            }
        }
    }
    return (void*)0;
}

VSSP_IPC_RET_T Vssp_send_notify_msg(cJSON* pv_msg,long mtype,const char *uri)
{
    REST_MSG_BUF notify_msg;
    memset(&notify_msg,0,sizeof(REST_MSG_BUF));
    notify_msg.mtype = mtype;
    strncpy(notify_msg.uri,uri,strlen(uri));
    if (NULL != pv_msg)
    {
        strcpy(notify_msg.data_str,cJSON_Print(pv_msg));
    }
    
    int i4_ret = Vssp_send_msg_2_rest(&notify_msg);
    if(0 != i4_ret)
    {
        REST_IPC_ERROR(("ipc send msg fail"));
        return VSSP_IPC_R_FAIL;
    }
    REST_IPC_DEBUG("Vssp_send_notify_msg-->uri:%s, res:%s !\r\n",uri,cJSON_Print(pv_msg));

    return VSSP_IPC_R_OK;
}

void initVsspExt(void) {
    // Module's initialization function
    // Will be called again if you use Python's reload()
    pthread_t t_id;
    PyImport_AddModule("VsspExt");
    Py_InitModule("VsspExt", VsspExtMethods);
    Vssp_ipc_init();
    pthread_create(&t_id,NULL,VsspExt_IpcTherad,NULL);
}

int main(int argc, char *argv[])
{
    Py_SetProgramName(argv[0]);
    Py_Initialize();

    initVsspExt();

    return 0;
}



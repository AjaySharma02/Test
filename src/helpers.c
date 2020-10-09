//
//  helpers.c
//  VIZIO Second Screen Protocol Helper functions
//
//  Created by Bill Baxter on 3/30/15.
//  Copyright (c) 2015 Bill Baxter. All rights reserved.
//

#include <Python.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>


#include "Vssp_ipc.h"
#include "helpers.h"




static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static bool b_has_response = FALSE;
static REST_MSG_BUF s_rest_msg = {0};

//
// splitStr
// Parameters:
//    str: string to split
//    pointers: arrays of string pointers of length maxComponents
//    splitChar: character to split string on
//    maxComponents: max number of components to split
// Returns:
//    Up to maxComponents strings split from str.
// Note:
//    It is up to caller to free the strings allocated by this method.
//
size_t
splitStr(const char * str, char*pointers[], char splitChar, int maxComponents) {
    // fprintf(stderr,"splitStr = %s", str);
    if (str == NULL) {
        return 0;
    }
    if (pointers == NULL) {
        return 0;
    }
    if (maxComponents < 1) {
        return 0;
    }
    int index = 0;
    size_t len = strlen(str);

    if (len == 0) {
        return 0;
    }

    char * start = (char*)str;
    char * curPtr = start;
    while (*curPtr) {
        //fprintf(stderr,"Str = %s\n", &(str[j]));
        if (index == maxComponents)
            break;
        if (*curPtr == '/') {
            size_t allocCount = (curPtr - start) + 1;
            pointers[index] = (char*)malloc(allocCount);
            memset(pointers[index],0,allocCount);
            strncpy(pointers[index], start, allocCount-1);
            fprintf(stderr,"SplitStr:Component[%d] = %s\n", index, pointers[index]);
            start = ++curPtr;
            ++index;
        }
        else {
            ++curPtr;
        }
    }

    // Pickup last item
    if (index < maxComponents && start < curPtr) {
        size_t allocCount = (curPtr - start) + 1;
        pointers[index] = (char*)malloc(allocCount);
        strcpy(pointers[index], start);
        fprintf(stderr,"SplitStr:Component[%d] = %s\n", index, pointers[index]);
        ++index;
    }
    return index;
}

PyObject* apiResultDict(int errCode) {
    PyObject* result = PyDict_New();
    if (!result){
        return NULL;
    }

    int err = PyDict_SetItemString(result,"SUCCESS", Py_BuildValue("i", errCode));
    if (-1 == err){
        fprintf(stderr, "apiResultDict fail!\n");
        return NULL;
    }
    fprintf(stderr, "apiResultDict success\n");

    return result;
}

void rest_mutex_lock(void)
{
    pthread_mutex_lock(&mtx);
}

void rest_cond_wait(void)
{
    pthread_cond_wait(&cond,&mtx);
}

void rest_cond_signal(void)
{
    pthread_cond_signal(&cond);
}

void rest_mutex_unlock(void)
{
    pthread_mutex_unlock(&mtx);
}

void set_response_data(REST_MSG_BUF *response)
{
    memcpy(&s_rest_msg,response,sizeof(REST_MSG_BUF));
}

REST_MSG_BUF * get_response_data(void)
{
    return &s_rest_msg;
}

bool is_has_response(void)
{
    return b_has_response;
}

void set_has_response(bool val)
{
    b_has_response = val;
}





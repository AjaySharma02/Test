//
//  helpers.h
//  VIZIO Second Screen Protocol Helper functions
//
//  Created by Bill Baxter on 3/30/15.
//  Copyright (c) 2015 Bill Baxter. All rights reserved.
//

#include <Python.h>
#include "Vssp_ipc.h"


typedef int bool;
#define TRUE 1
#define FALSE 0

size_t splitStr(const char * str, char* pointers[], char splitChar, int maxComponents);
PyObject* apiResultDict(int errCode);
void rest_mutex_lock(void);
void rest_cond_wait(void);
void rest_cond_signal(void);
void rest_mutex_unlock(void);
void set_response_data(REST_MSG_BUF *response);
REST_MSG_BUF * get_response_data(void);
bool is_has_response(void);
void set_has_response(bool val);


#ifndef __GLOBALS_H
#define __GLOBALS_H

#include "allinc.h"

extern int http_listen_handle;
extern int https_listen_handle;
extern char readblob[8192];
extern char writeblob[8192];
extern int* handles;
extern unsigned int thread_count;
extern pthread_t* threads;

#endif //__GLOBALS_H

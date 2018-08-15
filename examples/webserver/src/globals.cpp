
#include "allinc.h"

int http_listen_handle;
int https_listen_handle;
char readblob[8192];
char writeblob[8192];
unsigned int thread_count;
int* handles;
pthread_t* threads;

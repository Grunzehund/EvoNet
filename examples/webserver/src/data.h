#ifndef __DATA_H
#define __DATA_H

#include "allinc.h"

struct Data
{
	static Data* chain_first;
	static Data* chain_last;

	Data();
	~Data();

	void Load(char* filename, int dirlen);

	char* data;
	char* path;
	char* real_path;

	unsigned long int data_len;

	struct Data* chain_prev = NULL;
	struct Data* chain_next = NULL;
};

#endif //__DATA_H


#include "allinc.h"

Map* save_map;
Map* load_map;

int main(int argc, char** argv)
{
	printf("Program started\n");

	save_map = new Map(2048, 2048);
	save_map->GenerateMap();
	save_map->InitMap();
	save_map->Save("rc/testmap.map");

	load_map = new Map("rc/testmap.map");

	printf("program terminated\n");

	return 0;
}

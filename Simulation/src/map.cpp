
#include "allinc.h"

Map::Map(uint64_t _width, uint64_t _height)
{
	width = _width;
	height = _height;

#ifdef DEBUG_STRUCTOR
	printf("Map Constructor called, width = %d, height = %d\n", width, height);
#endif

	noise_map = new double*[width];
	for (uint64_t i=0; i < width; i++)
		noise_map[i] = new double[height];
	landscape_map = new double*[width];
	for (uint64_t i=0; i < width; i++)
		landscape_map[i] = new double[height];

	used_features = FILE_FLAG_NOISEMAP | FILE_FLAG_LANDMAP;
#ifdef DEBUG_STRUCTORS
	printf("[%p][%p] allocations done\n", noise_map, landscape_map);
	printf("using features with code %zu\n", used_features);
	printf("min_land_height = %.5f\n", min_land_height);
#endif
}
Map::Map(const char* _filename)
{
	Load(_filename);
}
Map::Map(char* _filename)
{
	Load(_filename);
}
Map::~Map()
{
	for (uint64_t i=0; i < width; i++)
		delete[] landscape_map[i];
	delete[] landscape_map;
	for (uint64_t i=0; i < width; i++)
		delete[] noise_map[i];
	delete[] noise_map;
}



/* Rendering stuff */
void Map::GrowAll(double v)
{
	for (uint64_t i=0; i < width; i++)
	{
		for (uint64_t j=0; j < height; j++)
		{
			if (landscape_map[i][j] >= 0)                                                                                                      //TODO: probably width: if (noise_map[i][j] > min_land_height)
				landscape_map[i][j] += v;
		}
	}
}
inline void Map::Add(uint64_t _x, uint64_t _y, double v)
{
#ifdef DEBUG_SAVE_MODE
	if (_x < 0 || _y < 0 || _x > width || _y > height)
		return;
#endif
	landscape_map[_x][_y] += v;
}
inline void Map::SetGrowthRate(double v)
{
	growth_rate = v;
}
inline void Map::SetMinLandHeight(double v)
{
	min_land_height = v;
}





/* Map Generation stuff */
bool Map::GenerateMap(NoiseMap nm)
{
#ifdef DEBUG_SAVE_MODE
	if (nm.width != width || nm.height != height)
		return false;
#endif

	return CreateMap(nm.map);
}
bool Map::GenerateMap()
{
	NoiseMap nm(width, height, log2(width) / 2, 1, 1);
	
	nm.GenerateMap();

	CreateMap(nm.map);

	return true;
}
bool Map::GenerateMap(double** _map)
{
#ifdef DEBUG_SAVE_MODE
	if (_map == nullptr)
		return false;
#endif

	return CreateMap(_map);
}
bool Map::InitMap()
{
	uint64_t error_cnt = 0;
	double f = 1.0 / (1.0 - min_land_height);
	printf("f = %.5f\n", f);

	for (uint64_t i=0; i < width; i++)
	{
		for (uint64_t j=0; j < height; j++)
		{
			if (landscape_map[i][j] >= 0.0)
			{
				landscape_map[i][j] = (noise_map[i][j] - min_land_height) * f;
#ifdef DEBUG_MAP_INITIALISATION
				if (landscape_map[i][j] < 0.0 || landscape_map[i][j] > 1.0)
					error_cnt++;
#endif
			}
#ifdef DEBUG_MAP_INITIALISATION
			printf("[%p]landscape_map[%zu][%zu] = %.5f\n", &landscape_map[i][j], i, j, landscape_map[i][j]);
#endif
		}
	}
#ifdef DEBUG_MAP_INITIALISATION
	printf("total %zu errors\n", error_cnt);
#endif
	return true;
}
bool Map::CreateMap(double** _map)
{
#ifdef DEBUG_SAVE_MODE
	if (noise_map == nullptr || landscape_map == nullptr || width <= 0 || height <= 0)
		return false;
#endif
#ifdef DEBUG_MAP_CREATION
	printf("min_land_height = %.5f\n", min_land_height);
#endif

	for (uint64_t i=0; i < width; i++)
	{
		for (uint64_t j=0; j < height; j++)
		{
			if (noise_map[i][j] >= min_land_height)
				landscape_map[i][j] = 0.0;
			else
				landscape_map[i][j] = -1.0;
		}
	}
	return true;
}
void Map::Render()
{
	GrowAll(growth_rate);
}




/* Load / Save stuff */
bool Map::Save(char* _filename)
{
	int f;

#ifdef DEBUG_LOADSAVE
	printf("Saving Map to %s: width (%zu), height (%zu), used_features (%zu), [%p]noise_map, [%p]landscape_map\n", _filename, width, height, used_features, noise_map, landscape_map);
#endif

	f = open(_filename, O_WRONLY | O_CREAT, 0644);

	if (f < 0)
	{
#ifdef DEBUG_LOADSAVE
		printf("failed to open file %s: handle f = %d\n", _filename, f);
#endif
		return false;
	}

	write(f, &width, sizeof(uint64_t));
	write(f, &height, sizeof(uint64_t));
	write(f, &used_features, sizeof(uint64_t));

#ifdef DEBUG_LOADSAVE
	printf("successfully wrote width (%zu), height (%zu), used_features (%zu)\n", width, height, used_features);
#endif

	for (uint64_t i=0; i < width; i++)
		write(f, noise_map[i], height * sizeof(double));
#ifdef DEBUG_LOADSAVE
	printf("successfully wrote noisemap\n");
#endif

	for (uint64_t i=0; i < width; i++)
		write(f, landscape_map[i], height * sizeof(double));
#ifdef DEBUG_LOADSAVE
	printf("successfully wrote landmap\n");
#endif
	
	close(f);

	return true;
}
bool Map::Save(const char* _filename)
{
	int f;

#ifdef DEBUG_LOADSAVE
	printf("Saving Map to %s: width (%zu), height (%zu), used_features (%zu), [%p]noise_map, [%p]landscape_map\n", _filename, width, height, used_features, noise_map, landscape_map);
#endif

	f = open(_filename, O_WRONLY | O_CREAT, 0644);

	if (f < 0)
	{
#ifdef DEBUG_LOADSAVE
		printf("failed to open file %s: handle f = %d\n", _filename, f);
#endif
		return false;
	}

	write(f, &width, sizeof(uint64_t));
	write(f, &height, sizeof(uint64_t));
	write(f, &used_features, sizeof(uint64_t));

#ifdef DEBUG_LOADSAVE
	printf("successfully wrote width (%zu), height (%zu), used_features (%zu)\n", width, height, used_features);
#endif

	for (uint64_t i=0; i < width; i++)
		write(f, noise_map[i], height * sizeof(double));
#ifdef DEBUG_LOADSAVE
	printf("successfully wrote noisemap\n");
#endif

	for (uint64_t i=0; i < width; i++)
		write(f, landscape_map[i], height * sizeof(double));
#ifdef DEBUG_LOADSAVE
	printf("successfully wrote landmap\n");
#endif
	
	close(f);

	return true;
}

bool Map::Load(const char* _filename)
{
	int f;

#ifdef DEBUG_LOADSAVE
	printf("Loading Map from file %s\n", _filename);
#endif

	f = open(_filename, O_RDONLY);

	if (f < 0)
	{
#ifdef DEBUG_LOADSAVE
		printf("failed to open file %s, f = %d\n", _filename, f);
#endif
		return false;
	}

	read(f, &width, sizeof(uint64_t));
	read(f, &height, sizeof(uint64_t));
	read(f, &used_features, sizeof(uint64_t));

#ifdef DEBUG_LOADSAVE
	printf("successfully loaded width(%zu), height(%zu), used_features(%zu)\n", width, height, used_features);
#endif

	if (used_features & FILE_FLAG_NOISEMAP)
	{
		noise_map = new double*[width];
		for (uint64_t i=0; i < width; i++)
		{
			noise_map[i] = new double[height];
			read(f, noise_map[i], height * sizeof(double));
		}
	}
#ifdef DEBUG_LOADSAVE
	printf("[%p]successfully loaded noisemap\n", noise_map);
#endif

	if (used_features & FILE_FLAG_LANDMAP)
	{
		landscape_map = new double*[width];
		for (uint64_t i=0; i < width; i++)
		{
			landscape_map[i] = new double[height];
			read(f, landscape_map[i], height * sizeof(double));
		}
	}
#ifdef DEBUG_LOADSAVE
	printf("[%p]successfully loaded landmap\n", landscape_map);
#endif

	close(f);
}
bool Map::Load(char* _filename)
{
	int f;

#ifdef DEBUG_LOADSAVE
	printf("Loading Map from file %s\n", _filename);
#endif

	f = open(_filename, O_RDONLY);

	if (f < 0)
	{
#ifdef DEBUG_LOADSAVE
		printf("failed to open file %s, f = %d\n", _filename, f);
#endif
		return false;
	}

	read(f, &width, sizeof(uint64_t));
	read(f, &height, sizeof(uint64_t));
	read(f, &used_features, sizeof(uint64_t));

#ifdef DEBUG_LOADSAVE
	printf("successfully loaded width(%zu), height(%zu), used_features(%zu)\n", width, height, used_features);
#endif

	if (used_features & FILE_FLAG_NOISEMAP)
	{
		noise_map = new double*[width];
		for (uint64_t i=0; i < width; i++)
		{
			noise_map[i] = new double[height];
			read(f, noise_map[i], height * sizeof(double));
		}
	}
#ifdef DEBUG_LOADSAVE
	printf("[%p]successfully loaded noisemap\n", noise_map);
#endif

	if (used_features & FILE_FLAG_LANDMAP)
	{
		landscape_map = new double*[width];
		for (uint64_t i=0; i < width; i++)
		{
			landscape_map[i] = new double[height];
			read(f, landscape_map[i], height * sizeof(double));
		}
	}
#ifdef DEBUG_LOADSAVE
	printf("[%p]successfully loaded landmap\n", landscape_map);
#endif

	close(f);
}

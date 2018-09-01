
#include "allinc.h"

Map::Map(uint64_t _width, uint64_t _height)
{
	width = _width;
	height = _height;

	map = new double*[width];
	for (uint64_t i=0; i < width; i++)
		map[i] = new double[height];
}
Map::~Map()
{
	for (uint64_t i=0; i < width; i++)
		delete[] map[i];
	delete[] map;
}
virtual void Map::GrowAll(double v)
{
	for (uint64_t i=0; i < width; i++)
	{
		for (uint64_t j=0; j < height; j++)
			map[i][j] += v;
	}
}
virtual bool Map::Load(const char* _filename)
{
	int f;

	f = open(_filename, O_RDONLY);

	if (f < 0)
		return false;

	read(f, &width, sizeof(uint64_t));
	read(f, &height, sizeof(uint64_t));

	map = new double*[width];
	for (uint64_t i=0; i < width; i++)
	{
		map[i] = new double[height];
		read(f, map[i], sizeof(double)*height);
	}

	close(f);

	return true;

}
virtual bool Map::Load(char* _filename)
{
	int f;

	f = open(_filename, O_RDONLY);

	if (f < 0)
		return false;

	read(f, &width, sizeof(uint64_t));
	read(f, &height, sizeof(uint64_t));

	map = new double*[width];
	for (uint64_t i=0; i < width; i++)
	{
		map[i] = new double[height];
		read(f, map[i], sizeof(double)*height);
	}

	close(f);

	return true;

}
virtual bool Map::Save(const char* _filename)
{
	int f;

	f = open(_filename, O_WRONLY | O_CREAT, 0644);

	if (f < 0)
		return false;

	write(f, &width, sizeof(uint64_t));
	write(f, &height, sizeof(uint64_t));

	for (uint64_t i=0; i < width; i++)
		write(f, map[i], sizeof(double)*height);

	close(f);

	return true;
}
virtual bool Map::Save(char* _filename)
{
	int f;

	f = open(_filename, O_WRONLY | O_CREAT, 0644);

	if (f < 0)
		return false;

	write(f, &width, sizeof(uint64_t));
	write(f, &height, sizeof(uint64_t));

	for (uint64_t i=0; i < width; i++)
		write(f, map[i], sizeof(double)*height);

	close(f);

	return true;
}
virtual inline void Map::Add(uint64_t _x, uint64_t _y, double v)
{
	map[i][j] += v;
}
virtual bool Map::GenerateMap(NoiseMap nm)
{
	if (nm.width != width || nm.height != height)
		return false;

	return CreateMap(&nm.map);
}
virtual bool Map::GenerateMap()
{
	NoiseMap nm(width, height, log2(width) / 2, 1, 1);
	
	nm.GenerateMap();

	CreateMap(&nm.map);

	return true;
}
virtual bool Map::GenerateMap(double** _map)
{
	if (_map == nullptr)
		return false;

	return CreateMap(_map);
}
virtual bool Map::InitMap();
virtual bool Map::CreateMap(double** _map);
virtual void Map::Render();
virtual inline void Map::SetGrowthRate(double v);
virtual inline void Map::SetMinLandHeight(double v);

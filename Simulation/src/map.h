#ifndef __MAP_H
#define __MAP_H

#include "allinc.h"

#define MIN_LAND_HEIGHT 0.5

class Map
{
	public:
		Map(uint64_t _width, uint64_t _height);
		~Map();
		virtual void GrowAll(double v);
		virtual bool Load(const char* _filename);
		virtual bool Load(char* _filename);
		virtual bool Save(const char* _filename);
		virtual bool Save(char* _filename);
		virtual inline void Add(uint64_t _x, uint64_t _y, double v);
		virtual bool GenerateMap(NoiseMap nm);
		virtual bool GenerateMap();
		virtual bool GenerateMap(double** _map);
		virtual bool InitMap();
		virtual bool CreateMap(double** _map);
		virtual void Render();
		virtual inline void SetGrowthRate(double v);
		virtual inline void SetMinLandHeight(double v);

		uint64_t width, height;
		double** map;
		double growth_rate, min_land_height;
};

#endif //__MAP_H

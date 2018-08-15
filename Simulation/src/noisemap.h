
#pragma once

#include "allinc.h"

class NoiseMap
{
    public:
        NoiseMap(uint64_t _width, uint64_t _height, uint64_t _octaves, uint64_t _start_freq_x, uint64_t _start_freq_y);
        bool Load(const char* _filename);
        bool Save(const char* _filename);
        void GenerateMap();
        double Interpolate(double a, double b, double p);

        uint64_t width, height, octaves, start_freq_x, start_freq_y;
        double** map;
        double*** maps;
        double*** points;
};

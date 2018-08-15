
#pragma once

#include "allinc.h"

class NoiseMap
{
    public:
        NoiseMap(uint64_t _width, uint64_t _height, uint64_t _octaves, uint64_t _start_freq_x, uint64_t _start_freq_y);
        void GenerateMap();
        double Interpolate(double a, double b, double p);

        bool Load(char* _filename, uint64_t* _width, uint64_t* _height);
        bool Save(char* _filename);
        bool Load(const char* _filename, uint64_t* _width, uint64_t* _height);
        bool Save(const char* _filename);
        NoiseMap(const char*  _filename);

        uint64_t width, height, octaves, start_freq_x, start_freq_y;
        double** map;
        double*** maps;
        double*** points;
};

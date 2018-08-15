
#include "allinc.h"

NoiseMap::NoiseMap(uint64_t _width, uint64_t _height, uint64_t _octaves, uint64_t _start_freq_x, uint64_t _start_freq_y)
{
    width = _width;
    height = _height;
    octaves = _octaves;
    start_freq_x = _start_freq_x;
    start_freq_y = _start_freq_y;
}
NoiseMap::NoiseMap(const char* _filename)
{
    Load(_filename, &width, &height);
}

void NoiseMap::GenerateMap()
{
    uint64_t current_freq_x, current_freq_y, current_array_width, current_array_height, a, w0, w1, arr_x, arr_y;
    double min, max;

    if (width <= 0 || height <= 0 || octaves <= 0)
        return;

    current_freq_x = start_freq_x;
    current_freq_y = start_freq_y;
    a = 16;
    max = 0.0;
    min = +INFINITY;

    map = new double*[width];
    for (uint64_t i=0; i < width; i++)
        map[i] = new double[height];

    for (uint64_t i=0; i < width; i++)
    {
        for (uint64_t j=0; j < height; j++)
            map[i][j] = 0.0;
    }

    maps = new double**[octaves];

    for (uint64_t current_octave=0; current_octave < octaves; current_octave++)
    {
        /* generate points in maps */
        current_array_width = width / current_freq_x + 2;
        current_array_height = height / current_freq_y + 2;

        //printf("current_array_width = %zu, current_array_height = %zu\n", current_array_width, current_array_height);
        //printf("current_freq_x = %zu, current_freq_y = %zu\n", current_freq_x, current_freq_y);

        maps[current_octave] = new double*[current_array_width];
        for (uint64_t i=0; i < current_array_width; i++)
            maps[current_octave][i] = new double[current_array_height];

        /* generate points to interpolate */
        for (uint64_t i=0; i < current_array_width; i++)
        {
            for (uint64_t j=0; j < current_array_height; j++)
                maps[current_octave][i][j] = rand() % a;
        }

        for (uint64_t i=0; i < width; i++)
        {
            for (uint64_t j=0; j < height; j++)
            {
                arr_x = i / current_freq_x;
                arr_y = j / current_freq_y;

                w0 = Interpolate(maps[current_octave][arr_x][arr_y], maps[current_octave][arr_x + 1][arr_y], (double)(i - arr_x*current_freq_x) / (double)current_freq_x);
                w1 = Interpolate(maps[current_octave][arr_x][arr_y + 1], maps[current_octave][arr_x + 1][arr_y + 1], (double)(i - arr_x*current_freq_x) / (double)current_freq_x);
                map[i][j] += Interpolate(w0, w1, (double)(j - arr_y*current_freq_y) / (double)current_freq_y);
            }
        }

        current_freq_x *= 2;
        current_freq_y *= 2;
        a *= 2;
    }

    for (uint64_t i=0; i < width; i++)
    {
        for (uint64_t j=0; j < height; j++)
        {
            if (map[i][j] < min)
                min = map[i][j];
            if (map[i][j] > max)
                max = map[i][j];
        }
    }
    max -= min;
    for (uint64_t i=0; i < width; i++)
    {
        for (uint64_t j=0; j < height; j++)
        {
            map[i][j] = (map[i][j] - min) / max;
            //printf("map[%zu][%zu] = %.25f\n", i, j, map[i][j]);
        }
    }
}

double NoiseMap::Interpolate(double a, double b, double p)
{
    //return p*a + (1.0 - p) * b;
    return a + p*(b - a);
}

bool NoiseMap::Save(char* _filename)
{
    printf("Saving file as %s\n", _filename);
    int f;

    f = open(_filename, O_WRONLY | O_CREAT, 0644);

    if (f < 0)
        return false;

    write(f, &width, sizeof(uint64_t));
    write(f, &height, sizeof(uint64_t));

    for (uint64_t i=0; i < width; i++)
        write(f, map[i], sizeof(double) * height);

    close(f);

    return true;
}
bool NoiseMap::Save(const char* _filename)
{
    printf("Saving file as %s\n", _filename);
    int f;

    f = open(_filename, O_WRONLY | O_CREAT, 0644);

    if (f < 0)
    {
        printf("could not open file\n");
        return false;
    }

    write(f, &width, sizeof(uint64_t));
    write(f, &height, sizeof(uint64_t));

    for (uint64_t i=0; i < width; i++)
        write(f, map[i], sizeof(double) * height);

    close(f);

    return true;
}

bool NoiseMap::Load(char* _filename, uint64_t* _width, uint64_t* _height)
{
    int f;

    f = open(_filename, O_RDONLY);

    if (f < 0)
        return false;

    read(f, &width, sizeof(uint64_t) * 1);
    read(f, &height, sizeof(uint64_t) * 1);

    *_width = width;
    *_height = height;

    if (*_width != width || *_height != height)
        return false;

    map = new double*[width];
    for (uint64_t i=0; i < width; i++)
        map[i] = new double[height];

    for (uint64_t i=0; i < width; i++)
        read(f, map[i], sizeof(double) * height);

    close(f);

    printf("going out of function\n");

    return true;
}
bool NoiseMap::Load(const char* _filename, uint64_t* _width, uint64_t* _height)
{
    int f;

    f = open(_filename, O_RDONLY);

    if (f < 0)
        return false;

    read(f, &width, sizeof(uint64_t) * 1);
    read(f, &height, sizeof(uint64_t) * 1);

    *_width = width;
    *_height = height;

    if (*_width != width || *_height != height)
        return false;

    map = new double*[width];
    for (uint64_t i=0; i < width; i++)
        map[i] = new double[height];

    for (uint64_t i=0; i < width; i++)
        read(f, map[i], sizeof(double) * height);

    close(f);

    return true;
}

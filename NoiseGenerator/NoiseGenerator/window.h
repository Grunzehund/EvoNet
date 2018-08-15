
#pragma once

#include "allinc.h"

class Window : public QWidget
{
    public:
     Window(uint64_t _x_expansion, uint64_t _y_expansion, uint64_t _tilesize, uint64_t _octaves);
     void Init();
     void InitMap();
     void InitMapRecursive();
     void r(int64_t x, int64_t y, double prob_land);

     virtual void paintEvent(QPaintEvent* e);
     virtual void keyPressEvent(QKeyEvent* e);
     virtual void keyReleaseEvent(QKeyEvent* e);
     virtual void timerEvent(QTimerEvent* e);

     uint64_t x_expansion;
     uint64_t y_expansion;
     uint64_t tilesize;
     uint64_t octaves;
     uint64_t offset_x, offset_y;
     uint64_t keys;

     QBasicTimer timer;

     double** map;
     NoiseMap* nm;
};

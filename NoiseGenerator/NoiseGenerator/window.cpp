#include "allinc.h"

#define MIN_LAND_HEIGHT 0.5
#define SPEED 5


inline double GetRandVal(double amp)         { return ((rand()%200 - 100) / 200.0) * amp; }
Window::Window(uint64_t _x_expansion, uint64_t _y_expansion, uint64_t _tilesize, uint64_t _octaves)
{
    showFullScreen();
    x_expansion = _x_expansion;
    y_expansion = _y_expansion;
    tilesize = _tilesize;
    octaves = _octaves;
    map = nullptr;

    srand(time(0));

    timer.start(20, this);

    Init();
}
void Window::Init()
{
    nm = new NoiseMap(x_expansion, y_expansion, octaves, 1, 1);
    nm->GenerateMap();
    nm->Save("testsave.noise");
    //nm = new NoiseMap("testsave.noise");

    map = new double*[x_expansion];
    for (uint64_t i=0; i < x_expansion; i++)
        map[i] = new double[y_expansion];

    for (uint64_t i=0; i < x_expansion; i++)
    {
        for (uint64_t j=0; j < y_expansion; j++)
        {
            if (nm->map[i][j] >= MIN_LAND_HEIGHT)
                map[i][j]= (nm->map[i][j] - 0.5) * 2.0;
            else
                map[i][j] = -1.0;
        }
    }

    delete nm;

}
void Window::paintEvent(QPaintEvent* e)
{
    QGraphicsScene scene(this);
    QGraphicsView view;
    QPainter painter(this);
    QColor color;
    QRect rect;

    Q_UNUSED(e);

    for (int i=0; i < (int)x_expansion; i++)
    {
        for (int j=0; j < (int)y_expansion; j++)
        {
            if (map[i][j] < 0.0)
                color.setRgb(0, 0, 255);
            else
                color.setRgb(((map[i][j] / 2.0) + 0.5) * 255, ((map[i][j] / 2.0) + 0.5) * 255, 0);
            painter.fillRect(i*tilesize + offset_x, j*tilesize + offset_y, tilesize, tilesize, color);
        }
    }
}
void Window::keyPressEvent(QKeyEvent* e)
{
    //printf("keyPressed\n");
    switch (e->key())
    {
        case Qt::Key_Left:
                keys |= KEY_LEFT;
                break;
        case Qt::Key_Right:
                keys |= KEY_RIGHT;
                break;
        case Qt::Key_Up:
                keys |= KEY_UP;
                break;
        case Qt::Key_Down:
                keys |= KEY_DOWN;
                break;
        default:
                QWidget::keyPressEvent(e);
    }
}
void Window::keyReleaseEvent(QKeyEvent* e)
{
    switch (e->key())
    {
        case Qt::Key_Left:
                keys &= ~KEY_LEFT;
                break;
        case Qt::Key_Right:
                keys &= ~KEY_RIGHT;
                break;
        case Qt::Key_Up:
                keys &= ~KEY_UP;
                break;
        case Qt::Key_Down:
                keys &= ~KEY_DOWN;
                break;
        default:
                QWidget::keyPressEvent(e);
    }
}
void Window::timerEvent(QTimerEvent* e)
{
    if (e->timerId() == timer.timerId())
    {
        //printf("timer\n");
        if (keys & KEY_LEFT)
            offset_x -= SPEED;
        if (keys & KEY_RIGHT)
            offset_x += SPEED;
        if (keys & KEY_UP)
            offset_y -= SPEED;
        if (keys & KEY_DOWN)
            offset_y += SPEED;

        if (keys != 0)
            repaint();
    }
}










































































































//void Window::r(int64_t x, int64_t y, double prob_land)
//{
//    double a = 0.001;
//
//    if (x < 0 || y < 0 || x >= x_expansion || y > y_expansion)                       /* if out of range, stop */
//        return;
//
//    printf("x = %zu, y = %zu\n", x, y);
//
//    if (map[x][y] != 0.0)
//        return;
//
//    if ((double)(rand()%100000) / 100000.0 > prob_land)
//        map[x][y] = 1.0;
//    else
//        map[x][y] = -1.0;
//
//    if (prob_land >= 1.0 || prob_land <= 0.0)
//    {
//        if (prob_land >= 0.5)
//                a = -0.001;
//        else
//                a = +0.001;
//    }
//
//    map[x][y] = 1.0;
//
//    /* the four other directions */
//    r((int64_t)x - 1, (int64_t)y, prob_land += a);
//    r((int64_t)x + 1, (int64_t)y, prob_land += a);
//    r((int64_t)x, (int64_t)y - 1, prob_land += a);
//    r((int64_t)x, (int64_t)y + 1, prob_land += a);
//}
//
//void Window::InitMapRecursive()
//{
//    map = new double*[x_expansion];
//    for (uint64_t i=0; i < x_expansion; i++)
//        map[i] = new double[y_expansion];
//
//    for (uint64_t i=0; i < x_expansion; i++)
//    {
//        for (uint64_t j=0; j < y_expansion; j++)
//            map[i][j] = 0.0;
//    }
//
//    r(51, 51, 0.5);
//}
//
//void Window::InitMap()
//{
//    //double var, diff_x, diff_y;
//    double*** maps;
//    uint64_t current_field_size;
//
//    map = new double*[x_expansion];
//    for (uint64_t i=0; i < x_expansion; i++)
//        map[i] = new double[y_expansion];
//
//    maps = new double**[9];
//    for (uint64_t i=0; i < 9; i++)                                             //TODO: improve with log2
//    {
//        maps[i] = new double*[x_expansion];
//        for (uint64_t j=0; j < x_expansion; j++)
//            maps[i][j] = new double[y_expansion];
//    }
//
//    for (uint64_t i=0; i < x_expansion; i++)
//    {
//        for (uint64_t j=0; j < y_expansion; j++)
//            map[i][j] = 0.0;
//    }
//    for (uint64_t i=TMP; i <= TMP2; i++)
//    {
//        current_field_size = (uint64_t)pow(2.0, i);
//        for (uint64_t j=1; j <= x_expansion; j += current_field_size)
//        {
//            for (uint64_t k=1; k <= y_expansion; k += current_field_size)
//            {
//                maps[i - 1][j - 1][k - 1] = GetRandVal(1.0 / pow(2, i));
//            }
//        }
//        if (current_field_size == 1)
//            continue;
//        for (uint64_t j=1; j <= x_expansion; j += current_field_size)
//        {
//            for (uint64_t k=1; k <= y_expansion; k += current_field_size)
//            {
//                for (uint64_t l=0; l < current_field_size - 1; l++)
//                {
//                    for (uint64_t m=0; m < current_field_size - 1; m++)
//                    {
//                        maps[i - 1][j - 1 + l][k - 1 + m] = (
//                                    (maps[i - 1][j - 1][k - 1] * (double)(l*m)) +
//                                    (maps[i - 1][j - 1 + current_field_size-1][k - 1] * (double)((current_field_size - l)*m)) +
//                                    (maps[i - 1][j - 1 + current_field_size-1][k - 1 + current_field_size] * (double)((current_field_size - l)*(current_field_size - m))) +
//                                    (maps[i - 1][j - 1][k - 1 + current_field_size-1] * (double)(l*(current_field_size - m)))
//                                    ) / (double)(current_field_size*current_field_size);                                                                                       //TODO: Interpolate 2D (areas)
//                        printf("i = %zu, j = %zu, k = %zu, l = %zu, m = %zu, current_field_size = %zu\n", i, j, k, l, m, current_field_size);
//                    }
//                }
//            }
//        }
//    }
//    for (uint64_t i=TMP; i <= TMP2; i++)
//    {
//        for (uint64_t j=0; j < x_expansion; j++)
//        {
//            for (uint64_t k=0; k < y_expansion; k++)
//                map[j][k] += maps[i - 1][j][k];
//        }
//    }
//    for (uint64_t i=0; i < x_expansion; i++)
//    {
//        for (uint64_t j=0; j < y_expansion; j++)
//        {
//            if (map[i][j] > 1.0)
//                map[i][j] = 1.0;
//            //map[i][j] = (rand()%200 - 100) / 100.0;
//        }
//    }
//    /* Corrections */
//    for (uint64_t i=0; i < x_expansion; i++)
//    {
//        for (uint64_t j=0; j < y_expansion; j++)
//        {
//            //if ((map[i][j - 1 < 0 ? j + 1 : j] == map[][]) && () && ())
//            //    map[i][j] *= -1.0;
//        }
//    }
//}
///*
//for (uint64_t tmp=64; tmp != 1; tmp /= 2)
//{
//    for (uint64_t i=0; i < x_expansion; i += tmp)
//    {
//         for (uint64_t j=0; j < y_expansion; j += tmp)
//         {
//             //var = ((rand()%100 - 25) / 1000.0) * tmp * (100.0 / 64.0);
//             var = ((rand()%200 - 100) / 2000.0) * tmp * (100.0 / 64.0);
//             if (var > 1.0)
//                 var = 1.0;
//             //printf("var = %.5f\n", var);
//             for (uint64_t k=0; k < tmp; k++)
//             {
//                 for (uint64_t l=0; l < tmp; l++)
//                {
//                     map[i + k][j + l] += var;
//                }
//             }
//        }
//    }
//}
//*/
///*
//    for (uint64_t tmp=512; tmp >= 512; tmp /= 2)
//    {
//        printf("tmp = %zu\n", tmp);
//        for (uint64_t i=0; i < x_expansion; i += tmp)
//        {
//             for (uint64_t j=0; j < y_expansion; j += tmp)
//             {
//                 map[i][j]                     += GetRandVal(tmp);
//                 map[i + tmp - 1][j]           += GetRandVal(tmp);
//                 map[i][j + tmp - 1]           += GetRandVal(tmp);
//                 map[i + tmp - 1][j + tmp - 1] += GetRandVal(tmp);
//
//                 for (uint64_t k=0; k < 2; k++)
//                 {
//                     diff_x = (map[i + tmp - 1][j + k*(tmp - 1)] - map[i][j + k*(tmp - 1)]) / tmp;
//                     //printf("diff_x = %.25f\n", diff_x);
//                     for (uint64_t l=1; l < tmp - 1; l++)
//                     {
//                         map[i + l][j + k*tmp] = map[i][j] + l*diff_x;
//                     }
//                 }
//                 for (uint64_t k=0; k < tmp; k++)
//                 {
//                     diff_y = (map[i + k][j + tmp - 1] - map[i + k][j]) / tmp;
//                     //printf("diff_y = %.25f\n", diff_y);
//                     for (uint64_t l=1; l < tmp - 1; l++)
//                         map[i + k][j + l] = map[i + k][j] + l*diff_y;
//                 }
//             }
//        }
//    }
//*/
//

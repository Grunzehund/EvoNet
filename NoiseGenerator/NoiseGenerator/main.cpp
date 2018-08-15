
#include "allinc.h"

#define MAP_SIZE 1024
#define TILE_SIZE 1
#define OCTAVES 5

QApplication* app;
Window* window;
NoiseMap* nm;

int main(int argc, char** argv)
{
    printf("program started\n");

    app = new QApplication(argc, argv);

    window = new Window(MAP_SIZE, MAP_SIZE, TILE_SIZE, OCTAVES);
    window->show();

    //QRect rect;

    //QGraphicsScene scene;
    //scene.addText("Hallo Welt!");

    //for (uint64_t i=0; i < 10; i++)
    //{
    //    rect.setRect(0, 2*i*10, 10, 10);
    //    scene.addRect(rect);
    //}

    //QGraphicsView view;
    //view.setScene(&scene);
    //view.show();

    return app->exec();
}

#include <iostream>
#include "../../vsgl2.h"
#include <ctime>
#include <stdlib.h>

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;

int main(int argc, char* argv[])
{
    init();
    srand(time(NULL));
    set_window(1240,720,"Hangman");
    while(!done())
    {
        set_background_color(Color(25,25,25,255));

        //number = to_string(points);
        //draw_text("Roboto-Thin.ttf",40,"Score",WIDTH/2-40,-5,Color(255,255,255,255));

        update();

        delay(5);
    }
    close();
    return 0;
}

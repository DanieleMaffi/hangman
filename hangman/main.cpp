#include <iostream>
#include "../../vsgl2.h"
#include <ctime>
#include <stdlib.h>
#include <string>

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;

const int HEIGHT = 720;
const int WIDTH = 1240;
const int DIM = 100;

class Underscore
{
    public:
        int margin;
        int width;
        int height;
        int x[DIM] = {0};
        int y = HEIGHT - 100;

        bool does_fit(int margin, int width, string s)
        {
            int len = s.size();
            int space = width*len + margin*len;
            if (space <= WIDTH)
                return true;
            else
                return false;
        }

        void init_position(string s)
        {
            int len = s.size();
            for (int i = 1; i < len; i++)
            {
                x[i] += x[i - 1] + margin + width;
            }
        }

        void draw(string s)
        {
            int len = s.size();
            for (int i = 0; i < len; i++)
            {
                draw_filled_rect(x[i], y, width, height, Color(255,255,255,255));
            }
        }
};

void init_guessed_word(string &guessed_word)
{
    for (int i = 0; i < guessed_word.size(); i++)
    {
        if (i != 0)
            guessed_word.at(i) = '_';
    }
}

void clean_string(string &s)
{
    int len = s.size();
    for (int i = 0; i < len; i++)
    {
        if (s.at(i) > 'z' || s.at(i) < 'a')
        {
            if (s.at(i) <= 'Z' && s.at(i) >= 'A')
                s.at(i) += 32;
            else
            {
                for (int j = i; j < len - 1; j++)
                    s.at(j) = s.at(j + 1);
                s.at(--len) = '\0';
                i--;
            }
        }
    }
}

int main(int argc, char* argv[])
{
    string secret_word = "paRola";
    string guessed_word;

    //adatta la dimensione di '_' per rientrare completamente nello schermo
    Underscore underscore;
    underscore.width = WIDTH/10;
    underscore.margin = underscore.width/2;
    underscore.height = 10;
    while(!underscore.does_fit(underscore.margin, underscore.width, secret_word))
    {
        underscore.width -= 50;
    }
    underscore.margin = underscore.width/2;

    init();
    srand(time(NULL));
    set_window(WIDTH,HEIGHT,"Hangman");

    clean_string(secret_word);
    guessed_word = secret_word;
    init_guessed_word(guessed_word);
    underscore.init_position(secret_word);

    cout << secret_word << endl;
    cout << guessed_word << endl;

    while(!done())
    {
        set_background_color(Color(25,25,25,255));
        underscore.draw(secret_word);

        update();
        //delay(5);
    }
    close();
    return 0;
}

//number = to_string(points);
//draw_text("Roboto-Thin.ttf",40,"Score",WIDTH/2-40,-5,Color(255,255,255,255));

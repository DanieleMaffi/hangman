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

const int HEIGHT = 1080;
const int WIDTH = 1920;
const int DIM = 100;

bool is_there(int v[], int n, char value)
{
    bool is_there = false;
    for (int i = 0; i < n; i++)
    {
        if (v[i] == value)
            is_there = true;
    }
    return is_there;
}

void update_attempts(int v[], int n, char value)
{
    if (!is_there(v, n, value))
    {
        for (int i = 0; i < n; i++)
        {
            if (v[i] == -1)
            {
                v[i] = value;
                return;
            }
        }
    }
}

void init_array(int v[], int n)
{
    for (int i = 0; i < n; i++)
        v[i] = -1;
}

bool is_guessed(char guess, string guessed_word, string secret_word)
{
    bool is_guessed = false;
    for (int i = 0; secret_word[i] != '\0'; i++)
    {
        if (secret_word.at(i) == guess)
            is_guessed = true;
    }
    return is_guessed;
}

void update_word(char guess, string &guessed_word, string secret_word, int attempts[], int n)
{
    for (int i = 0; secret_word[i] != '\0'; i++)
    {
        if (secret_word.at(i) == guess)
            guessed_word.at(i) = guess;
        //cout << guessed_word << endl;
        update_attempts(attempts, n, guess);
    }
}

int my_stringlength(string s)
{
    int conta = 0;
    for (int i = 0; s[i] != '\0'; i++)
        conta++;
    return conta;
}

void init_guessed_word(string &guessed_word, string secret_word)
{
    for (int i = 0; i < secret_word[i] != '\0'; i++)
    {
        if (i != 0)
            guessed_word.at(i) = '_';
    }
}

void clean_string(string &s)
{
    int len = my_stringlength(s);
    int j, i;
    for (i = 0; s[i] != '\0'; i++)
    {
        if (s.at(i) > 'z' || s.at(i) < 'a')
        {
            if (s.at(i) <= 'Z' && s.at(i) >= 'A')
                s.at(i) += 32; //conversione a minuscole
            else
            {
                for (j = i; j < len - 1; j++)
                    s.at(j) = s.at(j + 1);
                s.at(j) = '\0';
                i = 0;
            }
        }
    }
}

class Underscore
{
    public:
        int margin;
        int width;
        int height;
        int x[DIM] = {0};
        int y = HEIGHT - 50;

        bool does_fit(int margin, int width, string s)
        {
            int len = my_stringlength(s);
            int space = width*len + margin*len;
            if (space <= WIDTH)
                return true;
            else
                return false;
        }

        void init_position(string s)
        {
            for (int i = 1; s[i] != '\0'; i++)
            {
                x[i] += x[i - 1] + margin + width;
            }
        }

        void draw(string s)
        {
            for (int i = 0; s[i] != '\0'; i++)
            {
                draw_filled_rect(x[i], y, width, height, Color(255,255,255,255));
            }
        }
};

int main(int argc, char* argv[])
{
    string secret_word = "pal   6! La";
    string guessed_word;

    int errors = 0;
    int max_errors = 10;
    int attempts[DIM];

    bool easy_mode = true;
    bool multiplayer = true;

    if (multiplayer)
        cin >> secret_word;
    //else
        //secret_word = ....

    //adatta la dimensione di '_' per rientrare completamente nello schermo
    Underscore underscore;
    underscore.width = WIDTH/8;
    underscore.margin = underscore.width/2;
    underscore.height = 5;
    while(!underscore.does_fit(underscore.margin, underscore.width, secret_word))
    {
        underscore.width--;
        underscore.margin = underscore.width/2;
    }

    init();
    srand(time(NULL));
    set_window(WIDTH,HEIGHT,"Hangman");
    init_array(attempts, DIM);

    clean_string(secret_word);
    guessed_word = secret_word;
    init_guessed_word(guessed_word, secret_word);
    underscore.init_position(secret_word);

    cout << secret_word << endl;
    cout << guessed_word << endl;

    while(!done() && errors < max_errors)
    {
        set_background_color(Color(25,25,25,255));
        underscore.draw(secret_word);

        //update_word('g',guessed_word, secret_word, attempts, DIM);
        update_word('l', guessed_word, secret_word, attempts, DIM);
        //if(is_guessed('l', guessed_word, secret_word))

        update();
        //delay(5);
    }
    close();
    return 0;
}

//number = to_string(points);
//draw_text("Roboto-Thin.ttf",40,"Score",WIDTH/2-40,-5,Color(255,255,255,255));

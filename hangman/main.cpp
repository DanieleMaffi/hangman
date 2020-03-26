#include <iostream>
#include "../../vsgl2.h"
#include <ctime>
#include <stdlib.h>
#include <string>
#include <fstream>

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;

const int HEIGHT = 720;
const int WIDTH = 1240;
const int DIM = 100;

bool easy_mode = true;      ///nella mod facile i tentativi vengono salvati in una stringa, cosi che il giocatore conosca le lettere gia inserite
bool multiplayer = true;    ///se vera viene attivata la mod a due giocatori

string parole[120000];
string images[] = {"images/1.png", "images/2.png", "images/3.png", "images/4.png", "images/5.png", "images/6.png", "images/7.png", "images/8.png", "images/9.png", "images/10.png", "images/11.png"};


///disegna i bottoni e li aggiorna se vengono premuti con il cursore
bool draw_buttons(int position_y, int position_x, bool &variable, int button_dimension)
{
    int mouse_x = get_mouse_x();
    int mouse_y = get_mouse_y();

    if (mouse_left_button_pressed() && mouse_x >= position_x && mouse_x <= position_x + 120 && mouse_y >= position_y && mouse_y <= position_y + 120)
        variable = true;

    if (mouse_left_button_pressed() && mouse_x >= position_x + 200 && mouse_x <= position_x + 320 && mouse_y >= position_y && mouse_y <= position_y + 120)
        variable = false;

    if (variable)
    {
        draw_image("images/si-btn-selected.png", position_x, position_y, button_dimension, button_dimension);
        draw_image("images/no-btn.png", position_x + 200, position_y, button_dimension, button_dimension);
    }
    else
    {
        draw_image("images/no-btn-selected.png", position_x + 200, position_y, button_dimension, button_dimension);
        draw_image("images/si-btn.png", position_x, position_y, button_dimension, button_dimension);
    }
}

///disegna a schermo i vari tentativi
void draw_attempts(string guesses)
{
    int margin_lr = 390;
    int margin_top = 40;
    for(int i = 0; i < guesses.size(); i++)
    {
        if (i % 8 == 0)
        {
            margin_top += 50;
            margin_lr = 390;
        }
        string letter(1, guesses.at(i));
        draw_text("Montserrat-Regular.otf", 40, letter, WIDTH - margin_lr, margin_top, Color(255,255,255,255));
        margin_lr -= 40;
    }
}

///disegna a schermo l'immagine dell' impiccato
void draw_hangman(int errors)
{
        draw_image(images[errors - 1], 10, 10, 345, 312);
}

bool parola_corretta(const string &s)
{
    for (int i = 0; i < s.size(); i++)
        if (s.at(i) < 'a' || s.at(i) > 'z')
            return false;
    return true;
}

int carica_parole(string nomefile, string parole[])
{
    ifstream in(nomefile);
    int num_parole = 0;
    string temp;
    while (in >> temp)
    {
        if (parola_corretta(temp))
        {
            parole[num_parole] = temp;
            num_parole++;
        }
    }
    return num_parole;
}

///controlla nella stringa, che contiene i tentativi, se un char si trova in esso
bool is_there(/*int v[], int n,*/ string guesses, char letter)
{
    /*bool is_there = false;
    for (int i = 0; i < n; i++)
    {
        if (v[i] == value)
            is_there = true;
    }
    return is_there;*/
    for (int i = 0; i < guesses.size(); i++)
    {
        if (guesses.at(i) == letter)
            return true;
    }
    return false;
}

///aggiorna la stringa che contiene i tentativi
void update_attempts(/*int v[], int n,*/ string &guesses , char letter)
{int margin_lr = 390;
    int margin_top = 40;
    for(int i = 0; i < guesses.size(); i++)
    {
        if (i % 8 == 0)
            margin_top += 50;
        string letter(1, guesses.at(i));
        draw_text("Montserrat-Regular.otf", 40, letter, WIDTH - margin_lr, margin_top, Color(255,255,255,255));
        margin_lr -= 40;
    }
    /*if (!is_there(v, n, value))
    {
        for (int i = 0; i < n; i++)
        {
            if (v[i] == -1)
            {
                v[i] = value;
                return;
            }
        }
    }*/
    if (!is_there(guesses, letter))
        guesses += letter;
}

/*void init_array(int v[], int n)
{
    for (int i = 0; i < n; i++)
        v[i] = -1;
}*/

/*bool is_guessed(char guess, string guessed_word, string secret_word)
{
    bool is_guessed = false;
    for (int i = 0; secret_word[i] != '\0'; i++)
    {
        if (secret_word.at(i) == guess)
            is_guessed = true;
    }
    return is_guessed;
}*/

///aggiorna la parola da indovinare
void update_word(char guess, string &guessed_word, string secret_word, /*int attempts[], int n,*/ string &guesses, int &errors)
{
    int guessed_characters = 0;
    for (int i = 0; secret_word[i] != '\0'; i++)
    {
        if (secret_word.at(i) == guess)
        {
            guessed_word.at(i) = guess;
            guessed_characters++;
        }
    }

    if (easy_mode)
    {
        if (guessed_characters == 0 && !is_there(/*attempts, DIM,*/ guesses, guess))
            errors++;
    }
    else if (guessed_characters == 0)
        errors++;

    update_attempts(/*attempts, n, guess*/ guesses, guess);
}

int my_stringlength(string s)
{
    int conta = 0;
    for (int i = 0; s[i] != '\0'; i++)
        conta++;
    return conta;
}

///inizializza la stringa che poi verrà stampata a schermo
void init_guessed_word(string &guessed_word, string secret_word)
{
    for (int i = 0; i < secret_word[i] != '\0'; i++)
    {
        if (i != 0)
            guessed_word.at(i) = '_';
    }
}

///pulisce la stringa da eventuali spazi, maiuscole, simboli...
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

        void draw(string s, string guessed_word, int underscore_width)
        {
            for (int i = 0; s[i] != '\0'; i++)
            {
                if (guessed_word.at(i) == '_')
                    draw_filled_rect(x[i], y, width, height, Color(255,255,255,255));
                else
                {
                    string letter(1, guessed_word.at(i));
                    draw_text("Montserrat-Regular.otf",underscore_width,letter,x[i] + underscore_width/2 - 20,y - underscore_width,Color(255,255,255,255));
                }
            }
        }
};

int main(int argc, char* argv[])
{
    string secret_word;
    string guessed_word;
    string guesses;

    int phases = -1; //'-1' selezione mod, '0' corrisponde alla fase di inserimento se la modalità multiplayer è vera,  '1' corrisponde alla fase di gioco, mentre '2' alla fase finale
    int errors = 0;
    int max_errors = 11;
    //int attempts[DIM];

    bool play_again = true;

    Underscore underscore;
    underscore.width = WIDTH/8;
    underscore.margin = underscore.width/2;
    underscore.height = 5;

    srand(time(NULL));

    init();
    set_window(WIDTH,HEIGHT,"Hangman");
    //init_array(attempts, DIM);

    while(!done() && play_again)
    {
        set_background_color(Color(25,25,25,255));

        if (phases == -1)
        {
            draw_text("Montserrat-Regular.otf", 100,"SELEZIONA MODALITÀ", 0, 0, Color(255,255,255,255));
            draw_text("Montserrat-Regular.otf", 80,"Modalità facile", 0, 300, Color(255,255,255,255));
            draw_text("Montserrat-Regular.otf", 80,"Multiplayer", 0, 500, Color(255,255,255,255));
            draw_text("Montserrat-Regular.otf", 50,"premi spazio per continuare", 0, 100, Color(255,255,255,255));

            draw_buttons(300, 700, easy_mode, 120);
            draw_buttons(500, 700, multiplayer, 120);

            if (is_pressed(VSGL_SPACE))
                phases = 0;
        }

        if (phases == 0) ///prima fase di inserimento della parola segreta in modalita multigiocatore
        {
            draw_filled_rect(0,0,WIDTH,HEIGHT,Color(25,25,25,255));
            if (multiplayer)
            {
                draw_text("Montserrat-Regular.otf", 100, "Inserisci parola", 0, 0, Color(255,255,255,255));
                update();
                secret_word = read_text("Montserrat-Regular.otf", 200, 20, HEIGHT/2 - 200, Color(255,255,255,255), 50);
                clean_string(secret_word);
            }
            else
            {
                int num_parole = carica_parole("italian", parole);
                secret_word = parole[(rand()*0xFFFF + rand()) % num_parole];
            }

            ///adatta la dimensione di '_' per rientrare completamente nello schermo
            while(!underscore.does_fit(underscore.margin, underscore.width, secret_word))
            {
                underscore.width--;
                underscore.margin = underscore.width/2;
            }
            guessed_word = secret_word;
            init_guessed_word(guessed_word, secret_word);
            underscore.init_position(secret_word);
            phases++;
        }
        else if (!multiplayer && phases == 0)
            phases = 1;

        if (phases == 1) ///gioco
        {
            draw_filled_rect(0,0,WIDTH,HEIGHT,Color(25,25,25,255));
            underscore.draw(secret_word, guessed_word, underscore.width);
            draw_hangman(errors);
            if (easy_mode)
            {
                draw_attempts(guesses);
                draw_image("images/cornice.png", WIDTH - 420, 20, 400, 500);
                draw_text("Montserrat-Regular.otf", 40, "Tentativi:", WIDTH - 310, 35, Color(255,255,255,255));
            }
            update();
            string guess = read_text("Montserrat-Regular.otf", 300, WIDTH/2 - 200, HEIGHT/2 - 200, Color(255,255,255,255), 1);
            update_word(guess[0],guessed_word, secret_word, /*attempts, DIM,*/ guesses, errors);
            //if(is_guessed('l', guessed_word, secret_word))
            if (guessed_word == secret_word || errors == max_errors)
                phases++;
        }

        if (phases == 2) ///fase finale in cui si chiede al giocatore se vuole giocare ancora
        {
            draw_filled_rect(0,0,WIDTH,HEIGHT,Color(25,25,25,255));
            if (guessed_word == secret_word)
                draw_text("Montserrat-Regular.otf", 100, "HAI VINTO!", 0, 0, Color(255,255,255,255));
            else
                draw_text("Montserrat-Regular.otf", 100, "HAI PERSO!", 0, 0, Color(255,255,255,255));
            draw_text("Montserrat-Regular.otf", 100, "Vuoi giocare ancora?", 0, 120, Color(255,255,255,255));
            draw_text("Montserrat-Regular.otf", 50,"premi spazio per continuare", 0, HEIGHT - 100, Color(255,255,255,255));
            draw_buttons(HEIGHT/2, WIDTH/2 - 200, play_again, 200);
            if (is_pressed(VSGL_SPACE))
            {
                if (play_again)
                {
                    secret_word.clear();
                    guessed_word.clear();
                    guesses.clear();
                    phases = -1;
                    errors = 0;
                    underscore.width = WIDTH/8;
                    underscore.margin = underscore.width/2;
                    for (int i = 0; i < DIM; i++)
                        underscore.x[i] = 0;
                }
            }
        }
        update();
    }
    close();
    return 0;
}

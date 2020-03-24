#include <iostream>
#include <cstring>

using namespace std;
const int DIM = 10;

bool is_there(string guesses, char lettera)
{
    bool trovato = false;
    for (int i  = 0; i < guesses.size(); i++)
        if (guesses.at(i) == lettera)
            trovato = true;
    return trovato;
}

int main()
{
    string s, p;
    char lettera;
    int n_lettere, errori = 0, i, modalita;
    cout << "scegliere modalita: facile(1) difficile(2)" << endl;
    cin >> modalita;
    cout << "inserisci parola da indovinare: " << endl;
    cin >> s;
    n_lettere = s.size();
    p = s;
    for (i = 0; i < n_lettere; i++)
    {
        p[i] = '_';
    }
    p[0] = s[0];
    cout << p << endl;
    string lettere_provate;
    while (errori < 10 && s != p)
    {
        cout << "inserisci una lettera: " << endl;
        cin >> lettera;
        if (!is_there(lettere_provate, lettera))
            lettere_provate += lettera;
        bool trovato = false;
        for (int j = 1; j < n_lettere; j++)
            if (lettera == s[j]){
                p[j] = s[j];
                trovato = true;
            }
        if (!trovato || is_there(lettere_provate, lettera))
        {
            errori++;
            cout << "hai sbagliato; riprova!" << endl;
        }
        else
            cout << "Lettera indovinata" << endl;
        if (modalita == 1 && s != p )
            cout << "Lettere gia provate: " << lettere_provate << endl;
        cout << p << endl;
        if (s == p)
                break;
        }
    if (errori == 10)
        cout << "HAI PERSO" << endl;
    else if (s == p)
        cout << "HAI VINTO" << endl;
    return 0;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <windows.h>
#include <conio.h>

using namespace std;

const int tapeLimit = 100; // Juostos pletimosi limitas
//simuliatorius interpretuoja b simboli kaip tarpa, nukaitymo funkcijoje ivyksta konvertavimas

struct rule
{
    string currentState;

    char currentSymbol1;
    char currentSymbol2;

    char newSymbol1;
    char newSymbol2;

    char direction1;
    char direction2;

    string newState;
};

void Read(int& n,int& headPos1, int& headPos2, vector<char>& tape1, vector<char>& tape2, map<string, rule>& rules, string document)
{
    ifstream doc;
    doc.open(document);

    doc >> n;

    //juostos nuskaitymas
    string juosta1;
    doc >> juosta1;
    for (int i = 0; i < juosta1.length(); i++)
    {
        tape1.push_back(juosta1[i]);
    }
    // Jei dvi juostos
    if (n == 2)
    {
        string juosta2;
        doc >> juosta2;
        for (int i = 0; i < juosta2.length(); i++)
        {
            tape2.push_back(juosta2[i]);
        }
    }
    int pos1;
    doc >> pos1;
    headPos1 = pos1 - 1;
    // Jei dvi juostos
    if (n == 2)
    {
        int pos2;
        doc >> pos2;
        headPos2 = pos2 - 1;
    }
    
    // tiurigo masinos kodo nuskaitymas
    if (n == 1)
    {
        string cSt, nSt;
        char cSm, nSm, d;
        while (doc >> cSt >> cSm >> nSm >> d >> nSt)
        {
            if (cSm == 'b')cSm = ' ';
            if (nSm == 'b')nSm = ' ';

            string k;
            k = cSt + '_' + cSm;
            rule r;
            r = { cSt,cSm,0,nSm,0,d,0,nSt };

            rules.insert({ k, r });
        }
    }
    // Jei dvi juostos
    if (n == 2)
    {
        string cSt, nSt;
        char cSm1, cSm2, nSm1, nSm2, d1, d2;
        while (doc >> cSt >> cSm1 >> cSm2 >> nSm1 >> nSm2 >> d1 >> d2 >> nSt)
        {
            if (cSm1 == 'b')cSm1 = ' '; if (cSm2 == 'b')cSm2 = ' ';
            if (nSm1 == 'b')nSm1 = ' '; if (nSm2 == 'b')nSm2 = ' ';

            string k;
            k = cSt + '_' + cSm1 + '_' + cSm2;
            rule r;
            r = { cSt,cSm1,cSm2,nSm1,nSm2,d1,d2,nSt };

            rules.insert({ k, r });
        }
    }
    
    doc.close();
}

void Show(int n, int headPos1, int headPos2, string state, char symbol1, char symbol2, vector<char>& tape1, vector<char>& tape2)
{
    cout << "\033[H";

    for (auto x : tape1)
    {
        cout << x;
    }
    if (symbol1 == ' ')
    {
        cout << " | state: " << state << "; symbol: b";
    }
    else
    {
        cout << " | state: " << state << "; symbol: " << symbol1;
    }
    cout << " " << endl;

    for (int i = 0; i < headPos1;i++)
    {
        cout << ' ';
    }
    cout << "^ " << endl;

    // Jei dvi juostos
    if (n == 2)
    {
        for (auto x : tape2)
        {
            cout << x;
        }
        if (symbol2 == ' ')
        {
            cout << " | state: " << state << "; symbol: b";
        }
        else
        {
            cout << " | state: " << state << "; symbol: " << symbol2;
        }
        cout << " " << endl;

        for (int i = 0; i < headPos2;i++)
        {
            cout << ' ';
        }
        cout << "^ " << endl;
    }

    cout << "To Pause/Resume a machine press P" << endl;
    cout << "To Stop a machine press E" << endl;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    map<string, rule> rules;
    vector<char> tape1;
    vector<char> tape2;
    int n; // juostu kiekis
    int headPos1=0, headPos2=0;

    string document = argv[1]; // irasykite nuskaitymo failo pavadinima

    Read(n, headPos1, headPos2, tape1, tape2, rules, document);

    // patikrinimai pries pradedant
    if (headPos1 < 0)
    {
        tape1.insert(tape1.begin(), ' ');
        headPos1 = 0;
    }
    if (tape1.empty())
    {
        tape1.push_back(' ');
    }
    if (n == 2)
    {
        if (headPos2 < 0)
        {
            tape2.insert(tape2.begin(), ' ');
            headPos2 = 0;
        }
        if (tape2.empty())
        {
            tape2.push_back(' ');
        }
    }
    else
    {
        tape2.push_back(' ');
    }
    
    //------------------------------------------------------------------------

    string state = "0";
    char symbol1 = tape1[headPos1];
    char symbol2 = ' ';
    if(n==2)
        symbol2 = tape2[headPos2];
    
    bool isPaused = false;
    while(true)
    {
        // Sustojimas su mygtuko paspaudimu
        if (_kbhit())
        {
            char buttonKey = _getch();
            if (buttonKey == 'p' || buttonKey == 'P')
            {
                isPaused = !isPaused;
            }
            if (buttonKey == 'E' || buttonKey == 'e')
            {
                break;
            }
        }
        if (isPaused)
        {
            Sleep(200);
            continue;
        }

        Show(n,headPos1, headPos2, state, symbol1, symbol2, tape1, tape2); // atvaizduojama ekrane

        // veikimo limitas
        if (tape1.size() > tapeLimit || tape2.size() > tapeLimit)
        {
            Show(n, headPos1, headPos2, state, symbol1, symbol2, tape1, tape2);
            cout << endl;
            cout << "Tape limit exceeded" << endl;
            return 0;
        }

        // ar galvute iseina uz juostos ribu
        if (headPos1 < 0)
        {
            tape1.insert(tape1.begin(), ' ');
            headPos1 = 0;
        }
        else if (headPos1 >= tape1.size())
        {
            tape1.push_back(' ');
        }
        // Jei dvi juostos
        if (n == 2)
        {
            if (headPos2 < 0)
            {
                tape2.insert(tape2.begin(), ' ');
                headPos2 = 0;
            }
            else if (headPos2 >= tape2.size())
            {
                tape2.push_back(' ');
            }
        }
        
        symbol1 = tape1[headPos1]; // nuskaitomas esamas simbolis
        if(n==2)
            symbol2 = tape2[headPos2];

        string key;               //sudaromas vykdomos komandos kodas
        if(n==1)key = state + '_' + symbol1;
        if (n == 2)key = state + '_' + symbol1 + '_' + symbol2;

        auto it = rules.find(key); // suranda reikiama komanda is mapo (grazina pointeri i komanda)

        if (it != rules.end())
        {
            rule do_rule = it->second;

            tape1[headPos1] = do_rule.newSymbol1; // pakeiciamas simbolis
            if(n==2) tape2[headPos2] = do_rule.newSymbol2;

            state = do_rule.newState; // pakeiciama busena
            
            //pajudinama galvute
            if (do_rule.direction1 == 'R')
            {
                headPos1++;
            }
            else if (do_rule.direction1 == 'L')
            {
                headPos1--;
            }
            //jei dvi juostos
            if (do_rule.direction2 == 'R')
            {
                headPos2++;
            }
            else if (do_rule.direction2 == 'L')
            {
                headPos2--;
            }
        }
        else
        {
            Show(n, headPos1, headPos2, state, symbol1, symbol2, tape1, tape2);

            // Tikrinama ar masina pasieke galutine busena ar tesiog nerado instrukcijos
            bool halted = true;
            for (auto x : rules)
            {
                if (state == x.second.currentState) halted = false;
            }
            if (halted)
            {
                cout << endl;
                cout << "machine halted" << endl;
            } 
            else if (!halted)
            {
                cout << endl;
                if (symbol1 == ' ') symbol1 = 'b';
                if (symbol2 == ' ' && n==2) symbol2 = 'b';
                if(n==1)cout << "missing condition for state " << state << " symbol " << symbol1 << endl;

                //jei dvi juostos
                if (n == 2)cout << "missing condition for state " << state << " symbol1 " << symbol1 << " symbol2 " << symbol2 << endl;
            }

            return 0;
        }
        Sleep(100);
    }
    return 0;
}

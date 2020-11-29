#include <string>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <io.h>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iterator>
using namespace std;

class Konvertavimas
{
private:
    vector<int> vektorius;
    vector<int> vektoriushex;
public:
    void KonvertuotiIUTF8(int skaicius);
    void KonvertuotiIHex(int skaicius);
    void KonvertuotiIDvejetaine(int skaicius);
    void KonvertuotiISimboli(string dvejetaine);
    void KonvertuotiFaila(string failas, string rezultatas, int lentele[]);
    void UzpildytiLentele(int lentele[], string failas);
    string IntIUTF8Failui(int cp437);
};

void Konvertavimas::KonvertuotiISimboli(string dvejetaine)
{
    ofstream fr("pirmadalisrez.txt", ios::app);
    if (dvejetaine.size() == 16) // < 2048
    {
        string a = dvejetaine.substr(0, 8); //baitu skaidymas i 2 dalis pvz.: 00000000000000001010100111000101 bus 10101001 11000101
        string b = dvejetaine.substr(8, 8);

        char apirmas = stoi(a, nullptr, 2);
        char bantras = stoi(b, nullptr, 2); //vienas baitas verciamas i desimtaine

        unsigned char A[2];
        A[0] = apirmas;
        A[1] = bantras;
        fr << "Simbolis: ";
        for (int i = 0; i < 2; i++)
        {
            fr << A[i];
        }
        fr << endl;
        fr.close();
    }
    else if (dvejetaine.size() == 24) // < 65535
    {
        ofstream fr("pirmadalisrez.txt", ios::app);
        string a = dvejetaine.substr(0, 8); //baitu skaidymas i 3 dalis
        string b = dvejetaine.substr(8, 8);
        string c = dvejetaine.substr(16, 8);

        char apirmas = stoi(a, nullptr, 2);
        char bantras = stoi(b, nullptr, 2);
        char ctrecias = stoi(c, nullptr, 2);

        unsigned char A[3];
        A[0] = apirmas;
        A[1] = bantras;
        A[2] = ctrecias;
        fr << "Simbolis: ";
        for (int i = 0; i < 3; i++)
        {
            fr << A[i];
        }
        fr << endl;
        fr.close();
    }
    else //32 // < 1114111
    {
        ofstream fr("pirmadalisrez.txt", ios::app);
        string a = dvejetaine.substr(0, 8); //baitu skaidymas i 4 dalis
        string b = dvejetaine.substr(8, 8);
        string c = dvejetaine.substr(16, 8);
        string d = dvejetaine.substr(24, 8);

        char apirmas = stoi(a, nullptr, 2);
        char bantras = stoi(b, nullptr, 2);
        char ctrecias = stoi(c, nullptr, 2);;
        char dketvirtas = stoi(d, nullptr, 2);

        unsigned char A[4];
        A[0] = apirmas;
        A[1] = bantras;
        A[2] = ctrecias;
        A[3] = dketvirtas;
        fr << "Simbolis: ";
        for (int i = 0; i < 4; i++)
        {
            fr << A[i];
        }
        fr << endl;
        fr.close();
    }
}

void Konvertavimas::KonvertuotiIHex(int skaicius) //16-taine
{
    ofstream fr("pirmadalisrez.txt", ios::app);
    char hexValue[100];

    _itoa_s(skaicius, hexValue, 16); //funkcija, kuri konvertuoja dvejetaine i sesioliktaine

    fr << "UTF-8: " << (string)hexValue << endl;
    fr.close();

};

void Konvertavimas::KonvertuotiIDvejetaine(int skaicius)
{
    while (skaicius > 0)
    {
        vektorius.push_back(skaicius % 2); //pildau vektoriu su 1 arba 0
        skaicius = skaicius / 2;
    }
    std::reverse(vektorius.begin(), vektorius.end()); //apsukamas vektorius, nes pildoma is kaires i desine pagal nutylejima

    for (int i = vektorius.size(); i < 32; i++) //vektorius pildomas nuliais kol pasieks 32 dydi pvz.: yra binary 101011010 tai po ciklo bus 0000000000000001011010
    {
        vektorius.insert(vektorius.begin(), 0);
    } //veliau programoje pagal ifus imsiu is dvejetaines kiek man reikia
}

void Konvertavimas::KonvertuotiIUTF8(int skaicius)
{

    if (skaicius < 128) //0 iki 127    nieko nereikia delioti, nes iki 127 yra paprasti simboliai, kuriuos galima printint naudojant (char)
    {
        ofstream fr("pirmadalisrez.txt", ios::app);
        KonvertuotiIHex(skaicius);
        fr << "Simbolis: " << (char)skaicius << endl;
        fr.close();
    }
    else if (skaicius < 2048) //128 iki 2047
    {
        KonvertuotiIDvejetaine(skaicius); //ivestas desimtainis skaicius verciamas i dvejetaine su 32 ilgiu
        vektoriushex.push_back(vektorius[31]); //deliojimas pagal skaidres
        vektoriushex.push_back(vektorius[30]);
        vektoriushex.push_back(vektorius[29]);
        vektoriushex.push_back(vektorius[28]);
        vektoriushex.push_back(vektorius[27]);
        vektoriushex.push_back(vektorius[26]);
        vektoriushex.push_back(10);
        vektoriushex.push_back(vektorius[25]);
        vektoriushex.push_back(vektorius[24]);
        vektoriushex.push_back(vektorius[23]);
        vektoriushex.push_back(vektorius[22]);
        vektoriushex.push_back(vektorius[21]);
        std::reverse(vektoriushex.begin(), vektoriushex.end());
        vektoriushex.insert(vektoriushex.begin(), 110);

        stringstream result;
        std::copy(vektoriushex.begin(), vektoriushex.end(), std::ostream_iterator<int>(result));
        string simbolis = result.str(); //vektorius int verciamas i string

        int dvejetaine = stoi(simbolis, nullptr, 2); //i int

        KonvertuotiIHex(dvejetaine);
        KonvertuotiISimboli(simbolis);
    }
    else if (skaicius < 65535) //2048 iki 65535
    {
        KonvertuotiIDvejetaine(skaicius);
        vektoriushex.push_back(vektorius[31]);
        vektoriushex.push_back(vektorius[30]);
        vektoriushex.push_back(vektorius[29]);
        vektoriushex.push_back(vektorius[28]);
        vektoriushex.push_back(vektorius[27]);
        vektoriushex.push_back(vektorius[26]);
        vektoriushex.push_back(10);
        vektoriushex.push_back(vektorius[25]);
        vektoriushex.push_back(vektorius[24]);
        vektoriushex.push_back(vektorius[23]);
        vektoriushex.push_back(vektorius[22]);
        vektoriushex.push_back(vektorius[21]);
        vektoriushex.push_back(vektorius[20]);
        vektoriushex.push_back(10);
        vektoriushex.push_back(vektorius[19]);
        vektoriushex.push_back(vektorius[18]);
        vektoriushex.push_back(vektorius[17]);
        vektoriushex.push_back(vektorius[16]);
        std::reverse(vektoriushex.begin(), vektoriushex.end());
        vektoriushex.insert(vektoriushex.begin(), 1110);

        stringstream result;
        std::copy(vektoriushex.begin(), vektoriushex.end(), std::ostream_iterator<int>(result));
        string simbolis = result.str();

        int dvejetaine = stoi(simbolis, nullptr, 2);

        KonvertuotiIHex(dvejetaine);
        KonvertuotiISimboli(simbolis);
    }
    else //65536 iki 1114111
    {
        KonvertuotiIDvejetaine(skaicius);
        vektoriushex.push_back(vektorius[31]);
        vektoriushex.push_back(vektorius[30]);
        vektoriushex.push_back(vektorius[29]);
        vektoriushex.push_back(vektorius[28]);
        vektoriushex.push_back(vektorius[27]);
        vektoriushex.push_back(vektorius[26]);
        vektoriushex.push_back(10);
        vektoriushex.push_back(vektorius[25]);
        vektoriushex.push_back(vektorius[24]);
        vektoriushex.push_back(vektorius[23]);
        vektoriushex.push_back(vektorius[22]);
        vektoriushex.push_back(vektorius[21]);
        vektoriushex.push_back(vektorius[20]);
        vektoriushex.push_back(10);
        vektoriushex.push_back(vektorius[19]);
        vektoriushex.push_back(vektorius[18]);
        vektoriushex.push_back(vektorius[17]);
        vektoriushex.push_back(vektorius[16]);
        vektoriushex.push_back(vektorius[15]);
        vektoriushex.push_back(vektorius[14]);
        vektoriushex.push_back(10);
        vektoriushex.push_back(vektorius[13]);
        vektoriushex.push_back(vektorius[12]);
        vektoriushex.push_back(vektorius[11]);
        std::reverse(vektoriushex.begin(), vektoriushex.end());
        vektoriushex.insert(vektoriushex.begin(), 11110);

        stringstream result;
        std::copy(vektoriushex.begin(), vektoriushex.end(), std::ostream_iterator<int>(result));
        string simbolis = result.str();
        //int dvejetaine = stoi(simbolis, nullptr, 2); su sia funkcija uzlusta programa kai skaicius daugiau negu 63365

            string num = simbolis;
            int dvejetaine = 0, base = 1;

            int len = num.length();
            for (int i = len - 1; i >= 0; i--)
            {
                if (num[i] == '1')
                    dvejetaine += base;
                base = base * 2;
            } //cia dvejetaines stringas verciamas i int vietoj funkcijos stoi

        KonvertuotiIHex(dvejetaine);
        KonvertuotiISimboli(simbolis);
    }
}

void Konvertavimas::KonvertuotiFaila(string failas, string rezultatas, int lentele[])
{
    basic_ifstream<unsigned char> fd(failas);
    ofstream out(rezultatas);
    unsigned char raide; // nuo 0 iki 255
    while (fd.get(raide)) //skaitoma kas raide/simboli
    {
        int baitai = (int)raide; 

        if (baitai < 128)
            out << raide;
        else //virs 127 yra blogi, reikia keist
        {
            out << IntIUTF8Failui(lentele[baitai - 128]);
        }
    }

    fd.close();
    out.close();
}

void Konvertavimas::UzpildytiLentele(int lentele[], string failas)
{

    ifstream fd(failas); 
    string unikodas;

    for (int i = 0; i < 128; i++)
    {
        fd >> unikodas; //nuskaitomas unikodas is failo
        unsigned int x = strtoul(unikodas.c_str(), NULL, 16); //versti hex i desimtaine
        lentele[i] = x; //idedamas unikodas i masyva
    }

    fd.close();
}

string Konvertavimas::IntIUTF8Failui(int cp437) //is https://stackoverflow.com/questions/18729405/how-to-convert-utf8-string-to-byte-array, mano metodas neveikia
{
    if (cp437 < 2048) //baitu skaidymas i 2 dalis
    {
        string out = "00"; //vietoj nuliu bus cp437 baitai
                          // 63     //128
        char a = (cp437 & 0x003F) + 0x80;//verciama i baita
        char b = ((cp437 & 0x07C0) >> 6) + 0xC0; //192 // >> zenklas perkelia dvejetaine israiska i desine 6 pozicijom, & yra loginis AND
        out[0] = b;
        out[1] = a;
        return out;
    }

    else if (cp437 < 65535) // baitu skaidymas i 3 dalis
    {
        string out = "000";

        char a = ((cp437 & 0xF000) >> 12) + 0x00E0; //224
        char b = ((cp437 & 0xFC0) >> 6) + 0x80; //128
        char c = (cp437 & 0x003F) + 0x80;

        out[0] = a;
        out[1] = b;
        out[2] = c;
        return out;
    }
    else if (cp437 < 1114111) //baitu skaidymas i 4 dalis
    {
        string out = "0000";

        char a = ((cp437 & 0x3F000) >> 18) + 0xF0; //240
        char b = ((cp437 & 0x3F000) >> 12) + 0x80;
        char c = ((cp437 & 0xFC0) >> 6) + 0x80;
        char d = (cp437 & 0x003F) + 0x80;
        out[0] = a;
        out[1] = b;
        out[2] = c;
        out[3] = d;

        return out;
    }
}

int main()
{
    Konvertavimas konvertuoti;
    int skaicius;

    _setmode(_fileno(stdout), _O_U8TEXT); //koduote tampa utf-8
    ofstream fr("pirmadalisrez.txt");

    wcout << L"Iveskite skaiciu, kuri konvertuot i UTF-8 ir UNICODE: ";
    cin >> skaicius;
    stringstream my_ss; //stringstream pagalba paverciu desimtaine i sesioliktaine
    my_ss << uppercase << hex << skaicius;
    string res = my_ss.str();

    if (skaicius < 16)
    {
        fr << "Unicode: U+000" << res << endl;                
        konvertuoti.KonvertuotiIUTF8(skaicius);
    }
    else if (skaicius < 256)
    {
        fr << "Unicode: U+00" << res << endl;
        konvertuoti.KonvertuotiIUTF8(skaicius);
    }
    else
    {
        fr << "Unicode: U+0" << res << endl;
        konvertuoti.KonvertuotiIUTF8(skaicius);
    } //nustatyt koki U+ naudot naudojau http://www.ltg.ed.ac.uk/~richard/utf-8.cgi?input=3000&mode=decimal puslapi

    int lentele[128]; //masyvas cp437
    konvertuoti.UzpildytiLentele(lentele, "cp437.txt");
    konvertuoti.KonvertuotiFaila("386intel.txt", "output.txt", lentele);

    fr.close();
    system("pause");
    return 0;
}
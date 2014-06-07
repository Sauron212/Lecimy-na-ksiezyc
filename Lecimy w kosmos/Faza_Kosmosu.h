#ifndef FAZA_KOSMOSU_H_INCLUDED
#define FAZA_KOSMOSU_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>

const double pi = atan (1) * 4;
std::ofstream dx ("dx.txt");

sf::Vector2f Grawitacja (double x_1, double y_1, double masa_1, double x_2, double y_2, double masa_2)
{
    double Fg = 6.67 * masa_1 * masa_2 / (pow ((x_2 - x_1), 2) + pow ((y_2 - y_1), 2)) * pow (10, -11);
    sf::Vector2f wektor_grawitacji (Fg / sqrt (pow ((x_2 - x_1), 2) + pow ((y_2 - y_1), 2)) * (x_2 - x_1), Fg / sqrt (pow ((x_2 - x_1), 2) + pow ((y_2 - y_1), 2)) * (y_2 - y_1));
    return wektor_grawitacji;
}

sf::Texture ziemia;

class Planeta
{
    public:
        double koordynata_x;
        double koordynata_y;
        double masa;
        double omega_obiegu;
        double omega_obrotu;
        double pozycja_katowa;
        double promien_orbity;
        double promien_planety;
        sf::Sprite sprite;
        Planeta (double c_masa, double c_T_obiegu, double c_T_obrotu, double c_pozycja_katowa, double c_promien_orbity, double c_promien_planety)
        {
            koordynata_x = cos (c_pozycja_katowa) * c_promien_orbity;
            koordynata_y = sin (c_pozycja_katowa) * c_promien_orbity;
            masa = c_masa;
            if (c_T_obiegu != 0) omega_obiegu = 2 * pi / c_T_obiegu;
            else omega_obiegu = 0;
            if (c_T_obrotu != 0) omega_obrotu = 2 * pi / c_T_obrotu;
            else omega_obrotu = 0;
            pozycja_katowa = c_pozycja_katowa;
            promien_orbity = c_promien_orbity;
            promien_planety = c_promien_planety;
            sprite.setTexture (ziemia);
            sprite.setOrigin (250, 250);
            sprite.setScale (promien_planety / 250000000, promien_planety / 250000000);
            sprite.setPosition (koordynata_x / 1000000, -koordynata_y / 1000000);
        }
        void Aktualizacja ()
        {
            pozycja_katowa += omega_obiegu;
            if (pozycja_katowa > 2 * pi) pozycja_katowa = 0;
            sprite.rotate (omega_obrotu * 180 / pi);
            koordynata_x = 0; //cos (pozycja_katowa) * promien_orbity;
            koordynata_y = 0; //sin (pozycja_katowa) * promien_orbity;
            sprite.setPosition (koordynata_x / 1000000, -koordynata_y / 1000000);
        }
};

std::vector <Planeta> planety;
sf::Texture ksiezyc;

class Satelita
{
    public:
        Planeta* planeta;
        double koordynata_x;
        double koordynata_y;
        double masa;
        double omega_obiegu;
        double omega_obrotu;
        double pozycja_katowa;
        double promien_orbity;
        double promien_satelity;
        sf::Sprite sprite;
        Satelita (double c_masa, double c_T_obiegu, double c_T_obrotu, double c_pozycja_katowa, double c_promien_orbity, double c_promien_satelity, Planeta* c_planeta)
        {
            planeta = c_planeta;
            koordynata_x = cos (c_pozycja_katowa) * c_promien_orbity + cos (planeta->pozycja_katowa) * planeta->promien_orbity;
            koordynata_y = sin (c_pozycja_katowa) * c_promien_orbity + sin (planeta->pozycja_katowa) * planeta->promien_orbity;
            masa = c_masa;
            if (c_T_obiegu != 0) omega_obiegu = 2 * pi / c_T_obiegu;
            else omega_obiegu = 0;
            if (c_T_obrotu != 0) omega_obrotu = 2 * pi / c_T_obrotu;
            else omega_obrotu = 0;
            pozycja_katowa = c_pozycja_katowa;
            promien_orbity = c_promien_orbity;
            promien_satelity = c_promien_satelity;
            sprite.setTexture (ksiezyc);
            sprite.setOrigin (250, 250);
            sprite.setScale (promien_satelity / 250000000, promien_satelity / 250000000);
            sprite.setPosition (koordynata_x / 1000000, -koordynata_y / 1000000);
        }
        void Aktualizacja ()
        {
            pozycja_katowa -= omega_obiegu;
            if (pozycja_katowa < 0) pozycja_katowa = 2 * pi;
            sprite.rotate (omega_obrotu * 180 / pi);
            koordynata_x = cos (pozycja_katowa) * promien_orbity + planeta->koordynata_x;
            koordynata_y = sin (pozycja_katowa) * promien_orbity + planeta->koordynata_y;
            sprite.setPosition (koordynata_x / 1000000, -koordynata_y / 1000000);
        }
};

std::vector <Satelita> satelity;

struct Czas
{
    int start;
    int czas_trwania;
};

std::vector <Czas> czasy_silnika;
int current_index = 0;
Czas czas_1;

class Rakieta
{
    public:
        double koordynata_x;
        double koordynata_y;
        double masa;
        double pozycja_katowa;
        double ciag;
        sf::Vector2f Fg;
        sf::Vector2f F1;
        sf::Vector2f F2;
        sf::Vector2f a;
        sf::Vector2f v;
        sf::Texture image;
        sf::Sprite sprite;
        bool ruch_obrotowy;
        double alfa_1;
        double alfa_2;
        double O;
        double E;
        double I;
        bool ox;
        Rakieta (double c_x, double c_y, double c_masa, bool kkk)
        {
            ox = kkk;
                koordynata_x = c_x;
                koordynata_y = c_y;
            masa = c_masa;
            Fg = sf::Vector2f (0, 0);
            a = Fg;
            if (kkk) v = sf::Vector2f (2453.91, -6273.57);
            else v = sf::Vector2f (0, -3080.975);
            pozycja_katowa = 0 * pi / 2;
            ciag = 1001000;
            image.loadFromFile ("4.png");
            image.setSmooth (true);
            sprite.setTexture (image);
            sprite.setOrigin (50, 150);
            sprite.setScale (0.025, 0.025);
            ruch_obrotowy = true;
            I = c_masa * 24 * 2;
            O = 0;
        }
        void RRakieta (double c_x, double c_y, double c_masa, bool kkk)
        {
            ox = kkk;
                koordynata_x = c_x;
                koordynata_y = c_y;
            masa = c_masa;
            Fg = sf::Vector2f (0, 0);
            a = Fg;
            if (kkk) v = sf::Vector2f (2453.91, -6273.57);
            else v = sf::Vector2f (0, -3080.975);
            pozycja_katowa =  atan2 (v.y, v.x);
            ciag = 1001000;
            image.loadFromFile ("4.png");
            image.setSmooth (true);
            sprite.setTexture (image);
            sprite.setOrigin (50, 150);
            sprite.setScale (0.025, 0.025);
            ruch_obrotowy = true;
            I = c_masa * 24 * 2;
            O = 0;
        }
        void Aktualizacja (int czas)
        {
            Fg = Grawitacja (koordynata_x, koordynata_y, masa, planety [0].koordynata_x, planety [0].koordynata_y, planety [0].masa) + Grawitacja (koordynata_x, koordynata_y, masa, satelity [0].koordynata_x, satelity [0].koordynata_y, satelity [0].masa);
            if (czasy_silnika.size () > current_index)
            {
                if (czas > czasy_silnika [current_index].start && czas < czasy_silnika [current_index].start + czasy_silnika [current_index].czas_trwania)
                {
                    Fg += sf::Vector2f (cos (pozycja_katowa) * ciag, sin (pozycja_katowa) * ciag);
                    masa -= 220.682;
                    I = 1/12 * masa * 24 * 24;
                }
            }
            a = sf::Vector2f (Fg.x / masa, Fg.y / masa);
            v += a;
            koordynata_x += v.x;
            koordynata_y += v.y;
            if (ruch_obrotowy) pozycja_katowa = atan2 (v.y, v.x);
            else
            {
                F1 = Grawitacja (koordynata_x + cos (pozycja_katowa) * 12, koordynata_y + sin (pozycja_katowa) * 12, masa / 2, planety [0].koordynata_x, planety [0].koordynata_y, planety [0].masa) +
                Grawitacja (koordynata_x + cos (pozycja_katowa) * 12, koordynata_y + sin (pozycja_katowa) * 12, masa / 2, satelity [0].koordynata_x, satelity [0].koordynata_y, satelity [0].masa);
                F2 = Grawitacja (koordynata_x - cos (pozycja_katowa) * 12, koordynata_y - sin (pozycja_katowa) * 12, masa / 2, planety [0].koordynata_x, planety [0].koordynata_y, planety [0].masa) +
                Grawitacja (koordynata_x - cos (pozycja_katowa) * 12, koordynata_y - sin (pozycja_katowa) * 12, masa / 2, satelity [0].koordynata_x, satelity [0].koordynata_y, satelity [0].masa);
                alfa_1 = pi - pozycja_katowa + atan2 (F1.y, F1.x);
                alfa_2 = pi - pozycja_katowa + atan2 (F2.y, F2.x);
                E = 6 * (sin (alfa_1) * sqrt (F1.x * F1.x + F1.y * F1.y) - sin (alfa_2) * sqrt (F2.x * F2.x + F2.y * F2.y)) / I;
                pozycja_katowa += O;
                O += E;
                double h = pozycja_katowa / (2 * pi);
            }
            if (pozycja_katowa < 0) pozycja_katowa += 2 * pi;
        }
};

Rakieta FK_Rakieta (6089920, 2944900, 131825.7, true); //-6563000   -7017530    31206900    131825.7
//Rakieta FK_Rakieta (42160000, 0, 131825.7);
double kat = 0;

class Button
{
    public:
        int koordynata_x;
        int koordynata_y;
        int szerokosc;
        int wysokosc;
        bool clicked;
        sf::Texture image;
        sf::Sprite sprite;
        Button (int c_koordynata_x, int c_koordynata_y, int c_szerokosc, int c_wysokosc, std::string image_path)
        {
            koordynata_x = c_koordynata_x;
            koordynata_y = c_koordynata_y;
            szerokosc = c_szerokosc;
            wysokosc = c_wysokosc;
            clicked = false;
            image.loadFromFile (image_path);
            image.setSmooth (true);
            sprite.setTexture (image);
            sprite.setOrigin (25, 25);
            sprite.setScale (0.4f, 0.4f);
            sprite.setPosition (koordynata_x, koordynata_y);
        }
};

std::vector <Button*> buttons;

void Laduj_Uklad ()
{
    ziemia.loadFromFile ("Ziemia.png");
    ksiezyc.loadFromFile ("Ksiezyc.png");
    planety.push_back (Planeta (5.9721 * pow (10, 24), 31558149.7635, 24 * 3600, 0, 0, 6378000));
    satelity.push_back (Satelita (7.347673 * pow (10, 22), 2360591.5104, 2360591.5104, 3.995833205769379715, 384400000, 1737064, &planety [0]));
    czas_1.czas_trwania = 350;
    czas_1.start = 0;
    czasy_silnika.push_back (czas_1);
}

void Laduj_Guziki ()
{
    buttons.push_back (new Button (940, 254, 20, 20, "Minus.png"));
    buttons.push_back (new Button (965, 254, 20, 20, "Plus.png"));
    buttons.push_back (new Button (940, 274, 20, 20, "Minus.png"));
    buttons.push_back (new Button (965, 274, 20, 20, "Plus.png"));
}

std::vector <sf::Vector2f> punkty;
std::vector <double> katy;
std::vector <double> czasy;
std::vector <double> predkosci;

void Symulacja (int czas)
{
    bool zderzenie = false;
    punkty.clear ();
    katy.clear ();
    czasy.clear ();
    predkosci.clear ();
    punkty.push_back (sf::Vector2f (FK_Rakieta.koordynata_x, FK_Rakieta.koordynata_y));
    katy.push_back (FK_Rakieta.pozycja_katowa);
    czasy.push_back (0);
    predkosci.push_back (sqrt (pow (FK_Rakieta.v.x, 2) + pow (FK_Rakieta.v.y, 2)));
    for (long i = 0; i < czas && !zderzenie; i++)
    {
        for (int j = 0; j < planety.size (); j++) planety [j].Aktualizacja ();
        for (int j = 0; j < satelity.size (); j++) satelity [j].Aktualizacja ();
        if (sqrt (pow (FK_Rakieta.koordynata_x - planety [0].koordynata_x, 2) + pow ( FK_Rakieta.koordynata_y - planety [0].koordynata_y, 2)) < planety [0].promien_planety || sqrt (pow (FK_Rakieta.koordynata_x - satelity [0].koordynata_x, 2) + pow ( FK_Rakieta.koordynata_y - satelity [0].koordynata_y, 2)) < satelity [0].promien_satelity) zderzenie = true;
        if (i > czasy_silnika [current_index].start + czasy_silnika [current_index].czas_trwania && current_index < czasy_silnika.size ()) current_index += 1;
        FK_Rakieta.Aktualizacja (i);
        if (pow ((FK_Rakieta.koordynata_x - punkty [punkty.size () - 1].x), 2) + pow ((FK_Rakieta.koordynata_y - punkty [punkty.size () - 1].y), 2) > 10000000000)
        {
            punkty.push_back (sf::Vector2f (FK_Rakieta.koordynata_x, FK_Rakieta.koordynata_y));
            katy.push_back (FK_Rakieta.pozycja_katowa);
            czasy.push_back (i);
            predkosci.push_back (sqrt (pow (FK_Rakieta.v.x, 2) + pow (FK_Rakieta.v.y, 2)));
        }
    }
}

void Dane ()
{

}

class Wektory
{
    public:
        sf::RectangleShape wektor;
        int dlugosc;
        double poczatkowe=0;
        Wektory(sf::Vector2f W,int x, int y,sf::Color s,bool zwrot=true);
};
Wektory::Wektory(sf::Vector2f W,int x, int y,sf::Color s,bool zwrot)    /// zwrot == true/false do gory/w dol
{
    dlugosc = sqrt(pow(W.x,2)+pow(W.y,2));
    wektor.setSize(sf::Vector2f(2,dlugosc/100000));
    wektor.setFillColor(s);
    wektor.setPosition(x,y);
    if(zwrot)
        wektor.setOrigin(0,wektor.getSize().y);
    wektor.setRotation(asin(W.x/dlugosc)*180/pi);
}

#endif // FAZA_KOSMOSU_H_INCLUDED

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

class Planeta
{
    public:
        double koordynata_x;
        double koordynata_y;
        double masa;
        double omega;
        double pozycja_katowa;
        double promien_orbity;
        double promien_planety;
        sf::CircleShape grafika;
        Planeta (double c_masa, double c_T, double c_pozycja_katowa, double c_promien_orbity, double c_promien_planety)
        {
            koordynata_x = cos (c_pozycja_katowa) * c_promien_orbity;
            koordynata_y = sin (c_pozycja_katowa) * c_promien_orbity;
            masa = c_masa;
            if (c_T != 0) omega = 2 * pi / c_T;
            else omega = 0;
            pozycja_katowa = 0;
            promien_orbity = c_promien_orbity;
            promien_planety = c_promien_planety;
            grafika.setRadius (promien_planety / pow (10, 6));
            grafika.setPosition (koordynata_x / pow (10, 6) - grafika.getRadius (), -koordynata_y / pow (10, 6) - grafika.getRadius ());
        }
        void Aktualizacja ()
        {
            pozycja_katowa += omega;
            if (pozycja_katowa > 2 * pi) pozycja_katowa = 0;
            koordynata_x = cos (pozycja_katowa) * promien_orbity;
            koordynata_y = sin (pozycja_katowa) * promien_orbity;
        }
};

std::vector <Planeta> planety;

class Satelita
{
    public:
        Planeta* planeta;
        double koordynata_x;
        double koordynata_y;
        double masa;
        double omega;
        double pozycja_katowa;
        double promien_orbity;
        double promien_satelity;
        sf::CircleShape grafika;
        Satelita (double c_masa, double c_T, double c_pozycja_katowa, double c_promien_orbity, double c_promien_satelity, Planeta* c_planeta)
        {
            planeta = c_planeta;
            koordynata_x = cos (c_pozycja_katowa) * c_promien_orbity + cos (planeta->pozycja_katowa) * planeta->promien_orbity;
            koordynata_y = sin (c_pozycja_katowa) * c_promien_orbity + sin (planeta->pozycja_katowa) * planeta->promien_orbity;
            masa = c_masa;
            if (c_T != 0) omega = 2 * pi / c_T;
            else omega = 0;
            pozycja_katowa = c_pozycja_katowa;
            promien_orbity = c_promien_orbity;
            promien_satelity = c_promien_satelity;
            grafika.setRadius (promien_satelity / pow (10, 6));
            grafika.setPosition (koordynata_x / pow (10, 6) - grafika.getRadius (), -koordynata_y / pow (10, 6) - grafika.getRadius ());
        }
        void Aktualizacja ()
        {
            pozycja_katowa += omega;
            if (pozycja_katowa > 2 * pi) pozycja_katowa = 0;
            koordynata_x = cos (pozycja_katowa) * promien_orbity + planeta->koordynata_x;
            koordynata_y = sin (pozycja_katowa) * promien_orbity + planeta->koordynata_y;
        }
};

std::vector <Satelita> satelity;

class Rakieta
{
    public:
        double koordynata_x;
        double koordynata_y;
        double masa;
        double epsilon;
        double omega;
        double pozycja_katowa;
        double dlugosc_1;
        double dlugosc_2;
        double alfa_1;
        double alfa_2;
        sf::Vector2f Fg;
        sf::Vector2f Fg_1;
        sf::Vector2f Fg_2;
        sf::Vector2f a;
        sf::Vector2f v;
        sf::Texture image;
        sf::Sprite sprite;
        Rakieta (double c_x, double c_y, double c_masa)
        {
            koordynata_x = c_x;
            koordynata_y = c_y;
            masa = c_masa;
            Fg = sf::Vector2f (0, 0);
            a = Fg;
            v = Fg;
            image.loadFromFile ("Rakieta.png");
            image.setSmooth (true);
            sprite.setTexture (image);
            sprite.setOrigin (187.5, 150);
            sprite.setScale (0.02, 0.02);
            dlugosc_1 = 15;
            dlugosc_2 = dlugosc_1;
            epsilon = 0;
            omega = 0;
            pozycja_katowa = pi / 2;
        }
        void Aktualizacja ()
        {
            Fg = sf::Vector2f (0, 0);
            Fg_1 = Fg;
            Fg_2 = Fg;
            for (int i = 0; i < planety.size (); i++)
            {
                Fg += Grawitacja (koordynata_x, koordynata_y, masa, planety [i].koordynata_x, planety [i].koordynata_y, planety [i].masa);
                Fg_1 += Grawitacja (koordynata_x + cos (pozycja_katowa) * dlugosc_1, koordynata_y + sin (pozycja_katowa) * dlugosc_1, masa / 2, planety [i].koordynata_x, planety [i].koordynata_y, planety [i].masa);
                Fg_2 += Grawitacja (koordynata_x + cos (pozycja_katowa + pi) * dlugosc_2, koordynata_y + sin (pozycja_katowa + pi) * dlugosc_2, masa / 2, planety [i].koordynata_x, planety [i].koordynata_y, planety [i].masa);
            }
            for (int i = 0; i < satelity.size (); i++)
            {
                Fg += Grawitacja (koordynata_x, koordynata_y, masa, satelity [i].koordynata_x, satelity [i].koordynata_y, satelity [i].masa);
                Fg_1 += Grawitacja (koordynata_x + cos (pozycja_katowa) * dlugosc_1, koordynata_y + sin (pozycja_katowa) * dlugosc_1, masa / 2, satelity [i].koordynata_x, satelity [i].koordynata_y, satelity [i].masa);
                Fg_2 += Grawitacja (koordynata_x + cos (pozycja_katowa + pi) * dlugosc_2, koordynata_y + sin (pozycja_katowa + pi) * dlugosc_2, masa / 2, satelity [i].koordynata_x, satelity [i].koordynata_y, satelity [i].masa);
            }
            a = sf::Vector2f (Fg.x / masa, Fg.y / masa);
            v += sf::Vector2f (a.x, a.y);
            koordynata_x += v.x;
            koordynata_y += v.y;
            alfa_1 = std::signbit (((atan2 (Fg_1.y, Fg_1.x) - std::signbit (sin (pozycja_katowa)) * pi) + std::signbit (atan2 (Fg_1.y, Fg_1.x) - std::signbit (sin (pozycja_katowa)) * pi) * 2 * pi - pi - pozycja_katowa + std::signbit (sin (pozycja_katowa)) * pi) * -2 + 1) * pi + atan2 (Fg_1.y, Fg_1.x) - std::signbit (sin (pozycja_katowa)) * pi + std::signbit (atan2 (Fg_1.y, Fg_1.x) - std::signbit (sin (pozycja_katowa)) * pi) * 2 * pi - (pozycja_katowa - std::signbit (sin (pozycja_katowa)) * pi);
            alfa_2 = std::signbit (((atan2 (Fg_2.y, Fg_2.x) - std::signbit (sin (pozycja_katowa)) * pi) + std::signbit (atan2 (Fg_2.y, Fg_2.x) - std::signbit (sin (pozycja_katowa)) * pi) * 2 * pi - pi - pozycja_katowa + std::signbit (sin (pozycja_katowa)) * pi) * -2 + 1) * pi + atan2 (Fg_2.y, Fg_2.x) - std::signbit (sin (pozycja_katowa)) * pi + std::signbit (atan2 (Fg_2.y, Fg_2.x) - std::signbit (sin (pozycja_katowa)) * pi) * 2 * pi - (pozycja_katowa - std::signbit (sin (pozycja_katowa)) * pi) + pi;
            epsilon = 3 * (dlugosc_1 * sin (alfa_1) * sqrt (Fg_1.x * Fg_1.x + Fg_1.y * Fg_1.y) + dlugosc_2 * sin (alfa_2) * sqrt (Fg_2.x * Fg_2.x + Fg_2.y * Fg_2.y)) / (masa * (dlugosc_1 + dlugosc_2) * (dlugosc_1 + dlugosc_2));
            omega += epsilon;
            pozycja_katowa = atan2 (v.y, v.x);
            if (pozycja_katowa < 0) pozycja_katowa += 2 * pi;
        }
};

Rakieta FK_Rakieta (1.49597870 * pow (10, 11), 42231860.82, 100);

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
            sprite.setPosition (koordynata_x, koordynata_y);
        }
};

std::vector <Button*> buttons;

void Laduj_Uklad ()
{
    //planety.push_back (Planeta (1.9891 * pow (10, 1), 0, 0, 0, 695500000));
    //planety.push_back (Planeta (3.3302 * pow (10, 23), 7600551.84, 4.47188260946, 5.7909176 * pow (10, 10), 2439500));
    //planety.push_back (Planeta (4.8685 * pow (10, 24), 19414162.944, 3.83379023493, 1.08208926 * pow (10, 11), 6052000));
    planety.push_back (Planeta (5.9721 * pow (10, 24), 31558149.7635, 3.12064870257, 1.49597870 * pow (10, 11), 6378000));
    //planety.push_back (Planeta (6.4185 * pow (10, 23), 59353352.64, 3.31019145933, 2.27936637 * pow (10, 11), 3402500));
    //planety.push_back (Planeta (1.8986 * pow (10, 27), 374395970.88, 1.9455185172, 7.7841202 * pow (10, 11), 71492000));
    //planety.push_back (Planeta (5.6846 * pow (10, 26), 929620800, 3.98615747863, 1.426725413 * pow (10, 12), 60268000));
    //planety.push_back (Planeta (8.6832 * pow (10, 25), 2653185024, 0.21502456384, 2.870972220  * pow (10, 12), 25559000));
    //planety.push_back (Planeta (1.0244 * pow (10, 26), 5203297440, 5.85016911976, 4.498252900  * pow (10, 12), 24764000));
    satelity.push_back (Satelita (7.347673 * pow (10, 22), 2360591.5104, 0.3 * pi, 384400000, 1737064, &planety [0]));
}

void Laduj_Guziki ()
{
    buttons.push_back (new Button (50, 50, 50, 50, "Minus.png"));
    buttons.push_back (new Button (150, 50, 50, 50, "Plus.png"));
    buttons.push_back (new Button (50, 150, 50, 50, "Minus.png"));
    buttons.push_back (new Button (150, 150, 50, 50, "Plus.png"));
    buttons.push_back (new Button (50, 250, 50, 50, "Minus.png"));
    buttons.push_back (new Button (150, 250, 50, 50, "Plus.png"));
    buttons.push_back (new Button (50, 300, 50, 50, "Plus.png"));
    buttons.push_back (new Button (50, 350, 50, 50, "Plus.png"));
    buttons.push_back (new Button (50, 400, 50, 50, "Plus.png"));
}

std::vector <sf::Vector2f> punkty;
std::vector <double> katy;
std::vector <double> czasy;

void Symulacja (int czas)
{
    bool zderzenie = false;
    punkty.clear ();
    katy.clear ();
    czasy.clear ();
    punkty.push_back (sf::Vector2f (FK_Rakieta.koordynata_x, FK_Rakieta.koordynata_y));
    katy.push_back (FK_Rakieta.pozycja_katowa);
    czasy.push_back (0);
    for (int j = 0; j < planety.size (); j++) planety [j].Aktualizacja ();
    for (long i = 0; i < czas && !zderzenie; i++)
    {
        for (int j = 0; j < satelity.size (); j++) satelity [j].Aktualizacja ();
        if (sqrt (pow (FK_Rakieta.koordynata_x - planety [0].koordynata_x, 2) + pow ( FK_Rakieta.koordynata_y - planety [0].koordynata_y, 2)) < planety [0].promien_planety) zderzenie = true;
        FK_Rakieta.Aktualizacja ();
        if (pow ((FK_Rakieta.koordynata_x - punkty [punkty.size () - 1].x), 2) + pow ((FK_Rakieta.koordynata_y - punkty [punkty.size () - 1].y), 2) > 1000000000000)
        {
            punkty.push_back (sf::Vector2f (FK_Rakieta.koordynata_x, FK_Rakieta.koordynata_y));
            katy.push_back (FK_Rakieta.pozycja_katowa);
            czasy.push_back (i);
        }
    }
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

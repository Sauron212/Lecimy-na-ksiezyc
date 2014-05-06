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
            pozycja_katowa += omega_obiegu;
            if (pozycja_katowa > 2 * pi) pozycja_katowa = 0;
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

class Rakieta
{
    public:
        double koordynata_x;
        double koordynata_y;
        double masa;
        double pozycja_katowa;
        double ciag;
        sf::Vector2f Fg;
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
            pozycja_katowa = pi / 2;
            ciag = 900;
            image.loadFromFile ("Rakieta.png");
            image.setSmooth (true);
            sprite.setTexture (image);
            sprite.setOrigin (187.5, 150);
            sprite.setScale (0.02, 0.02);
        }
        void Aktualizacja (int czas)
        {
            Fg = sf::Vector2f (0, 0);
            if (czasy_silnika.size () > current_index)
            {
                if (czas > czasy_silnika [current_index].start && czas < czasy_silnika [current_index].start + czasy_silnika [current_index].czas_trwania) Fg += sf::Vector2f (cos (pozycja_katowa) * ciag, sin (pozycja_katowa) * ciag);
            }
            for (int i = 0; i < planety.size (); i++) Fg += Grawitacja (koordynata_x, koordynata_y, masa, planety [i].koordynata_x, planety [i].koordynata_y, planety [i].masa);
            for (int i = 0; i < satelity.size (); i++) Fg += Grawitacja (koordynata_x, koordynata_y, masa, satelity [i].koordynata_x, satelity [i].koordynata_y, satelity [i].masa);
            a = sf::Vector2f (Fg.x / masa, Fg.y / masa);
            v += sf::Vector2f (a.x, a.y);
            koordynata_x += v.x;
            koordynata_y += v.y;
            pozycja_katowa = atan2 (v.y, v.x);
            if (pozycja_katowa < 0) pozycja_katowa += 2 * pi;
        }
};

Rakieta FK_Rakieta (0, -6555074.56, 100);

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
    ziemia.loadFromFile ("Ziemia.png");
    ksiezyc.loadFromFile ("Ksiezyc.png");
    //planety.push_back (Planeta (5.9721 * pow (10, 24), 31558149.7635, 3.12064870257, 1.49597870 * pow (10, 11), 6378000));
    planety.push_back (Planeta (5.9721 * pow (10, 24), 31558149.7635, 24 * 3600, 3.12064870257, 0, 6378000));
    satelity.push_back (Satelita (7.347673 * pow (10, 22), 2360591.5104, 2360591.5104, 0.3 * pi, 384400000, 1737064, &planety [0]));    Czas cz;
    cz.czas_trwania = 700;
    cz.start = 0;
    czasy_silnika.push_back (cz);
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
    for (long i = 0; i < czas && !zderzenie; i++)
    {
        for (int j = 0; j < planety.size (); j++) planety [j].Aktualizacja ();
        for (int j = 0; j < satelity.size (); j++) satelity [j].Aktualizacja ();
        if (sqrt (pow (FK_Rakieta.koordynata_x - planety [0].koordynata_x, 2) + pow ( FK_Rakieta.koordynata_y - planety [0].koordynata_y, 2)) < planety [0].promien_planety) zderzenie = true;
        if (i > czasy_silnika [current_index].start + czasy_silnika [current_index].czas_trwania && current_index < czasy_silnika.size ()) current_index += 1;
        FK_Rakieta.Aktualizacja (i);
        if (pow ((FK_Rakieta.koordynata_x - punkty [punkty.size () - 1].x), 2) + pow ((FK_Rakieta.koordynata_y - punkty [punkty.size () - 1].y), 2) > 1000000000000)
        {
            punkty.push_back (sf::Vector2f (FK_Rakieta.koordynata_x, FK_Rakieta.koordynata_y));
            katy.push_back (FK_Rakieta.pozycja_katowa);
            czasy.push_back (i);
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

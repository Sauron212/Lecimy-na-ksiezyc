#ifndef FAZA_KOSMOSU_H_INCLUDED
#define FAZA_KOSMOSU_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <fstream>
std::ofstream dx ("dx.txt");

sf::Vector2f Grawitacja (double x_1, double y_1, double masa_1, double x_2, double y_2, double masa_2)
{
    double Fg = masa_1 * masa_2 / (pow ((x_2 - x_1), 2) + pow ((y_2 - y_1), 2));
    sf::Vector2f wektor_grawitacji (Fg / sqrt (pow ((x_2 - x_1), 2) + pow ((y_2 - y_1), 2)) * (x_2 - x_1), Fg / sqrt (pow ((x_2 - x_1), 2) + pow ((y_2 - y_1), 2)) * (y_2 - y_1));
    return wektor_grawitacji;
}

class Obiekt_Kosmiczny
{
    public:
        int ID;
        double koordynata_x;
        double koordynata_y;
        double masa;
        sf::Vector2f Fg;
        sf::Vector2f a;
        sf::Vector2f v;
        Obiekt_Kosmiczny (int c_ID, double c_x, double c_y, double c_masa)
        {
            ID = c_ID;
            koordynata_x = c_x;
            koordynata_y = c_y;
            masa = c_masa;
            Fg.x = 0;
            Fg.y = 0;
            a.x = 0;
            a.y = 0;
            v.x = 0;
            v.y = 0;
        }
        void Aktualizacja ();
};

class Planeta : public Obiekt_Kosmiczny
{
    public:
        double promien;
        sf::CircleShape grafika;
        Planeta (int c_ID, double c_x, double c_y, double c_masa, double c_promien) : Obiekt_Kosmiczny (c_ID, c_x, c_y, c_masa)
        {
            promien = c_promien;
            grafika.setRadius (promien);
            grafika.setPosition (koordynata_x - promien, koordynata_y - promien);
        }
};

std::vector <Planeta> planety;

class Rakieta : public Obiekt_Kosmiczny
{
    public:
        Rakieta (int c_ID, double c_x, double c_y, double c_masa) : Obiekt_Kosmiczny (c_ID, c_x, c_y, c_masa)
        {

        }
};

void Laduj_Uklad ()
{
    planety.push_back (Planeta (1, 100, 100, 200, 100));
    planety [0].v.x = 10.5409255339;
    planety.push_back (Planeta (2, 100, 1000, 100000, 100));
}

void Obiekt_Kosmiczny::Aktualizacja ()
{
    Fg.x = 0;
    Fg.y = 0;
    a.x = 0;
    a.y = 0;
    for (int i = 0; i < planety.size (); i++) if (planety [i].ID != ID) Fg += Grawitacja (koordynata_x, koordynata_y, masa, planety [i].koordynata_x, planety [i].koordynata_y, planety [i].masa);
    a.x += Fg.x / masa;
    a.y += Fg.y / masa;
    v += a;
    koordynata_x += v.x;
    koordynata_y += v.y;
}

#endif // FAZA_KOSMOSU_H_INCLUDED

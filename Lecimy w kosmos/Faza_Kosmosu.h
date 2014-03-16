#ifndef FAZA_KOSMOSU_H_INCLUDED
#define FAZA_KOSMOSU_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <fstream>

sf::Vector2f Grawitacja (double x_1, double y_1, double masa_1, double x_2, double y_2, double masa_2)
{
    double Fg = masa_1 * masa_2 / (pow ((x_2 - x_1), 2) + pow ((y_2 - y_1), 2));
    sf::Vector2f wektor_grawitacji (Fg / sqrt (pow ((x_2 - x_1), 2) + pow ((y_2 - y_1), 2)) * (x_2 - x_1), Fg / sqrt (pow ((x_2 - x_1), 2) + pow ((y_2 - y_1), 2)) * (y_2 - y_1));
    return wektor_grawitacji;
}

class Planeta
{
    public:
        double koordynata_x;
        double koordynata_y;
        double masa;
        double promien;
        sf::CircleShape grafika;
        Planeta (double c_x, double c_y, double c_masa, double c_promien)
        {
            koordynata_x = c_x;
            koordynata_y = c_y;
            masa = c_masa;
            promien = c_promien;
            grafika.setRadius (promien);
            grafika.setPosition (koordynata_x, koordynata_y);
        }
};

std::vector <Planeta> planety;

class Rakieta
{
    public:
        double koordynata_x;
        double koordynata_y;
        double masa;
        sf::Vector2f Fg;
        sf::Vector2f a;
        sf::Vector2f v;
        Rakieta (double c_x, double c_y, double c_masa)
        {
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
        void Aktualizacja ()
        {
            for (int i = 0; i < planety.size (); i++) Fg += Grawitacja (koordynata_x, koordynata_y, masa, planety [i].koordynata_x, planety [i].koordynata_y, planety [i].masa);
            a.x += Fg.x / masa;
            a.y += Fg.y / masa;
            v += a;
            koordynata_x += v.x;
            koordynata_y += v.y;
        }
};

void Laduj_Uklad ()
{
    planety.push_back (Planeta (100, 100, 200, 100));
    planety.push_back (Planeta (100, 1000, 10000, 100));
}

#endif // FAZA_KOSMOSU_H_INCLUDED

#ifndef FAZA_KOSMOSU_H_INCLUDED
#define FAZA_KOSMOSU_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <fstream>
std::ofstream dx ("dx.txt");

sf::Vector2f Grawitacja (double x_1, double y_1, double masa_1, double x_2, double y_2, double masa_2)
{
    double Fg = 6.67 * masa_1 * masa_2 / (pow ((x_2 - x_1), 2) + pow ((y_2 - y_1), 2)) * pow (10, -11);
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
    planety.push_back (Planeta (1, 0, 0, 1989100000000000, 70));
    planety.push_back (Planeta (2, 57.909176, 0, 333020000, 20));
    planety [1].v.y = 47.87; // * pow (10, -6);
    planety.push_back (Planeta (3, 108.208926, 0, 4868500000, 20));
    planety [2].v.y = 35.020; // * pow (10, -6);
    planety.push_back (Planeta (4, 149.597870, 0, 5972100000, 20));
    planety [3].v.y = 29.783; //* pow (10, -6);
    planety.push_back (Planeta (5, 227.936637, 0, 641850000, 20));
    planety [4].v.y = 24.13; //* pow (10, -6);
    planety.push_back (Planeta (6, 778.412020, 0, 1898600000000, 20));
    planety [5].v.y = 13.07; //* pow (10, -6);
    planety.push_back (Planeta (7, 1426.725413, 0, 568460000000, 40));
    planety [6].v.y = 9.638;//* pow (10, -6);
    planety.push_back (Planeta (8, 2870.972220, 0, 86832000000, 40));
    planety [7].v.y = 6.795; //* pow (10, -6);
    planety.push_back (Planeta (9, 4498.252900, 0, 102440000000, 40));
    planety [8].v.y = 5.478; //* pow (10, -6);
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

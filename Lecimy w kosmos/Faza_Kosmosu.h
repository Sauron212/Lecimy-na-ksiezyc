#ifndef FAZA_KOSMOSU_H_INCLUDED
#define FAZA_KOSMOSU_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>

const double pi = atan (1) * 4;
int modyfikator = 864000;
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
        double masa;
        double omega;
        double pozycja_katowa;
        double promien_orbity;
        double promien_planety;
        sf::CircleShape grafika;
        sf::Clock stoper;
        sf::Time czas;
        sf::Time t2;
        Planeta (double c_masa, double c_T, double c_pozycja_katowa, double c_promien_orbity, double c_promien_planety)
        {
            masa = c_masa;
            if (c_T != 0) omega = 2 * pi / c_T;
            else omega = 0;
            pozycja_katowa = 0;
            promien_orbity = c_promien_orbity;
            promien_planety = c_promien_planety;
            grafika.setRadius (promien_planety);
            grafika.setPosition (cos (pozycja_katowa) * promien_orbity - promien_planety, sin (pozycja_katowa) * promien_orbity - promien_planety);
            czas = sf::Time::Zero;
            t2 = sf::Time::Zero;
        }
        void Aktualizacja ()
        {
            pozycja_katowa += omega * czas.asSeconds () * modyfikator;
            czas = stoper.getElapsedTime ();
            t2 += czas;
            stoper.restart ();
            if (pozycja_katowa > 2 * pi)
            {
                pozycja_katowa = 0;
                dx<<masa<<":"<<t2.asSeconds ()<<std::endl;
            }
            grafika.setPosition (cos (pozycja_katowa) * promien_orbity - promien_planety, sin (pozycja_katowa) * promien_orbity - promien_planety);
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
        Rakieta (int c_ID, double c_x, double c_y, double c_masa)
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
        void Aktualizacja (double czas)
        {
            Fg.x = 0;
            Fg.y = 0;
            a.x = 0;
            a.y = 0;
            a.x += Fg.x / masa;
            a.y += Fg.y / masa;
            v += a;
            koordynata_x += v.x * czas * modyfikator;
            koordynata_y += v.y * czas * modyfikator;
        }
};

void Laduj_Uklad ()
{
    planety.push_back (Planeta (1989100000000000, 0, 0, 0, 70));
    planety.push_back (Planeta (333020000, 7600551.84, 4.47188260946, 57.909176, 20));
    planety.push_back (Planeta (4868500000, 19414162.944, 3.83379023493, 108.208926, 20));
    planety.push_back (Planeta (5972100000, 31558149.7635, 3.12064870257, 149.597870, 20));
    planety.push_back (Planeta (641850000, 59353352.64, 3.31019145933, 227.936637, 20));
    planety.push_back (Planeta (1898600000000, 374395970.88, 1.9455185172, 778.412020, 20));
    planety.push_back (Planeta (568460000000, 929620800, 3.98615747863, 1426.725413, 40));
    planety.push_back (Planeta (86832000000, 2653185024, 0.21502456384, 2870.972220, 40));
    planety.push_back (Planeta (102440000000, 5203297440, 5.85016911976, 4498.252900, 40));
}

#endif // FAZA_KOSMOSU_H_INCLUDED

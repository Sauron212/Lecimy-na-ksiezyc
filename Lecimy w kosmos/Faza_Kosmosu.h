#ifndef FAZA_KOSMOSU_H_INCLUDED
#define FAZA_KOSMOSU_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <cmath>

const double pi = atan (1) * 4;

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
        Planeta (double c_masa, double c_T, double c_pozycja_katowa, double c_promien_orbity, double c_promien_planety)
        {
            masa = c_masa;
            if (c_T != 0) omega = 2 * pi / c_T;
            else omega = 0;
            pozycja_katowa = 0;
            promien_orbity = c_promien_orbity;
            promien_planety = c_promien_planety;
            grafika.setRadius (promien_planety / pow (10, 6));
            grafika.setPosition (cos (pozycja_katowa) * promien_orbity / pow (10, 6) - grafika.getRadius (), -sin (pozycja_katowa) * promien_orbity / pow (10, 6) - grafika.getRadius ());
        }
        void Aktualizacja ()
        {
            pozycja_katowa += omega;
            if (pozycja_katowa > 2 * pi) pozycja_katowa = 0;
            grafika.setPosition (cos (pozycja_katowa) * promien_orbity / pow (10, 6) - grafika.getRadius (), -sin (pozycja_katowa) * promien_orbity / pow (10, 6) - grafika.getRadius ());
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
        sf::CircleShape grafika;
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
            grafika.setRadius (2);
            grafika.setFillColor (sf::Color::Yellow);
        }
        void Aktualizacja ()
        {
            Fg.x = 0;
            Fg.y = 0;
            a.x = 0;
            a.y = 0;
            for (int i = 0; i < planety.size (); i++) Fg += Grawitacja (koordynata_x, koordynata_y, masa, cos (planety [i].pozycja_katowa) * planety [i].promien_orbity, sin (planety [i].pozycja_katowa) * planety [i].promien_orbity, planety [i].masa);
            a.x += Fg.x / masa;
            a.y += Fg.y / masa;
            v.x += a.x;
            v.y += a.y;
            koordynata_x += v.x;
            koordynata_y += v.y;
            grafika.setPosition (koordynata_x / pow (10, 6) - 2, -koordynata_y / pow (10, 6) - 2);
        }
};

Rakieta FK_Rakieta (1.49597870 * pow (10, 11) - 42231860.82, 0, 100);

void Laduj_Uklad ()
{
    planety.push_back (Planeta (1.9891 * pow (10, 30), 0, 0, 0, 695500000));
    planety.push_back (Planeta (3.3302 * pow (10, 23), 7600551.84, 4.47188260946, 5.7909176 * pow (10, 10), 2439500));
    planety.push_back (Planeta (4.8685 * pow (10, 24), 19414162.944, 3.83379023493, 1.08208926 * pow (10, 11), 6052000));
    planety.push_back (Planeta (5.9721 * pow (10, 24), 31558149.7635, 3.12064870257, 1.49597870 * pow (10, 11), 6378000));
    planety.push_back (Planeta (6.4185 * pow (10, 23), 59353352.64, 3.31019145933, 2.27936637 * pow (10, 11), 3402500));
    planety.push_back (Planeta (1.8986 * pow (10, 27), 374395970.88, 1.9455185172, 7.7841202 * pow (10, 11), 71492000));
    planety.push_back (Planeta (5.6846 * pow (10, 26), 929620800, 3.98615747863, 1.426725413 * pow (10, 12), 60268000));
    planety.push_back (Planeta (8.6832 * pow (10, 25), 2653185024, 0.21502456384, 2.870972220  * pow (10, 12), 25559000));
    planety.push_back (Planeta (1.0244 * pow (10, 26), 5203297440, 5.85016911976, 4.498252900  * pow (10, 12), 24764000));
}

std::vector <sf::Vertex> punkty;
std::vector <double> czasy;

void Symulacja (int czas)
{
    punkty.clear ();
    czasy.clear ();
    punkty.push_back (sf::Vertex (sf::Vector2f (FK_Rakieta.koordynata_x / 1000000, -FK_Rakieta.koordynata_y  / 1000000)));
    czasy.push_back (0);
    for (long i = 0; i < czas; i++)
    {
        for (int i = 0; i < planety.size () - 1; i++) planety [i].Aktualizacja ();
        FK_Rakieta.Aktualizacja ();
        if (pow ((FK_Rakieta.koordynata_x - punkty [punkty.size () - 1].position.x), 2) + pow ((FK_Rakieta.koordynata_y - punkty [punkty.size () - 1].position.y), 2) > 1000000000000)
        {
            punkty.push_back (sf::Vertex (sf::Vector2f (FK_Rakieta.koordynata_x / 1000000, -FK_Rakieta.koordynata_y  / 1000000)));
            czasy.push_back (i);
        }
    }
}

#endif // FAZA_KOSMOSU_H_INCLUDED

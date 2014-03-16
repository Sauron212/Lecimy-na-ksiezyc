#ifndef FAZA_KOSMOSU_H_INCLUDED
#define FAZA_KOSMOSU_H_INCLUDED

class Planeta
{
    public:
        double koordynata_x;
        double koordynata_y;
        double masa;
        double promien;
        Planeta (double c_x, double c_y, double, double c_masa, double c_promien)
        {
            koordynata_x = c_x;
            koordynata_y = c_y;
            masa = c_masa;
            promien = c_promien;
        }
};

class Rakieta
{
    public:
        double koordynata_x;
        double koordynata_y;
        double masa;
        Rakieta (double c_x, double c_y, double, double c_masa)
        {
            koordynata_x = c_x;
            koordynata_y = c_y;
        }
};

#endif // FAZA_KOSMOSU_H_INCLUDED

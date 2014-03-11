#include <SFML/Graphics.hpp>
#include <cmath>
#include <sstream>
#include <iostream>
using namespace sf;
using namespace std;

void wyswietlanie_danych(sf::Clock czas, long double przyspieszenie, long double predkosc, long double odleglosc, long double kinetyczna);
inline void czcionka(void);

sf::RenderWindow okno(sf::VideoMode(800, 600), "Lecimy w kosmos");
sf::Font font;

int main()
{
    int x{375},y{400}; // odleglosc rakeity od srodka ziemi ; x,y - wspolrzedne rakiety wzgledem okna;

    long double odleglosc{6370};

    bool start=false;                      // ustawienie startu rakiety na klikniecie spacja.

    long double m=2902991.17;      // masa rakiety (kg) na starcie

    const long double G=0.0000000000667; // Stala grawitacji 6,67*10^-11
    const long double Mz=5.9736;          // Masa ziemi (kg), trzeba mnozyc razy 10^24
    long double g=G*Mz/(odleglosc*odleglosc);  // przyspieszenie grawitacyjne
    long double przyspieszenie=0;
    long double predkosc=0;
    long double kinetyczna=0;

    long double F1 = 33850966.49;    // Sila silnikow pierwszego stopnia w N

    sf::Event zdarzenie;
    sf::Clock czas,czas_pod;             // czas - okres ruchu, czas_pod - czas podró¿y

    font.loadFromFile("arial.ttf");                 // ustawienie czcionki

    sf::Texture tlo_tekstura;                       //    Stworzenie tekstury  tlo_tekstura
    tlo_tekstura.loadFromFile("tlo-start.png");

    sf::Sprite tlo;                                 //    Ustawienie Sprite z tekstura tlo_tekstura
    tlo.setTexture(tlo_tekstura);
    tlo.setPosition(0,-7297);

    sf::RectangleShape rakieta(sf::Vector2f(10,110)); //  Stworzenie rakiety, wymiary, pozycja, color itp skala 1:1
    rakieta.setPosition(x,y);
    rakieta.setFillColor(sf::Color(0,0,0));

    while(okno.isOpen())
    {
        while(odleglosc <= 106370)
        {
            while(okno.pollEvent(zdarzenie))
            {
                if( zdarzenie.type == sf::Event::Closed )
                 okno.close();
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                 start=true;
            }
            if(start==false)                // Zerowanie czasu jak jest na ziemii
                czas_pod.restart();
            if(czas.getElapsedTime().asMilliseconds() >= 1 && start)
            {
                g=G*Mz/(odleglosc*odleglosc);
                przyspieszenie =((F1 - m*g)/m)/1000000;
                predkosc = przyspieszenie*czas_pod.getElapsedTime().asMilliseconds();
                kinetyczna=0.5*m*predkosc*predkosc;

                if(rakieta.getPosition().y>=300 )   // ustawienie rakiety na srodku ekranu
                    rakieta.move(0,-predkosc);
                else
                    tlo.move(0,predkosc);   // poruszanie sie tla
                odleglosc+=predkosc;
                czas.restart();
            }
            okno.clear(sf::Color(255,255,255));
            okno.draw(tlo);
            if(odleglosc>17000 && odleglosc<22000)                  // Wyswietlanie komunikatu o sferach
            {
                sf::Text stratosfera("Weszlismy w STRATOSFERE", font, 18);
                stratosfera.setPosition(300,400);
                stratosfera.setColor(sf::Color::Red);
                okno.draw(stratosfera);
            }
            if(odleglosc>55000 && odleglosc<60000)
            {
                sf::Text mezosfera("Weszlismy w MEZOSFERE", font, 18);
                mezosfera.setPosition(300,400);
                mezosfera.setColor(sf::Color::Red);
                okno.draw(mezosfera);
            }
            okno.draw(rakieta);
            wyswietlanie_danych(czas_pod,przyspieszenie,predkosc,odleglosc,kinetyczna);
            okno.display();
        }
        while(odleglosc > 106370)
        {
             while(okno.pollEvent(zdarzenie))
            {
                if( zdarzenie.type == sf::Event::Closed )
                 okno.close();
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                 start=true;

            }
            okno.display();
        }
    }
    return 0;
}
void wyswietlanie_danych(sf::Clock czas, long double przyspieszenie, long double predkosc, long double odleglosc, long double kinetyczna)
{
    ostringstream ss;                       // potrzebne do konwersji z inta/clocka na stringa
    sf::Time czas2 = czas.getElapsedTime();
    int a = czas2.asSeconds();

            int godziny = a/3600;
            int minuty = (a-godziny*3600)/60;
            int sekundy = a-godziny*3600-minuty*60;

    ss<< godziny << " h " << minuty << " min " << sekundy << " sekund";
    string czas3 = ss.str();                    // przekazanie ss do zmiennej string
    sf::Text czas_wys(czas3, font, 20);
    czas_wys.setColor((sf::Color::Black));
    czas_wys.setPosition(400, 480);
    ss.str("");
    ss<<"Przyspieszenie "<<przyspieszenie*1000000<<"m/s^2";
    string przyspieszenie_w = ss.str();
    sf::Text przyspieszenie_wys(przyspieszenie_w, font,20);
    przyspieszenie_wys.setColor((sf::Color::Black));
    przyspieszenie_wys.setPosition(400,500);
    ss.str("");
    ss<<"Predkosc "<<predkosc*1000<<"m/s";
    string predkosc_w = ss.str();
    sf::Text predkosc_wys(predkosc_w, font,20);
    predkosc_wys.setColor((sf::Color::Black));
    predkosc_wys.setPosition(400,520);
    ss.str("");
    ss<<"Wysokosc nad poziomem morza "<<odleglosc-6370<<"m";
    string wysokosc_w = ss.str();
    sf::Text wysokosc_wys(wysokosc_w, font,20);
    wysokosc_wys.setColor((sf::Color::Black));
    wysokosc_wys.setPosition(400,540);
    ss.str("");
    if(kinetyczna<=1000000)
        ss<<"Energia kinetyczna "<<kinetyczna<<"J";
    else
        ss<<"Energia kinetyczna "<<kinetyczna/1000000<<"MJ";
    string kinetyczna_w = ss.str();
    sf::Text kinetyczna_wys(kinetyczna_w, font,20);
    kinetyczna_wys.setColor((sf::Color::Black));
    kinetyczna_wys.setPosition(400,560);


    okno.draw(wysokosc_wys);                    // wyswietlenie wysokosci
    okno.draw(predkosc_wys);                    // wyswietlenie predkosci
    okno.draw(przyspieszenie_wys);              // wyswietlenie przyspieszenia
    okno.draw(czas_wys);                        // wyswietlenie czasu
    okno.draw(kinetyczna_wys);                  // wyswietlenie energii kinetycznej
    }

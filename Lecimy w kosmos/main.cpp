#include <SFML/Graphics.hpp>
#include <cmath>
#include <sstream>
#include <iostream>
using namespace sf;
using namespace std;

void czas_podrozy(sf::Clock czas);
inline void czcionka(void);

sf::RenderWindow okno(sf::VideoMode(800, 600), "Lecimy w kosmos");
sf::Font font;

int main()
{
    int odleglosc{6370},x{375},y{400}; // odleglosc rakeity od srodka ziemi ; x,y - wspolrzedne rakiety wzgledem okna;

    bool start=false;                      // ustawienie startu rakiety na klikniecie spacja.

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
                if(rakieta.getPosition().y>=300 )   // ustawienie rakiety na srodku ekranu
                    rakieta.move(0,-1);
                else
                    tlo.move(0,1);   // poruszanie sie tla

                odleglosc+=14;
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
            czas_podrozy(czas_pod);
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
void czas_podrozy(sf::Clock czas)
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
    czas_wys.setPosition(600, 500);
    okno.draw(czas_wys);                        // wyswietlenie czasu
}

#include <SFML/Graphics.hpp>
#include <cmath>
#include <sstream>
#include <iostream>
#include "Faza_Kosmosu.h"
using namespace sf;
using namespace std;

void wyswietlanie_danych(sf::Clock czas, long double przyspieszenie, long double predkosc, long double odleglosc, long double kinetyczna);
inline void czcionka(void);
    sf::ContextSettings settings( 0,0,8,2,0 );
sf::RenderWindow okno(sf::VideoMode(800, 600), "Lecimy w kosmos",sf::Style::Default,settings);
sf::Font font;


int main()
{
    font.loadFromFile("arial.ttf");                 // ustawienie czcionki
    int menu=0;
    sf::Text ziemiaT("ZIEMIA", font,20);
    sf::Text kosmosT("KOSMOS", font,20);
    sf::Text wyjscieT("WYJSCIE", font,20);
    ziemiaT.setColor((sf::Color::Black));
    kosmosT.setColor((sf::Color::Black));
    wyjscieT.setColor((sf::Color::Black));
    ziemiaT.setPosition(300,200);
    kosmosT.setPosition(300,250);
    wyjscieT.setPosition(300,300);
    /* ZMIENNE GRUPY GRZESIA : */
    int x{394},y{400}; // odleglosc rakeity od srodka ziemi ; x,y - wspolrzedne rakiety wzgledem okna;

    long double odleglosc = 6371000; // Faza Kosmosu -> 106370001   Faza Ziemska -> 6371000

    bool start=false;                      // ustawienie startu rakiety na klikniecie spacja.

    long double m=2902991.17;      // masa rakiety (kg) na starcie

    const long double G=0.0000000000667; // Stala grawitacji 6,67*10^-11
    const long double Mz=5.9736;          // Masa ziemi (kg), trzeba mnozyc razy 10^24
    long double g=G*Mz*pow(10,24)/(odleglosc*odleglosc);  // przyspieszenie grawitacyjne
    long double przyspieszenie=0;               // przyspieszenie rakiety (ciag silnikow - grawitacja - opor powietrza)
    long double predkosc=0;                     // predkosc z jaka porusza sie rakieta
    long double kinetyczna=0;                   // Energia kinetyczna
    double pole = 112.97;                       // pole jakie jest brane pod uwage w oporze
    double Cx = 0.3;                          // wspó³czynnik si³y oporu


    long double F1 = 39428858.47;                    // Sila silnikow pierwszego stopnia w N
    long double opor = Cx*1.1717*predkosc*predkosc*pole/2;      // opor powietrza

    sf::Event zdarzenie;
    sf::Clock czas,czas_pod, czas_rotacja;             // czas - okres ruchu, czas_pod - czas podró¿y, czas_rotacja - zmiana rotacji co sekudne;

    sf::Texture tlo_tekstura;                       //    Stworzenie tekstury  tlo_tekstura
    tlo_tekstura.loadFromFile("tlo-start.png");

    sf::Sprite tlo;                                 //    Ustawienie Sprite z tekstura tlo_tekstura
    tlo.setTexture(tlo_tekstura);
    tlo.setPosition(0,-7297);


    sf::RectangleShape rakieta(sf::Vector2f(11,111)); //  Stworzenie rakiety, wymiary, pozycja, color itp skala 1:1
    rakieta.setPosition(x,y);
    rakieta.setFillColor(sf::Color(0,0,0));
    sf::RectangleShape prostokat(sf::Vector2f(0.12*okno.getSize().x-5,0.12*okno.getSize().y-5));
        prostokat.setPosition(0.86*okno.getSize().x-3,0.02*okno.getSize().y-2);
        prostokat.setFillColor(sf::Color(0,0,0));
            sf::View minimapa;
            minimapa.setViewport(sf::FloatRect(0.86f, 0.02, 0.12f, 0.15f));
            minimapa.setSize(500,500);
            minimapa.zoom(2);


            sf::View mapa; //stwoerzenie widoku mapy
            mapa.reset(sf::FloatRect(0,0,800, 600));
            mapa.setViewport(sf::FloatRect(0, 0, 1, 1));



    /* ZMIENNE GRUPY JARKA : */
    Rakieta KRakieta (1.49597870 * pow (10, 11) - 6378000, 0, 100);
    KRakieta.v.y = 7910;
    Laduj_Uklad ();

    sf::Texture Ktlo_tekstura;
    Ktlo_tekstura.loadFromFile("Ktlo.png");
    sf::Sprite Ktlo;
    Ktlo.setTexture(Ktlo_tekstura);
    Ktlo.setPosition(KRakieta.koordynata_x / pow (10, 6) - 400, KRakieta.koordynata_y / pow (10, 6) - 300);

    double Ktangens = ( 2000 - 100 ) / ( 2000 - 100 );

    double Kkatrakiety;
    Kkatrakiety = atan ( Ktangens ) * 180 / 3.14;

    bool Kstart = false;

    sf::View klip;
    klip.reset (sf::FloatRect (0, 0, 8000, 6000));
    klip.setViewport (sf::FloatRect (0.0f, 0.0f, 1.0f, 1.0f));

    sf::RectangleShape Krakieta( sf::Vector2f ( 5, 30 ) ); //  Stworzenie rakiety w Kosmosie, wymiary, pozycja, color itp
    Krakieta.setPosition(KRakieta.koordynata_x / pow (10, 9), KRakieta.koordynata_y / pow (10, 9));
    Krakieta.setFillColor( sf::Color::Yellow );
    Krakieta.setRotation( Kkatrakiety + 90 );

    while(okno.isOpen())
    {
        while(okno.isOpen () && menu==0)
        {
            sf::Vector2i myszka2 = sf::Mouse::getPosition(okno);
            sf::FloatRect myszka;
            myszka.left = myszka2.x;
            myszka.top=myszka2.y;
            myszka.height=myszka.width=1;

            sf::FloatRect ziemia = ziemiaT.getGlobalBounds();
            sf::FloatRect kosmos = kosmosT.getGlobalBounds();
            sf::FloatRect wyjscie = wyjscieT.getGlobalBounds();
            while(okno.pollEvent(zdarzenie))
            {
                if( zdarzenie.type == sf::Event::Closed )
                    okno.close();
                if(myszka.intersects(ziemia) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    menu=1;
                if(myszka.intersects(kosmos) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    menu=2;
                if(myszka.intersects(wyjscie) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    okno.close();
            }
            okno.clear(sf::Color(255,255,255));
            okno.draw(ziemiaT);
            okno.draw(kosmosT);
            okno.draw(wyjscieT);
            okno.display();
        }
        while(okno.isOpen ()&& (odleglosc <= 6471000 && menu==1))
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
                g=G*Mz*pow(10,24)/(odleglosc*odleglosc);                // uatkualnienie g
                if(czas_pod.getElapsedTime().asSeconds()-6<0)// uatkualnie masy(spalanie paliwa)DOPOPRAWKI
                    m-=5876.59/1000;
                if(czas_pod.getElapsedTime().asSeconds()-6>0 && czas_pod.getElapsedTime().asSeconds()-6<=70 )
                    m-=13169.063/1000;

                if(czas_rotacja.getElapsedTime().asMilliseconds() >= 1) // Co sekundę...
                {
                    if(czas_pod.getElapsedTime().asSeconds()-6>=30 && czas_pod.getElapsedTime().asSeconds()-6<80) // ...w wyznaczonym czasie (dodanie 6 sekund z powodu opóźnionego startu)...
                    rakieta.rotate(0.7280000/1000);//...obrót rakiety o tyle stopni

                    else if(czas_pod.getElapsedTime().asSeconds()-6>=80 && czas_pod.getElapsedTime().asSeconds()-6<135)
                    rakieta.rotate(0.4696364/1000);

                    else if(czas_pod.getElapsedTime().asSeconds()-6>=135 && czas_pod.getElapsedTime().asSeconds()-6<165)
                    rakieta.rotate(0.2970000/1000);
                    czas_rotacja.restart();
                }

                opor = Cx*1.1717*predkosc*predkosc*pole/2;          // uatkualnienie oporu powietrza
                przyspieszenie =((F1 - m*g-opor)/m)/1000000;                // obliczanie przyspieszenia
                if(czas_pod.getElapsedTime().asSeconds()-6>=0)
                {
                    int t = czas.getElapsedTime().asMilliseconds();
                    predkosc += przyspieszenie*t; // obliczanie predkosci
                    kinetyczna=0.5*m*predkosc*predkosc;                                          // obliczanie Ek

                    if(rakieta.getPosition().y<244 )   // ustawienie rakiety na srodku ekranu
                    {
                    mapa.setCenter(rakieta.getPosition().x+6,rakieta.getPosition().y+56);//podązanie za rakietą
                    }
                    double radiany=(rakieta.getRotation()*pi)/180.0;
                    rakieta.move(predkosc*sin(radiany), -predkosc*cos(radiany));
                    odleglosc+=predkosc*t;
                }
                czas.restart();
            }
            okno.clear(sf::Color(255,255,255));
            okno.draw(tlo);

            okno.setView(okno.getDefaultView());
                okno.draw(prostokat);

            okno.setView(minimapa);                 // minimapa
                okno.draw(tlo);                     // to znajduje sie na minimapie
                okno.draw(rakieta);                 // to znajduje sie na minimapie

            okno.setView(okno.getDefaultView());
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
            okno.setView(mapa);
                okno.draw(rakieta);

            okno.setView(okno.getDefaultView());
                wyswietlanie_danych(czas_pod,przyspieszenie,predkosc,odleglosc,kinetyczna);

            okno.setView(mapa);
            okno.display();
        }
        while(okno.isOpen ()&&(odleglosc > 6471000 || menu==2))
        {
            while( okno.pollEvent ( zdarzenie ) )
            {
                if (zdarzenie.type == sf::Event::Closed) okno.close ();
                if (sf::Keyboard::isKeyPressed (sf::Keyboard::Space))
                {
                    Kstart = true;
                    KRakieta.stoper.restart ();
                    for (int i = 0; i < planety.size (); i++) planety [i].stoper.restart ();
                }
                if (sf::Keyboard::isKeyPressed (sf::Keyboard::Q))
                {
                    okno.close ();
                    dx.close ();
                }
                if (sf::Keyboard::isKeyPressed (sf::Keyboard::S)) klip.setSize (klip.getSize () + sf::Vector2f (12, 9));
                if (sf::Keyboard::isKeyPressed (sf::Keyboard::W)) klip.setSize (klip.getSize () + sf::Vector2f (-12, -9));
            }
            if (Kstart)
            {
                KRakieta.Aktualizacja ();
            }
            Krakieta.setPosition (KRakieta.koordynata_x / pow (10, 6), KRakieta.koordynata_y / pow (10, 6));
            klip.setCenter (KRakieta.koordynata_x / pow (10, 6), -KRakieta.koordynata_y / pow (10, 6));
            okno.setView (klip);
            okno.clear (sf::Color::Black);
            okno.draw (Ktlo);
            for (int i = 0; i < planety.size (); i++)
            {
                okno.draw (planety [i].grafika);
            }
            okno.draw (KRakieta.grafika);
            okno.display ();
        }
    }
    return 0;
}
void wyswietlanie_danych(sf::Clock czas, long double przyspieszenie, long double predkosc, long double odleglosc, long double kinetyczna)
{
    ostringstream ss;                       // potrzebne do konwersji z inta/clocka na stringa
    sf::Time czas2 = czas.getElapsedTime();
    int a = czas2.asSeconds();
            a-=6;
            int godziny = a/3600;
            int minuty = (a-godziny*3600)/60;
            int sekundy = a-godziny*3600-minuty*60;
    ss<< godziny << " h " << minuty << " min " << sekundy << " sekund";
    string czas3 = ss.str();                    // przekazanie ss do zmiennej string
    sf::Text czas_wys(czas3, font, 20);
    czas_wys.setColor((sf::Color::Black));
    czas_wys.setPosition(410, 480);
    ss.str("");
    ss<<"Przyspieszenie "<<przyspieszenie*1000000<<"m/s^2";
    string przyspieszenie_w = ss.str();
    sf::Text przyspieszenie_wys(przyspieszenie_w, font,20);
    przyspieszenie_wys.setColor((sf::Color::Black));
    przyspieszenie_wys.setPosition(410,500);
    ss.str("");
    ss<<"Predkosc "<<predkosc*1000<<"m/s";
    string predkosc_w = ss.str();
    sf::Text predkosc_wys(predkosc_w, font,20);
    predkosc_wys.setColor((sf::Color::Black));
    predkosc_wys.setPosition(410,520);
    ss.str("");
    ss<<"Wysokosc nad poziomem morza "<<odleglosc-6371000<<"m";
    string wysokosc_w = ss.str();
    sf::Text wysokosc_wys(wysokosc_w, font,20);
    wysokosc_wys.setColor((sf::Color::Black));
    wysokosc_wys.setPosition(410,540);
    ss.str("");
    if(kinetyczna<=1000000)
        ss<<"Energia kinetyczna "<<kinetyczna<<"J";
    else
        ss<<"Energia kinetyczna "<<kinetyczna/1000000<<"MJ";
    string kinetyczna_w = ss.str();
    sf::Text kinetyczna_wys(kinetyczna_w, font,20);
    kinetyczna_wys.setColor((sf::Color::Black));
    kinetyczna_wys.setPosition(410,560);


    okno.draw(wysokosc_wys);                    // wyswietlenie wysokosci
    okno.draw(predkosc_wys);                    // wyswietlenie predkosci
    okno.draw(przyspieszenie_wys);              // wyswietlenie przyspieszenia
    okno.draw(czas_wys);                        // wyswietlenie czasu
    okno.draw(kinetyczna_wys);                  // wyswietlenie energii kinetycznej
    }

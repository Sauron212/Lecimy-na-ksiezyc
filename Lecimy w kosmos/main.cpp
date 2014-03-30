#include <SFML/Graphics.hpp>
#include <cmath>
#include <sstream>
#include <iostream>
#include "Faza_Kosmosu.h"
using namespace sf;
using namespace std;

void wyswietlanie_danych(sf::Clock czas, long double przyspieszenie, long double predkosc, long double odleglosc, long double kinetyczna);
    sf::ContextSettings settings( 0,0,8,2,0 );

sf::RenderWindow okno(sf::VideoMode(800, 600), "Lecimy w kosmos",sf::Style::Default,settings);
sf::Font font[2];

int main()
{
    bool fullscreen = false; // jak chcecie miec w fullscreenie to zmiencie na true
    if(fullscreen)
        okno.create(VideoMode::getDesktopMode(), "Lecimy w kosmos",sf::Style::Fullscreen,settings);
    sf::Texture tlo_menu;
    tlo_menu.loadFromFile("tlo-menu.png");
    sf::Sprite tlo1;
    tlo1.setTexture(tlo_menu);
    tlo1.setScale(okno.getSize().x/1920.0,okno.getSize().y/1080.0);
    font[0].loadFromFile("arial.ttf");  // ustawienie czcionki
    font[1].loadFromFile("ocr.ttf");

    int menu=0;
    sf::Text ziemiaT("ZIEMIA", font[0],20);
    sf::Text lecimyT("Lecimy w kosmos!", font[1],80);
    sf::Text kosmosT("KOSMOS", font[0],20);
    sf::Text wyjscieT("WYJSCIE", font[0],20);
    lecimyT.setColor(sf::Color::White);
    ziemiaT.setColor(sf::Color::Black);
    kosmosT.setColor(sf::Color::Black);
    wyjscieT.setColor(sf::Color::Black);
    lecimyT.setPosition((okno.getSize().x-lecimyT.getGlobalBounds().width)/2, 20);
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
    sf::Vector2f przyspieszenie; przyspieszenie.x=0; przyspieszenie.y=0;
    sf::Vector2f predkosc;predkosc.x=0;predkosc.y=0;// predkosc z jaka porusza sie rakieta
    sf::Vector2f F;F.x = 0; F.y=0;
    sf::Vector2f Fg; Fg.x = 0; Fg.y=0;
    long double kinetyczna=0;                   // Energia kinetyczna
    double IpredkoscI{0}, IprzyspieszenieI{0};
    double pole = 112.97;                       // pole jakie jest brane pod uwage w oporze
    double Cx = 0.3;                          // wspó³czynnik si³y oporu

    float t;
    double radiany;
    double k=1.41;
    double R=287;
    double T=301.15;
    double predkosc_dzwieku=sqrt(k*R*T);
    double Q = 1.1717;                              //gestosc powietrza

    long double moc_silnikow[4] = {39428858.47,0,0,0};     // Sila silnikow pierwszego stopnia w N
    sf::Vector2f opor; opor.x=0; opor.y=0;     // opor powietrza

    sf::Event zdarzenie;
    sf::Clock czas,czas_pod, czas_rotacja;             // czas - okres ruchu, czas_pod - czas podró¿y, czas_rotacja - zmiana rotacji co sekudne;

    sf::Texture tlo_tekstura;                       //    Stworzenie tekstury  tlo_tekstura
    tlo_tekstura.loadFromFile("tlo-start.png");

    sf::Sprite tlo;                                 //    Ustawienie Sprite z tekstura tlo_tekstura
    tlo.setTexture(tlo_tekstura);
    tlo.setPosition(0,-7297);

    sf::RectangleShape w_sil(sf::Vector2f(1, (moc_silnikow[0]/100000) )); //wektor siły silnikow
    w_sil.setPosition(x+5,y-(moc_silnikow[0]/100000)+55);
    w_sil.setFillColor(sf::Color(0,0,0));


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


    sf::Text v_plusT("Plus", font[0],20);
    sf::Text v_minusT("Minus", font[0],20);

    v_plusT.setColor(sf::Color::White);
    v_minusT.setColor(sf::Color::White);
    v_plusT.setPosition(500,200);
    v_minusT.setPosition(500,250);

    Rakieta KRakieta (1.49597870 * pow (10, 11) - 6378000, 0, 100);
    Laduj_Uklad ();
    FK_Rakieta.v.y = 3071.187586 + planety [3].omega * planety [3].promien_orbity;

    ostringstream s;
    s<< KRakieta.v.y;
    string wyswietlanie_KRakieta_v_y = s.str();
    sf::Text Tekst_KRakieta_v_y(wyswietlanie_KRakieta_v_y, font[0], 20);
    Tekst_KRakieta_v_y.setColor((sf::Color::White));
    Tekst_KRakieta_v_y.setPosition(500, 150);

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
            sf::FloatRect v_plus = v_plusT.getGlobalBounds();
            sf::FloatRect v_minus = v_minusT.getGlobalBounds();
            while(okno.pollEvent(zdarzenie))
            {
                if( zdarzenie.type == sf::Event::Closed )
                    okno.close();
                if(myszka.intersects(v_plus) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        KRakieta.v.y += 1000;
                        s.str("");
                        s<< KRakieta.v.y;
                        wyswietlanie_KRakieta_v_y = s.str();
                        Tekst_KRakieta_v_y.setString(wyswietlanie_KRakieta_v_y);

                    }
                if(myszka.intersects(v_minus) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        KRakieta.v.y -= 1000;
                        s.str("");
                        s<< KRakieta.v.y;
                        wyswietlanie_KRakieta_v_y = s.str();
                        Tekst_KRakieta_v_y.setString(wyswietlanie_KRakieta_v_y);

                    }
                if(myszka.intersects(ziemia) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    menu=1;
                if(myszka.intersects(kosmos) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    menu=2;
                if(myszka.intersects(wyjscie) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    okno.close();
            }
            okno.clear(sf::Color(255,255,255));
            okno.draw(tlo1);
            okno.draw( v_plusT );
            okno.draw( v_minusT );
            okno.draw( Tekst_KRakieta_v_y );
            okno.draw(lecimyT);
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
                if(sf::Keyboard::isKeyPressed (sf::Keyboard::Escape))
                    okno.close();
            }
            if(start==false)                // Zerowanie czasu jak jest na ziemii
                czas_pod.restart();
            if(czas.getElapsedTime().asMilliseconds() >= 1 && start)
            {
                g=G*Mz*pow(10,24)/(odleglosc*odleglosc);                // uatkualnienie g
                if(czas_pod.getElapsedTime().asSeconds()-6<0)// uatkualnie masy(spalanie paliwa)DOPOPRAWKI
                    m-=6562.2/1000;
                else if(czas_pod.getElapsedTime().asSeconds()-6>0 && czas_pod.getElapsedTime().asSeconds()-6<=70)
                    m-=13169.063/1000;
                else if(czas_pod.getElapsedTime().asSeconds()-6>70 && czas_pod.getElapsedTime().asSeconds()-6<135)
                    m-=13374.6246/1000;

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

                if(czas_pod.getElapsedTime().asSeconds()-6>=0)
                {
                    radiany=(rakieta.getRotation()*pi)/180.0;
                    t = czas.getElapsedTime().asMilliseconds();
                    opor.x = Cx*Q*pow(predkosc.x,2)*pole/2;          // uatkualnienie oporu powietrza
                    opor.y = Cx*Q*pow(predkosc.y,2)*pole/2;
                    Fg.x = 0;Fg.y = m*g;
                    F.x = moc_silnikow[0]*sin(radiany) ; F.y = moc_silnikow[0]*cos(radiany);
                    przyspieszenie.x = (F.x-Fg.x-opor.x)/(m*1000000);
                    przyspieszenie.y = (F.y-Fg.y-opor.y)/(m*1000000);
                    predkosc.x += przyspieszenie.x*t; predkosc.y += przyspieszenie.y*t;
                    IpredkoscI = sqrt(pow(predkosc.x,2)+pow(predkosc.y,2));
                    IprzyspieszenieI =sqrt(pow(przyspieszenie.x,2)+pow(przyspieszenie.y,2));
                    predkosc_dzwieku=sqrt(k*R*T);
                    float Ma = IpredkoscI*1000/predkosc_dzwieku;
                        if(Ma<0.5)
                        {
                            Cx-=0.000002;
                            cout << Ma << endl;
                            cout << Cx << endl;
                        }
                        else if(Ma>=0.5&&Ma<1.3)
                        {
                          Cx+=0.000025;
                        }
                    kinetyczna=0.5*m*pow(IpredkoscI,2);                                          // obliczanie Ek

                    if(rakieta.getPosition().y<244 )   // ustawienie rakiety na srodku ekranu
                    {
                    mapa.setCenter(rakieta.getPosition().x+6,rakieta.getPosition().y+56);//podązanie za rakietą
                    }

                    rakieta.move(predkosc.x, -predkosc.y);
                    w_sil.move(predkosc.x, -predkosc.y);
                    odleglosc+=predkosc.y*t;
                    Q-=predkosc.y*t*0.0001;
                    T-=predkosc.y*t*0.0045;
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
                sf::Text stratosfera("Weszlismy w STRATOSFERE", font[0], 18);
                stratosfera.setPosition(300,400);
                stratosfera.setColor(sf::Color::Red);
                okno.draw(stratosfera);
            }
            if(odleglosc>55000 && odleglosc<60000)
            {
                sf::Text mezosfera("Weszlismy w MEZOSFERE", font[0], 18);
                mezosfera.setPosition(300,400);
                mezosfera.setColor(sf::Color::Red);
                okno.draw(mezosfera);
            }
            okno.setView(mapa);
                okno.draw(rakieta);
                okno.draw(w_sil);

            okno.setView(okno.getDefaultView());
                wyswietlanie_danych(czas_pod,IprzyspieszenieI,IpredkoscI,odleglosc,kinetyczna);

            okno.setView(mapa);
            okno.display();
        }
        while (okno.isOpen () && (odleglosc > 6471000 || menu == 2))
        {
            while (okno.pollEvent (zdarzenie))
            {
                if (zdarzenie.type == sf::Event::Closed) okno.close ();
                if (sf::Keyboard::isKeyPressed (sf::Keyboard::Space)) Symulacja (86400);
                if (sf::Keyboard::isKeyPressed (sf::Keyboard::Q)) okno.close ();
                if (sf::Keyboard::isKeyPressed (sf::Keyboard::S)) klip.setSize (klip.getSize () + sf::Vector2f (12, 9));
                if (sf::Keyboard::isKeyPressed (sf::Keyboard::W)) klip.setSize (klip.getSize () + sf::Vector2f (-12, -9));
            }
            klip.setCenter (FK_Rakieta.koordynata_x / pow (10, 6), -FK_Rakieta.koordynata_y / pow (10, 6));
            okno.setView (klip);
            okno.clear (sf::Color::Black);
            okno.draw (Ktlo);
            okno.draw (&punkty [0], punkty.size(), sf::LinesStrip);
            for (int i = 0; i < planety.size (); i++) okno.draw (planety [i].grafika);
            okno.draw (FK_Rakieta.grafika);
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
    sf::Text czas_wys(czas3, font[0], 20);
    czas_wys.setColor((sf::Color::Black));
    czas_wys.setPosition(410, 480);
    ss.str("");
    ss<<"Przyspieszenie "<<przyspieszenie*1000000<<"m/s^2";
    string przyspieszenie_w = ss.str();
    sf::Text przyspieszenie_wys(przyspieszenie_w, font[0],20);
    przyspieszenie_wys.setColor((sf::Color::Black));
    przyspieszenie_wys.setPosition(410,500);
    ss.str("");
    ss<<"Predkosc "<<predkosc*1000<<"m/s";
    string predkosc_w = ss.str();
    sf::Text predkosc_wys(predkosc_w, font[0],20);
    predkosc_wys.setColor((sf::Color::Black));
    predkosc_wys.setPosition(410,520);
    ss.str("");
    ss<<"Wysokosc nad poziomem morza "<<odleglosc-6371000<<"m";
    string wysokosc_w = ss.str();
    sf::Text wysokosc_wys(wysokosc_w, font[0],20);
    wysokosc_wys.setColor((sf::Color::Black));
    wysokosc_wys.setPosition(410,540);
    ss.str("");
    if(kinetyczna<=1000000)
        ss<<"Energia kinetyczna "<<kinetyczna<<"J";
    else
        ss<<"Energia kinetyczna "<<kinetyczna/1000000<<"MJ";
    string kinetyczna_w = ss.str();
    sf::Text kinetyczna_wys(kinetyczna_w, font[0],20);
    kinetyczna_wys.setColor((sf::Color::Black));
    kinetyczna_wys.setPosition(410,560);


    okno.draw(wysokosc_wys);                    // wyswietlenie wysokosci
    okno.draw(predkosc_wys);                    // wyswietlenie predkosci
    okno.draw(przyspieszenie_wys);              // wyswietlenie przyspieszenia
    okno.draw(czas_wys);                        // wyswietlenie czasu
    okno.draw(kinetyczna_wys);                  // wyswietlenie energii kinetycznej
    }

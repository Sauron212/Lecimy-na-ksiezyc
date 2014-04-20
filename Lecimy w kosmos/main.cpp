#include <SFML/Graphics.hpp>
#include <cmath>
#include <sstream>
#include <iostream>
#include "Faza_Kosmosu.h"
using namespace sf;
using namespace std;

void wyswietlanie_danych(float czas_podrozy, long double przyspieszenie, long double predkosc, long double odleglosc, long double kinetyczna, long double paliwo[3],int numer);
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
    int x{390},y{455}; // odleglosc rakeity od srodka ziemi ; x,y - wspolrzedne rakiety wzgledem okna;

    long double odleglosc = 6371000; // Faza Kosmosu -> 106370001   Faza Ziemska -> 6371000

    bool start=false;                      // ustawienie startu rakiety na klikniecie spacja.
    bool wektor=false;                      //ustawienie wyswietlania wektorow

    long double m=242186.12;      // masa rakiety (kg) na starcie
    long double paliwo[3]={2145798.08,443235.04,107095.43};
    long double Mc = m+paliwo[0]+paliwo[1]+paliwo[2];


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

    double t;
    double radiany;
    float Ma;
    double k=1.41;
    double R=287;
    double T=301.15;
    double predkosc_dzwieku=sqrt(k*R*T);
    double Q = 1.1717;                              //gestosc powietrza


    long double moc_silnikow[4] = {34318696.99,5104334.30,0,0};     // Sila silnikow pierwszego stopnia w N
    int numer = 0;
    sf::Vector2f opor; opor.x=0; opor.y=0;     // opor powietrza

    sf::Event zdarzenie;
    sf::Clock czas,czas_pod, czas_rotacja; // czas - okres ruchu, czas_pod - czas podró¿y, czas_rotacja - zmiana rotacji co sekudne;
    float czas_podrozy=-7; // nie wiem czemu -7 ale jak jest -6 to pokazuje -5 -,-
    float szybkosc_sym=1;

    sf::Texture tlo_tekstura;                       //    Stworzenie tekstury  tlo_tekstura
    tlo_tekstura.loadFromFile("tlo-start.png");

    sf::Sprite tlo;                                 //    Ustawienie Sprite z tekstura tlo_tekstura
    tlo.setTexture(tlo_tekstura);
    tlo.setPosition(0,-7297);

    sf::RectangleShape rakieta(sf::Vector2f(11,111)); //  Stworzenie rakiety, wymiary, pozycja, color itp skala 1:1
    rakieta.setPosition(x,y);
    rakieta.setOrigin(5,55);
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
    sf::Text time_frame_plusT("Plus", font[0],20);
    sf::Text time_frame_minusT("Minus", font[0],20);
    time_frame_plusT.setColor(sf::Color::White);
    time_frame_minusT.setColor(sf::Color::White);
    time_frame_plusT.setPosition(500,200);
    time_frame_minusT.setPosition(500,250);

    sf::Text time_modifier_plusT("Plus", font[0],20);
    sf::Text time_modifier_minusT("Minus", font[0],20);
    time_modifier_plusT.setColor(sf::Color::White);
    time_modifier_minusT.setColor(sf::Color::White);
    time_modifier_plusT.setPosition(600,200);
    time_modifier_minusT.setPosition(600,250);


    Rakieta KRakieta (1.49597870 * pow (10, 11) - 6378000, 0, 100);
    Laduj_Uklad ();
    Laduj_Guziki ();
    FK_Rakieta.v.y = 4603.187586 + planety [3].omega * planety [3].promien_orbity;
    FK_Rakieta.v.x = 3071.187586;
    int time_frame, time_start, time_stop, time_current, time_current_index, time_modifier = 0;
    time_frame = 4*86400;
    time_start = 0;
    time_stop = 4*86400;
    time_modifier = 15;
    bool simulation_start = false;
    sf::Vector2f mouse;
    sf::View gui_view;
    gui_view.reset (sf::FloatRect (0, 0, 800, 600));
    gui_view.setViewport (sf::FloatRect (0.0f, 0.0f, 1.0f, 1.0f));


    ostringstream ss_time_frame;
    ss_time_frame<< time_frame;
    string wyswietlanie_time_frame = ss_time_frame.str();
    sf::Text Tekst_time_frame(wyswietlanie_time_frame, font[0], 20);
    Tekst_time_frame.setColor((sf::Color::White));
    Tekst_time_frame.setPosition(500, 150);

    ostringstream ss_time_modifier;
    ss_time_modifier<< time_modifier;
    string wyswietlanie_time_modifier = ss_time_modifier.str();
    sf::Text Tekst_time_modifier(wyswietlanie_time_modifier, font[0], 20);
    Tekst_time_modifier.setColor((sf::Color::White));
    Tekst_time_modifier.setPosition(600, 150);

    sf::Texture Ktlo_tekstura;
    Ktlo_tekstura.loadFromFile("Ktlo.png");
    sf::Sprite Ktlo;
    Ktlo.setTexture(Ktlo_tekstura);
    Ktlo.setPosition(KRakieta.koordynata_x / pow (10, 6) - 400, KRakieta.koordynata_y / pow (10, 6) - 300);
    FK_Rakieta.sprite.setPosition (FK_Rakieta.koordynata_x / 1000000, -FK_Rakieta.koordynata_y / 1000000);
    double Ktangens = ( 2000 - 100 ) / ( 2000 - 100 );
    double Kkatrakiety;
    Kkatrakiety = atan ( Ktangens ) * 180 / 3.14;
    bool Kstart = false;
    sf::View klip;
    klip.reset (sf::FloatRect (0, 0, 800, 600));
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
            sf::FloatRect time_frame_plus = time_frame_plusT.getGlobalBounds();
            sf::FloatRect time_frame_minus = time_frame_minusT.getGlobalBounds();
            sf::FloatRect time_modifier_plus = time_modifier_plusT.getGlobalBounds();
            sf::FloatRect time_modifier_minus = time_modifier_minusT.getGlobalBounds();
            while(okno.pollEvent(zdarzenie))
            {
                if( zdarzenie.type == sf::Event::Closed )
                    okno.close();
                if(myszka.intersects(time_frame_plus) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        time_frame += 1000;
                        ss_time_frame.str("");
                        ss_time_frame<< time_frame;
                        wyswietlanie_time_frame = ss_time_modifier.str();
                        Tekst_time_frame.setString(wyswietlanie_time_frame);
                    }
                if(myszka.intersects(time_frame_minus) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        time_frame -= 1000;
                        ss_time_frame.str("");
                        ss_time_frame<< time_frame;
                        wyswietlanie_time_frame = ss_time_frame.str();
                        Tekst_time_frame.setString(wyswietlanie_time_frame);
                    }
                if(myszka.intersects(time_modifier_plus) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        time_modifier += 10;
                        ss_time_modifier.str("");
                        ss_time_modifier<< time_modifier;
                        wyswietlanie_time_modifier = ss_time_modifier.str();
                        Tekst_time_modifier.setString(wyswietlanie_time_modifier);
                    }
                if(myszka.intersects(time_modifier_minus) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        time_modifier -= 10;
                        ss_time_modifier.str("");
                        ss_time_modifier<< time_modifier;
                        wyswietlanie_time_modifier = ss_time_modifier.str();
                        Tekst_time_modifier.setString(wyswietlanie_time_modifier);
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
            okno.draw( time_frame_plusT );
            okno.draw( time_frame_minusT );
            okno.draw( Tekst_time_frame );
            okno.draw( time_modifier_plusT );
            okno.draw( time_modifier_minusT );
            okno.draw( Tekst_time_modifier );
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
                if(zdarzenie.type == sf::Event::KeyPressed && zdarzenie.key.code == sf::Keyboard::W) //Wywolanie akcji klawiszem W
                    wektor=!wektor;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
                    szybkosc_sym<6?szybkosc_sym+=0.5:szybkosc_sym;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
                    szybkosc_sym>0.5?szybkosc_sym-=0.5:szybkosc_sym;

            }
            if(start==false)                // Zerowanie czasu jak jest na ziemii
                czas_pod.restart();
            czas_podrozy += czas_pod.getElapsedTime().asSeconds()*szybkosc_sym;
            czas_pod.restart();

            if(czas.getElapsedTime().asMilliseconds() >= 1 && start)
            {
                t = szybkosc_sym*czas.getElapsedTime().asMicroseconds()/1000.0;
                g=G*Mz*pow(10,24)/(odleglosc*odleglosc);                // uatkualnienie g
                // spalanie 1 silnik
                if(czas_podrozy<0)// uatkualnie masy(spalanie paliwa)DOPOPRAWKI
                    paliwo[0]-=t*(6562.2/1000);
                else if(czas_podrozy>=0 && czas_podrozy<=70)
                    paliwo[0]-=t*(13225.7/1000);
                else if(czas_podrozy>70 && czas_podrozy<135)
                    paliwo[0]-=t*(13374.6246/1000);
                else if(czas_podrozy>135 && czas_podrozy<161)
                    paliwo[0]-=t*(10894.06/1000);//10716.83
                // spalanie 2 silnik
                else if(czas_podrozy>164 && czas_podrozy<=460)
                    paliwo[1]-=t*(1225.45/1000);
                else if(czas_podrozy>460 && czas_podrozy<=498)
                    paliwo[1]-=t*(980.36/1000);
                else if(czas_podrozy>498 && czas_podrozy<=548)
                    paliwo[1]-=t*(728.52/1000);
                //spalanie 3 silnik
                else if(czas_podrozy>552 && czas_podrozy<=699)
                    paliwo[2]-=t*(213.42/1000);

                t = szybkosc_sym*czas.getElapsedTime().asMicroseconds()/1000.0;
                if(czas_rotacja.getElapsedTime().asMilliseconds() >= 1) // Co sekundę...
                {
                    if(czas_podrozy>=30 && czas_podrozy<80) // ...w wyznaczonym czasie (dodanie 6 sekund z powodu opóźnionego startu)...
                        rakieta.rotate(t*0.7280000/1000);//...obrót rakiety o tyle stopni
                    else if(czas_podrozy>=80 && czas_podrozy<135)
                        rakieta.rotate(t*0.4696364/1000);
                    else if(czas_podrozy>=135 && czas_podrozy<165)
                        rakieta.rotate(t*0.2970000/1000);

                czas_rotacja.restart();
                }
                t = szybkosc_sym*czas.getElapsedTime().asMicroseconds()/1000.0;
                    if(numer==0)
                    {
                        if(paliwo[0]>=315239.89)
                            moc_silnikow[0]+=9.5*t;
                        else if(paliwo[0]<315239.89 && paliwo[0]>315200.89) // wylaczenie centralnego silnika
                            moc_silnikow[0]=31942035;
                        else if(paliwo[0]<315239.89 && paliwo[0]>31994.14)
                            moc_silnikow[0]+=0.0000002;
                        else if(paliwo[0]>0 && paliwo[0]<=31994.14)  // odpada silnik
                        {
                            numer = 1;
                            m -= 132890.32;
                            paliwo[0]=0;
                        }
                    }
                    if(numer==1)
                    {
                        if(paliwo[1]>=79742.90)
                            moc_silnikow[1]+=0;
                        else if(paliwo[1]<79742.90 && paliwo[1]>=43097.17 )
                            moc_silnikow[1]+=0;
                        else if(paliwo[1]<43097.17 && paliwo[1]>=6510.86)
                            moc_silnikow[1]+=0;
                        else if(paliwo[1]<6510.86 && paliwo[1]>=0)
                        {
                            numer = 2;
                            m -= 40392.40;
                            paliwo[1]=0;
                        }
                    }
                    if(numer==2)
                    {
                        if(paliwo[2]>=75695.49)
                            moc_silnikow[2]+=0;
                    }

                if(czas_podrozy>=0)
                {

                    Mc = m+paliwo[0]+paliwo[1]+paliwo[2];
                    radiany=(rakieta.getRotation()*pi)/180.0;
                    opor.x = Cx*(Q*pow(predkosc.x,2)/2)*pole;          // uatkualnienie oporu powietrza
                    opor.y = Cx*(Q*pow(predkosc.y,2)/2)*pole;
                    Fg.x = 0;Fg.y = Mc*g;
                    F.x = moc_silnikow[numer]*sin(radiany) ; F.y = moc_silnikow[numer]*cos(radiany);
                    przyspieszenie.x = (F.x-Fg.x-opor.x)/(Mc*1000000);
                    przyspieszenie.y = (F.y-Fg.y-opor.y)/(Mc*1000000);
                    t =  szybkosc_sym*czas.getElapsedTime().asMicroseconds()/1000.0;
                    predkosc.x += przyspieszenie.x*t; predkosc.y += przyspieszenie.y*t;
                    IpredkoscI = sqrt(pow(predkosc.x,2)+pow(predkosc.y,2));
                    IprzyspieszenieI =sqrt(pow(przyspieszenie.x,2)+pow(przyspieszenie.y,2));
                    predkosc_dzwieku=sqrt(k*R*T/0.029);
                    Ma = IpredkoscI*1000/predkosc_dzwieku;
                        if(Ma<0.5)
                        {
                          Cx-=0.000002;
                        }
                        else if(Ma>=0.5&&Ma<1.3)
                        {
                          Cx+=0.000025;
                        }
                    kinetyczna=0.5*m*pow(IpredkoscI,2);                                          // obliczanie Ek

                    if(rakieta.getPosition().y<244 )   // ustawienie rakiety na srodku ekranu
                    {
                        mapa.setCenter(rakieta.getPosition().x+11,rakieta.getPosition().y+56);//podązanie za rakietą
                    }
                    t = szybkosc_sym*czas.getElapsedTime().asMicroseconds()/1000.0;
                    odleglosc+=predkosc.y*t;
                    Q-=predkosc.y*t*0.0001;
                    T-=predkosc.y*t*0.0045;
                    rakieta.move(predkosc.x*t, -predkosc.y*t);
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
                if(wektor) //wyswietlenie wektorow
                {
                    Wektory grawitacja(Fg,rakieta.getPosition().x,rakieta.getPosition().y,sf::Color::Red,false);
                    Wektory ciag(F,rakieta.getPosition().x,rakieta.getPosition().y, sf::Color::Blue);
                    okno.draw(ciag.wektor);
                    okno.draw(grawitacja.wektor);
                }

            okno.setView(okno.getDefaultView());
            wyswietlanie_danych(czas_podrozy,IprzyspieszenieI,IpredkoscI,odleglosc,kinetyczna,paliwo,numer);

            okno.setView(mapa);
            okno.display();
        }
        while (okno.isOpen () && (odleglosc > 6471000 || menu == 2))
        {
            while (okno.pollEvent (zdarzenie))
            {
                switch (zdarzenie.type)
                {
                    case sf::Event::Closed:
                        okno.close ();
                        break;
                    case sf::Event::KeyPressed:
                        switch (zdarzenie.key.code)
                        {
                            case sf::Keyboard::Escape:
                                okno.close ();
                                break;
                            case sf::Keyboard::Space:
                                Symulacja (time_frame);
                                break;
                            case sf::Keyboard::Return:
                                simulation_start = !simulation_start;
                                time_current = time_start;
                                time_current_index = 0;
                                for (int i = 0;(i < czasy.size ()) && (czasy [i] < time_start); i++) time_current_index = i - 1;
                                for (int i = 0; i < planety.size (); i++) planety [i].pozycja_katowa = planety [i].omega * time_current;
                                for (int i = 0; i < satelity.size (); i++) satelity [i].pozycja_katowa = satelity [i].omega * time_current;
                                break;
                            case sf::Keyboard::Add:
                                klip.setSize (klip.getSize () + sf::Vector2f (-12, -9));
                                break;
                            case sf::Keyboard::Subtract:
                                klip.setSize (klip.getSize () + sf::Vector2f (12, 9));
                                break;
                            default:
                                break;
                        }
                        break;
                    case sf::Event::MouseButtonReleased:
                        if (!simulation_start)
                        {
                            mouse = okno.mapPixelToCoords (sf::Mouse::getPosition (okno), gui_view);
                            for (int i = 0; i < buttons.size (); i++) if (((mouse.x > buttons [i]->koordynata_x - buttons [i]->szerokosc / 2) && (mouse.x < buttons [i]->koordynata_x + buttons [i]->szerokosc / 2)) && ((mouse.y > buttons [i]->koordynata_y - buttons [i]->wysokosc / 2) && (mouse.y < buttons [i]->koordynata_y + buttons [i]->wysokosc / 2))) buttons [i]->clicked = true;
                        }
                        if (buttons [0]->clicked && time_frame > 200) time_frame -= 200;
                        if (buttons [1]->clicked) time_frame += 200;
                        if (buttons [2]->clicked && time_stop > 200) time_stop -= 200;
                        if (buttons [3]->clicked && time_stop < time_frame) time_stop += 200;
                        if (buttons [4]->clicked && time_modifier > 1) time_modifier -= 1;
                        if (buttons [5]->clicked) time_modifier += 1;
                        for (int i = 0; i < buttons.size (); i++) buttons [i]->clicked = false;
                        break;
                    default:
                        break;
                }
            }
            if (simulation_start)
            {
                do if (time_current > czasy [time_current_index]) time_current_index += 1;
                while (time_current > czasy [time_current_index] && time_current_index < czasy.size () - 1);
                FK_Rakieta.koordynata_x = punkty [time_current_index].x;
                FK_Rakieta.koordynata_y = punkty [time_current_index].y;
                FK_Rakieta.pozycja_katowa = katy [time_current_index];
                FK_Rakieta.sprite.setPosition (FK_Rakieta.koordynata_x / 1000000, -FK_Rakieta.koordynata_y / 1000000);
                FK_Rakieta.sprite.setRotation (-FK_Rakieta.pozycja_katowa * 180 / pi + 90);
                for (int i = 0; i < time_modifier; i++)
                {
                    for (int i = 0; i < planety.size (); i++) planety [i].Aktualizacja ();
                    for (int i = 0; i < satelity.size (); i++) satelity [i].Aktualizacja ();
                }
                time_current += time_modifier;
                if (time_current >= time_stop) simulation_start = false;
            }
            klip.setCenter (FK_Rakieta.koordynata_x / pow (10, 6), -FK_Rakieta.koordynata_y / pow (10, 6));
            okno.setView (klip);
            okno.clear (sf::Color::Black);
            okno.draw (Ktlo);
            for (int i = 0; i < planety.size (); i++) okno.draw (planety [i].grafika);
            for (int i = 0; i < satelity.size (); i++) okno.draw (satelity [i].grafika);
            okno.draw (FK_Rakieta.sprite);
            okno.setView (gui_view);
            for (int i = 0; i < buttons.size (); i++) okno.draw (buttons [i]->sprite);
            okno.display ();
        }
    }
    return 0;
}
void wyswietlanie_danych(float czas_podrozy, long double przyspieszenie, long double predkosc, long double odleglosc, long double kinetyczna, long double paliwo[3], int numer)
{
    ostringstream ss;                       // potrzebne do konwersji z inta/clocka na stringa
            int godziny = czas_podrozy/3600;
            int minuty = (czas_podrozy-godziny*3600)/60;
            int sekundy = czas_podrozy-godziny*3600-minuty*60;
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

    ss.str("");
    ss<<"Zbiornik " << numer+1;
    long double pelne_zbiorniki[3]={2145798.08,443235.04,107095.43};
    string pal_w = ss.str();
    sf::Text pal_wys(pal_w, font[0],20);
    pal_wys.setColor((sf::Color::Black));
    pal_wys.setPosition(100,100);

    sf::RectangleShape czerwony(sf::Vector2f(20,200));
    czerwony.setPosition(100,120);
    czerwony.setFillColor(sf::Color::Red);
    sf::RectangleShape zielony(sf::Vector2f(20, paliwo[numer]/pelne_zbiorniki[numer]*200));
    zielony.setOrigin(0,0);
    zielony.setFillColor(sf::Color::Green);
    zielony.setPosition(120,320);
    zielony.setRotation(180);

    okno.draw(wysokosc_wys);                    // wyswietlenie wysokosci
    okno.draw(predkosc_wys);                    // wyswietlenie predkosci
    okno.draw(przyspieszenie_wys);              // wyswietlenie przyspieszenia
    okno.draw(czas_wys);                        // wyswietlenie czasu
    okno.draw(kinetyczna_wys);                  // wyswietlenie energii kinetycznej
    okno.draw(pal_wys);
    okno.draw(czerwony);
    okno.draw(zielony);
    }

#include <SFML/Graphics.hpp>
#include <cmath>
#include <sstream>
#include <iostream>
#include "Faza_Kosmosu.h"
using namespace sf;
using namespace std;

void wyswietlanie_danych(float czas_podrozy, long double przyspieszenie, long double predkosc, long double odleglosc, long double kinetyczna, long double paliwo[3],int numer,float szybkosc_sym,float temperatura);
    sf::ContextSettings settings( 0,0,8,2,0);
void wyswietlanie_danych_kosmos(float czas_podrozy_w_kosmosie, float czas_podrozy_na_ziemi, double ziemia_x, double ziemia_y, double ksiezyc_x, double ksiezyc_y, double rakieta_x, double rakieta_y);

sf::RenderWindow okno(sf::VideoMode(1000, 700), "Lecimy w kosmos",sf::Style::Default,settings);
sf::Font font[2];

int main()
{
    bool fullscreen = false; // jak chcecie miec w fullscreenie to zmiencie na true
    if(fullscreen)
        okno.create(VideoMode::getDesktopMode(), "Lecimy w kosmos",sf::Style::Fullscreen,settings);
    sf::Texture tlo_glowne,tlo_menu,tlo_dane;
    tlo_glowne.loadFromFile("tlo-menu.png");
    tlo_menu.loadFromFile("menu.png");
    tlo_dane.loadFromFile("tlo-dane.png");

    sf::Sprite tlo1,menuP,tloDane;
    tlo1.setTexture(tlo_glowne);
    tloDane.setTexture(tlo_dane);
    tloDane.setPosition( (okno.getSize().x-1180)/2,(okno.getSize().y - 668)/2);
    menuP.setTexture(tlo_menu);
    tlo1.setScale(okno.getSize().x/1920.0,okno.getSize().y/1080.0);
    menuP.setScale(350/594.0,295/500.0);
    menuP.setTextureRect(sf::IntRect(0,0,594,500.0));
    menuP.setPosition(0.15*okno.getSize().x,0.32*okno.getSize().y);

    font[0].loadFromFile("arial.ttf");  // ustawienie czcionki
    font[1].loadFromFile("ocr.ttf");

    int menu=0;
    sf::Text ziemiaT("ZIEMIA", font[0],24);
    sf::Text lecimyT("Lecimy w kosmos!", font[1],80);
    sf::Text kosmosT("KOSMOS", font[0],24);
    sf::Text daneT("DANE", font[0],24);
    sf::Text wyjscieT("WYJSCIE", font[0],24);

    lecimyT.setColor(sf::Color::White);
    ziemiaT.setColor(sf::Color::White);
    kosmosT.setColor(sf::Color::White);
    daneT.setColor(sf::Color::White);
    wyjscieT.setColor(sf::Color::White);

    lecimyT.setPosition((okno.getSize().x-lecimyT.getGlobalBounds().width)/2, 20);
    ziemiaT.setPosition(menuP.getPosition().x+120,menuP.getPosition().y+30);
    kosmosT.setPosition(menuP.getPosition().x+120,menuP.getPosition().y+90);
    daneT.setPosition(menuP.getPosition().x+120,menuP.getPosition().y+150);
    wyjscieT.setPosition(menuP.getPosition().x+120,menuP.getPosition().y+210);

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
    float T=301.15;
    double predkosc_dzwieku=sqrt(k*R*T);
    double Q = 1.1717;                              //gestosc powietrza

    long double moc_silnikow[4] = {34318696.99,5104334.30,901223.04,0};     // Sila silnikow pierwszego stopnia w N
    int numer = 0;
    sf::Vector2f opor; opor.x=0; opor.y=0;     // opor powietrza

    sf::Event zdarzenie;
    sf::Clock czas,czas_pod, czas_rotacja; // czas - okres ruchu, czas_pod - czas podró¿y, czas_rotacja - zmiana rotacji co sekudne;
    float czas_podrozy=-7; // nie wiem czemu -7 ale jak jest -6 to pokazuje -5 -,-
    float szybkosc_sym=1;

    sf::Texture tlo_tekstura1,tlo_tekstura2,tlo_tekstura3,tlo_tekstura4,tlo_tekstura5,tlo_tekstura6,tlo_rakieta, tlo_background;                       //    Stworzenie tekstury  tlo_tekstura
    tlo_tekstura1.loadFromFile("tlo0.png");
    tlo_tekstura2.loadFromFile("tlo1.png");
    tlo_tekstura3.loadFromFile("tlo2.png");
    tlo_tekstura4.loadFromFile("tlo3.png");
    tlo_tekstura5.loadFromFile("tlo4.png");
    tlo_tekstura6.loadFromFile("tlo5.png");
    tlo_background.loadFromFile("roboczy.png");
    tlo_rakieta.loadFromFile("rakieta_cala.png");
    tlo_tekstura1.setRepeated(true);
    tlo_tekstura2.setRepeated(true);
    tlo_tekstura3.setRepeated(true);
    tlo_tekstura4.setRepeated(true);
    tlo_tekstura5.setRepeated(true);
    tlo_tekstura6.setRepeated(true);


    sf::Sprite tlo,tlo2,tlo3,tlo4,tlo5,tlo6,tlo_niewiem;                                 //    Ustawienie Sprite z tekstura tlo_tekstura
    tlo.setTextureRect(sf::IntRect(0, 0, 100000, 8000));
    tlo2.setTextureRect(sf::IntRect(0, 0, 100000, 8000));
    tlo3.setTextureRect(sf::IntRect(0, 0, 100000, 30000));
    tlo4.setTextureRect(sf::IntRect(0, 0, 100000, 8000));
    tlo5.setTextureRect(sf::IntRect(0, 0, 100000, 8000));
    tlo6.setTextureRect(sf::IntRect(0, 0, 100000000, 23000000));

    tlo.setTexture(tlo_tekstura1);
    tlo2.setTexture(tlo_tekstura2);
    tlo3.setTexture(tlo_tekstura3);
    tlo4.setTexture(tlo_tekstura4);
    tlo5.setTexture(tlo_tekstura5);
    tlo6.setTexture(tlo_tekstura6);
    tlo_niewiem.setTexture(tlo_background);
    tlo_niewiem.setScale(okno.getSize().x/1000.0,okno.getSize().y/700.0);

    tlo.setPosition(0,-7297);
    tlo2.setPosition(0,-15297);
    tlo3.setPosition(0,-45297);
    tlo4.setPosition(0,-53297);
    tlo5.setPosition(0,-61297);
    tlo6.setPosition(0,-2300000);

    sf::RectangleShape rakieta(sf::Vector2f(11,111)); //  Stworzenie rakiety, wymiary, pozycja, color itp skala 1:1
    rakieta.setPosition(x,y);
    rakieta.setOrigin(5,55);
    rakieta.setTexture(&tlo_rakieta);

    sf::Vector2f j_ziemi; j_ziemi.x=x; j_ziemi.y=6371000+111+455;


            sf::View mapa; //stwoerzenie widoku mapy
            mapa.reset(sf::FloatRect(100,10,4000,5000));
            mapa.setViewport(sf::FloatRect(0, 0, 1, 1));

    sf::Texture silnik_tex; //odpadajace silniki
    silnik_tex.loadFromFile("silnik.png");
    sf::Sprite silnik1;
    silnik1.setTexture(silnik_tex);
    bool zrzut1;


    /* ZMIENNE GRUPY JARKA : */
    Rakieta KRakieta (1.49597870 * pow (10, 11) - 6378000, 0, 100);
    Laduj_Uklad ();
    Laduj_Guziki ();
    FK_Rakieta.v.y = -1000; //+ planety [3].omega * planety [3].promien_orbity;
    FK_Rakieta.v.x = 6250;
    int time_frame, time_start, time_stop, time_current, time_current_index, time_modifier = 0;
    time_frame = 10*86400;
    time_start = 0;
    time_stop = 10*86400;
    time_modifier = 25;
    bool simulation_start = false;
    sf::Vector2f mouse;
    sf::View gui_view;
    gui_view.reset (sf::FloatRect (0, 0, 800, 600));
    gui_view.setViewport (sf::FloatRect (0.0f, 0.0f, 1.0f, 1.0f));

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
            sf::FloatRect dane = daneT.getGlobalBounds();
            while(okno.pollEvent(zdarzenie))
            {
                if( zdarzenie.type == sf::Event::Closed )
                    okno.close();
                if(myszka.intersects(ziemia) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    menu=1;
                if(myszka.intersects(kosmos) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    menu=2;
                if(myszka.intersects(dane) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    menu=3;
                if(myszka.intersects(wyjscie) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    okno.close();
            }
            okno.clear(sf::Color(255,255,255));
            okno.draw(tlo1);
            okno.draw(menuP);
            okno.draw(lecimyT);
            okno.draw(ziemiaT);
            okno.draw(kosmosT);
            okno.draw(daneT);
            okno.draw(wyjscieT);
            okno.display();
        }
        while(okno.isOpen() && menu==3)
        {
            sf::Vector2i myszka2 = sf::Mouse::getPosition(okno);
            sf::FloatRect myszka;
            myszka.left = myszka2.x;
            myszka.top=myszka2.y;
            myszka.height=myszka.width=1;

            sf::Text wsteczT("WSTECZ", font[0],20);
            wsteczT.setColor(sf::Color::White);
            wsteczT.setPosition(116,okno.getSize().y-45);

            sf::FloatRect wstecz = wsteczT.getGlobalBounds();

            sf::Text dane("Zmiana danych", font[0], 25);
            dane.setPosition(sf::Vector2f(50,50));
                while(okno.pollEvent(zdarzenie))
                {
                if( zdarzenie.type == sf::Event::Closed )
                 okno.close();
                if(myszka.intersects(wstecz) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                 menu=0;
                if(sf::Keyboard::isKeyPressed (sf::Keyboard::Escape))
                    okno.close();
                }
            okno.clear(sf::Color(255,255,255));
            okno.draw(tlo1);
            okno.draw(tloDane);
            okno.draw(dane);
            okno.draw(wsteczT);
            okno.display();
        }
        while(okno.isOpen()&&menu==1)
        {
            while(okno.pollEvent(zdarzenie))
            {
                if( zdarzenie.type == sf::Event::Closed )
                 okno.close();
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                {
                 start=true;
                 czas.restart();
                }
                if(sf::Keyboard::isKeyPressed (sf::Keyboard::Escape))
                    okno.close();
                if(zdarzenie.type == sf::Event::KeyPressed && zdarzenie.key.code == sf::Keyboard::W) //Wywolanie akcji klawiszem W
                    wektor=!wektor;
                if(zdarzenie.type == sf::Event::KeyPressed && zdarzenie.key.code == sf::Keyboard::Add)
                    szybkosc_sym<6?szybkosc_sym+=0.5:szybkosc_sym;
                if(zdarzenie.type == sf::Event::KeyPressed && zdarzenie.key.code == sf::Keyboard::Subtract)
                    szybkosc_sym>0.5?szybkosc_sym-=0.5:szybkosc_sym;

            }
            if(start==false)                // Zerowanie czasu jak jest na ziemii
                czas_pod.restart();
            czas_podrozy += czas_pod.getElapsedTime().asSeconds()*szybkosc_sym;
            czas_pod.restart();

            if(czas.getElapsedTime().asMilliseconds() >= 1 && start)
            {
                t = szybkosc_sym*czas.getElapsedTime().asMicroseconds()/1000.0;
                // spalanie 1 silnik
                if(czas_podrozy<=0)
                    paliwo[0]-=t*(6562.2/1000);
                else if(czas_podrozy>=0 && czas_podrozy<=70)
                    paliwo[0]-=t*(13225.7/1000);
                else if(czas_podrozy>70 && czas_podrozy<=135)
                    paliwo[0]-=t*(13374.6246/1000);
                else if(czas_podrozy>=135 && czas_podrozy<=164)
                    paliwo[0]-=t*(10894.06/1000);
                // spalanie 2 silnik
                else if(czas_podrozy>=164 && czas_podrozy<=461)
                    paliwo[1]-=t*(1225.45/1000);
                else if(czas_podrozy>461 && czas_podrozy<=498)
                    paliwo[1]-=t*(980.4/1000);
                else if(czas_podrozy>498 && czas_podrozy<=552)
                    paliwo[1]-=t*(728.52/1000);
                //spalanie 3 silnik
                else if(czas_podrozy>552 && czas_podrozy<=699)
                    paliwo[2]-=t*(213.4/1000);

                t = szybkosc_sym*czas.getElapsedTime().asMicroseconds()/1000.0;
                    if(czas_podrozy>=30 && czas_podrozy<80) // ...w wyznaczonym czasie (dodanie 6 sekund z powodu opóźnionego startu)...
                        rakieta.rotate(t*0.7280000/1000);//...obrót rakiety o tyle stopni
                    else if(czas_podrozy>=80 && czas_podrozy<135)
                        rakieta.rotate(t*0.4696364/1000);
                    else if(czas_podrozy>=135 && czas_podrozy<165)
                        rakieta.rotate(t*0.2970000/1000);
                    else if(czas_podrozy>=165 && czas_podrozy<185)
                        rakieta.rotate(t*(-0.5285000)/1000);
                    else if(czas_podrozy>=185 && czas_podrozy<320)
                        rakieta.rotate(t*0.0309630/1000);
                    else if(czas_podrozy>=320 && czas_podrozy<460)
                        rakieta.rotate(t*0.0900000/1000);
                    else if(czas_podrozy>=460 && czas_podrozy<480)
                        rakieta.rotate(t*(-0.1380000)/1000);
                    else if(czas_podrozy>=480 && czas_podrozy<550)
                        rakieta.rotate(t*0.0971429/1000);
                    else if(czas_podrozy>=550 && czas_podrozy<570)
                        rakieta.rotate(t*(-0.2070000)/1000);
                    else if(czas_podrozy>=570 && czas_podrozy<640)
                        rakieta.rotate(t*0.1117143/1000);
                    else if(czas_podrozy>=640 && czas_podrozy<705)
                        rakieta.rotate(t*0.0486154/1000);
                t = szybkosc_sym*czas.getElapsedTime().asMicroseconds()/1000.0;
                    if(numer==0)
                    {
                        if(paliwo[0]>=315239.89)
                            moc_silnikow[0]+=42*t;
                        else if(paliwo[0]>315100.89 && paliwo[0]<315239.89) // wylaczenie centralnego silnika
                            moc_silnikow[0]=31942035;
                        else if(paliwo[0]<315239.89 && paliwo[0]>31994.14)
                            moc_silnikow[0]+=0.000000002*t;
                        else if(paliwo[0]>0 && paliwo[0]<=31994.14)  // odpada silnik
                        {
                            numer = 1;
                            m -= 132890.32;
                            paliwo[0]=0;
                            silnik1.setPosition(rakieta.getPosition().x,rakieta.getPosition().y);
                        }
                    }
                    else if(numer==1)
                    {
                        if(paliwo[1]>=43097.17)
                            moc_silnikow[1]=4066742.13;
                        else if(paliwo[1]<43097.17)
                            moc_silnikow[1]=3050634.87;
                        if(paliwo[1]<=6510.86 && paliwo[1]>=0)
                        {
                            numer = 2;
                            m -= 40392.40;
                            paliwo[1]=0;
                            cout << "przerzucilem";
                        }
                    }

                if(czas_podrozy>=0)
                {

                    Mc = m+paliwo[0]+paliwo[1]+paliwo[2];
                    radiany=(rakieta.getRotation()*pi)/180.0;
                    if(odleglosc-6371000<90000)
                    {
                    opor.x = Cx*(Q*pow(predkosc.x,2)/2)*pole;          // uatkualnienie oporu powietrza
                    opor.y = Cx*(Q*pow(predkosc.y,2)/2)*pole;
                    }
                    else
                    {
                        opor.x=0;
                        opor.y=0;
                    }
                    Fg.x = 0;Fg.y = Mc*g;
                    F.x = moc_silnikow[numer]*sin(radiany) ; F.y = moc_silnikow[numer]*cos(radiany);
                    przyspieszenie.x = (F.x-Fg.x-opor.x)/(Mc*1000000);
                    przyspieszenie.y = (F.y-Fg.y-opor.y)/(Mc*1000000);
                    t =  szybkosc_sym*czas.getElapsedTime().asMicroseconds()/1000.0;
                    predkosc.x += przyspieszenie.x*t; predkosc.y += przyspieszenie.y*t;
                    IpredkoscI = sqrt(pow(predkosc.x,2)+pow(predkosc.y,2));
                    IprzyspieszenieI =sqrt(pow(przyspieszenie.x,2)+pow(przyspieszenie.y,2));
                    predkosc_dzwieku=sqrt(k*R*T);
                    Ma = IpredkoscI*1000/predkosc_dzwieku;
                        if(Ma<0.5)
                        {
                          Cx-=0.0000006*t;
                        }
                        else if(Ma>=0.5&&Ma<1.3)
                        {
                          Cx+=0.000006*t;
                        }
                        else if(Ma>=1.3 && Ma<3.5)
                        {
                          Cx-=0.000005*t;
                        }
                        else if(Ma>=3.5 && Ma<5)
                        {
                          Cx-=0.0000001*t;
                        }
                        else if(Ma>=5 && Ma<8.6)
                        {
                         Cx+=0.00000001*t;
                        }
                        else if(Ma>=8.6)
                        {
                            Cx = 0.26;
                        }
//                        if(czas_podrozy>66.5)
//                          {cout << moc_silnikow[0] << " " << Ma<< endl;cin.get();cin.get();}

                    kinetyczna=0.5*m*pow(IpredkoscI,2);                                          // obliczanie Ek
                    rakieta.move(predkosc.x*t, -predkosc.y*t);
                    if(rakieta.getPosition().y<244 )   // ustawienie rakiety na srodku ekranu
                    {
                        mapa.setCenter(rakieta.getPosition().x+110,rakieta.getPosition().y+66);//podązanie za rakietą
                    }
                    odleglosc=sqrt( (rakieta.getPosition().y-j_ziemi.y)*(rakieta.getPosition().y-j_ziemi.y)+(rakieta.getPosition().x-j_ziemi.x)*(rakieta.getPosition().x-j_ziemi.x));
                    Q=1.1717-0.1003*(odleglosc-6371000)/1000;
                    T-=predkosc.y*t*0.0001;
                }
                czas.restart();
            }
            okno.clear(sf::Color(255,255,255));
            okno.draw(tlo6);
            okno.draw(tlo);
            okno.draw(tlo2);
            okno.draw(tlo3);
            okno.draw(tlo4);
            okno.draw(tlo5);
            okno.draw(silnik1);

            okno.setView(okno.getDefaultView());
            okno.setView(mapa);
                okno.draw(rakieta);
                if(wektor) //wyswietlenie wektorow
                {
                    Wektory grawitacja(Fg,rakieta.getPosition().x,rakieta.getPosition().y,sf::Color::Red,false);
                    Wektory ciag(F,rakieta.getPosition().x,rakieta.getPosition().y, sf::Color::Blue);
                    Wektory wypadkowa(F-Fg-opor,rakieta.getPosition().x,rakieta.getPosition().y, sf::Color::Yellow);
                    okno.draw(ciag.wektor);
                    okno.draw(grawitacja.wektor);
                    okno.draw(wypadkowa.wektor);
                }
            okno.setView(okno.getDefaultView());
                okno.draw(tlo_niewiem);
                wyswietlanie_danych(czas_podrozy,IprzyspieszenieI,IpredkoscI,odleglosc,kinetyczna,paliwo,numer,szybkosc_sym,T);
                    if(czas_podrozy>=710)
                    {
                        //komunikat "wejscie na orbite"
                    }
                    if(czas_podrozy>=720)
                    {
                        //komunikat "Trwa zmiana widoku na widok kosmiczny czy ocs w tym stylu"
                        //przekazywanie potrzebnych danych z tej czesci projektu do drugiej.
                        menu=2;
                    }
            okno.setView(mapa);
            okno.display();
        }
        while (okno.isOpen () && menu == 2)
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
                                for (int i = 0; i < planety.size (); i++) planety [i].pozycja_katowa = planety [i].omega_obiegu * time_current;
                                for (int i = 0; i < satelity.size (); i++) satelity [i].pozycja_katowa = 0.3 * pi;
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
                        if (buttons [0]->clicked && time_frame > 86400) time_frame -= 86400;
                        if (buttons [1]->clicked) time_frame += 86400;
                        if (buttons [2]->clicked && time_stop > 86400) time_stop -= 86400;
                        if (buttons [3]->clicked && time_stop < time_frame) time_stop += 86400;
                        if (buttons [4]->clicked && time_modifier > 1) time_modifier -= 1;
                        if (buttons [5]->clicked) time_modifier += 1;
                        if (buttons [6]->clicked) FK_Rakieta.v.y -= 1;
                        if (buttons [7]->clicked) FK_Rakieta.v.y += 1;
                        if (buttons [8]->clicked)
                        {
                            FK_Rakieta.koordynata_x = 1.49597870 * pow (10, 11) - 42231860.82;
                            FK_Rakieta.masa = 100;
                            FK_Rakieta.koordynata_y = 0;
                            FK_Rakieta.v.y = 4300.800000;
                            FK_Rakieta.v.x = 2071.187586;
                            time_current = 0;
                        }
                        for (int i = 0; i < buttons.size (); i++) buttons [i]->clicked = false;
                        break;
                    default:
                        break;
                }
            }
            if (simulation_start)
            {
                do if (time_current > czasy [time_current_index] && time_current_index < czasy.size () - 1) time_current_index += 1;
                while (time_current > czasy [time_current_index] && time_current_index < czasy.size () - 1);
                FK_Rakieta.koordynata_x = punkty [time_current_index].x;
                FK_Rakieta.koordynata_y = punkty [time_current_index].y;
                FK_Rakieta.pozycja_katowa = katy [time_current_index];
                FK_Rakieta.sprite.setPosition (FK_Rakieta.koordynata_x / 1000000, -FK_Rakieta.koordynata_y / 1000000);
                FK_Rakieta.sprite.setRotation (-FK_Rakieta.pozycja_katowa * 180 / pi + 90);
                for (int i = 0; i < time_modifier; i++)
                {
                    for (int i = 0; i < planety.size (); i++)
                    {
                        planety [i].Aktualizacja ();
                        planety [i].sprite.setPosition (planety [i].koordynata_x / 1000000, -planety [i].koordynata_y / 1000000);
                    }
                    for (int i = 0; i < satelity.size (); i++)
                    {
                        satelity [i].Aktualizacja ();
                        satelity [i].sprite.setPosition (satelity [i].koordynata_x / 1000000, -satelity [i].koordynata_y / 1000000);
                    }
                }
                time_current += time_modifier;
                if (time_current >= time_stop) simulation_start = false;
            }
            klip.setCenter (FK_Rakieta.koordynata_x / pow (10, 6), -FK_Rakieta.koordynata_y / pow (10, 6));
            okno.setView (klip);
            okno.clear (sf::Color::Black);
            okno.draw (Ktlo);
            for (int i = 0; i < planety.size (); i++) okno.draw (planety [i].sprite);
            for (int i = 0; i < satelity.size (); i++) okno.draw (satelity [i].sprite);
            okno.draw (FK_Rakieta.sprite);
            okno.setView (gui_view);
            for (int i = 0; i < buttons.size (); i++) okno.draw (buttons [i]->sprite);
            wyswietlanie_danych_kosmos(time_current, czas_podrozy, planety[0].koordynata_x, planety[0].koordynata_y, satelity[0].koordynata_x, satelity[0].koordynata_y, FK_Rakieta.koordynata_x, FK_Rakieta.koordynata_y);
            okno.display ();
        }
    }
    return 0;
}
void wyswietlanie_danych(float czas_podrozy, long double przyspieszenie, long double predkosc, long double odleglosc, long double kinetyczna, long double paliwo[3], int numer,float szybkosc_sym,float temperatura)
{
    ostringstream ss;
    ss.precision(3);                       // potrzebne do konwersji z inta/clocka na stringa
            int godziny = czas_podrozy/3600;
            int minuty = (czas_podrozy-godziny*3600)/60;
            int sekundy = czas_podrozy-godziny*3600-minuty*60;
    ss<<"Czas podr�zy: "<< godziny << "h " << minuty << "min " << sekundy << "s";
    string czas3 = ss.str();                    // przekazanie ss do zmiennej string
    sf::Text czas_wys(czas3, font[0], 22);
    czas_wys.setColor((sf::Color::White));
    czas_wys.setPosition(okno.getSize().x*0.71, okno.getSize().y*0.1);
    ss.str("");

    ss<<"Przyspieszenie: "<<przyspieszenie*1000000<<"m/s^2";
    string przyspieszenie_w = ss.str();
    sf::Text przyspieszenie_wys(przyspieszenie_w, font[0],22);
    przyspieszenie_wys.setColor((sf::Color::White));
    przyspieszenie_wys.setPosition(okno.getSize().x*0.71,okno.getSize().y*0.1+40);
    ss.str("");

    ss<<"Predkosc: "<<fixed<<predkosc*1000<<"m/s";
        if(predkosc*1000 > 950)
        {
            ss.str("");
            ss<<"Predkosc: "<<fixed<<predkosc*1000*10/36<<"km/h";
        }
    string predkosc_w = ss.str();
    sf::Text predkosc_wys(predkosc_w, font[0],22);
    predkosc_wys.setColor((sf::Color::White));
    predkosc_wys.setPosition(okno.getSize().x*0.71,okno.getSize().y*0.1+80);
    ss.str("");

    ss<<"Wysokosc: "<<odleglosc-6371000<<"m n.p.m";
        if(odleglosc-6371000>=1000)
        {
            ss.str("");
            ss<<"Wysokosc: "<<(odleglosc-6371000)/1000<<"km n.p.m";
        }
    string wysokosc_w = ss.str();
    sf::Text wysokosc_wys(wysokosc_w, font[0],22);
    wysokosc_wys.setColor((sf::Color::White));
    wysokosc_wys.setPosition(okno.getSize().x*0.71,okno.getSize().y*0.1+120);
//    ss.str("");
//    if(kinetyczna<=1000000)
//        ss<<"Energia kinetyczna "<<kinetyczna<<"J";
//    else
//        ss<<"Energia kinetyczna "<<kinetyczna/1000000<<"MJ";
//    string kinetyczna_w = ss.str();
//    sf::Text kinetyczna_wys(kinetyczna_w, font[0],20);
//    kinetyczna_wys.setColor((sf::Color::Black));
//    kinetyczna_wys.setPosition(410,560);
    long double pelne_zbiorniki[3]={2145798.08,443235.04,107095.43};
    ss.str("");
    ss<<"Zbiorniki paliwa";
    string pal_w = ss.str();
    sf::Text zbiorniki(pal_w, font[0],22);
    zbiorniki.setColor((sf::Color::White));
    zbiorniki.setPosition(okno.getSize().x*0.74,okno.getSize().y*0.1+200);

    ss.str("");
    ss<<"I";
    pal_w = ss.str();
    sf::Text zbiornik1(pal_w, font[0],22);
    zbiornik1.setColor((sf::Color::White));
    zbiornik1.setPosition(okno.getSize().x*0.72+19,okno.getSize().y*0.1+240);

    sf::RectangleShape czerwony1(sf::Vector2f(40,250));
    czerwony1.setPosition(okno.getSize().x*0.72,okno.getSize().y*0.1+280);
    czerwony1.setFillColor(sf::Color::Red);

    sf::RectangleShape zielony1(sf::Vector2f(40, paliwo[0]/pelne_zbiorniki[0]*250));
    zielony1.setOrigin(0,0);
    zielony1.setFillColor(sf::Color::Green);
    zielony1.setPosition(okno.getSize().x*0.72+40,okno.getSize().y*0.1+530);
    zielony1.setRotation(180);

    ss.str("");
    ss<<"II";
    pal_w = ss.str();
    sf::Text zbiornik2(pal_w, font[0],22);
    zbiornik2.setColor((sf::Color::White));
    zbiornik2.setPosition(okno.getSize().x*0.80+17,okno.getSize().y*0.1+240);

    sf::RectangleShape czerwony2(sf::Vector2f(40,250));
    czerwony2.setPosition(okno.getSize().x*0.80,okno.getSize().y*0.1+280);
    czerwony2.setFillColor(sf::Color::Red);
    sf::RectangleShape zielony2(sf::Vector2f(40, paliwo[1]/pelne_zbiorniki[1]*250));
    zielony2.setOrigin(0,0);
    zielony2.setFillColor(sf::Color::Green);
    zielony2.setPosition(okno.getSize().x*0.80+40,okno.getSize().y*0.1+530);
    zielony2.setRotation(180);

    ss.str("");
    ss<<"III";
    pal_w = ss.str();
    sf::Text zbiornik3(pal_w, font[0],22);
    zbiornik3.setColor((sf::Color::White));
    zbiornik3.setPosition(okno.getSize().x*0.88+16,okno.getSize().y*0.1+240);

    sf::RectangleShape czerwony3(sf::Vector2f(40,250));
    czerwony3.setPosition(okno.getSize().x*0.88,okno.getSize().y*0.1+280);
    czerwony3.setFillColor(sf::Color::Red);
    sf::RectangleShape zielony3(sf::Vector2f(40, paliwo[2]/pelne_zbiorniki[2]*250));
    zielony3.setOrigin(0,0);
    zielony3.setFillColor(sf::Color::Green);
    zielony3.setPosition(okno.getSize().x*0.88+40,okno.getSize().y*0.1+530);
    zielony3.setRotation(180);

    ss.str("");
    ss<<"x"<<szybkosc_sym;
    pal_w = ss.str();
    sf::Text przewijanie(pal_w, font[0],22);
    przewijanie.setColor((sf::Color::Black));
    przewijanie.setPosition(okno.getSize().x*0.08,okno.getSize().y*0.1);

    ss.str("");
    ss<<temperatura-273<<"*C";
    pal_w = ss.str();
    sf::Text temp(pal_w, font[0],22);
    temp.setColor((sf::Color::Black));
    temp.setPosition(okno.getSize().x*0.65,okno.getSize().y*0.1);


    okno.draw(przewijanie);
    okno.draw(temp);
    okno.draw(wysokosc_wys);                    // wyswietlenie wysokosci
    okno.draw(predkosc_wys);                    // wyswietlenie predkosci
    okno.draw(przyspieszenie_wys);              // wyswietlenie przyspieszenia
    okno.draw(czas_wys);                        // wyswietlenie czasu
//    okno.draw(kinetyczna_wys);                  // wyswietlenie energii kinetycznej
    okno.draw(zbiorniki);
    okno.draw(zbiornik1);
    okno.draw(czerwony1);
    if(paliwo[0]/pelne_zbiorniki[0]*200>0)
    okno.draw(zielony1);

    okno.draw(zbiornik2);
    okno.draw(czerwony2);
    if(paliwo[1]/pelne_zbiorniki[1]*200>0)
    okno.draw(zielony2);

    okno.draw(zbiornik3);
    okno.draw(czerwony3);
    if(paliwo[2]/pelne_zbiorniki[2]*200>0)
    okno.draw(zielony3);

    }

    void wyswietlanie_danych_kosmos(float czas_podrozy_w_kosmosie, float czas_podrozy_na_ziemi, double ziemia_x, double ziemia_y, double ksiezyc_x, double ksiezyc_y, double rakieta_x, double rakieta_y)
{
    float czas_podrozy = czas_podrozy_na_ziemi + czas_podrozy_w_kosmosie;
    ostringstream ss;
            int godziny = czas_podrozy/3600;
            int minuty = (czas_podrozy-godziny*3600)/60;
            int sekundy = czas_podrozy-godziny*3600-minuty*60;
    ss<< godziny << " h " << minuty << " min " << sekundy << " sekund";
    string czas3 = ss.str();
    sf::Text czas_wys(czas3, font[0], 20);
    czas_wys.setColor((sf::Color::White));
    czas_wys.setPosition(410, 510);
    ss.str("");

    double odleglosc_ziemia = sqrt((rakieta_x - ziemia_x) * (rakieta_x - ziemia_x) + (rakieta_y - ziemia_y) * (rakieta_y - ziemia_y));
    ss<<"Odleglosc od ziemi: "<<fixed<<odleglosc_ziemia / 1000<<"km";
    string odleglosc_ziemia_str = ss.str();
    sf::Text odleglosc_ziemia_txt(odleglosc_ziemia_str, font[0],20);
    odleglosc_ziemia_txt.setColor((sf::Color::White));
    odleglosc_ziemia_txt.setPosition(410,540);
    ss.str("");

    double odleglosc_ksiezyc = sqrt((rakieta_x - ksiezyc_x) * (rakieta_x - ksiezyc_x) + (rakieta_y - ksiezyc_y) * (rakieta_y - ksiezyc_y));
    ss<<"Odleglosc od ziemi: "<<fixed<<odleglosc_ksiezyc / 1000<<"km";
    string odleglosc_ksiezyc_str = ss.str();
    sf::Text odleglosc_ksiezyc_txt(odleglosc_ksiezyc_str, font[0],20);
    odleglosc_ksiezyc_txt.setColor((sf::Color::White));
    odleglosc_ksiezyc_txt.setPosition(410,570);
    ss.str("");

    okno.draw(odleglosc_ksiezyc_txt);
    okno.draw(odleglosc_ziemia_txt);
    okno.draw(czas_wys);

    }

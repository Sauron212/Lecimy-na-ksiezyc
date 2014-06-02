#include <SFML/Graphics.hpp>
#include <cmath>
#include <sstream>
#include <iostream>
#include "Faza_Kosmosu.h"
using namespace sf;
using namespace std;

void wyswietlanie_danych(float czas_podrozy, long double przyspieszenie, long double predkosc, long double odleglosc, long double kinetyczna, long double paliwo[3],int numer,float szybkosc_sym,float temperatura,double promien);
    sf::ContextSettings settings( 0,0,8,2,0);
void wyswietlanie_danych_kosmos(float czas_podrozy_w_kosmosie, float czas_podrozy_na_ziemi, double ziemia_x, double ziemia_y, double ksiezyc_x, double ksiezyc_y, double rakieta_x, double rakieta_y, int czas_symulacji, int czas_modyfikator, double predkosc);

sf::RenderWindow okno(sf::VideoMode(1000, 700), "Lecimy w kosmos",sf::Style::Default,settings);
sf::Font font[2];

int main()
{
    ios_base::sync_with_stdio(0);
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

    long double odleglosc = 6373000; // Faza Kosmosu -> 106370001   Faza Ziemska -> 6371000

    bool start=false;                      // ustawienie startu rakiety na klikniecie spacja.
    bool wektor=false;                      //ustawienie wyswietlania wektorow

    long double m=242186.12;      // masa rakiety (kg) na starcie
    long double paliwo[3]={2145798.08,443235.04,107095.43};
    long double Mc = m+paliwo[0]+paliwo[1]+paliwo[2];


    const long double G=0.0000000000667; // Stala grawitacji 6,67*10^-11
    const long double Mz=5.9736;          // Masa ziemi (kg), trzeba mnozyc razy 10^24
    long double g=G*Mz*pow(10,24)/(odleglosc*odleglosc);  // przyspieszenie grawitacyjne
    sf::Vector2f przyspieszenie; przyspieszenie.x=0; przyspieszenie.y=0;
    sf::Vector2f predkosc;predkosc.x;predkosc.y=0;// predkosc z jaka porusza sie rakieta
    sf::Vector2f F;F.x = 0; F.y=0;
    sf::Vector2f Fg; Fg.x = 0; Fg.y=0;
    long double kinetyczna=0;                   // Energia kinetyczna
    double IpredkoscI{0}, IprzyspieszenieI{0};
    double pole = 112.97;                       // pole jakie jest brane pod uwage w oporze
    double Cx = 0.3;                          // wspÃ³Â³czynnik siÂ³y oporu

    double t;
    double radiany;
    double rotacja=0;
    float Ma;
    double k=1.41;
    double R=287;
    float T=301.15;
    double predkosc_dzwieku=sqrt(k*R*T);
    double Q = 1.1717;                              //gestosc powietrza

    long double moc_silnikow[4] = {34318696.99,5104334.30,901223.04,0};     // Sila silnikow pierwszego stopnia w N
    int numer = 0;
    double pi=3.14159265359;
    sf::Vector2f opor; opor.x=0; opor.y=0;     // opor powietrza

    sf::Event zdarzenie;
    sf::Clock czas,czas_pod, czas_rotacja; // czas - okres ruchu, czas_pod - czas podrÃ³Â¿y, czas_rotacja - zmiana rotacji co sekudne;
    float czas_podrozy=-7; // nie wiem czemu -7 ale jak jest -6 to pokazuje -5 -,-
    float szybkosc_sym=1;

    sf::Texture ogien1_tek,ogien2_tek,tlo_tekstura1,tlo_tekstura2,tlo_tekstura3,tlo_tekstura4,tlo_tekstura5,tlo_tekstura6,tlo_rakieta, tlo_background;                       //    Stworzenie tekstury  tlo_tekstura
    tlo_tekstura1.loadFromFile("tlo00.png");
    tlo_tekstura2.loadFromFile("tlo11.png");
    tlo_tekstura3.loadFromFile("tlo2.png");
    tlo_tekstura4.loadFromFile("tlo3.png");
    tlo_tekstura5.loadFromFile("tlo4.png");
    tlo_tekstura6.loadFromFile("tlo5.png");
    ogien1_tek.loadFromFile("ogien1.png");
    ogien2_tek.loadFromFile("ogien2.png");
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
    tlo6.setTextureRect(sf::IntRect(0, 0,100000000, 23000000));

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

    sf::RectangleShape ogien1(sf::Vector2f(15,30));
    sf::RectangleShape ogien2(sf::Vector2f(11,30));
    ogien1.setTexture(&ogien1_tek);
    ogien2.setTexture(&ogien2_tek);
    ogien1.setPosition(0,0);
    ogien2.setPosition(100,100);


    double odchylenie;
    double odchylenie1;

    int A=6378200;
    int B=6356900;
    double predkoscPN;
    double predkoscWgore;
    double jadro[2];jadro[0]=0;jadro[1]=0;
    float polozenie[2];
    float KatZiemi = 28.75*pi/180;
                    polozenie[0]=A*cos(KatZiemi);
                    polozenie[1]=B*sin(KatZiemi);
                    odleglosc = sqrt(polozenie[0]*polozenie[0]+polozenie[1]*polozenie[1]);
    long double Promien = sqrt(pow(A*cos(KatZiemi),2)+pow(B*sin(KatZiemi),2));

    sf::Vector2f j_ziemi; j_ziemi.x=x-sin(28.45*pi/180)*Promien; j_ziemi.y=y+cos(28.45*pi/180)*Promien;

            sf::View mapa; //stwoerzenie widoku mapy
            mapa.reset(sf::FloatRect(100,10,800,600));
            mapa.setViewport(sf::FloatRect(0, 0, 1, 1));

    sf::Texture silnik_tex; //odpadajace silniki
    silnik_tex.loadFromFile("silnik.png");
    sf::Sprite silnik1;
    silnik1.setTexture(silnik_tex);
    bool zrzut1;

    sf::Text atmosfera;
    atmosfera.setColor((sf::Color::Black));
    atmosfera.setPosition(220,100);
    atmosfera.setFont(font[0]);
    atmosfera.setCharacterSize(25);

    /* ZMIENNE GRUPY JARKA : */
    Laduj_Uklad ();
    Laduj_Guziki ();
    int czas_symulacji, czas_aktualny, czas_aktualny_indeks, czas_modyfikator = 0;
    czas_symulacji = 7*86400;
    czas_modyfikator = 35;
    bool start_symulacji = false;
    sf::Vector2f mysz;
    sf::View widok;
    widok.reset (sf::FloatRect (0, 0, 658, 543));
    widok.setViewport (sf::FloatRect (0.026f, 0.088f, 0.658f, 0.775f));
    sf::View gui;
    gui.reset (sf::FloatRect (0, 0, 1000, 700));
    gui.setViewport (sf::FloatRect (0.0f, 0.0f, 1.0f, 1.0f));
    double o = satelity [0].pozycja_katowa;
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
                 ogien1.setPosition(rakieta.getPosition().x-7,rakieta.getPosition().y+54);
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
                g=G*Mz*pow(10,24)/(odleglosc*odleglosc);
                t = szybkosc_sym*czas.getElapsedTime().asMicroseconds()/1000.0;
                // spalanie 1 silnik
                if(czas_podrozy<0)
                    paliwo[0]-=t*(6562.2/1000);
                else if(czas_podrozy>=0 && czas_podrozy<=70)
                    paliwo[0]-=t*(13225.7/1000);
                else if(czas_podrozy>70 && czas_podrozy<=135)
                    paliwo[0]-=t*(13374.6246/1000);
                else if(czas_podrozy>=135 && czas_podrozy<=164&&paliwo[0]!=0)
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
                    if(czas_podrozy>=30 && czas_podrozy<80) // ...w wyznaczonym czasie (dodanie 6 sekund z powodu opÃ³Åºnionego startu)...
                        rotacja+=t*0.7280000/1000;//...obrÃ³t rakiety o tyle stopni
                    else if(czas_podrozy>=80 && czas_podrozy<135)
                         rotacja+=t*0.4696364/1000;
                    else if(czas_podrozy>=135 && czas_podrozy<165)
                       rotacja+=t*0.2970000/1000;
                    else if(czas_podrozy>=165 && czas_podrozy<185)
                        rotacja+=-t*0.5285000/1000;
                    else if(czas_podrozy>=185 && czas_podrozy<320)
                        rotacja+=t*0.0309630/1000;
                    else if(czas_podrozy>=320 && czas_podrozy<460)
                        rotacja+=t*0.0900000/1000;
                    else if(czas_podrozy>=460 && czas_podrozy<480)
                        rotacja+=-t*0.138000/1000;
                    else if(czas_podrozy>=480 && czas_podrozy<550)
                        rotacja+=t*0.0971429/1000;
                    else if(czas_podrozy>=550 && czas_podrozy<570)
                        rotacja+=-t*0.2070000/1000;
                    else if(czas_podrozy>=570 && czas_podrozy<640)
                        rotacja+=t*0.1117143/1000;
                    else if(czas_podrozy>=640 && czas_podrozy<705)
                        rotacja+=t*0.0486154/1000;

                t = szybkosc_sym*czas.getElapsedTime().asMicroseconds()/1000.0;
                    if(czas_podrozy>=164.0 && czas_podrozy<=196.0)
                        m-=278.5625/1000*t;
                    if(numer==0)
                    {
                        if(paliwo[0]>=315239.89)
                            moc_silnikow[0]+=40.5*t;
                        else if(paliwo[0]>315100.89 && paliwo[0]<315239.89) // wylaczenie centralnego silnika
                            moc_silnikow[0]=31942035;
                        else if(paliwo[0]<315239.89 && paliwo[0]>31994.14)
                            moc_silnikow[0]+=0.000000002*t;
                        else if(paliwo[0]>0 && paliwo[0]<=31994.14)  // odpada silnik
                        {
                            numer = 1;
                            m -= 132890.32;
                            paliwo[0]=0;
                            ogien1.setTexture(&ogien2_tek);
                           // rakieta.setTexture(&rakieta_cala2);
                            silnik1.setPosition(rakieta.getPosition().x,rakieta.getPosition().y);
                            break;
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

                if(czas_podrozy>=0.63)
                {
                    //odleglosc=sqrt( (rakieta.getPosition().y-j_ziemi.y)*(rakieta.getPosition().y-j_ziemi.y)+(rakieta.getPosition().x-j_ziemi.x)*(rakieta.getPosition().x-j_ziemi.x))+25.5;
                    odchylenie = (rakieta.getPosition().x-j_ziemi.x)/odleglosc;//sinus

                    rakieta.setRotation(rotacja); //wyliczenie wychylenia
                    Mc = m+paliwo[0]+paliwo[1]+paliwo[2];
                    radiany=(rakieta.getRotation()*pi)/180.0;
                    if(odleglosc-6373000<90000)
                    {
                        if(odleglosc-6373000<2000)
                        {
                            T=301.15-(odleglosc-6373000)*12.99/2000;
                            Q=1.1717-(odleglosc-6373000)*0.20067/2000;
                        }
                        else if(odleglosc-6373000<6000)
                        {
                            T=288.15-(odleglosc-6373000-2000)*10.97/2000;
                            Q=0.97103-(odleglosc-6373000-2000)*0.164185/2000;
                        }

                        else if(odleglosc-6373000<8000)
                        {
                            T=266.22-(odleglosc-6373000-6000)*13.93/2000;
                            Q=0.64266-(odleglosc-6373000-6000)*0.12126/2000;
                        }
                        else if(odleglosc-6373000<10000)
                        {
                            T=252.29-(odleglosc-6373000-8000)*13.94/2000;
                            Q=0.52140-(odleglosc-6373000-8000)*0.10313/2000;
                        }
                        else if(odleglosc-6373000<14000)
                        {
                            T=238.35-(odleglosc-6373000-10000)*13.93/2000;
                            Q=0.41827-(odleglosc-6373000-10000)*0.08002/2000;
                        }
                        else if(odleglosc-6373000<16000)
                        {
                            T=210.49-(odleglosc-6373000-14000)*7.34/2000;
                            Q=0.25823-(odleglosc-6373000-14000)*0.06609/2000;
                        }

                        else if(odleglosc-6373000<18000)
                        {
                            T=203.15+(odleglosc-6373000-16000)*4.23/2000;
                            Q=0.19214-(odleglosc-6373000-16000)*0.05693/2000;
                        }
                        else if(odleglosc-6373000<20000)
                        {
                            T=207.38+(odleglosc-6373000-18000)*4.37/2000;
                            Q=0.13521-(odleglosc-6373000-18000)*0.05693/2000;
                        }
                        else if(odleglosc-6373000<30000)
                        {
                            T=211.75+(odleglosc-6373000-20000)*4.36/2000;
                            Q=0.095807-(odleglosc-6373000-20000)*0.0153676/2000;
                        }
                        else if(odleglosc-6373000<34000)
                        {
                            T=233.55+(odleglosc-6373000-30000)*4.35/2000;
                            Q=0.018969-(odleglosc-6373000-30000)*0.004303/2000;//DALEJ!
                        }
                        else if(odleglosc-6373000<36000)
                        {
                            T=242.25+(odleglosc-6373000-34000)*4.34/2000;
                            Q=0.010363-(odleglosc-6373000-34000)*0.0026382/2000;
                        }
                        else if(odleglosc-6373000<38000)
                        {
                            T=246.59+(odleglosc-6373000-36000)*4.86/2000;
                            Q=0.0077248-(odleglosc-6373000-36000)*0.0019469/2000;
                        }
                        else if(odleglosc-6373000<42000)
                        {
                            T=251.45+(odleglosc-6373000-38000)*4.93/2000;
                            Q=0.0057779-(odleglosc-6373000-38000)*0.0012456/2000;
                        }
                        else if(odleglosc-6373000<44000)
                        {
                            T=261.31+(odleglosc-6373000-42000)*3.13/2000;
                            Q=0.0032867-(odleglosc-6373000-42000)*0.0007723/2000;
                        }
                        else if(odleglosc-6373000<46000)
                        {
                            T=264.44+(odleglosc-6373000-44000)*2.75/2000;
                            Q=0.0025144-(odleglosc-6373000-44000)*0.0005823/2000;
                        }
                        else if(odleglosc-6373000<48000)
                        {
                            T=267.19+(odleglosc-6373000-46000)*1.96/2000;
                            Q=0.0019321-(odleglosc-6373000-46000)*0.000439/2000;
                        }
                        else if(odleglosc-6373000<50000)
                        {
                            T=269.15;
                            Q=0.0014931-(odleglosc-6373000-48000)*0.0003299/2000;
                        }
                        else if(odleglosc-6373000<52000)
                        {
                            T=269.15-(odleglosc-6373000-50000)*1.81/2000;
                            Q=0.0011632-(odleglosc-6373000-50000)*0.00025123/2000;
                        }
                        else if(odleglosc-6373000<54000)
                        {
                            T=267.34-(odleglosc-6373000-52000)*2.36/2000;
                            Q=0.00091197-(odleglosc-6373000-52000)*0.00019693/2000;
                        }
                        else if(odleglosc-6373000<56000)
                        {
                            T=264.98-(odleglosc-6373000-54000)*3.48/2000;
                            Q=0.00071504-(odleglosc-6373000-54000)*0.000134565/2000;
                        }
                        else if(odleglosc-6373000<58000)
                        {
                            T=261.50-(odleglosc-6373000-56000)*7.46/2000;
                            Q=0.00056185-(odleglosc-6373000-56000)*0.00007094/2000;
                        }
                        else if(odleglosc-6373000<62000)
                        {
                            T=254.04-(odleglosc-6373000-58000)*7.45/2000;
                            Q=0.00044591-(odleglosc-6373000-58000)*0.000085385/2000;
                        }
                        else if(odleglosc-6373000<64000)
                        {
                            T=239.14-(odleglosc-6373000-62000)*7.44/2000;
                            Q=0.00027514-(odleglosc-6373000-62000)*0.00006141/2000;
                        }
                        else if(odleglosc-6373000<66000)
                        {
                            T=231.70-(odleglosc-6373000-64000)*7.43/2000;
                            Q=0.00021373-(odleglosc-6373000-64000)*0.00004904/2000;
                        }
                        else if(odleglosc-6373000<68000)
                        {
                            T=224.27-(odleglosc-6373000-66000)*7.44/2000;
                            Q=0.00016469-(odleglosc-6373000-66000)*0.00003889/2000;
                        }
                        else if(odleglosc-6373000<70000)
                        {
                            T=216.83-(odleglosc-6373000-68000)*7.42/2000;
                            Q=0.00012581-(odleglosc-6373000-68000)*0.000030576/2000;
                        }
                        else if(odleglosc-6373000<72000)
                        {
                            T=209.41-(odleglosc-6373000-70000)*4.91/2000;
                            Q=0.000095234-(odleglosc-6373000-70000)*0.000024656/2000;
                        }
                        else if(odleglosc-6373000<74000)
                        {
                            T=204.50-(odleglosc-6373000-72000)*2.92/2000;
                            Q=0.000070578-(odleglosc-6373000-72000)*0.000019021/2000;
                        }
                        else if(odleglosc-6373000<78000)
                        {
                            T=201.58-(odleglosc-6373000-74000)*2.93/2000;
                            Q=0.000051557-(odleglosc-6373000-74000)*0.0000122045/2000;
                        }
                        else if(odleglosc-6373000<86000)
                        {
                            T=195.72-(odleglosc-6373000-78000)*2.92/2000;
                            Q=0.000027148-(odleglosc-6373000-78000)*0.00000442522/2000;
                        }
                        else if(odleglosc-6373000<90000)
                        {
                            T=181.13-(odleglosc-6373000-86000)*2.91/2000;
                            Q=0.0000050219-(odleglosc-6373000-86000)*0.000001494/2000;
                        }
                        opor.x = Cx*(Q*pow(predkosc.x,2)/2)*pole;          // uatkualnienie oporu powietrza
                        opor.y = Cx*(Q*pow(predkosc.y,2)/2)*pole;
                    }
                    else
                    {
                        opor.x=0;
                        opor.y=0;
                    }

                    Fg.x =0;Fg.y = Mc*g;
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
                       //if(czas_podrozy>1.3)
                       //{cout << paliwo[1]+paliwo[0]+paliwo[2] << " "<< Cx << " "<<Ma<<" " << moc_silnikow[0]<<endl;cin.get();cin.get();}

                    kinetyczna=0.5*m*pow(IpredkoscI,2);                                          // obliczanie Ek
                    ogien1.setRotation(rakieta.getRotation());
                    ogien1.setPosition(rakieta.getPosition().x-7-sin(radiany)*56,rakieta.getPosition().y+cos(radiany)*52);
                    rakieta.move(predkosc.x*t, -predkosc.y*t);
                    ogien1.setPosition(rakieta.getPosition().x-7-sin(radiany)*56,rakieta.getPosition().y+cos(radiany)*52);

                    if(rakieta.getPosition().y<244 || czas_podrozy>60 )   // ustawienie rakiety na srodku ekranu
                    {
                        mapa.setCenter(rakieta.getPosition().x+110,rakieta.getPosition().y+66);//podÄ…zanie za rakietÄ…
                    }
                   //  odleglosc=sqrt( (rakieta.getPosition().y-j_ziemi.y)*(rakieta.getPosition().y-j_ziemi.y)+(rakieta.getPosition().x-j_ziemi.x)*(rakieta.getPosition().x-j_ziemi.x))+25.5;
                    // Ziemia jajo
                    predkoscPN = cos(72*pi/180)*predkosc.x;
                    predkoscWgore = predkosc.y;
                    polozenie[0]+=predkoscWgore*t; polozenie[1]+=predkoscPN*t;
                    odleglosc = sqrt(pow(polozenie[0],2)+pow(polozenie[1],2));
                    Promien = sqrt(pow(A*cos(KatZiemi),2)+pow(B*sin(KatZiemi),2));
                    KatZiemi = asin(polozenie[1]/odleglosc);
//                    cout.precision(15);
//                    cout << odleglosc<< " " << Promien << endl;
//                    cout << (odleglosc-Promien)*cos(KatZiemi);
//                    cin.get();
//                    cin.get();
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
            okno.draw(ogien1);
            okno.draw(ogien2);

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
                wyswietlanie_danych(czas_podrozy,IprzyspieszenieI,IpredkoscI,odleglosc,kinetyczna,paliwo,numer,szybkosc_sym,T,Promien);

                if(odleglosc-Promien>=12000 && odleglosc-Promien<20000)
                {
                atmosfera.setString("Weszlismy w stratosfere");
                okno.draw(atmosfera);
                }
                else if(odleglosc-Promien>=50000 && odleglosc-Promien<60000)
                {
                atmosfera.setColor((sf::Color::White));
                atmosfera.setString("Weszlismy w mezosfere");
                okno.draw(atmosfera);
                }
                else if(odleglosc-Promien>=80000 && odleglosc-Promien<90000)
                {
                atmosfera.setString("Weszlismy w termosfere");
                okno.draw(atmosfera);
                }

                    if(czas_podrozy>=710)
                    {
                        atmosfera.setString("Wejscie na orbite");
                        okno.draw(atmosfera);
                    }
                    if(czas_podrozy>=720)
                    {
                        //FK_Rakieta.koordynata_x = cos (kat - (rakieta.getPosition ().x / (rakieta.getPosition ().y + j_ziemi.y))) * (rakieta.getPosition ().y + j_ziemi.y);
                        //FK_Rakieta.koordynata_y = sin (kat - (rakieta.getPosition ().x / (rakieta.getPosition ().y + j_ziemi.y))) * (rakieta.getPosition ().y + j_ziemi.y);
                        //FK_Rakieta.pozycja_katowa = (rakieta.getRotation () + (kat - (rakieta.getPosition ().x / (rakieta.getPosition ().y + j_ziemi.y))) * 180 / pi - 180) * pi / 180;
                        //FK_Rakieta.sprite.setPosition (FK_Rakieta.koordynata_x / 1000000, -FK_Rakieta.koordynata_y / 1000000);
                        //FK_Rakieta.sprite.setRotation (-FK_Rakieta.pozycja_katowa * 180 / pi + 90);
                        //FK_Rakieta.v.x = cos (FK_Rakieta.pozycja_katowa) * IpredkoscI * 1000;
                        //FK_Rakieta.v.y = sin (FK_Rakieta.pozycja_katowa) * IpredkoscI * 1000;
                        FK_Rakieta.koordynata_x = 6089920;
                        FK_Rakieta.koordynata_y = 2944900;
                        FK_Rakieta.v.y = -6273.57;
                        FK_Rakieta.v.x = 2453.91;
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
                                Symulacja (czas_symulacji);
                                break;
                            case sf::Keyboard::Return:
                                start_symulacji = !start_symulacji;
                                czas_aktualny = 0;
                                czas_aktualny_indeks = 0;
                                for (int i = 0; i < planety.size (); i++) planety [i].pozycja_katowa = planety [i].omega_obiegu * czas_aktualny;
                                for (int i = 0; i < satelity.size (); i++) satelity [i].pozycja_katowa = o;
                                break;
                            case sf::Keyboard::Add:
                                widok.setSize (widok.getSize () + sf::Vector2f (-12, -9.9027));
                                break;
                            case sf::Keyboard::Subtract:
                                widok.setSize (widok.getSize () + sf::Vector2f (12, 9.9027));
                                break;
                            case sf::Keyboard::R:
                                std::cout<<string (100, '\n');
                                std::cout<<"WprowadŸ x rakiety."<<std::endl;
                                std::cin>>FK_Rakieta.koordynata_x;
                                std::cout<<"WprowadŸ y rakiety."<<std::endl;
                                std::cin>>FK_Rakieta.koordynata_y;
                                std::cout<<"WprowadŸ x predkosci rakiety."<<std::endl;
                                std::cin>>FK_Rakieta.v.x;
                                std::cout<<"WprowadŸ y predkosci rakiety."<<std::endl;
                                std::cin>>FK_Rakieta.v.y;
                                std::cout<<"WprowadŸ mase rakiety."<<std::endl;
                                std::cin>>FK_Rakieta.masa;
                                std::cout<<"WprowadŸ ciag silnika rakiety."<<std::endl;
                                std::cin>>FK_Rakieta.ciag;
                                std::cout<<"WprowadŸ czas dzialania silnika rakiety."<<std::endl;
                                std::cin>>czas_1.czas_trwania;
                                std::cout<<"WprowadŸ czas zaplonu silnika rakiety."<<std::endl;
                                std::cin>>czas_1.start;
                                czasy_silnika.push_back (czas_1);
                                std::cout<<"WprowadŸ pozycje katowa ksiezyca."<<std::endl;
                                std::cin>>satelity [0].pozycja_katowa;
                                o = satelity [0].pozycja_katowa;
                                break;
                            default:
                                break;
                        }
                        break;
                    case sf::Event::MouseButtonReleased:
                        mysz = okno.mapPixelToCoords (sf::Mouse::getPosition (okno), gui);
                        for (int i = 0; i < buttons.size (); i++) if (((mysz.x > buttons [i]->koordynata_x - buttons [i]->szerokosc / 2) && (mysz.x < buttons [i]->koordynata_x + buttons [i]->szerokosc / 2)) && ((mysz.y > buttons [i]->koordynata_y - buttons [i]->wysokosc / 2) && (mysz.y < buttons [i]->koordynata_y + buttons [i]->wysokosc / 2))) buttons [i]->clicked = true;
                        if (buttons [0]->clicked && czas_symulacji > 10000 && !start_symulacji) czas_symulacji -= 10000;
                        if (buttons [1]->clicked && !start_symulacji) czas_symulacji += 10000;
                        if (buttons [2]->clicked && czas_modyfikator > 1) czas_modyfikator -= 1;
                        if (buttons [3]->clicked) czas_modyfikator += 1;;
                        for (int i = 0; i < buttons.size (); i++) buttons [i]->clicked = false;
                        break;
                    default:
                        break;
                }
            }
            if (start_symulacji)
            {
                do if (czas_aktualny > czasy [czas_aktualny_indeks] && czas_aktualny_indeks < czasy.size () - 1) czas_aktualny_indeks += 1;
                while (czas_aktualny > czasy [czas_aktualny_indeks] && czas_aktualny_indeks < czasy.size () - 1);
                FK_Rakieta.koordynata_x = punkty [czas_aktualny_indeks].x;
                FK_Rakieta.koordynata_y = punkty [czas_aktualny_indeks].y;
                FK_Rakieta.pozycja_katowa = katy [czas_aktualny_indeks];
                FK_Rakieta.sprite.setPosition (FK_Rakieta.koordynata_x / 1000000, -FK_Rakieta.koordynata_y / 1000000);
                FK_Rakieta.sprite.setRotation (-FK_Rakieta.pozycja_katowa * 180 / pi + 90);
                FK_Rakieta.v.x = predkosci [czas_aktualny_indeks] * cos (FK_Rakieta.pozycja_katowa);
                FK_Rakieta.v.y = predkosci [czas_aktualny_indeks] * sin (FK_Rakieta.pozycja_katowa);
                for (int i = 0; i < czas_modyfikator; i++)
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
                czas_aktualny += czas_modyfikator;
                if (czas_aktualny >= czas_symulacji) start_symulacji = false;
            }
            okno.clear (sf::Color::Black);
            widok.setCenter (FK_Rakieta.koordynata_x / pow (10, 6), -FK_Rakieta.koordynata_y / pow (10, 6));
            okno.setView (widok);
            for (int i = 0; i < planety.size (); i++) okno.draw (planety [i].sprite);
            for (int i = 0; i < satelity.size (); i++) okno.draw (satelity [i].sprite);
            okno.draw (FK_Rakieta.sprite);
            okno.setView (gui);
            okno.draw (tlo_niewiem);
            for (int i = 0; i < buttons.size (); i++) okno.draw (buttons [i]->sprite);
            wyswietlanie_danych_kosmos(czas_aktualny, czas_podrozy, planety[0].koordynata_x, planety[0].koordynata_y, satelity[0].koordynata_x, satelity[0].koordynata_y, FK_Rakieta.koordynata_x, FK_Rakieta.koordynata_y, czas_symulacji, czas_modyfikator, sqrt (pow (FK_Rakieta.v.x, 2) + pow (FK_Rakieta.v.y, 2)));
            okno.display ();
        }
    }
    return 0;
}
void wyswietlanie_danych(float czas_podrozy, long double przyspieszenie, long double predkosc, long double odleglosc, long double kinetyczna, long double paliwo[3], int numer,float szybkosc_sym,float temperatura,double promien)
{
    ostringstream ss;
    ss.precision(3);                       // potrzebne do konwersji z inta/clocka na stringa
            int godziny = czas_podrozy/3600;
            int minuty = (czas_podrozy-godziny*3600)/60;
            int sekundy = czas_podrozy-godziny*3600-minuty*60;
    ss<<"Czas podrózy: "<< godziny << "h " << minuty << "min " << sekundy << "s";
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
            ss<<"Predkosc: "<<fixed<<predkosc<<"km/s";
        }
    string predkosc_w = ss.str();
    sf::Text predkosc_wys(predkosc_w, font[0],22);
    predkosc_wys.setColor((sf::Color::White));
    predkosc_wys.setPosition(okno.getSize().x*0.71,okno.getSize().y*0.1+80);
    ss.str("");

    ss<<"Wysokosc: "<<odleglosc-promien<<"m n.p.m";
        if(odleglosc-promien>=1000)
        {
            ss.str("");
            ss<<"Wysokosc: "<<(odleglosc-promien)/1000<<"km n.p.m";
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

void wyswietlanie_danych_kosmos (float czas_podrozy_w_kosmosie, float czas_podrozy_na_ziemi, double ziemia_x, double ziemia_y, double ksiezyc_x, double ksiezyc_y, double rakieta_x, double rakieta_y, int czas_symulacji, int czas_modyfikator, double predkosc)
{
    float czas_podrozy = czas_podrozy_na_ziemi + czas_podrozy_w_kosmosie;
    ostringstream ss;
    ss.precision (2);
            int godziny = czas_podrozy/3600;
            int minuty = (czas_podrozy-godziny*3600)/60;
            int sekundy = czas_podrozy-godziny*3600-minuty*60;
    ss<< godziny << " h " << minuty << " min " << sekundy << " sekund";
    string czas3 = ss.str();
    sf::Text czas_wys(czas3, font[0], 20);
    czas_wys.setColor((sf::Color::White));
    czas_wys.setPosition(710, 100);
    ss.str("");

    sf::Text dane ("Dane:", font [0], 20);
    dane.setPosition (710, 62);
    dane.setColor (sf::Color::White);

    double odleglosc_ziemia = sqrt((rakieta_x - ziemia_x) * (rakieta_x - ziemia_x) + (rakieta_y - ziemia_y) * (rakieta_y - ziemia_y)) / 1000;
    ss<<"Odl. od ziemi: "<<fixed<<odleglosc_ziemia<<"km";
    string odleglosc_ziemia_str = ss.str();
    sf::Text odleglosc_ziemia_txt(odleglosc_ziemia_str, font[0],20);
    odleglosc_ziemia_txt.setColor((sf::Color::White));
    odleglosc_ziemia_txt.setPosition(710, 120);
    ss.str("");

    double odleglosc_ksiezyc = sqrt((rakieta_x - ksiezyc_x) * (rakieta_x - ksiezyc_x) + (rakieta_y - ksiezyc_y) * (rakieta_y - ksiezyc_y)) / 1000;
    ss<<"Odl. od ksiezyca: "<<fixed<<odleglosc_ksiezyc<<"km";
    string odleglosc_ksiezyc_str = ss.str();
    sf::Text odleglosc_ksiezyc_txt(odleglosc_ksiezyc_str, font[0],20);
    odleglosc_ksiezyc_txt.setColor((sf::Color::White));
    odleglosc_ksiezyc_txt.setPosition(710, 140);
    ss.str("");

    ss<<"Predkosc: "<<fixed<<predkosc<<"m/s";
    string predkosc_string = ss.str();
    sf::Text predkosc_tekst (predkosc_string, font[0],20);
    predkosc_tekst.setColor((sf::Color::White));
    predkosc_tekst.setPosition(710, 160);
    ss.str("");

    sf::Text symulacja_tekst ("Symulacja:", font [0], 20);
    symulacja_tekst.setPosition (710, 202);
    symulacja_tekst.setColor (sf::Color::White);

    ss<<"Czas: "<<czas_symulacji<<"s";
    string czas_symulacji_string = ss.str();
    sf::Text czas_symulacji_tekst (czas_symulacji_string, font [0], 20);
    czas_symulacji_tekst.setColor((sf::Color::White));
    czas_symulacji_tekst.setPosition (710, 244);
    ss.str("");

    ss<<"Szybkosc: "<<czas_modyfikator;
    string czas_modyfikator_string = ss.str();
    sf::Text czas_modyfikator_tekst (czas_modyfikator_string, font [0], 20);
    czas_modyfikator_tekst.setColor((sf::Color::White));
    czas_modyfikator_tekst.setPosition (710, 264);
    ss.str("");

    okno.draw(dane);
    okno.draw(symulacja_tekst);
    okno.draw(odleglosc_ksiezyc_txt);
    okno.draw(odleglosc_ziemia_txt);
    okno.draw(czas_wys);
    okno.draw(czas_symulacji_tekst);
    okno.draw(czas_modyfikator_tekst);
    okno.draw(predkosc_tekst);
}

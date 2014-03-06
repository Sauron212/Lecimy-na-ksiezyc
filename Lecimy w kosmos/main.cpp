#include <SFML/Graphics.hpp>
#include <cmath>
using namespace sf;
int main()
{
    int odleglosc{6370},x{375},y{400};
    // odleglosc rakeity od srodka ziemi ; x,y - wspolrzedne rakiety wzgledem okna;
    bool start=false;
    // ustawienie startu rakiety na klikniecie spacja.
    sf::Event zdarzenie;
    sf::RenderWindow okno( sf::VideoMode( 800, 600 ), "Lecimy w kosmos" );
    sf::Clock czas;
    sf::Texture tlo_tekstura;                       /*    Stworzenie tekstury  tlo_tekstura   */
    tlo_tekstura.loadFromFile("tlo-start.png");
    sf::Sprite tlo;                                 /*    Ustawienie Sprite z tekstura tlo_tekstura     */
    tlo.setTexture(tlo_tekstura);
    tlo.setPosition(0,-4297);
    sf::RectangleShape rakieta(sf::Vector2f(50,100));
    rakieta.setPosition(x,y);
    rakieta.setFillColor(sf::Color(0,0,0));
    while(okno.isOpen())
    {
        while(odleglosc <= 206370)
        {
            while(okno.pollEvent(zdarzenie))
            {
                if( zdarzenie.type == sf::Event::Closed )
                 okno.close();
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                 start=true;

            }
            if(czas.getElapsedTime().asMilliseconds() >= 3 && start)
            {
                if(rakieta.getPosition().y>=300 )
                    rakieta.move(0,-1);
                else
                    tlo.move(0,1);
                czas.restart();
            }
            okno.clear(sf::Color(255,255,255));
            okno.draw(tlo);
            okno.draw(rakieta);
            okno.display();
        }
        while(odleglosc > 206370)
        {

        }
    }
    return 0;
}

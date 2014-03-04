#include <SFML/Graphics.hpp>
#include <cmath>
using namespace sf;
int main()
{
    int odleglosc = 6370;
    int x,y;
    x = 400;
    y = 550;
    sf::Event zdarzenie;
    sf::RenderWindow okno( sf::VideoMode( 800, 600 ), "Lecimy w kosmos" );
    sf::Clock czas;
    sf::RectangleShape rakieta( sf::Vector2f( 25, 50 ) );
    rakieta.setPosition( x, y );
    while( okno.isOpen() )
    {
        while(odleglosc <= 206370)
        {
            sf::Event event;
            while( okno.pollEvent( event ) )
            {
                if( event.type == sf::Event::Closed )
                 okno.close();

            }
            if(czas.getElapsedTime().asMilliseconds() >= 3)
            {
                rakieta.move(0,-1);
                czas.restart();
            }

            okno.clear( sf::Color( 0, 127, 255 ) );
            okno.draw( rakieta );
            okno.display();
        }
        while(odleglosc > 206370)
        {

        }
    }
    return 0;
}

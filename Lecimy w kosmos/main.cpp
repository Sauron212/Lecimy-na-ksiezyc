#include <SFML/Graphics.hpp>
#include <cmath>
using namespace sf;
int main()
{
    int odleglosc = 6370;
    sf::Event zdarzenie;
    sf::RenderWindow okno( sf::VideoMode( 800, 600 ), "Lecimy w kosmos" );
    sf::Clock czas;
    while( okno.isOpen() )
    {
        while(odleglosc <= 206370)
        {
            while( okno.pollEvent( zdarzenie ) )
            {
                if( zdarzenie.type == sf::Event::Closed )
                     okno.close();
            }
            okno.clear(sf::Color(255,225,255));
            okno.display();
        }
        while(odleglosc > 206370)
        {

        }
    }
    return 0;
}

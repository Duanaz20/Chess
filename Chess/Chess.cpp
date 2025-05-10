// Chess.cpp : Defines the entry point for the application.
//

#include "board.h"
#include <SFML/Graphics.hpp>
using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Chess Test");

    // Create a test pawn (white, at position (3, 3))
    //Pawn testPawn(true, { 3, 3 }); // x=3, y=3 — means (300px, 300px)

    Board board;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

		board.draw(window); // Draw the board

        window.display();
    }
	return 0;
}

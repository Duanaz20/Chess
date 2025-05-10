#include "board.h"

void Board::draw(sf::RenderWindow& window) {
    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            sf::RectangleShape square(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            square.setPosition(col * TILE_SIZE, row * TILE_SIZE);
            bool isWhite = (row + col) % 2 == 0;
            square.setFillColor(isWhite ? sf::Color(240, 217, 181) : sf::Color(181, 136, 99)); // Light and dark squares
            window.draw(square);
        }
    }
}

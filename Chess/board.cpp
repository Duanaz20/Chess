#include "board.h"

Board::Board() {
    // Add white pawns
    for (int i = 0; i < 8; ++i) {
        pieces.push_back(std::make_unique<Pawn>(true, std::make_pair(i, 6)));
    }

    // Add black pawns
    for (int i = 0; i < 8; ++i) {
        pieces.push_back(std::make_unique<Pawn>(false, std::make_pair(i, 1)));
    }

    // Add a few more if you want (rooks, knights, etc.)
}


void Board::draw(sf::RenderWindow& window) {
	// Draw the chessboard
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            sf::RectangleShape square(sf::Vector2f(100, 100));
            square.setPosition(col * 100, row * 100);
            if ((row + col) % 2 == 0)
                square.setFillColor(sf::Color(222, 184, 135)); // light brown
            else
                square.setFillColor(sf::Color(139, 69, 19));   // dark brown
            window.draw(square);
        }
    }

    // Draw all the pieces
    for (auto& piece : pieces) {
        piece->draw(window);
    }
}

#include "board.h"

void drawBoard(sf::RenderWindow& window) {
    const int squareSize = 100;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j) {
            sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
            square.setPosition(i * squareSize, j * squareSize);
            square.setFillColor((i + j) % 2 == 0 ? sf::Color(222, 184, 135) : sf::Color(139, 69, 19));
            window.draw(square);
        }
}

void drawPieces(sf::RenderWindow& window, std::vector<Chesspiece*>& pieces) {
    const int squareSize = 100;
    for (auto& piece : pieces) {
        sf::Sprite sprite(piece->texture);
        sprite.setPosition(piece->position.first * squareSize - 3, piece->position.second * squareSize - 5);
        sprite.setScale(0.8f, 0.8f);
        window.draw(sprite);
    }
}

// Function to check if the king is in check
bool isKingInCheck(bool kingColor, const std::vector<Chesspiece*>& pieceslist) {
    std::pair<int, int> kingPos;

    // Find the king's position
    for (auto* piece : pieceslist) {
        if (piece->type == "King" && piece->color == kingColor) {
            kingPos = piece->position;
            break;
        }
    }

    // Check if any opposing piece can move to the king
    for (auto* piece : pieceslist) {
        if (piece->color != kingColor) {
            for (const auto& move : piece->getPossibleMoves(pieceslist)) {
                if (move == kingPos)
                    return true;
            }
        }
    }

    return false;
}


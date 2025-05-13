// Chess.cpp : Defines the entry point for the application.
//
#include<iostream>
#include "board.h"
#include "pieces.h"
#include <SFML/Graphics.hpp>
using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML Chess");

    std::vector<Chesspiece*> pieceslist;

    // White pawns
    for (int i = 0; i < 8; ++i)
        pieceslist.push_back(new Pawn(true, { i, 6 }));

    // White back row
    pieceslist.push_back(new Rook(true, { 0, 7 }));
    pieceslist.push_back(new Knight(true, { 1, 7 }));
    pieceslist.push_back(new Bishop(true, { 2, 7 }));
    pieceslist.push_back(new Queen(true, { 3, 7 }));
    pieceslist.push_back(new King(true, { 4, 7 }));
    pieceslist.push_back(new Bishop(true, { 5, 7 }));
    pieceslist.push_back(new Knight(true, { 6, 7 }));
    pieceslist.push_back(new Rook(true, { 7, 7 }));

    // Black pawns
    for (int i = 0; i < 8; ++i)
        pieceslist.push_back(new Pawn(false, { i, 1 }));

    // Black back row
    pieceslist.push_back(new Rook(false, { 0, 0 }));
    pieceslist.push_back(new Knight(false, { 1, 0 }));
    pieceslist.push_back(new Bishop(false, { 2, 0 }));
    pieceslist.push_back(new Queen(false, { 3, 0 }));
    pieceslist.push_back(new King(false, { 4, 0 }));
    pieceslist.push_back(new Bishop(false, { 5, 0 }));
    pieceslist.push_back(new Knight(false, { 6, 0 }));
    pieceslist.push_back(new Rook(false, { 7, 0 }));

    bool selection = false;
    Chesspiece* selectedPiece = nullptr;
    bool whiteTurn = true;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int x = mousePos.x / 100;
                int y = mousePos.y / 100;

                bool moveMade = false;

                if (selection && selectedPiece != nullptr) {
                    for (auto& move : selectedPiece->getPossibleMoves(pieceslist)) {
                        if (move.first == x && move.second == y) {
                            for (auto it = pieceslist.begin(); it != pieceslist.end(); ++it) {
                                if ((*it)->position.first == x && (*it)->position.second == y &&
                                    (*it)->color != selectedPiece->color) {
									if ((*it)->type == "King") {
										std::cout << "Checkmate!" << std::endl;
										window.close();
									}
                                    delete* it;
                                    pieceslist.erase(it);
                                    break;
                                }
                            }
                            selectedPiece->position = move;
                            selection = false;
                            selectedPiece = nullptr;
                            moveMade = true;
                            whiteTurn = !whiteTurn;

                            if (isKingInCheck(!whiteTurn, pieceslist)) {
                                std::cout << "Check!" << std::endl;
                            }

                            break;
                        }
                    }
                }

                if (!moveMade) {
                    selection = false;
                    selectedPiece = nullptr;
                    for (auto& piece : pieceslist) {
                        if (piece->position.first == x && piece->position.second == y && piece->color == whiteTurn)
                        {
                            selectedPiece = piece;
                            selection = true;
                            break;
                        }
                    }
                }
            }
        }

        window.clear();
        drawBoard(window);

        if (selection && selectedPiece != nullptr) {
            sf::RectangleShape highlight(sf::Vector2f(100.f, 100.f));
            highlight.setPosition(selectedPiece->position.first * 100, selectedPiece->position.second * 100);
            highlight.setFillColor(sf::Color(130, 228, 124)); // Light green
            window.draw(highlight);

            for (const auto& move : selectedPiece->getPossibleMoves(pieceslist)) {

                sf::RectangleShape square(sf::Vector2f(100.f, 100.f));
                square.setPosition(move.first * 100, move.second * 100);
                square.setFillColor(sf::Color(173, 216, 230)); // Light blue
                window.draw(square);
            }
        }

        drawPieces(window, pieceslist);
        window.display();
    }

    // Clean up
    for (auto* piece : pieceslist)
        delete piece;

    return 0;
}

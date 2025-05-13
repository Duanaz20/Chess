#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "pieces.h"

void drawBoard(sf::RenderWindow& window);
void drawPieces(sf::RenderWindow& window, std::vector<Chesspiece*>& pieces);
bool isKingInCheck(bool kingColor, const std::vector<Chesspiece*>& pieceslist);

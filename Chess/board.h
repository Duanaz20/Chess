#pragma once
#include <SFML/Graphics.hpp>
#include "pieces.h" // Include the pieces header
#include <vector>
#include <iostream>


class Board {
private:
	std::vector<std::unique_ptr<Chesspiece>> pieces; // 2D vector to hold chess pieces on the board
public:
    Board();
    static const int SIZE = 8;          // 8x8 board
    static const int TILE_SIZE = 100;   // 100 pixels per tile

    void draw(sf::RenderWindow& window);
};

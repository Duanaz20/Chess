#pragma once
#include <SFML/Graphics.hpp>
#include "pieces.h" // Include the pieces header
#include <vector>
#include <iostream>


class Board {
public:
    static const int SIZE = 8;          // 8x8 board
    static const int TILE_SIZE = 100;   // 100 pixels per tile

    void draw(sf::RenderWindow& window);
};

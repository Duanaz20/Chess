#pragma once
#include <string>
#include <utility> // for std::pair
#include <SFML/Graphics.hpp>

class Chesspiece
{
protected:
    std::string type; // type of chess piece (e.g., pawn, knight, bishop, etc.)
    bool color;  // color of the chess piece (true for white, false for black)
    std::pair<int, int> position;    // position of the chess piece on the board
public:
	Chesspiece() {}; // default constructor
    Chesspiece(std::string t, bool c, std::pair<int, int> pos) : type(std::move(t)), color(c), position(pos)
    {
        /*type = std::move(t); move(t) is used to avoid copying the string
        color = c;
        position = pos;*/
    }

	virtual ~Chesspiece() = default; // virtual destructor for proper cleanup of derived classes

    virtual void draw(sf::RenderWindow& window) = 0;
    virtual bool isValidMove(std::pair<int, int> newPos) = 0;

    std::pair<int, int> getPosition() const { return position; }
    void setPosition(std::pair<int, int> pos) { position = pos; }
    bool getColor() const { return color; }
    std::string getType() const { return type; }
};

class Pawn : public Chesspiece {
public:
    Pawn() {}
    Pawn(bool color, std::pair<int, int> pos) : Chesspiece("Pawn", color, pos) {}

    void draw(sf::RenderWindow& window) override {
        // Temporary: draw a circle or placeholder until you add sprite
        sf::CircleShape shape(20);
        shape.setFillColor(color ? sf::Color::White : sf::Color::Black);
        shape.setPosition(position.first * 100, position.second * 100);
        window.draw(shape);
    }

    bool isValidMove(std::pair<int, int> newPos) override {
        // dummy implementation for now
        return true;
    }
};


class Knight : public Chesspiece
{
public:
    Knight() {}; // default constructor
    Knight(bool color, std::pair<int, int> pos) : Chesspiece("Knight", color, pos) {

    }
};

class Bishop : public Chesspiece
{
public:
    Bishop() {}; // default constructor
    Bishop(bool color, std::pair<int, int> pos) : Chesspiece("Bishop", color, pos) {
    }
};

class Rook : public Chesspiece
{
public:
    Rook() {}; // default constructor
    Rook(bool color, std::pair<int, int> pos) : Chesspiece("Rook", color, pos) {
    }
};

class Queen : public Chesspiece
{
public:
    Queen() {}; // default constructor
    Queen(bool color, std::pair<int, int> pos) : Chesspiece("Queen", color, pos) {
    }
};

class King : public Chesspiece
{
public:
    King() {}; // default constructor
    King(bool color, std::pair<int, int> pos) : Chesspiece("King", color, pos) {
    }
};
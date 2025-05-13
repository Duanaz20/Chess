#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <utility>

class Chesspiece {
public:
    std::string type;
    bool color;
    std::pair<int, int> position;
    sf::Texture texture;

    Chesspiece(std::string t, bool c, std::pair<int, int> pos) {
        type = t;
        color = c;
        position = pos;

        std::string basePath = "resources/";
        if (type == "Pawn")
            texture.loadFromFile(basePath + "pawn-" + (color ? "w" : "b") + ".png");
        else if (type == "King")
            texture.loadFromFile(basePath + "king-" + (color ? "w" : "b") + ".png");
        else if (type == "Queen")
            texture.loadFromFile(basePath + "queen-" + (color ? "w" : "b") + ".png");
        else if (type == "Bishop")
            texture.loadFromFile(basePath + "bishop-" + (color ? "w" : "b") + ".png");
        else if (type == "Knight")
            texture.loadFromFile(basePath + "knight-" + (color ? "w" : "b") + ".png");
        else if (type == "Rook")
            texture.loadFromFile(basePath + "rook-" + (color ? "w" : "b") + ".png");

        texture.setSmooth(true);
    }

    virtual std::vector<std::pair<int, int>> getPossibleMoves(const std::vector<Chesspiece*>& boardState) = 0;
    bool isOccupied(int x, int y, const std::vector<Chesspiece*>& boardState, bool& isEnemy) {
        for (auto* piece : boardState) {
            if (piece->position.first == x && piece->position.second == y) {
                isEnemy = (piece->color != this->color);
                return true;
            }
        }
        return false;
    }

};

class Pawn : public Chesspiece {
public:
    Pawn(bool color, std::pair<int, int> pos) : Chesspiece("Pawn", color, pos) {}
    std::vector<std::pair<int, int>> getPossibleMoves(const std::vector<Chesspiece*>& boardState) override {
        std::vector<std::pair<int, int>> moves;
        int dir = color ? -1 : 1;
        int x = position.first, y = position.second;

        bool dummy;

        // Move forward
        if (!isOccupied(x, y + dir, boardState, dummy) && y + dir >= 0 && y + dir < 8)
            moves.emplace_back(x, y + dir);

        // Initial two-step move
        if ((color && y == 6) || (!color && y == 1)) {
            if (!isOccupied(x, y + dir, boardState, dummy) &&
                !isOccupied(x, y + 2 * dir, boardState, dummy))
                moves.emplace_back(x, y + 2 * dir);
        }

        // Captures
        for (int dx : {-1, 1}) {
            int nx = x + dx;
            int ny = y + dir;
            if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
                if (isOccupied(nx, ny, boardState, dummy) && dummy)
                    moves.emplace_back(nx, ny);
            }
        }

        return moves;
    }


};
class Knight : public Chesspiece {
public:
    Knight(bool color, std::pair<int, int> pos) : Chesspiece("Knight", color, pos) {}
    std::vector<std::pair<int, int>> getPossibleMoves(const std::vector<Chesspiece*>& boardState) override {
        std::vector<std::pair<int, int>> moves;
        int x = position.first, y = position.second;
        std::vector<std::pair<int, int>> offsets = {
            {1,2}, {2,1}, {2,-1}, {1,-2},
            {-1,-2}, {-2,-1}, {-2,1}, {-1,2}
        };

        for (auto [dx, dy] : offsets) {
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
                bool isEnemy;
                if (!isOccupied(nx, ny, boardState, isEnemy) || isEnemy)
                    moves.emplace_back(nx, ny);
            }
        }

        return moves;
    }

};
class Bishop : public Chesspiece {
public:
    Bishop(bool color, std::pair<int, int> pos) : Chesspiece("Bishop", color, pos) {}
    std::vector<std::pair<int, int>> getPossibleMoves(const std::vector<Chesspiece*>& boardState) override {
        std::vector<std::pair<int, int>> moves;
        int x = position.first, y = position.second;

        for (int dx : {-1, 1})
            for (int dy : {-1, 1}) {
                int nx = x + dx, ny = y + dy;
                while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
                    bool isEnemy;
                    if (isOccupied(nx, ny, boardState, isEnemy)) {
                        if (isEnemy) moves.emplace_back(nx, ny);
                        break;
                    }
                    moves.emplace_back(nx, ny);
                    nx += dx;
                    ny += dy;
                }
            }

        return moves;
    }

};
class Rook : public Chesspiece {
public:
	Rook(bool color, std::pair<int, int> pos) : Chesspiece("Rook", color, pos) {}
    std::vector<std::pair<int, int>> getPossibleMoves(const std::vector<Chesspiece*>& boardState) override {
        std::vector<std::pair<int, int>> moves;
        int x = position.first, y = position.second;

        std::vector<std::pair<int, int>> directions = { {1,0}, {-1,0}, {0,1}, {0,-1} };

        for (auto [dx, dy] : directions) {
            int nx = x + dx, ny = y + dy;
            while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
                bool isEnemy;
                if (isOccupied(nx, ny, boardState, isEnemy)) {
                    if (isEnemy) moves.emplace_back(nx, ny);
                    break;
                }
                moves.emplace_back(nx, ny);
                nx += dx;
                ny += dy;
            }
        }

        return moves;
    }

};
class Queen : public Chesspiece {
public:
	Queen(bool color, std::pair<int, int> pos) : Chesspiece("Queen", color, pos) {}
    std::vector<std::pair<int, int>> getPossibleMoves(const std::vector<Chesspiece*>& boardState) override {
        std::vector<std::pair<int, int>> moves;
        Rook rook(color, position);
        Bishop bishop(color, position);
        moves = rook.getPossibleMoves(boardState);
        auto bishopMoves = bishop.getPossibleMoves(boardState);
        moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());
        return moves;
    }

};
class King : public Chesspiece {
public:
	King(bool color, std::pair<int, int> pos) : Chesspiece("King", color, pos) {}
    std::vector<std::pair<int, int>> getPossibleMoves(const std::vector<Chesspiece*>& boardState) override {
        std::vector<std::pair<int, int>> moves;
        int x = position.first, y = position.second;

        for (int dx = -1; dx <= 1; ++dx)
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;
                int nx = x + dx, ny = y + dy;
                if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
                    bool isEnemy;
                    if (!isOccupied(nx, ny, boardState, isEnemy) || isEnemy)
                        moves.emplace_back(nx, ny);
                }
            }

        return moves;
    }

};

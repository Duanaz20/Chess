#include <iostream>
using namespace std;

class Chesspiece
{
public:
    std::string type;
    bool color;  //true for white
    std::pair<int, int> position; //x & y
    sf::Texture texture;
    int moveCount = 0;

    Chesspiece(std::string t, bool c, std::pair<int, int> pos)
    {
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

    virtual vector<std::pair<int, int>> getPossibleMoves() = 0;

    virtual vector<std::pair<int, int>> getValidMoves(vector<Chesspiece*> list) = 0;

};

class Pawn : public Chesspiece {

public:
    Pawn(bool color, std::pair<int, int> pos) : Chesspiece("Pawn", color, pos) {
    }

    vector<std::pair<int, int>> getPossibleMoves() override {
        vector<std::pair<int, int>> moves;
        if (color) {
            moves.push_back({ position.first, position.second - 1 });
            moves.push_back({ position.first + 1, position.second - 1 });
            moves.push_back({ position.first - 1, position.second - 1 });
            if (moveCount == 0) {
                moves.push_back({ position.first, position.second - 2 });
            }
        }
        else {
            moves.push_back({ position.first, position.second + 1 });
            moves.push_back({ position.first + 1, position.second + 1 });
            moves.push_back({ position.first - 1, position.second + 1 });
            if (moveCount == 0) {
                moves.push_back({ position.first, position.second + 2 });
            }
        }
        return moves;
    }

    vector<std::pair<int, int>> getValidMoves(vector<Chesspiece*> list) override {

        vector<std::pair<int, int>> moves = getPossibleMoves();

        vector<std::pair<int, int>> filteredmoves;

        for (int i = 0; i < moves.size(); i++) {
            std::pair<int, int> move = moves[i];
            bool filtered = false;

            if (move.first < 0 || move.first > 7 || move.second < 0 || move.second > 7) {       // remove moves out of bounds
                continue;
            }


            for (int j = 0; j < list.size(); j++) {     //check all the pieces on the board

                Chesspiece* piece = list[j];

                if (piece->color == color && piece->position == move) {        //remove move if it is occupied by a piece of the same color
                    filtered = true;
                    break;
                }

                if (piece->color != color && piece->position == move && move.first == position.first) {   //remove move if it is occupied by a piece of the opposite color
                    filtered = true;
                    break;
                }

                if (move.first != position.first) {   //is diagonal move
                    if (piece->color != color && piece->position == move) {    //is there anything to capture
                        filtered = false;
                        break;
                    }
                    else {
                        filtered = true;        //don't break as we need to check all the pieces
                    }
                }

            }

            if (filtered) {
                continue;
            }

            filteredmoves.push_back(move);

        }
        return filteredmoves;
    }

};


class Knight : public Chesspiece {

public:
    Knight(bool color, std::pair<int, int> pos) : Chesspiece("Knight", color, pos) {}

    vector<std::pair<int, int>> getPossibleMoves() override {
        vector<std::pair<int, int>> moves;
        moves.push_back({ position.first + 2, position.second + 1 });
        moves.push_back({ position.first + 2, position.second - 1 });
        moves.push_back({ position.first + 1, position.second + 2 });
        moves.push_back({ position.first - 1, position.second + 2 });
        moves.push_back({ position.first - 2, position.second + 1 });
        moves.push_back({ position.first - 2, position.second - 1 });
        moves.push_back({ position.first + 1, position.second - 2 });
        moves.push_back({ position.first - 1, position.second - 2 });
        return moves;
    }

    vector<std::pair<int, int>> getValidMoves(vector<Chesspiece*> list) override {
        vector<std::pair<int, int>> moves = getPossibleMoves();

        vector<std::pair<int, int>> filteredmoves;

        for (int i = 0; i < moves.size(); i++) {
            std::pair<int, int> move = moves[i];
            bool filtered = false;

            if (move.first < 0 || move.first > 7 || move.second < 0 || move.second > 7) {       // remove moves out of bounds
                continue;
            }


            for (int j = 0; j < list.size(); j++) {     //check all the pieces on the board

                Chesspiece* piece = list[j];

                if (piece->color == color && piece->position == move) {        //remove move if it is occupied by a piece of the same color
                    filtered = true;
                    break;
                }
            }

            if (filtered) {
                continue;
            }

            filteredmoves.push_back(move);

        }
        return filteredmoves;
    }

};

class Bishop : public Chesspiece {

public:
    Bishop(bool color, std::pair<int, int> pos) : Chesspiece("Bishop", color, pos) {}

    vector<std::pair<int, int>> getPossibleMoves() override {
        vector<std::pair<int, int>> moves;
        moves.push_back({ position.first + 1, position.second + 1 });
        moves.push_back({ position.first + 2, position.second + 2 });
        moves.push_back({ position.first + 3, position.second + 3 });
        moves.push_back({ position.first + 4, position.second + 4 });
        moves.push_back({ position.first + 5, position.second + 5 });
        moves.push_back({ position.first + 6, position.second + 6 });
        moves.push_back({ position.first + 7, position.second + 7 });

        moves.push_back({ position.first + 1, position.second - 1 });
        moves.push_back({ position.first + 2, position.second - 2 });
        moves.push_back({ position.first + 3, position.second - 3 });
        moves.push_back({ position.first + 4, position.second - 4 });
        moves.push_back({ position.first + 5, position.second - 5 });
        moves.push_back({ position.first + 6, position.second - 6 });
        moves.push_back({ position.first + 7, position.second - 7 });

        moves.push_back({ position.first - 1, position.second + 1 });
        moves.push_back({ position.first - 2, position.second + 2 });
        moves.push_back({ position.first - 3, position.second + 3 });
        moves.push_back({ position.first - 4, position.second + 4 });
        moves.push_back({ position.first - 5, position.second + 5 });
        moves.push_back({ position.first - 6, position.second + 6 });
        moves.push_back({ position.first - 7, position.second + 7 });

        moves.push_back({ position.first - 1, position.second - 1 });
        moves.push_back({ position.first - 2, position.second - 2 });
        moves.push_back({ position.first - 3, position.second - 3 });
        moves.push_back({ position.first - 4, position.second - 4 });
        moves.push_back({ position.first - 5, position.second - 5 });
        moves.push_back({ position.first - 6, position.second - 6 });
        moves.push_back({ position.first - 7, position.second - 7 });
        return moves;
    }

    vector<std::pair<int, int>> getValidMoves(vector<Chesspiece*> list) override {
        vector<std::pair<int, int>> moves = getPossibleMoves();

        vector<std::pair<int, int>> filteredmoves;

        double distance[4] = { 65,65,65,65 };  //distance for the four diagonal directions

        for (int i = 0; i < moves.size(); i++) {
            std::pair<int, int> move = moves[i];
            bool filtered = false;

            if (move.first < 0 || move.first > 7 || move.second < 0 || move.second > 7) {       // remove moves out of bounds
                continue;
            }


            for (int j = 0; j < list.size(); j++) {     //check all the pieces on the board

                Chesspiece* piece = list[j];

                if (piece->color == color && piece->position == move) {        //remove move if it is occupied by a piece of the same color
                    filtered = true;
                    double d = (piece->position.first - position.first) * (piece->position.first - position.first) + (piece->position.second - position.second) * (piece->position.second - position.second) - 2; //subtracting arbitary value to make last boundary unavailable
                    if (piece->position.first > position.first && piece->position.second > position.second) {      //top right
                        distance[0] = min<double>(distance[0], d);
                    }
                    else if (piece->position.first > position.first && piece->position.second < position.second) {   //bottom right
                        distance[1] = min<double>(distance[1], d);
                    }
                    else if (piece->position.first<position.first && piece->position.second>position.second) {   //top left
                        distance[2] = min<double>(distance[2], d);
                    }
                    else if (piece->position.first < position.first && piece->position.second < position.second) {   //bottom left
                        distance[3] = min<double>(distance[3], d);
                    }

                    break;
                }

                if (piece->color != color && piece->position == move) {        //remove move if it is occupied by a piece of the opponent color
                    filtered = true;
                    double d = (piece->position.first - position.first) * (piece->position.first - position.first) + (piece->position.second - position.second) * (piece->position.second - position.second);
                    if (piece->position.first > position.first && piece->position.second > position.second) {      //top right
                        distance[0] = min<double>(distance[0], d);
                    }
                    else if (piece->position.first > position.first && piece->position.second < position.second) {   //bottom right
                        distance[1] = min<double>(distance[1], d);
                    }
                    else if (piece->position.first<position.first && piece->position.second>position.second) {   //top left
                        distance[2] = min<double>(distance[2], d);
                    }
                    else if (piece->position.first < position.first && piece->position.second < position.second) {   //bottom left
                        distance[3] = min<double>(distance[3], d);
                    }
                    break;
                }

            }

        }

        for (int i = 0; i < moves.size(); i++) {
            std::pair<int, int> move = moves[i];

            if (move.first < 0 || move.first > 7 || move.second < 0 || move.second > 7) {       // remove moves out of bounds
                continue;
            }

            if (move.first > position.first && move.second > position.second) {      //top right
                double d = (move.first - position.first) * (move.first - position.first) + (move.second - position.second) * (move.second - position.second);
                if (d > distance[0]) {
                    continue;
                }
            }
            else if (move.first > position.first && move.second < position.second) {   //bottom right
                double d = (move.first - position.first) * (move.first - position.first) + (move.second - position.second) * (move.second - position.second);
                if (d > distance[1]) {
                    continue;
                }
            }
            else if (move.first<position.first && move.second>position.second) {   //top left
                double d = (move.first - position.first) * (move.first - position.first) + (move.second - position.second) * (move.second - position.second);
                if (d > distance[2]) {
                    continue;
                }
            }
            else if (move.first < position.first && move.second < position.second) {   //bottom left
                double d = (move.first - position.first) * (move.first - position.first) + (move.second - position.second) * (move.second - position.second);
                if (d > distance[3]) {
                    continue;
                }
            }
            filteredmoves.push_back(move);

        }

        return filteredmoves;
    }
};

class Rook : public Chesspiece {
public:
    Rook(bool color, std::pair<int, int> pos) : Chesspiece("Rook", color, pos) {}

    vector<std::pair<int, int>> getPossibleMoves() override {
        vector<std::pair<int, int>> moves;

        moves.push_back({ position.first + 1, position.second });
        moves.push_back({ position.first + 2, position.second });
        moves.push_back({ position.first + 3, position.second });
        moves.push_back({ position.first + 4, position.second });
        moves.push_back({ position.first + 5, position.second });
        moves.push_back({ position.first + 6, position.second });
        moves.push_back({ position.first + 7, position.second });

        moves.push_back({ position.first - 1, position.second });
        moves.push_back({ position.first - 2, position.second });
        moves.push_back({ position.first - 3, position.second });
        moves.push_back({ position.first - 4, position.second });
        moves.push_back({ position.first - 5, position.second });
        moves.push_back({ position.first - 6, position.second });
        moves.push_back({ position.first - 7, position.second });

        moves.push_back({ position.first, position.second + 1 });
        moves.push_back({ position.first, position.second + 2 });
        moves.push_back({ position.first, position.second + 3 });
        moves.push_back({ position.first, position.second + 4 });
        moves.push_back({ position.first, position.second + 5 });
        moves.push_back({ position.first, position.second + 6 });
        moves.push_back({ position.first, position.second + 7 });

        moves.push_back({ position.first, position.second - 1 });
        moves.push_back({ position.first, position.second - 2 });
        moves.push_back({ position.first, position.second - 3 });
        moves.push_back({ position.first, position.second - 4 });
        moves.push_back({ position.first, position.second - 5 });
        moves.push_back({ position.first, position.second - 6 });
        moves.push_back({ position.first, position.second - 7 });
        return moves;
    }

    vector<std::pair<int, int>> getValidMoves(vector<Chesspiece*> list) override {
        vector<std::pair<int, int>> moves = getPossibleMoves();

        vector<std::pair<int, int>> filteredmoves;

        double distance[4] = { 65,65,65,65 };  //distance for the four diagonal directions

        for (int i = 0; i < moves.size(); i++) {
            std::pair<int, int> move = moves[i];
            bool filtered = false;

            if (move.first < 0 || move.first > 7 || move.second < 0 || move.second > 7) {       // remove moves out of bounds
                continue;
            }


            for (int j = 0; j < list.size(); j++) {     //check all the pieces on the board

                Chesspiece* piece = list[j];

                if (piece->color == color && piece->position == move) {        //remove move if it is occupied by a piece of the same color
                    filtered = true;
                    double d = (piece->position.first - position.first) * (piece->position.first - position.first) + (piece->position.second - position.second) * (piece->position.second - position.second) - 2; //subtracting arbitary value to make last boundary unavailable
                    if (piece->position.first == position.first && piece->position.second > position.second) {      //top 
                        distance[0] = min<double>(distance[0], d);
                    }
                    else if (piece->position.first == position.first && piece->position.second < position.second) {   //bottom 
                        distance[1] = min<double>(distance[1], d);
                    }
                    else if (piece->position.first > position.first && piece->position.second == position.second) {   //right
                        distance[2] = min<double>(distance[2], d);
                    }
                    else if (piece->position.first < position.first && piece->position.second == position.second) {   //left
                        distance[3] = min<double>(distance[3], d);
                    }

                    break;
                }

                if (piece->color != color && piece->position == move) {        //remove move if it is occupied by a piece of the opponent color
                    filtered = true;
                    double d = (piece->position.first - position.first) * (piece->position.first - position.first) + (piece->position.second - position.second) * (piece->position.second - position.second);
                    if (piece->position.first == position.first && piece->position.second > position.second) {      //top 
                        distance[0] = min<double>(distance[0], d);
                    }
                    else if (piece->position.first == position.first && piece->position.second < position.second) {   //bottom 
                        distance[1] = min<double>(distance[1], d);
                    }
                    else if (piece->position.first > position.first && piece->position.second == position.second) {   //right
                        distance[2] = min<double>(distance[2], d);
                    }
                    else if (piece->position.first < position.first && piece->position.second == position.second) {   //left
                        distance[3] = min<double>(distance[3], d);
                    }
                    break;
                }

            }

        }

        for (int i = 0; i < moves.size(); i++) {
            std::pair<int, int> move = moves[i];

            if (move.first < 0 || move.first > 7 || move.second < 0 || move.second > 7) {       // remove moves out of bounds
                continue;
            }

            if (move.first == position.first && move.second > position.second) {      //top
                double d = (move.first - position.first) * (move.first - position.first) + (move.second - position.second) * (move.second - position.second);
                if (d > distance[0]) {
                    continue;
                }
            }
            else if (move.first == position.first && move.second < position.second) {   //bottom
                double d = (move.first - position.first) * (move.first - position.first) + (move.second - position.second) * (move.second - position.second);
                if (d > distance[1]) {
                    continue;
                }
            }
            else if (move.first > position.first && move.second == position.second) {   //right
                double d = (move.first - position.first) * (move.first - position.first) + (move.second - position.second) * (move.second - position.second);
                if (d > distance[2]) {
                    continue;
                }
            }
            else if (move.first < position.first && move.second == position.second) {   //left
                double d = (move.first - position.first) * (move.first - position.first) + (move.second - position.second) * (move.second - position.second);
                if (d > distance[3]) {
                    continue;
                }
            }
            filteredmoves.push_back(move);

        }

        return filteredmoves;
    }
};

class Queen : public Chesspiece {

public:
    Queen(bool color, std::pair<int, int> pos) : Chesspiece("Queen", color, pos) {}

    vector<std::pair<int, int>> getPossibleMoves() override {
        vector<std::pair<int, int>> moves;
        moves.push_back({ position.first + 1, position.second });
        moves.push_back({ position.first + 2, position.second });
        moves.push_back({ position.first + 3, position.second });
        moves.push_back({ position.first + 4, position.second });
        moves.push_back({ position.first + 5, position.second });
        moves.push_back({ position.first + 6, position.second });
        moves.push_back({ position.first + 7, position.second });

        moves.push_back({ position.first - 1, position.second });
        moves.push_back({ position.first - 2, position.second });
        moves.push_back({ position.first - 3, position.second });
        moves.push_back({ position.first - 4, position.second });
        moves.push_back({ position.first - 5, position.second });
        moves.push_back({ position.first - 6, position.second });
        moves.push_back({ position.first - 7, position.second });

        moves.push_back({ position.first, position.second + 1 });
        moves.push_back({ position.first, position.second + 2 });
        moves.push_back({ position.first, position.second + 3 });
        moves.push_back({ position.first, position.second + 4 });
        moves.push_back({ position.first, position.second + 5 });
        moves.push_back({ position.first, position.second + 6 });
        moves.push_back({ position.first, position.second + 7 });

        moves.push_back({ position.first, position.second - 1 });
        moves.push_back({ position.first, position.second - 2 });
        moves.push_back({ position.first, position.second - 3 });
        moves.push_back({ position.first, position.second - 4 });
        moves.push_back({ position.first, position.second - 5 });
        moves.push_back({ position.first, position.second - 6 });
        moves.push_back({ position.first, position.second - 7 });

        moves.push_back({ position.first + 1, position.second + 1 });
        moves.push_back({ position.first + 2, position.second + 2 });
        moves.push_back({ position.first + 3, position.second + 3 });
        moves.push_back({ position.first + 4, position.second + 4 });
        moves.push_back({ position.first + 5, position.second + 5 });
        moves.push_back({ position.first + 6, position.second + 6 });
        moves.push_back({ position.first + 7, position.second + 7 });

        moves.push_back({ position.first + 1, position.second - 1 });
        moves.push_back({ position.first + 2, position.second - 2 });
        moves.push_back({ position.first + 3, position.second - 3 });
        moves.push_back({ position.first + 4, position.second - 4 });
        moves.push_back({ position.first + 5, position.second - 5 });
        moves.push_back({ position.first + 6, position.second - 6 });
        moves.push_back({ position.first + 7, position.second - 7 });

        moves.push_back({ position.first - 1, position.second + 1 });
        moves.push_back({ position.first - 2, position.second + 2 });
        moves.push_back({ position.first - 3, position.second + 3 });
        moves.push_back({ position.first - 4, position.second + 4 });
        moves.push_back({ position.first - 5, position.second + 5 });
        moves.push_back({ position.first - 6, position.second + 6 });
        moves.push_back({ position.first - 7, position.second + 7 });

        moves.push_back({ position.first - 1, position.second - 1 });
        moves.push_back({ position.first - 2, position.second - 2 });
        moves.push_back({ position.first - 3, position.second - 3 });
        moves.push_back({ position.first - 4, position.second - 4 });
        moves.push_back({ position.first - 5, position.second - 5 });
        moves.push_back({ position.first - 6, position.second - 6 });
        moves.push_back({ position.first - 7, position.second - 7 });
        return moves;
    }

    vector<std::pair<int, int>> getValidMoves(vector<Chesspiece*> list) override {
        vector<std::pair<int, int>> moves = getPossibleMoves();

        vector<std::pair<int, int>> filteredmoves;

        double distance[8] = { 65,65,65,65,65,65,65,65 };  //distance for the four diagonal directions

        for (int i = 0; i < moves.size(); i++) {
            std::pair<int, int> move = moves[i];
            bool filtered = false;

            if (move.first < 0 || move.first > 7 || move.second < 0 || move.second > 7) {       // remove moves out of bounds
                continue;
            }


            for (int j = 0; j < list.size(); j++) {     //check all the pieces on the board

                Chesspiece* piece = list[j];

                if (piece->color == color && piece->position == move) {        //remove move if it is occupied by a piece of the same color
                    filtered = true;
                    double d = (piece->position.first - position.first) * (piece->position.first - position.first) + (piece->position.second - position.second) * (piece->position.second - position.second) - 2; //subtracting arbitary value to make last boundary unavailable
                    if (piece->position.first == position.first && piece->position.second > position.second) {      //top 
                        distance[0] = min<double>(distance[0], d);
                    }
                    else if (piece->position.first == position.first && piece->position.second < position.second) {   //bottom 
                        distance[1] = min<double>(distance[1], d);
                    }
                    else if (piece->position.first > position.first && piece->position.second == position.second) {   //right
                        distance[2] = min<double>(distance[2], d);
                    }
                    else if (piece->position.first < position.first && piece->position.second == position.second) {   //left
                        distance[3] = min<double>(distance[3], d);
                    }
                    else if (piece->position.first > position.first && piece->position.second > position.second) {      //top right
                        distance[4] = min<double>(distance[4], d);
                    }
                    else if (piece->position.first > position.first && piece->position.second < position.second) {   //bottom right
                        distance[5] = min<double>(distance[5], d);
                    }
                    else if (piece->position.first<position.first && piece->position.second>position.second) {   //top left
                        distance[6] = min<double>(distance[6], d);
                    }
                    else if (piece->position.first < position.first && piece->position.second < position.second) {   //bottom left
                        distance[7] = min<double>(distance[7], d);
                    }

                    break;
                }

                if (piece->color != color && piece->position == move) {        //remove move if it is occupied by a piece of the opponent color
                    filtered = true;
                    double d = (piece->position.first - position.first) * (piece->position.first - position.first) + (piece->position.second - position.second) * (piece->position.second - position.second);
                    if (piece->position.first == position.first && piece->position.second > position.second) {      //top 
                        distance[0] = min<double>(distance[0], d);
                    }
                    else if (piece->position.first == position.first && piece->position.second < position.second) {   //bottom 
                        distance[1] = min<double>(distance[1], d);
                    }
                    else if (piece->position.first > position.first && piece->position.second == position.second) {   //right
                        distance[2] = min<double>(distance[2], d);
                    }
                    else if (piece->position.first < position.first && piece->position.second == position.second) {   //left
                        distance[3] = min<double>(distance[3], d);
                    }
                    else if (piece->position.first > position.first && piece->position.second > position.second) {      //top right
                        distance[4] = min<double>(distance[4], d);
                    }
                    else if (piece->position.first > position.first && piece->position.second < position.second) {   //bottom right
                        distance[5] = min<double>(distance[5], d);
                    }
                    else if (piece->position.first<position.first && piece->position.second>position.second) {   //top left
                        distance[6] = min<double>(distance[6], d);
                    }
                    else if (piece->position.first < position.first && piece->position.second < position.second) {   //bottom left
                        distance[7] = min<double>(distance[7], d);
                    }
                    break;
                }

            }

        }

        for (int i = 0; i < moves.size(); i++) {
            std::pair<int, int> move = moves[i];

            if (move.first < 0 || move.first > 7 || move.second < 0 || move.second > 7) {       // remove moves out of bounds
                continue;
            }

            if (move.first == position.first && move.second > position.second) {      //top
                double d = (move.first - position.first) * (move.first - position.first) + (move.second - position.second) * (move.second - position.second);
                if (d > distance[0]) {
                    continue;
                }
            }
            else if (move.first == position.first && move.second < position.second) {   //bottom
                double d = (move.first - position.first) * (move.first - position.first) + (move.second - position.second) * (move.second - position.second);
                if (d > distance[1]) {
                    continue;
                }
            }
            else if (move.first > position.first && move.second == position.second) {   //right
                double d = (move.first - position.first) * (move.first - position.first) + (move.second - position.second) * (move.second - position.second);
                if (d > distance[2]) {
                    continue;
                }
            }
            else if (move.first < position.first && move.second == position.second) {   //left
                double d = (move.first - position.first) * (move.first - position.first) + (move.second - position.second) * (move.second - position.second);
                if (d > distance[3]) {
                    continue;
                }
            }
            else if (move.first > position.first && move.second > position.second) {      //top right
                double d = (move.first - position.first) * (move.first - position.first) + (move.second - position.second) * (move.second - position.second);
                if (d > distance[4]) {
                    continue;
                }
            }
            else if (move.first > position.first && move.second < position.second) {   //bottom right
                double d = (move.first - position.first) * (move.first - position.first) + (move.second - position.second) * (move.second - position.second);
                if (d > distance[5]) {
                    continue;
                }
            }
            else if (move.first<position.first && move.second>position.second) {   //top left
                double d = (move.first - position.first) * (move.first - position.first) + (move.second - position.second) * (move.second - position.second);
                if (d > distance[6]) {
                    continue;
                }
            }
            else if (move.first < position.first && move.second < position.second) {   //bottom left
                double d = (move.first - position.first) * (move.first - position.first) + (move.second - position.second) * (move.second - position.second);
                if (d > distance[7]) {
                    continue;
                }
            }
            filteredmoves.push_back(move);

        }

        return filteredmoves;

    }
};

class King : public Chesspiece {

public:
    King(bool color, std::pair<int, int> pos) : Chesspiece("King", color, pos) {}

    vector<std::pair<int, int>> getPossibleMoves() override {
        vector<std::pair<int, int>> moves;
        moves.push_back({ position.first + 1, position.second });
        moves.push_back({ position.first - 1, position.second });
        moves.push_back({ position.first, position.second + 1 });
        moves.push_back({ position.first, position.second - 1 });
        moves.push_back({ position.first + 1, position.second + 1 });
        moves.push_back({ position.first + 1, position.second - 1 });
        moves.push_back({ position.first - 1, position.second + 1 });
        moves.push_back({ position.first - 1, position.second - 1 });
        return moves;
    }

    vector<std::pair<int, int>> getValidMoves(vector<Chesspiece*> list) override {
        vector<std::pair<int, int>> moves = getPossibleMoves();

        vector<std::pair<int, int>> filteredmoves;

        for (int i = 0; i < moves.size(); i++) {
            std::pair<int, int> move = moves[i];
            bool filtered = false;

            if (move.first < 0 || move.first > 7 || move.second < 0 || move.second > 7) {       // remove moves out of bounds
                continue;
            }


            for (int j = 0; j < list.size(); j++) {     //check all the pieces on the board

                Chesspiece* piece = list[j];

                if (piece->color == color && piece->position == move) {        //remove move if it is occupied by a piece of the same color
                    filtered = true;
                    break;
                }
            }

            if (filtered) {
                continue;
            }

            filteredmoves.push_back(move);

        }
        return filteredmoves;
    }

    bool inCheck(vector<Chesspiece*> list) {

        vector<Chesspiece*> opponentPieces;

        for (int i = 0; i < list.size(); i++) {      //get all opponent pieces
            if (list[i]->color != color) {
                opponentPieces.push_back(list[i]);
            }
        }

        for (int i = 0; i < opponentPieces.size(); i++) {     //check all the opponent pieces on the board

            Chesspiece* piece = opponentPieces[i];

            vector<std::pair<int, int>> moves = piece->getValidMoves(list);     //get all the valid moves of the piece

            for (int j = 0; j < moves.size(); j++) {     //check if any of the moves is the king's position
                if (moves[j].first == position.first && moves[j].second == position.second) {
                    return true; // King is in check
                }
            }
        }


        return false; // King is not in check
    }
};
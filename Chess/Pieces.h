
class Chesspiece
{
private:
    string type; // type of chess piece (e.g., pawn, knight, bishop, etc.)
    bool color;  // color of the chess piece (true for white, false for black)
    pair<int, int> position;    // position of the chess piece on the board
public:
	Chesspiece() {}; // default constructor
    Chesspiece(string t, bool c, pair<int, int> pos); {
        type = t;
        color = c;
        position = pos;
    };
};

class Pawn : public Chesspiece
{
private:
    bool hasMoved = false; // whether the pawn has moved from its starting position 

public:
    Pawn(bool color, pair<int, int> pos) : Chesspiece("Pawn", color, pos) {

    }
}

class Knight : public Chesspiece
{
public:
    Knight(bool color, pair<int, int> pos) : Chesspiece("Knight", color, pos) {

    }
}

class Bishop : public Chesspiece
{
public:
    Bishop(bool color, pair<int, int> pos) : Chesspiece("Bishop", color, pos) {
    }
}

class Rook : public Chesspiece
{
public:
    Rook(bool color, pair<int, int> pos) : Chesspiece("Rook", color, pos) {
    }
}

class Queen : public Chesspiece
{
public:
    Queen(bool color, pair<int, int> pos) : Chesspiece("Queen", color, pos) {
    }
}

class King : public Chesspiece
{
public:
    King(bool color, pair<int, int> pos) : Chesspiece("King", color, pos) {
    }
}
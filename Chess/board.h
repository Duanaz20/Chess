#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include <algorithm>
#include "pieces.h"
#include <chrono>
using namespace std;
using namespace std::chrono;

class PieceTable {       //piece tables for the board evaluation
private:
    static constexpr  int pawn_table[8][8] = {
    {  0,  0,  0,  0,  0,  0,  0,  0 },
    { 50, 50, 50, 50, 50, 50, 50, 50 },
    { 10, 10, 20, 30, 30, 20, 10, 10 },
    {  5,  5, 10, 25, 25, 10,  5,  5 },
    {  0,  0,  0, 20, 20,  0,  0,  0 },
    {  5, -5,-10,  0,  0,-10, -5,  5 },
    {  5, 10, 10,-20,-20, 10, 10,  5 },
    {  0,  0,  0,  0,  0,  0,  0,  0 }
    };

    static constexpr int knight_table[8][8] = {
    { -50,-40,-30,-30,-30,-30,-40,-50 },
    { -40,-20,  0,  0,  0,  0,-20,-40 },
    { -30,  0, 10, 15, 15, 10,  0,-30 },
    { -30,  5, 15, 20, 20, 15,  5,-30 },
    { -30,  0, 15, 20, 20, 15,  0,-30 },
    { -30,  5, 10, 15, 15, 10,  5,-30 },
    { -40,-20,  0,  5,  5,  0,-20,-40 },
    { -50,-40,-30,-30,-30,-30,-40,-50 }
    };

    static constexpr  int bishop_table[8][8] = {
    { -20,-10,-10,-10,-10,-10,-10,-20 },
    { -10,  0,  0,  0,  0,  0,  0,-10 },
    { -10,  0,  5, 10, 10,  5,  0,-10 },
    { -10,  5,  5, 10, 10,  5,  5,-10 },
    { -10,  0, 10, 10, 10, 10,  0,-10 },
    { -10, 10, 10, 10, 10, 10, 10,-10 },
    { -10,  5,  0,  0,  0,  0,  5,-10 },
    { -20,-10,-10,-10,-10,-10,-10,-20 }
    };

    static constexpr int rook_table[8][8] = {
    {  0,  0,  0,  0,  0,  0,  0,  0 },
    {  5, 10, 10, 10, 10, 10, 10,  5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    {  0,  0,  0,  5,  5,  0,  0,  0 }
    };

    static constexpr int queen_table[8][8] = {
    { -20,-10,-10, -5, -5,-10,-10,-20 },
    { -10,  0,  0,  0,  0,  0,  0,-10 },
    { -10,  0,  5,  5,  5,  5,  0,-10 },
    {  -5,  0,  5,  5,  5,  5,  0, -5 },
    {   0,  0,  5,  5,  5,  5,  0, -5 },
    { -10,  5,  5,  5,  5,  5,  0,-10 },
    { -10,  0,  5,  0,  0,  0,  0,-10 },
    { -20,-10,-10, -5, -5,-10,-10,-20 }
    };

public:
    static int getPieceSquareBonus(Chesspiece* piece) {
        int y = piece->position.second;
        if (!piece->color) { // if black
            y = 7 - y;  // mirror table for black
        }

        if (piece->type == "Pawn") {
            return pawn_table[y][piece->position.first];
        }
        else if (piece->type == "Knight") {
            return knight_table[y][piece->position.first];
        }
        else if (piece->type == "Bishop") {
            return bishop_table[y][piece->position.first];
        }
        else if (piece->type == "Rook") {
            return rook_table[y][piece->position.first];
        }
        else if (piece->type == "Queen") {
            return queen_table[y][piece->position.first];
        }
        return 0;
    }

    static int getPieceValue(Chesspiece* piece) {       //piece value for the evaluation of the board
        if (piece->type == "Pawn")   return 100;
        if (piece->type == "Knight") return 320;
        if (piece->type == "Bishop") return 330;
        if (piece->type == "Rook")   return 500;
        if (piece->type == "Queen")  return 900;
        if (piece->type == "King")   return 20000;
        return 0;
    }
};

class ChessMove {
public:
    Chesspiece* piece;
    pair<int, int> from[2];
    pair<int, int> to[2];
    Chesspiece* capturedPiece;
    bool followup;

    ChessMove(Chesspiece* p, pair<int, int> f, pair<int, int> t, Chesspiece* c = nullptr, bool fup = false) {
        piece = p;
        from[0] = f;
        to[0] = t;
        capturedPiece = c;
        followup = fup;
    }
};

class MoveInfo {
public:
    Chesspiece* piece;
    pair<int, int> position;
    bool followup;
    Chesspiece* piece2;
    pair<int, int> position2;

    MoveInfo() {
        piece = nullptr;
        position = { -1, -1 };
        followup = false;
    }

    MoveInfo(Chesspiece* p, std::pair<int, int> pos) {
        piece = p;
        position = pos;
        followup = false;
    }

    void setFollowup(Chesspiece* p2, std::pair<int, int> pos2) {
        followup = true;
        piece2 = p2;
        position2 = pos2;
    }

};

class ChessBoard {
private:
    // game state code
    enum class GameState {
        Intro,
        Playing,
        GameOver
    };

    GameState gameState = GameState::Intro;
    bool vsAI = false; // false = 2 player, true = vs computer
    bool restartRequested = false;

    //adding fonts and buttons + background image
    sf::Texture bgTexture;
    sf::Sprite bgSprite;
    sf::Font font;
    sf::Text titleText;
    sf::Text modeText;
    sf::Text twoPlayerText;
    sf::Text vsAIText;
    sf::Text turnText;
    sf::Text statusText;
    sf::RectangleShape replayButton;
    sf::RectangleShape exitButton;
    sf::Text replayText;
    sf::Text exitText;


    sf::RectangleShape twoPlayerButton;
    sf::RectangleShape vsAIButton;


    vector<Chesspiece*> pieceslist;
    vector<Chesspiece*> capturedlist;

    bool currentmove = true; //true for white, false for black

    MoveInfo selectedMove;

    sf::RenderWindow window;

    bool board[8][8];   //boolean array to check if the squares are empty

    vector<ChessMove> MoveList;        //save the moves happening in board in the format of piece,from,to,capturedpiece(if any)

public:
    ChessBoard() {
        window.create(sf::VideoMode({ 800, 800 }), "Chess");

        // load fonts
        font.loadFromFile("resources/static/AncizarSerif-BoldItalic.ttf");

        // Trun Text (Move Indicator)
        turnText.setFont(font);
        turnText.setCharacterSize(24);
        turnText.setFillColor(sf::Color::White);
        turnText.setPosition(10, 760); // Bottom of window
        
        // Check / Chekmate indicator
        statusText.setFont(font);
        statusText.setCharacterSize(30);
        statusText.setFillColor(sf::Color::Red);
        statusText.setPosition(300, 10); // Top center-ish



        // load background image
        bgTexture.loadFromFile("resources/background.jpg");
        bgSprite.setTexture(bgTexture);
        bgSprite.setScale(
            float(window.getSize().x) / bgTexture.getSize().x,
            float(window.getSize().y) / bgTexture.getSize().y
        );


        // Title
        titleText.setFont(font);
        titleText.setString("Welcome to the game of Chess");
        titleText.setCharacterSize(48);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(130, 100);

        // Mode Text
        modeText.setFont(font);
        modeText.setString("Choose Game Mode");
        modeText.setCharacterSize(38);
        modeText.setFillColor(sf::Color::White);
        modeText.setPosition(250, 170);

        // Two Player Button
        twoPlayerButton.setSize(sf::Vector2f(300, 60));
        twoPlayerButton.setPosition(250, 250);
        twoPlayerButton.setFillColor(sf::Color(70, 130, 180));

        twoPlayerText.setFont(font);
        twoPlayerText.setString("Two Player");
        twoPlayerText.setCharacterSize(28);
        twoPlayerText.setFillColor(sf::Color::White);
        twoPlayerText.setPosition(310, 260);

        // VS AI Button
        vsAIButton.setSize(sf::Vector2f(300, 60));
        vsAIButton.setPosition(250, 350);
        vsAIButton.setFillColor(sf::Color(105, 105, 105));

        vsAIText.setFont(font);
        vsAIText.setString("Play vs Computer");
        vsAIText.setCharacterSize(28);
        vsAIText.setFillColor(sf::Color::White);
        vsAIText.setPosition(265, 360);

        // after game over
        replayButton.setSize({ 200, 60 });
        replayButton.setPosition(150, 350);
        replayButton.setFillColor(sf::Color(70, 130, 180));

        exitButton.setSize({ 200, 60 });
        exitButton.setPosition(450, 350);
        exitButton.setFillColor(sf::Color(178, 34, 34));

        replayText.setFont(font);
        replayText.setString("Replay");
        replayText.setCharacterSize(24);
        replayText.setFillColor(sf::Color::White);
        replayText.setPosition(200, 365);

        exitText.setFont(font);
        exitText.setString("Exit");
        exitText.setCharacterSize(24);
        exitText.setFillColor(sf::Color::White);
        exitText.setPosition(510, 365);


        // Push piece code
        pieceslist.push_back(new Pawn(true, { 0, 6 }));
        pieceslist.push_back(new Pawn(true, { 1, 6 }));
        pieceslist.push_back(new Pawn(true, { 2, 6 }));
        pieceslist.push_back(new Pawn(true, { 3, 6 }));
        pieceslist.push_back(new Pawn(true, { 4, 6 }));
        pieceslist.push_back(new Pawn(true, { 5, 6 }));
        pieceslist.push_back(new Pawn(true, { 6, 6 }));
        pieceslist.push_back(new Pawn(true, { 7, 6 }));

        pieceslist.push_back(new Rook(true, { 0, 7 }));
        pieceslist.push_back(new Knight(true, { 1, 7 }));
        pieceslist.push_back(new Bishop(true, { 2, 7 }));
        pieceslist.push_back(new Queen(true, { 3, 7 }));
        pieceslist.push_back(new King(true, { 4, 7 }));
        pieceslist.push_back(new Bishop(true, { 5, 7 }));
        pieceslist.push_back(new Knight(true, { 6, 7 }));
        pieceslist.push_back(new Rook(true, { 7, 7 }));

        pieceslist.push_back(new Pawn(false, { 0, 1 }));
        pieceslist.push_back(new Pawn(false, { 1, 1 }));
        pieceslist.push_back(new Pawn(false, { 2, 1 }));
        pieceslist.push_back(new Pawn(false, { 3, 1 }));
        pieceslist.push_back(new Pawn(false, { 4, 1 }));
        pieceslist.push_back(new Pawn(false, { 5, 1 }));
        pieceslist.push_back(new Pawn(false, { 6, 1 }));
        pieceslist.push_back(new Pawn(false, { 7, 1 }));

        pieceslist.push_back(new Rook(false, { 0, 0 }));
        pieceslist.push_back(new Knight(false, { 1, 0 }));
        pieceslist.push_back(new Bishop(false, { 2, 0 }));
        pieceslist.push_back(new Queen(false, { 3, 0 }));
        pieceslist.push_back(new King(false, { 4, 0 }));
        pieceslist.push_back(new Bishop(false, { 5, 0 }));
        pieceslist.push_back(new Knight(false, { 6, 0 }));
        pieceslist.push_back(new Rook(false, { 7, 0 }));

    }

    // Intro Screen
    void drawIntroScreen() {

        window.clear(sf::Color::Black);
        window.draw(bgSprite);
        window.draw(titleText);
        window.draw(modeText);
        window.draw(twoPlayerButton);
        window.draw(twoPlayerText);
        window.draw(vsAIButton);
        window.draw(vsAIText);
        window.display();
    }


    void drawBoard() {
        int boardSize = 8;
        int squareSize = 100;

        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
                square.setPosition(sf::Vector2f(i * squareSize, j * squareSize));
                if ((i + j) % 2 == 0) {
                    square.setFillColor(sf::Color(222, 184, 135));
                }
                else {
                    square.setFillColor(sf::Color(139, 69, 19));
                }
                window.draw(square);
            }
        }
    }

    void drawPieces() {
        int boardSize = 8;
        int squareSize = 100;

        for (int i = 0; i < pieceslist.size(); ++i) {
            sf::Sprite sprite(pieceslist[i]->texture);
            sprite.setPosition(sf::Vector2f(pieceslist[i]->position.first * squareSize - 3, pieceslist[i]->position.second * squareSize - 5));
            sprite.setScale(sf::Vector2f(0.8f, 0.8f));
            window.draw(sprite);
        }
    }

    int evaluateBoard() {  
       auto state = getGameResult();  

       if (state == 1) {  
           return 1000000; // Checkmate in favor of White  
       }  
       else if (state == -1) {  
           return -1000000; // Checkmate in favor of Black  
       }  
       else if (state == 0) {  
           return 0; // Stalemate is a draw  
       }  

       int score = 0;  

       for (auto& piece : pieceslist) {  
           int baseValue = PieceTable::getPieceValue(piece);  
           int x = piece->position.first;  
           int y = piece->position.second;  

           int squareBonus = PieceTable::getPieceSquareBonus(piece);  
           int total = baseValue + squareBonus;  

           score += piece->color ? total : -total;  
       }  

       return score;  
    }

    void display() {
        window.clear();
        drawBoard();
        drawPieces();
        window.display();
    }

    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();

                if (gameState == GameState::Intro) {
                    if (event.type == sf::Event::MouseButtonPressed) {
                        sf::Vector2i mouse = sf::Mouse::getPosition(window);

                        if (twoPlayerButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
                            vsAI = false;
                            gameState = GameState::Playing;
                        }
                        if (vsAIButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
                            vsAI = true;
                            gameState = GameState::Playing;
                        }
                    }
                }
                else if (gameState == GameState::Playing) {
                    if (event.type == sf::Event::MouseButtonPressed)
                        handleMouseClick();
                }
            }

            if (gameState == GameState::Intro) {
                drawIntroScreen();
            }
            else if (gameState == GameState::Playing) {
                window.clear();
                drawBoard();


                int result = getGameResult();  // FIRST, check result

                if (result != 2) {
                    if (result == 1)
                        statusText.setString("Checkmate! Black wins!");
                    else if (result == -1)
                        statusText.setString("Checkmate! White wins!");
                    else
                        statusText.setString("Stalemate!");

                    gameState = GameState::GameOver;
                    drawPieces();
                    turnText.setString("");  // clear turn text
                    window.draw(statusText);
                    window.display();
                    continue;  // skip the rest of Playing block
                }


                makeTurn(); // handles AI if needed

                // Check for checkmate and switch to game over
                //int result = getGameResult();

                if (result == 2) {
                    // Game still on
                    King* king = nullptr;
                    for (Chesspiece* piece : pieceslist) {
                        if (piece->type == "King" && piece->color == currentmove) {
                            king = dynamic_cast<King*>(piece);
                            break;
                        }
                    }
                    if (king && king->inCheck(pieceslist)) {
                        statusText.setString("Check!");
                        window.draw(statusText);
                    }
                }

                drawPieces();
                turnText.setString(currentmove ? "White to move" : "Black to move");
                window.draw(turnText);
                window.display();
            }
            if (gameState == GameState::GameOver) {
                drawGameOverScreen();

                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2i mouse = sf::Mouse::getPosition(window);

                    if (replayButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
                        // Restart game
                        restartRequested = true;
                        window.close();
                    }

                    if (exitButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
                        restartRequested = false;
                        window.close();
                    }
                }
            }

        }

        // Clean up
        for (auto p1 : pieceslist) delete p1;
        for (auto p2 : capturedlist) delete p2;
    }

    void isEmpty() {      //boolean array to check if the squares are empty
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                board[i][j] = true;
            }
        }

        for (int i = 0; i < pieceslist.size(); i++) {
            Chesspiece* piece = pieceslist[i];
            board[piece->position.first][piece->position.second] = false;
        }
    }

    int minimax(int depth, int alpha, int beta, bool maximizingPlayer) {     //minimax algorithm with alpha-beta pruning
        if (depth == 0) {
            return evaluateBoard();
        }

        vector<MoveInfo> moves = generateAllMoves(currentmove);
        if (moves.empty()) {
            return evaluateBoard();
        }

        if (maximizingPlayer) {
            int maxEval = -100000;
            for (const MoveInfo& move : moves) {
                doMove(move);
                int eval = minimax(depth - 1, alpha, beta, false);
                undoMove();
                maxEval = max(maxEval, eval);
                alpha = max(alpha, eval);
                if (beta <= alpha) {
                    break;
                }
            }
            return maxEval;
        }
        else {
            int minEval = 100000;
            for (const MoveInfo& move : moves) {
                doMove(move);
                int eval = minimax(depth - 1, alpha, beta, true);
                undoMove();
                minEval = min(minEval, eval);
                beta = min(beta, eval);
                if (beta <= alpha) {
                    break;
                }
            }
            return minEval;
        }
    }

    vector<MoveInfo> getCastleMoves() {

        vector<MoveInfo> moves;

        vector<Chesspiece*> colorpieces = getPieces(currentmove);        //put all the pieces of the color in a vector
        isEmpty();        //check if the squares are empty

        King* colorking;     //get the king of the color

        for (auto& piece : colorpieces) {
            if (piece->color == currentmove && piece->type == "King") {
                colorking = dynamic_cast<King*>(piece);
                break;
            }
        }

        if (colorking->moveCount > 0) { // Check if the king has moved
            return moves; // No castling possible
        }

        for (auto& piece : colorpieces) {
            if (piece->color == currentmove && piece->type == "Rook") {
                Chesspiece* rook = piece;

                if (rook->moveCount == 0) { // Check if the rook has not moved

                    if (rook->position.first == 0 && colorking->position.first == 4) { // Left side castling
                        if (board[1][rook->position.second] && board[2][rook->position.second] && board[3][rook->position.second]) {
                            MoveInfo move(colorking, { 2, colorking->position.second });
                            move.setFollowup(rook, { 3, colorking->position.second }); // Set the rook's move
                            moves.push_back(move); // Add the castling move
                        }

                    }
                    else if (rook->position.first == 7 && colorking->position.first == 4) { // Right side castling
                        if (board[5][rook->position.second] && board[6][rook->position.second]) {
                            MoveInfo move(colorking, { 6, colorking->position.second });
                            move.setFollowup(rook, { 5, colorking->position.second }); // Set the rook's move
                            moves.push_back(move); // Add the castling move
                        }
                    }
                }
            }
        }
        return moves;
    }

    int getGameResult() {
        vector<MoveInfo> moves = generateAllMoves(currentmove);

        // Get the player's king
        King* king = nullptr;
        for (Chesspiece* piece : pieceslist) {
            if (piece->type == "King" && piece->color == currentmove) {
                king = dynamic_cast<King*>(piece);
                break;
            }

        }

        if (moves.empty()) {
            if (king->inCheck(pieceslist)) {
                return currentmove ? 1 : -1; // Checkmate 1 for black, -1 for white
            }
            else {
                return 0;   //0 for statemate
            }
        }
        return 2;   //2 for game not over
    }

    void makeTurn() {
        if (getGameResult() != 2) {
            return; // Game is over
        }

        if (currentmove) {
            //white's turn
            vector<MoveInfo> moves = generateAllMoves(currentmove);

            if (selectedMove.piece != nullptr) {

                //highlight the selected piece
                sf::RectangleShape green(sf::Vector2f(100.f, 100.f));
                green.setPosition(sf::Vector2f(selectedMove.piece->position.first * 100, selectedMove.piece->position.second * 100));
                green.setFillColor(sf::Color(130, 228, 124)); // Light green
                window.draw(green);

                for (auto& move : moves) {
                    if (move.piece == selectedMove.piece) {
                        //highlight the valid moves
                        sf::RectangleShape blue(sf::Vector2f(100.f, 100.f));
                        blue.setPosition(sf::Vector2f(move.position.first * 100, move.position.second * 100));
                        blue.setFillColor(sf::Color(173, 216, 230)); // Light blue
                        window.draw(blue);
                    }
                }
            }

            if (selectedMove.position.first != -1 && selectedMove.position.second != -1) {
                doMove(selectedMove); // Move the piece
                selectedMove.piece = nullptr; // Reset selected piece
                selectedMove.position = { -1,-1 }; // Reset selected move
                currentmove = false; // Switch to black
            }

        }
        else {

            if (vsAI) {
                vector<MoveInfo> moves = generateAllMoves(currentmove);
                int timeLimitMillis = 100;
                int maxDepth = 8;

                auto startTime = steady_clock::now();

                int bestScore = 100000;
                MoveInfo bestMove = moves[0];

                for (int depth = 0; depth <= maxDepth; ++depth) {
                    if (duration_cast<milliseconds>(steady_clock::now() - startTime).count() > timeLimitMillis)
                        break;

                    for (auto& move : moves) {
                        if (duration_cast<milliseconds>(steady_clock::now() - startTime).count() > timeLimitMillis)
                            break;

                        doMove(move);
                        int score = minimax(0, -100000, 100000, true);
                        undoMove();

                        if (score < bestScore) {
                            bestScore = score;
                            bestMove = move;
                        }
                    }
                }

                doMove(bestMove); // AI plays
                currentmove = true;
            }
            else {
                vector<MoveInfo> moves = generateAllMoves(currentmove);

                if (selectedMove.piece != nullptr) {

                    //highlight the selected piece
                    sf::RectangleShape green(sf::Vector2f(100.f, 100.f));
                    green.setPosition(sf::Vector2f(selectedMove.piece->position.first * 100, selectedMove.piece->position.second * 100));
                    green.setFillColor(sf::Color(130, 228, 124)); // Light green
                    window.draw(green);

                    for (auto& move : moves) {
                        if (move.piece == selectedMove.piece) {
                            //highlight the valid moves
                            sf::RectangleShape blue(sf::Vector2f(100.f, 100.f));
                            blue.setPosition(sf::Vector2f(move.position.first * 100, move.position.second * 100));
                            blue.setFillColor(sf::Color(173, 216, 230)); // Light blue
                            window.draw(blue);
                        }
                    }
                }

                if (selectedMove.position.first != -1 && selectedMove.position.second != -1) {
                    doMove(selectedMove); // Move the piece
                    selectedMove.piece = nullptr; // Reset selected piece
                    selectedMove.position = { -1,-1 }; // Reset selected move
                    currentmove = true; // Switch to white
                }
            }
        }
    }

    vector<Chesspiece*> getPieces(bool col) {
        vector<Chesspiece*> colorpieces;
        for (auto& piece : pieceslist) {
            if (piece->color == col) {
                colorpieces.push_back(piece);
            }
        }
        return colorpieces;
    }

    vector<MoveInfo> generateAllMoves(bool col) {     //returns in the format of piece and move
        vector<MoveInfo> moves;

        vector<Chesspiece*> colorpieces = getPieces(col);        //put all the pieces of the color in a vector

        King* colorking=nullptr;     //get the king of the color

        for (auto& piece : colorpieces) {
            if (piece->color == col && piece->type == "King") {
                colorking = dynamic_cast<King*>(piece);
                break;
            }
        }
        if (!colorking) {
            std::cerr << "Error: Could not find the king for color " << col << "\n";
            return {};
        }

        for (auto& piece : colorpieces) {
            vector<std::pair<int, int>> pieceMoves = piece->getValidMoves(pieceslist);
            for (auto& move : pieceMoves) {
                doMove(MoveInfo(piece, move)); // Simulate the move
                if (!colorking->inCheck(pieceslist)) {
                    moves.push_back(MoveInfo(piece, move)); // Add the move if it doesn't put the king in check
                }
                undoMove(); // Undo the move
            }
        }

        vector<MoveInfo> castlingMoves = getCastleMoves();
        for (auto& move : castlingMoves) {
            doMove(move); // Simulate the castling move
            if (!colorking->inCheck(pieceslist)) {
                moves.push_back(move); // Add the castling move if it doesn't put the king in check
            }
            undoMove();
        }

        return moves;
    }

    void doMove(MoveInfo move, bool followup = false) { // Move the piece
        pair<int, int> from = move.piece->position; // Get the piece's current position
        pair<int, int> to = move.position;

        move.piece->position = to; // Update the piece's position
        move.piece->moveCount++; // Increment the move count

        Chesspiece* capturedPiece = nullptr;

        for (auto& p : pieceslist) {
            if (p->position == to && p != move.piece) {
                capturedPiece = p;
                break;
            }
        }

        if (capturedPiece) {
            capturedlist.push_back(capturedPiece); // Add captured piece to the captured list
            pieceslist.erase(std::remove(pieceslist.begin(), pieceslist.end(), capturedPiece), pieceslist.end()); // Remove from pieces list
        }

        // PAWN PROMOTION TO QUEEN (For Now)
        if (move.piece->type == "Pawn") {
            int promotionRow = move.piece->color ? 0 : 7;
            if (move.position.second == promotionRow) {
                // Replace with a queen
                Chesspiece* promoted = new Queen(move.piece->color, move.position);
                pieceslist.push_back(promoted);

                capturedlist.push_back(move.piece); // to delete later
                pieceslist.erase(std::remove(pieceslist.begin(), pieceslist.end(), move.piece), pieceslist.end());

                return; // Skip adding to MoveList because the piece is now gone
            }
        }
        //Pawn promotion end

        if (followup) {
            MoveList.push_back(ChessMove(move.piece, from, to, capturedPiece, followup));
        }
        else {
            MoveList.push_back(ChessMove(move.piece, from, to, capturedPiece));
        }


        if (move.followup) {
            doMove(MoveInfo(move.piece2, move.position2), true); // do the followup move
        }
    }

    void drawGameOverScreen() {
        window.clear(sf::Color::Black);
        window.draw(bgSprite);
        window.draw(statusText);
        window.draw(replayButton);
        window.draw(replayText);
        window.draw(exitButton);
        window.draw(exitText);
        window.display();
    }

    bool shouldRestart() const {
        return restartRequested;
    }

    void undoMove() {
        if (MoveList.empty()) return;

        ChessMove lastMove = MoveList.back();
        MoveList.pop_back();


        // Restore the piece's position
        lastMove.piece->position = lastMove.from[0];
        lastMove.piece->moveCount--; // Decrement the move count

        // Remove the captured piece from the captured list
        if (lastMove.capturedPiece) {
            capturedlist.erase(std::remove(capturedlist.begin(), capturedlist.end(), lastMove.capturedPiece), capturedlist.end());
        }

        // If a piece was captured, restore it
        if (lastMove.capturedPiece) {
            pieceslist.push_back(lastMove.capturedPiece);
            lastMove.capturedPiece->position = lastMove.to[0];
        }
        if (lastMove.followup) {
            undoMove(); // do the followup move
        }
    }

    void handleMouseClick() {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos); // Adjust for view

        // Convert to board coordinates
        int x = static_cast<int>(worldPos.x / 100);
        int y = static_cast<int>(worldPos.y / 100);

        vector<MoveInfo> moves = generateAllMoves(currentmove);

        if (selectedMove.piece == nullptr) {
            // Step 1: Select the piece if it's your turn
            for (auto& piece : getPieces(currentmove)) {
                if (piece->position.first == x && piece->position.second == y) {
                    selectedMove.piece = piece;
                    return;
                }
            }
        }
        else {
            // Step 2: Check if clicked square is a valid move
            for (auto& move : moves) {
                if (move.piece == selectedMove.piece &&
                    move.position.first == x && move.position.second == y) {
                    doMove(move);
                    selectedMove.piece = nullptr;
                    selectedMove.position = { -1, -1 };
                    currentmove = !currentmove; // switch turn
                    return;
                }
            }

            // Step 3: Deselect or reselect another piece of same player
            for (auto& piece : getPieces(currentmove)) {
                if (piece->position.first == x && piece->position.second == y) {
                    selectedMove.piece = piece;
                    return;
                }
            }

            // Step 4: Clicked invalid square — deselect
            selectedMove.piece = nullptr;
            selectedMove.position = { -1, -1 };
        }
    }



};
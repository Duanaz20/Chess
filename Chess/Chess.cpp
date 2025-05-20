#include "board.h"
using namespace std;

int main() {
    while (true) {
        ChessBoard board;
        board.run();

        // When the game ends, ask: Replay or Exit
        if (!board.shouldRestart())
            break;  // Exit the loop and program
    }

    return 0;
}
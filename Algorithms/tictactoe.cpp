/*
Program to simulate a tic-tac-toe game between the player and a perfect AI
*/

#include <iostream>

using namespace std;

#define other(p) (p=='X'?'O':'X')

char whoWon(const char b[][3])
{
    if (b[0][0] != '_') {
        if (b[0][0] == b[1][0] && b[0][0] == b[2][0] ||
            b[0][0] == b[0][1] && b[0][0] == b[0][2]) return b[0][0];
    }
    if (b[2][2] != '_') {
        if (b[2][2] == b[1][2] && b[2][2] == b[0][2] ||
            b[2][2] == b[2][1] && b[2][2] == b[2][0]) return b[2][2];
    }
    if (b[1][1] != '_') {
        if (b[1][1] == b[1][0] && b[1][1] == b[1][2] ||
            b[1][1] == b[0][1] && b[1][1] == b[2][1] ||
            b[1][1] == b[0][0] && b[1][1] == b[2][2] ||
            b[1][1] == b[2][0] && b[1][1] == b[0][2]) return b[1][1];
    }
    return '_';
}

double place(char p, int toR, int toC, char board[][3])
{
    char win = whoWon(board);
    if (win != '_')
        return (win==p?1:-1);
    double winChance = 0;
    int numMoves = 0;
    for (int r = 2; r >= 0; r--) {
        for (int c = 2; c >= 0; c--) {
            if (board[r][c] == '_') {
                board[r][c] = other(p);
                double result = -1 * place(other(p), r, c, board);
                board[r][c] = '_';
                if (result == -1)
                    return -1;
                winChance += result;
                numMoves++;
            }
        }
    }
    if (numMoves == 0)
        return 0;
    return winChance / numMoves;
}

void nextMove(char player, char board[][3])
{
    double best = -2;
    int bR, bC;
    for (int r = 2; r >= 0 && best < 1; r--) {
        for (int c = 2; c >= 0 && best < 1; c--) {
            if (board[r][c] == '_') {
                board[r][c] = player;
                double result = place(player, r, c, board);
                // Uncomment to see AI analysis
                cout << "AI chance of winning at " << r+1 << " " << c+1 << ": " << result << endl;
                board[r][c] = '_';
                if (best < result) {
                    best = result;
                    bR = r;
                    bC = c;
                }
            }
        }
    }
    board[bR][bC] = player;
}

void prompt(char p, char board[][3])
{
    int r, c;
    do {
        cout << "Enter row: "; cin >> r;
        cout << "Enter col: "; cin >> c;
    } while (r>3 || r<1 || c>3 || c<1 || board[r-1][c-1] != '_');
    board[r-1][c-1] = p;
}

void printBoard(char board[][3])
{
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++)
            cout << board[r][c];
        cout << endl;
    }
}

bool hasSpots(char board[][3])
{
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (board[r][c] == '_')
                return true;
    return false;
}

int main()
{
    char board[3][3] = {{'_','_','_'},{'_','_','_'},{'_','_','_'}};
    char player, choice;
    cout << "Go first? (y/n) ";
    cin >> choice;
    if (choice == 'y') {
        player = 'X';
        printBoard(board);
        prompt(player, board);
    }
    else {
        player = 'O';
    }

    while (true) {
        nextMove(other(player), board);
        if (whoWon(board) != '_' || !hasSpots(board)) break;
        printBoard(board);
        prompt(player, board);
        if (whoWon(board) != '_' || !hasSpots(board)) break;
    }
    printBoard(board);
    if (whoWon(board) == '_') cout << "Draw!" << endl;
    else cout << whoWon(board) << " wins!" << endl;

    return 0;
}

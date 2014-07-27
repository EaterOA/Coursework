/*
Algorithm to determine what moves to make at what state during the game
"10-10", where two players take turn removing one stone from either or both of
two piles of stones. The player who removes the final stone wins.

In this algorithm, 'r' represents the first pile, 'c' the second pile, 'b'
means both piles, and 'x' means there is no optimal move.

Position row = 2, col = 1 for instance means there are 2 stones in pile one and
1 stone in pile two.
*/

#include <iostream>

using namespace std;

#define TSIZE 10

char table[TSIZE][TSIZE];

bool canWin(int r, int c)
{
	return r >= 0 && c >= 0 && table[r][c] == 'x';
}

int main()
{
    // Player who cannot remove any stone loses
	table[0][0] = 'x';
	for (int c = 1; c < TSIZE; c++) {
		for (int r = 0; r <= c; r++) {
            // Straightforward dynamic programming solution
            // You want to push your opponent to a lose state
			if (canWin(r-1,c)) table[r][c] = 'r';
			else if (canWin(r,c-1)) table[r][c] = 'c';
			else if (canWin(r-1,c-1)) table[r][c] = 'b';
			else table[r][c] = 'x';

            // Decisions are antisymmetric, e.g. picking 'r' for row 1 col 0
            // means picking 'c' for row 0 col 1
			if (table[r][c] == 'c') table[c][r] = 'r';
			else if (table[r][c] == 'r') table[c][r] = 'c';
			else table[c][r] = table[r][c];
		}
	}
	for (int r = 0; r < TSIZE; r++) {
		for (int c = 0; c < TSIZE; c++)
			cout << table[r][c];
		cout << "\n";
	}
}

/*
Problem: Given a sudoku board, computes and outputs a solution to the board

Input:
81 whitespace separated numbers representing the board from stdin

Sample input:
0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0

Sample output:
1 2 3 4 5 6 7 8 9
4 5 6 7 8 9 1 2 3
7 8 9 1 2 3 4 5 6
2 3 1 6 7 4 8 9 5
8 7 5 9 1 2 3 6 4
6 9 4 5 3 8 2 1 7
3 1 7 2 6 5 9 4 8
5 4 2 8 9 7 6 3 1
9 6 8 3 4 1 5 7 2
*/

#include <iostream>

using namespace std;

bool solve(int grid[][9])
{
	// Find unused numbers in each row, column, and 3x3 box
	bool y[9][9] = {0}, x[9][9] = {0}, b[9][9] = {0};
	for (int r = 0; r < 9; r++) {
		for (int c = 0; c < 9; c++){
			if (grid[r][c]) {
				x[c][grid[r][c]-1] = 1;
				y[r][grid[r][c]-1] = 1;
				b[r/3*3 + c/3][grid[r][c]-1] = 1;
			}
		}
	}
	// Find the point with lowest amount of numbers that can fit there
	bool validNums[9][9][9] = {0};
	int lowest = 10, lowestR, lowestC;
	for (int r = 0; r < 9; r++) {
		for (int c = 0; c < 9; c++){
			if (grid[r][c]) continue;
			int fit = 0;
			for (int i = 0; i < 9; i++) {
				validNums[r][c][i] = !x[c][i] && !y[r][i] && !b[r/3*3 + c/3][i];
				if (validNums[r][c][i]) fit++;
			}
			if (fit < lowest) {
				lowest = fit;
				lowestR = r;
				lowestC = c;
			}
		}
	}
	// If no point was found, puzzle is complete
	if (lowest == 10) {
		for (int r = 0; r < 9; r++) {
			for (int c = 0; c < 9; c++)
				cout << grid[r][c] << " ";
			cout << endl;
		}
		return true;
	}
	// If a point with 0 fit was found, puzzle is invalid
	if (lowest == 0) return false;
	// Try each valid number on that point and recurse
	for (int i = 0; i < 9; i++) {
		if (validNums[lowestR][lowestC][i]) {
			grid[lowestR][lowestC] = i + 1;
			if (solve(grid)) return true;
			grid[lowestR][lowestC] = 0;
		}
	}
	return false;
}

int main()
{
	int board[9][9];
	for (int j = 0; j < 9; j++)
		for (int k = 0; k < 9; k++)
			cin >> board[j][k];
	solve(board);
	return 0;
}

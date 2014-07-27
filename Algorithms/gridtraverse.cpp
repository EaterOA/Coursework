/*
Algorithm to calculate the number of paths to every square in a grid, starting from the upper left corner and only moving right or down
*/

#include <iostream>
#include <queue>
#include <cstdlib>
#include <string.h>

using namespace std;

struct Point
{
    Point(int rr, int cc): r(rr), c(cc) {}
	int r, c;
};

void error(const string &msg)
{
    cout << msg << "\n";
    exit(1);
}

int main()
{
    cout << "Size of grid: ";
    int N;
    cin >> N;

    if (N <= 0) error("Weird number you got there");
    if (N >= 1000) error("Way too big for me!");

    long long *map = new long long[N*N];
    memset(map, 0, sizeof(long long)*N*N);

	map[0] = 1;
    queue<Point> q;
	q.push(Point(0, 0));
	while (!q.empty()) {
        // Straightforward dynamic programming solution
        // For each square, the number of ways to get to it is the sum of the
        // solutions for the square above and the square to the left
		const Point p = q.front(); q.pop();
		if (p.c + 1 < N) {
			map[p.r*N + p.c + 1] += map[p.r*N + p.c];
            // Avoid double counting; only "move" right on the first row
			if (p.r == 0)
				q.push(Point(p.r, p.c+1));
		}
		if (p.r + 1 < N) {
			map[(p.r + 1)*N + p.c] += map[p.r*N + p.c];
			q.push(Point(p.r + 1, p.c));
		}
	}
	cout << map[N*N-1] << endl;

    delete map;
}

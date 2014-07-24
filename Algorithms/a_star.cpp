#include <iostream>
#include <time.h>
#include <string.h>
#include <algorithm>
#include <cstdlib>
#include <queue>
#include <math.h>

using namespace std;

#define FSIZE 50
#define RAND(a,b) (rand() % ((b)-(a)+1) + (a))

const int DR[] = {0, 1, 0, -1};
const int DC[] = {1, 0, -1, 0};

struct Node 
{
    Node(int rr, int cc, int ddist): r(rr), c(cc), dist(ddist) {}
    int r, c;
    int dist;
    bool operator<(const Node& other) const { return dist < other.dist; }
};

inline bool isValid(int r, int c)
{
    return (r >= 0 && r < FSIZE && c >= 0 && c < FSIZE);
}

inline int getDist(int r1, int c1, int r2, int c2)
{
    return abs(r1-r2) + abs(c1-c2);
}

void placeWall(char field[FSIZE][FSIZE], int row, int col, int height, int width)
{
    for (int i = row; i < row+height; i++)
        for (int j = col; j < col+width; j++)
            field[i][j] = '@';
}

void placeRandomWall(char field[FSIZE][FSIZE])
{
    int height = RAND(FSIZE/7, FSIZE-1);
    int width = RAND(1, FSIZE/12);
    if (RAND(0,1)) {
        swap(height, width);
    }
    int row = RAND(0, FSIZE-height);
    int col = RAND(0, FSIZE-width);
    placeWall(field, row, col, height, width);
}

void printField(char field[FSIZE][FSIZE])
{
    for (int i = 0; i < FSIZE; i++) {
        for (int j = 0; j < FSIZE; j++)
            cout << field[i][j];
        cout << "\n";
    }
}

int main()
{
    srand((unsigned)time(0));
    char field[FSIZE][FSIZE];
    memset(field, '-', FSIZE*FSIZE);
    for (int i = 0; i < 10; i++)
        placeRandomWall(field);
    int curR = -1, curC = -1, endR = -1, endC = -1;
    for (int dist = 0; curR == -1; dist++)
        for (int diag = 0; diag <= dist; diag++) {
            int r = diag;
            int c = dist-diag;
            if (field[r][c] == '-') {
                curR = r;
                curC = c;
                field[r][c] = '*';
            }
        }
    for (int dist = 0; endR == -1; dist++)
        for (int diag = 0; diag <= dist; diag++) {
            int r = FSIZE-1-diag;
            int c = FSIZE-1-dist+diag;
            if (field[r][c] == '-') {
                endR = r;
                endC = c;
                field[r][c] = 'x';
            }
        }

    priority_queue<Node> q;
    //WRONG. Also account for dist from start
    q.push(Node(curR, curC, getDist(curR, curC, endR, endC)));
    while (!q.empty()) {
        Node n = q.top(); q.pop();
        curR = n.r;
        curC = n.c;
        if (getDist(curR, curC, endR, endC) == 0) break;
        for (int i = 0; i < 4; i++) {
            if (isValid(curR + DR[i], curC + DC[i])) {

            }
        }
    }
    printField(field);
    //if (system("CLS")) system("clear");
}
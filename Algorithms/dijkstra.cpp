/*
A program that implements and demonstrates Dijkstra's algorithm.

Problem: Given a set of points connected by directed edges, find the two
points furthest apart in terms of minimum edge length required to get from one
to the other.
(Basically, find the maximum of the shortest paths in a connected graph)

Input:
Line 1:             N, the number of points
Lines 2 to N+1:     The coordinate of every point
Lines N+2 to 2*N+1: Adjacency list showing which points this connects to
                    <number of connections> [point 1] [point 2]...

Sample input:
8
10 10
15 10
20 10
15 15
20 15
30 15
25 10
30 10
1 1
4 0 2 3 4
2 1 4
2 1 4
4 1 2 3 5
1 6
2 5 7
1 6

Sample output:
The longest shortest path is: 0 -> 1 -> 4 -> 5 -> 6 -> 7
Distance: 34.142136
*/

#include <iostream>
#include <math.h>
#include <string>
#include <queue>
#include <vector>
#include <limits>
#include <algorithm>
#include <set>

using namespace std;

// Define which dijkstra implementation to use
#define IMPL 3

struct Point {
    int x, y, id;
    double dist;
    int from;
    bool operator<(const Point &other) const {
        if (dist != other.dist)
            return dist > other.dist;
        // Little hack to guarantee uniqueness
        if (x != other.x)
            return x > other.x;
        return y > other.y;
    }
};

struct Edge {
    int start, end;
    double dist;
};

inline double getDist(Point &p1, Point &p2)
{
    return sqrt((double)(p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
}

int main()
{
    const double INF = numeric_limits<double>::infinity();

    int N;
    vector<Point> points;
    vector<vector<Edge> > edges;
    vector<int> farthestSeq(1, 0);
    double farthestDist = 0;

    // Read input and initialize a vector of points
    // and a weighted adjacency matrix of edges
    cin >> N;
    for (int i = 0; i < N; i++) {
        int x, y;
        cin >> x >> y;
        Point p = {x, y, i, 0};
        points.push_back(p);
    }
    for (int i = 0; i < N; i++) {
        int size, next;
        cin >> size;
        edges.push_back(vector<Edge>());
        for (int j = 0; j < size; j++) {
            cin >> next;
            Edge e = {i, next, getDist(points[i], points[next])};
            edges.back().push_back(e);
        }
    }

    // Use Dijkstra's algorithm on every city to find the shortest path
    // from city i to every other city
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            points[j].dist = INF;
        points[i].dist = 0;

        #if IMPL == 1
        // Implementation #1: O(V^2)
        vector<bool> visited(N, 0);
        for (int j = 0; j < N; j++) {
            int minNode = -1;
            for (int k = 0; k < N; k++)
                if (!visited[k] && points[k].dist != INF && (minNode == -1 || points[k].dist < points[minNode].dist))
                    minNode = k;
            if (minNode == -1) break;
            visited[minNode] = 1;
            for (int k = 0; k < edges[minNode].size(); k++) {
                int next = edges[minNode][k].end;
                double d = edges[minNode][k].dist + points[minNode].dist;
                if (d < points[next].dist) {
                    points[next].dist = d;
                    points[next].from = minNode;
                }
            }
        }

        #elif IMPL == 2
        // Implementation #2: O(ElogE)
        priority_queue<Point> pq;
        pq.push(points[i]);
        while (!pq.empty()) {
            Point p = pq.top(); pq.pop();
            for (int j = 0; j < edges[p.id].size(); j++) {
                int next = edges[p.id][j].end;
                double d = edges[p.id][j].dist + points[p.id].dist;
                if (d < points[next].dist) {
                    points[next].dist = d;
                    points[next].from = p.id;
                    pq.push(points[next]);
                }
            }
        }

        #else
        // Implementation #3: O(ElogV)
        set<Point> s;
        s.insert(points[i]);
        while (!s.empty()) {
            set<Point>::iterator last = --s.end();
            Point p = *last;
            s.erase(last);
            for (int j = 0; j < edges[p.id].size(); j++) {
                int next = edges[p.id][j].end;
                double d = edges[p.id][j].dist + points[p.id].dist;
                if (d < points[next].dist) {
                    if (points[next].dist != INF) {
                        set<Point>::iterator it = s.find(points[next]);
                        if (it != s.end())
                            s.erase(it);
                    }
                    points[next].dist = d;
                    points[next].from = p.id;
                    s.insert(points[next]);
                }
            }
        }

        #endif
        // Record the distance of the city farthest from city n
        for (int j = 0; j < N; j++) {
            if (points[j].dist != INF
                && points[j].dist > farthestDist) {
                farthestDist = points[j].dist;
                farthestSeq = vector<int>();
                for (int k = j; k != i; k = points[k].from)
                    farthestSeq.push_back(k);
                farthestSeq.push_back(i);
                reverse(farthestSeq.begin(), farthestSeq.end());
            }
        }
    }

    // Output!
    cout.setf(ios::fixed);
    cout.precision(6);
    cout << "The longest shortest path is: " << farthestSeq[0];
    for (int i = 1; i < farthestSeq.size(); i++)
        cout << " -> " << farthestSeq[i];
    cout << "\n";
    cout << "Distance: " << farthestDist << "\n";
}


/*
Problem: A country is made up of multiple cities that are connected by roads.
The size of a country is the distance between the two cities that are farthest
from each other when taking the shortest route. Given two countries, find a road
to connect them in such a way that the size of the unified country is minimized.

Input:
- N, the number of cities
- The coordinate of every city
- An unweighted adjacency matrix showing which cities are connected to each other

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
01000000
10111000
01001000
01001000
01110000
00000010
00000101
00000010

Sample output:
Connect (20, 10) with (25, 10)
to form a country of size 22.071068
*/

#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <queue>
#include <vector>

#define INT_MAX 2147483647
#define max(a,b) (a>b?a:b)

using namespace std;

struct City {
	int x, y, country, id;
	double farthest, dist;
    bool operator<(const City &other) const { return dist > other.dist; }
};

int N;
vector<City> cities;
vector<vector<double> > roads;
vector<vector<City> > countries;

//Returns the pythagorean distance between two cities
double getDist(City &p1, City &p2)
{
	return sqrt((double)(p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
}

//Adds a city and all its neighbors to a country's list of cities
void flood(int c, int country)
{
	if (cities[c].country > -1) return;
	cities[c].country = country;
	if (country == countries.size()) countries.push_back(vector<City>());
	countries[country].push_back(cities[c]);
	for (int n = 0; n < N; n++) {
		if (roads[c][n] > 0) flood(n, country);
	}
}

//Returns the size of a country
double findSize(int country)
{
	double size = 0;
	for (int n = 0; n < countries[country].size(); n++) {
		if (size < countries[country][n].farthest) {
			size = countries[country][n].farthest;
		}
	}
	return size;
}

int main()
{
	//Read input and initialize a vector of cities
	//and a weighted adjacency matrix of roads
	cin >> N;
	cities.resize(N);
	roads.resize(N);
	for (int n = 0; n < N; n++) {
		roads[n].resize(N);
		int x, y;
		cin >> x >> y;
		City c = {x, y, -1, n, 0, 0};
		cities[n] = c;
	}
	cin.ignore(100, '\n');
	for (int i = 0; i < N; i++) {
		string s;
		getline(cin, s);
		for (int j = 0; j < N; j++) {
			if (s[j] == '1') roads[i][j] = getDist(cities[i], cities[j]);
			else roads[i][j] = 0;
		}
	}

	//Use Dijkstra's algorithm on every city to find the shortest path
	//from city n to every other city
	for (int n = 0; n < N; n++) {
		for (int i = 0; i < N; i++) cities[i].dist = INT_MAX;
		cities[n].dist = 0;
		priority_queue<City> pq;
		pq.push(cities[n]);
		while (!pq.empty()) {
			City c = pq.top();
			pq.pop();
			for (int next = 0; next < N; next++) {
				if (roads[c.id][next] > 0) {
					double d = roads[c.id][next] + cities[c.id].dist;
					if (d < cities[next].dist) {
						cities[next].dist = d;
						pq.push(cities[next]);
					}
				}
			}
		}
		//Record the distance of the city farthest from city n
		for (int i = 0; i < N; i++) {
			if (cities[i].dist != INT_MAX && cities[i].dist > cities[n].farthest) {
				cities[n].farthest = cities[i].dist;
			}
		}
	}

	//Flood fill to form two countries
	for (int n = 0; n < N; n++) flood(n, countries.size());

	//Form a road from every city in country 0 to every city in country 1
	//and find the connection that results in the smallest combined country
	double lowest = INT_MAX;
	City p1, p2;
	for (int r = 0; r < countries[0].size(); r++) {
		for (int c = 0; c < countries[1].size(); c++) {
			double d = countries[0][r].farthest + countries[1][c].farthest + getDist(countries[0][r], countries[1][c]);
			if (d < lowest) {
				lowest = d;
				p1 = countries[0][r];
				p2 = countries[1][c];
			}
		}
	}

	//Smallest combined country cannot be smaller than original countries
	double d0 = findSize(0), d1 = findSize(1);
	if (lowest < max(d0, d1)) {
		lowest = max(d0, d1);
	}

	//Output!
	cout.setf(ios::fixed);
	cout.precision(6);
	cout << "Connect (" << p1.x << ", " << p1.y << ") with (" << p2.x << ", " << p2.y << ")\n";
	cout << "to form a country of size " << lowest << endl;
    return 0;
}


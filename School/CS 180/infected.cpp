/*
Implemented solution to Ch.3 #11, something about the spread of a virus across 
a computer network
*/

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct Connection
{
	Connection(int s, int d, int t): src(s), des(d), time(t) {}
	int src, des;
	int time;
	bool operator<(const Connection &other) { return time < other.time; }
};

struct Computer
{
	vector<int> adj_list;
	char visited;
};

map<int, Computer> comp_map;
bool* infected;

bool has_infected(int cur)
{
	bool flag = infected[cur];
	comp_map[cur].visited = 1;
	for (int i = 0; i < comp_map[cur].adj_list.size(); i++) {
		int next = comp_map[cur].adj_list[i];
		if (comp_map[next].visited < 1)
			flag |= has_infected(next);
	}
	return flag;
}

void spread_infection(int cur)
{
	infected[cur] = true;
	comp_map[cur].visited = 2;
	for (int i = 0; i < comp_map[cur].adj_list.size(); i++) {
		int next = comp_map[cur].adj_list[i];
		if (comp_map[next].visited < 2)
			spread_infection(next);
	}
}

bool is_infected(int source, int des, int stime, int etime, int n, vector<Connection> list)
{
	infected = new bool[n];
	memset(infected, 0, n);
	infected[source] = true;
	for (int con = 0; con < list.size() && list[con].time <= etime;) {
		if (list[con].time < stime) continue;
		do {
			comp_map[list[con].src].adj_list.push_back(list[con].des);
			comp_map[list[con].src].visited = false;
			comp_map[list[con].des].adj_list.push_back(list[con].src);
			comp_map[list[con].des].visited = false;
		} while (++con < list.size() && list[con-1].time == list[con].time);
		for (map<int, Computer>::iterator iter = comp_map.begin(); iter != comp_map.end(); iter++) {
			if (!iter->second.visited && has_infected(iter->first)) {
				spread_infection(iter->first);
			}
		}
		comp_map = map<int, Computer>();
	}
	for (int i = 0; i < n; i++) {
		cout << infected[i] << " ";
	}
	cout << endl;
	bool res = infected[des];
	delete infected;
	return res;
}

#include <time.h>
#include <windows.h>

class Time
{
  public:
    Time()
    {
        QueryPerformanceCounter(&m_time);
    }
    double operator-(Time start) const
    {
        LARGE_INTEGER ticksPerSecond;
        QueryPerformanceFrequency(&ticksPerSecond);
        return (1000.0 * (m_time.QuadPart - start.m_time.QuadPart)) / ticksPerSecond.QuadPart;
    }
  private:
    LARGE_INTEGER m_time;
};

#define M 40000
#define N 12000
#define T 16000

int main()
{
	srand(time(0));
	vector<Connection> list;
	for (int i = 0; i < M; i++) {
		list.push_back(Connection(rand() % N, rand() % N, rand() % T));
	}
	sort(list.begin(), list.end());
	Time start;
	cout << is_infected(0, 1, 0, T, N, list) << endl;
	Time end;
	cout << "Elapsed: " << end-start << endl;
}
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

vector<int> gale_shapley(vector<vector<int> > pref_list1, vector<vector<int> > pref_list2)
{
    int N = pref_list1.size();
    vector<int> match_list1(N, -1), match_list2(N, -1);
    queue<int> unengaged;
    vector<int*> pref_map2;
    for (int i = 0; i < N; i++) {
        unengaged.push(i);
        pref_map2.push_back(new int[N]);
        for (int j = 0; j < N; j++) pref_map2[i][pref_list2[i][j]] = j;
        reverse(pref_list1[i].begin(), pref_list1[i].end());
    }
    while (!unengaged.empty()) {
        int cur = unengaged.front(); unengaged.pop();
        while (match_list1[cur] == -1) {
            int target = pref_list1[cur].back(); pref_list1[cur].pop_back();
            int rival = match_list2[target];
            if (rival == -1 || pref_map2[target][cur] < pref_map2[target][rival]) {
                match_list1[cur] = target;
                match_list2[target] = cur;
                if (rival != -1) {
                    match_list1[rival] = -1;
                    unengaged.push(rival);
                }
            }
        }
    }
    for (int i = 0; i < N; i++) delete pref_map2[i];
    return match_list1;
}

int main()
{
    vector<vector<int> > m, w;
    int mw0[] = {1, 0, 2}; m.push_back(vector<int>(begin(mw0), end(mw0)));
    int mw1[] = {2, 0, 1}; m.push_back(vector<int>(begin(mw1), end(mw1)));
    int mw2[] = {1, 2, 0}; m.push_back(vector<int>(begin(mw2), end(mw2)));
    int wm0[] = {1, 0, 2}; w.push_back(vector<int>(begin(wm0), end(wm0)));
    int wm1[] = {0, 1, 2}; w.push_back(vector<int>(begin(wm1), end(wm1)));
    int wm2[] = {0, 2, 1}; w.push_back(vector<int>(begin(wm2), end(wm2)));
    vector<int> matching = gale_shapley(m, w);
    for (int i = 0; i < 3; i++)
        cout << i << " - " << matching[i] << endl;
}

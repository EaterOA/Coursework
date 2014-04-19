#include <iostream>
#include <algorithm>

using namespace std;

const int PINS = 9;
const int FULLCONFIG = (1<<PINS)-1;

int dfs(int config, int cur, int memo[PINS][FULLCONFIG])
{
    if (memo[cur][config]) return memo[cur][config];
    int res = 1;
    for (int i = 0; i < 9; i++) {

        //Check legality
        int s = cur, t = i;
        if (config & (1<<s)) continue;  //s must be closed
        if (~config & (1<<t)) continue; //t must be open
        if (s > t) swap(s, t);          //blocks are bidirectional
        if ((config>>4)&(s==0)&(t==8)|  //topleft-bottomright block
            (config>>4)&(s==1)&(t==7)|  //top-bottom block
            (config>>4)&(s==2)&(t==6)|  //topright-bottomleft block
            (config>>4)&(s==3)&(t==5)|  //left-right block
            (config>>1)&(s==0)&(t==2)|  //top block
            (config>>3)&(s==0)&(t==6)|  //left block
            (config>>5)&(s==2)&(t==8)|  //right block
            (config>>7)&(s==6)&(t==8))  //bottom block
            continue;

        //DFS solution
        res += dfs(config & ~(1<<i), i, memo);

    }
    memo[cur][config] = res;
    return res;
}

int main()
{
    int memo[PINS][FULLCONFIG] = {0};   //Memoize for speed
    int res = 0;
    res += 4 * dfs(FULLCONFIG & ~(1<<0), 0, memo);  //Start at topleft (same as any corner)
    res += 4 * dfs(FULLCONFIG & ~(1<<1), 1, memo);  //Start at top (same as any side)
    res += 1 * dfs(FULLCONFIG & ~(1<<4), 4, memo);  //Start at middle
    cout << res << endl;
}
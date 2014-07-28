#include <iostream>
#include <vector>

using namespace std;

/*
Input: number
Output: A list of numbers representing the balanced ternary set that makes up
that number
*/
vector<int> toBTernary(int x)
{
    bool invert = x < 0;
    if (invert) x = -x;

    //Convert to ternary
    vector<int> ternary;
    while (x > 0) {
        ternary.push_back(x % 3);
        x /= 3;
    }

    //Add 111[...] with carry while subtracting 111[...] without carry
    int j = ternary.size();
    for (int i = 0; i < ternary.size(); i++) {
        ternary[i]++;
        if (ternary[i] > 2) {
            if (i+1 == j) ternary.push_back(0);
            ternary[i+1]++;
            ternary[i] -= 3;
        }
        ternary[i]--;
        if (invert) ternary[i] = -ternary[i];
    }
    return ternary;
}

int main()
{
    vector<int> bternary = toBTernary(100);
    for (int i = 0, j = 1; i < bternary.size(); i++, j *= 3)
        if (bternary[i])
            cout << bternary[i]*j << endl;
}

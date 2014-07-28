/*
A program to generate 4x4 Karnaugh maps with Miodrag specifications
(i.e. a map with x minterms, y prime implicants, of which z are essential)
*/

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class Implicator
{
public:
    //Geometrically unique excludes rotations and reflections of known kmaps
    //Functionally unique excludes maps that have the same switching expression form
    //as a known map (e.g. abc+bcd' has the same form as dcb+cba'), but this is not implemented
    enum Maptype { ALL, GEO_UNIQUE };

    Implicator()
    {
        //Initializing 4 input bit blocks
        X[0] = 0x6666;
        X[1] = 0x0ff0;
        X[2] = 0x3333;
        X[3] = 0x00ff;

        //Finding and organizing all geometrically unique kmaps
        bool seen[0x10000] = {0};
        for (unsigned i = 0; i <= 0xffff; i++) {
            if (seen[i]) continue;
            kmaps_gu[numBits(i)].push_back(i);
            seen[i] = true;
            seen[reflect(0, i)] = true;
            seen[reflect(1, i)] = true;
            i = rotate(i); seen[i] = true;
            i = rotate(i); seen[i] = true;
            i = rotate(i); seen[i] = true;
        }

        //Creating the prime implicant graph
        for (unsigned i = 0; i < 81; i++) {
            imp[i].block = computeBlock(i);
            for (int j = 0, k = 1; j < 4; j++, k *= 3) {
                if (i / k % 3 == 0) {
                    imp[i].children.push_back(i + k);
                    imp[i].children.push_back(i + 2*k);
                }
            }
        }
    }
    void printMap(unsigned kmap) const
    {
        for (int j = 15; j >= 0; j--) {
            cout << ((kmap >> j) & 1);
            if (j % 4 == 0) cout << "\n";
        }
    }
    vector<unsigned> findAllKMaps(int numEPIs, int numPIs, int numMinterms, Maptype type)
    {
        if (numMinterms < 0 || numMinterms > 16) return vector<unsigned>();
        if (numPIs <= 0 && numEPIs <= 0) return kmaps_gu[numMinterms];
        vector<unsigned> res;
        unsigned size, kmap;
        size = type == ALL ? 0xffff : kmaps_gu[numMinterms].size();
        for (unsigned i = 0; i < size; i++) {
            if (type == ALL) {
                kmap = i;
                if (numBits(kmap) != numMinterms) continue;
            }
            else {
                kmap = kmaps_gu[numMinterms][i];
            }
            vector<unsigned> piList = getPI(kmap);
            vector<unsigned> epiList = getEPI(kmap, piList);
            bool flag = (numPIs > 0 ? piList.size() == numPIs : 1) && (numEPIs > 0 ? epiList.size() == numEPIs : 1);
            if (flag) {
                res.push_back(kmap);
            }
        }
        return res;
    }
private:
    struct Implicant
    {
        vector<int> children;
        unsigned block;
    };

    unsigned X[4];
    vector<unsigned> kmaps_gu[17];
    Implicant imp[81];

    unsigned computeBlock(unsigned exp) const
    {
        unsigned block = 0xffff;
        for (int i = 0; i < 4; i++) {
            char opt = exp % 3;
            if (opt == 1) block &= X[i];
            if (opt == 2) block &= invert(X[i]);
            exp /= 3;
        }
        return block;
    }

    int numBits(unsigned n) const
    {
        int count = 0;
        for (; n; n >>= 1) count += n & 1;
        return count;
    }

    unsigned rotate(unsigned kmap) const
    {
        unsigned newKMap = 0;
        for (unsigned i = 0; i < 4; i++) {
            char row = (kmap >> (i*4)) & 0xf;
            for (unsigned j = 0; j < 4; j++) {
                unsigned value = (row & 1) << (4*(j+1)) >> (i+1);
                newKMap |= value;
                row >>= 1;
            }
        }
        return newKMap;
    }

    unsigned reflect(bool isHorizontal, unsigned kmap) const
    {
        if (isHorizontal) kmap = rotate(rotate(kmap));
        unsigned newKMap = 0;
        newKMap |= ((kmap >> 12) & 0xf) << 0;
        newKMap |= ((kmap >> 8) & 0xf) << 4;
        newKMap |= ((kmap >> 4) & 0xf) << 8;
        newKMap |= ((kmap >> 0) & 0xf) << 12;
        return newKMap;
    }

    unsigned invert(unsigned block) const
    {
        return block ^ 0xffff;
    }

    vector<unsigned> getEPI(unsigned kmap, const vector<unsigned> &pi) const
    {
        vector<unsigned> epi;
        char hitCount[16] = {0};
        for (unsigned i = 0; i < pi.size(); i++) {
            unsigned p = pi[i];
            for (unsigned j = 0; j < 16; j++) {
                hitCount[j] += p & 1;
                p >>= 1;
            }
        }
        for (unsigned i = 0; i < pi.size(); i++) {
            unsigned p = pi[i];
            for (unsigned j = 0; j < 16; j++) {
                if ((p & 1) && hitCount[j] == 1) {
                    epi.push_back(pi[i]);
                    break;
                }
                p >>= 1;
            }
        }
        return epi;
    }

    void nullChildren(bool enc[], unsigned exp) const
    {
        for (unsigned i = 0; i < imp[exp].children.size(); i++) {
            unsigned c = imp[exp].children[i];
            if (!enc[c]) nullChildren(enc, c);
            enc[c] = true;
        }
    }

    vector<unsigned> getPI(unsigned kmap) const
    {
        vector<unsigned> pi;
        queue<unsigned> q;
        q.push(0);
        bool enc[81] = {0};
        while (!q.empty()) {
            unsigned next = q.front(); q.pop();
            if (enc[next]) continue;
            enc[next] = true;
            const Implicant &cur = imp[next];
            if ((kmap | cur.block) == kmap) {
                nullChildren(enc, next);
                pi.push_back(cur.block);
            }
            else if ((kmap & cur.block) == 0) {
                nullChildren(enc, next);
            }
            else {
                for (unsigned i = 0; i < cur.children.size(); i++) {
                    q.push(cur.children[i]);
                }
            }
        }
        return pi;
    }
};

int main()
{
    Implicator imp;
    //Implicator::findAllKMaps(number of EPIs (<= 0 to ignore), number of PIs (<= 0 to ignore), number of minterms, ALL or GEO_UNIQUE)
    vector<unsigned> kmaps = imp.findAllKMaps(2, 4, 9, Implicator::GEO_UNIQUE);
    if (!kmaps.empty()) imp.printMap(kmaps[0]);
    for (unsigned i = 1; i < kmaps.size(); i++) {
        cout << "------\n";
        imp.printMap(kmaps[i]);
    }
}

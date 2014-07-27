#include <iostream>
#include <vector>
#include <math.h>
#include <cstring>

using namespace std;

vector<int> getPrimes(int P)
{
	if (P < 2) return vector<int>();
	int sievebound = (P-1)/2;
	int crossbound = ((int)sqrt((double)P)-1)/2;
	bool *sieve = new bool[(P-1)/2+1];
    memset(sieve, 0, (P-1)/2+1);
	for (int i = 1; i <= crossbound; i++)
		if (!sieve[i])
			for (int j = 2*i*(i+1); j <= sievebound; j += 2*i+1)
				sieve[j] = 1;
	vector<int> primes(1, 2);
	for (int i = 1; i <= sievebound; i++)
		if (!sieve[i])
			primes.push_back(2*i+1);
	delete [] sieve;
	return primes;
}


int main()
{
	vector<int> primes = getPrimes(100);
	for (int i = 0; i < primes.size(); i++)
		cout << primes[i] << endl;
}

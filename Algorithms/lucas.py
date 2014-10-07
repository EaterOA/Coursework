import math

def lucas(n,r,p):

    # Determine the base terms list [p^k, p^(k-1), ..., 1]
    bound = max(n,r)
    p_terms = [1]
    next = p
    while next <= bound:
        p_terms.append(next)
        next *= p
    p_terms.reverse()

    # Find the coefficients of n and r in base p
    n_coeff = []
    r_coeff = []
    for t in p_terms:
        n_coeff.append(n / t)
        r_coeff.append(r / t)
        n %= t
        r %= t

    # Compute the product of all the binomials
    res = 1
    for i, j in zip(n_coeff, r_coeff):
        res = (res * nCr(i, j)) % p
    return res

def nCr(n,r):
    if (r > n):
        return 0
    greater = max(r, n-r)
    lesser = min(r, n-r)
    numerator = 1
    for i in xrange(n, greater, -1):
        numerator *= i
    denominator = math.factorial(lesser)
    return numerator / denominator

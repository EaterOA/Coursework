#ifndef BIGNUM_HPP
#define BIGNUM_HPP

#include <vector>
#include <string>

class BigNum
{
public:
    BigNum()
    {
        sign = 0;
        d = std::vector<char>(1, 0);
    }
    BigNum(int i)
    {
        sign = i < 0;
        d = std::vector<char>(10, 0);
        for (int idx = 0; i; idx++, i /= 10)
            d[idx] = (sign ? -1 : 1) * i % 10;
    }
    void reduce()
    {
        for (int idx = d.size()-1; idx > 0 && d[idx] == 0; idx--)
            d.pop_back();
    }
    bool operator<(const BigNum& o)
    {
        if (sign && !o.sign) return true;
        if (!sign && o.sign) return false;
        int idx = max(d.size(), o.d.size());
        while (idx >= 0) {
            if (d[idx] != o.d[idx]) 
                return sign ^ (d[idx] < o.d[idx]);
            idx--;
        }
        return false;
    }
    BigNum operator+(const BigNum& o) const
    {
        BigNum r;
        if (!sign && o.sign)
            abs_sub(*this, o, r);
        else if (sign && !o.sign)
            abs_sub(o, *this, r);
        else
            abs_add(*this, o, r);
        return r;
    }
    BigNum operator-(const BigNum& o) const
    {
        BigNum r;
        if (!sign && o.sign)
            abs_add(*this, o, r);
        else if (sign && !o.sign) {
            abs_add(*this, o, r);
            r.sign = 1;
        }
        else
            abs_sub(*this, o, r);
        return r;
    }
    void operator+=(const BigNum& o)
    {
        BigNum r = *this + o;
        sign = r.sign;
        d = r.d;
    }
    void operator-=(const BigNum& o)
    {
        BigNum r = *this - o;
        sign = r.sign;
        d = r.d;
    }
    void toString(char* s) const
    {
        int i = 0, idx = d.size()-1;
        while (idx > 0 && d[idx] == 0) idx--;
        if (sign) s[i++] = '-';
        for (; idx >= 0; idx--, i++)
            s[i] = d[idx]+'0';
        s[i] = 0;

    }
    std::string toString() const
    {
        char* s = new char[d.size()+2];
        toString(s);
        std::string res(s);
        delete s;
        return res;
    }
    
private:
    template <class T>
    T max(T a, T b) const
    {
        return (a > b ? a : b);
    }
    void abs_sub(const BigNum& a, const BigNum& b, BigNum& c) const
    {
        unsigned m = max(a.d.size(), b.d.size());
        c.d.resize(m, 0);
        bool borrow = 0;
        for (unsigned idx = 0; idx < m; idx++) {
            c.d[idx] = (idx < a.d.size() ? a.d[idx] : 0) - (idx < b.d.size() ? b.d[idx] : 0) - borrow;
            if (c.d[idx] < 0) {
                c.d[idx] += 10;
                borrow = 1;
            }
            else {
                borrow = 0;
            }
        }
        c.reduce();
        c.sign = borrow ^ a.sign;
    }
    void abs_add(const BigNum& a, const BigNum& b, BigNum& c) const
    {
        unsigned m = max(a.d.size(), b.d.size());
        c.d.resize(m, 0);
        char carry = 0;
        for (unsigned idx = 0; idx < m; idx++) {
            char res = (idx < a.d.size() ? a.d[idx] : 0) + (idx < b.d.size() ? b.d[idx] : 0) + carry;
            carry = res / 10;
            c.d[idx] = res % 10;
        }
        if (carry) c.d.push_back(1);
        c.sign = a.sign;
    }

    bool sign;
    std::vector<char> d;
};

#endif
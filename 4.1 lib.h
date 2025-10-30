#ifndef LIB_H
#define LIB_H

#include <iostream>
#include <stdexcept>
using namespace std;

class Fraction {
private:
    int numerator;
    int denominator;

    void simplify(); // rút gọn phân số

public:
    // Constructors
    Fraction(int num = 0, int den = 1);
    Fraction(const Fraction& other);

    // Arithmetic Operators
    Fraction operator+(const Fraction& other) const;
    Fraction operator-(const Fraction& other) const;
    Fraction operator*(const Fraction& other) const;
    Fraction operator/(const Fraction& other) const;

    // Comparison Operators
    bool operator==(const Fraction& other) const;
    bool operator!=(const Fraction& other) const;
    bool operator>(const Fraction& other) const;
    bool operator<(const Fraction& other) const;
    bool operator>=(const Fraction& other) const;
    bool operator<=(const Fraction& other) const;

    // Assignment Operators
    Fraction& operator=(const Fraction& other);
    Fraction& operator+=(const Fraction& other);
    Fraction& operator*=(const Fraction& other);

    // Increment / Decrement
    Fraction& operator++();    // prefix ++
    Fraction operator++(int);  // postfix ++
    Fraction& operator--();    // prefix --
    Fraction operator--(int);  // postfix --

    // Type-cast Operators
    operator float() const;
    operator int() const;

    // Input / Output
    friend ostream& operator<<(ostream& os, const Fraction& f);
    friend istream& operator>>(istream& is, Fraction& f);
};

#endif

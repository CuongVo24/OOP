#include "lib.h"
#include <cmath>

// Private Helper
void Fraction::simplify() {
    int a = abs(numerator), b = abs(denominator);
    while (b != 0) {
        int t = a % b;
        a = b;
        b = t;
    }
    if (a != 0) {
        numerator /= a;
        denominator /= a;
    }
    if (denominator < 0) {
        numerator = -numerator;
        denominator = -denominator;
    }
}

// Constructors
Fraction::Fraction(int num, int den) {
    if (den == 0) throw invalid_argument("Denominator cannot be zero");
    numerator = num;
    denominator = den;
    simplify();
}

Fraction::Fraction(const Fraction& other) {
    numerator = other.numerator;
    denominator = other.denominator;
}

// Arithmetic Operators
Fraction Fraction::operator+(const Fraction& other) const {
    return Fraction(numerator * other.denominator + other.numerator * denominator,
                    denominator * other.denominator);
}

Fraction Fraction::operator-(const Fraction& other) const {
    return Fraction(numerator * other.denominator - other.numerator * denominator,
                    denominator * other.denominator);
}

Fraction Fraction::operator*(const Fraction& other) const {
    return Fraction(numerator * other.numerator, denominator * other.denominator);
}

Fraction Fraction::operator/(const Fraction& other) const {
    if (other.numerator == 0)
        throw invalid_argument("Division by zero fraction");
    return Fraction(numerator * other.denominator, denominator * other.numerator);
}

// Comparison Operators
bool Fraction::operator==(const Fraction& other) const {
    return numerator == other.numerator && denominator == other.denominator;
}
bool Fraction::operator!=(const Fraction& other) const {
    return !(*this == other);
}
bool Fraction::operator>(const Fraction& other) const {
    return numerator * other.denominator > other.numerator * denominator;
}
bool Fraction::operator<(const Fraction& other) const {
    return other > *this;
}
bool Fraction::operator>=(const Fraction& other) const {
    return !(*this < other);
}
bool Fraction::operator<=(const Fraction& other) const {
    return !(*this > other);
}

// Assignment Operators
Fraction& Fraction::operator=(const Fraction& other) {
    if (this != &other) {
        numerator = other.numerator;
        denominator = other.denominator;
    }
    return *this;
}

Fraction& Fraction::operator+=(const Fraction& other) {
    *this = *this + other;
    return *this;
}

Fraction& Fraction::operator*=(const Fraction& other) {
    *this = *this * other;
    return *this;
}

// Increment / Decrement
Fraction& Fraction::operator++() {
    numerator += denominator;
    simplify();
    return *this;
}

Fraction Fraction::operator++(int) {
    Fraction temp = *this;
    ++(*this);
    return temp;
}

Fraction& Fraction::operator--() {
    numerator -= denominator;
    simplify();
    return *this;
}

Fraction Fraction::operator--(int) {
    Fraction temp = *this;
    --(*this);
    return temp;
}

// Type-cast Operators
Fraction::operator float() const {
    return (float)numerator / denominator;
}

Fraction::operator int() const {
    return numerator / denominator;
}

// Input / Output
ostream& operator<<(ostream& os, const Fraction& f) {
    os << f.numerator;
    if (f.denominator != 1)
        os << "/" << f.denominator;
    return os;
}

istream& operator>>(istream& is, Fraction& f) {
    char slash;
    is >> f.numerator;
    if (is.peek() == '/') {
        is >> slash >> f.denominator;
    } else {
        f.denominator = 1;
    }
    f.simplify();
    return is;
}

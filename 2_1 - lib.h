#pragma once
#include <iostream>
#include <string>

class Fraction {
private:
    int numerator;
    int denominator;
    void normalize(); // chuẩn hóa dấu & rút gọn

public:
    // Constructors
    Fraction();
    Fraction(int num, int denom);
    Fraction(int value);
    Fraction(const std::string &str);
    Fraction(const Fraction &other);

    // Getters / Setters
    int getNum() const;
    int getDenom() const;
    void setNum(int num);
    void setDenom(int denom);

    // Input / Output
    void input();
    void output() const;

    // Core Methods
    Fraction reduce() const;
    Fraction inverse() const;
    Fraction add(const Fraction &other) const;
    int compare(const Fraction &other) const;
};

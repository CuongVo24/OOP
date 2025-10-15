#include "lib.h"
#include <sstream>
#include <numeric> // sử dụng hàm lấy ước chung lớn nhất
#include <stdexcept> // cung cấp các lớp ngoại lệ chuẩn
using namespace std;


int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

// Chuẩn hóa phân số: đảm bảo mẫu > 0 và rút gọn
void Fraction::normalize() {
    if (denominator == 0)
        throw invalid_argument("Denominator cannot be zero.");

    if (denominator < 0) {
        numerator = -numerator;
        denominator = -denominator;
    }

    int g = gcd(numerator, denominator);
    numerator /= g;
    denominator /= g;
}

// Constructors
Fraction::Fraction() : numerator(0), denominator(1) {}

Fraction::Fraction(int num, int denom) : numerator(num), denominator(denom) {
    normalize();
}

Fraction::Fraction(int value) : numerator(value), denominator(1) {}

Fraction::Fraction(const string& str) {
    stringstream ss(str);
    char slash;
    ss >> numerator >> slash >> denominator;
    if (ss.fail() || slash != '/')
        throw invalid_argument("Invalid fraction string format. Expected 'num/denom'.");
    normalize();
}

Fraction::Fraction(const Fraction& other)
    : numerator(other.numerator), denominator(other.denominator) {}

// --- Getters / Setters ---
int Fraction::getNum() const { return numerator; }
int Fraction::getDenom() const { return denominator; }

void Fraction::setNum(int num) {
    numerator = num;
    normalize();
}

void Fraction::setDenom(int denom) {
    denominator = denom;
    normalize();
}

// --- Input / Output ---
void Fraction::input() {
    char slash;
    cout << "Enter fraction (num/den): ";
    cin >> numerator >> slash >> denominator;
    if (denominator == 0)
        throw invalid_argument("Denominator cannot be zero.");
    normalize();
}

void Fraction::output() const {
    if (denominator == 1)
        cout << numerator;
    else
        cout << numerator << "/" << denominator;
}

// --- Core Methods ---
Fraction Fraction::reduce() const {
    Fraction result(*this);
    result.normalize();
    return result;
}

Fraction Fraction::inverse() const {
    if (numerator == 0)
        throw domain_error("Cannot invert a zero fraction.");
    return Fraction(denominator, numerator);
}

Fraction Fraction::add(const Fraction& other) const {
    int num = numerator * other.denominator + other.numerator * denominator;
    int den = denominator * other.denominator;
    return Fraction(num, den);
}

int Fraction::compare(const Fraction& other) const {
    long long lhs = static_cast<long long>(numerator) * other.denominator;
    long long rhs = static_cast<long long>(other.numerator) * denominator;
    if (lhs == rhs) return 0;
    return (lhs < rhs) ? -1 : 1;
}

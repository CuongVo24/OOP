#ifndef LIB_H
#define LIB_H

#include <iostream>
#include <stdexcept>
using namespace std;

class Array {
private:
    int* data;
    int size;

public:
    // Constructors & Destructor
    Array(int n = 0);
    Array(const Array& other);
    ~Array();

    // Assignment Operators
    Array& operator=(const Array& other);
    Array& operator+=(const Array& other);

    // Concatenation (+)
    Array operator+(const Array& other) const;

    // Index Operator []
    int& operator[](int index);
    const int& operator[](int index) const;

    // Type-cast Operator
    operator int*() const;

    // Input / Output
    friend ostream& operator<<(ostream& os, const Array& arr);
    friend istream& operator>>(istream& is, Array& arr);

    // Getter
    int getSize() const;
};

#endif

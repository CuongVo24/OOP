#include "lib.h"

// Constructors & Destructor
Array::Array(int n) {
    if (n < 0) throw invalid_argument("Size cannot be negative");
    size = n;
    data = (size > 0) ? new int[size]() : nullptr;
}

Array::Array(const Array& other) {
    size = other.size;
    if (size > 0) {
        data = new int[size];
        for (int i = 0; i < size; ++i)
            data[i] = other.data[i];
    } else {
        data = nullptr;
    }
}

Array::~Array() {
    delete[] data;
}


// Assignment Operators
Array& Array::operator=(const Array& other) {
    if (this != &other) {
        delete[] data;
        size = other.size;
        if (size > 0) {
            data = new int[size];
            for (int i = 0; i < size; ++i)
                data[i] = other.data[i];
        } else {
            data = nullptr;
        }
    }
    return *this;
}

Array& Array::operator+=(const Array& other) {
    int newSize = size + other.size;
    int* newData = new int[newSize];

    for (int i = 0; i < size; ++i)
        newData[i] = data[i];
    for (int i = 0; i < other.size; ++i)
        newData[size + i] = other.data[i];

    delete[] data;
    data = newData;
    size = newSize;
    return *this;
}

// Concatenation (+)
Array Array::operator+(const Array& other) const {
    Array result(size + other.size);
    for (int i = 0; i < size; ++i)
        result.data[i] = data[i];
    for (int i = 0; i < other.size; ++i)
        result.data[size + i] = other.data[i];
    return result;
}

// Index Operator []
int& Array::operator[](int index) {
    if (index < 0 || index >= size)
        throw out_of_range("Index out of range");
    return data[index];
}

const int& Array::operator[](int index) const {
    if (index < 0 || index >= size)
        throw out_of_range("Index out of range");
    return data[index];
}

// Type-cast Operator
Array::operator int*() const {
    return data;
}

// Input / Output
ostream& operator<<(ostream& os, const Array& arr) {
    os << "{";
    for (int i = 0; i < arr.size; ++i) {
        os << arr.data[i];
        if (i < arr.size - 1)
            os << ", ";
    }
    os << "}";
    return os;
}

istream& operator>>(istream& is, Array& arr) {
    cout << "Nhap so phan tu: ";
    is >> arr.size;
    delete[] arr.data;
    arr.data = new int[arr.size];
    cout << "Nhap " << arr.size << " phan tu: ";
    for (int i = 0; i < arr.size; ++i)
        is >> arr.data[i];
    return is;
}

// Getter
int Array::getSize() const {
    return size;
}

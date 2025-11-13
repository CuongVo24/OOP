#ifndef LIB_H
#define LIB_H

#include <iostream>
using namespace std;

class Array {
protected:
    int* data;
    int size;

public:
    // Constructors & destructor
    Array(int n = 0, const int* arr = nullptr);
    Array(const Array& other);
    Array& operator=(const Array& other);
    virtual ~Array();

    // Accessors
    int getSize() const;
    int getValue(int index) const;
    virtual void setValue(int index, int value);

    // Operator []
    virtual int& operator[](int index);

    // Display
    void display() const;
};

class HistogramArray : public Array {
protected:
    int* histogram;
    int histSize; // giá trị tối đa + 1 (ví dụ 0-4 -> 5)

public:
    HistogramArray(int n, const int* arr, int hSize);
    HistogramArray(const HistogramArray& other);
    HistogramArray& operator=(const HistogramArray& other);
    ~HistogramArray();

    void computeHist();
    void displayHist() const;

    // Ghi đè để tự động cập nhật histogram
    void setValue(int index, int value) override;
    int& operator[](int index) override;
};

#endif

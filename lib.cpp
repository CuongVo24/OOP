#include "lib.h"

// ================= Array =================
Array::Array(int n, const int* arr) : size(n) {
    data = (size > 0) ? new int[size] : nullptr;
    if (arr != nullptr) {
        for (int i = 0; i < size; ++i) data[i] = arr[i];
    } else {
        for (int i = 0; i < size; ++i) data[i] = 0;
    }
}

Array::Array(const Array& other) : size(other.size) {
    data = new int[size];
    for (int i = 0; i < size; ++i) data[i] = other.data[i];
}

Array& Array::operator=(const Array& other) {
    if (this != &other) {
        delete[] data;
        size = other.size;
        data = new int[size];
        for (int i = 0; i < size; ++i) data[i] = other.data[i];
    }
    return *this;
}

Array::~Array() {
    delete[] data;
}

int Array::getSize() const { return size; }

int Array::getValue(int index) const {
    if (index < 0 || index >= size) throw out_of_range("Index out of range");
    return data[index];
}

void Array::setValue(int index, int value) {
    if (index < 0 || index >= size) throw out_of_range("Index out of range");
    data[index] = value;
}

int& Array::operator[](int index) {
    if (index < 0 || index >= size) throw out_of_range("Index out of range");
    return data[index];
}

void Array::display() const {
    for (int i = 0; i < size; ++i)
        cout << data[i] << " ";
    cout << endl;
}

// ================= HistogramArray =================
HistogramArray::HistogramArray(int n, const int* arr, int hSize)
    : Array(n, arr), histSize(hSize)
{
    histogram = new int[histSize];
    for (int i = 0; i < histSize; ++i) histogram[i] = 0;
}

HistogramArray::HistogramArray(const HistogramArray& other)
    : Array(other), histSize(other.histSize)
{
    histogram = new int[histSize];
    for (int i = 0; i < histSize; ++i)
        histogram[i] = other.histogram[i];
}

HistogramArray& HistogramArray::operator=(const HistogramArray& other) {
    if (this != &other) {
        Array::operator=(other);
        delete[] histogram;
        histSize = other.histSize;
        histogram = new int[histSize];
        for (int i = 0; i < histSize; ++i)
            histogram[i] = other.histogram[i];
    }
    return *this;
}

HistogramArray::~HistogramArray() {
    delete[] histogram;
}

void HistogramArray::computeHist() {
    for (int i = 0; i < histSize; ++i) histogram[i] = 0;
    for (int i = 0; i < size; ++i) {
        int v = data[i];
        if (v >= 0 && v < histSize)
            histogram[v]++;
    }
}

void HistogramArray::displayHist() const {
    for (int i = 0; i < histSize; ++i) {
        cout << i << " -> " << histogram[i] << endl;
    }
}

// Override setValue để tự động cập nhật histogram
void HistogramArray::setValue(int index, int value) {
    if (index < 0 || index >= size) throw out_of_range("Index out of range");

    int oldVal = data[index];
    data[index] = value;

    // cập nhật histogram nếu trong phạm vi
    if (oldVal >= 0 && oldVal < histSize) histogram[oldVal]--;
    if (value >= 0 && value < histSize) histogram[value]++;
}

// Override operator[]
int& HistogramArray::operator[](int index) {
    if (index < 0 || index >= size) throw out_of_range("Index out of range");
    // Trả về tham chiếu có thể sửa — ta phải cập nhật lại histogram khi đổi
    return data[index];
}

#include "lib.h"

// --- Constructors & Destructor ---

Array::Array() {
    size = 0;
    data = nullptr;
}

Array::Array(int n) {
    if (n < 0) n = 0;
    size = n;
    data = new int[size];
    for (int i = 0; i < size; ++i)
        data[i] = 0;
}

Array::Array(const int arr[], int n) {
    size = n;
    data = new int[size];
    for (int i = 0; i < size; ++i)
        data[i] = arr[i];
}

Array::Array(const Array& other) {
    size = other.size;
    data = new int[size];
    for (int i = 0; i < size; ++i)
        data[i] = other.data[i];
}

Array::~Array() {
    delete[] data;  // tránh rò rỉ bộ nhớ
    data = nullptr;
}

// --- Input/Output ---

void Array::input() {
    cout << "Enter array size: ";
    cin >> size;
    delete[] data; // giải phóng nếu đã cấp phát
    data = new int[size];

    cout << "Enter " << size << " elements:\n";
    for (int i = 0; i < size; ++i)
        cin >> data[i];
}

void Array::output() const {
    cout << "Array elements: ";
    for (int i = 0; i < size; ++i)
        cout << data[i] << " ";
    cout << endl;
}

// --- Getter/Setter ---

int Array::getSize() const {
    return size;
}

void Array::setSize(int newSize) {
    if (newSize < 0) return;
    int* newData = new int[newSize];
    int minSize = (newSize < size) ? newSize : size;

    for (int i = 0; i < minSize; ++i)
        newData[i] = data[i];

    for (int i = minSize; i < newSize; ++i)
        newData[i] = 0;

    delete[] data;
    data = newData;
    size = newSize;
}

int Array::getElement(int index) const {
    if (index < 0 || index >= size) {
        cout << "Index out of range!\n";
        return -1;
    }
    return data[index];
}

void Array::setElement(int index, int value) {
    if (index < 0 || index >= size) {
        cout << "Index out of range!\n";
        return;
    }
    data[index] = value;
}

// --- Functional Methods ---

int Array::find(int value) const {
    for (int i = 0; i < size; ++i) {
        if (data[i] == value)
            return i;
    }
    return -1;
}

void Array::sort(bool ascending) {
    for (int i = 0; i < size - 1; ++i) {
        for (int j = i + 1; j < size; ++j) {
            if ((ascending && data[i] > data[j]) ||
                (!ascending && data[i] < data[j])) {
                int temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }
}

// --- Utility ---

void Array::copyFrom(const Array& other) {
    if (this == &other) return;
    delete[] data;
    size = other.size;
    data = new int[size];
    for (int i = 0; i < size; ++i)
        data[i] = other.data[i];
}

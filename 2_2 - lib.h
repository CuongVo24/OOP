#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
using namespace std;

class Array {
private:
    int* data;    // con trỏ mảng động
    int size;     // kích thước mảng

public:
    // --- Constructors & Destructor ---
    Array();                                 // mặc định, mảng rỗng
    Array(int n);                            // tạo mảng có n phần tử, gán 0
    Array(const int arr[], int n);           // tạo mảng từ mảng có sẵn
    Array(const Array& other);               // sao chép mảng khác
    ~Array();                                // hủy mảng, tránh memory leak

    // --- Input/Output ---
    void input();                            // nhập mảng từ bàn phím
    void output() const;                     // xuất mảng ra màn hình

    // --- Getter/Setter ---
    int getSize() const;                     // trả về kích thước mảng
    void setSize(int newSize);               // thay đổi kích thước mảng
    int getElement(int index) const;         // lấy giá trị tại vị trí index
    void setElement(int index, int value);   // gán giá trị cho phần tử

    // --- Functional Methods ---
    int find(int value) const;               // tìm phần tử, trả về chỉ số hoặc -1
    void sort(bool ascending = true);        // sắp xếp tăng/giảm

    // --- Utility ---
    void copyFrom(const Array& other);       // sao chép dữ liệu từ mảng khác
};

#endif

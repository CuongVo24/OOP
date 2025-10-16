#include "lib.h"

int main() {
    // --- Khởi tạo mặc định ---
    Array arr1;
    arr1.input();
    arr1.output();

    // --- Khởi tạo với kích thước ---
    Array arr2(5);
    arr2.setElement(0, 10);
    arr2.setElement(1, 20);
    arr2.output();

    // --- Khởi tạo từ mảng có sẵn ---
    int a[] = {4, 1, 9, 2};
    Array arr3(a, 4);
    arr3.output();

    // --- Sao chép từ mảng khác ---
    Array arr4(arr3);
    arr4.output();

    // --- Tìm kiếm ---
    int value = 9;
    int pos = arr3.find(value);
    if (pos != -1)
        cout << "Value " << value << " found at index " << pos << endl;
    else
        cout << "Value " << value << " not found\n";

    // --- Sắp xếp ---
    cout << "Sorting ascending...\n";
    arr3.sort(true);
    arr3.output();

    cout << "Sorting descending...\n";
    arr3.sort(false);
    arr3.output();

    // --- Kiểm tra copyFrom ---
    arr1.copyFrom(arr3);
    cout << "Copied arr3 into arr1:\n";
    arr1.output();

    return 0;
}

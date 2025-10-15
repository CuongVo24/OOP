#include "lib.h"
#include <iostream>
using namespace std;

int main() {
    cout << "=== Fraction Class Demo ===\n";

    // 1. Các cách khởi tạo
    Fraction a;              // mặc định = 0
    Fraction b(3, 4);        // 3/4
    Fraction c(2);           // 2/1
    Fraction d("5/10");      // từ chuỗi "5/10"
    Fraction e(b);           // copy constructor

    cout << "a = "; a.output(); cout << endl;
    cout << "b = "; b.output(); cout << endl;
    cout << "c = "; c.output(); cout << endl;
    cout << "d (reduced) = "; d.reduce().output(); cout << endl;
    cout << "e (copy of b) = "; e.output(); cout << endl;

    // 2. Cộng hai phân số
    Fraction sum = b.add(d);
    cout << "\nSum of b and d: ";
    sum.output();
    cout << endl;

    // 3. Phân số nghịch đảo
    cout << "Inverse of b: ";
    b.inverse().output();
    cout << endl;

    // 4. So sánh phân số
    int cmp = b.compare(d);
    cout << "\nCompare b and d: ";
    if (cmp == 0)
        cout << "b = d";
    else if (cmp < 0)
        cout << "b < d";
    else
        cout << "b > d";
    cout << endl;

    // 5. Nhập phân số từ bàn phím
    cout << "\nEnter a new fraction:\n";
    Fraction userFrac;
    userFrac.input();

    cout << "You entered: ";
    userFrac.output();
    cout << endl;

    // 6. Cộng thủ công với phân số khác
    Fraction result = userFrac.add(b);
    cout << "Result of (your fraction + b): ";
    result.output();
    cout << endl;

    return 0;
}

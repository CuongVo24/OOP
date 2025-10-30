#include "fraction.h"

int main() {
    Fraction a(1, 2), b(3, 4);

    cout << "a = " << a << ", b = " << b << endl;
    cout << "a + b = " << (a + b) << endl;
    cout << "a - b = " << (a - b) << endl;
    cout << "a * b = " << (a * b) << endl;
    cout << "a / b = " << (a / b) << endl;

    cout << "a > b ? " << (a > b) << endl;
    cout << "a == b ? " << (a == b) << endl;

    a += b;
    cout << "a += b => " << a << endl;

    ++a;
    cout << "++a => " << a << endl;

    Fraction c = a++;
    cout << "a++ => c = " << c << ", a = " << a << endl;

    cout << "(float)a = " << (float)a << endl;
    cout << "(int)b = " << (int)b << endl;

    Fraction d;
    cout << "Nhap phan so d (vd: 7/8 hoac 3): ";
    cin >> d;
    cout << "d = " << d << endl;

    return 0;
}

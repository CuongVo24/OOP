#include "lib.h"

int main() {
    Array a(3), b(2);
    a[0] = 1; a[1] = 2; a[2] = 3;
    b[0] = 4; b[1] = 5;

    cout << "a = " << a << endl;
    cout << "b = " << b << endl;

    Array c = a + b;
    cout << "a + b = " << c << endl;

    a += b;
    cout << "a += b -> " << a << endl;

    cout << "a[2] = " << a[2] << endl;

    int* p = (int*)a;
    cout << "p[0] = " << p[0] << endl;

    Array d;
    cin >> d;
    cout << "d = " << d << endl;

    return 0;
}

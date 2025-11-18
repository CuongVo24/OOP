#include <iostream>

using namespace std;

#define NOE 10

class A {
private:
    int info[NOE];

public:
    typedef int* indexOf;

    A() {
        for (int i = 0; i < NOE; ++i) {
            info[i] = i + NOE;
        }
    }

    indexOf GetInfo(const int& i) {
        return info + i;
    }

    void Xuat(ostream& os) {
        for (int i = 0; i < NOE; ++i) {
            os << info[i] << ", ";
        }
        os << endl;
    }
};

int main() {
    const int i = 7;
    A a;
    
    A::indexOf io = a.GetInfo(i);
    
    cout << *io << endl; // 
    
    *io = 10;
    
    a.Xuat(cout);
    
    system("pause");
    return 0;
}

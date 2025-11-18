#include <iostream>
#include <algorithm> 
#include <cmath> 
#include <iomanip>


using namespace std;

class PhanSo {
private:
    int tu;
    int mau;

    static int UCLN(int a, int b) {
        a = abs(a);
        b = abs(b);
        while (b != 0) {
            int temp = a % b;
            a = b;
            b = temp;
        }
        return a;
    }


    void rutGon() {

        if (mau == 0) {
            cout << "LOI: Mau so bang 0! Dat ve 0/1." << endl;
            tu = 0;
            mau = 1;
            return;
        }
  
        if (tu == 0) {
            mau = 1;
            return;
        }

        if (mau < 0) {
            tu = -tu;
            mau = -mau;
        }

        int uoc = UCLN(tu, mau);
        tu /= uoc;
        mau /= uoc;
    }

public:

    PhanSo() : tu(0), mau(1) {}

    PhanSo(int t, int m) : tu(t), mau(m) {
        rutGon();
    }



    friend ostream& operator<<(ostream& os, const PhanSo& p) {
        os << p.tu << "/" << p.mau;
        return os;
    }

    friend istream& operator>>(istream& is, PhanSo& p) {
        cout << "  Nhap tu: ";
        is >> p.tu;
        cout << "  Nhap mau: ";
        is >> p.mau;
        
        p.rutGon();
        return is;
    }


    bool operator<(const PhanSo& other) const {
        return (long long)this->tu * other.mau < (long long)other.tu * this->mau;
    }


    PhanSo operator+(const PhanSo& other) const {
        PhanSo tong;
        tong.tu = this->tu * other.mau + other.tu * this->mau;
        tong.mau = this->mau * other.mau;
        tong.rutGon();
        return tong;
    }
};


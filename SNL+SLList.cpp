#include <iostream>
#include <cstring>
#include <cstdio>
#include "SLList.h" 

using namespace std;

class SoNguyenLon {
private:
    SLList<int> chuSo;

public:
    SoNguyenLon() {}

    SoNguyenLon(const char* s) {
        int len = strlen(s);
        for (int i = len - 1; i >= 0; i--) {
            if (s[i] >= '0' && s[i] <= '9') {
                chuSo.push_back(s[i] - '0');
            }
        }
        if (chuSo.empty()) chuSo.push_back(0);
    }

    SoNguyenLon(long long n) {
        if (n == 0) chuSo.push_back(0);
        while (n > 0) {
            chuSo.push_back(n % 10);
            n /= 10;
        }
    }


    friend ostream& operator<<(ostream& os, SoNguyenLon& snl) {
        int n = snl.chuSo.size();
        if (n == 0) {
            os << "0";
            return os;
        }

        int* temp = new int[n];
        int i = 0;
        
        for (SLList<int>::Iterator it = snl.chuSo.begin(); it != snl.chuSo.end(); ++it) {
            temp[i++] = *it;
        }

        for (int j = n - 1; j >= 0; j--) {
            os << temp[j];
        }

        delete[] temp;
        
        return os;
    }

    friend istream& operator>>(istream& is, SoNguyenLon& snl) {
        char buffer[1000];
        is >> buffer;
        snl = SoNguyenLon(buffer);
        return is;
    }

    SoNguyenLon operator+(SoNguyenLon& other) {
        SoNguyenLon ketQua;
        
        SLList<int>::Iterator it1 = this->chuSo.begin();
        SLList<int>::Iterator it2 = other.chuSo.begin();
        SLList<int>::Iterator end1 = this->chuSo.end();
        SLList<int>::Iterator end2 = other.chuSo.end();

        int nho = 0;
        while (it1 != end1 || it2 != end2) {
            int val1 = (it1 != end1) ? *it1 : 0;
            int val2 = (it2 != end2) ? *it2 : 0;
            
            int tong = val1 + val2 + nho;
            ketQua.chuSo.push_back(tong % 10);
            nho = tong / 10;

            if (it1 != end1) ++it1;
            if (it2 != end2) ++it2;
        }
        if (nho > 0) ketQua.chuSo.push_back(nho);
        
        return ketQua;
    }


    bool operator<(SoNguyenLon& other) {
        int n1 = this->chuSo.size();
        int n2 = other.chuSo.size();
        
        if (n1 != n2) return n1 < n2;

        int* arr1 = new int[n1];
        int* arr2 = new int[n2];
        
        int i = 0;
        for (SLList<int>::Iterator it = this->chuSo.begin(); it != this->chuSo.end(); ++it) arr1[i++] = *it;
        
        i = 0;
        for (SLList<int>::Iterator it = other.chuSo.begin(); it != other.chuSo.end(); ++it) arr2[i++] = *it;

        bool result = false;
        for (int j = n1 - 1; j >= 0; j--) {
            if (arr1[j] != arr2[j]) {
                result = arr1[j] < arr2[j];
                goto CleanUp;
            }
        }

    CleanUp:
        delete[] arr1;
        delete[] arr2;
        return result;
    }
};

int main() {
    // Test Constructor char*
    SoNguyenLon a("123");
    SoNguyenLon b("9");
    
    cout << "A: " << a << endl; // Xuất: 123
    cout << "B: " << b << endl; // Xuất: 9

    // Test Cộng
    SoNguyenLon c = a + b;
    cout << "A + B = " << c << endl; // Xuất: 132

    // Test Nhập
    /*
    SoNguyenLon d;
    cout << "Nhap so D: ";
    cin >> d;
    cout << "Ban vua nhap: " << d << endl;
    */

    // Test So sánh
    if (b < a) cout << "B nho hon A" << endl;
    else cout << "B khong nho hon A" << endl;

    return 0;
}

#include <iostream>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <cstdlib>


using namespace std;


class SoNguyenLon {
private:
    int* chuSo;
    int soLuong;

    void xoaSoKhong() {
        while (soLuong > 1 && chuSo[soLuong - 1] == 0) {
            soLuong--;
        }
    }
    

    void resize(int newSize) {
        int* temp = new int[newSize];
        memset(temp, 0, newSize * sizeof(int));
        int n = (soLuong < newSize) ? soLuong : newSize;
        memcpy(temp, chuSo, n * sizeof(int));
        delete[] chuSo;
        chuSo = temp;
        soLuong = newSize;
    }

public:

    SoNguyenLon() {
        soLuong = 1;
        chuSo = new int[1];
        chuSo[0] = 0;
    }

    SoNguyenLon(const char* s) {
        soLuong = strlen(s);
        if (soLuong == 0) {
            soLuong = 1;
            chuSo = new int[1];
            chuSo[0] = 0;
        } else {
            chuSo = new int[soLuong];
            // Lưu ngược: "123" -> {3, 2, 1}
            for (int i = 0; i < soLuong; i++) {
                chuSo[i] = s[soLuong - 1 - i] - '0';
            }
            xoaSoKhong();
        }
    }


    SoNguyenLon(const SoNguyenLon& other) {
        this->soLuong = other.soLuong;
        this->chuSo = new int[this->soLuong];
        memcpy(this->chuSo, other.chuSo, this->soLuong * sizeof(int));
    }

    ~SoNguyenLon() {
        delete[] chuSo;
    }

    SoNguyenLon& operator=(const SoNguyenLon& other) {
        if (this == &other) {
            return *this;
        }

        delete[] this->chuSo;

        this->soLuong = other.soLuong;
        this->chuSo = new int[this->soLuong];
        memcpy(this->chuSo, other.chuSo, this->soLuong * sizeof(int));

        return *this;
    }

    friend ostream& operator<<(ostream& os, const SoNguyenLon& snl) {
        for (int i = snl.soLuong - 1; i >= 0; i--) {
            os << snl.chuSo[i];
        }
        return os;
    }

friend istream& operator>>(istream& is, SoNguyenLon& snl) {
        if (snl.chuSo != nullptr) {
            delete[] snl.chuSo;
            snl.chuSo = nullptr;
        }

        int capacity = 16;
        int len = 0;
        char* buffer = new char[capacity];
        char ch;


        while (is.get(ch) && (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r'));
        if (ch >= '0' && ch <= '9') {
            buffer[len++] = ch;
        } else {
            delete[] buffer;
            snl.soLuong = 1;
            snl.chuSo = new int[1];
            snl.chuSo[0] = 0;
            return is;
        }

        while (is.get(ch)) {
            if (ch >= '0' && ch <= '9') {
                if (len >= capacity) {
                    int newCapacity = capacity * 2;
                    char* newBuffer = new char[newCapacity];
                    memcpy(newBuffer, buffer, len);
                    delete[] buffer;
                    buffer = newBuffer;
                    capacity = newCapacity;
                }
                buffer[len++] = ch;
            } else {
                is.putback(ch);
                break;
            }
        }

        snl.soLuong = len;
        snl.chuSo = new int[snl.soLuong];
        for (int i = 0; i < len; i++) {
            snl.chuSo[i] = buffer[len - 1 - i] - '0'; // Trừ '0' để ra số nguyên
        }

        delete[] buffer;
        snl.xoaSoKhong(); 
        return is;
    }

    bool operator<(const SoNguyenLon& other) const {
        if (this->soLuong != other.soLuong) {
            return this->soLuong < other.soLuong;
        }
        for (int i = this->soLuong - 1; i >= 0; i--) {
            if (this->chuSo[i] != other.chuSo[i]) {
                return this->chuSo[i] < other.chuSo[i];
            }
        }
        return false;
    }

    SoNguyenLon operator+(const SoNguyenLon& other) const {
        int maxLen = max(this->soLuong, other.soLuong);
        SoNguyenLon tong;
        tong.resize(maxLen + 1);
        
        int nho = 0;
        for (int i = 0; i < maxLen; i++) {
            int a = (i < this->soLuong) ? this->chuSo[i] : 0;
            int b = (i < other.soLuong) ? other.chuSo[i] : 0;
            
            int s = a + b + nho;
            tong.chuSo[i] = s % 10;
            nho = s / 10;
        }
        
        tong.chuSo[maxLen] = nho;
        tong.xoaSoKhong();
        
        return tong;
    }

    SoNguyenLon operator+=(const SoNguyenLon& other) const {
        *this = *this + other;
        return *this;
    }
};


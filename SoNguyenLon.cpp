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
    int dau;

    void trim() {
        while (soLuong > 1 && chuSo[soLuong - 1] == 0) {
            soLuong--;
        }
    }

    int compareAbs(const SoNguyenLon& other) const {
        if (soLuong != other.soLuong) {
            return (soLuong > other.soLuong) ? 1 : -1;
        }
        for (int i = soLuong - 1; i >= 0; i--) {
            if (chuSo[i] > other.chuSo[i]) return 1;
            if (chuSo[i] < other.chuSo[i]) return -1;
        }
        return 0;
    }

    SoNguyenLon addAbs(const SoNguyenLon& other) const {
        SoNguyenLon tong; // Mặc định là 0
        delete[] tong.chuSo;

        int maxLen = max(soLuong, other.soLuong);
        tong.chuSo = new int[maxLen + 1];
        tong.soLuong = maxLen;
        
        int nho = 0;
        for (int i = 0; i < maxLen; i++) {
            int a = (i < soLuong) ? chuSo[i] : 0;
            int b = (i < other.soLuong) ? other.chuSo[i] : 0;
            int s = a + b + nho;
            
            tong.chuSo[i] = s % 10;
            nho = s / 10;
        }

        if (nho > 0) {
            tong.chuSo[maxLen] = nho;
            tong.soLuong = maxLen + 1;
        } else {
            tong.soLuong = maxLen;
        }
        
        return tong;
    }

    SoNguyenLon subtractAbs(const SoNguyenLon& be) const {
        SoNguyenLon hieu;
        delete[] hieu.chuSo;

        int maxLen = soLuong;
        hieu.chuSo = new int[maxLen];
        hieu.soLuong = maxLen;

        int muon = 0;
        for (int i = 0; i < maxLen; i++) {
            int a = chuSo[i];
            int b = (i < be.soLuong) ? be.chuSo[i] : 0;
            
            int s = a - b - muon;
            if (s < 0) {
                s += 10;
                muon = 1;
            } else {
                muon = 0;
            }
            hieu.chuSo[i] = s;
        }

        hieu.trim();
        return hieu;
    }

public:

    SoNguyenLon() {
        soLuong = 1;
        chuSo = new int[1];
        chuSo[0] = 0;
        dau = 1;
    }

    SoNguyenLon(const char* s) {
        int i = 0;
        dau = 1;
        
        if (s[0] == '-') {
            dau = -1;
            i = 1;
        } else if (s[0] == '+') {
            i = 1;
        }

        int len = strlen(s);
        if (len == i) {
            soLuong = 1;
            chuSo = new int[1];
            chuSo[0] = 0;
            dau = 1;
            return;
        }

        soLuong = len - i;
        chuSo = new int[soLuong];
        
        // Lưu ngược: "-123" -> 3, 2, 1
        for (int j = 0; j < soLuong; j++) {
            chuSo[j] = s[len - 1 - j] - '0';
        }

        trim();
        
        if (soLuong == 1 && chuSo[0] == 0) dau = 1;
    }


    SoNguyenLon(long long n) {
        if (n < 0) {
            dau = -1;
            n = -n;
        } else {
            dau = 1;
        }

        if (n == 0) {
            soLuong = 1;
            chuSo = new int[1];
            chuSo[0] = 0;
        } else {
            int temp[20]; 
            int count = 0;
            while (n > 0) {
                temp[count++] = n % 10;
                n /= 10;
            }
            
            soLuong = count;
            chuSo = new int[soLuong];
            for(int i=0; i<soLuong; i++) chuSo[i] = temp[i];
        }
    }



    SoNguyenLon(const SoNguyenLon& other) {
        this->soLuong = other.soLuong;
        this->dau = other.dau;
        this->chuSo = new int[this->soLuong];
        memcpy(this->chuSo, other.chuSo, this->soLuong * sizeof(int));
    }

    ~SoNguyenLon() {
        if (chuSo) delete[] chuSo;
    }

    SoNguyenLon& operator=(const SoNguyenLon& other) {
        if (this == &other) return *this;

        delete[] this->chuSo;

        this->soLuong = other.soLuong;
        this->dau = other.dau;
        this->chuSo = new int[this->soLuong];
        memcpy(this->chuSo, other.chuSo, this->soLuong * sizeof(int));

        return *this;
    }



    friend ostream& operator<<(ostream& os, const SoNguyenLon& snl) {
        if (snl.dau == -1) os << "-";
        for (int i = snl.soLuong - 1; i >= 0; i--) {
            os << snl.chuSo[i];
        }
        return os;
    }

    friend istream& operator>>(istream& is, SoNguyenLon& snl) {

        char buffer[10000]; 
        is >> buffer;
        snl = SoNguyenLon(buffer);
        return is;
    }


    bool operator<(const SoNguyenLon& other) const {
        if (dau != other.dau) return dau < other.dau;
        
        int absCmp = compareAbs(other);
        
        if (dau == 1) return absCmp < 0;
        else return absCmp > 0;
    }
    
    bool operator>(const SoNguyenLon& other) const { return other < *this; }
    bool operator==(const SoNguyenLon& other) const {
        return dau == other.dau && compareAbs(other) == 0;
    }
    bool operator!=(const SoNguyenLon& other) const { return !(*this == other); }


    SoNguyenLon operator+(const SoNguyenLon& other) const {
        SoNguyenLon kq;
        
        if (this->dau == other.dau) {
            kq = addAbs(other);
            kq.dau = this->dau;
        } else {
            int cmp = compareAbs(other);
            if (cmp >= 0) {
                kq = subtractAbs(other);
                kq.dau = this->dau;
            } else {
                kq = other.subtractAbs(*this);
                kq.dau = other.dau;
            }
        }

        if (kq.soLuong == 1 && kq.chuSo[0] == 0) kq.dau = 1;
        
        return kq;
    }

    SoNguyenLon operator-(const SoNguyenLon& other) const {
        SoNguyenLon soDoi = other;
        soDoi.dau = -soDoi.dau;
        return *this + soDoi;
    }
    
    SoNguyenLon& operator+=(const SoNguyenLon& other) {
        *this = *this + other;
        return *this;
    }
    
    SoNguyenLon& operator-=(const SoNguyenLon& other) {
        *this = *this - other;
        return *this;
    }
};


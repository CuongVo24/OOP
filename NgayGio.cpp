#include <iostream>
#include <algorithm>
#include <cmath>
#include <iomanip>

using namespace std;

class NgayGio {
private:
    int ngay, thang, nam, gio;

    static bool laNamNhuan(int n) {
        return (n % 400 == 0) || (n % 4 == 0 && n % 100 != 0);
    }

    static int soNgayTrongThang(int m, int n) {
        if (m == 2) {
            return laNamNhuan(n) ? 29 : 28;
        } else if (m == 4 || m == 6 || m == 9 || m == 11) {
            return 30;
        }
        return 31;
    }


    void chuanHoa() {
        if (nam <= 0) nam = abs(nam);
        if (thang <= 0) thang = abs(thang);
        if (ngay <= 0) ngay = abs(ngay);
        if (gio < 0) gio = abs(gio);

        ngay += gio / 24;
        gio = gio % 24;

        if (thang > 12) {
            nam += (thang - 1) / 12;
            thang = (thang - 1) % 12 + 1;
        }

        while (ngay > soNgayTrongThang(thang, nam)) {
            ngay -= soNgayTrongThang(thang, nam);
            thang++;
            if (thang > 12) {
                thang = 1;
                nam++;
            }
        }
    }

public:
    NgayGio() : ngay(1), thang(1), nam(1), gio(0) {}
    
    NgayGio(int d, int h, int y) : ngay(d), thang(1), nam(y), gio(h) {
        chuanHoa();
    }
    
    NgayGio(int d, int h) : ngay(d), thang(1), nam(1), gio(h) {
        chuanHoa();
    }

    NgayGio(int h) : ngay(1), thang(1), nam(1), gio(h) {
        chuanHoa();
    }
    
    NgayGio(int d, int m, int y, int h) : ngay(d), thang(m), nam(y), gio(h) {
        chuanHoa();
    }
    
    int getThang() const { return thang; }
    int getNam() const { return nam; }

    friend ostream& operator<<(ostream& os, const NgayGio& ng) {
        // "xuất ra định dạng x/x-x"
        os << ng.ngay << "/" << ng.thang << "/" << ng.nam << "-" 
           << setfill('0') << setw(2) << ng.gio << "h";
        return os;
    }

    friend istream& operator>>(istream& is, NgayGio& ng) {
        is >> ng.ngay >> ng.thang >> ng.nam >> ng.gio;
        ng.chuanHoa();
        return is;
    }
    

    bool operator<(const NgayGio& other) const {
        if (nam != other.nam) return nam < other.nam;
        if (thang != other.thang) return thang < other.thang;
        if (ngay != other.ngay) return ngay < other.ngay;
        return gio < other.gio;
    }
    
    bool operator>(const NgayGio& other) const {
        return other < *this;
    }


    NgayGio operator+(int gioThem) const {
        NgayGio temp = *this;
        temp.gio += gioThem;
        temp.chuanHoa();
        return temp;
    }

    friend NgayGio operator+(int gioThem, const NgayGio& ng) {
        return ng + gioThem;
    }

    NgayGio& operator++() {
        this->gio++;
        this->chuanHoa();
        return *this;
    }

    NgayGio operator++(int) {
        NgayGio temp = *this;
        this->gio++;
        this->chuanHoa();
        return temp;
    }
};


bool funcFindIf(const NgayGio& ng) {
    return ng.getThang() > 11;
}

#include <iostream>
using namespace std;

class Ngay {
private:
    int d, m, y; // ngày, tháng, năm

    // Kiểm tra năm nhuận
    bool isLeap(int year) const {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    // Số ngày trong tháng
    int daysInMonth(int month, int year) const {
        static int days[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
        if (month == 2 && isLeap(year)) return 29;
        return days[month];
    }

    // Chuyển ngày/tháng/năm → số ngày tuyệt đối (từ mốc 1/1/1)
    long toDays() const {
        long total = d;
        for (int i = 1; i < m; i++)
            total += daysInMonth(i, y);
        for (int i = 1; i < y; i++)
            total += isLeap(i) ? 366 : 365;
        return total;
    }

    // Chuyển từ số ngày tuyệt đối → ngày/tháng/năm
    void fromDays(long total) {
        y = 1;
        while (true) {
            int daysInYear = isLeap(y) ? 366 : 365;
            if (total > daysInYear) {
                total -= daysInYear;
                y++;
            } else break;
        }

        m = 1;
        while (true) {
            int dm = daysInMonth(m, y);
            if (total > dm) {
                total -= dm;
                m++;
            } else break;
        }

        d = total;
    }

public:
    // Constructors
    Ngay(int day = 1, int month = 1, int year = 1)
        : d(day), m(month), y(year) {}

    // Toán tử cộng số ngày
    Ngay operator+(long n) const {
        Ngay tmp = *this;
        tmp.fromDays(tmp.toDays() + n);
        return tmp;
    }

    // Toán tử trừ số ngày
    Ngay operator-(long n) const {
        Ngay tmp = *this;
        tmp.fromDays(tmp.toDays() - n);
        return tmp;
    }

    // Trừ hai ngày → số ngày chênh lệch
    long operator-(const Ngay& other) const {
        return this->toDays() - other.toDays();
    }

    // Cộng hai ngày
    Ngay operator+(const Ngay& other) const {
        long total = this->toDays() + other.toDays();
        Ngay tmp;
        tmp.fromDays(total);
        return tmp;
    }

    // Toán tử so sánh
    bool operator>(const Ngay& other) const {
        return this->toDays() > other.toDays();
    }

    // Toán tử xuất
    friend ostream& operator<<(ostream& os, const Ngay& n) {
        os << n.d << "/" << n.m << "/" << n.y;
        return os;
    }
};

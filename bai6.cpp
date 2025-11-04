#include <iostream>
#include <iomanip>
using namespace std;

class ThoiGian {
private:
    int m_totalSeconds; // Tổng số giây kể từ 00:00:00

    void normalize() {
        const int SECONDS_IN_DAY = 24 * 3600;
        m_totalSeconds %= SECONDS_IN_DAY;
        if (m_totalSeconds < 0)
            m_totalSeconds += SECONDS_IN_DAY;
    }

public:
    // Các hàm dựng
    ThoiGian() : m_totalSeconds(0) {}

    ThoiGian(int totalSeconds) : m_totalSeconds(totalSeconds) {
        normalize();
    }

    ThoiGian(int minutes, int seconds)
        : m_totalSeconds(minutes * 60 + seconds) {
        normalize();
    }

    ThoiGian(int hours, int minutes, int seconds)
        : m_totalSeconds(hours * 3600 + minutes * 60 + seconds) {
        normalize();
    }

    // Toán tử cộng
    ThoiGian operator+(const ThoiGian& other) const {
        return ThoiGian(m_totalSeconds + other.m_totalSeconds);
    }

    ThoiGian operator+(int seconds) const {
        return ThoiGian(m_totalSeconds + seconds);
    }

    friend ThoiGian operator+(int seconds, const ThoiGian& t) {
        return ThoiGian(t.m_totalSeconds + seconds);
    }

    // Toán tử trừ
    ThoiGian operator-(const ThoiGian& other) const {
        return ThoiGian(m_totalSeconds - other.m_totalSeconds);
    }

    ThoiGian operator-(int seconds) const {
        return ThoiGian(m_totalSeconds - seconds);
    }

    friend ThoiGian operator-(int seconds, const ThoiGian& t) {
        return ThoiGian(seconds - t.m_totalSeconds);
    }

    // Toán tử so sánh giữa 2 ThoiGian
    bool operator==(const ThoiGian& other) const {
        return m_totalSeconds == other.m_totalSeconds;
    }

    bool operator!=(const ThoiGian& other) const {
        return m_totalSeconds != other.m_totalSeconds;
    }

    bool operator<(const ThoiGian& other) const {
        return m_totalSeconds < other.m_totalSeconds;
    }

    bool operator<=(const ThoiGian& other) const {
        return m_totalSeconds <= other.m_totalSeconds;
    }

    bool operator>(const ThoiGian& other) const {
        return m_totalSeconds > other.m_totalSeconds;
    }

    bool operator>=(const ThoiGian& other) const {
        return m_totalSeconds >= other.m_totalSeconds;
    }

    // So sánh giữa int và ThoiGian
    friend bool operator<=(int seconds, const ThoiGian& t) {
        return seconds <= t.m_totalSeconds;
    }

    // Xuất dạng hh:mm:ss
    friend ostream& operator<<(ostream& os, const ThoiGian& t) {
        int hours = t.m_totalSeconds / 3600;
        int minutes = (t.m_totalSeconds % 3600) / 60;
        int seconds = t.m_totalSeconds % 60;
        os << setfill('0') << setw(2) << hours << ":"
            << setw(2) << minutes << ":" << setw(2) << seconds;
        return os;
    }
};


int main() {
    ThoiGian tg1;  
    ThoiGian tg2(1212);
    ThoiGian tg3(125, 45);
    ThoiGian tg4(12, 239, -78);
    ThoiGian tg5 = tg2 + tg3;
    ThoiGian tg6 = 5000 + tg2;
    ThoiGian tg7 = tg4 - tg6;
    ThoiGian tg8 = 12300 - tg4;
    ThoiGian tg9, tg10;

    if (tg8 <= tg3) {
        tg9 = tg1 + tg2 + 36000;
    }
    if (12345 <= tg5) {
        tg10 = tg5 + 12345;
    }

    cout << tg1 << endl
        << tg2 << endl
        << tg3 << endl
        << tg4 << endl
        << tg5 << endl
        << tg6 << endl
        << tg7 << endl
        << tg8 << endl
        << tg9 << endl
        << tg10 << endl;

    return 0;
}

#include <iostream>
using namespace std;

class Time {
private:
    int h, m, s;
    void normalize() {
        m += s / 60; s %= 60;
        h += m / 60; m %= 60;
        h %= 24;
    }

public:
    Time(int hh=0, int mm=0, int ss=0) : h(hh), m(mm), s(ss) { normalize(); }

    void addSeconds(int x) { s += x; normalize(); }

    bool isEarlier(const Time &other) const {
        if (h != other.h) return h < other.h;
        if (m != other.m) return m < other.m;
        return s < other.s;
    }

    void display() const {
        cout << (h<10?"0":"") << h << ":" 
             << (m<10?"0":"") << m << ":" 
             << (s<10?"0":"") << s << endl;
    }
};

int main() {
    Time t(23, 59, 50);
    t.addSeconds(15);
    t.display();
}

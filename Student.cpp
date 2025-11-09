#include <iostream>
#include <string>
using namespace std;

class Student {
private:
    string name, id;
    double math, eng, cs;

public:
    Student(string n, string i, double m, double e, double c)
        : name(n), id(i), math(m), eng(e), cs(c) {}

    double average() const { return (math + eng + cs) / 3; }

    string rank() const {
        double avg = average();
        if (avg >= 8) return "Gioi";
        if (avg >= 6.5) return "Kha";
        return "Trung binh";
    }

    void display() const {
        cout << name << " - " << id << " - TB: " << average()
             << " - Xep loai: " << rank() << endl;
    }
};

int main() {
    Student s("Cuong Vo", "SV01", 8.5, 7.8, 9.0);
    s.display();
}

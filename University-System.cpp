#include <iostream>
#include <string>
using namespace std;

// -------------------- CLASS COURSE --------------------
class Course {
private:
    string name;
public:
    Course(string n = "") : name(n) {}
    string getName() const { return name; }
};

// -------------------- CLASS STUDENT --------------------
class Student {
private:
    string name;
    Course** courses;   // mảng con trỏ đến Course
    int count;          // số khóa học hiện có
    int capacity;       // số lượng tối đa hiện tại

public:
    // Constructor
    Student(string n, int cap = 5)
        : name(n), count(0), capacity(cap)
    {
        courses = new Course*[capacity];
    }

    // Destructor — giải phóng mảng con trỏ
    ~Student() {
        delete[] courses;
    }

    // Thêm khóa học (nếu đầy thì tự động mở rộng)
    void enroll(Course& c) {
        if (count == capacity) {
            capacity *= 2;
            Course** newArr = new Course*[capacity];
            for (int i = 0; i < count; ++i)
                newArr[i] = courses[i];
            delete[] courses;
            courses = newArr;
        }
        courses[count++] = &c;
    }

    // Hiển thị danh sách khóa học
    void showCourses() const {
        cout << name << " enrolled: ";
        for (int i = 0; i < count; ++i) {
            cout << courses[i]->getName();
            if (i < count - 1) cout << ", ";
        }
        cout << endl;
    }
};

// -------------------- MAIN --------------------
int main() {
    Course c1("Math"), c2("CS"), c3("OOP");
    Student s("Cuong");

    s.enroll(c1);
    s.enroll(c2);
    s.enroll(c3);

    s.showCourses();

    return 0;
}

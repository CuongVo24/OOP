#include <iostream>
#include <string>
using namespace std;

// ---------------- PERSON ----------------
class Person {
protected:
    string name;
public:
    Person(string n) : name(n) {}
    virtual ~Person() {} // destructor ảo (rất quan trọng khi dùng con trỏ)
    virtual void display() const {
        cout << "Person: " << name << endl;
    }
};

// ---------------- EMPLOYEE ----------------
class Employee : public Person {
protected:
    double salary;
public:
    Employee(string n, double s) : Person(n), salary(s) {}
    double getSalary() const { return salary; }
    void display() const override {
        cout << "Employee: " << name << ", Salary: " << salary << endl;
    }
};

// ---------------- MANAGER ----------------
class Manager : public Employee {
private:
    Employee** team;   // mảng con trỏ đến Employee
    int teamSize;      // số thành viên hiện tại
    int capacity;      // số lượng tối đa

public:
    Manager(string n, double s, int cap = 5)
        : Employee(n, s), teamSize(0), capacity(cap)
    {
        team = new Employee*[capacity]; // cấp phát mảng con trỏ
    }

    ~Manager() override {
        delete[] team; // chỉ xóa mảng con trỏ, không xóa từng Employee
    }

    void addMember(Employee* e) {
        if (teamSize < capacity) {
            team[teamSize++] = e;
        } else {
            cout << "Team is full, cannot add more members.\n";
        }
    }

    void display() const override {
        cout << "Manager: " << name << ", Salary: " << salary << endl;
        cout << "Team members:\n";
        for (int i = 0; i < teamSize; ++i) {
            cout << "  - ";
            team[i]->display();
        }
    }
};

// ---------------- MAIN ----------------
int main() {
    Employee* e1 = new Employee("An", 700);
    Employee* e2 = new Employee("Binh", 800);
    Manager* m = new Manager("Cuong", 1500, 3);

    m->addMember(e1);
    m->addMember(e2);

    m->display();

    // Giải phóng bộ nhớ
    delete e1;
    delete e2;
    delete m;

    return 0;
}

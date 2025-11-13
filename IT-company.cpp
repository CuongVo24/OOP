#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

// ====================== Lớp cơ sở trừu tượng ======================
class Employee {
protected:
    string id, name;
    double baseSalary, coefSalary;
    string startDate;
public:
    Employee(string id, string name, double base, double coef, string start)
        : id(id), name(name), baseSalary(base), coefSalary(coef), startDate(start) {}
    virtual ~Employee() {}

    virtual double calcSalary() const = 0;
    virtual void display() const = 0;
};

// ====================== Lớp Manager ======================
class Manager : public Employee {
private:
    vector<Employee*> subordinates;
public:
    Manager(string id, string name, double base, double coef, string start)
        : Employee(id, name, base, coef, start) {}

    void addSubordinate(Employee* e) {
        subordinates.push_back(e);
    }

    double calcSalary() const override {
        return baseSalary * coefSalary;
    }

    int countAllSubordinates() const {
        int total = 0;
        for (auto e : subordinates) {
            total++;
            Manager* m = dynamic_cast<Manager*>(e);
            if (m)
                total += m->countAllSubordinates();
        }
        return total;
    }

    void display() const override {
        cout << "[Manager] " << name << " | ID: " << id
             << " | Salary: " << calcSalary()
             << " | Subordinates: " << subordinates.size() << endl;
    }
};

// ====================== Lớp Programmer ======================
class Programmer : public Employee {
    double overtime;
public:
    Programmer(string id, string name, double base, double coef, string start, double ot)
        : Employee(id, name, base, coef, start), overtime(ot) {}

    double calcSalary() const override {
        return baseSalary * coefSalary + overtime;
    }

    void display() const override {
        cout << "[Programmer] " << name << " | ID: " << id
             << " | Salary: " << calcSalary() << endl;
    }
};

// ====================== Lớp Designer ======================
class Designer : public Employee {
    double bonus;
public:
    Designer(string id, string name, double base, double coef, string start, double bonus)
        : Employee(id, name, base, coef, start), bonus(bonus) {}

    double calcSalary() const override {
        return baseSalary * coefSalary + bonus;
    }

    void display() const override {
        cout << "[Designer] " << name << " | ID: " << id
             << " | Salary: " << calcSalary() << endl;
    }
};

// ====================== Lớp Tester ======================
class Tester : public Employee {
    int errorCount;
public:
    Tester(string id, string name, double base, double coef, string start, int err)
        : Employee(id, name, base, coef, start), errorCount(err) {}

    double calcSalary() const override {
        return baseSalary * coefSalary + errorCount * 200000;
    }

    void display() const override {
        cout << "[Tester] " << name << " | ID: " << id
             << " | Salary: " << calcSalary() << endl;
    }
};

// ====================== HÀM MAIN ======================
int main() {
    vector<Employee*> company;

    // ---- Nhập dữ liệu mẫu ----
    Manager* m1 = new Manager("M01", "Nguyen Van A", 5000000, 3.0, "01/01/2022");
    Manager* m2 = new Manager("M02", "Tran Thi B", 5000000, 2.8, "01/02/2022");
    Programmer* p1 = new Programmer("P01", "Le Van C", 4000000, 2.0, "10/03/2022", 500000);
    Designer* d1 = new Designer("D01", "Pham Thi D", 4500000, 2.2, "15/03/2022", 700000);
    Tester* t1 = new Tester("T01", "Hoang Van E", 4000000, 1.8, "20/03/2022", 3);

    // ---- Thiết lập quan hệ quản lý ----
    m1->addSubordinate(m2);
    m1->addSubordinate(p1);
    m2->addSubordinate(d1);
    m2->addSubordinate(t1);

    // ---- Thêm vào danh sách công ty ----
    company.push_back(m1);
    company.push_back(m2);
    company.push_back(p1);
    company.push_back(d1);
    company.push_back(t1);

    // ---- In thông tin ----
    cout << "=== DANH SÁCH NHÂN VIÊN CÔNG TY ===\n";
    for (auto e : company) {
        e->display();
    }

    // ---- Tính tổng lương công ty ----
    double totalSalary = 0;
    for (auto e : company)
        totalSalary += e->calcSalary();
    cout << "\nTổng tiền lương công ty phải trả: " << totalSalary << " VND\n";

    // ---- Đếm Manager có >10 cấp dưới ----
    int bigManagers = 0;
    for (auto e : company) {
        Manager* m = dynamic_cast<Manager*>(e);
        if (m && m->countAllSubordinates() > 10)
            bigManagers++;
    }
    cout << "\nSố Manager quản lý trên 10 cấp dưới (trực tiếp hoặc gián tiếp): "
         << bigManagers << endl;

    // ---- Giải phóng bộ nhớ ----
    for (auto e : company) delete e;

    return 0;
}

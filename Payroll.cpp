#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

// ==============================
// Interface trừu tượng Payable
// ==============================
class Payable {
public:
    virtual double getPaymentAmount() const = 0; // phương thức thuần ảo
    virtual void print() const = 0;              // in thông tin đối tượng
    virtual ~Payable() {}                        // destructor ảo
};

// ==============================
// Lớp Invoice (Hóa đơn)
// ==============================
class Invoice : public Payable {
private:
    string partNumber;
    string partDescription;
    int quantity;
    double pricePerItem;

public:
    Invoice(const string& part, const string& desc, int qty, double price)
        : partNumber(part), partDescription(desc), quantity(qty), pricePerItem(price) {}

    double getPaymentAmount() const override {
        return quantity * pricePerItem;
    }

    void print() const override {
        cout << fixed << setprecision(2);
        cout << "Invoice:\n"
             << " Part Number: " << partNumber
             << "\n Description: " << partDescription
             << "\n Quantity: " << quantity
             << "\n Price per item: $" << pricePerItem
             << "\n => Total: $" << getPaymentAmount() << "\n\n";
    }
};

// ==============================
// Lớp trừu tượng Employee (Nhân viên)
// ==============================
class Employee : public Payable {
protected:
    string firstName;
    string lastName;
    string ssn;

public:
    Employee(const string& first, const string& last, const string& id)
        : firstName(first), lastName(last), ssn(id) {}

    virtual void print() const override {
        cout << "Employee: " << firstName << " " << lastName
             << "\nSSN: " << ssn << endl;
    }
};

// ==============================
// SalariedEmployee (Lương cố định)
// ==============================
class SalariedEmployee : public Employee {
private:
    double weeklySalary;

public:
    SalariedEmployee(const string& first, const string& last, const string& id, double salary)
        : Employee(first, last, id), weeklySalary(salary) {}

    double getPaymentAmount() const override {
        return weeklySalary;
    }

    void print() const override {
        cout << fixed << setprecision(2);
        cout << "Salaried Employee: " << firstName << " " << lastName
             << "\nWeekly Salary: $" << weeklySalary
             << "\n=> Payment Amount: $" << getPaymentAmount() << "\n\n";
    }
};

// ==============================
// HourlyEmployee (Làm theo giờ)
// ==============================
class HourlyEmployee : public Employee {
private:
    double wage;
    double hours;

public:
    HourlyEmployee(const string& first, const string& last, const string& id, double hourlyWage, double hoursWorked)
        : Employee(first, last, id), wage(hourlyWage), hours(hoursWorked) {}

    double getPaymentAmount() const override {
        if (hours <= 40)
            return wage * hours;
        else
            return 40 * wage + (hours - 40) * wage * 1.5; // overtime
    }

    void print() const override {
        cout << fixed << setprecision(2);
        cout << "Hourly Employee: " << firstName << " " << lastName
             << "\nWage: $" << wage << " | Hours: " << hours
             << "\n=> Payment Amount: $" << getPaymentAmount() << "\n\n";
    }
};

// ==============================
// CommissionEmployee (Hoa hồng)
// ==============================
class CommissionEmployee : public Employee {
protected:
    double grossSales;
    double commissionRate;

public:
    CommissionEmployee(const string& first, const string& last, const string& id,
                       double sales, double rate)
        : Employee(first, last, id), grossSales(sales), commissionRate(rate) {}

    double getPaymentAmount() const override {
        return grossSales * commissionRate;
    }

    void print() const override {
        cout << fixed << setprecision(2);
        cout << "Commission Employee: " << firstName << " " << lastName
             << "\nGross Sales: $" << grossSales
             << " | Commission Rate: " << commissionRate
             << "\n=> Payment Amount: $" << getPaymentAmount() << "\n\n";
    }
};

// ==============================
// BasePlusCommissionEmployee (Hoa hồng + Lương cơ bản)
// ==============================
class BasePlusCommissionEmployee : public CommissionEmployee {
private:
    double baseSalary;

public:
    BasePlusCommissionEmployee(const string& first, const string& last, const string& id,
                               double sales, double rate, double base)
        : CommissionEmployee(first, last, id, sales, rate), baseSalary(base) {}

    double getPaymentAmount() const override {
        return baseSalary + (grossSales * commissionRate);
    }

    void print() const override {
        cout << fixed << setprecision(2);
        cout << "Base Plus Commission Employee: " << firstName << " " << lastName
             << "\nGross Sales: $" << grossSales
             << " | Commission Rate: " << commissionRate
             << " | Base Salary: $" << baseSalary
             << "\n=> Payment Amount: $" << getPaymentAmount() << "\n\n";
    }
};

// ==============================
// Hàm main
// ==============================
int main() {
    vector<Payable*> payables = {
        new Invoice("001A", "CPU Intel i9", 3, 550.0),
        new Invoice("002B", "RAM 16GB", 5, 90.0),
        new SalariedEmployee("Nguyen", "An", "111-11-1111", 1200.0),
        new HourlyEmployee("Le", "Binh", "222-22-2222", 25.0, 42),
        new CommissionEmployee("Tran", "Cuong", "333-33-3333", 10000.0, 0.06),
        new BasePlusCommissionEmployee("Pham", "Duong", "444-44-4444", 5000.0, 0.04, 300.0)
    };

    cout << "====== PAYROLL SYSTEM ======\n\n";
    for (auto p : payables) {
        p->print();
        cout << "---------------------------------------------\n";
    }

    // Giải phóng bộ nhớ
    for (auto p : payables) delete p;

    return 0;
}

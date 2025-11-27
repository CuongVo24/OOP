// main.cpp
#include "lib.h"

int main() {
    vector<Employee*> empList;

    empList.push_back(new Engineer("Alice", 1500, 10, 20)); 
    empList.push_back(new Engineer("Bob", 1600, 5, 20));

    empList.push_back(new Manager("Charlie", 2500, 5, 100));
    empList.push_back(new Manager("Diana", 3000, 10, 150));

    cout << "=== COMPANY EMPLOYEE MANAGEMENT SYSTEM ===\n" << endl;

    double totalSalary = 0;

    cout << fixed << setprecision(2);

    for (size_t i = 0; i < empList.size(); ++i) {
        Employee* emp = empList[i];

        cout << "-----------------------------------" << endl;
        cout << "Name: " << emp->getName() << " | Role: " << emp->getRole() << endl;

        cout << "Work: ";
        emp->describeWork();

        double salary = emp->getSalary();
        cout << "Salary: $" << salary << endl;

        totalSalary += salary;
    }

    cout << "===================================" << endl;
    cout << "TOTAL COMPANY SALARY: $" << totalSalary << endl;


    for (size_t i = 0; i < empList.size(); ++i) {
        delete empList[i];
    }
    empList.clear();

    return 0;
}

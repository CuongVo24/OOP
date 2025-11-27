
#include "lib.h"

// Implement Employee
Employee::Employee(string n, double base) : name(n), baseSalary(base) {}

Employee::~Employee() {
}

string Employee::getName() const {
    return name;
}

// Implement Engineer
Engineer::Engineer(string n, double base, double hours, double rate) 
    : Employee(n, base), overtimeHours(hours), overtimeRate(rate) {}

string Engineer::getRole() const {
    return "Engineer";
}

void Engineer::describeWork() const {
    cout << name << " is analyzing requirements and writing code." << endl;
}

double Engineer::getSalary() const {
    return baseSalary + (overtimeHours * overtimeRate);
}

// Implement Manager
Manager::Manager(string n, double base, int size, double bonusRate)
    : Employee(n, base), teamSize(size), bonusPerMember(bonusRate) {}

string Manager::getRole() const {
    return "Manager";
}

void Manager::describeWork() const {
    cout << name << " is managing a team of " << teamSize << " people." << endl;
}

double Manager::getSalary() const {
    return baseSalary + (teamSize * bonusPerMember);
}

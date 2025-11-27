
#ifndef LIB_H
#define LIB_H

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

class Employee {
protected:
    string name;
    double baseSalary;

public:
    Employee(string n, double base);

    virtual ~Employee();


    virtual string getRole() const = 0;
    virtual void describeWork() const = 0;
    virtual double getSalary() const = 0;

    string getName() const;
};

class Engineer : public Employee {
private:
    double overtimeHours;
    double overtimeRate;

public:
    Engineer(string n, double base, double hours, double rate);

    string getRole() const override;
    void describeWork() const override;
    double getSalary() const override;
};

class Manager : public Employee {
private:
    int teamSize;
    double bonusPerMember;

public:
    Manager(string n, double base, int size, double bonusRate);

    string getRole() const override;
    void describeWork() const override;
    double getSalary() const override;
};

#endif

#ifndef LIB_H
#define LIB_H

#include <iostream>
#include <string>
using namespace std;

// Class Address
class Address {
private:
    string street;
    string city;
    string state;

public:
    Address();
    Address(string st, string c, string s);
    void display() const;
};


// Class Student
class Student {
protected:
    string name;
    string studentID;
    double gpa;
    Address address;

public:
    Student();
    Student(string n, string id, double g, Address addr);

    char getGrade() const;
    virtual void display() const;
};


// Class ExchangeStudent
class ExchangeStudent : public Student {
private:
    string homeCountry;
    int durationMonths;

public:
    ExchangeStudent(string n, string id, double g, Address addr,
                    string country, int duration);

    void updateDuration(int newDuration);
    void display() const override;
};

#endif

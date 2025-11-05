#include "lib.h"
#include <iomanip>

// Address
Address::Address() : street(""), city(""), state("") {}
Address::Address(string st, string c, string s)
    : street(st), city(c), state(s) {}

void Address::display() const {
    cout << street << ", " << city << ", " << state;
}

// Student
Student::Student() : name(""), studentID(""), gpa(0.0), address() {}
Student::Student(string n, string id, double g, Address addr)
    : name(n), studentID(id), gpa(g), address(addr) {}

char Student::getGrade() const {
    if (gpa >= 3.6) return 'A';
    if (gpa >= 3.0) return 'B';
    if (gpa >= 2.0) return 'C';
    if (gpa >= 1.0) return 'D';
    return 'F';
}

void Student::display() const {
    cout << fixed << setprecision(2);
    cout << "Name: " << name << endl;
    cout << "Student ID: " << studentID << endl;
    cout << "GPA: " << gpa << " (" << getGrade() << ")" << endl;
    cout << "Address: ";
    address.display();
    cout << endl;
}

// ExchangeStudent
ExchangeStudent::ExchangeStudent(string n, string id, double g, Address addr,
                                 string country, int duration)
    : Student(n, id, g, addr), homeCountry(country), durationMonths(duration) {}

void ExchangeStudent::updateDuration(int newDuration) {
    durationMonths = newDuration;
}

void ExchangeStudent::display() const {
    Student::display();
    cout << "Home Country: " << homeCountry << endl;
    cout << "Exchange Duration: " << durationMonths << " months" << endl;
}

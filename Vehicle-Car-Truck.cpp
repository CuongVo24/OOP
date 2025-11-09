#include <iostream>
#include <string>
using namespace std;

class Vehicle {
private :
    string brand; int year; double mileage;

public:
    //truy cập tt private
    string getBrand() const { return brand; }
    int getYear() const { return year; }
    double getMileage() const { return mileage; }

    Vehicle(string b, int y, double m) : brand(b), year(y), mileage(m) {}

    virtual double maintenanceCost() const = 0;
    virtual void display() const {
        cout << brand << " (" << year << "), Mileage: " << mileage << endl;
    }
};

class Car : public Vehicle { // Xe LÀ(IS-A) 1 loại phương tiện
public:
    Car(string b, int y, double m) : Vehicle(b, y, m) {}

    double maintenanceCost() const override {
    return getMileage() * 0.05;
}
};


class Truck : public Vehicle { // Xe Tải LÀ(IS-A) 1 loại phương tiện
    double payload;
public:
    Truck(string b, int y, double m, double p)
        : Vehicle(b, y, m), payload(p) {}
    double maintenanceCost() const override {
    return getMileage() * 0.08 + payload * 2;
}
};

int main() {
    Vehicle* v1 = new Car("Toyota", 2020, 30000);
    Vehicle* v2 = new Truck("Volvo", 2019, 50000, 2000);
    v1->display(); cout << "Cost: " << v1->maintenanceCost() << endl;
    v2->display(); cout << "Cost: " << v2->maintenanceCost() << endl;
}

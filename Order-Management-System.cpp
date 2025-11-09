#include <iostream>
#include <string>
using namespace std;

// -------------------- Product --------------------
class Product {
private:
    string name;
    double price;
public:
    Product(string n = "", double p = 0.0): name(n), price(p) {}

    string getName() const { return name; }
    double getPrice() const { return price; }
};

// -------------------- OrderItem --------------------
class OrderItem {
private:
    Product product;
    int qty;
public:
    OrderItem(Product p = Product(), int q = 0): product(p), qty(q) {}

    double total() const { return product.getPrice() * qty; }

    void display() const {
        cout << product.getName() << " x" << qty
             << " = " << total() << endl;
    }
};

// -------------------- Order --------------------
class Order {
private:
    OrderItem* items;
    int count;
    int capacity;

public:
    Order(int cap = 5): count(0), capacity(cap) {
        items = new OrderItem[capacity];
    }

    ~Order() {
        delete[] items;
    }

    void addProduct(Product p, int q) {
        if (count == capacity) {
            // Tăng kích thước mảng khi đầy
            capacity *= 2;
            OrderItem* newArr = new OrderItem[capacity];
            for (int i = 0; i < count; ++i)
                newArr[i] = items[i];
            delete[] items;
            items = newArr;
        }
        items[count++] = OrderItem(p, q);
    }

    double total() const {
        double sum = 0;
        for (int i = 0; i < count; ++i)
            sum += items[i].total();
        return sum;
    }

    void display() const {
        for (int i = 0; i < count; ++i)
            items[i].display();
        cout << "Total: " << total() << endl;
    }
};

// -------------------- Main --------------------
int main() {
    Product p1("Milk Tea", 3.5), p2("Cake", 2.0);

    Order o;
    o.addProduct(p1, 2);
    o.addProduct(p2, 3);
    o.display();

    return 0;
}

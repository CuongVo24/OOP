#include <iostream>
#include <string>
using namespace std;

class Book {
private: // thuộc tín bỏ trong private gồm tên sách, tên tác giả và năm xuất bản
    string title, author;
    double price;
    int year;

public: //Constructor, hàm giảm giá và hàm in
    Book(string t, string a, double p, int y)
        : title(t), author(a), price(p), year(y) {}

    void discount(double percent) { price -= price * percent / 100; }

    void display() const {
        cout << "Title: " << title << ", Author: " << author
             << ", Price: " << price << ", Year: " << year << endl;
    }
};

int main() {
    Book b("Clean Code", "Robert Martin", 25.0, 2008);
    b.discount(10);
    b.display();
}

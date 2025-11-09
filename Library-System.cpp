#include <iostream>
#include <string>
using namespace std;

class Book {
private:
    string title;
    bool available;
public:
    Book(string t): title(t), available(true) {}

    string getTitle() const { return title; }
    bool isAvailable() const { return available; }

    void setAvailable(bool a) { available = a; }
};

class Member {
private:
    string name;
    Book** borrowed;   // mảng con trỏ Book
    int count;         // số sách đã mượn
    int capacity;      // kích thước mảng hiện tại

public:
    Member(string n): name(n), count(0), capacity(2) {
        borrowed = new Book*[capacity];
    }

    ~Member() {
        delete[] borrowed; // giải phóng bộ nhớ
    }

    void borrow(Book &b) {
        if (!b.isAvailable()) {
            cout << b.getTitle() << " is not available!\n";
            return;
        }
        if (count == capacity) { // mở rộng mảng nếu đầy
            capacity *= 2;
            Book** newArr = new Book*[capacity];
            for (int i = 0; i < count; ++i)
                newArr[i] = borrowed[i];
            delete[] borrowed;
            borrowed = newArr;
        }
        borrowed[count++] = &b;
        b.setAvailable(false);
    }

    void returnBook(Book &b) {
        b.setAvailable(true);
        // Tìm vị trí của sách trong danh sách mượn
        for (int i = 0; i < count; ++i) {
            if (borrowed[i] == &b) {
                // Dịch các phần tử còn lại về trái
                for (int j = i; j < count - 1; ++j)
                    borrowed[j] = borrowed[j + 1];
                count--;
                cout << name << " returned: " << b.getTitle() << endl;
                return;
            }
        }
        cout << name << " didn't borrow this book!\n";
    }
        

    void display() const {
        cout << name << " borrowed: ";
        if (count == 0) cout << "nothing.";
        else {
            for (int i = 0; i < count; ++i)
                cout << borrowed[i]->getTitle() << (i == count - 1 ? "" : ", ");
        }
        cout << endl;
    }
};

int main() {
    Book b1("OOP"), b2("DSA"), b3("AI");
    Member m("Cuong");

    m.borrow(b1);
    m.borrow(b2);
    m.display();

    m.returnBook(b1);
    m.borrow(b3);
    m.display();

    return 0;
}

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// === Class Base (Trừu tượng) ===
class Base {
public:
    // Virtual Destructor cực kỳ quan trọng khi delete qua con trỏ Base
    virtual ~Base() {}

    // Hàm ảo thuần túy, buộc lớp con phải định nghĩa
    virtual void Print(ostream& os) const = 0;

    // Operator << đóng vai trò trung gian
    // Nó gọi hàm ảo Print -> nhờ đó đạt được tính đa hình
    friend ostream& operator<<(ostream& os, const Base& b) {
        b.Print(os);
        return os;
    }
    
    // Hỗ trợ in con trỏ Base* (để dùng trong vector<Base*>)
    friend ostream& operator<<(ostream& os, const Base* b) {
        b->Print(os);
        return os;
    }
};

// === Class Derived1 ===
class Derived1 : public Base {
private:
    int _ofD1;
public:
    Derived1(int v) : _ofD1(v) {}

    // Override hàm Print để in thông tin riêng của Derived1
    void Print(ostream& os) const override {
        os << "D1 - " << _ofD1;
    }
};

// === Class Derived2 ===
class Derived2 : public Base {
private:
    int _ofD2;
public:
    Derived2(int v) : _ofD2(v) {}

    // Override hàm Print để in thông tin riêng của Derived2
    void Print(ostream& os) const override {
        os << "D2 - " << _ofD2;
    }
};

int main() {
    vector<Base*> ds;
    
    ds.push_back(new Derived1(13));
    ds.push_back(new Derived1(17));
    ds.push_back(new Derived2(24));
    ds.push_back(new Derived2(28));

    cout << "--- VI DU 1: POLYMORPHISM << ---" << endl;
    for (int i = 0; i < ds.size(); i++) {
        // Gọi operator<< (con trỏ), nó sẽ gọi Print() tương ứng
        cout << ds[i] << endl;
    }

    // Clean up memory
    for (auto x : ds) delete x;
    ds.clear();

    return 0;
}

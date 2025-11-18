#include <iostream>

using namespace std;

#define NOE 10 // [cite: 100]

class A {
private:
    int info[NOE]; // [cite: 103]

public:
    // Định nghĩa kiểu indexOf là con trỏ int để có thể trả về tham chiếu/địa chỉ
    typedef int* indexOf; // [cite: 107]

    A() {
        for (int i = 0; i < NOE; ++i) {
            info[i] = i + NOE; // [cite: 119]
        }
    }

    // Trả về địa chỉ của phần tử thứ i
    indexOf GetInfo(const int& i) {
        // Logic: info là mảng, info + i là địa chỉ phần tử thứ i
        return info + i; // [cite: 128]
    }

    void Xuat(ostream& os) {
        for (int i = 0; i < NOE; ++i) {
            os << info[i] << ", "; // [cite: 141]
        }
        os << endl;
    }
};

int main() {
    const int i = 7; // [cite: 147]
    A a;
    
    // Sử dụng kiểu dữ liệu nội bộ A::indexOf
    A::indexOf io = a.GetInfo(i); // [cite: 151]
    
    // Dereference con trỏ để lấy giá trị
    cout << *io << endl; // 
    
    // Thay đổi giá trị thông qua con trỏ (minh họa việc truy cập trực tiếp vùng nhớ)
    *io = 10; // [cite: 154]
    
    a.Xuat(cout); // [cite: 155]
    
    system("pause");
    return 0;
}

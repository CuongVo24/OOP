#include <iostream>
#include <string>

using namespace std;

// === Singleton Class ===
class PrintConsoleDevice {
private:
    ostream* os;
    
    // Private Constructor: Chỉ class này mới tự tạo được instance
    PrintConsoleDevice() {
        os = &cout; 
    }
    
    // Cấm copy constructor và phép gán để đảm bảo tính duy nhất
    PrintConsoleDevice(const PrintConsoleDevice&) = delete;
    void operator=(const PrintConsoleDevice&) = delete;

public:
    void Print(const string& s) {
        *os << s;
    }

    // Static Method để truy cập instance duy nhất
    static PrintConsoleDevice& GetInstance() {
        static PrintConsoleDevice instance; // Khởi tạo 1 lần duy nhất khi chạy qua dòng này
        return instance;
    }
};

int main() {
    cout << "\n--- VI DU 3: SINGLETON PATTERN ---" << endl;

    PrintConsoleDevice& print1 = PrintConsoleDevice::GetInstance();
    print1.Print("Su dung in lan 1\n");


    PrintConsoleDevice& print2 = PrintConsoleDevice::GetInstance();
    print2.Print("Su dung in lan 2\n");

    // Kiểm tra địa chỉ bộ nhớ để chứng minh 2 biến là 1
    cout << "Dia chi print1: " << &print1 << endl;
    cout << "Dia chi print2: " << &print2 << endl;
    if (&print1 == &print2) {
        cout << "-> Cung la mot doi tuong duy nhat!" << endl;
    }

    return 0;
}

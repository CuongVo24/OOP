#include <iostream>
#include <string>
#include <cstdlib>

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

    PrintConsoleDevice& print1 = PrintConsoleDevice::GetInstance();
    print1.Print("Su dung in lan 1\n");


    PrintConsoleDevice& print2 = PrintConsoleDevice::GetInstance();
    print2.Print("Su dung in lan 2\n");

    system("pause");
    return 0;
}

#include <iostream>
#include <cmath>
#include <exception>
#include <stdexcept>
#include <cstdlib>

using namespace std;

// --- Class CA (Dữ liệu cơ sở) ---
class CA {
private:
    double _value;
public:
    CA(const double& v) { _value = v; }
    double GetTest() const { return _value; }
};

// --- Class Level1 (Tầng phát hiện và ném lỗi) ---
class Level1 {
public:
    // VÍ DỤ 4: Minh họa ném ngoại lệ (Throw Exception)
    static double RunTest3(CA* pCA) {
        if (pCA == nullptr) {
            throw runtime_error("Argument must not be null");
        }
        
        double rs = pCA->GetTest();
        return sqrt(rs);
    }
};

// --- Class Level2 (Tầng trung gian) ---
class Level2 {
public:
    // VÍ DỤ 5: Hàm gọi nhưng không xử lý lỗi, để lỗi trôi (propagate) lên trên
    static double RunTest2() {
        CA* pCA = nullptr; // Giả lập lỗi
        
        // Gọi RunTest3 của Level1. 
        // Vì không có try-catch ở đây, nếu có lỗi, nó sẽ bay thẳng ra hàm main.
        return Level1::RunTest3(pCA);
    }
};

int main() {
    cout << "--- BAI TAP 2: VI DU 4 va 5 (Exception Handling) ---" << endl;

    // Bắt đầu khối try để giám sát lỗi
    try {
        cout << "Dang goi Level2::RunTest2()..." << endl;
        
        // Dòng này sẽ gây ra exception vì bên trong nó truyền nullptr
        double result = Level2::RunTest2(); 
        
        // Các dòng dưới này sẽ bị bỏ qua nếu có lỗi xảy ra ở dòng trên
        cout << "Ket qua: " << result << endl;
    }
    // Khối catch để bắt lỗi. 
    // 'const exception& ex' bắt tất cả các lỗi kế thừa từ class exception chuẩn.
    catch (const exception& ex) {
        cout << "!!! DA BAT DUOC LOI !!!" << endl;
        cout << "Noi dung loi (ex.what()): " << ex.what() << endl;
    }

    cout << "\nChuong trinh van tiep tuc chay binh thuong sau khi catch..." << endl;
    system("pause");
    return 0;
}

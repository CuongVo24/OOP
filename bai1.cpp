#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

// --- Class CA (Dữ liệu cơ sở) ---
class CA {
private:
    double _value;
public:
    CA(const double& v) { _value = v; }

    double GetTest() {
        return this->_value;
    }
};

// --- Class Level1 (Tầng xử lý thấp) ---
class Level1 {
public:
    // VÍ DỤ 1: Hàm minh họa gây lỗi (Crash)
    // Không kiểm tra pCA có null hay không trước khi gọi GetTest()
    static double RunTest1(CA* pCA) {
        double rs = pCA->GetTest(); // <--- Dòng này sẽ gây Crash nếu pCA là nullptr
        return sqrt(rs);
    }

    // VÍ DỤ 2: Xử lý lỗi thông qua thông báo lỗi bằng giá trị trả về
    // Có kiểm tra null, trả về -1 nếu lỗi
    static double RunTest2(CA* pCA) {
        if (pCA == nullptr) {
            return -1; // Mã lỗi quy ước
        }
        double rs = pCA->GetTest();
        return sqrt(rs);
    }
};

// --- Class Level2 (Tầng xử lý cao hơn) ---
class Level2 {
public:
    // VÍ DỤ 3: Chuyển lỗi lên cao hơn theo cách làm thông báo lỗi qua giá trị trả về
    // Gọi hàm của Level 1, nếu Level 1 báo lỗi (-1) thì báo tiếp lên trên
    static double RunTest1() {
        CA* pCA = nullptr; // Giả lập con trỏ null
        
        // Gọi xuống tầng thấp hơn
        double dRS = Level1::RunTest2(pCA);
        
        // Kiểm tra kết quả trả về từ tầng thấp
        if (dRS == -1) {
            return -1; // Tiếp tục đẩy lỗi ra ngoài (hoặc xử lý tại đây)
        }
        
        return sqrt(dRS);
    }
};

int main() {
    cout << "--- BAI TAP 1: VI DU 1, 2, 3 ---" << endl;

    // --- TEST VÍ DỤ 1 (Sẽ crash chương trình) ---
    // cout << "Testing Vi du 1 (Crash): " << Level1::RunTest1(nullptr) << endl;
    cout << "[Skip Vi du 1] De tranh crash chuong trinh do truy cap null pointer." << endl;

    // --- TEST VÍ DỤ 2 ---
    cout << "\n--- Testing Vi du 2 (Return Error Code) ---" << endl;
    double res2 = Level1::RunTest2(nullptr);
    if (res2 == -1) {
        cout << "Loi: Con tro null (Ma loi -1)" << endl;
    } else {
        cout << "Ket qua: " << res2 << endl;
    }

    // --- TEST VÍ DỤ 3 ---
    cout << "\n--- Testing Vi du 3 (Propagate Error) ---" << endl;
    double res3 = Level2::RunTest1();
    if (res3 == -1) {
        cout << "Loi: Phat hien loi tu Level 1 day len (Ma loi -1)" << endl;
    } else {
        cout << "Ket qua: " << res3 << endl;
    }

    cout << endl;
    system("pause");
    return 0;
}

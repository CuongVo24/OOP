#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <cmath>

using namespace std;

// --- Class CA (Dữ liệu cơ sở) ---
class CA {
private:
    double _value;
public:
    CA(const double& v) : _value(v) {}
    double GetTest() const { return _value; }
};

// --- Class MyException (Custom Exception) ---
// Kế thừa từ std::runtime_error để tận dụng tính năng lưu trữ thông điệp lỗi (what)
class MyException : public runtime_error {
private:
    int _errorCode;
    string _errorDescription;

    // Hàm helper để tạo thông điệp lỗi đầy đủ
    static string BuildErrorMessage(int errCode, const string& errDes) {
        ostringstream oss;
        oss << "Error code: " << errCode << " - Description: " << errDes;
        return oss.str();
    }

public:
    // Constructor
    MyException(int errCode, const string& errDes)
        : runtime_error(BuildErrorMessage(errCode, errDes)), // Gọi base constructor
          _errorCode(errCode),
          _errorDescription(errDes) 
    {
    }

    // Getter cho các thuộc tính riêng
    string GetErrorDescription() const {
        return _errorDescription;
    }

    int GetErrorCode() const {
        return _errorCode;
    }
};

// --- Class Level1 ---
class Level1 {
public:
    // VÍ DỤ 6: Ném ra Custom Exception
    static double RunTest4(CA* pCA) {
        if (pCA == nullptr) {
            // Ném exception tự định nghĩa kèm mã lỗi (123) và thông điệp
            throw MyException(123, "Argument must not be null");
        }
        return sqrt(pCA->GetTest());
    }
};

// --- Class Level2 ---
class Level2 {
public:
    // Hàm gọi Level 1 (nơi sẽ sinh ra MyException)
    static double RunTest3() {
        CA* pCA = nullptr;
        return Level1::RunTest4(pCA);
    }

    // Hàm sinh ra lỗi thông thường (Standard Exception)
    static void RunErr() {
        // Ném exception chuẩn
        throw runtime_error("Common error"); 
    }
};

int main() {
    cout << "--- BAI TAP 3: CUSTOM EXCEPTION (Vi du 6) ---" << endl;

    // Vòng lặp giả lập 2 tình huống:
    // i = 0: Gây lỗi Custom (MyException)
    // i = 1: Gây lỗi thường (std::exception/runtime_error)
    for (int i = 0; i < 2; i++) {
        cout << "\n**************** Case " << i << " ****************" << endl;
        try {
            if (i == 0) {
                // Tình huống 1: Gọi hàm gây ra MyException
                cout << "Goi Level2::RunTest3()..." << endl;
                cout << Level2::RunTest3() << endl;
            } else {
                // Tình huống 2: Gọi hàm gây ra Exception thường
                cout << "Goi Level2::RunErr()..." << endl;
                Level2::RunErr();
            }
        }
        // CATCH 1: Bắt lỗi Custom cụ thể trước
        catch (const MyException& me) {
            cout << "=> Bat duoc CUSTOM error!" << endl;
            cout << "   Full Log: " << me.what() << endl; // Lấy chuỗi từ runtime_error
            cout << "   Code rieng: " << me.GetErrorCode() << endl;
            cout << "   Desc rieng: " << me.GetErrorDescription() << endl;
        }
        // CATCH 2: Bắt các lỗi còn lại (nếu không phải MyException)
        catch (const exception& ex) {
            cout << "=> Bat duoc COMMON error!" << endl;
            cout << "   Log: " << ex.what() << endl;
        }
    }

    cout << endl;
    system("pause");
    return 0;
}

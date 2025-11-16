#include <iostream>
#include <fstream>
#include <algorithm> // Dùng std::sort, std::max
#include <cstring>   // Dùng strlen, memset, memcpy
#include <cmath>

// KHÔNG DÙNG <vector>, <string>, <list>

using namespace std;

/*
 * LỚP SONGUYENLON
 * Mục tiêu: Kiểm tra "Rule of Three" (Quy tắc Bộ Ba)
 * do sử dụng CẤP PHÁT ĐỘNG (con trỏ int*).
 * Quy ước: Mảng `chuSo` lưu NGƯỢC
 * Ví dụ: "123" -> chuSo = {3, 2, 1}
 * Cách này giúp phép cộng cực kỳ dễ dàng.
 */
class SoNguyenLon {
private:
    int* chuSo;   // Con trỏ tới mảng động
    int soLuong;  // Số lượng chữ số

    // Hàm tiện ích: Xóa số 0 vô nghĩa ở đầu
    // (ví dụ: 00123 -> 123, hoặc 0 -> 0)
    void xoaSoKhong() {
        while (soLuong > 1 && chuSo[soLuong - 1] == 0) {
            soLuong--;
        }
    }
    
    // Hàm tiện ích: Thay đổi kích thước mảng
    // (dùng khi cộng 99+1=100)
    void resize(int newSize) {
        int* temp = new int[newSize];
        // Khởi tạo mảng mới = 0
        memset(temp, 0, newSize * sizeof(int));
        // Copy dữ liệu cũ
        int n = (soLuong < newSize) ? soLuong : newSize;
        memcpy(temp, chuSo, n * sizeof(int));
        // Xóa mảng cũ, trỏ mảng mới
        delete[] chuSo;
        chuSo = temp;
        soLuong = newSize;
    }

public:
    // --- 1. CONSTRUCTORS ---

    // Constructor mặc định (số "0")
    SoNguyenLon() {
        soLuong = 1;
        chuSo = new int[1];
        chuSo[0] = 0;
    }

    // Constructor từ chuỗi (const char*)
    SoNguyenLon(const char* s) {
        soLuong = strlen(s);
        if (soLuong == 0) { // Xử lý chuỗi rỗng
            soLuong = 1;
            chuSo = new int[1];
            chuSo[0] = 0;
        } else {
            chuSo = new int[soLuong];
            // Lưu ngược: "123" -> {3, 2, 1}
            for (int i = 0; i < soLuong; i++) {
                chuSo[i] = s[soLuong - 1 - i] - '0';
            }
            xoaSoKhong(); // Xử lý trường hợp "000"
        }
    }

    // --- 2. "RULE OF THREE" (CỰC KỲ QUAN TRỌNG) ---

    // (A) Copy Constructor (Thực hiện Deep Copy)
    SoNguyenLon(const SoNguyenLon& other) {
        // Sao chép giá trị
        this->soLuong = other.soLuong;
        // Cấp phát vùng nhớ MỚI (Deep Copy)
        this->chuSo = new int[this->soLuong];
        // Sao chép NỘI DUNG (không phải con trỏ)
        memcpy(this->chuSo, other.chuSo, this->soLuong * sizeof(int));
    }

    // (B) Destructor (Hàm hủy)
    ~SoNguyenLon() {
        // Phải giải phóng bộ nhớ đã cấp phát bằng 'new[]'
        delete[] chuSo;
    }

    // (C) Assignment Operator= (Toán tử gán)
    SoNguyenLon& operator=(const SoNguyenLon& other) {
        // 1. Kiểm tra tự gán (VD: a = a)
        // Nếu không có, bạn sẽ delete[] chính mình!
        if (this == &other) {
            return *this;
        }

        // 2. Giải phóng tài nguyên CŨ của 'this'
        delete[] this->chuSo;

        // 3. Thực hiện Deep Copy (giống Copy Constructor)
        this->soLuong = other.soLuong;
        this->chuSo = new int[this->soLuong];
        memcpy(this->chuSo, other.chuSo, this->soLuong * sizeof(int));

        // 4. Trả về *this để cho phép gán liên hoàn (a = b = c)
        return *this;
    }

    // --- 3. TOÁN TỬ ---

    // Toán tử Xuất (friend)
    friend ostream& operator<<(ostream& os, const SoNguyenLon& snl) {
        // In ngược lại (từ cuối mảng về đầu)
        for (int i = snl.soLuong - 1; i >= 0; i--) {
            os << snl.chuSo[i];
        }
        return os;
    }

    // Toán tử So sánh (cho std::sort)
    bool operator<(const SoNguyenLon& other) const {
        // So sánh số lượng chữ số
        if (this->soLuong != other.soLuong) {
            return this->soLuong < other.soLuong;
        }
        // Nếu bằng, so từ chữ số lớn nhất (đang ở cuối mảng)
        for (int i = this->soLuong - 1; i >= 0; i--) {
            if (this->chuSo[i] != other.chuSo[i]) {
                return this->chuSo[i] < other.chuSo[i];
            }
        }
        return false; // Bằng nhau
    }

    // Toán tử Cộng (logic cộng tay)
    SoNguyenLon operator+(const SoNguyenLon& other) const {
        int maxLen = max(this->soLuong, other.soLuong);
        SoNguyenLon tong;
        tong.resize(maxLen + 1); // Cấp phát (vd 99+1=100)
        
        int nho = 0;
        for (int i = 0; i < maxLen; i++) {
            int a = (i < this->soLuong) ? this->chuSo[i] : 0;
            int b = (i < other.soLuong) ? other.chuSo[i] : 0;
            
            int s = a + b + nho;
            tong.chuSo[i] = s % 10;
            nho = s / 10;
        }
        
        // Xử lý số nhớ cuối cùng
        tong.chuSo[maxLen] = nho;
        tong.xoaSoKhong(); // Xóa số 0 thừa
        
        return tong;
    }
};

// --- Hàm Main ---
int main() {
    int n = 5;
    SoNguyenLon mang[100]; // Mảng C-style tĩnh

    cout << "--- Khoi tao 5 SoNguyenLon ---" << endl;
    mang[0] = SoNguyenLon("12345678901234567890");
    mang[1] = SoNguyenLon("999");
    mang[2] = SoNguyenLon("12345678901234567891"); // Lớn hơn [0]
    mang[3] = SoNguyenLon("888888");
    mang[4] = SoNguyenLon("999"); // Giống [1]

    cout << "\n--- Mang truoc khi sap xep ---" << endl;
    for (int i = 0; i < n; i++) cout << mang[i] << endl;

    // YÊU CẦU CỐT LÕI: DÙNG <algorithm>
    sort(mang, mang + n); // Tự động gọi operator<

    cout << "\n--- Mang sau khi sap xep (tang dan) ---" << endl;
    for (int i = 0; i < n; i++) cout << mang[i] << endl;

    // Kiểm tra operator+
    SoNguyenLon tong = SoNguyenLon("999") + SoNguyenLon("1");
    cout << "\n--- Kiem tra phep cong ---" << endl;
    cout << "999 + 1 = " << tong << endl; // Phải ra 1000

    // Kiểm tra "Rule of Three" (gán và sao chép)
    cout << "\n--- Kiem tra Rule of Three ---" << endl;
    SoNguyenLon a("111");
    SoNguyenLon b = a;  // Gọi Copy Constructor
    SoNguyenLon c("222");
    c = a;              // Gọi Assignment Operator=
    a = a;              // Kiểm tra tự gán
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "c = " << c << endl;
    // Khi hàm main kết thúc, Destructor của a, b, c, 
    // và 100 phần tử mảng sẽ được gọi.
    // Nếu chương trình không crash -> Rule of Three ĐÚNG.

    return 0;
}

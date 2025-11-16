#include <iostream>
#include <fstream>      // Đọc file
#include <algorithm>    // Dùng std::sort
#include <cmath>        // Dùng abs()
#include <iomanip>      // Dùng setw

// KHÔNG DÙNG <vector>, <string>, <list>

using namespace std;

/*
 * LỚP PHANSO
 
 * Mục tiêu: Chuẩn hóa dữ liệu (rutGon) và nạp chồng toán tử
 * để tương thích với std::sort.
 */

class PhanSo {
private:
    int tu;
    int mau;

    // Hàm tiện ích private static (dùng chung, không cần đối tượng)
    static int UCLN(int a, int b) {
        a = abs(a);
        b = abs(b);
        while (b != 0) {
            int temp = a % b;
            a = b;
            b = temp;
        }
        return a;
    }

    // Hàm chuẩn hóa "thần thánh"
    void rutGon() {
        // 1. Xử lý mẫu = 0
        if (mau == 0) {
            cout << "LOI: Mau so bang 0! Dat ve 0/1." << endl;
            tu = 0;
            mau = 1;
            return;
        }
        // 2. Xử lý tử = 0
        if (tu == 0) {
            mau = 1;
            return;
        }
        // 3. Xử lý dấu (đảm bảo mẫu luôn dương)
        if (mau < 0) {
            tu = -tu;
            mau = -mau;
        }
        // 4. Tối giản
        int uoc = UCLN(tu, mau);
        tu /= uoc;
        mau /= uoc;
    }

public:
    // Constructor mặc định
    PhanSo() : tu(0), mau(1) {}

    // Constructor tham số
    PhanSo(int t, int m) : tu(t), mau(m) {
        rutGon(); // Chuẩn hóa ngay khi tạo
    }

    // --- TOÁN TỬ ---

    // 1. Toán tử Nhập/Xuất (bắt buộc friend)
    // Dùng const PhanSo& vì xuất không làm thay đổi đối tượng
    friend ostream& operator<<(ostream& os, const PhanSo& p) {
        os << p.tu << "/" << p.mau;
        return os;
    }

    // Phải là tham chiếu (PhanSo& p) vì nhập làm thay đổi đối tượng
    friend istream& operator>>(istream& is, PhanSo& p) {
        cout << "  Nhap tu: ";
        is >> p.tu;
        cout << "  Nhap mau: ";
        is >> p.mau;
        
        p.rutGon(); // Chuẩn hóa ngay khi nhập
        return is;
    }

    // 2. Toán tử So sánh (cho std::sort)
    // Đây là chìa khóa để std::sort(mang, mang + n) hiểu
    bool operator<(const PhanSo& other) const {
        // So sánh chéo (long long) để tránh tràn số
        return (long long)this->tu * other.mau < (long long)other.tu * this->mau;
    }

    // 3. Toán tử Số học
    PhanSo operator+(const PhanSo& other) const {
        // Trả về một đối tượng MỚI (return-by-value)
        PhanSo tong;
        tong.tu = this->tu * other.mau + other.tu * this->mau;
        tong.mau = this->mau * other.mau;
        tong.rutGon(); // Rất quan trọng: kết quả phải được chuẩn hóa
        return tong;
    }
};

// Hàm main
int main() {
    int n = 5;
    PhanSo mang[100]; // Dùng mảng C-style tĩnh (do không có <vector>)

    cout << "--- Nhap 5 phan so ---" << endl;
    mang[0] = PhanSo(1, 2);    // 0.5
    mang[1] = PhanSo(-7, 2);   // -3.5
    mang[2] = PhanSo(8, 4);    // 2.0
    mang[3] = PhanSo(0, 10);   // 0.0
    mang[4] = PhanSo(-10, -2); // 5.0

    cout << "\n--- Mang truoc khi sap xep ---" << endl;
    for (int i = 0; i < n; i++) {
        cout << mang[i] << "  ";
    }
    cout << endl;

    // YÊU CẦU CỐT LÕI: DÙNG <algorithm>
    // std::sort sẽ tự động tìm và gọi PhanSo::operator<
    sort(mang, mang + n);

    cout << "\n--- Mang sau khi sap xep (tang dan) ---" << endl;
    for (int i = 0; i < n; i++) {
        cout << mang[i] << "  ";
    }
    cout << endl;

    // Kiểm tra operator+
    PhanSo tong = mang[0] + mang[1];
    cout << "\n--- Kiem tra phep cong ---" << endl;
    cout << mang[0] << " + " << mang[1] << " = " << tong << endl;

    return 0;
}

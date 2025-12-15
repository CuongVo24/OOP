#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

#define PI 3.14159f

// --- Class Cơ sở: HinhHocPhang ---
class HinhHocPhang {
public:
    // Constructor mặc định (cần thiết cho dòng: new HinhHocPhang())
    HinhHocPhang() {}

    // Destructor ảo (Good practice: để tránh memory leak khi delete qua con trỏ cha)
    virtual ~HinhHocPhang() {}

    // Từ khóa virtual là CHÌA KHÓA của bài này
    virtual float TinhDienTich() {
        return 0.0f; // Mặc định trả về 0
    }
};

// --- Class Dẫn xuất: HinhChuNhat ---
class HinhChuNhat : public HinhHocPhang {
private:
    float dai, rong;
public:
    // Constructor có tham số
    HinhChuNhat(float d, float r) : dai(d), rong(r) {}

    // Ghi đè phương thức tính diện tích
    float TinhDienTich() override {
        return dai * rong;
    }
};

// --- Class Dẫn xuất: HinhTron ---
class HinhTron : public HinhHocPhang {
private:
    float banKinh;
public:
    // Constructor có tham số
    HinhTron(float bk) : banKinh(bk) {}

    // Ghi đè phương thức tính diện tích
    float TinhDienTich() override {
        return PI * banKinh * banKinh;
    }
};

int main() {
    /// Sử dụng pointer
    HinhHocPhang* hhp = new HinhHocPhang();
    HinhChuNhat* hcn = new HinhChuNhat(34, 56.8);
    HinhTron* ht = new HinhTron(73.9);

    cout << hhp->TinhDienTich() << "\t"
        << hcn->TinhDienTich() << "\t"
        << ht->TinhDienTich();
    cout << endl;

    /// Vì HinhChuNhat là một loại HinhHocPhang nên có thể gán
    delete hhp;

    hhp = hcn;

    cout << hhp->TinhDienTich() << endl;

    cout << endl << endl;

    delete hcn;
    delete ht;

    system("pause");
    return 0;
}

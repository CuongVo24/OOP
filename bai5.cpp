
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip> // Để format output đẹp

using namespace std;

const float PI = 3.14159f;

// --- Abstract Base Class (Lớp trừu tượng) ---
class Hinh {
public:
    virtual ~Hinh() {}
    virtual string GetTen() const = 0; // Hàm thuần ảo
    virtual void Xuat() const = 0;     // Hàm thuần ảo để in thông tin
};

// ================= NHÓM HÌNH 2D =================


class HinhTron : public Hinh {
private:
    float r;
public:
    HinhTron(float _r) : r(_r) {}
    string GetTen() const override { return "Hinh Tron"; }

    void Xuat() const override {
        float P = 2 * PI * r;
        float S = PI * r * r;
        cout << GetTen() << " [P = " << P << ", S = " << S << "]" << endl;
    }
};

class HinhTamGiacDeu : public Hinh {
private:
    float a;
public:
    HinhTamGiacDeu(float _a) : a(_a) {}
    string GetTen() const override { return "Hinh Tam Giac Deu"; }

    void Xuat() const override {
        float P = 3 * a;
        float S = (sqrt(3) / 4) * a * a;
        cout << GetTen() << " [P = " << P << ", S = " << S << "]" << endl;
    }
};

class HinhChuNhat : public Hinh {
protected: // protected để HinhVuong có thể truy cập nếu kế thừa (hoặc dùng riêng)
    float d, r;
public:
    HinhChuNhat(float _d, float _r) : d(_d), r(_r) {}
    string GetTen() const override { return "Hinh Chu Nhat"; }

    void Xuat() const override {
        float P = 2 * (d + r);
        float S = d * r;
        cout << GetTen() << " [P = " << P << ", S = " << S << "]" << endl;
    }
};

// HinhVuong có thể kế thừa HinhChuNhat để tận dụng code (IS-A relationship)
class HinhVuong : public HinhChuNhat {
public:
    HinhVuong(float canh) : HinhChuNhat(canh, canh) {} // Gọi constructor cha
    string GetTen() const override { return "Hinh Vuong"; }
};

// ================= NHÓM HÌNH 3D =================

class HinhCau : public Hinh {
private:
    float r;
public:
    HinhCau(float _r) : r(_r) {}
    string GetTen() const override { return "Hinh Cau"; }

    void Xuat() const override {
        float Sxp = 4 * PI * r * r; // Diện tích mặt cầu
        float V = (4.0f / 3.0f) * PI * r * r * r;
        cout << GetTen() << " [Sxp = " << Sxp << ", V = " << V << "]" << endl;
    }
};

class HinhLapPhuong : public Hinh {
private:
    float a;
public:
    HinhLapPhuong(float _a) : a(_a) {}
    string GetTen() const override { return "Hinh Lap Phuong"; }

    void Xuat() const override {
        float Sxp = 4 * a * a; // Diện tích xung quanh (4 mặt)
        float V = a * a * a;
        cout << GetTen() << " [Sxp = " << Sxp << ", V = " << V << "]" << endl;
    }
};

class HinhHop : public Hinh {
private:
    float d, r, c; // Dài, Rộng, Cao
public:
    HinhHop(float _d, float _r, float _c) : d(_d), r(_r), c(_c) {}
    string GetTen() const override { return "Hinh Hop"; }

    void Xuat() const override {
        float Sxp = 2 * c * (d + r); // Chu vi đáy * chiều cao
        float V = d * r * c;
        cout << GetTen() << " [Sxp = " << Sxp << ", V = " << V << "]" << endl;
    }
};

class HinhTru : public Hinh {
private:
    float r, h;
public:
    HinhTru(float _r, float _h) : r(_r), h(_h) {}
    string GetTen() const override { return "Hinh Tru"; }

    void Xuat() const override {
        float Sxp = 2 * PI * r * h;
        float V = PI * r * r * h;
        cout << GetTen() << " [Sxp = " << Sxp << ", V = " << V << "]" << endl;
    }
};

class HinhNon : public Hinh {
private:
    float r, h;
public:
    HinhNon(float _r, float _h) : r(_r), h(_h) {}
    string GetTen() const override { return "Hinh Non"; }

    void Xuat() const override {
        float l = sqrt(r * r + h * h); // Đường sinh
        float Sxp = PI * r * l;
        float V = (1.0f / 3.0f) * PI * r * r * h;
        cout << GetTen() << " [Sxp = " << Sxp << ", V = " << V << "]" << endl;
    }
};

int main() {

    vector<Hinh*> dsHinh;

    dsHinh.push_back(new HinhTron(3.5));
    dsHinh.push_back(new HinhTamGiacDeu(8.5));
    dsHinh.push_back(new HinhChuNhat(3.7, 1.9));
    dsHinh.push_back(new HinhVuong(4.1));
    dsHinh.push_back(new HinhCau(13.5));
    dsHinh.push_back(new HinhLapPhuong(2.9));
    dsHinh.push_back(new HinhHop(1.2, 3.7, 6.3));
    dsHinh.push_back(new HinhTru(6.1, 4.9));
    dsHinh.push_back(new HinhNon(8.7, 3.2));

    cout << fixed << setprecision(2);

    for (auto h : dsHinh) {
        h->Xuat();
    }

    for (auto h : dsHinh) delete h;
    dsHinh.clear();

    return 0;
}

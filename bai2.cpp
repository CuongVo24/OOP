#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

// ================= BASE CLASS =================
class HinhHocPhang {
public:
    virtual ~HinhHocPhang() {}
    
    virtual void Input(istream& is) = 0;
    virtual void Output(ostream& os) const = 0;
    virtual float TinhDienTich() const = 0;
    virtual float TinhChuVi() const = 0;

    friend istream& operator>>(istream& is, HinhHocPhang& h) {
        h.Input(is);
        return is;
    }

    friend ostream& operator<<(ostream& os, const HinhHocPhang& h) {
        h.Output(os);
        return os;
    }
};

// ================= DERIVED CLASSES =================

// 1. Hình Chữ Nhật
class HinhChuNhat : public HinhHocPhang {
private:
    float dai, rong;
public:
    void Input(istream& is) override {
        // Format: 12.5,8.1
        is >> dai;
        is.ignore(); // Bỏ qua dấu phẩy
        is >> rong;
    }
    void Output(ostream& os) const override {
        os << "HCN (Dai: " << dai << ", Rong: " << rong << ")";
    }
    float TinhDienTich() const override { return dai * rong; }
    float TinhChuVi() const override { return (dai + rong) * 2; }
};

// 2. Hình Tròn
class HinhTron : public HinhHocPhang {
private:
    float banKinh;
public:
    void Input(istream& is) override {
        is >> banKinh;
    }
    void Output(ostream& os) const override {
        os << "Hinh Tron (R: " << banKinh << ")";
    }
    float TinhDienTich() const override { return 3.14159 * banKinh * banKinh; }
    float TinhChuVi() const override { return 2 * 3.14159 * banKinh; }
};

// 3. Hình Tam Giác
class HinhTamGiac : public HinhHocPhang {
private:
    float a, b, c;
public:
    void Input(istream& is) override {
        // Format: 7.4,9.4,5
        is >> a; is.ignore();
        is >> b; is.ignore();
        is >> c;
    }
    void Output(ostream& os) const override {
        os << "Tam Giac (" << a << ", " << b << ", " << c << ")";
    }
    float TinhDienTich() const override {
        float p = (a + b + c) / 2;
        return sqrt(p * (p - a) * (p - b) * (p - c));
    }
    float TinhChuVi() const override { return a + b + c; }
};

// 4. Hình Thang Vuông
class HinhThangVuong : public HinhHocPhang {
private:
    float dayLon, dayBe, chieuCao;
public:
    void Input(istream& is) override {
        // Format: 16,10.5,7.9 (Giả sử thứ tự: Đáy lớn, Đáy bé, Cao)
        is >> dayLon; is.ignore();
        is >> dayBe; is.ignore();
        is >> chieuCao;
    }
    void Output(ostream& os) const override {
        os << "Hinh Thang Vuong (DayLon: " << dayLon << ", DayBe: " << dayBe << ", Cao: " << chieuCao << ")";
    }
    float TinhDienTich() const override {
        return ((dayLon + dayBe) * chieuCao) / 2;
    }
    float TinhChuVi() const override {
        // Tính cạnh bên huyền
        float canhHuyen = sqrt(pow(abs(dayLon - dayBe), 2) + pow(chieuCao, 2));
        return dayLon + dayBe + chieuCao + canhHuyen;
    }
};

// ================= DEMO MAIN CHO BÀI 2 =================
int main() {
    // Tạo file mẫu
    ofstream taoFile("hhp.txt");
    taoFile << "hcn: 12.5,8.1\n";
    taoFile << "htr: 23.5\n";
    taoFile << "htg: 7.4,9.4,5\n";
    taoFile << "hthv: 16,10.5,7.9\n";
    taoFile.close();

    ifstream file("hhp.txt");
    vector<HinhHocPhang*> dsHinh;
    string type;
    
    // Logic đọc file: Đọc loại trước, sau đó loại bỏ dấu ':', rồi mới gọi operator>>
    while (getline(file, type, ':')) {
        HinhHocPhang* hinh = nullptr;
        
        // Loại bỏ khoảng trắng sau dấu hai chấm nếu có
        // Lưu ý: getline ở trên đã 'ăn' mất dấu ':' rồi
        
        if (type.find("hcn") != string::npos) hinh = new HinhChuNhat();
        else if (type.find("htr") != string::npos) hinh = new HinhTron();
        else if (type.find("htg") != string::npos) hinh = new HinhTamGiac();
        else if (type.find("hthv") != string::npos) hinh = new HinhThangVuong();

        if (hinh != nullptr) {
            file >> *hinh;
            // Quan trọng: Sau khi đọc số, con trỏ file vẫn ở cuối dòng hiện tại hoặc đầu dòng mới
            // Cần bỏ qua ký tự xuống dòng để vòng lặp sau getline đọc đúng tên loại tiếp theo
            file.ignore(100, '\n'); 
            dsHinh.push_back(hinh);
        }
    }

    cout << "=== DANH SACH HINH HOC ===" << endl;
    for (const auto& h : dsHinh) {
        cout << *h << " -> Dien Tich: " << h->TinhDienTich() << endl;
    }

    // Dọn dẹp
    for (auto h : dsHinh) delete h;
    return 0;
}

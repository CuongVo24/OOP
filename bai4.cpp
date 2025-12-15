#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iomanip>

using namespace std;

// === 1. BASE CLASS ===
class HinhHocPhang {
public:
    virtual ~HinhHocPhang() {}
    virtual float TinhChuVi() const = 0;
    virtual float TinhDienTich() const = 0;
    virtual string GetLoai() const = 0;

    // Hàm khởi tạo từ chuỗi tham số (Factory Method Pattern support)
    virtual void Init(const vector<float>& params) = 0;
};

// === 2. CÁC CLASS DẪN XUẤT ===

class HinhChuNhat : public HinhHocPhang {
    float d, r;
public:
    string GetLoai() const override { return "Hinh Chu Nhat"; }
    void Init(const vector<float>& params) override {
        if (params.size() >= 2) { d = params[0]; r = params[1]; }
    }
    float TinhChuVi() const override { return (d + r) * 2; }
    float TinhDienTich() const override { return d * r; }
};

class HinhTron : public HinhHocPhang {
    float r;
public:
    string GetLoai() const override { return "Hinh Tron"; }
    void Init(const vector<float>& params) override {
        if (params.size() >= 1) { r = params[0]; }
    }
    float TinhChuVi() const override { return 2 * 3.14159 * r; }
    float TinhDienTich() const override { return 3.14159 * r * r; }
};

class HinhTamGiac : public HinhHocPhang {
    float a, b, c;
public:
    string GetLoai() const override { return "Hinh Tam Giac"; }
    void Init(const vector<float>& params) override {
        if (params.size() >= 3) { a = params[0]; b = params[1]; c = params[2]; }
    }
    float TinhChuVi() const override { return a + b + c; }
    float TinhDienTich() const override {
        float p = (a + b + c) / 2;
        return sqrt(p * (p - a) * (p - b) * (p - c)); // Công thức Heron
    }
};

class HinhThangVuong : public HinhHocPhang {
    float dayLon, dayBe, chieuCao;
public:
    string GetLoai() const override { return "Hinh Thang Vuong"; }
    void Init(const vector<float>& params) override {
        if (params.size() >= 3) {
            dayLon = params[0]; dayBe = params[1]; chieuCao = params[2];
        }
    }
    float TinhChuVi() const override {
        // Tính cạnh bên (huyền của tam giác vuông tạo bởi chiều cao và hiệu 2 đáy)
        float canhBen = sqrt(chieuCao * chieuCao + pow(abs(dayLon - dayBe), 2));
        return dayLon + dayBe + chieuCao + canhBen;
    }
    float TinhDienTich() const override {
        return ((dayLon + dayBe) * chieuCao) / 2;
    }
};

// === 3. LỚP QUẢN LÝ (CONTROLLER) ===
class QuanLyHinhHoc {
private:
    vector<HinhHocPhang*> dsHinh;

public:
    ~QuanLyHinhHoc() {
        for (auto h : dsHinh) delete h;
    }

    void DocFile(string filename) {
        ifstream f(filename);
        if (!f.is_open()) return;

        string line;
        while (getline(f, line)) {
            stringstream ss(line);
            string loai, paramStr;

            // Đọc phần loại hình (ví dụ "hcn:")
            if (!getline(ss, loai, ':')) continue;

            // Đọc phần số liệu còn lại
            getline(ss, paramStr);

            // Parse chuỗi số liệu "12.5, 8.1" thành vector<float>
            vector<float> params;
            stringstream ssParams(paramStr);
            string valStr;
            while (getline(ssParams, valStr, ',')) {
                params.push_back(stof(valStr));
            }

            HinhHocPhang* h = nullptr;
            if (loai == "hcn") h = new HinhChuNhat();
            else if (loai == "htr") h = new HinhTron();
            else if (loai == "htg") h = new HinhTamGiac();
            else if (loai == "hthv") h = new HinhThangVuong();

            if (h) {
                h->Init(params); // Đẩy thông số vào
                dsHinh.push_back(h);
            }
        }
        f.close();
    }

    void XuatThongKe() {
        int countHCN = 0, countHT = 0, countHTG = 0, countHTV = 0;
        float tongDT = 0, tongCV = 0;

        cout << "\n=== DANH SACH HINH DOC TU FILE ===" << endl;
        cout << left << setw(20) << "LOAI HINH" << setw(15) << "DIEN TICH" << setw(15) << "CHU VI" << endl;

        for (auto h : dsHinh) {
            float dt = h->TinhDienTich();
            float cv = h->TinhChuVi();
            string loai = h->GetLoai();

            tongDT += dt;
            tongCV += cv;

            // Đếm số lượng (Dùng string compare thay vì dynamic_cast cho đơn giản)
            if (loai == "Hinh Chu Nhat") countHCN++;
            else if (loai == "Hinh Tron") countHT++;
            else if (loai == "Hinh Tam Giac") countHTG++;
            else if (loai == "Hinh Thang Vuong") countHTV++;

            cout << left << setw(20) << loai
                << setw(15) << fixed << setprecision(2) << dt
                << setw(15) << cv << endl;
        }

        cout << "\n=== THONG KE ===" << endl;
        cout << "Tong so hinh: " << dsHinh.size() << endl;
        cout << "- Hinh Chu Nhat: " << countHCN << endl;
        cout << "- Hinh Tron: " << countHT << endl;
        cout << "- Hinh Tam Giac: " << countHTG << endl;
        cout << "- Hinh Thang Vuong: " << countHTV << endl;
        cout << "Tong Dien Tich: " << tongDT << endl;
        cout << "Tong Chu Vi: " << tongCV << endl;
    }
};

int main() {

    QuanLyHinhHoc ql;
    ql.DocFile("hhp.txt");
    ql.XuatThongKe();

    return 0;
}

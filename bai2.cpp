#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <string>
#include <sstream>

using namespace std;

// === 1. LỚP CƠ SỞ ===
class HinhHocPhang {
public:
    virtual ~HinhHocPhang() {}
    virtual void Nhap(vector<float> params) = 0;
    virtual float TinhDienTich() const = 0;
    virtual float TinhChuVi() const = 0;
    virtual string GetTenLoai() const = 0;
};

// === 2. CÁC LỚP HÌNH CỤ THỂ ===
class HinhChuNhat : public HinhHocPhang {
    float dai, rong;
public:
    void Nhap(vector<float> params) override {
        if (params.size() >= 2) { dai = params[0]; rong = params[1]; }
    }
    float TinhDienTich() const override { return dai * rong; }
    float TinhChuVi() const override { return (dai + rong) * 2; }
    string GetTenLoai() const override { return "Hinh Chu Nhat"; }
};

class HinhTron : public HinhHocPhang {
    float r;
public:
    void Nhap(vector<float> params) override {
        if (params.size() >= 1) r = params[0];
    }
    float TinhDienTich() const override { return 3.14 * r * r; }
    float TinhChuVi() const override { return 2 * 3.14 * r; }
    string GetTenLoai() const override { return "Hinh Tron"; }
};

class HinhTamGiac : public HinhHocPhang {
    float a, b, c;
public:
    void Nhap(vector<float> params) override {
        if (params.size() >= 3) { a = params[0]; b = params[1]; c = params[2]; }
    }
    float TinhDienTich() const override {
        float p = (a + b + c) / 2; // Nửa chu vi
        return sqrt(p * (p - a) * (p - b) * (p - c)); // Công thức Heron
    }
    float TinhChuVi() const override { return a + b + c; }
    string GetTenLoai() const override { return "Hinh Tam Giac"; }
};

class HinhThangVuong : public HinhHocPhang {
    float dayLon, dayBe, cao;
public:
    void Nhap(vector<float> params) override {
        if (params.size() >= 3) { dayLon = params[0]; dayBe = params[1]; cao = params[2]; }
    }
    float TinhDienTich() const override {
        return ((dayLon + dayBe) * cao) / 2;
    }
    float TinhChuVi() const override {
        float canhHuyen = sqrt(pow(cao, 2) + pow(abs(dayLon - dayBe), 2));
        return dayLon + dayBe + cao + canhHuyen;
    }
    string GetTenLoai() const override { return "Hinh Thang Vuong"; }
};

// === 3. LỚP QUẢN LÝ ===
class QuanLyHinhHoc {
    vector<HinhHocPhang*> dsHinh;
public:
    ~QuanLyHinhHoc() {
        for (auto h : dsHinh) delete h;
        dsHinh.clear();
    }

    void DocFile(string filename) {
        ifstream f(filename);
        if (!f.is_open()) return;

        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            
            // Xử lý chuỗi: "hcn: 12.5,8.1"
            size_t pos = line.find(':');
            string type = line.substr(0, pos);
            string paramStr = line.substr(pos + 1);

            // Parse các con số cách nhau bởi dấu phẩy
            vector<float> params;
            stringstream ss(paramStr);
            string val;
            while (getline(ss, val, ',')) {
                params.push_back(atof(val.c_str()));
            }

            // Factory tạo đối tượng
            HinhHocPhang* h = NULL;
            if (type == "hcn") h = new HinhChuNhat();
            else if (type == "htr") h = new HinhTron();
            else if (type == "htg") h = new HinhTamGiac();
            else if (type == "hthv") h = new HinhThangVuong();

            if (h) {
                h->Nhap(params);
                dsHinh.push_back(h);
            }
        }
        f.close();
    }

    void BaoCao() {
        float tongDT = 0, tongCV = 0;
        int countHCN = 0, countHTR = 0, countHTG = 0, countHTHV = 0;

        for (auto h : dsHinh) {
            tongDT += h->TinhDienTich();
            tongCV += h->TinhChuVi();
            string loai = h->GetTenLoai();
            if (loai == "Hinh Chu Nhat") countHCN++;
            else if (loai == "Hinh Tron") countHTR++;
            else if (loai == "Hinh Tam Giac") countHTG++;
            else if (loai == "Hinh Thang Vuong") countHTHV++;
        }

        cout << "=== BAO CAO ===" << endl;
        cout << "So luong HCN: " << countHCN << endl;
        cout << "So luong Hinh Tron: " << countHTR << endl;
        cout << "So luong Tam Giac: " << countHTG << endl;
        cout << "So luong Hinh Thang Vuong: " << countHTHV << endl;
        cout << "Tong Dien Tich: " << tongDT << endl;
        cout << "Tong Chu Vi: " << tongCV << endl;
    }
};

// === MAIN DEMO CHO BÀI 2 ===
void DemoBai2() {
    QuanLyHinhHoc ql;
    ql.DocFile("hhp.txt");
    ql.BaoCao();
}

int main() {
    // Bạn có thể comment/uncomment để chạy từng bài
    // DemoBai1(); 
    cout << "-----------------" << endl;
    DemoBai2();
    return 0;
}

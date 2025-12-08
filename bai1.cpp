#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

// === 1. LỚP CƠ SỞ (BASE CLASS) ===
class NhanVien {
protected:
    string hoTen;
    string diaChi;

public:
    virtual ~NhanVien() {} // Destructor ảo cực kỳ quan trọng để tránh Memory Leak

    // Phương thức thuần ảo (Pure Virtual) hoặc ảo
    virtual void Nhap(string paramLine) {
        // Xử lý chuỗi format: "Ten (DiaChi)"
        // Tìm vị trí các dấu ngoặc
        size_t moNgoac = paramLine.find('(');
        size_t dongNgoac = paramLine.find(')');
        
        if (moNgoac != string::npos && dongNgoac != string::npos) {
            this->hoTen = paramLine.substr(0, moNgoac);
            // Xóa khoảng trắng thừa ở cuối tên
            while (!hoTen.empty() && hoTen.back() == ' ') hoTen.pop_back();
            
            this->diaChi = paramLine.substr(moNgoac + 1, dongNgoac - moNgoac - 1);
        }
    }

    virtual void Xuat() const {
        cout << "Ho ten: " << hoTen << " - Dia chi: " << diaChi;
    }

    // Logic nghiệp vụ quan trọng: Đa hình
    virtual bool IsXuatSac() const = 0; // Thuần ảo, bắt buộc con phải định nghĩa
};

// === 2. CÁC LỚP DẪN XUẤT (DERIVED CLASSES) ===

class NhanVienThuKy : public NhanVien {
private:
    string chungChiNN;
    int soBaoCao;
public:
    void Nhap(string paramLine) override {
        // Format: "Ten (DiaChi) [ChungChi] <SoBaoCao>"
        
        // Gọi lớp cha xử lý phần Tên và Địa chỉ trước
        NhanVien::Nhap(paramLine);

        // Xử lý phần riêng
        size_t moVuong = paramLine.find('[');
        size_t dongVuong = paramLine.find(']');
        size_t moNhon = paramLine.find('<');
        size_t dongNhon = paramLine.find('>');

        if (moVuong != string::npos && dongVuong != string::npos) {
            this->chungChiNN = paramLine.substr(moVuong + 1, dongVuong - moVuong - 1);
        }
        if (moNhon != string::npos && dongNhon != string::npos) {
            string soStr = paramLine.substr(moNhon + 1, dongNhon - moNhon - 1);
            this->soBaoCao = atoi(soStr.c_str());
        }
    }

    void Xuat() const override {
        NhanVien::Xuat();
        cout << " - Loai: Thu Ky - CC: " << chungChiNN << " - Bao cao: " << soBaoCao << endl;
    }

    bool IsXuatSac() const override {
        return soBaoCao >= 12;
    }
};

class NhanVienKyThuat : public NhanVien {
private:
    string chungChiNganh;
    int soSangKien;
public:
    void Nhap(string paramLine) override {
        // Tái sử dụng logic parse (thực tế nên viết hàm utility tách chuỗi riêng để clean code)
        NhanVien::Nhap(paramLine);

        size_t moVuong = paramLine.find('[');
        size_t dongVuong = paramLine.find(']');
        size_t moNhon = paramLine.find('<');
        size_t dongNhon = paramLine.find('>');

        if (moVuong != string::npos && dongVuong != string::npos) {
            this->chungChiNganh = paramLine.substr(moVuong + 1, dongVuong - moVuong - 1);
        }
        if (moNhon != string::npos && dongNhon != string::npos) {
            string soStr = paramLine.substr(moNhon + 1, dongNhon - moNhon - 1);
            this->soSangKien = atoi(soStr.c_str());
        }
    }

    void Xuat() const override {
        NhanVien::Xuat();
        cout << " - Loai: Ky Thuat - Nganh: " << chungChiNganh << " - Sang kien: " << soSangKien << endl;
    }

    bool IsXuatSac() const override {
        return soSangKien >= 6;
    }
};

// === 3. LỚP QUẢN LÝ (MANAGER CLASS) ===
class CongTy {
private:
    vector<NhanVien*> dsNV;
public:
    ~CongTy() {
        for (NhanVien* nv : dsNV) delete nv;
        dsNV.clear();
    }

    void DocFile(string filename) {
        ifstream f(filename);
        if (!f.is_open()) {
            cout << "Khong the mo file!" << endl;
            return;
        }

        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;

            // Tách phần loại nhân viên (trước dấu :)
            size_t pos = line.find(':');
            if (pos == string::npos) continue;

            string type = line.substr(0, pos);
            string data = line.substr(pos + 1);
            
            // Xóa khoảng trắng đầu chuỗi data
            while (!data.empty() && data.front() == ' ') data.erase(0, 1);

            NhanVien* nv = NULL;
            if (type == "nvtk") {
                nv = new NhanVienThuKy();
            } else if (type == "nvkt") {
                nv = new NhanVienKyThuat();
            }

            if (nv != NULL) {
                nv->Nhap(data);
                dsNV.push_back(nv);
            }
        }
        f.close();
    }

    void XuatDSXuatSac() {
        cout << "\n=== DANH SACH NHAN VIEN XUAT SAC ===" << endl;
        for (size_t i = 0; i < dsNV.size(); i++) {
            if (dsNV[i]->IsXuatSac()) {
                dsNV[i]->Xuat();
            }
        }
    }
};

// === MAIN DEMO CHO BÀI 1 ===
void DemoBai1() {
    CongTy cty;
    // Giả sử file input.txt đã có nội dung như hình
    cty.DocFile("input.txt");
    cty.XuatDSXuatSac();
}

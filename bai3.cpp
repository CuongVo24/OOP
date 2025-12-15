#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

// === 1. LỚP CƠ SỞ TRỪU TƯỢNG ===
class NhanVien {
protected:
    string hoTen;
    string diaChi;

public:
    NhanVien(string ten = "", string dc = "") : hoTen(ten), diaChi(dc) {}
    virtual ~NhanVien() {}

    // Hàm thuần ảo: Bắt buộc lớp con phải định nghĩa
    virtual bool IsXuatSac() const = 0;
    virtual void Xuat() const {
        cout << " - " << hoTen << " (" << diaChi << ")";
    }

    // Hàm hỗ trợ parse dữ liệu từ chuỗi (Helper function)
    virtual void ParseData(string data) = 0;
};

// === 2. LỚP NHÂN VIÊN KỸ THUẬT ===
class NhanVienKyThuat : public NhanVien {
private:
    string nganh;
    int soSangKien;

public:
    NhanVienKyThuat() : soSangKien(0) {}

    bool IsXuatSac() const override {
        return soSangKien >= 6; // Tiêu chí: >= 6 sáng kiến
    }

    void Xuat() const override {
        NhanVien::Xuat();
        cout << " [Ky Thuat: " << nganh << "] <" << soSangKien << " sang kien>" << endl;
    }

    // Parse format: "Ten (DiaChi) [Nganh] <SoSangKien>"
    void ParseData(string data) override {
        // Kỹ thuật xử lý chuỗi thủ công để tách các phần trong ngoặc
        size_t p1 = data.find('(');
        size_t p2 = data.find(')');
        size_t p3 = data.find('[');
        size_t p4 = data.find(']');
        size_t p5 = data.find('<');
        size_t p6 = data.find('>');

        if (p1 != string::npos && p3 != string::npos && p5 != string::npos) {
            hoTen = data.substr(0, p1 - 1); // Cắt bỏ khoảng trắng cuối tên
            // Trim tên nếu cần
            while (!hoTen.empty() && hoTen.back() == ' ') hoTen.pop_back();

            diaChi = data.substr(p1 + 1, p2 - p1 - 1);
            nganh = data.substr(p3 + 1, p4 - p3 - 1);
            soSangKien = stoi(data.substr(p5 + 1, p6 - p5 - 1));
        }
    }
};

// === 3. LỚP NHÂN VIÊN THƯ KÝ ===
class NhanVienThuKy : public NhanVien {
private:
    string chungChiNN;
    int soBaoCao;

public:
    NhanVienThuKy() : soBaoCao(0) {}

    bool IsXuatSac() const override {
        return soBaoCao >= 12; // Tiêu chí: >= 12 báo cáo
    }

    void Xuat() const override {
        NhanVien::Xuat();
        cout << " [Thu Ky: " << chungChiNN << "] <" << soBaoCao << " bao cao>" << endl;
    }

    // Parse format tương tự: "Ten (DiaChi) [ChungChi] <SoBaoCao>"
    void ParseData(string data) override {
        size_t p1 = data.find('(');
        size_t p2 = data.find(')');
        size_t p3 = data.find('[');
        size_t p4 = data.find(']');
        size_t p5 = data.find('<');
        size_t p6 = data.find('>');

        if (p1 != string::npos && p3 != string::npos && p5 != string::npos) {
            hoTen = data.substr(0, p1 - 1);
            while (!hoTen.empty() && hoTen.back() == ' ') hoTen.pop_back();

            diaChi = data.substr(p1 + 1, p2 - p1 - 1);
            chungChiNN = data.substr(p3 + 1, p4 - p3 - 1);
            soBaoCao = stoi(data.substr(p5 + 1, p6 - p5 - 1));
        }
    }
};

// === 4. LỚP QUẢN LÝ CÔNG TY ===
class CongTy {
private:
    vector<NhanVien*> dsNV;

public:
    ~CongTy() {
        for (auto nv : dsNV) delete nv;
        dsNV.clear();
    }

    void DocFile(const string& filename) {
        ifstream f(filename);
        if (!f.is_open()) {
            cout << "Loi mo file!" << endl;
            return;
        }

        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;

            // Tách loại nhân viên
            size_t splitPos = line.find(':');
            if (splitPos == string::npos) continue;

            string type = line.substr(0, splitPos);
            string data = line.substr(splitPos + 1);

            // Xóa khoảng trắng đầu data nếu có
            if (!data.empty() && data[0] == ' ') data.erase(0, 1);

            NhanVien* nv = nullptr;
            if (type == "nvkt") {
                nv = new NhanVienKyThuat();
            }
            else if (type == "nvtk") {
                nv = new NhanVienThuKy();
            }

            if (nv) {
                nv->ParseData(data); // Đa hình: Gọi hàm parse của lớp con tương ứng
                dsNV.push_back(nv);
            }
        }
        f.close();
    }

    void XuatDS() const {
        cout << "\n=== DANH SACH NHAN VIEN ===" << endl;
        for (auto nv : dsNV) nv->Xuat();
    }

    void LietKeXuatSac() const {
        cout << "\n=== DANH SACH NHAN VIEN XUAT SAC ===" << endl;
        for (auto nv : dsNV) {
            if (nv->IsXuatSac()) { // Đa hình: Tự động gọi đúng tiêu chí
                nv->Xuat();
            }
        }
    }
};

int main() {
    CongTy myCompany;

    myCompany.DocFile("input.txt");
    myCompany.XuatDS();
    myCompany.LietKeXuatSac();

    return 0;
}

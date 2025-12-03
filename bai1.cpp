#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

// ================= BASE CLASS =================
class NhanVien {
protected:
    string hoTen;
    string diaChi;

public:
    NhanVien() {}
    virtual ~NhanVien() {}

    // Phương thức thuần ảo để đa hình hóa input/output
    virtual void Input(istream& is) = 0;
    virtual void Output(ostream& os) const = 0;
    virtual string getLoai() const = 0; // Để nhận diện loại nhân viên
    virtual bool isXuatSac() const = 0; // Phục vụ yêu cầu tìm nhân viên xuất sắc

    // Operator friend gọi phương thức ảo
    friend istream& operator>>(istream& is, NhanVien& nv) {
        nv.Input(is);
        return is;
    }

    friend ostream& operator<<(ostream& os, const NhanVien& nv) {
        nv.Output(os);
        return os;
    }
};

// ================= DERIVED CLASS: NHÂN VIÊN THƯ KÝ =================
class NhanVienThuKy : public NhanVien {
private:
    string chungChiNgoaiNgu;
    int soBaoCao;

public:
    string getLoai() const override { return "nvtk"; }

    bool isXuatSac() const override {
        return soBaoCao >= 12;
    }

    void Input(istream& is) override {
        // Format:  Tran Van A (Tp Ho Chi Minh) [Dien] <10>
        // Lưu ý: Lúc này con trỏ file đang đứng sau chữ "nvtk: "
        
        // Đọc họ tên đến trước dấu '('
        getline(is, hoTen, '('); 
        // Xóa khoảng trắng dư ở cuối tên nếu có (tùy chọn)
        while (!hoTen.empty() && hoTen.back() == ' ') hoTen.pop_back();

        // Đọc địa chỉ đến trước dấu ')'
        getline(is, diaChi, ')');

        // Bỏ qua các ký tự cho đến khi gặp '['
        is.ignore(100, '[');
        getline(is, chungChiNgoaiNgu, ']');

        // Bỏ qua các ký tự cho đến khi gặp '<'
        is.ignore(100, '<');
        is >> soBaoCao;
        
        // Bỏ qua ký tự '>' và xuống dòng còn lại
        is.ignore(100, '\n');
    }

    void Output(ostream& os) const override {
        os << "Nhan vien thu ky: " << hoTen 
           << " - DC: " << diaChi 
           << " - CC: " << chungChiNgoaiNgu 
           << " - Bao cao: " << soBaoCao;
    }
};

// ================= DERIVED CLASS: NHÂN VIÊN KỸ THUẬT =================
class NhanVienKyThuat : public NhanVien {
private:
    string chungChiNganh;
    int soSangKien;

public:
    string getLoai() const override { return "nvkt"; }

    bool isXuatSac() const override {
        return soSangKien >= 6;
    }

    void Input(istream& is) override {
        // Format tương tự: Tran Van B (Ha Noi) [IT] <5>
        getline(is, hoTen, '(');
        while (!hoTen.empty() && hoTen.back() == ' ') hoTen.pop_back();

        getline(is, diaChi, ')');

        is.ignore(100, '[');
        getline(is, chungChiNganh, ']');

        is.ignore(100, '<');
        is >> soSangKien;
        
        is.ignore(100, '\n');
    }

    void Output(ostream& os) const override {
        os << "Nhan vien ky thuat: " << hoTen 
           << " - DC: " << diaChi 
           << " - Nganh: " << chungChiNganh 
           << " - Sang kien: " << soSangKien;
    }
};

// ================= DEMO MAIN CHO BÀI 1 =================
int main() {
    // Giả lập đọc file input.txt
    // Trong thực tế bạn dùng: ifstream file("input.txt");
    // Ở đây tôi tạo file mẫu để bạn copy chạy thử ngay
    ofstream taoFile("input.txt");
    taoFile << "nvkt: Tran Van A (Tp Ho Chi Minh) [Dien] <10>\n";
    taoFile << "nvtk: Nguyen Thi B (Tp Ho Chi Minh) [Anh] <11>\n";
    taoFile << "nvkt: Le Van C (Tp Ha Noi) [IT] <4>\n";
    taoFile.close();

    ifstream file("input.txt");
    if (!file.is_open()) {
        cerr << "Khong the mo file!" << endl;
        return 1;
    }

    vector<NhanVien*> dsNhanVien;
    string line;
    
    // Xử lý đọc file (Factory Pattern đơn giản)
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string type;
        // Đọc phần loại nhân viên (vd: "nvkt:")
        getline(ss, type, ':'); 
        
        // Bỏ qua khoảng trắng sau dấu :
        ss.ignore(); 

        NhanVien* nv = nullptr;
        if (type == "nvtk") {
            nv = new NhanVienThuKy();
        } else if (type == "nvkt") {
            nv = new NhanVienKyThuat();
        }

        if (nv != nullptr) {
            // Gọi operator>> (đa hình)
            // Lưu ý: ss đang chứa phần còn lại của dòng (thông tin nhân viên)
            ss >> *nv; 
            dsNhanVien.push_back(nv);
        }
    }

    // Xuất danh sách dùng operator<<
    cout << "=== DANH SACH NHAN VIEN ===" << endl;
    for (const auto& nv : dsNhanVien) {
        cout << *nv << endl;
    }

    // Dọn dẹp bộ nhớ
    for (auto nv : dsNhanVien) delete nv;
    file.close();

    return 0;
}

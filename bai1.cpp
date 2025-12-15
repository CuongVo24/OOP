
#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>

using namespace std;

// --- Class NhanVien ---
class NhanVien {
protected: // Dùng protected để lớp con có thể truy cập nếu cần
    string hoTen;
    string diaChi;

public:
    NhanVien(const string& ht = "", const string& dc = "") 
        : hoTen(ht), diaChi(dc) {} // Member Initializer List

    virtual ~NhanVien() {} // Destructor ảo là bắt buộc khi có kế thừa

    friend ostream& operator<<(ostream& os, const NhanVien& nv);
};

ostream& operator<<(ostream& os, const NhanVien& nv) {
    os << nv.hoTen << " (" << nv.diaChi << ")";
    return os;
}

// --- Class NhanVienKyThuat ---
class NhanVienKyThuat : public NhanVien {
private:
    string chungChiNganh;

public:
    NhanVienKyThuat(const string& ht = "", const string& dc = "", const string& ccn = "")
        : NhanVien(ht, dc), chungChiNganh(ccn) {}

    friend ostream& operator<<(ostream& os, const NhanVienKyThuat& nvkt);
};

ostream& operator<<(ostream& os, const NhanVienKyThuat& nvkt) {
    // Kỹ thuật tái sử dụng code của lớp cha: Ép kiểu về tham chiếu lớp cha
    os << (const NhanVien&)nvkt; 
    os << " [" << nvkt.chungChiNganh << "]";
    return os;
}


int main() {
    NhanVienKyThuat nv("Nguyen Van A", "Q.5, TP.HCM", "CNTT");
    cout << nv << endl;
    return 0;
}

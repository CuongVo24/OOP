
#include <iostream>
#include <string>

using namespace std;

// --- Class NhanVien (Base) ---
class NhanVien {
private:
    string hoTen;
    string diaChi;

public:
    // Constructor
    NhanVien(const string& ht = "", const string& dc = "")
        : hoTen(ht), diaChi(dc) {} // Member Initializer List (Tối ưu hiệu năng)

    virtual ~NhanVien() {} // Luôn nên có virtual destructor khi làm việc với kế thừa

    // Friend operator
    friend ostream& operator<<(ostream& os, const NhanVien& nv);
};

ostream& operator<<(ostream& os, const NhanVien& nv) {
    os << nv.hoTen << " (" << nv.diaChi << ")";
    return os;
}

// --- Class NhanVienKyThuat (Derived) ---
class NhanVienKyThuat : public NhanVien {
private:
    string chungChiNganh;

public:
    // Constructor chaining: Gọi constructor của lớp cha
    NhanVienKyThuat(const string& ht = "", const string& dc = "", const string& cc = "")
        : NhanVien(ht, dc), chungChiNganh(cc) {}

    ~NhanVienKyThuat() {}

    friend ostream& operator<<(ostream& os, const NhanVienKyThuat& nvkt);
};

ostream& operator<<(ostream& os, const NhanVienKyThuat& nvkt) {
    // KỸ THUẬT: Ép kiểu về lớp cha để dùng lại operator<< của cha
    os << (NhanVien)nvkt;

    os << " [" << nvkt.chungChiNganh << "]";
    return os;
}

int main() {
    NhanVienKyThuat nvkt("Nguyen Van A", "Q.5, TP.HCM", "CNTT");
    cout << nvkt << endl;
    return 0;
}

#include <iostream>
using namespace std;

class SoPhuc {
private:
    int thuc, ao;
    int sl;            // Biến đếm cục bộ (thuộc về từng đối tượng)
    static int ssl;    // Biến đếm tĩnh (dùng chung cho tất cả các đối tượng)

public:
    // Constructor mặc định
    SoPhuc() {
        thuc = ao = 0;
        sl++;
        ssl++;
    }

    // Constructor có tham số
    SoPhuc(const int& th, const int& a) {
        thuc = th;
        ao = a;
        sl++;
        ssl++;
    }

    // Constructor sao chép
    SoPhuc(const SoPhuc& sp) {
        thuc = sp.thuc;
        ao = sp.ao;
        sl++;
        ssl++;
    }

    // Destructor
    ~SoPhuc() {
        sl--;
        ssl--;
    }

    // Hàm cộng số phức
    SoPhuc Cong(const SoPhuc& sp) {
        SoPhuc kq(*this);
        kq.thuc += sp.thuc;
        kq.ao += sp.ao;
        return kq;
    }

    // Lấy số lượng đối tượng (phiên bản thành viên)
    int SoLuongSP() {
        return sl;
    }

    // Lấy số lượng đối tượng (phiên bản tĩnh)
    static int SSoLuongSP() {
        return ssl;
    }
};

// Khởi tạo biến static bên ngoài lớp
int SoPhuc::ssl = 0;

int main() {
    SoPhuc sp1;
    SoPhuc sp2(5, 10);
    SoPhuc sp3(sp1);
    SoPhuc sp4 = sp2.Cong(sp1);

    cout << "So phuc hien co (ssl): " << SoPhuc::SSoLuongSP() << endl; // 4

    {
        SoPhuc sp5;
        SoPhuc sp6 = sp2.Cong(sp4);
        cout << "Trong block - so luong doi tuong: " << sp4.SSoLuongSP() << endl; // 6
    } // sp5 và sp6 bị hủy tại đây → ssl giảm 2

    cout << "Sau block - so luong doi tuong: " << SoPhuc::SSoLuongSP() << endl; // 4

    system("pause");
    return 0;
}

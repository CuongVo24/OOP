#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>

using namespace std;

// Định nghĩa lớp HocSinh
class HocSinh
{
private:
    string HoTen;
    float DiemToan, DiemVan, DiemNgoaiNgu;
    float DiemTrungBinh;

    //Biến tĩnh để theo dõi sinh viên giỏi nhất
    // Biến tĩnh lưu điểm TB cao nhất
    static float DiemTBCaoNhat;
    // Biến tĩnh lưu một bản sao của sinh viên có điểm TB cao nhất
    static HocSinh HocSinhGioiNhat;

    // Phương thức private trợ giúp
    // Tính điểm trung bình (xử lý điểm âm)
    void TinhDiemTrungBinh()
    {
        // Coi điểm âm là 0 khi tính trung bình
        float dt = (DiemToan < 0) ? 0 : DiemToan;
        float dv = (DiemVan < 0) ? 0 : DiemVan;
        float dnn = (DiemNgoaiNgu < 0) ? 0 : DiemNgoaiNgu;
        DiemTrungBinh = (dt + dv + dnn) / 3.0;
    }

    // Kiểm tra và cập nhật sinh viên giỏi nhất
    void CapNhatHocSinhGioiNhat()
    {
        if (this->DiemTrungBinh > HocSinh::DiemTBCaoNhat)
        {
            HocSinh::DiemTBCaoNhat = this->DiemTrungBinh;
            HocSinh::HocSinhGioiNhat = *this; // Lưu bản sao của đối tượng này
        }
    }

public:
    // 1. Hàm dựng mặc định
    HocSinh()
    {
        HoTen = "Chua co ten";
        DiemToan = DiemVan = DiemNgoaiNgu = 0;
        TinhDiemTrungBinh();
        // Ngay cả sinh viên mặc định (0 điểm) cũng cần được kiểm tra
        CapNhatHocSinhGioiNhat();
    }

    // 4. Hàm dựng có tham số
    HocSinh(string ten, float dToan, float dVan, float dNN)
    {
        HoTen = ten;
        DiemToan = dToan;
        DiemVan = dVan;
        DiemNgoaiNgu = dNN;
        TinhDiemTrungBinh();      // Tính ĐTB
        CapNhatHocSinhGioiNhat(); // Kiểm tra xem có phải là giỏi nhất không
    }

    // Hàm hủy
    ~HocSinh()
    {
    }

    // 2. Phương thức gán họ tên
    void DatHoTen(string ten)
    {
        HoTen = ten;
    }

    // 3. Phương thức gán điểm
    void GanDiem(float dToan, float dVan, float dNN)
    {
        DiemToan = dToan;
        DiemVan = dVan;
        DiemNgoaiNgu = dNN;
        TinhDiemTrungBinh();
        CapNhatHocSinhGioiNhat();
    }

    // 5. Phương thức xuất thông tin
    void In(ostream& os)
    {
        os << "HS: " << HoTen << ", DTB: "
           << fixed << setprecision(2) << DiemTrungBinh;
    }

    // 6. Phương thức tĩnh trả về sinh viên giỏi nhất
    static HocSinh HSDiemTBCaoNhat()
    {
        return HocSinh::HocSinhGioiNhat;
    }
};

// Khởi tạo các thành viên tĩnh bên ngoài lớp
// Khởi tạo điểm TB cao nhất là -1 (thấp hơn mọi điểm khả thi)
float HocSinh::DiemTBCaoNhat = -1.0;
// Khởi tạo đối tượng tĩnh (sẽ gọi hàm dựng mặc định)
HocSinh HocSinh::HocSinhGioiNhat;


// Hàm main
int main()
{
    HocSinh hs1;
    hs1.DatHoTen("Nguyen Van A");
    hs1.GanDiem(7, 8, 10);

    HocSinh hs2("Tran Thi B", 5, 8, 4.5);
    HocSinh hs3("Hoang Thi C", -9.5, 8.5, 4.5);
    HocSinh hs4("Le Van D", 7.5, 9, -10);

    // Dòng này được bình luận trong file gốc
    //HS hs("Nguyen Van A, MS: 23120001, DTB: 7.53"

    hs1.In(cout);
    cout << endl;
    hs2.In(cout);
    cout << endl;
    hs3.In(cout);
    cout << endl;
    hs4.In(cout);
    cout << endl;

    cout << "Hoc sinh co diem TB cao nhat: " << endl;
    HocSinh::HSDiemTBCaoNhat().In(cout);
    cout << endl;
    
    system("pause");
    return 0;
}

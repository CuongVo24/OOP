#include <iostream>
#include <cstdlib>

using namespace std;

// --- Định nghĩa lớp Ngay ---
class Ngay
{
private:
    int _ng, _th, _n;
    // Mảng tĩnh chứa số ngày của các tháng
    // Hàng 0: Năm không nhuận, Hàng 1: Năm nhuận
    // Cột 0 không dùng, Cột 1-12 tương ứng T1-T12
    static int arrNgayCuaThang[2][13];
    // Hàm tĩnh kiểm tra năm nhuận
    static bool LaNamNhuan(const int& _n);

public:
    Ngay(); // Hàm dựng mặc định
    Ngay(const int& n); // Hàm dựng từ tổng số ngày
    ~Ngay(); // Hàm hủy
    void Xuat(ostream& os); // Hàm xuất ra màn hình
};

// --- Khởi tạo thành viên tĩnh ---
// Khởi tạo mảng số ngày của các tháng
int Ngay::arrNgayCuaThang[2][13] = {
    {-1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, // Hàng 0: Năm không nhuận
    {-1, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}  // Hàng 1: Năm nhuận
};

// --- Cài đặt các hàm thành viên ---

// Hàm kiểm tra năm nhuận
bool Ngay::LaNamNhuan(const int& n)
{
    // Trả về true nếu n chia hết cho 400
    // hoặc (n chia hết cho 4 và không chia hết cho 100)
    return ((n % 400 == 0) || (n % 4 == 0 && n % 100 != 0));
}

// Hàm dựng mặc định
Ngay::Ngay()
{
    // Khởi tạo ngày 1/1/1
    _ng = _th = _n = 1;
}

// Hàm dựng từ tổng số ngày (n)
Ngay::Ngay(const int& n)
{
    // Gán tổng số ngày, khởi tạo tháng = 1, năm = 1
    _ng = (n >= 0) ? n : -n; // Lấy trị tuyệt đối
    _th = _n = 1;
    
    // Nếu n = 0, coi là ngày 1/1/1
    if (_ng == 0)
    {
        _ng = 1;
        return;
    }

    // Lấy chỉ số năm nhuận (0 hoặc 1)
    int idxN = LaNamNhuan(_n);
    
    // Vòng lặp để xác định ngày, tháng, năm
    // Lặp khi số ngày còn lại lớn hơn số ngày trong tháng hiện tại
    while (_ng > arrNgayCuaThang[idxN][_th])
    {
        // Trừ đi số ngày của tháng hiện tại
        _ng -= arrNgayCuaThang[idxN][_th];
        // Tăng tháng lên
        _th++;
        
        // Nếu tháng vượt quá 12
        if (_th > 12)
        {
            _th = 1; // Quay về tháng 1
            _n++;    // Tăng năm lên
            idxN = LaNamNhuan(_n); // Tính lại chỉ số năm nhuận
        }
    }
}

// Hàm hủy
Ngay::~Ngay()
{
}

// Hàm xuất ngày tháng năm ra stream
void Ngay::Xuat(ostream& os)
{
    os << _ng << " - " << _th << " - " << _n;
}

//Hàm main
int main()
{
    Ngay n1;        // Mặc định: 1/1/1
    Ngay n2(2000);  // Ngày thứ 2000
    Ngay n3(3000);  // Ngày thứ 3000
    Ngay n4(5000);  // Ngày thứ 5000
    Ngay n5(50000); // Ngày thứ 50000

    // Xuất các đối tượng ra console
    n1.Xuat(cout);
    cout << endl;

    n2.Xuat(cout);
    cout << endl;

    n3.Xuat(cout);
    cout << endl;

    n4.Xuat(cout);
    cout << endl;

    n5.Xuat(cout);
    cout << endl;

    // Dừng màn hình để xem kết quả
    system("pause");
    
    return 0;
}

#include <iostream>
#include <fstream>
#include <algorithm> // Dùng sort, find_if, min_element
#include <cmath>     // Dùng abs()
#include <iomanip>   // Dùng setw, setfill

using namespace std;

/*
 * LỚP NGAYGIO (Dựa trên ảnh đề thi)
 * Mục tiêu: Logic chuẩn hóa siêu phức tạp và nạp chồng TOÀN DIỆN
 * các toán tử để dùng với <algorithm>.
 */
class NgayGio {
private:
    int ngay, thang, nam, gio;

    // --- Hàm tiện ích Static (giống HDTH_Tuan02) ---
    static bool laNamNhuan(int n) {
        return (n % 400 == 0) || (n % 4 == 0 && n % 100 != 0);
    }

    static int soNgayTrongThang(int m, int n) {
        if (m == 2) {
            return laNamNhuan(n) ? 29 : 28;
        } else if (m == 4 || m == 6 || m == 9 || m == 11) {
            return 30;
        }
        return 31;
    }

    // --- Hàm Chuẩn Hóa ---
    // Đây là logic khó nhất, chuẩn hóa từ thấp lên cao (giờ -> năm)
    void chuanHoa() {
        // 1. Xử lý giá trị âm (theo đề ng1(20, 30, -2113))
        // Giả định: năm âm -> dương, tháng/ngày/giờ âm -> 1
        // (Đây là logic an toàn, hoặc có thể quy đổi hết)
        // Kịch bản dễ hơn (giống HDTH_Tuan01): Chỉ xử lý số dương
        if (nam <= 0) nam = 1;
        if (thang <= 0) thang = 1;
        if (ngay <= 0) ngay = 1;
        if (gio < 0) gio = 0;

        // 2. Chuẩn hóa GIỜ -> NGÀY
        ngay += gio / 24;
        gio = gio % 24;

        // 3. Chuẩn hóa THÁNG -> NĂM
        // (Làm trước khi chuẩn hóa ngày để biết năm chính xác)
        if (thang > 12) {
            nam += (thang - 1) / 12;
            thang = (thang - 1) % 12 + 1;
        }

        // 4. Chuẩn hóa NGÀY -> THÁNG
        // Phải dùng vòng lặp vì có thể cộng dồn nhiều tháng
        while (ngay > soNgayTrongThang(thang, nam)) {
            ngay -= soNgayTrongThang(thang, nam);
            thang++;
            if (thang > 12) {
                thang = 1;
                nam++;
            }
        }
    }

public:
    // --- Constructors ---
    NgayGio() : ngay(1), thang(1), nam(1), gio(0) {}
    
    // Constructor (ngày, giờ, năm) - giống ng1(20, 30, -2113)
    NgayGio(int d, int h, int y) : ngay(d), thang(1), nam(y), gio(h) {
        chuanHoa();
    }
    
    // Constructor (ngày, giờ) - giống ng3(56, 762)
    NgayGio(int d, int h) : ngay(d), thang(1), nam(1), gio(h) {
        chuanHoa();
    }

    // Constructor (giờ) - giống ng4(4864)
    NgayGio(int h) : ngay(1), thang(1), nam(1), gio(h) {
        chuanHoa();
    }
    
    // Constructor đầy đủ (cho mục đích đọc file)
    NgayGio(int d, int m, int y, int h) : ngay(d), thang(m), nam(y), gio(h) {
        chuanHoa();
    }
    
    // --- Getters (cần cho find_if) ---
    int getThang() const { return thang; }
    int getNam() const { return nam; }

    // --- TOÁN TỬ ---

    // 1. Toán tử Nhập/Xuất (friend)
    friend ostream& operator<<(ostream& os, const NgayGio& ng) {
        // "xuất ra định dạng x/x-x"
        os << ng.ngay << "/" << ng.thang << "/" << ng.nam << "-" 
           << setfill('0') << setw(2) << ng.gio << "h";
        return os;
    }

    // Đọc 4 giá trị từ file
    friend istream& operator>>(istream& is, NgayGio& ng) {
        is >> ng.ngay >> ng.thang >> ng.nam >> ng.gio;
        ng.chuanHoa();
        return is;
    }
    
    // 2. Toán tử So sánh (cho sort, min_element)
    // Phải so sánh từ đơn vị lớn nhất (năm)
    bool operator<(const NgayGio& other) const {
        if (nam != other.nam) return nam < other.nam;
        if (thang != other.thang) return thang < other.thang;
        if (ngay != other.ngay) return ngay < other.ngay;
        return gio < other.gio;
    }
    
    bool operator>(const NgayGio& other) const {
        return other < *this; // Tái sử dụng operator<
    }

    // 3. Toán tử Cộng (theo đề "Lưu ý +int áp dụng lên giờ")
    // Dạng: ng1 + 3345
    NgayGio operator+(int gioThem) const {
        NgayGio temp = *this; // Dùng copy constructor
        temp.gio += gioThem;
        temp.chuanHoa();
        return temp;
    }

    // Dạng: 10000 + ng4 (bắt buộc friend)
    friend NgayGio operator+(int gioThem, const NgayGio& ng) {
        return ng + gioThem; // Tái sử dụng operator+ ở trên
    }

    // 4. Toán tử Tăng (Prefix ++ng1)
    NgayGio& operator++() {
        this->gio++;
        this->chuanHoa();
        return *this; // Trả về tham chiếu (return-by-reference)
    }

    // 5. Toán tử Tăng (Postfix ng1++)
    // Tham số int dummy để phân biệt
    NgayGio operator++(int) {
        NgayGio temp = *this; // Tạo bản sao giá trị CŨ
        this->gio++;          // Tăng giá trị
        this->chuanHoa();
        return temp;          // Trả về bản sao CŨ (return-by-value)
    }
};

// --- Hàm cho std::find_if ---
// "Tìm ng đầu tiên có số tháng > 11" (tức là tháng 12)
bool funcFindIf(const NgayGio& ng) {
    return ng.getThang() > 11;
}

// --- Hàm main (Mô phỏng file ảnh) ---
int main() {
    // 1d: Khởi tạo
    NgayGio ng1(20, 30, 2024);     // 20 ngày, 30 giờ, năm 2024
    NgayGio ng3(56, 762);         // 56 ngày, 762 giờ (năm 1)
    NgayGio ng4(4864);            // 4864 giờ (năm 1)

    cout << "--- 1d: Khoi tao object ---" << endl;
    cout << "ng1: " << ng1 << endl; // 21/1/2024-6h
    cout << "ng3: " << ng3 << endl; // 26/3/1-18h
    cout << "ng4: " << ng4 << endl; // 22/7/1-16h

    // 1.5d: Dùng mảng và +int
    NgayGio mang[100]; // Mảng C-style
    int n = 0;
    
    mang[n++] = ng1;
    mang[n++] = ng3;
    mang[n++] = ng4;
    mang[n++] = ng1 + 3345;      // Gọi operator+
    mang[n++] = 10000 + ng4;    // Gọi friend operator+

    cout << "\n--- Mang truoc khi sort ---" << endl;
    for (int i = 0; i < n; i++) cout << mang[i] << endl;

    // 1.5d: Sắp xếp
    sort(mang, mang + n); // Gọi operator<

    cout << "\n--- Mang sau khi sort ---" << endl;
    for (int i = 0; i < n; i++) cout << mang[i] << endl;

    // 1.5d: Tìm ng đầu tiên có tháng > 11
    NgayGio* pTimThay = find_if(mang, mang + n, funcFindIf);
    
    cout << "\n--- Tim (find_if) thang > 11 ---" << endl;
    if (pTimThay != mang + n) {
        cout << "Tim thay: " << *pTimThay << endl;
    } else {
        cout << "Khong tim thay." << endl;
    }

    // 1.5d: Tìm ng nhỏ nhất (ngNhoNhat)
    // Cách 1: Dùng min_element (chuẩn)
    NgayGio* pMin = min_element(mang, mang + n); // Gọi operator<
    
    // Cách 2: Dùng for_each (như trong ảnh)
    NgayGio ngMin = mang[0]; // Giả sử
    // for_each(mang + 1, mang + n, [&](const NgayGio& ng) {
    //    if (ng < ngMin) ngMin = ng;
    // });
    // (Lưu ý: Lambda [...] có thể không được phép,
    // nên dùng std::min_element là chuẩn nhất)

    cout << "\n--- Tim (min_element) ---" << endl;
    cout << "Nho nhat (auto): " << *pMin << endl;

    return 0;
}

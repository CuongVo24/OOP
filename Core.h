#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <exception>
#include <sstream>

using namespace std;

// ================================================================
// MODULE 1: CUSTOM EXCEPTION (Xử lý lỗi - Tuần 10)
// ----------------------------------------------------------------
// Chức năng: Dùng để ném ngoại lệ kèm thông báo chi tiết.
// Cách dùng: throw MyException("Loi nhap sai du lieu!");
// ================================================================
class MyException : public exception {
private:
    string _msg;
public:
    MyException(string msg) : _msg(msg) {}
    
    // Ghi đè hàm what() chuẩn của C++
    const char* what() const throw() override {
        return _msg.c_str();
    }
};

// ================================================================
// MODULE 2: DATE UTILITY (Xử lý ngày tháng - Tiện ích)
// ----------------------------------------------------------------
// Chức năng: Quản lý ngày tháng, tự động kiểm tra tính hợp lệ.
// Cách dùng: Khai báo Date d; cin >> d; cout << d;
// ================================================================
class Date {
private:
    int d, m, y;
public:
    Date() : d(1), m(1), y(2000) {}
    Date(int day, int month, int year) : d(day), m(month), y(year) {}

    // Hàm kiểm tra nhuận
    bool isLeap() const {
        return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    }

    // Nạp chồng toán tử nhập >>
    friend istream& operator>>(istream& is, Date& date) {
        char sep; // Ký tự phân cách (/, -, .)
        is >> date.d >> sep >> date.m >> sep >> date.y;
        if (date.d < 1 || date.m < 1 || date.m > 12 || date.d > 31) {
            // Ném ngoại lệ nếu ngày sai (Tuần 10)
            throw MyException("Ngay thang khong hop le!"); 
        }
        return is;
    }

    // Nạp chồng toán tử xuất <<
    friend ostream& operator<<(ostream& os, const Date& date) {
        os << (date.d < 10 ? "0" : "") << date.d << "/"
           << (date.m < 10 ? "0" : "") << date.m << "/" << date.y;
        return os;
    }
};

// ================================================================
// MODULE 3: BASE OBJECT (Khung sườn Đa hình - Tuần 7, 8)
// ----------------------------------------------------------------
// Chức năng: Class cha của mọi class. Quy định chuẩn Nhập/Xuất.
// Cách dùng: Class NhanVien : public BaseObject { ... }
// ================================================================
class BaseObject {
public:
    // 1. Virtual Destructor (BẮT BUỘC): Để delete con trỏ cha mà hủy được con
    virtual ~BaseObject() {}

    // 2. Hàm ảo thuần túy Nhập/Xuất: Bắt buộc con phải viết lại
    virtual void Nhap(istream& is) = 0;
    virtual void Xuat(ostream& os) const = 0;

    // 3. Hàm trả về loại đối tượng (Hữu ích khi đọc file phân loại)
    // VD: return 1 là NhanVienA, return 2 là NhanVienB
    virtual int GetType() const { return 0; }
};

// ================================================================
// MODULE 4: IO WRAPPER (Nạp chồng toán tử Đa hình)
// ----------------------------------------------------------------
// Chức năng: Giúp cin/cout gọi được hàm ảo của đối tượng
// Cách dùng: 
//    BaseObject* obj = new NhanVien();
//    cin >> *obj; (Tự động gọi NhanVien::Nhap)
//    cout << *obj; (Tự động gọi NhanVien::Xuat)
// ================================================================

// Wrapper cho operator >> (Gọi hàm Nhap ảo)
istream& operator>>(istream& is, BaseObject& obj) {
    obj.Nhap(is);
    return is;
}

// Wrapper cho operator << (Gọi hàm Xuat ảo)
ostream& operator<<(ostream& os, const BaseObject& obj) {
    obj.Xuat(os);
    return os;
}

// Hàm hỗ trợ xóa bộ nhớ đệm (Tránh trôi lệnh khi nhập chuỗi sau số)
void ClearBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

#endif // CORE_H

#include "Core.h" // <--- Gắn mảnh Lego số 1 vào

class NhanVien : public BaseObject { // Kế thừa ngay BaseObject
protected:
    string hoTen;
    Date ngaySinh; // Dùng luôn class Date của Core.h
public:
    // Override hàm Nhap của BaseObject
    void Nhap(istream& is) override {
        cout << "Nhap ten: "; 
        ClearBuffer(); // Hàm tiện ích trong Core.h
        getline(is, hoTen);
        cout << "Nhap ngay sinh (dd/mm/yyyy): ";
        is >> ngaySinh; // Dùng operator>> của Date
    }

    // Override hàm Xuat của BaseObject
    void Xuat(ostream& os) const override {
        os << "Ten: " << hoTen << ", NS: " << ngaySinh;
    }
};


int main() {
    try {
        // Khai báo con trỏ lớp cha BaseObject
        BaseObject* nv = new NhanVien(); 
        
        // NHỜ CÓ MODULE 4 (IO WRAPPER), BẠN VIẾT ĐƯỢC THẾ NÀY:
        cin >> *nv;  // Tự động gọi NhanVien::Nhap
        cout << *nv; // Tự động gọi NhanVien::Xuat

        delete nv; // Tự động gọi Virtual Destructor trong Core.h
    }
    catch (exception& e) {
        // NHỜ CÓ MODULE 1 (EXCEPTION), BẮT LỖI NGAY TẠI ĐÂY
        cout << "Loi xay ra: " << e.what() << endl;
    }
    return 0;
}

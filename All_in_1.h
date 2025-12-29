#ifndef ALL_IN_ONE_H
#define ALL_IN_ONE_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <exception>
#include <iomanip>
#include <fstream>

using namespace std;

// =========================================================================
// PHẦN 1: UTILITIES & EXCEPTION (CÔNG CỤ HỖ TRỢ)
// =========================================================================

// [DÙNG KHI]: Đề bài yêu cầu ném ngoại lệ (Exception) khi dữ liệu sai, quá tải...
// Ví dụ: throw AppError("Qua tai he thong!");
class AppError : public exception {
    string msg;
public:
    AppError(string m) : msg(m) {}
    const char* what() const throw() override { return msg.c_str(); }
};

// [DÙNG KHI]: Đề bài có Ngày/Tháng/Năm (Sinh nhật nhân viên, Ngày hóa đơn)
// Hỗ trợ nhập xuất dạng dd/mm/yyyy
class Date {
public:
    int d, m, y;
    Date(int D=1, int M=1, int Y=2000) : d(D), m(M), y(Y) {}
    
    friend istream& operator>>(istream& is, Date& date) {
        char sep; 
        is >> date.d >> sep >> date.m >> sep >> date.y;
        return is;
    }
    friend ostream& operator<<(ostream& os, const Date& date) {
        os << (date.d<10?"0":"") << date.d << "/" << (date.m<10?"0":"") << date.m << "/" << date.y;
        return os;
    }
};

// [DÙNG KHI]: Đọc file hoặc xử lý chuỗi phức tạp.
// Ví dụ: Input là "Light, 100" -> Cần tách ra để lấy "Light" và số 100.
class Utils {
public:
    // Cắt chuỗi theo ký tự phân cách (mặc định là dấu phẩy)
    static vector<string> Split(string s, char del = ',') {
        vector<string> res; string token; stringstream ss(s);
        while (getline(ss, token, del)) {
            // Xóa khoảng trắng thừa đầu đuôi (Trim)
            size_t first = token.find_first_not_of(" \t");
            if (string::npos == first) continue;
            size_t last = token.find_last_not_of(" \t");
            res.push_back(token.substr(first, (last - first + 1)));
        }
        return res;
    }
    // Chuyển chuỗi sang số an toàn (không bị crash nếu chuỗi lỗi)
    static double ToNum(string s) { try { return stod(s); } catch(...) { return 0; } }
};

// =========================================================================
// PHẦN 2: BASE OBJECT (NỀN TẢNG ĐA HÌNH)
// =========================================================================

// [DÙNG KHI]: Bắt đầu làm bài. MỌI CLASS (Nhân viên, File, Hàng hóa) ĐỀU PHẢI KẾ THỪA CLASS NÀY.
// Tác dụng: Có sẵn tên, nhập xuất đa hình, và hàm lấy giá trị chung.
class BaseObject {
protected:
    string _name; // Hầu hết đề thi đều có Tên hoặc Mã
public:
    BaseObject(string name = "") : _name(name) {}
    virtual ~BaseObject() {} // Bắt buộc để tránh Memory Leak

    // Hàm ảo nhập xuất (Con viết lại logic riêng)
    virtual void Nhap(istream& is) {} 
    virtual void Xuat(ostream& os) const { os << _name; }

    // [QUAN TRỌNG]: Hàm này giúp lấy Lương/Size/Giá trị mà không cần ép kiểu (dynamic_cast)
    // Nếu class con là File -> trả về Size. Nếu là Nhân viên -> Trả về Lương.
    virtual double GetValue() const { return 0; }
    
    string GetName() const { return _name; }
};

// [TỰ ĐỘNG]: Wrapper giúp cin >> *ptr và cout << *ptr gọi đúng hàm ảo của con
istream& operator>>(istream& is, BaseObject& obj) { obj.Nhap(is); return is; }
ostream& operator<<(ostream& os, const BaseObject& obj) { obj.Xuat(os); return os; }


// =========================================================================
// PHẦN 3: DESIGN PATTERNS (MẪU THIẾT KẾ)
// =========================================================================

// [DÙNG KHI]: Đề bài yêu cầu "Hệ thống duy nhất", "Quản lý tổng", "Logger".
// Cách dùng: class QuanLyDien : public Singleton<QuanLyDien> { ... }
template <class T>
class Singleton {
protected:
    Singleton() {}
    static T* _instance;
public:
    Singleton(const Singleton&) = delete;
    void operator=(const Singleton&) = delete;
    static T* GetInstance() {
        if (!_instance) _instance = new T();
        return _instance;
    }
};
template <class T> T* Singleton<T>::_instance = nullptr;


// [DÙNG KHI]: Đề bài dạng Cây/Tổ hợp (Thư mục chứa File, Thùng chứa Hộp, Nhóm thiết bị).
// Tác dụng: Tự động quản lý danh sách con, tự động hủy bộ nhớ, tự động hỗ trợ for-loop STL.
// Cách dùng: class Folder : public BaseObject, public CompositeNode { ... }
class CompositeNode {
protected:
    vector<BaseObject*> _children;
public:
    virtual ~CompositeNode() {
        for (auto c : _children) delete c; // Tự động dọn dẹp bộ nhớ
        _children.clear();
    }

    // Thêm con vào danh sách
    virtual void Add(BaseObject* obj) { if (obj) _children.push_back(obj); }

    // [SIÊU MẠNH]: Hỗ trợ Iterator để dùng được các hàm STL (std::count_if, for-each)
    // Giúp class của bạn chạy được vòng lặp: for (auto x : folder) { ... }
    typedef vector<BaseObject*>::iterator iterator;
    typedef vector<BaseObject*>::const_iterator const_iterator;
    iterator begin() { return _children.begin(); }
    iterator end() { return _children.end(); }
    const_iterator begin() const { return _children.begin(); }
    const_iterator end() const { return _children.end(); }
};

// =========================================================================
// PHẦN 4: MACROS (CODE NHANH - CHEAT CODE)
// =========================================================================

// [DÙNG KHI]: Muốn tạo Constructor nhanh có Tên và 1 Giá trị (Size, Lương...)
// VD: DECLARE_CTOR(File, _size)  -> Sinh ra: File(string n, double v) : BaseObject(n), _size(v) {}
#define DECLARE_CTOR(ClassName, ValueVar) \
    ClassName(string n, double v) : BaseObject(n), ValueVar(v) {}

// [DÙNG KHI]: Đề bắt cộng số vào đối tượng (VD: File + 10, 10 + File)
// VD: ENABLE_OP_PLUS_INT(File, _size)
#define ENABLE_OP_PLUS_INT(ClassName, ValueVar) \
    ClassName operator+(int val) { ClassName t = *this; t.ValueVar += val; return t; } \
    friend ClassName operator+(int val, const ClassName& obj) { ClassName t = obj; t.ValueVar += val; return t; }

// [DÙNG KHI]: Đề bắt dùng toán tử ++ (Tăng tiền tố và hậu tố)
// VD: ENABLE_OP_INC(File, _size)
#define ENABLE_OP_INC(ClassName, ValueVar) \
    ClassName& operator++() { this->ValueVar++; return *this; } \
    ClassName operator++(int) { ClassName t = *this; this->ValueVar++; return t; }

#endif // ALL_IN_ONE_H

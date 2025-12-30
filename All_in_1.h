#ifndef ALL_IN_ONE_H
#define ALL_IN_ONE_H

// =========================================================================
// KHU VỰC 0: THƯ VIỆN & CẤU HÌNH (BẮT BUỘC)
// =========================================================================
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <exception>
#include <iomanip>
#include <fstream>
#include <stdexcept>
#include <cmath>

using namespace std;

// =========================================================================
// KHU VỰC 1: CÔNG CỤ CƠ BẢN (UTILS & EXCEPTION)
// [TƯ DUY]: Đây là "Dao & Thớt" để sơ chế dữ liệu trước khi nấu.
// =========================================================================

// 1.1. XỬ LÝ LỖI (AppError)
// [DÙNG KHI]: Đề yêu cầu "Ném ngoại lệ", "Báo lỗi", "Dữ liệu sai".
// [CÁCH DÙNG]: throw AppError("Loi qua tai!");
class AppError : public exception {
    string msg;
public:
    AppError(string m) : msg(m) {}
    const char* what() const throw() override { return msg.c_str(); }
};

// 1.2. XỬ LÝ CHUỖI & SỐ (Utils)
// [DÙNG KHI]: Đề cho input dạng chuỗi phức tạp VD: "Laptop, 1500, 2kg".
// [CÁCH DÙNG]: vector<string> data = Utils::Split(s, ',');
class Utils {
public:
    // Cắt chuỗi và tự động xóa khoảng trắng thừa
    static vector<string> Split(string s, char del = ',') {
        vector<string> res; string token; stringstream ss(s);
        while (getline(ss, token, del)) {
            size_t first = token.find_first_not_of(" \t\r\n");
            if (string::npos == first) continue;
            size_t last = token.find_last_not_of(" \t\r\n");
            res.push_back(token.substr(first, (last - first + 1)));
        }
        return res;
    }
    // Chuyển chuỗi sang số an toàn (tránh crash chương trình)
    static double ToNum(string s) {
        try { return stod(s); } catch (...) { return 0; }
    }
};

// 1.3. XỬ LÝ NGÀY THÁNG (Date)
// [DÙNG KHI]: Đề bài có thông tin ngày sinh, ngày hết hạn...
class Date {
public:
    int d, m, y;
    Date(int D = 1, int M = 1, int Y = 2000) : d(D), m(M), y(Y) {}
    // Nhập dạng d/m/y
    friend istream& operator>>(istream& is, Date& date) {
        char sep; is >> date.d >> sep >> date.m >> sep >> date.y; return is;
    }
    // Xuất dạng dd/mm/yyyy
    friend ostream& operator<<(ostream& os, const Date& date) {
        os << setfill('0') << setw(2) << date.d << "/" << setw(2) << date.m << "/" << date.y;
        return os;
    }
};

// =========================================================================
// KHU VỰC 2: NỀN TẢNG ĐA HÌNH (BASE OBJECT)
// [TƯ DUY]: Mọi đối tượng trong đề (Dù là Xe, Hàng, Người) đều là BaseObject.
// =========================================================================

class BaseObject {
protected:
    string _name; // Hầu hết đề thi đều cần Tên hoặc Mã
public:
    BaseObject(string name = "") : _name(name) {}
    virtual ~BaseObject() {} // [QUAN TRỌNG]: Để tránh rò rỉ bộ nhớ khi delete cha

    // Hàm ảo Nhập/Xuất (Con sẽ viết chi tiết lại)
    virtual void Nhap(istream& is) {}
    virtual void Xuat(ostream& os) const { os << _name; }

    // [VŨ KHÍ BÍ MẬT]: Hàm đa năng lấy giá trị
    // - Nếu là File -> Trả về Size
    // - Nếu là Nhân viên -> Trả về Lương
    // - Giúp tính tổng mà KHÔNG CẦN ép kiểu (dynamic_cast) liên tục.
    virtual double GetValue() const { return 0; }

    string GetName() const { return _name; }
};

// Wrapper để cin/cout gọi thẳng vào hàm ảo của đối tượng
inline istream& operator>>(istream& is, BaseObject& obj) { obj.Nhap(is); return is; }
inline ostream& operator<<(ostream& os, const BaseObject& obj) { obj.Xuat(os); return os; }

// =========================================================================
// KHU VỰC 3: MẪU THIẾT KẾ (PATTERNS)
// [TƯ DUY]: Giải quyết các bài toán cấu trúc phức tạp.
// =========================================================================

// 3.1. SINGLETON (Quản lý duy nhất)
// [DÙNG KHI]: Thấy từ khóa "Hệ thống", "Quản lý tổng", "Logger".
// [CÁCH DÙNG]: class Manager : public Singleton<Manager> { ... }
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

// 3.2. COMPOSITE NODE (Cấu trúc Cây/Chứa đựng)
// [DÙNG KHI]: Thấy quan hệ "Chứa đựng" (Giỏ hàng chứa Hàng, Folder chứa File).
// [LỢI ÍCH]: Tự động quản lý bộ nhớ, Tự động hỗ trợ vòng lặp for (STL).
// [CÁCH DÙNG]: class Folder : public BaseObject, public CompositeNode { ... }
class CompositeNode {
protected:
    vector<BaseObject*> _children;
public:
    // Destructor tự động dọn dẹp con cái -> Không lo Memory Leak
    virtual ~CompositeNode() {
        for (auto c : _children) delete c;
        _children.clear();
    }

    // Hàm thêm con cơ bản
    virtual void Add(BaseObject* obj) { if (obj) _children.push_back(obj); }

    // Iterator Adapter -> Giúp class dùng được với sort, count_if, for-each
    typedef vector<BaseObject*>::iterator iterator;
    typedef vector<BaseObject*>::const_iterator const_iterator;
    iterator begin() { return _children.begin(); }
    iterator end() { return _children.end(); }
    const_iterator begin() const { return _children.begin(); }
    const_iterator end() const { return _children.end(); }
};

// =========================================================================
// KHU VỰC 4: CHEAT CODES (MACROS)
// [TƯ DUY]: Giảm thời gian gõ code cho những việc lặp lại nhàm chán.
// =========================================================================

// 4.1. TẠO CONSTRUCTOR NHANH
// [CÁCH DÙNG]: DECLARE_CTOR(File, _size)
// -> Sinh ra: File(string n, double v) : BaseObject(n), _size(v) {}
#define DECLARE_CTOR(ClassName, ValueVar) \
    ClassName(string n, double v) : BaseObject(n), ValueVar(v) {}

// 4.2. TẠO OPERATOR + (Cộng số)
// [CÁCH DÙNG]: ENABLE_OP_PLUS_INT(File, _size)
// -> Sinh ra code cho phép: file + 10  VÀ  10 + file
#define ENABLE_OP_PLUS_INT(ClassName, ValueVar) \
    ClassName operator+(int val) { ClassName t = *this; t.ValueVar += val; return t; } \
    friend ClassName operator+(int val, const ClassName& obj) { ClassName t = obj; t.ValueVar += val; return t; }

// 4.3. TẠO OPERATOR ++ (Tăng giảm)
// [CÁCH DÙNG]: ENABLE_OP_INC(File, _size)
// -> Sinh ra code cho phép: ++file  VÀ  file++
#define ENABLE_OP_INC(ClassName, ValueVar) \
    ClassName& operator++() { this->ValueVar++; return *this; } \
    ClassName operator++(int) { ClassName t = *this; this->ValueVar++; return t; }

#endif // ALL_IN_ONE_H

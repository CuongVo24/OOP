#include "AllInOne.h"
#include <set> // Dùng set để check trùng ID nhanh nhất (Full STL cho phép)

// ========================================================================
// BƯỚC 1: XÂY DỰNG SINGLETON QUẢN LÝ ID (HR MANAGER)
// Tư duy:
// - Khác với bài xe tải (check tổng tải trọng), bài này cần kiểm tra "Sự tồn tại".
// - Dùng std::set<string> để lưu danh sách ID đã đăng ký.
// - Nếu ID đã có trong set -> Ném Exception.
// ========================================================================

class HRManager : public Singleton<HRManager> {
    friend class Singleton<HRManager>;
    set<string> _registeredIDs; // Danh sách ID đã tồn tại

    HRManager() {} // Constructor Private

public:
    // Hàm đăng ký và kiểm tra trùng lặp
    void RegisterID(string id) {
        // count trả về 1 nếu đã có, 0 nếu chưa
        if (_registeredIDs.count(id)) {
            throw AppError("TRUNG MA NHAN VIEN: " + id + " da ton tai trong he thong!");
        }
        _registeredIDs.insert(id);
    }
    
    // Hàm reset (nếu cần cho test case mới)
    void Clear() { _registeredIDs.clear(); }
};

// ========================================================================
// BƯỚC 2: ĐỊNH NGHĨA INTERFACE CHUNG CHO NHÂN SỰ
// Tư duy:
// - BaseObject trong AllInOne.h chỉ có GetValue().
// - Bài này cần tính Lương Trung Bình = Tổng Lương / Tổng Người.
// - Vì vậy ta cần thêm hàm GetCount() (Đếm số người).
// - Ta tạo class HRUnit kế thừa BaseObject để mở rộng thêm hàm này.
// ========================================================================

class HRUnit : public BaseObject {
public:
    HRUnit(string name) : BaseObject(name) {}
    
    // Hàm ảo mới: Đếm số lượng nhân viên thực tế
    // Mặc định trả về 0 (đối với các node trừu tượng), con sẽ override
    virtual int GetCount() const { return 0; }
};

// ========================================================================
// BƯỚC 3: XÂY DỰNG CLASS LÁ - NHÂN VIÊN (EMPLOYEE)
// Tư duy:
// - Nhân viên là đơn vị nhỏ nhất.
// - GetValue() -> Trả về Lương.
// - GetCount() -> Trả về 1 (Vì 1 object Employee là 1 người).
// ========================================================================

class Employee : public HRUnit {
private:
    string _id;
    double _salary;
public:
    // Constructor
    Employee(string id, string name, double salary) 
        : HRUnit(name), _id(id), _salary(salary) {}

    // Override GetValue để trả về Lương (phục vụ tính tổng lương)
    double GetValue() const override { return _salary; }

    // Override GetCount để trả về 1 (phục vụ tính trung bình)
    int GetCount() const override { return 1; }

    void Xuat(ostream& os) const override {
        os << "[NV] " << _id << " - " << _name << " ($" << _salary << ")";
    }
};

// ========================================================================
// BƯỚC 4: XÂY DỰNG CLASS COMPOSITE - PHÒNG BAN (DEPARTMENT)
// Tư duy:
// - Phòng ban chứa nhân viên hoặc phòng ban con.
// - Khi Add nhân viên: Phải gọi HRManager để check trùng ID.
// - Tính Tổng Lương: Duyệt con cộng dồn GetValue() (Logic cũ).
// - Tính Tổng Người: Duyệt con cộng dồn GetCount() (Logic mới).
// ========================================================================

class Department : public HRUnit, public CompositeNode {
public:
    Department(string name) : HRUnit(name) {}

    // 1. Tính Tổng Lương (Cộng dồn đệ quy)
    double GetValue() const override {
        double totalSalary = 0;
        for (auto child : *this) {
            // Ép kiểu về HRUnit để gọi được GetValue (BaseObject đã có sẵn)
            totalSalary += child->GetValue();
        }
        return totalSalary;
    }

    // 2. Tính Tổng Số Nhân Viên (Logic quan trọng để tính trung bình)
    int GetCount() const override {
        int totalPeople = 0;
        for (auto child : *this) {
            // Cần ép kiểu sang HRUnit* để gọi hàm GetCount()
            // Vì _children lưu BaseObject* (không có hàm GetCount)
            HRUnit* unit = dynamic_cast<HRUnit*>(child);
            if (unit) {
                totalPeople += unit->GetCount();
            }
        }
        return totalPeople;
    }

    // 3. Hàm tính lương trung bình
    double GetAverageSalary() const {
        int count = GetCount();
        if (count == 0) return 0;
        return GetValue() / count;
    }

    // 4. Override Add để check trùng ID
    // Lưu ý: BaseObject không có ID, nên ta chỉ check nếu obj là Employee
    void Add(BaseObject* obj) override {
        // Thử xem obj có phải là Nhân viên không?
        // (Trong thiết kế này ID nằm ở Employee, nên ta không check ID của Department)
        // Tuy nhiên Factory tạo Employee đã check rồi (xem Bước 5), 
        // nhưng để an toàn tuyệt đối ("Phòng thủ 2 lớp"), ta check lại cũng được.
        // Ở đây tôi để Factory check để code gọn.
        
        if (obj) CompositeNode::Add(obj);
    }

    void Xuat(ostream& os) const override {
        os << "[PHONG] " << _name 
           << " { Tong Luong: " << GetValue() 
           << ", Nhan su: " << GetCount() 
           << ", TB: " << GetAverageSalary() << " }";
           
        for (auto c : *this) {
            os << "\n    |-- " << *c;
        }
    }
};

// ========================================================================
// BƯỚC 5: FACTORY - XỬ LÝ CHUỖI & CHECK ID SỚM
// Tư duy:
// - Input: "NV01, Nguyen Van A, 1000"
// - Trước khi new Employee, phải gọi HRManager check "NV01" ngay lập tức.
// - Nếu trùng -> Ném lỗi luôn, không tạo đối tượng -> Không rò rỉ bộ nhớ.
// ========================================================================

class EmpFactory {
public:
    static BaseObject* Create(string s) {
        // Cắt chuỗi
        vector<string> data = Utils::Split(s, ',');
        
        if (data.size() >= 3) {
            string id = data[0];
            string name = data[1];
            double salary = Utils::ToNum(data[2]);

            // [QUAN TRỌNG]: Check ID trước khi tạo
            // Nếu trùng, hàm này ném AppError, dòng return bên dưới không chạy -> An toàn
            HRManager::GetInstance()->RegisterID(id);

            return new Employee(id, name, salary);
        }
        return nullptr;
    }
};




// MAIN.CPP
#include "Solutions.cpp"

int main() {
    try {
        // 1. Singleton: HRManager
        // Nhiệm vụ: Không cho phép 2 nhân viên trùng Mã Số
        // BẪY: Phải dùng vector/set để lưu history các ID đã add.
        HRManager::GetInstance()->RegisterID("NV01");
        HRManager::GetInstance()->RegisterID("NV02");

        // 2. Composite: PhongBan (Department)
        Department* devDept = new Department("Phong Dev");

        // 3. Leaf: NhanVien (Employee)
        // Format: "NV01, Nguyen Van A, 1000" (Ma, Ten, Luong)
        // BẪY: BaseObject chỉ có _name. Bạn cần tách Ma và Ten ra sao? 
        // Gợi ý: _name lưu "Nguyen Van A", thêm thuộc tính _id.
        Employee* e1 = EmpFactory::Create("NV03, Tran B, 2000"); // Đăng ký ID NV03 luôn trong Factory
        devDept->Add(e1);
        
        // Thử add trùng ID đã có
        // Employee* eFail = EmpFactory::Create("NV01, Fake A, 500"); -> Phải ném Exception tại đây

        // 4. STL Algorithm: Tìm nhân viên lương cao nhất
        // BẪY: Phải viết hàm begin()/end() trong CompositeNode để chạy for
        cout << "Nhan vien luong cao nhat phong Dev: ";
        Employee* bestEmp = nullptr;
        double maxSal = -1;
        
        for (auto child : *devDept) {
            if (child->GetValue() > maxSal) {
                maxSal = child->GetValue();
                // bestEmp = child; // Cần ép kiểu dynamic_cast an toàn
            }
        }
        // In bestEmp...

        // 5. LOGIC KHÓ: Tính Lương Trung Bình của Phòng
        // GetValue() trả về Tổng Lương.
        // Cần thêm hàm CountMembers() để chia.
        cout << "Tong luong: " << devDept->GetValue() << endl;
        cout << "Luong trung binh: " << devDept->GetValue() / devDept->CountMembers() << endl;

    } catch (exception& e) {
        cout << "LOI NHAN SU: " << e.what() << endl;
    }
    return 0;
}

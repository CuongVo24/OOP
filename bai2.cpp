#include "AllInOne.h"
#include <set> // [FULL STL]: Dùng set để check trùng ID nhanh nhất

// =========================================================================
// KHU VỰC 1: SINGLETON - QUẢN LÝ MÃ NHÂN VIÊN (HR MANAGER)
// [MAP TO MAIN]: HRManager::GetInstance()->RegisterID("NV01");
// [TƯ DUY]: Khác với bài Mạch điện (check Max), bài này check "Sự tồn tại".
// =========================================================================
class HRManager : public Singleton<HRManager> {
    friend class Singleton<HRManager>;
    set<string> _registeredIDs; // Danh sách ID đã tồn tại

    HRManager() {}

public:
    // [MAP TO MAIN]: Logic check trùng lặp
    void RegisterID(string id) {
        // count trả về 1 nếu đã có -> Ném lỗi ngay
        if (_registeredIDs.count(id)) {
            throw AppError("TRUNG MA NHAN VIEN: " + id + " da ton tai!");
        }
        _registeredIDs.insert(id);
    }
};

// =========================================================================
// KHU VỰC 2: INTERFACE TRUNG GIAN (HR UNIT)
// [TƯ DUY]: Tại sao cần class này?
// - BaseObject chỉ có GetValue().
// - Bài này cần tính Lương Trung Bình = Tổng Lương / Tổng Người.
// - Nên ta cần thêm hàm ảo GetCount() để đếm số người.
// =========================================================================
class HRUnit : public BaseObject {
public:
    HRUnit(string name) : BaseObject(name) {}
    
    // Hàm ảo mới: Đếm số lượng nhân sự
    // Mặc định trả về 0, class con sẽ override logic riêng
    virtual int GetCount() const { return 0; }
};

// =========================================================================
// KHU VỰC 3: CLASS LÁ - NHÂN VIÊN (EMPLOYEE)
// [MAP TO MAIN]: EmpFactory::Create("NV03, Tran B, 2000");
// =========================================================================
class Employee : public HRUnit {
private:
    string _id;
    double _salary;
public:
    Employee(string id, string name, double salary) 
        : HRUnit(name), _id(id), _salary(salary) {}

    // [MAP TO MAIN]: GetValue trả về Lương (để tính Tổng lương)
    double GetValue() const override { return _salary; }

    // [LOGIC MỚI]: 1 Object Nhân viên = 1 Người
    int GetCount() const override { return 1; }

    void Xuat(ostream& os) const override {
        os << "[NV] " << _id << " - " << _name << " ($" << _salary << ")";
    }
};

// =========================================================================
// KHU VỰC 4: COMPOSITE - PHÒNG BAN (DEPARTMENT)
// [MAP TO MAIN]: Department* devDept = new Department("Phong Dev");
// [TƯ DUY]: Phòng ban chứa Nhân viên hoặc Phòng ban con.
// =========================================================================
class Department : public HRUnit, public CompositeNode {
public:
    Department(string name) : HRUnit(name) {}

    // [LOGIC 1]: Tính Tổng Lương (Cộng dồn đệ quy)
    double GetValue() const override {
        double totalSalary = 0;
        for (auto child : *this) totalSalary += child->GetValue();
        return totalSalary;
    }

    // [LOGIC 2]: Tính Tổng Số Nhân Viên (Cộng dồn đệ quy)
    int GetCount() const override {
        int totalPeople = 0;
        for (auto child : *this) {
            // [BẪY ÉP KIỂU]: _children lưu BaseObject*, ta phải ép sang HRUnit* mới gọi được GetCount
            HRUnit* unit = dynamic_cast<HRUnit*>(child);
            if (unit) totalPeople += unit->GetCount();
        }
        return totalPeople;
    }

    // [MAP TO MAIN]: Tính Lương Trung Bình = Tổng Tiền / Tổng Người
    double GetAverageSalary() const {
        int count = GetCount();
        if (count == 0) return 0;
        return GetValue() / count;
    }

    void Xuat(ostream& os) const override {
        os << "[PHONG] " << _name 
           << " { Tong: " << GetValue() 
           << ", Nhan su: " << GetCount() 
           << ", TB: " << GetAverageSalary() << " }";
        for (auto c : *this) os << "\n    |-- " << *c;
    }
};

// =========================================================================
// KHU VỰC 5: FACTORY - TẠO ĐỐI TƯỢNG AN TOÀN
// [MAP TO MAIN]: EmpFactory::Create("NV03, Tran B, 2000")
// =========================================================================
class EmpFactory {
public:
    static BaseObject* Create(string s) {
        // [XỬ LÝ CHUỖI]: Cắt dấu phẩy
        vector<string> data = Utils::Split(s, ',');
        
        if (data.size() >= 3) {
            string id = data[0];
            string name = data[1];
            double salary = Utils::ToNum(data[2]);

            // [QUAN TRỌNG]: Gọi Singleton check ID TRƯỚC khi new
            // Nếu trùng -> Ném lỗi -> Không new -> Không leak bộ nhớ
            HRManager::GetInstance()->RegisterID(id);

            return new Employee(id, name, salary);
        }
        return nullptr;
    }
};

// =========================================================================
// KHU VỰC 6: HÀM MAIN (ĐỀ BÀI)
// =========================================================================
/*
int main() {
    try {
        // 1. Singleton: Check trùng mã
        HRManager::GetInstance()->RegisterID("NV01");
        HRManager::GetInstance()->RegisterID("NV02");

        // 2. Composite: Phòng ban
        Department* devDept = new Department("Phong Dev");

        // 3. Factory: Tạo nhân viên an toàn
        // (NV03 được check và add vào Singleton ngay tại đây)
        Employee* e1 = (Employee*)EmpFactory::Create("NV03, Tran B, 2000"); 
        devDept->Add(e1);
        
        // 4. Test Lỗi Trùng Mã (Sẽ nhảy vào catch)
        // Employee* eFail = EmpFactory::Create("NV01, Fake A, 500");

        // 5. Tính toán
        cout << *devDept << endl;
        cout << "Luong TB: " << devDept->GetAverageSalary() << endl;

        // 6. Tìm nhân viên lương cao nhất (Duyệt bằng Iterator có sẵn)
        double maxSal = -1;
        for (auto child : *devDept) {
            if (child->GetValue() > maxSal) maxSal = child->GetValue();
        }
        cout << "Max Salary: " << maxSal << endl;

    } catch (exception& e) {
        cout << "[LOI NHAN SU]: " << e.what() << endl;
    }
    return 0;
}
*/

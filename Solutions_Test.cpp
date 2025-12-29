// Solution.cpp
#include "AllInOne.h"

// Không cần include vector, string, iostream nữa vì AllInOne.h đã có đủ.

// BƯỚC 2: XỬ LÝ SINGLETON (QUẢN LÝ TỔNG GIỜ LÀM)
// Tư duy:

// Đề bài có TaskManager::GetInstance()->SetMaxHours(...). -> Đây là Singleton.

// Nhiệm vụ: Quản lý tổng giờ làm (currentTotal) và giới hạn (maxHours).

// Logic nghiệp vụ: Trước khi thêm Task mới, phải kiểm tra xem currentTotal + newHours có vượt maxHours không. Nếu vượt -> Ném Exception.


// Kế thừa Singleton từ AllInOne.h để có ngay hàm GetInstance()
class TaskManager : public Singleton<TaskManager> {
    friend class Singleton<TaskManager>; // Để class cha truy cập được constructor private
private:
    double maxHours;
    double currentTotal;
    
    // Constructor Private (Chuẩn Singleton)
    TaskManager() : maxHours(0), currentTotal(0) {} 

public:
    void SetMaxHours(double h) { maxHours = h; }

    // Hàm kiểm tra logic nghiệp vụ (quan trọng nhất)
    void CheckAndCommit(double hours) {
        if (currentTotal + hours > maxHours) {
            // Dùng AppError có sẵn trong AllInOne.h
            throw AppError("Qua tai du an! Gioi han: " + to_string((int)maxHours) + 
                           "h. Hien tai: " + to_string((int)currentTotal) + 
                           "h. Them: " + to_string((int)hours) + "h.");
        }
        // Nếu không lỗi thì cộng dồn luôn
        currentTotal += hours;
    }
    
    // Hàm debug nếu cần
    double GetTotal() const { return currentTotal; }
};

// BƯỚC 3: XỬ LÝ ĐỐI TƯỢNG LÁ (TASK - CÔNG VIỆC)
// Tư duy:

// Task là đơn vị nhỏ nhất -> Kế thừa BaseObject.

// Thuộc tính: Tên (có sẵn trong BaseObject), Giờ làm (_hours).

// Yêu cầu đề bài: t1++ (tăng giờ), 5 + *t2 (cộng thêm giờ). -> Dùng Macro ENABLE_OP... trong AllInOne.h để đỡ phải viết tay 20 dòng code toán tử.

// Cần override GetValue() để trả về số giờ (phục vụ tính toán).

class Task : public BaseObject {
private:
    double _hours;
public:
    // 1. Dùng MACRO sinh Constructor: Task(string n, double v)
    DECLARE_CTOR(Task, _hours)

    // 2. Override hàm lấy giá trị (để hệ thống tính tổng)
    double GetValue() const override { return _hours; }

    // 3. Override hàm Xuat để in ra màn hình
    void Xuat(ostream& os) const override {
        os << "[Task] " << _name << " (" << _hours << "h)";
    }

    // 4. Dùng MACRO sinh Operator ++ (tăng 1h) và + (cộng thêm h)
    ENABLE_OP_INC(Task, _hours)      // Sinh ra: operator++
    ENABLE_OP_PLUS_INT(Task, _hours) // Sinh ra: operator+(int) và friend operator+(int, Task)
};

// BƯỚC 4: XỬ LÝ ĐỐI TƯỢNG COMPOSITE (PROJECT - DỰ ÁN)
// Tư duy:

// Project chứa nhiều Task hoặc Project con -> Kế thừa BaseObject VÀ CompositeNode.

// Logic quan trọng: Khi Add một thành phần vào Project, phải gọi TaskManager để kiểm tra xem có bị lố giờ không.

// CompositeNode trong AllInOne.h đã lo việc lưu trữ vector, Iterator (để duyệt for) và Destructor (xóa bộ nhớ). Mình chỉ cần lo logic Add và Xuat.

class Project : public BaseObject, public CompositeNode {
public:
    // Constructor đơn giản
    Project(string name) : BaseObject(name) {}

    // 1. Override hàm lấy giá trị: Tổng giờ dự án = Tổng giờ các con
    double GetValue() const override {
        double total = 0;
        // Dùng range-based for (nhờ Iterator của CompositeNode)
        for (auto child : *this) {
            total += child->GetValue(); // Đệ quy tự nhiên
        }
        return total;
    }

    // 2. Override hàm Add để chèn logic kiểm tra Singleton
    void Add(BaseObject* obj) override {
        if (obj) {
            // Kiểm tra với Ông Trùm Singleton trước
            // Lưu ý: obj->GetValue() sẽ lấy giờ của Task hoặc tổng giờ của Project con
            TaskManager::GetInstance()->CheckAndCommit(obj->GetValue());
            
            // Nếu ok thì mới thêm vào danh sách (gọi hàm cha)
            CompositeNode::Add(obj);
        }
    }

    // 3. Override hàm Xuat để in cây thư mục đẹp
    void Xuat(ostream& os) const override {
        os << "[PROJECT] " << _name << " (Total: " << GetValue() << "h)";
        // Duyệt danh sách con
        for (auto child : *this) {
            os << "\n    -> "; 
            // In đối tượng con (nhờ đa hình operator<< của BaseObject)
            os << *child; 
        }
    }
};

// BƯỚC 5: XỬ LÝ FACTORY (NHÀ MÁY TẠO ĐỐI TƯỢNG)
// Tư duy:

// Đề bài có: TaskFactory::Create("Design DB, 10").

// Dùng Utils::Split để cắt dấu phẩy.

// Dùng Utils::ToNum để chuyển chuỗi "10" thành số 10.0.

class TaskFactory {
public:
    static Task* Create(string config) {
        // Cắt chuỗi: "Ten, Gio"
        vector<string> parts = Utils::Split(config, ',');
        
        if (parts.size() >= 2) {
            string name = parts[0];          // Phần 1 là Tên
            double hours = Utils::ToNum(parts[1]); // Phần 2 là Giờ
            return new Task(name, hours);
        }
        return nullptr;
    }
};

// BƯỚC 6: KIỂM TRA LẠI VỚI MAIN (MENTAL CHECK)
// Chúng ta hãy đối chiếu Code vừa viết với hàm main của đề bài xem có khớp không:

// TaskManager::GetInstance()->SetMaxHours(100); -> OK (Có Singleton).

// Project* webProj = new Project(...) -> OK (Class Project).

// TaskFactory::Create(...) -> OK (Class Factory).

// backend->Add(t1++) -> OK (Macro ENABLE_OP_INC hỗ trợ ++).

// backend->Add(5 + (*t2)) -> OK (Macro ENABLE_OP_PLUS_INT hỗ trợ friend operator 5 + obj).

// webProj->Add(backend) -> OK (Project nhận BaseObject* nên nhận được cả Task và Project).

// cout << *webProj -> OK (BaseObject có operator<< gọi hàm ảo Xuat).

// catch (exception& e) -> OK (Class AppError kế thừa exception).

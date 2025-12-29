// MAIN.CPP (ĐỀ BÀI)
#include "AllInOne.h" 
// #include "Solution.cpp" // Sinh viên viết file này

int main() {
    try {
        // 1. TaskManager (Singleton): Quản lý tổng giờ làm
        TaskManager::GetInstance()->SetMaxHours(100); // Tối đa 100h

        // 2. Project (Composite): Dự án chứa Task con hoặc Dự án con
        Project* webProj = new Project("Web Redesign");
        Project* backend = new Project("Backend API");
        
        // 3. Task (Leaf): Công việc cụ thể
        // Factory tạo task từ chuỗi "TenTask, SoGio"
        Task* t1 = TaskFactory::Create("Design DB, 10"); 
        Task* t2 = TaskFactory::Create("Code API, 20");
        
        // 4. Test Operator:
        // t1++ : Tăng giờ làm lên 1h
        // 5 + t2: Tăng giờ làm thêm 5h (Friend operator)
        backend->Add(t1++); 
        backend->Add(5 + (*t2)); 

        webProj->Add(backend);
        webProj->Add(TaskFactory::Create("Frontend HTML, 15"));

        // 5. In ra cây dự án
        cout << *webProj << endl; 

        // 6. Test Exception: Thêm task làm lố giờ (Tổng đang là ~50h)
        // Thêm task 60h -> Tổng > 100h -> Báo lỗi
        webProj->Add(TaskFactory::Create("Testing, 60")); 

    } catch (exception& e) {
        cout << "LOI: " << e.what() << endl;
    }
    
    // Cleanup: delete webProj; (Composite tự hủy con)
    return 0;
}

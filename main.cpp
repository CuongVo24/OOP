#include <iostream>
#include <vector>
#include <string>
#include <fstream>  // Để đọc file rules.txt (nếu cần)
#include "lib.h"    // Kết nối với các class chúng ta đã viết

using namespace std;

// Hàm giả lập việc đọc file cấu hình "rules.txt"
// Trong thực tế, em sẽ dùng ifstream để đọc từng dòng từ file thật.
vector<string> readRuleConfig() {
    // Giả sử nội dung file rules.txt như đề bài yêu cầu [cite: 18-21]
    return {
        "NormalizeCasing",
        "RemoveSpecialCharacters", // Có thể thêm tham số Regex=[.-_\s] nếu class hỗ trợ
        "AddDatePrefix"
    };
}

int main() {
    // 1. Dữ liệu đầu vào (Input Data)
    // Lấy đúng theo ví dụ trong đề bài [cite: 7]
    vector<string> filenames = {
        "Nguyen Van Minh.pDF",
        "Tran Thi Lan.DOCX",
        "LE-HOANG ANH.Pdf",
        "PhAm DUC HUy.Docx" 
        // Lưu ý: Đề bài có dòng "PhAm DỤc HUy" có dấu tiếng Việt, 
        // xử lý dấu tiếng Việt trong C++ console khá phức tạp (cần wstring),
        // nên ở mức cơ bản ta dùng không dấu hoặc chấp nhận lỗi hiển thị font.
    };

    // 2. Khởi tạo UseCase (Bộ xử lý trung tâm)
    RenameCVFileUseCase useCase;

    // 3. Cấu hình luật (Configuration)
    // Thay vì hard-code: useCase.addRule(new NormalizeCasing()),
    // Ta dùng Factory để tạo luật động từ danh sách text.
    cout << "--- Loading Rules Configuration ---\n";
    vector<string> ruleConfigs = readRuleConfig();

    for (const string& ruleName : ruleConfigs) {
        // Gọi "Nhà máy" sản xuất ra đúng loại luật cần dùng
        IRule* rule = RuleFactory::createRule(ruleName);

        if (rule != nullptr) {
            useCase.addRule(rule);
            cout << "[OK] Added rule: " << ruleName << endl;
        } else {
            // Xử lý ngoại lệ nếu tên luật trong file config bị sai 
            cerr << "[ERROR] Unknown rule found: " << ruleName << endl;
        }
    }
    cout << "-----------------------------------\n\n";

    // 4. Thực thi (Execution)
    cout << "Renaming files...\n"; // Output đúng yêu cầu đề bài [cite: 12]
    
    for (const auto& origin : filenames) {
        try {
            // Gọi hàm rename của UseCase, nó sẽ tự chạy qua chuỗi các luật
            string renamed = useCase.rename(origin);
            
            // In kết quả: Tên cũ -> Tên mới
            cout << origin << " -> " << renamed << "\n";
        } 
        catch (const exception& e) {
            // Catch lỗi nếu có sự cố trong quá trình đổi tên
            cerr << "Error renaming file " << origin << ": " << e.what() << "\n";
        }
    }

    return 0;
    // Khi hàm main kết thúc, Destructor của useCase được gọi,
    // tự động dọn dẹp bộ nhớ các rule đã new.
}

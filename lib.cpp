#include "lib.h"
#include <iostream>
#include <algorithm> // cho transform
#include <regex>     // cho regex_replace
#include <ctime>     // cho time
#include <iomanip>   // cho put_time
#include <sstream>   // cho stringstream

using namespace std;

string NormalizeCasing::rename(const string& filename) {
    string result = filename;
    bool newWord = true;

    for (int i = 0; i < result.length(); ++i) {
        // Nếu gặp ký tự phân cách (không phải chữ và số), đánh dấu từ mới
        if (!isalnum(result[i])) {
            newWord = true;
            continue; 
        }

        if (newWord) {
            result[i] = toupper(result[i]);
            newWord = false;
        } else {
            result[i] = tolower(result[i]);
        }
    }
    return result;
}

string RemoveSpecialCharacters::rename(const string& filename) {
    // B1: Tách phần tên và phần mở rộng
    size_t lastDot = filename.find_last_of(".");
    string namePart = filename;
    string extPart = "";

    if (lastDot != string::npos) {
        namePart = filename.substr(0, lastDot);
        extPart = filename.substr(lastDot); // Bao gồm cả dấu chấm (VD: .pdf)
    }

    // B2: Định nghĩa Regex để tìm các ký tự cần xóa
    // Đề bài yêu cầu xóa: dấu gạch dưới, gạch ngang, khoảng trắng, dấu chấm [cite: 10, 20]
    // Regex: [_\-\s\.]
    regex specialChars(R"([_\-\s\.])"); 

    // B3: Thay thế các ký tự tìm thấy bằng chuỗi rỗng
    string cleanedName = regex_replace(namePart, specialChars, "");

    // B4: Ghép lại
    return cleanedName + extPart;
}

string AddDatePrefix::rename(const string& filename) {
    // Lấy thời gian hiện tại
    auto now = time(nullptr);
    auto tm = *localtime(&now);

    // Format string sử dụng stringstream
    stringstream ss;
    ss << put_time(&tm, "%Y%m%d") << "_"; // Định dạng yyyymmdd_
    
    return ss.str() + filename;
}

// ==========================================
// RuleFactory Implementation
// ==========================================
IRule* RuleFactory::createRule(const string& ruleType) {
    if (ruleType == "NormalizeCasing") {
        return new NormalizeCasing();
    } 
    else if (ruleType.find("RemoveSpecialCharacters") != string::npos) {
        // Lưu ý: Đề bài phần mở rộng có nhắc đến tham số Regex[cite: 20].
        // Ở mức độ cơ bản này, ta tạm thời trả về Default.
        // Nếu muốn nâng cao, ta cần tách chuỗi để lấy tham số Regex.
        return new RemoveSpecialCharacters();
    } 
    else if (ruleType.find("AddDatePrefix") != string::npos) {
        return new AddDatePrefix();
    }
    return nullptr; // Trả về null nếu không tìm thấy luật phù hợp
}

// ==========================================
// RenameCVFileUseCase Implementation
// ==========================================

void RenameCVFileUseCase::addRule(IRule* rule) {
    if (rule != nullptr) {
        rules.push_back(rule);
    }
}

string RenameCVFileUseCase::rename(string filename) {
    string currentName = filename;
    
    // Design Pattern: Pipeline
    // Output của luật trước là Input của luật sau
    for (IRule* rule : rules) {
        currentName = rule->rename(currentName);
    }
    
    return currentName;
}

// Destructor: Dọn dẹp bộ nhớ (Giải phóng các con trỏ đã new)
RenameCVFileUseCase::~RenameCVFileUseCase() {
    for (IRule* rule : rules) {
        delete rule;
    }
    rules.clear();
}

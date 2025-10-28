#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cmath>
using namespace std;

// Định nghĩa lớp SoNguyenLon
class SoNguyenLon
{
private:
    string so; // Biến lưu trữ số dưới dạng chuỗi

    // Biến tĩnh để lưu số lớn nhất
    static SoNguyenLon max_snl;

    // Hàm trợ giúp: so sánh (this > other)
    // Trả về true nếu số hiện tại > số other
    bool is_greater(const SoNguyenLon& other) const
    {
        if (so.length() > other.so.length())
            return true;
        if (so.length() < other.so.length())
            return false;
        // Nếu độ dài bằng nhau, so sánh chuỗi
        return so > other.so;
    }

    // Hàm trợ giúp: Cập nhật số max
    void update_max()
    {
        if (this->is_greater(max_snl))
        {
            max_snl = *this;
        }
    }

    // Hàm trợ giúp tĩnh: Cộng hai chuỗi số
    static string add_strings(string s1, string s2)
    {
        string result = "";
        int carry = 0;
        
        // Đảo chuỗi để cộng từ hàng đơn vị
        reverse(s1.begin(), s1.end());
        reverse(s2.begin(), s2.end());

        int n1 = s1.length(), n2 = s2.length();
        int max_len = max(n1, n2);

        for (int i = 0; i < max_len; i++)
        {
            // Lấy chữ số (hoặc 0 nếu chuỗi đã hết)
            int digit1 = (i < n1) ? (s1[i] - '0') : 0;
            int digit2 = (i < n2) ? (s2[i] - '0') : 0;
            
            int sum = digit1 + digit2 + carry;
            
            result.push_back((sum % 10) + '0');
            carry = sum / 10;
        }

        // Nếu còn số nhớ ở cuối
        if (carry)
        {
            result.push_back(carry + '0');
        }

        // Đảo ngược lại kết quả
        reverse(result.begin(), result.end());
        
        // Xử lý trường hợp 0 + 0 = ""
        if (result.empty()) return "0";
        
        return result;
    }

public:
    // 1. Hàm dựng mặc định
    SoNguyenLon()
    {
        so = "0";
    }

    // 2. Hàm dựng từ chuỗi (const char*)
    SoNguyenLon(const char* s)
    {
        so = s;
        update_max();
    }
    
    // Hàm dựng từ string (trợ giúp nội bộ)
    SoNguyenLon(const string& s)
    {
        so = s;
        update_max();
    }

    // 3. Hàm dựng sao chép
    SoNguyenLon(const SoNguyenLon& other)
    {
        so = other.so;
        // Không cần update_max vì đây là bản sao
    }

    // 4. Hàm dựng từ số nguyên (dùng long long cho an toàn)
    SoNguyenLon(long long n)
    {
        so = to_string(n);
        update_max();
    }
    
    // Hàm hủy
    ~SoNguyenLon() {}

    // 5. Toán tử gán
    SoNguyenLon& operator=(const SoNguyenLon& other)
    {
        so = other.so;
        // Không cần update_max vì gán bằng giá trị đã tồn tại
        return *this;
    }

    // 6. Hàm In
    void In(ostream& os)
    {
        os << so;
    }

    // 7. Hàm Cộng (với SoNguyenLon)
    SoNguyenLon Cong(const SoNguyenLon& other)
    {
        string sum_str = add_strings(this->so, other.so);
        return SoNguyenLon(sum_str); // Hàm dựng sẽ gọi update_max
    }

    // 8. Hàm Cộng (với long long)
    SoNguyenLon Cong(long long n)
    {
        string sum_str = add_strings(this->so, to_string(n));
        return SoNguyenLon(sum_str);
    }
    
    // 9. Hàm Cộng (với const char*)
    SoNguyenLon Cong(const char* s)
    {
        string sum_str = add_strings(this->so, string(s));
        return SoNguyenLon(sum_str);
    }

    // 10. Hàm TraVe (trả về tham số truyền vào)
    SoNguyenLon TraVe(const SoNguyenLon& other)
    {
        return other;
    }

    // 11. Hàm tĩnh trả về số lớn nhất
    static SoNguyenLon SNLonMax()
    {
        return max_snl;
    }
};

// Khởi tạo thành viên tĩnh bên ngoài lớp
// Khởi tạo số max ban đầu là "0"
SoNguyenLon SoNguyenLon::max_snl("0");


// Hàm main
int main()
{
    SoNguyenLon snl1;
    SoNguyenLon snl2("1234567");
    SoNguyenLon snl3(snl2);
    SoNguyenLon snl4(20);
    SoNguyenLon snl5;
    snl5 = snl2;
    SoNguyenLon snl6;
    snl6 = snl5;
    snl1.In(cout);
    cout << endl;
    snl2.In(cout);
    cout << endl;
    snl3.In(cout);
    cout << endl;
    snl4.In(cout);
    cout << endl;
    snl5.In(cout);
    cout << endl;
    snl6.In(cout);
    cout << endl;
    
    snl4 = snl3.TraVe(snl2.Cong(123456789));
    snl3 = snl1.Cong(snl2).Cong(snl3).Cong("999999999");
    snl5 = snl6.Cong(1234567891011);
    
    snl1.In(cout);
    cout << endl;
    snl2.In(cout);
    cout << endl;
    snl3.In(cout);
    cout << endl;
    snl4.In(cout);
    cout << endl;
    snl5.In(cout);
    cout << endl;
    snl6.In(cout);
    cout << endl;
    
    cout << "SoNguyenLon Max: " << endl;
    SoNguyenLon::SNLonMax().In(cout);
    cout << endl;
    
    system("pause");
    return 0;
}

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

// Hàm trợ giúp đảo chuỗi
void reverse_string(std::string& str) 
{
    int n = str.length();
    if (n <= 1) return;
    int left = 0;
    int right = n - 1;
    while (left < right) 
    {
        char temp = str[left];
        str[left] = str[right];
        str[right] = temp;
        left++;
        right--;
    }
}


// Định nghĩa lớp SoNguyenLon
class SoNguyenLon
{
private:
    string so; // Biến lưu trữ số dưới dạng chuỗi

    // Biến tĩnh để lưu số lớn nhất
    static SoNguyenLon max_snl;

    // Hàm trợ giúp: so sánh (this > other)
    bool is_greater(const SoNguyenLon& other) const
    {
        if (so.length() > other.so.length()) return true;
        if (so.length() < other.so.length()) return false;
        return so > other.so;
    }

    // Hàm trợ giúp: so sánh (this < other)
    bool is_less(const SoNguyenLon& other) const
    {
        if (so.length() < other.so.length()) return true;
        if (so.length() > other.so.length()) return false;
        return so < other.so;
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
        
        reverse_string(s1);
        reverse_string(s2);

        int n1 = s1.length(), n2 = s2.length();
        int max_len = (n1 > n2) ? n1 : n2;

        for (int i = 0; i < max_len; i++)
        {
            int digit1 = (i < n1) ? (s1[i] - '0') : 0;
            int digit2 = (i < n2) ? (s2[i] - '0') : 0;
            
            int sum = digit1 + digit2 + carry;
            result.push_back((sum % 10) + '0');
            carry = sum / 10;
        }
        if (carry)
        {
            result.push_back(carry + '0');
        }
        reverse_string(result);
        
        return result.empty() ? "0" : result;
    }

    // Hàm trợ giúp tĩnh: Trừ hai chuỗi số (s1 - s2)
    static string subtract_strings(string s1, string s2)
    {
        // Kiểm tra s1 < s2
        bool s1_is_less = false;
        if (s1.length() < s2.length()) {
            s1_is_less = true;
        } else if (s1.length() == s2.length() && s1 < s2) {
            s1_is_less = true;
        }

        // Giả định SoNguyenLon không xử lý số âm, trả về "0" nếu s1 < s2
        if (s1_is_less) {
            return "0";
        }
        if (s1 == s2) {
            return "0";
        }

        string result = "";
        int borrow = 0;

        reverse_string(s1);
        reverse_string(s2);

        int n1 = s1.length(), n2 = s2.length();

        for (int i = 0; i < n1; i++)
        {
            int digit1 = s1[i] - '0';
            int digit2 = (i < n2) ? (s2[i] - '0') : 0;

            int diff = digit1 - digit2 - borrow;

            if (diff < 0) {
                diff = diff + 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result.push_back(diff + '0');
        }

        reverse_string(result);

        // Xóa các số 0 ở đầu (ví dụ: "00123" -> "123")
        size_t first_digit = result.find_first_not_of('0');
        if (string::npos == first_digit) {
            return "0"; // Kết quả là "0" hoặc "000"
        }
        
        return result.substr(first_digit);
    }

public:
    // CÁC HÀM THÀNH VIÊN (PUBLIC)

    // 1. Hàm dựng mặc định
    SoNguyenLon()
    {
        so = "0";
    }

    // 2. Hàm dựng từ chuỗi
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

    // 3. Hàm dựng từ (chữ số, số lượng)
    SoNguyenLon(int digit, int count)
    {
        so = "";
        if (digit >= 0 && digit <= 9 && count > 0)
        {
            for (int i = 0; i < count; i++)
            {
                so.push_back(digit + '0');
            }
        }
        else
        {
            so = "0"; // Giá trị mặc định nếu đầu vào không hợp lệ
        }
        
        // Trường hợp đặc biệt: (0, 5) -> "00000" -> nên là "0"
        if (so.length() > 1 && so[0] == '0')
        {
            so = "0";
        }
        
        update_max();
    }
    
    // Hàm dựng từ số nguyên (dùng cho HieuVoiHangSo)
    SoNguyenLon(long long n)
    {
        so = to_string(n);
        update_max();
    }
    
    // Hàm dựng sao chép
    SoNguyenLon(const SoNguyenLon& other)
    {
        so = other.so;
        // Không cần update_max vì đây là bản sao
    }

    // Hàm hủy
    ~SoNguyenLon() {}

    // Toán tử gán
    SoNguyenLon& operator=(const SoNguyenLon& other)
    {
        so = other.so;
        return *this;
    }

    // 5. Hàm In
    void In(ostream& os)
    {
        os << so;
    }

    // 6. Hàm Trừ
    SoNguyenLon Tru(const SoNguyenLon& other)
    {
        string diff_str = subtract_strings(this->so, other.so);
        return SoNguyenLon(diff_str); // Hàm dựng sẽ gọi update_max
    }

    // 7. Hàm Cộng (cần cho snl7)
    SoNguyenLon Cong(long long n)
    {
        string sum_str = add_strings(this->so, to_string(n));
        return SoNguyenLon(sum_str);
    }
    
    // 8. Hàm tĩnh HieuVoiHangSo
    static SoNguyenLon HieuVoiHangSo(long long n, const SoNguyenLon& other)
    {
        string s_n = to_string(n);
        string diff_str = subtract_strings(s_n, other.so);
        return SoNguyenLon(diff_str);
    }

    // 9. Hàm tĩnh trả về số lớn nhất
    static SoNguyenLon SNLonMax()
    {
        return max_snl;
    }
};

// Khởi tạo thành viên tĩnh bên ngoài lớp
SoNguyenLon SoNguyenLon::max_snl("0");


// Hàm main
int main()
{
    SoNguyenLon snl1;
    SoNguyenLon snl2("1234567");
    SoNguyenLon snl3(3,14); // "33333333333333"
    SoNguyenLon snl4(9,20); // "99999999999999999999"
    SoNguyenLon snl5 = snl3.Tru(snl2); // snl3 - snl2
    SoNguyenLon snl6 = SoNguyenLon::HieuVoiHangSo(45678910, snl2); // 45678910 - snl2
    SoNguyenLon snl7 = snl4.Tru(snl3).Cong(123456789); // (snl4 - snl3) + 123456789
    
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
    snl7.In(cout);
    cout << endl;
    
    cout << "SoNguyenLon Max: " << endl;
    SoNguyenLon::SNLonMax().In(cout);
    cout << endl;
    
    system("pause");
    return 0;
}

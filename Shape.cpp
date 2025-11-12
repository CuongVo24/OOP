#include <iostream> // Cho viec xuat nhap (cout)
#include <string>   // Su dung std::string
#include <vector>   // Su dung std::vector
#include <memory>   // Su dung std::unique_ptr, std::make_unique
#include <typeinfo> // Su dung dynamic_cast

// Su dung namespace std de code gon gang hon
using namespace std;

/**
 * @brief Lop tien ich de luu tru toa do diem
 */
class Point {
public:
    double x, y;

    Point(double x = 0, double y = 0) : x(x), y(y) {}

    void print() const {
        cout << "(" << x << ", " << y << ")";
    }

    // Toan tu so sanh de giup ham equals() cua cac hinh
    bool operator==(const Point& other) const {
        // So sanh so thuc nen co the can dung epsilon, 
        // nhung trong bai tap nay ta gia su so sanh truc tiep la du
        return (x == other.x && y == other.y);
    }
};

/**
 * @brief Giao dien (Interface) cho Painter
 * Dinh nghia cac kieu ve ma Shape co the su dung
 */
class Painter {
public:
    virtual ~Painter() {} // Destructor ao
    virtual string getStyle() const = 0; // Ham thuan ao
};

/**
 * @brief Lop Painter cu the: Kieu but long
 */
class Brush : public Painter {
public:
    string getStyle() const override {
        return "net kieu but long";
    }
};

/**
 * @brief Lop Painter cu the: Kieu but bi
 */
class BallPen : public Painter {
public:
    string getStyle() const override {
        return "net kieu but bi";
    }
};

/**
 * @brief (a) Lop co so truu tuong (Abstract Base Class) cho tat ca cac hinh
 * Day la "giao dien" chung ma Line, Circle, Rectangle, va Image deu phai tuan thu.
 */
class Shape {
public:
    virtual ~Shape() {} // Destructor ao la bat buoc cho lop co so da hinh

    // Ham thuan ao (pure virtual) 'print', bat buoc lop con phai dinh nghia
    virtual void print() const = 0;

    // Ham thuan ao 'equals', bat buoc lop con phai dinh nghia
    // Day la phien ban C++ cua 'boolean equals(Object o)' trong Java
    virtual bool equals(const Shape& other) const = 0;
};

// --- Cac lop hinh hoc cu the ---

class Line : public Shape {
protected:
    Point p1, p2;
    Painter* m_painter; // Su dung con tro tho, vi Painter duoc so huu o ben ngoai

public:
    Line(Point p1, Point p2, Painter* p) : p1(p1), p2(p2), m_painter(p) {}

    void print() const override {
        cout << "Duong thang: tu ";
        p1.print();
        cout << " den ";
        p2.print();
        cout << " (ve bang: " << m_painter->getStyle() << ")" << endl;
    }

    bool equals(const Shape& other) const override {
        // Kiem tra xem 'other' co phai la 'Line' khong
        const Line* otherLine = dynamic_cast<const Line*>(&other);
        if (otherLine) {
            // Neu dung, so sanh cac thuoc tinh
            return (this->p1 == otherLine->p1 && this->p2 == otherLine->p2);
        }
        // Neu khac kieu thi chac chan khong bang nhau
        return false;
    }
};

class Circle : public Shape {
protected:
    Point center;
    double radius;
    Painter* m_painter;

public:
    Circle(Point c, double r, Painter* p) : center(c), radius(r), m_painter(p) {}

    void print() const override {
        cout << "Hinh tron: Tam ";
        center.print();
        cout << ", ban kinh " << radius;
        cout << " (ve bang: " << m_painter->getStyle() << ")" << endl;
    }

    bool equals(const Shape& other) const override {
        const Circle* otherCircle = dynamic_cast<const Circle*>(&other);
        if (otherCircle) {
            return (this->center == otherCircle->center && this->radius == otherCircle->radius);
        }
        return false;
    }
};

class Rectangle : public Shape {
protected:
    Point topLeft;
    Point bottomRight;
    Painter* m_painter;

public:
    Rectangle(Point tl, Point br, Painter* p) : topLeft(tl), bottomRight(br), m_painter(p) {}

    void print() const override {
        cout << "Hinh chu nhat: Goc trai-tren ";
        topLeft.print();
        cout << ", goc phai-duoi ";
        bottomRight.print();
        cout << " (ve bang: " << m_painter->getStyle() << ")" << endl;
    }

    /**
     * @brief (b) Cai dat phuong thuc 'equals' cho Rectangle
     * Ghi de (override) phuong thuc ao tu lop Shape
     */
    bool equals(const Shape& other) const override {
        // 1. Su dung dynamic_cast de kiem tra 'other' co phai la Rectangle khong
        // day la cach an toan de ep kieu trong da hinh
        const Rectangle* otherRect = dynamic_cast<const Rectangle*>(&other);

        // 2. Neu 'other' dung la mot Rectangle (con tro khong phai la nullptr)
        if (otherRect) {
            // So sanh cac thuoc tinh cua 'this' voi 'otherRect'
            return (this->topLeft == otherRect->topLeft && 
                    this->bottomRight == otherRect->bottomRight);
        }

        // 3. Neu 'other' khong phai la Rectangle, chung khong bang nhau
        return false;
    }
};

/**
 * @brief (c) Dinh nghia lop Image (Mau thiet ke Composite)
 * Lop 'Image' cung LA MOT 'Shape' (de co the long nhau)
 * va 'Image' CHUA nhieu 'Shape' khac.
 */
class Image : public Shape {
protected:
    // Su dung unique_ptr de tu dong quan ly bo nho cua cac Shape ma no chua
    vector<unique_ptr<Shape>> m_shapes;

public:
    // Image khong can Painter, no chi la mot "container"
    Image() {} 

    // Ham de them mot Shape vao Image
    // Nhan vao mot unique_ptr va "move" quyen so huu vao vector
    void add(unique_ptr<Shape> shape) {
        m_shapes.push_back(std::move(shape));
    }

    /**
     * @brief (c) Dinh nghia phuong thuc print() cho Image
     * Se in ra thong tin cua tat ca cac Shape ma no chua
     */
    void print() const override {
        cout << "--- Bat dau Image ---" << endl;
        // Duyet qua vector va goi ham print() cua moi Shape con
        for (const auto& shape : m_shapes) {
            cout << "  "; // Thut le cho de nhin
            shape->print(); // Day la tinh da hinh
        }
        cout << "--- Ket thuc Image ---" << endl;
    }

    /**
     * @brief Dinh nghia equals cho Image
     * Hai Image bang nhau neu chung co cung so luong Shape
     * va cac Shape o tung vi tri tuong ung cung bang nhau
     */
    bool equals(const Shape& other) const override {
        const Image* otherImage = dynamic_cast<const Image*>(&other);
        if (otherImage) {
            // Kiem tra so luong phan tu
            if (this->m_shapes.size() != otherImage->m_shapes.size()) {
                return false;
            }

            // Kiem tra tung phan tu con
            for (size_t i = 0; i < m_shapes.size(); ++i) {
                // De quy goi ham equals() cua cac Shape con
                if (!this->m_shapes[i]->equals(*otherImage->m_shapes[i])) {
                    return false;
                }
            }
            // Neu tat ca deu dung
            return true;
        }
        // Khac kieu
        return false;
    }
};


// --- Ham main() de kiem tra chuong trinh ---

int main() {
    // Tao ra cac "cong cu ve" (Painter)
    // Cac Painter nay se duoc so huu boi ham main va duoc cac Shape su dung
    Brush brush;
    BallPen ballpen;

    cout << "--- (a) Tao cac doi tuong Shape ---" << endl;
    // Tao cac hinh cu the
    // Su dung make_unique (C++14 tro len) de tao con tro thong minh
    unique_ptr<Shape> line = make_unique<Line>(Point(0, 0), Point(10, 10), &brush);
    unique_ptr<Shape> circle = make_unique<Circle>(Point(5, 5), 3.0, &ballpen);
    unique_ptr<Shape> rect1 = make_unique<Rectangle>(Point(1, 1), Point(8, 8), &brush);
    
    line->print();
    circle->print();
    rect1->print();

    cout << "\n--- (b) Kiem tra Rectangle.equals() ---" << endl;
    // Tao them 2 Rectangle de kiem tra
    unique_ptr<Shape> rect2 = make_unique<Rectangle>(Point(1, 1), Point(8, 8), &ballpen); // Cung toa do, khac painter
    unique_ptr<Shape> rect3 = make_unique<Rectangle>(Point(0, 0), Point(1, 1), &brush); // Khac toa do

    // boolalpha giup in ra 'true'/'false' thay vi '1'/'0'
    cout << "rect1.equals(*rect2) (Cung toa do): " << boolalpha << rect1->equals(*rect2) << endl;
    cout << "rect1.equals(*rect3) (Khac toa do): " << boolalpha << rect1->equals(*rect3) << endl;
    cout << "rect1.equals(*line)  (Khac kieu): " << boolalpha << rect1->equals(*line) << endl;

    cout << "\n--- (c) Kiem tra lop Image ---" << endl;
    Image myImage;

    // Chuyen quyen so huu cac Shape vao trong Image
    // Sau khi move, line, circle, rect1 se tro thanh nullptr
    myImage.add(std::move(line));
    myImage.add(std::move(circle));
    myImage.add(std::move(rect1));
    
    // rect2, rect3 khong duoc them vao, se tu dong bi huy khi het pham vi main
    
    // Goi ham print() cua Image
    myImage.print();

    // Kiem tra long nhau: Tao mot Image khac va them vao Image goc
    unique_ptr<Image> subImage = make_unique<Image>();
    subImage->add(make_unique<Line>(Point(1, 1), Point(2, 2), &ballpen));
    
    cout << "\n--- Them mot Image con vao Image chinh ---" << endl;
    myImage.add(std::move(subImage));
    myImage.print();

    return 0;
}

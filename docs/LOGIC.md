# Tài liệu Logic & Mathematical Algorithms

**Tác giả:** Sang

---

## 1. Tổng quan chức năng logic & toán học trong kernel

- Chức năng toán học là trung tâm của hệ điều hành này: giải phương trình bậc 2 dạng `ax^2 + bx + c = 0`.
- Toàn bộ thuật toán được viết thủ công, không dùng thư viện ngoài, đảm bảo chạy tốt trong môi trường kernel tối giản.

## 2. Các hàm toán học chính

### a. Hàm căn bậc hai nguyên (sqrt_int)
```cpp
int32_t sqrt_int(int32_t x)
{
    if(x <= 1) return x;
    int32_t guess = x / 2;
    int32_t prev_guess;
    for(int i = 0; i < 10; i++) {
        prev_guess = guess;
        guess = (guess + x / guess) / 2;
        if(guess == prev_guess) break;
    }
    return guess;
}
```
- Tính căn bậc hai nguyên bằng phương pháp Newton, giới hạn số vòng lặp để tránh treo máy.
- Trả về phần nguyên gần đúng nhất.

### b. Hàm chuyển chuỗi sang số nguyên (stringToInt)
```cpp
int32_t stringToInt(char* str, int len)
{
    int32_t result = 0;
    int32_t sign = 1;
    int start = 0;
    if(str[0] == '-') { sign = -1; start = 1; }
    for(int i = start; i < len; i++) {
        if(str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
        }
    }
    return result * sign;
}
```
- Tự viết, hỗ trợ số âm, không phụ thuộc thư viện chuẩn.

### c. Cấu trúc lưu nghiệm phương trình (QuadraticResult)
```cpp
struct QuadraticResult {
    bool hasRealRoots;
    int32_t x1_int;  // phần nguyên nghiệm 1
    int32_t x1_frac; // phần thập phân * 1000
    int32_t x2_int;  // phần nguyên nghiệm 2
    int32_t x2_frac; // phần thập phân * 1000
    int32_t discriminant;
};
```
- Lưu trữ nghiệm, phân biệt nghiệm thực/vô nghiệm, lưu cả phần nguyên và phần thập phân (x1000).

### d. Hàm giải phương trình bậc 2 (solveQuadratic)
```cpp
QuadraticResult solveQuadratic(int32_t a, int32_t b, int32_t c)
{
    QuadraticResult result;
    if(a == 0) { result.hasRealRoots = false; return result; }
    result.discriminant = b * b - 4 * a * c;
    if(result.discriminant < 0) { result.hasRealRoots = false; return result; }
    result.hasRealRoots = true;
    int32_t sqrt_disc = sqrt_int(result.discriminant);
    int32_t numerator1 = (-b + sqrt_disc) * 1000;
    int32_t numerator2 = (-b - sqrt_disc) * 1000;
    int32_t denominator = 2 * a;
    result.x1_int = numerator1 / denominator / 1000;
    result.x1_frac = (numerator1 / denominator) % 1000;
    if(result.x1_frac < 0) result.x1_frac = -result.x1_frac;
    result.x2_int = numerator2 / denominator / 1000;
    result.x2_frac = (numerator2 / denominator) % 1000;
    if(result.x2_frac < 0) result.x2_frac = -result.x2_frac;
    return result;
}
```
- Tính delta (discriminant), kiểm tra nghiệm thực/vô nghiệm.
- Tính nghiệm, lưu phần nguyên và phần thập phân để hiển thị đẹp.

## 3. Hiển thị kết quả và xử lý logic giao diện
- Hàm `displayQuadraticInterface()` sẽ gọi `solveQuadratic()` và hiển thị nghiệm lên màn hình.
- Phân biệt các trường hợp: vô nghiệm, nghiệm kép, hai nghiệm phân biệt.
- Hiển thị phần thập phân với 3 chữ số, căn chỉnh vị trí số trên giao diện.

## 4. Lưu ý thực tiễn
- Luôn kiểm tra hệ số a ≠ 0 trước khi giải.
- Nếu delta < 0, báo vô nghiệm.
- Làm tròn phần thập phân, tránh lỗi hiển thị số âm.
- Không dùng phép chia thực (float/double), chỉ dùng số nguyên để đảm bảo tương thích kernel.

## 5. Vai trò của Sang
- Thiết kế và hiện thực toàn bộ thuật toán toán học trong kernel.
- Đảm bảo thuật toán chạy ổn định, chính xác, không phụ thuộc thư viện ngoài.
- Hỗ trợ các thành viên khác tích hợp logic toán vào giao diện và input.

## 6. Tài liệu tham khảo
- [Wikipedia: Quadratic Equation](https://en.wikipedia.org/wiki/Quadratic_equation)
- [OSDev: Integer Math](https://wiki.osdev.org/Integer_Math)

---

**Sang - Mathematical Algorithms & Logic** 
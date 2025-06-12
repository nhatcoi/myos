# Tài liệu User Interface & Display

**Tác giả:** Minh

---

## 1. Tổng quan giao diện người dùng (UI)

- Giao diện của hệ điều hành được xây dựng hoàn toàn trên **VGA text mode** (không dùng GUI đồ họa).
- Mục tiêu: hiển thị rõ ràng, đẹp mắt, dễ sử dụng, hỗ trợ nhập liệu trực tiếp và phản hồi tức thì.

## 2. Các hàm hiển thị giao diện chính

### a. Xóa màn hình (clearScreen)
```cpp
void clearScreen() {
    uint16_t* VideoMemory = (uint16_t*)0xb8000;
    for(int i = 0; i < 80*25; i++) {
        VideoMemory[i] = 0x0720; // Space, màu trắng trên nền đen
    }
}
```
- Xóa toàn bộ màn hình, trả về trạng thái sạch.

### b. Vẽ khung viền (drawBorder)
```cpp
void drawBorder() {
    uint16_t* VideoMemory = (uint16_t*)0xb8000;
    // Vẽ border trên, dưới, trái, phải bằng ký tự ASCII
    // ... (xem code kernel.cpp)
}
```
- Tạo khung viền bằng ký tự ASCII, giúp giao diện gọn gàng, chuyên nghiệp.

### c. In text tại vị trí bất kỳ (printAt)
```cpp
void printAt(char* text, int x, int y, uint8_t color = 0x0F) {
    uint16_t* VideoMemory = (uint16_t*)0xb8000;
    for(int i = 0; text[i] != '\0'; i++) {
        VideoMemory[y*80 + x + i] = (color << 8) | text[i];
    }
}
```
- In chuỗi ký tự tại vị trí (x, y) với màu sắc tùy chọn.

### d. In text căn giữa (printCentered)
```cpp
void printCentered(char* text, int line, uint8_t color = 0x0F) {
    // Tính toán vị trí bắt đầu để căn giữa
    // In từng ký tự ra dòng 'line'
}
```
- Dùng để in tiêu đề, thông báo, menu chính giữa màn hình.

### e. In số nguyên tại vị trí (printIntAt)
```cpp
void printIntAt(int32_t num, int x, int y, uint8_t color = 0x0F) {
    // Chuyển số thành chuỗi, in từng ký tự tại (x, y)
}
```
- Hỗ trợ hiển thị số nguyên (cả âm và dương) tại vị trí bất kỳ.

## 3. Tổ chức layout & các trạng thái giao diện

- Giao diện chia thành nhiều trạng thái:
  - **MAIN_MENU**: Menu chính, chọn chức năng.
  - **WAITING_A/B/C**: Nhập hệ số phương trình, hiển thị input và con trỏ.
  - **SHOWING_RESULT**: Hiển thị phương trình và nghiệm.
  - **HELP_SCREEN, ABOUT_SCREEN**: Hướng dẫn, thông tin nhóm.
- Mỗi trạng thái có layout riêng, dùng các hàm trên để vẽ nội dung phù hợp.

### Ví dụ: Hiển thị giao diện nhập hệ số
```cpp
void displayQuadraticInterface() {
    clearScreen();
    drawBorder();
    printCentered("GIAI PHUONG TRINH BAC 2", 2, 0x0E);
    // ...
    switch(current_state) {
        case WAITING_A:
            printAt("Nhap he so a (a != 0): ", 10, 8, 0x0F);
            break;
        // ...
    }
    // ...
}
```

## 4. Hiển thị input và con trỏ nhập liệu

- Khi nhập hệ số, input được hiển thị trực tiếp trên màn hình.
- Con trỏ nhấp nháy (`_`) giúp người dùng biết vị trí nhập tiếp theo.
- Hàm `displayCurrentInput()` sẽ xóa vùng nhập, in lại input buffer và con trỏ.

## 5. Màu sắc & thẩm mỹ
- Sử dụng màu sắc khác nhau cho tiêu đề, border, thông báo lỗi, kết quả, ...
- Giúp giao diện sinh động, dễ nhìn, phân biệt các vùng chức năng.

## 6. Lưu ý thực tiễn
- Luôn xóa vùng nhập trước khi in input mới để tránh đè ký tự cũ.
- Đảm bảo không ghi đè border khi in text.
- Căn chỉnh vị trí số, text để giao diện cân đối.
- Tối ưu số lượng ký tự trên mỗi dòng để không bị tràn màn hình.

## 7. Vai trò của Minh
- Thiết kế, hiện thực toàn bộ giao diện text mode.
- Đảm bảo giao diện thân thiện, dễ sử dụng, đẹp mắt.
- Hỗ trợ các thành viên khác tích hợp input, logic toán vào giao diện.

## 8. Tài liệu tham khảo
- [OSDev Wiki: VGA Text Mode](https://wiki.osdev.org/Text_Mode_Cursor)
- [ASCII Table](https://www.asciitable.com/)

---

**Minh - User Interface & Display**

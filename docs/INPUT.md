# Tài liệu Keyboard Driver & Input Handling

**Tác giả:** Ngoc

---

## 1. Tổng quan về Keyboard Driver

- **Keyboard driver** là thành phần trung gian giữa phần cứng bàn phím và hệ điều hành.
- Nhiệm vụ: Nhận tín hiệu từ bàn phím, chuyển thành mã ký tự, gửi đến kernel để xử lý.
- Trong dự án này, driver bàn phím đóng vai trò trung tâm cho mọi thao tác nhập liệu (menu, nhập hệ số, điều hướng giao diện).

## 2. Nguyên lý hoạt động

- Khi người dùng nhấn phím, bàn phím gửi mã ngắt (interrupt) đến CPU.
- Kernel đã đăng ký một hàm xử lý ngắt (ISR) cho bàn phím.
- ISR đọc mã phím từ cổng I/O, chuyển thành ký tự ASCII.
- Ký tự này được chuyển đến handler để xử lý logic ứng dụng.

### Sơ đồ luồng xử lý:
1. Người dùng nhấn phím →
2. Bàn phím gửi tín hiệu →
3. CPU nhận ngắt →
4. Kernel gọi Keyboard ISR →
5. Đọc mã phím, chuyển thành ký tự →
6. Gửi ký tự đến handler →
7. Handler xử lý (cập nhật input, chuyển trạng thái, ...)

## 3. Cấu trúc driver & event handler

- Kế thừa từ `KeyboardEventHandler`:
  ```cpp
  class PrintfKeyboardEventHandler : public KeyboardEventHandler {
  public:
      void OnKeyDown(char c) {
          // Xử lý ký tự nhập vào
      }
  };
  ```
- Đăng ký handler với driver:
  ```cpp
  PrintfKeyboardEventHandler kbhandler;
  KeyboardDriver keyboard(&interrupts, &kbhandler);
  drvManager.AddDriver(&keyboard);
  ```

## 4. Xử lý input & các trạng thái giao diện

- Hệ điều hành chia thành nhiều trạng thái giao diện:
  - `MAIN_MENU`: Chờ chọn chức năng.
  - `WAITING_A/B/C`: Nhập hệ số phương trình.
  - `SHOWING_RESULT`: Hiển thị kết quả.
  - `HELP_SCREEN`, `ABOUT_SCREEN`: Hướng dẫn, thông tin nhóm.
- Mỗi trạng thái có logic xử lý input riêng:
  - Ở menu: chỉ nhận phím số 1-4.
  - Ở nhập hệ số: nhận số, dấu trừ, backspace, enter.
  - Ở kết quả: nhận 'r' để giải lại, ESC để về menu.

### Ví dụ xử lý input:
```cpp
void OnKeyDown(char c) {
    if(current_state == MAIN_MENU) {
        switch(c) {
            case '1': /* ... */ break;
            // ...
        }
        return;
    }
    if(c == 27) { // ESC
        current_state = MAIN_MENU;
        displayMainMenu();
        return;
    }
    // ...
    if(current_state >= WAITING_A && current_state <= WAITING_C) {
        if(c == '\n' || c == '\r') { /* xác nhận nhập */ }
        if(c == '\b' || c == 127) { /* xóa ký tự */ }
        if((c >= '0' && c <= '9') || (c == '-' && input_pos == 0)) {
            /* thêm ký tự vào input_buffer */
        }
    }
}
```

## 5. Validate & hiển thị input

- Chỉ cho phép nhập số, dấu trừ ở đầu.
- Nếu nhập sai định dạng (ví dụ a=0), báo lỗi và cho nhập lại.
- Hiển thị input trực tiếp trên màn hình, có con trỏ nhấp nháy.
- Hỗ trợ backspace, xóa ký tự, giới hạn độ dài input.

## 6. Lưu ý thực tiễn
- Luôn kiểm tra input buffer tránh tràn (giới hạn độ dài).
- Xử lý đúng mã ASCII cho các phím đặc biệt (Enter: `\n`/`\r`, Backspace: `\b`/127, ESC: 27).
- Đảm bảo input thread-safe nếu mở rộng đa luồng.
- Khi debug, có thể in mã phím nhận được để kiểm tra.

## 7. Tài liệu tham khảo
- [OSDev Wiki: PS/2 Keyboard](https://wiki.osdev.org/%228042%22_PS/2_Controller)
- [OSDev Wiki: Keyboard Driver](https://wiki.osdev.org/Keyboard_Driver)
- [ASCII Table](https://www.asciitable.com/)

## 8. Cách thức hoạt động & Giải thích code Keyboard/Input trong kernel.cpp

### 8.1. Luồng xử lý phím từ phần cứng đến ứng dụng

1. Người dùng nhấn phím trên bàn phím vật lý.
2. Bàn phím gửi tín hiệu (interrupt) đến CPU (thường là IRQ1).
3. CPU gọi hàm xử lý ngắt đã đăng ký cho bàn phím trong kernel (ISR).
4. Driver bàn phím (`KeyboardDriver`) đọc mã phím (scancode) từ cổng I/O, chuyển sang ký tự ASCII.
5. Ký tự ASCII được chuyển tới event handler (`PrintfKeyboardEventHandler`).
6. Handler xử lý ký tự theo trạng thái giao diện (menu, nhập hệ số, kết quả, ...).
7. Giao diện được cập nhật trực tiếp trên màn hình (VGA text mode).

### 8.2. Giải thích code chính

#### a. Đăng ký driver và event handler
```cpp
class PrintfKeyboardEventHandler : public KeyboardEventHandler {
public:
    void OnKeyDown(char c) {
        // Xử lý input ở đây
    }
};

PrintfKeyboardEventHandler kbhandler;
KeyboardDriver keyboard(&interrupts, &kbhandler);
drvManager.AddDriver(&keyboard);
```
- Định nghĩa handler kế thừa từ `KeyboardEventHandler`, override hàm `OnKeyDown` để xử lý từng ký tự nhập vào.
- Đăng ký driver với `DriverManager` để hệ điều hành quản lý và kích hoạt.

#### b. Xử lý input trong OnKeyDown
```cpp
void OnKeyDown(char c) {
    // 1. Ở menu chính: chọn chức năng
    if(current_state == MAIN_MENU) {
        switch(c) {
            case '1': ... break;
            case '2': ... break;
            case '3': ... break;
            case '4': ... break;
        }
        return;
    }
    // 2. ESC: quay về menu chính
    if(c == 27) { ... }
    // 3. Ở màn hình hướng dẫn/thông tin: chỉ nhận ESC
    if(current_state == HELP_SCREEN || current_state == ABOUT_SCREEN) { ... }
    // 4. Ở màn hình kết quả: nhận 'r' để giải lại
    if(current_state == SHOWING_RESULT) { ... }
    // 5. Ở trạng thái nhập hệ số (a, b, c)
    if(current_state >= WAITING_A && current_state <= WAITING_C) {
        // Enter: xác nhận nhập hệ số
        if(c == '\n' || c == '\r') { ... }
        // Backspace: xóa ký tự cuối
        if(c == '\b' || c == 127) { ... }
        // Nhập số hoặc dấu trừ (chỉ ở đầu)
        if((c >= '0' && c <= '9') || (c == '-' && input_pos == 0)) { ... }
    }
}
```
- Tùy trạng thái giao diện (`current_state`), input được xử lý khác nhau.
- Ở menu: chỉ nhận phím số 1-4.
- ESC: luôn cho phép quay về menu chính.
- Nhập hệ số: nhận số, dấu trừ (ở đầu), backspace, enter.
- Khi nhấn Enter: kiểm tra input, validate (a ≠ 0), lưu hệ số, chuyển trạng thái.
- Khi nhấn Backspace: xóa ký tự cuối cùng trong buffer.
- Khi nhập số: thêm vào buffer, hiển thị lại input và con trỏ.

#### c. Hiển thị input và con trỏ
```cpp
void displayCurrentInput() {
    // ...
    // Xóa vùng nhập
    // Hiển thị input
    // Hiển thị con trỏ nhấp nháy
}
```
- Mỗi lần nhập/xóa ký tự, hàm này sẽ cập nhật vùng nhập liệu và con trỏ trên màn hình.

### 8.3. Tổng kết
- Keyboard driver nhận tín hiệu phần cứng, chuyển thành ký tự, chuyển cho handler.
- Handler xử lý input theo trạng thái giao diện, validate, cập nhật buffer, chuyển trạng thái, hiển thị lại.
- Giao diện được cập nhật trực tiếp trên VGA text mode, giúp người dùng thấy ngay kết quả nhập liệu.

---

**Ngoc - Keyboard Driver & Input Handling** 
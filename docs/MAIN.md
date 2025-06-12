# Tài liệu Quản lý Hệ điều hành, Kernel & Memory Management

## 1. Tổng quan dự án

Dự án này là một hệ điều hành nhỏ (mini OS) được phát triển dựa trên mã nguồn mở **wyoos** (Write Your Own Operating System). Mục tiêu là xây dựng một hệ điều hành tối giản, có giao diện đẹp, hỗ trợ nhập liệu thông minh, và giải quyết một bài toán thực tế: **giải phương trình bậc 2**. Dự án đồng thời là ví dụ thực tiễn về cách tổ chức, phát triển, và quản lý một hệ điều hành từ đầu đến cuối.

### Mục tiêu chính
- Hiểu cấu trúc và nguyên lý hoạt động của một hệ điều hành.
- Làm chủ các thành phần: kernel, driver, quản lý bộ nhớ, giao diện người dùng.
- Ứng dụng thực tế: giải phương trình bậc 2 với giao diện thân thiện.
- Tối ưu hóa, loại bỏ các thành phần không cần thiết để hệ điều hành gọn nhẹ.

## 2. Giới thiệu về wyoos

[wyoos](https://github.com/TSnake41/wyoos) là một dự án mã nguồn mở hướng dẫn từng bước xây dựng hệ điều hành từ đầu bằng C++. wyoos cung cấp các thành phần cơ bản như:
- **Kernel**: lõi hệ điều hành, quản lý tài nguyên, điều phối các thành phần khác.
- **Driver**: thư viện điều khiển phần cứng (bàn phím, chuột, VGA, ổ đĩa, mạng, ...).
- **Hardware Communication**: giao tiếp trực tiếp với phần cứng qua cổng I/O, ngắt, ...
- **Multitasking**: hỗ trợ đa tiến trình cơ bản.
- **Memory Management**: quản lý bộ nhớ động, cấp phát và giải phóng vùng nhớ.
- **GUI**: thư viện giao diện đồ họa cơ bản (window, widget, desktop, ...).

Dự án này đã **tối giản** wyoos, chỉ giữ lại các thành phần cần thiết cho mục tiêu giải phương trình bậc 2.

## 3. Cấu trúc hệ điều hành của dự án

### 3.1. Thành phần chính
- **Kernel** (`kernel.cpp`): Lõi hệ điều hành, khởi tạo hệ thống, quản lý bộ nhớ, điều phối driver, xử lý vòng lặp chính.
- **Driver**:
  - `keyboard.h/cpp`: Driver bàn phím, nhận tín hiệu và chuyển thành ký tự.
  - `vga.h/cpp`: Driver VGA, điều khiển hiển thị text trên màn hình.
  - (Đã loại bỏ: mouse, net, ata, pci, gui, ... để tối giản)
- **Hardware Communication**:
  - `port.h/cpp`: Giao tiếp với cổng I/O (ví dụ: shutdown QEMU qua port 0x604).
  - `interrupts.h/cpp`: Quản lý ngắt phần cứng.
- **Memory Management**:
  - `memorymanager.h/cpp`: Quản lý cấp phát bộ nhớ động cho hệ điều hành.
- **Multitasking**:
  - `multitasking.h/cpp`: Hỗ trợ đa tiến trình (chỉ khởi tạo, không sử dụng trong phiên bản tối giản).
- **Giao diện người dùng**:
  - Toàn bộ giao diện được vẽ bằng text mode (VGA), không dùng GUI đồ họa.

### 3.2. Sơ đồ luồng khởi động
1. **Khởi động**: Bootloader nạp kernel vào bộ nhớ.
2. **Khởi tạo GDT, Interrupts**: Thiết lập bảo vệ bộ nhớ, quản lý ngắt.
3. **Khởi tạo MemoryManager**: Cấp phát vùng heap cho OS.
4. **Khởi tạo Driver**: Đăng ký driver bàn phím, VGA.
5. **Hiển thị giao diện**: Vẽ menu chính, nhận input từ người dùng.
6. **Xử lý logic**: Nhận hệ số, giải phương trình, hiển thị kết quả.

## 4. Kernel & Memory Management (Chi tiết)

### 4.1. Kernel: Vai trò và Cách thức hoạt động

#### a. Kernel là gì?
- **Kernel** là thành phần lõi, chạy ở chế độ đặc quyền nhất (ring 0), kiểm soát toàn bộ tài nguyên phần cứng và phần mềm.
- Kernel chịu trách nhiệm khởi tạo hệ thống, quản lý bộ nhớ, điều phối các driver, xử lý ngắt, và cung cấp API cho các thành phần khác.

#### b. Các bước khởi tạo kernel
1. **Khởi tạo GDT (Global Descriptor Table):**
   - Thiết lập các phân đoạn bộ nhớ, bảo vệ vùng nhớ, phân quyền truy cập.
   - Đảm bảo kernel và user code không thể truy cập lẫn nhau trái phép.
2. **Khởi tạo Interrupt Manager:**
   - Đăng ký các handler cho ngắt phần cứng (bàn phím, timer, ...).
   - Cho phép kernel phản ứng với sự kiện từ phần cứng.
3. **Khởi tạo Memory Manager:**
   - Cấp phát vùng heap cho kernel và các tiến trình.
4. **Khởi tạo Driver:**
   - Đăng ký driver bàn phím, VGA, ...
5. **Kích hoạt Interrupts:**
   - Cho phép nhận ngắt từ phần cứng.
6. **Vòng lặp chính:**
   - Giữ hệ điều hành luôn hoạt động, chờ sự kiện từ người dùng hoặc phần cứng.

#### c. Quản lý ngắt (Interrupt Handling)
- **Interrupt** là tín hiệu từ phần cứng hoặc phần mềm báo cho CPU biết cần xử lý sự kiện.
- Kernel đăng ký các hàm xử lý ngắt (ISR - Interrupt Service Routine).
- Ví dụ: Khi nhấn phím, driver bàn phím gửi ngắt, kernel gọi handler để đọc ký tự và xử lý.
- Quản lý ngắt giúp OS phản ứng nhanh, không bị treo khi chờ input.

#### d. Vòng lặp chính (Main Loop)
- Kernel thường chạy vòng lặp vô hạn:
  ```cpp
  while(1) {
      // Chờ sự kiện, xử lý input, cập nhật giao diện
  }
  ```
- Trong dự án này, vòng lặp chính giữ OS luôn chạy, không có scheduler đa tiến trình thực sự (có thể mở rộng với TaskManager).

#### e. Quản lý trạng thái giao diện
- Kernel lưu trạng thái hiện tại (menu, nhập hệ số, kết quả, ...).
- Khi nhận input, kernel chuyển đổi trạng thái và cập nhật giao diện tương ứng.

### 4.2. Memory Management: Nguyên lý và Triển khai

#### a. Phân vùng bộ nhớ trong OS
- **Bộ nhớ hệ điều hành** thường chia thành các vùng:
  - **Code segment**: Chứa mã lệnh thực thi.
  - **Data segment**: Chứa biến toàn cục, hằng số.
  - **Heap**: Vùng cấp phát động (new/delete, malloc/free).
  - **Stack**: Vùng lưu biến cục bộ, địa chỉ trả về hàm, tham số hàm.
- Trong kernel, quản lý heap và stack rất quan trọng để tránh lỗi tràn bộ nhớ, ghi đè vùng nhớ quan trọng.

#### b. Quản lý heap với MemoryManager
- **MemoryManager** là lớp tự viết, quản lý cấp phát và giải phóng vùng nhớ động cho kernel.
- Khởi tạo heap:
  ```cpp
  size_t heap = 10*1024*1024; // 10MB cho heap
  MemoryManager memoryManager(heap, (*memupper)*1024 - heap - 10*1024);
  ```
- Không dùng malloc/free chuẩn C mà dùng hàm riêng của OS để kiểm soát chặt chẽ hơn.
- Khi cần cấp phát bộ nhớ cho driver, task, ... kernel gọi hàm của MemoryManager.

#### c. Quản lý stack
- Mỗi tiến trình (nếu có) sẽ có stack riêng.
- Stack dùng để lưu biến cục bộ, địa chỉ trả về, tham số hàm.
- Stack tràn (stack overflow) có thể gây lỗi nghiêm trọng, cần kiểm soát kích thước stack.

#### d. Bảo vệ bộ nhớ
- Sử dụng GDT để phân vùng và bảo vệ bộ nhớ, tránh truy cập trái phép giữa kernel và user code.
- Nếu mở rộng, có thể dùng paging để bảo vệ tốt hơn (chưa triển khai trong dự án này).

#### e. Ví dụ code quản lý bộ nhớ
- Cấp phát bộ nhớ động cho một driver:
  ```cpp
  KeyboardDriver* keyboard = new KeyboardDriver(&interrupts, &kbhandler);
  drvManager.AddDriver(keyboard);
  // Khi không dùng nữa:
  delete keyboard;
  ```
- Cấp phát vùng nhớ cho một buffer:
  ```cpp
  char* buffer = (char*)memoryManager.malloc(256);
  // ... sử dụng buffer ...
  memoryManager.free(buffer);
  ```

#### f. Lưu ý thực tiễn khi phát triển kernel & quản lý bộ nhớ
- Luôn kiểm tra vùng nhớ trước khi truy cập.
- Tránh tràn buffer, tràn stack.
- Không dùng các hàm cấp phát động chuẩn C nếu không kiểm soát được heap.
- Khi debug lỗi bộ nhớ, nên in địa chỉ, giá trị vùng nhớ để xác định lỗi.
- Đảm bảo giải phóng vùng nhớ khi không còn sử dụng để tránh memory leak.

## 5. Các thư viện & thành phần mở rộng (đã loại bỏ/tối giản)
- **Mouse driver**: Đã loại bỏ để tập trung vào input bàn phím.
- **Network driver**: Không cần thiết cho mục tiêu giải phương trình.
- **ATA, PCI, GUI**: Loại bỏ để giảm kích thước kernel, tăng tốc độ boot.
- **GUI graphics**: Chỉ giữ lại text mode, không dùng window/widget.

## 6. Ưu điểm & Bài học kinh nghiệm
- **Tối ưu hóa**: Loại bỏ thành phần không cần thiết giúp OS nhẹ, nhanh, dễ debug.
- **Modular**: Có thể mở rộng lại các driver, GUI, network nếu cần.
- **Hiểu sâu về kernel**: Tự viết hàm quản lý bộ nhớ, driver, giao tiếp phần cứng.
- **Thực tiễn**: Ứng dụng giải toán, giao diện thân thiện, dễ sử dụng.

## 7. Tài liệu tham khảo
- [wyoos trên GitHub](https://github.com/TSnake41/wyoos)
- [OSDev Wiki](https://wiki.osdev.org/Main_Page)
- [VGA Text Mode](https://wiki.osdev.org/Text_Mode_Cursor)
- [Memory Management](https://wiki.osdev.org/Memory_Management)
- [Interrupts](https://wiki.osdev.org/Interrupts)
- [Global Descriptor Table](https://wiki.osdev.org/Global_Descriptor_Table)

---

**Tác giả tài liệu:** Nhat (Kernel & Memory Management) 
# Tài liệu Testing & Documentation

**Tác giả:** Quynh

---

## 1. Tổng quan kiểm thử hệ điều hành

- Kiểm thử (testing) là bước quan trọng để đảm bảo hệ điều hành hoạt động ổn định, đúng chức năng, giao diện thân thiện.
- Documentation giúp các thành viên khác và người dùng hiểu rõ cách sử dụng, phát triển, bảo trì hệ thống.

## 2. Hướng dẫn kiểm thử hệ điều hành

### a. Test input
- Thử nhập các giá trị hợp lệ và không hợp lệ cho hệ số a, b, c:
  - a = 0 (báo lỗi, không cho nhập)
  - a, b, c là số âm, số lớn, số nhỏ, ký tự đặc biệt (báo lỗi)
  - Nhập số hợp lệ, Enter xác nhận
  - Nhập sai, dùng Backspace để sửa

### b. Test giao diện
- Kiểm tra hiển thị menu chính, hướng dẫn, thông tin nhóm, giao diện nhập hệ số, giao diện kết quả.
- Đảm bảo các vùng text không bị đè, border hiển thị đúng, màu sắc rõ ràng.
- Kiểm tra con trỏ nhập liệu nhấp nháy đúng vị trí.

### c. Test kết quả toán học
- So sánh nghiệm phương trình với kết quả thực tế:
  - delta < 0: báo vô nghiệm
  - delta = 0: nghiệm kép, hiển thị đúng
  - delta > 0: hai nghiệm phân biệt, kiểm tra phần thập phân
- Kiểm tra hiển thị số âm, số lớn, làm tròn phần thập phân.

### d. Test điều hướng giao diện
- Dùng phím số để chọn chức năng ở menu.
- Dùng ESC để quay lại menu chính ở mọi trạng thái.
- Dùng R để giải lại phương trình khi ở màn hình kết quả.
- Dùng Backspace để xóa input, Enter để xác nhận.

## 3. Hướng dẫn sử dụng hệ điều hành
- Build: chạy `make`
- Chạy trên QEMU: `make qemu`
- Chọn chức năng bằng phím số 1-4
- Nhập hệ số, nhấn Enter xác nhận
- Dùng ESC để quay lại menu, R để giải lại

## 4. Documentation code
- Đã chú thích các hàm chính trong kernel.cpp, giải thích rõ vai trò từng hàm.
- README.md có thông tin nhóm, hướng dẫn build/run, mô tả chức năng.
- Các file tài liệu riêng cho từng thành viên: management.md, input.md, Logic.md, UI.md, Testing.md

## 5. Lưu ý thực tiễn
- Test trên QEMU, chú ý các phím đặc biệt (Backspace, Enter, ESC).
- Đảm bảo input không bị tràn buffer, không nhập ký tự lạ.
- Ghi lại các lỗi phát hiện và cách fix vào tài liệu.
- Cập nhật tài liệu khi có thay đổi code hoặc giao diện.

## 6. Vai trò của Quynh
- Thiết kế, thực hiện các ca kiểm thử cho toàn bộ hệ điều hành.
- Ghi chép, tài liệu hóa quy trình kiểm thử, hướng dẫn sử dụng, chú thích code.
- Đảm bảo sản phẩm cuối cùng dễ dùng, dễ bảo trì, dễ mở rộng.

## 7. Tài liệu tham khảo
- [OSDev Wiki: Testing](https://wiki.osdev.org/Testing)
- [README.md của dự án]

---

**Quynh - Testing & Documentation** 
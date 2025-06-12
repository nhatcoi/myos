# 🖥️ MYOS v3.0 - GUI User Guide

## 🚀 **Khởi động hệ thống:**

### **Bước 1: Boot OS**
```bash
# Build và chạy
make clean && make
make qemu

# Hoặc với UTM
# Load mykernel.iso vào VM
```

### **Bước 2: Loading Screen**
```
Khi khởi động, bạn sẽ thấy:
- Logo ASCII art của MYOS
- Dòng chữ "Dang khoi dong he dieu hanh..."
- Delay 2 giây để hiển thị
- Tự động chuyển sang Main Menu
```

---

## 🏠 **Main Menu - Giao diện chính:**

### **Layout:**
```
╔═══════════════════════════════════════════════════════════════════════════════╗
║                    * MY OPERATING SYSTEM - MYOS v3.0 *                       ║
║                    ===================================                        ║
║                                                                               ║
║                           ** MENU CHINH **                                   ║
║                           ================                                   ║
║                                                                               ║
║    TINH NANG CHINH:                    TINH NANG THEM:                       ║
║      [1] Phuong Trinh Bac 2              [5] Soan Thao Van Ban              ║
║      [2] Quan Ly Tep Tin                 [6] Tro Choi Mini                  ║
║      [3] May Tinh Da Nang                [7] Thong Tin Bo Nho               ║
║      [4] Dong Ho Thoi Gian               [8] Tro Giup                       ║
║                                                                               ║
║                    [9] Thong Tin He Thong                                    ║
║                    [0] Tat May                                               ║
║                                                                               ║
║             Nhan phim so tuong ung de chon chuc nang                         ║
║         Phat trien boi: Nhat + Ngoc + Sang + Minh + Quynh                   ║
╚═══════════════════════════════════════════════════════════════════════════════╝
```

---

## ⌨️ **Cách điều khiển:**

### **Điều hướng cơ bản:**
| Phím | Chức năng |
|------|-----------|
| **1-9** | Chọn tính năng tương ứng |
| **0** | Tắt máy |
| **ESC** | Quay lại menu chính (từ submenu) |
| **Enter** | Xác nhận input (khi nhập dữ liệu) |
| **Backspace** | Xóa ký tự (khi nhập dữ liệu) |

### **Navigation Pattern:**
```
Main Menu → Chọn số → Submenu → ESC → Quay lại Main Menu
```

---

## 🔧 **Chi tiết từng tính năng:**

### **[1] 📐 Phương trình bậc 2 (HOẠT ĐỘNG ĐẦY ĐỦ)**

#### **Cách sử dụng:**
1. **Ấn "1"** từ main menu
2. **Nhập hệ số a** (a ≠ 0):
   - Gõ số nguyên (có thể âm với dấu -)
   - Ấn Enter để xác nhận
3. **Nhập hệ số b**:
   - Gõ số nguyên
   - Ấn Enter
4. **Nhập hệ số c**:
   - Gõ số nguyên  
   - Ấn Enter
5. **Xem kết quả**:
   - Hiển thị phương trình dạng ax² + bx + c = 0
   - Kết quả nghiệm (thực/ảo/kép)
   - Ấn "R" để giải phương trình mới
   - Ấn ESC để về menu

#### **Ví dụ sử dụng:**
```
Input: a=1, b=-5, c=6
Output: 
  Phuong trinh: 1x^2 + -5x + 6 = 0
  KET QUA: Co 2 nghiem phan biet
  x1 = 3
  x2 = 2
```

### **[2] 📁 Quản lý tệp tin (UI DEMO)**

#### **Menu hiển thị:**
```
╔═══════════════════════════════════════════════════════════════════════════════╗
║                  QUAN LY TEP TIN - FILE MANAGER MINI                         ║
║                  ===================================                          ║
║                                                                               ║
║                     CHUC NANG QUAN LY TEP                                    ║
║                     =====================                                    ║
║                                                                               ║
║                    [1] Hien Thi Danh Sach Tep                               ║
║                    [2] Tao Tep Van Ban Moi                                  ║
║                    [3] Doc Noi Dung Tep                                     ║
║                    [4] Ghi Noi Dung Vao Tep                                 ║
║                    [5] Xoa Tep                                               ║
║                    [6] Sao Chep Tep                                          ║
║                                                                               ║
║              THONG TIN: Chua phat trien day du                               ║
║                         Chi la giao dien demo                               ║
║                                                                               ║
║                 Nhan ESC de quay lai menu chinh                             ║
╚═══════════════════════════════════════════════════════════════════════════════╝
```

### **[3] 🧮 Máy tính đa năng (UI DEMO)**

#### **Menu hiển thị:**
```
╔═══════════════════════════════════════════════════════════════════════════════╗
║                MAY TINH DA NANG - ADVANCED CALCULATOR                        ║
║                ======================================                        ║
║                                                                               ║
║                       CHUC NANG TINH TOAN                                   ║
║                       ===================                                   ║
║                                                                               ║
║    PHEP TOAN CO BAN:              PHEP TOAN NANG CAO:                       ║
║      [1] Cong (+)                   [5] Luy Thua (x^y)                      ║
║      [2] Tru (-)                    [6] Can Bac Hai                         ║
║      [3] Nhan (*)                   [7] Sin, Cos, Tan                       ║
║      [4] Chia (/)                   [8] Logarit (log)                       ║
║                                                                               ║
║              THONG TIN: Chua phat trien day du                               ║
║                         Chi la giao dien demo                               ║
╚═══════════════════════════════════════════════════════════════════════════════╝
```

### **[4] 🕐 Đồng hồ thời gian (UI DEMO)**

#### **Menu hiển thị:**
```
╔═══════════════════════════════════════════════════════════════════════════════╗
║               DONG HO THOI GIAN THUC - REAL-TIME CLOCK                      ║
║               =========================================                      ║
║                                                                               ║
║                      CHUC NANG THOI GIAN                                    ║
║                      ===================                                    ║
║                                                                               ║
║                    [1] Hien Thi Gio Hien Tai                               ║
║                    [2] Hien Thi Ngay Thang                                 ║
║                    [3] Dat Bao Thuc                                         ║
║                    [4] Dem Nguoc (Countdown)                               ║
║                    [5] Dung Ho Bam Gio                                     ║
║                                                                               ║
║                THOI GIAN HE THONG (DEMO):                                   ║
║                  Ngay: 15/12/2024                                           ║
║                  Gio:  14:30:25                                             ║
║                                                                               ║
║              THONG TIN: Chua phat trien day du                               ║
╚═══════════════════════════════════════════════════════════════════════════════╝
```

### **[5] ✏️ Text Editor (UI DEMO)**
### **[6] 🎮 Games (UI DEMO)**
### **[7] 💾 Memory Info (UI DEMO)**

*(Tương tự format trên với menu options riêng)*

### **[8] ❓ Trợ giúp (HOẠT ĐỘNG)**

#### **Hiển thị:**
- Hướng dẫn sử dụng phương trình bậc 2
- Các phím điều khiển
- ESC để quay lại

### **[9] ℹ️ Thông tin hệ thống (HOẠT ĐỘNG)**

#### **Hiển thị:**
- Thông tin nhóm phát triển
- Tính năng của OS
- Credits

### **[0] ⚡ Tắt máy (HOẠT ĐỘNG)**

#### **Chức năng:**
- Shutdown QEMU ngay lập tức
- Hiển thị "Shutting down..."

---

## 🎨 **Color Scheme & UI Design:**

### **Màu sắc:**
- **Yellow (0x0E)**: Headers, titles
- **Cyan (0x0B)**: Subheaders, categories  
- **Green (0x0A)**: Menu options, active items
- **Red (0x0C)**: Status, warnings, info
- **White (0x0F)**: Normal text
- **Gray (0x07)**: Instructions, help text
- **Light Gray (0x08)**: Credits, footer

### **Layout Elements:**
- **Borders**: ASCII box drawing characters
- **Centering**: Automatic text centering
- **Spacing**: Consistent padding và margins
- **Typography**: Fixed-width console font

---

## 📋 **Quick Start Workflow:**

### **Typical User Journey:**
```
1. 🚀 Boot OS
   ↓
2. 🏠 Main Menu appears
   ↓  
3. 🔢 Press number (1-9, 0)
   ↓
4. 📱 Submenu appears
   ↓
5. ⌨️ Interact with feature
   ↓
6. ⬅️ Press ESC to go back
   ↓
7. 🔄 Repeat or shutdown (0)
```

### **Testing Checklist:**
- [ ] Boot successfully
- [ ] Main menu displays correctly
- [ ] All numbers (1-9, 0) work
- [ ] ESC returns to main menu
- [ ] Phương trình bậc 2 hoạt động
- [ ] UI demo menus display
- [ ] Shutdown works

---

## 🎯 **Tips & Tricks:**

### **Best Practices:**
1. **Navigation**: Luôn dùng ESC để quay lại
2. **Input**: Enter để xác nhận, Backspace để sửa
3. **Testing**: Test từng menu một cách có hệ thống
4. **Debugging**: Nếu bị treo, restart VM

### **Keyboard Shortcuts:**
- **ESC**: Universal "go back" 
- **Enter**: Universal "confirm"
- **R**: Reset (trong phương trình bậc 2)
- **0**: Quick shutdown từ main menu

---

## 🚨 **Troubleshooting:**

### **Common Issues:**
1. **Menu không hiển thị**: Restart VM
2. **Keyboard không respond**: Check UTM keyboard settings
3. **Màn hình đen**: Wait for loading, restart nếu cần
4. **Crash khi input**: Chỉ nhập số nguyên hợp lệ

### **Performance:**
- OS nhẹ, chạy mượt trên 128MB RAM
- Response time tức thì cho navigation
- Stable với continuous usage

**GUI sử dụng đơn giản, intuitive, và professional! 🎊** 
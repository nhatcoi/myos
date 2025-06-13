# Tổ chức Code - Separation of Concerns

## 📋 Tổng quan

Code trong `kernel.cpp` đã được chia thành các file riêng biệt theo chức năng và tổ chức trong folder `modules/` để dễ đọc, bảo trì và phát triển.

## 📂 Cấu trúc thư mục:
```
src/
├── kernel.cpp              # Original kernel (642 lines)
├── kernel_clean.cpp         # Clean refactored kernel  
└── modules/                 # Separated modules
    ├── printf.cpp           # Print functions
    ├── math_functions.cpp   # Math operations
    ├── ui_functions.cpp     # UI & Display  
    └── app_logic.cpp        # Application logic
```

## 🗂️ Cấu trúc File mới

### 1. 🖨️ `src/modules/printf.cpp` - Print Functions
**Chức năng**: Tất cả functions liên quan đến in ấn và hiển thị text
```cpp
// Functions được implement:
void printf(char* str)           // In string cơ bản
void printfHex(uint8_t key)      // In số hex
void printInt(int32_t num)       // In số nguyên
void printIntAt(int x, y, color) // In số tại vị trí cụ thể
```

### 2. 🧮 `src/modules/math_functions.cpp` - Math Operations  
**Chức năng**: Các phép tính toán học và xử lý số
```cpp
// Functions được implement:
int32_t sqrt_int(int32_t x)                    // Căn bậc hai
int32_t stringToInt(char* str, int len)        // Chuyển string thành int
QuadraticResult solveQuadratic(a, b, c)        // Giải phương trình bậc 2

// Struct được define:
struct QuadraticResult {
    bool hasRealRoots;
    int32_t x1_int, x1_frac, x2_int, x2_frac, discriminant;
}
```

### 3. 🖥️ `src/modules/ui_functions.cpp` - UI & Display
**Chức năng**: Giao diện người dùng và layout
```cpp
// Functions được implement:
void clearScreen()                    // Xóa màn hình
void drawBorder()                     // Vẽ viền
void printCentered(text, line, color) // In text ở giữa
void printAt(text, x, y, color)       // In text tại vị trí
void displayMainMenu()                // Hiển thị menu chính
void displayHelpScreen()              // Hiển thị màn hình help
void displayAboutScreen()             // Hiển thị màn hình about
void displayBootScreen()              // Hiển thị màn hình boot
```

### 4. ⚙️ `src/modules/app_logic.cpp` - Application Logic
**Chức năng**: Logic ứng dụng, state management và keyboard handling
```cpp
// Functions được implement:
void shutdown()                      // Tắt hệ thống
void displayQuadraticInterface()     // Hiển thị giao diện phương trình
void displayCurrentInput()           // Hiển thị input hiện tại

// Class được implement:
class PrintfKeyboardEventHandler     // Xử lý keyboard events

// Global variables:
int32_t coeff_a, coeff_b, coeff_c   // Hệ số phương trình
char input_buffer[100]               // Buffer input
InterfaceState current_state         // Trạng thái hiện tại
```

### 5. 🎯 `src/kernel_clean.cpp` - Clean Kernel
**Chức năng**: Kernel chính được refactor với forward declarations rõ ràng
```cpp
// Chỉ chứa:
- Forward declarations cho tất cả functions
- Kernel initialization logic
- System setup
- Main loop

// Chia thành sections rõ ràng:
- FORWARD DECLARATIONS FOR OUR MODULES
- KERNEL INITIALIZATION  
- BOOT SCREEN
- SYSTEM INITIALIZATION
- DRIVER INITIALIZATION
- START APPLICATION
- MAIN LOOP
```

## 🔄 Makefile Updates

Đã thêm targets mới:
```bash
# Build original version
make original
make qemu

# Build clean refactored version  
make clean-refactored
make qemu-clean

# Build all versions
make all-versions
```

## 📊 So sánh

| **Aspect** | **Trước** | **Sau** |
|------------|-----------|---------|
| **File structure** | 1 file 642 dòng | 5 files in modules/ + kernel |
| **Readability** | Khó đọc, lộn xộn | Rõ ràng, có tổ chức |
| **Maintainability** | Khó maintain | Dễ dàng tìm và sửa |
| **Modularity** | Monolithic | Modular components |
| **Testing** | Khó test | Có thể test từng module |
| **Code reuse** | Khó tái sử dụng | Dễ dàng import functions |

## 🎯 Lợi ích

### ✅ **Separation of Concerns**
- Mỗi file có một trách nhiệm cụ thể
- Dễ dàng tìm kiếm functions theo chức năng

### ✅ **Easy to Read** 
- Code được tổ chức logic
- Comments và sections rõ ràng
- Forward declarations giúp hiểu structure

### ✅ **Easy to Import**
- Chỉ cần forward declare functions cần dùng
- Không cần complex header files
- Linker tự động link các object files

### ✅ **Easy to Maintain**
- Bug fix chỉ cần sửa trong 1 file cụ thể
- Thêm feature mới vào đúng file chức năng
- Code review dễ dàng hơn

### ✅ **Scalable**
- Dễ dàng thêm modules mới
- Có thể tách tiếp thành sub-modules
- Cấu trúc sẵn sàng cho large projects

## 🚀 Cách sử dụng

### Build & Run:
```bash
# Clean build
make clean

# Build refactored version
make clean-refactored

# Run in QEMU
make qemu-clean
```

### Development:
1. **Thêm math function mới** → Sửa `src/modules/math_functions.cpp`
2. **Thêm UI screen mới** → Sửa `src/modules/ui_functions.cpp`  
3. **Thêm keyboard shortcut** → Sửa `src/modules/app_logic.cpp`
4. **Thêm print function** → Sửa `src/modules/printf.cpp`
5. **Kernel changes** → Sửa `src/kernel_clean.cpp`

## 🔮 Future Improvements

1. **Thêm modules mới**:
   - `file_system.cpp` - File operations
   - `network.cpp` - Network functions
   - `graphics.cpp` - Advanced graphics

2. **Header files** (optional):
   - Có thể tạo `.h` files nếu cần type safety
   - Hiện tại dùng forward declarations cho simplicity

3. **Unit testing**:
   - Test từng module riêng biệt
   - Mock các dependencies

Code giờ đây clean, organized và professional! 🎉 
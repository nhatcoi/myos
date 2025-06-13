# Modules Directory

Thư mục này chứa các module được tách ra từ `kernel.cpp` gốc để tổ chức code theo nguyên tắc **Separation of Concerns**.

## 📁 Các modules:

### 🖨️ `printf.cpp` - Print Functions
- `printf()` - In string cơ bản
- `printfHex()` - In số hex  
- `printInt()` - In số nguyên
- `printIntAt()` - In số tại vị trí cụ thể

### 🧮 `math_functions.cpp` - Math Operations
- `sqrt_int()` - Căn bậc hai số nguyên
- `stringToInt()` - Chuyển string thành int
- `solveQuadratic()` - Giải phương trình bậc 2
- `QuadraticResult` struct

### 🖥️ `ui_functions.cpp` - UI & Display  
- `clearScreen()` - Xóa màn hình
- `drawBorder()` - Vẽ viền
- `printCentered()` - In text ở giữa
- `displayMainMenu()` - Menu chính
- `displayHelpScreen()` - Màn hình help
- `displayAboutScreen()` - Màn hình about
- `displayBootScreen()` - Màn hình boot

### ⚙️ `app_logic.cpp` - Application Logic
- `shutdown()` - Tắt hệ thống
- `displayQuadraticInterface()` - Giao diện phương trình
- `displayCurrentInput()` - Hiển thị input
- Global variables: `coeff_a`, `coeff_b`, `coeff_c`, `input_buffer`, `current_state`

## 🔗 Usage trong kernel:

```cpp
// Forward declarations
void printf(char* str);
int32_t sqrt_int(int32_t x);
void displayMainMenu();
void shutdown();

// Linker sẽ tự động link các object files
```

## 🎯 Lợi ích:

- ✅ **Modular**: Mỗi file có chức năng riêng biệt
- ✅ **Maintainable**: Dễ dàng tìm và sửa lỗi
- ✅ **Reusable**: Functions có thể tái sử dụng
- ✅ **Organized**: Code được tổ chức khoa học 



# Build original version
make original
make qemu

# Build clean refactored version với modules
make clean-refactored  
make qemu-clean

# Clean all
make clean
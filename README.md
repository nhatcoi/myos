# MyOS

<div align="center">
  <img src="https://img.shields.io/badge/Language-C++-blue.svg" alt="Language">
  <img src="https://img.shields.io/badge/Architecture-x86-green.svg" alt="Architecture">
  <img src="https://img.shields.io/badge/License-MIT-yellow.svg" alt="License">
  <img src="https://img.shields.io/badge/Status-Active-success.svg" alt="Status">
</div>

## 📋 Mục Lục

- [Tổng Quan](#tổng-quan)
- [Tính Năng](#tính-năng)  
- [Kiến Thức Nền Tảng](#kiến-thức-nền-tảng)
- [Cách Thức Hoạt Động](#cách-thức-hoạt-động)
- [Cài Đặt và Chạy](#cài-đặt-và-chạy)
- [Cấu Trúc Dự Án](#cấu-trúc-dự-án)
- [Hướng Dẫn Sử Dụng](#hướng-dẫn-sử-dụng)
- [Phát Triển](#phát-triển)
- [Troubleshooting](#troubleshooting)
- [Credits](#credits)

## 🌟 Tổng Quan

MyOS là một hệ điều hành đơn giản được phát triển từ đầu (từ kernel) bằng C++ và Assembly, chuyên biệt cho việc giải phương trình bậc 2. Dự án này được thiết kế như một công cụ học tập để hiểu về:

- **Kernel Development**: Phát triển kernel từ cơ bản
- **Operating System Concepts**: Các khái niệm hệ điều hành
- **Low-level Programming**: Lập trình mức thấp
- **Hardware Interaction**: Tương tác với phần cứng

### 🎯 Mục Tiêu Dự Án

1. **Giáo dục**: Học về phát triển OS từ cơ bản
2. **Thực hành**: Áp dụng kiến thức hệ điều hành
3. **Chuyên biệt**: Tập trung vào một tính năng cụ thể (giải PT bậc 2)
4. **Cross-platform**: Chạy được trên nhiều emulator/virtualizer

## ✨ Tính Năng

### 🔢 Chức Năng Chính
- **Giải phương trình bậc 2**: ax² + bx + c = 0
- **Hỗ trợ số âm**: Nhập được hệ số âm
- **Kết quả chính xác**: Tính toán với độ chính xác cao
- **Validation**: Kiểm tra dữ liệu đầu vào (a ≠ 0)

### 🖥️ Giao Diện
- **Menu tiếng Việt**: Giao diện hoàn toàn bằng tiếng Việt
- **ASCII Art Logo**: Logo đẹp mắt khi khởi động
- **Navigation**: Điều hướng bằng phím số
- **Help System**: Hệ thống trợ giúp tích hợp

### 🔧 Tính Năng Kỹ Thuật
- **Kernel 32-bit**: Kernel x86 32-bit
- **Memory Management**: Quản lý bộ nhớ tùy chỉnh
- **Interrupt Handling**: Xử lý ngắt phần cứng
- **Keyboard Driver**: Driver bàn phím tối ưu
- **VGA Output**: Xuất hình ảnh VGA text mode

## 📚 Kiến Thức Nền Tảng

### 🏗️ Operating System Concepts

#### Kernel
- **Monolithic Kernel**: MyOS sử dụng kiến trúc kernel đơn khối
- **Kernel Space vs User Space**: Hiện tại chỉ có kernel space
- **Boot Process**: Quá trình khởi động từ bootloader đến kernel

#### Memory Management
```cpp
class MemoryManager {
    MemoryChunk* first;
    void* malloc(size_t size);
    void free(void* ptr);
};
```
- **Heap Management**: Quản lý heap tùy chỉnh
- **Memory Allocation**: Cấp phát bộ nhớ động
- **Fragmentation**: Xử lý phân mảnh bộ nhớ

#### Hardware Abstraction
```cpp
class Port8Bit {
    uint16_t portnumber;
    virtual uint8_t Read();
    virtual void Write(uint8_t data);
};
```
- **Port I/O**: Giao tiếp với hardware qua port
- **Interrupt Handling**: Xử lý ngắt từ keyboard
- **Driver Model**: Mô hình driver cho các thiết bị

### 🔧 Low-Level Programming

#### Assembly Integration
```assembly
.section .multiboot
    .long 0x1BADB002
    .long 0x00
    .long -(0x1BADB002 + 0x00)
```
- **Multiboot Header**: Tuân thủ Multiboot specification
- **GDT Setup**: Thiết lập Global Descriptor Table
- **Interrupt Stubs**: Assembly stubs cho interrupt handlers

#### C++ Features Used
- **Classes and Objects**: OOP design patterns
- **Virtual Functions**: Polymorphism cho drivers
- **Operator Overloading**: Custom new/delete operators
- **Namespaces**: Tổ chức code với namespaces

## ⚙️ Cách Thức Hoạt Động

### 🚀 Boot Process

1. **BIOS/UEFI**: Firmware khởi tạo hệ thống
2. **Bootloader (GRUB)**: Load kernel vào memory
3. **Kernel Entry**: Chạy hàm `kernelMain()`
4. **Hardware Init**: Khởi tạo GDT, IDT, Memory Manager
5. **Driver Loading**: Load keyboard và VGA drivers
6. **Main Loop**: Vào vòng lặp chính của OS

### 🧠 Memory Layout

```
High Memory  +------------------+
             |    Kernel Stack   |
             +------------------+
             |    Heap Space     |
             +------------------+
             |    Kernel Code    |
             +------------------+
Low Memory   |    Boot Sector    |
             +------------------+
```

### ⌨️ Input Processing

```cpp
void OnKeyDown(char c) {
    switch(current_state) {
        case MAIN_MENU:
            handleMenuNavigation(c);
            break;
        case WAITING_A:
        case WAITING_B:
        case WAITING_C:
            handleCoefficientInput(c);
            break;
    }
}
```

### 🔢 Mathematical Algorithm

```cpp
QuadraticResult solveQuadratic(int32_t a, int32_t b, int32_t c) {
    int32_t discriminant = b*b - 4*a*c;
    
    if(discriminant < 0) {
        result.hasRealRoots = false;
    } else {
        int32_t sqrt_disc = sqrt_int(discriminant);
        result.x1_int = (-b + sqrt_disc) / (2*a);
        result.x2_int = (-b - sqrt_disc) / (2*a);
        // Handle fractional parts...
    }
    return result;
}
```

## 🛠️ Cài Đặt và Chạy

### 📋 Yêu Cầu Hệ Thống

#### Linux/WSL
```bash
sudo apt-get install build-essential
sudo apt-get install gcc-multilib g++-multilib
sudo apt-get install grub-legacy xorriso
sudo apt-get install qemu-system-x86
```

#### macOS
```bash
brew install i686-elf-binutils    # Assembler, linker, etc.
brew install i686-elf-gcc         # C/C++ compiler
brew install qemu   
brew install i686-elf-grub       # GRUB bootloader
brew install xorriso              # ISO 9660 filesystem
brew install mtools               # MS-DOS filesystem tools
```


#### Windows
- WSL2 với Ubuntu
- QEMU for Windows
- VirtualBox

### 🚀 Build và Chạy

#### 1. Clone Repository
```bash
git clone <repository-url>
cd wyoos-ptb2
```

#### 2. Build Commands
```bash
# Dọn dẹp build files
make clean

# Build phiên bản monolithic (kernel.cpp)
make original

# Build phiên bản modular (kernel_clean.cpp + modules)
make clean-refactored

# Build cả hai phiên bản
make all-versions
```

#### 3. Run Commands
```bash
# Chạy phiên bản monolithic
make qemu

# Chạy phiên bản modular
make qemu-clean

# Chạy với debug mode (GDB debugging)
make qemu-debug
```

#### 4. UTM/VirtualBox/VMware
```bash
# Sử dụng file ISO đã build
# Original version: mykernel.iso
# Modular version: mykernel_clean.iso

# Hoặc chạy trực tiếp với qemu-system
qemu-system-i386 -cdrom mykernel.iso -m 256M
qemu-system-i386 -cdrom mykernel_clean.iso -m 256M
```

### 🖥️ Cấu Hình Virtual Machine

#### QEMU Settings
- **Architecture**: i386
- **Memory**: 128MB - 256MB
- **Boot**: CD-ROM first
- **Display**: VGA

#### UTM Settings
- **Architecture**: x86_64 (emulation)
- **System**: Standard PC (i440FX + PIIX)
- **Memory**: 256MB
- **Boot Order**: CD/DVD first

#### VirtualBox Settings
- **Type**: Other/Unknown
- **Memory**: 256MB
- **Boot Order**: CD/DVD first
- **Enable VT-x/AMD-V**: Yes

## 📁 Cấu Trúc Dự Án

```
wyoos-ptb2/
├── src/                          # Source code chính
│   ├── kernel.cpp               # ⭐ Kernel gốc (monolithic)
│   ├── kernel_clean.cpp         # 🆕 Kernel refactored (modular)
│   ├── loader.s                 # Assembly bootloader
│   ├── gdt.cpp                  # Global Descriptor Table
│   ├── memorymanagement.cpp     # Quản lý bộ nhớ
│   ├── multitasking.cpp         # Đa nhiệm (prepared but unused)
│   ├── syscalls.cpp             # System calls
│   │
│   ├── modules/                 # 🆕 MODULES TÁCH RỜI
│   │   ├── printf.cpp           # Print functions
│   │   ├── math_functions.cpp   # Math operations (sqrt, solve)
│   │   ├── ui_functions.cpp     # UI functions (menu, display)
│   │   ├── app_logic.cpp        # Application logic & states
│   │   └── README.md            # Module documentation
│   │
│   ├── drivers/                 # Hardware drivers
│   │   ├── driver.cpp           # Base driver class
│   │   ├── keyboard.cpp         # ⭐ Keyboard driver (core)
│   │   └── vga.cpp              # ⭐ VGA text mode (core)
│   │
│   └── hardwarecommunication/   # Hardware communication
│       ├── interrupts.cpp       # ⭐ Interrupt management
│       ├── interruptstubs.s     # Assembly interrupt stubs
│       └── port.cpp             # ⭐ Port I/O operations
│
├── include/                      # Header files
│   ├── common/
│   │   └── types.h              # Common type definitions
│   ├── drivers/
│   │   ├── driver.h
│   │   ├── keyboard.h           # ⭐ Core header
│   │   └── vga.h                # ⭐ Core header
│   ├── hardwarecommunication/
│   │   ├── interrupts.h         # ⭐ Core header
│   │   └── port.h               # ⭐ Core header
│   ├── gdt.h
│   ├── memorymanagement.h       # ⭐ Core header
│   ├── multitasking.h
│   └── syscalls.h
│
├── docs/                         # 🆕 DOCUMENTATION
│   ├── MAIN.md                  # Kernel & Memory (Nhat)
│   ├── UI.md                    # User Interface (Minh)
│   ├── INPUT.md                 # Input Handling (Ngoc)
│   ├── LOGIC.md                 # Math Algorithms (Sang)
│   └── TESTING.md               # Testing & QA (Quynh)
│
├── grub/                         # 🆕 GRUB configuration
├── obj/                          # Compiled object files (auto-generated)
├── .git/                         # Git repository
├── .gitignore                    # Git ignore rules
│
├── mykernel.bin                  # Kernel binary (generated)
├── mykernel.iso                  # Bootable ISO original (generated)
├── mykernel_clean.bin            # Clean kernel binary (generated)
├── mykernel_clean.iso            # Clean bootable ISO (generated)
├── linker.ld                     # Linker script
├── makefile                      # 🔧 Enhanced build configuration
├── LICENSE                       # 🆕 MIT License
└── README.md                     # 📖 Comprehensive documentation
```

### 🎯 **Ký hiệu giải thích:**
- ⭐ **Core components**: Thành phần cốt lõi của hệ thống
- 🆕 **New additions**: Thành phần mới được thêm vào
- 🔧 **Enhanced**: Thành phần được cải tiến
- 📖 **Documentation**: Tài liệu và hướng dẫn

### 🔄 **Hai phiên bản kernel:**
- **kernel.cpp**: Phiên bản monolithic gốc (tất cả code trong 1 file)
- **kernel_clean.cpp**: Phiên bản modular tối ưu (sử dụng modules tách rời + tách hàm main)

### 📦 **Build targets:**
```bash
make original           # Build kernel.cpp
make clean-refactored   # Build kernel_clean.cpp với modules
make qemu              # Run original version
make qemu-clean        # Run modular version
```

### 🔍 File Chi Tiết

#### 🔬 **Core System Files**
- **`kernel.cpp`**: Kernel monolithic gốc (tất cả logic trong 1 file)
- **`kernel_clean.cpp`**: Kernel modular tối ưu với:
  - ✅ Sử dụng forward declarations cho modules
  - ✅ Tách hàm `kernelMain` thành các hàm nhỏ:
    - `initializeBootScreen()` - Hiển thị boot screen
    - `initializeSystemCore()` - Khởi tạo GDT, Memory, TaskManager, Interrupts  
    - `initializeDrivers()` - Khởi tạo driver manager và keyboard
    - `startApplication()` - Bắt đầu ứng dụng và kích hoạt interrupts
    - `runMainLoop()` - Vòng lặp chính của OS
  - ✅ Sử dụng struct để nhóm các component liên quan
- **`gdt.cpp`**: Global Descriptor Table - segment management
- **`memorymanagement.cpp`**: Dynamic memory allocation (10MB heap)
- **`loader.s`**: Assembly entry point với multiboot header

#### 📦 **Modules (Separation of Concerns)**
- **`printf.cpp`**: Print functions (printf, printfHex, printInt, printIntAt)
- **`math_functions.cpp`**: Math operations (sqrt_int, stringToInt, solveQuadratic)
- **`ui_functions.cpp`**: UI functions (clearScreen, drawBorder, displayMenus)
- **`app_logic.cpp`**: Application logic (states, input handling, shutdown)

#### 🎛️ **Hardware Drivers**
- **`keyboard.cpp`**: PS/2 keyboard driver với UTM/QEMU compatibility
- **`vga.cpp`**: VGA text mode (80x25, 16 colors, 0xB8000)
- **`driver.cpp`**: Base class cho tất cả drivers

#### 🔌 **Hardware Communication**
- **`interrupts.cpp`**: IDT setup, interrupt routing, keyboard IRQ1
- **`port.cpp`**: Port I/O abstraction (inb/outb, shutdown port 0x604)

#### 📚 **Documentation**
- **`docs/MAIN.md`**: Kernel & Memory Management (Nhat)
- **`docs/UI.md`**: User Interface & Display (Minh)
- **`docs/INPUT.md`**: Keyboard Driver & Input (Ngoc)
- **`docs/LOGIC.md`**: Mathematical Algorithms (Sang)
- **`docs/TESTING.md`**: Testing & Documentation (Quynh)

## 📖 Hướng Dẫn Sử Dụng

### 🎮 Điều Khiển

#### Menu Chính
- **1**: Giải phương trình bậc 2
- **2**: Xem hướng dẫn
- **3**: Thông tin phát triển
- **4**: Thoát (QEMU only)
- **ESC**: Quay lại menu chính

#### Nhập Hệ Số
- **Số (0-9)**: Nhập chữ số
- **- (dấu trừ)**: Hệ số âm (chỉ ở đầu)
- **Enter**: Xác nhận và chuyển hệ số tiếp theo
- **Backspace**: Xóa ký tự cuối
- **ESC**: Quay lại menu chính

#### Xem Kết Quả
- **R**: Giải phương trình mới
- **ESC**: Quay lại menu chính

### 🔢 Giải Phương Trình

#### Bước 1: Nhập hệ số a
- Hệ số a phải khác 0
- Có thể nhập số âm
- Nhấn Enter để xác nhận

#### Bước 2: Nhập hệ số b
- Có thể là 0, âm hoặc dương
- Nhấn Enter để tiếp tục

#### Bước 3: Nhập hệ số c
- Hệ số tự do
- Nhấn Enter để tính toán

#### Bước 4: Xem kết quả
- **Delta > 0**: Hai nghiệm phân biệt
- **Delta = 0**: Nghiệm kép
- **Delta < 0**: Vô nghiệm (phức)

### 📝 Ví Dụ Sử Dụng

```
Phương trình: x² - 5x + 6 = 0
Input: a = 1, b = -5, c = 6
Output: x₁ = 3, x₂ = 2

Phương trình: x² - 4x + 4 = 0  
Input: a = 1, b = -4, c = 4
Output: x = 2 (nghiệm kép)

Phương trình: x² + x + 1 = 0
Input: a = 1, b = 1, c = 1
Output: Vô nghiệm thực
```

## 👨‍💻 Phát Triển

### 🔧 Thêm Tính Năng Mới

#### 1. Thêm Menu Item
```cpp
// Trong displayMainMenu()
printAt("5. Tinh nang moi", 30, 15, 0x0F);

// Trong KeyboardEventHandler
case '5':
    current_state = NEW_FEATURE;
    displayNewFeature();
    break;
```

#### 2. Thêm Driver Mới
```cpp
class NewDriver : public Driver {
public:
    void Activate();
    int Reset();
};

// Trong kernelMain()
NewDriver newdriver(&interrupts);
drvManager.AddDriver(&newdriver);
```

#### 3. Thêm System Call
```cpp
// Trong syscalls.cpp
void SYSCALL_NEW_FUNCTION() {
    // Implementation
}
```

### 🧪 Testing

#### Unit Tests
```bash
# Test math functions
make test-math

# Test memory management  
make test-memory

# Test drivers
make test-drivers
```

#### Debug Mode
```bash
# Chạy với debug symbols
make qemu-debug

# Kết nối GDB
gdb mykernel.bin
(gdb) target remote localhost:1234
(gdb) continue
```

### 🐛 Known Issues

1. **Floating Point**: Chưa hỗ trợ FPU, dùng fixed-point arithmetic
2. **Multi-threading**: Chưa implement threading
3. **File System**: Chưa có file system
4. **Network**: Chưa hỗ trợ networking

## 🔧 Troubleshooting

### ❌ Build Errors

#### "No such file or directory"
```bash
# Đảm bảo có cross-compiler
which i686-elf-gcc
# Hoặc
which x86_64-elf-gcc
```

#### "Permission denied"
```bash
# Fix permissions
chmod +x setup_scripts.sh
sudo make install-deps
```

### 💥 Runtime Errors

#### "Boot hangs at logo"
- **UTM**: Kiểm tra architecture (x86_64, không phải ARM)
- **QEMU**: Thử `-m 256M` thay vì 128M
- **VirtualBox**: Enable VT-x/AMD-V

#### "Keyboard not working"
- Kiểm tra VM có USB controller
- UTM: Enable USB support in settings
- VirtualBox: Install Guest Additions

#### "Screen artifacts"
- Thử display settings khác
- UTM: Thử VGA thay vì virtio-gpu
- Increase video memory

### 🔍 Debug Tips

#### 1. Serial Output Debug
```cpp
// Thêm vào kernel.cpp
void serial_debug(const char* msg) {
    Port8Bit serial(0x3F8);
    while(*msg) {
        serial.Write(*msg++);
    }
}
```

#### 2. Memory Debug
```cpp
// Kiểm tra memory leaks
void debug_memory() {
    printf("Active memory manager: ");
    printf(MemoryManager::activeMemoryManager ? "OK" : "NULL");
}
```

#### 3. Interrupt Debug
```bash
# Chạy QEMU với monitor
qemu-system-i386 -cdrom mykernel.iso -monitor stdio

# Commands trong QEMU monitor
(qemu) info registers
(qemu) info memory
(qemu) info interrupts
```

### 📊 Performance Tips

#### 1. Optimization Flags
```makefile
GCCPARAMS += -O2 -ffast-math -funroll-loops
```

#### 2. Memory Allocation
```cpp
// Pre-allocate large objects
static char buffer[4096];
// Thay vì malloc/free nhiều lần
```

#### 3. Interrupt Efficiency
```cpp
// Minimize work in interrupt handlers
uint32_t HandleInterrupt(uint32_t esp) {
    // Process quickly
    return esp;
}
```

## 🏆 Credits

### 👥 Nhóm Phát Triển
- **Nhat**: Kernel & Memory Management - MAIN.md
- **Ngoc**: Keyboard Driver & Input Handling  - INPUT.md
- **Sang**: Mathematical Algorithms & Logic - LOGIC.md
- **Minh**: User Interface & Display - UI.md
- **Quynh**: Testing & Documentation - TESTING.md

### 📚 Tài Liệu Tham Khảo
- [OSDev Wiki](https://wiki.osdev.org/)
- [Intel x86 Manual](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)
- [Multiboot Specification](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)
- [James Molloy's Kernel Tutorial](http://www.jamesmolloy.co.uk/tutorial_html/)

### 🛠️ Tools & Libraries
- **GCC Cross Compiler**: i686-elf-gcc
- **GNU Assembler**: gas (GNU as)
- **GNU Linker**: ld
- **GRUB**: Bootloader
- **QEMU**: Emulator
- **UTM**: macOS virtualization
- **Xorriso**: ISO creation

### 📜 License
MIT License - Chi tiết xem file LICENSE

### 🤝 Contributing
Contributions welcome! Please read CONTRIBUTING.md for guidelines.

### 📧 Contact
- Project Repository: [GitHub Link]
- Documentation: [Wiki Link]
- Issues: [Issues Link]

---

<div align="center">
  <p><strong>MyOS v2.0 - Hệ Điều Hành Giải Phương Trình Bậc 2</strong></p>
  <p>Được phát triển với ❤️ bởi nhóm sinh viên IT</p>
</div>
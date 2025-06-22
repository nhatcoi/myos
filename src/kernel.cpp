#include <common/types.h>
#include <gdt.h>
#include <memorymanagement.h>
#include <hardwarecommunication/interrupts.h>
#include <syscalls.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/vga.h>
#include <multitasking.h>
#include <hardwarecommunication/port.h>

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;

// === FORWARD DECLARATIONS FOR OUR MODULES ===
// These functions are implemented in separate files for better organization

// import hàm từ printf.cpp
void printf(char* str);
void printfHex(uint8_t key);
void printInt(int32_t num);
void printIntAt(int32_t num, int x, int y, uint8_t color = 0x0F);

// import hàm từ math_functions.cpp
int32_t sqrt_int(int32_t x);
int32_t stringToInt(char* str, int len);
struct QuadraticResult {
    bool hasRealRoots;
    int32_t x1_int, x1_frac, x2_int, x2_frac, discriminant;
};
QuadraticResult solveQuadratic(int32_t a, int32_t b, int32_t c);

// import hàm từ ui_functions.cpp
void clearScreen();
void drawBorder();
void printCentered(char* text, int line, uint8_t color = 0x0F);
void printAt(char* text, int x, int y, uint8_t color = 0x0F);
void displayMainMenu();
void displayHelpScreen();
void displayAboutScreen();
void displayBootScreen();

// import hàm từ app_logic.cpp  
void shutdown();
void displayQuadraticInterface();
void displayCurrentInput();

// biến global từ app_logic.cpp
extern int32_t coeff_a, coeff_b, coeff_c;
extern char input_buffer[100];
extern int input_pos;
enum InterfaceState { 
    MAIN_MENU, 
    WAITING_A, 
    WAITING_B, 
    WAITING_C, 
    SHOWING_RESULT,
    HELP_SCREEN,
    ABOUT_SCREEN
};
extern InterfaceState current_state;

// Xử lí sự kiện keyboard cho OS
class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void OnKeyDown(char c)
    {
        // Xử lí menu chính
        if(current_state == MAIN_MENU) {
            switch(c) {
                case '1':
                    current_state = WAITING_A;
                    input_pos = 0;
                    coeff_a = coeff_b = coeff_c = 0;
                    displayQuadraticInterface();
                    displayCurrentInput();
                    return;
                case '2':
                    current_state = HELP_SCREEN;
                    displayHelpScreen();
                    return;
                case '3':
                    current_state = ABOUT_SCREEN;
                    displayAboutScreen();
                    return;
                case '4':
                    printf("Shutting down...\n");
                    shutdown(); // shutdown với qemu
                    return;
            }
            return;
        }
        
        // Xử lí ESC key - quay về menu chính
        if(c == 27) { // ESC key
            current_state = MAIN_MENU;
            displayMainMenu();
            return;
        }
        
        // Xử lí help và about screen
        if(current_state == HELP_SCREEN || current_state == ABOUT_SCREEN) {
            if(c == 27) { // ESC
                current_state = MAIN_MENU;
                displayMainMenu();
            }
            return;
        }
        
        // Xử lí result screen
        if(current_state == SHOWING_RESULT) {
            if(c == 'r' || c == 'R') {
                // Reset cho phương trình mới
                current_state = WAITING_A;
                input_pos = 0;
                coeff_a = coeff_b = coeff_c = 0;
                displayQuadraticInterface();
                displayCurrentInput();
                return;
            }
            return;
        }
        
        // Xử lí input cho hệ số
        if(current_state >= WAITING_A && current_state <= WAITING_C) {
            
            // Enter key
            if(c == '\n' || c == '\r') {
                if(input_pos > 0) {
                    input_buffer[input_pos] = '\0';
                    int32_t value = stringToInt(input_buffer, input_pos);
                    
                    // Check validate hệ số a != 0
                    if(current_state == WAITING_A && value == 0) {
                        printAt("Loi: He so a phai khac 0!", 10, 15, 0x0C);
                        return;
                    }
                    
                    switch(current_state) {
                        case WAITING_A:
                            coeff_a = value;
                            current_state = WAITING_B;
                            break;
                        case WAITING_B:
                            coeff_b = value;
                            current_state = WAITING_C;
                            break;
                        case WAITING_C:
                            coeff_c = value;
                            current_state = SHOWING_RESULT;
                            break;
                    }
                    
                    input_pos = 0;
                    displayQuadraticInterface();
                    if(current_state <= WAITING_C) {
                        displayCurrentInput();
                    }
                    return;
                }
            }
            
            // sự kiện backspace
            if(c == '\b' || c == 127) {
                if(input_pos > 0) {
                    input_pos--;
                    input_buffer[input_pos] = '\0';
                    displayCurrentInput();
                }
                return;
            }
            
            // sự kiện nhập số và dấu trừ
            if((c >= '0' && c <= '9') || (c == '-' && input_pos == 0)) {
                if(input_pos < 15) {
                    input_buffer[input_pos++] = c;
                    displayCurrentInput();
                }
                return;
            }
        }
    }
};

// === KERNEL INITIALIZATION ===

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/)
{
    // === BOOT SCREEN ===
    displayBootScreen();

    // Chờ 2s
    for (volatile int i = 0; i < 1000000000; ++i);

    // === SYSTEM INITIALIZATION ===
    
    // Khởi tạo GDT để quản lý các segment bộ nhớ trong chế độ protected mode của x86
    GlobalDescriptorTable gdt;
    
    // Khởi tạo Memory Management để quản lý cấp phát bộ nhớ động cho kernel
    // *memupper là lượng RAM trống phía trên low memory (thông tin lấy từ multiboot).
    // heap là lượng RAM dành cho kernel
    // MemoryManager là class quản lý bộ nhớ động cho kernel
    uint32_t* memupper = (uint32_t*)(((size_t)multiboot_structure) + 8);
    size_t heap = 10*1024*1024;
    MemoryManager memoryManager(heap, (*memupper)*1024 - heap - 10*1024);
    
    // Khởi tạo Task Manager để quản lý đa nhiệm (nhiều task cùng chạy)
    TaskManager taskManager;
    
    // 	•	Quản lý các ngắt phần cứng và phần mềm.
	// •	0x20 là offset vector cho interrupt.
	// •	Kết nối với GDT và TaskManager để hỗ trợ ngắt đa nhiệm.
    InterruptManager interrupts(0x20, &gdt, &taskManager);
    
    // === DRIVER INITIALIZATION ===
    
    // Khởi tạo Driver Manager để quản lý các driver thiết bị
    DriverManager drvManager;
    
    // Khởi tạo KeyboardEventHandler để xử lý sự kiện bàn phím
    PrintfKeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts, &kbhandler);
    drvManager.AddDriver(&keyboard);
    
    // Kích hoạt tất cả các driver
    drvManager.ActivateAll();
    
    // === START APPLICATION ===
    
    // Hiển thị menu chính
    displayMainMenu();
    
    // Kích hoạt các interrupt
    interrupts.Activate();
    
    // === MAIN LOOP ===
    // OS đang chạy!
    while(1)
    {
       
    }
} 
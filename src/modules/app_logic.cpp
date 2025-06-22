                         #include <common/types.h>
#include <drivers/keyboard.h>
#include <hardwarecommunication/port.h>

using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;

// import simple functions
void printf(char* str);
void printfHex(uint8_t key);
void printInt(int32_t num);
void printIntAt(int32_t num, int x, int y, uint8_t color = 0x0F);
int32_t stringToInt(char* str, int len);

// Import decimal support
#define DECIMAL_SCALE 1000

struct DecimalNumber {
    int32_t value;  // Giá trị nhân với DECIMAL_SCALE
    
    DecimalNumber() : value(0) {}
    DecimalNumber(int32_t val) : value(val * DECIMAL_SCALE) {}
    DecimalNumber(int32_t int_part, int32_t frac_part) : 
        value(int_part * DECIMAL_SCALE + frac_part) {}
};

DecimalNumber stringToDecimal(char* str, int len);

// struct kết quả ptb2 với decimal support
struct QuadraticResult {
    bool hasRealRoots;
    DecimalNumber x1;     // Nghiệm 1
    DecimalNumber x2;     // Nghiệm 2  
    DecimalNumber discriminant;
    
    // Legacy fields for backward compatibility
    int32_t x1_int;  
    int32_t x1_frac; 
    int32_t x2_int;   
    int32_t x2_frac; 
};

// import functions từ ui
QuadraticResult solveQuadraticDecimal(DecimalNumber a, DecimalNumber b, DecimalNumber c);
QuadraticResult solveQuadratic(int32_t a, int32_t b, int32_t c);
void displayMainMenu();
void displayHelpScreen();
void displayAboutScreen();
void printAt(char* text, int x, int y, uint8_t color = 0x0F);
void printCentered(char* text, int line, uint8_t color = 0x0F);
void clearScreen();
void drawBorder();

// Biến global cho ui - thay đổi thành decimal
DecimalNumber coeff_a, coeff_b, coeff_c;
char input_buffer[100];
int input_pos = 0;
enum InterfaceState { 
    MAIN_MENU, 
    WAITING_A, 
    WAITING_B, 
    WAITING_C, 
    SHOWING_RESULT,
    HELP_SCREEN,
    ABOUT_SCREEN
};
InterfaceState current_state = MAIN_MENU;

void shutdown()
{
    // Lệnh đặc biệt cho QEMU
    Port16Bit port(0x604);
    port.Write(0x2000);
}

// Hàm hiển thị decimal number
void printDecimalAt(DecimalNumber num, int x, int y, uint8_t color = 0x0F)
{
    int32_t int_part = num.value / DECIMAL_SCALE;
    int32_t frac_part = num.value % DECIMAL_SCALE;
    if(frac_part < 0) frac_part = -frac_part;
    
    // Print integer part
    printIntAt(int_part, x, y, color);
    
    // Calculate position after integer part
    int pos = x;
    if(int_part == 0) pos += 1;
    else {
        int32_t temp = int_part;
        if(temp < 0) { temp = -temp; pos += 1; } // cho dấu trừ
        while(temp > 0) { temp /= 10; pos++; }
    }
    
    // Print decimal point and fractional part if needed
    if(frac_part > 0) {
        printAt(".", pos, y, color);
        pos++;
        
        // Print fractional part với leading zeros
        if(frac_part < 100) {
            printAt("0", pos, y, color);
            pos++;
        }
        if(frac_part < 10) {
            printAt("0", pos, y, color);
            pos++;
        }
        printIntAt(frac_part, pos, y, color);
    }
}

// Hàm hiển thị giao diện ptb2
void displayQuadraticInterface()
{
    clearScreen();
    drawBorder();
    
    printCentered("GIAI PHUONG TRINH BAC 2", 2, 0x0E);
    printCentered("======================", 3, 0x0E);
    
    printAt("Dang tong quat: ax^2 + bx + c = 0", 22, 5, 0x0B);
    printCentered("(Cho phep nhap so thap phan: vd 1.5, -2.75)", 6, 0x08);

    switch(current_state) {
        case WAITING_A:
            printAt("Nhap he so a (a != 0): ", 10, 8, 0x0F);
            break;
        case WAITING_B:
            printAt("He so a = ", 10, 8, 0x0A);
            printDecimalAt(coeff_a, 20, 8, 0x0E);
            printAt("Nhap he so b: ", 10, 10, 0x0F);
            break;
        case WAITING_C:
            printAt("He so a = ", 10, 8, 0x0A);
            printDecimalAt(coeff_a, 20, 8, 0x0E);
            printAt("He so b = ", 10, 9, 0x0A);
            printDecimalAt(coeff_b, 20, 9, 0x0E);
            printAt("Nhap he so c: ", 10, 11, 0x0F);
            break;
        case SHOWING_RESULT:
            // Display equation with actual values
            printAt("Phuong trinh: ", 10, 8, 0x0B);
            printDecimalAt(coeff_a, 24, 8, 0x0E);
            printAt("x^2 + ", 30, 8, 0x0B);
            printDecimalAt(coeff_b, 36, 8, 0x0E);
            printAt("x + ", 42, 8, 0x0B);
            printDecimalAt(coeff_c, 46, 8, 0x0E);
            printAt(" = 0", 52, 8, 0x0B);
            
            QuadraticResult result = solveQuadraticDecimal(coeff_a, coeff_b, coeff_c);
            
            if(!result.hasRealRoots) {
                printAt("KET QUA: Phuong trinh vo nghiem", 10, 12, 0x0C);
                printAt("(Discriminant < 0)", 10, 13, 0x0C);
            } else if(result.discriminant.value == 0) {
                printAt("KET QUA: Nghiem kep", 10, 12, 0x0A);
                printAt("x = ", 10, 13, 0x0A);
                printDecimalAt(result.x1, 14, 13, 0x0E);
            } else {
                printAt("KET QUA: Co 2 nghiem phan biet", 10, 12, 0x0A);
                printAt("x1 = ", 10, 13, 0x0A);
                printDecimalAt(result.x1, 15, 13, 0x0E);
                
                printAt("x2 = ", 10, 14, 0x0A);
                printDecimalAt(result.x2, 15, 14, 0x0E);
            }
            
            printCentered("Nhan 'r' de giai lai, ESC de ve menu", 20, 0x0C);
            break;
    }
    
    if(current_state >= WAITING_A && current_state <= WAITING_C) {
        printCentered("Nhan Enter de xac nhan, ESC de ve menu", 20, 0x07);
        printCentered("Su dung dau '.' cho so thap phan", 21, 0x08);
    }
}

void displayCurrentInput()
{
    uint16_t* VideoMemory = (uint16_t*)0xb8000;
    
    // Xác định vị trí input dựa trên trạng thái hiện tại
    int input_x = (current_state == WAITING_A) ? 33 : 
                  (current_state == WAITING_B) ? 24 : 24;
    int input_y = (current_state == WAITING_A) ? 8 : 
                  (current_state == WAITING_B) ? 10 : 11;
    
    // Xóa input area (20 characters)
    for(int i = 0; i < 20; i++) {
        VideoMemory[input_y*80 + input_x + i] = 0x0720;
    }
    
    // Hiển thị input hiện tại
    for(int i = 0; i < input_pos; i++) {
        VideoMemory[input_y*80 + input_x + i] = 0x0F00 | input_buffer[i];
    }
    
    // Hiển thị cursor
    VideoMemory[input_y*80 + input_x + input_pos] = 0x7000 | '_';
}

// PrintfKeyboardEventHandler class is now defined in kernel_clean.cpp
// All keyboard handling logic moved there for better organization
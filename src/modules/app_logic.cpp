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

// struct kết quả ptb2
struct QuadraticResult {
    bool hasRealRoots;
    int32_t x1_int;  
    int32_t x1_frac; 
    int32_t x2_int;   
    int32_t x2_frac; 
    int32_t discriminant;
};

// import functions từ ui
QuadraticResult solveQuadratic(int32_t a, int32_t b, int32_t c);
void displayMainMenu();
void displayHelpScreen();
void displayAboutScreen();
void printAt(char* text, int x, int y, uint8_t color = 0x0F);
void printCentered(char* text, int line, uint8_t color = 0x0F);
void clearScreen();
void drawBorder();

// Biến global cho ui
int32_t coeff_a = 0, coeff_b = 0, coeff_c = 0;
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

// Hàm hiển thị giao diện ptb2
void displayQuadraticInterface()
{
    clearScreen();
    drawBorder();
    
    printCentered("GIAI PHUONG TRINH BAC 2", 2, 0x0E);
    printCentered("======================", 3, 0x0E);
    
    printAt("Dang tong quat: ax^2 + bx + c = 0", 22, 5, 0x0B);

    switch(current_state) {
        case WAITING_A:
            printAt("Nhap he so a (a != 0): ", 10, 8, 0x0F);
            break;
        case WAITING_B:
            printAt("He so a = ", 10, 8, 0x0A);
            printIntAt(coeff_a, 20, 8, 0x0E);
            printAt("Nhap he so b: ", 10, 10, 0x0F);
            break;
        case WAITING_C:
            printAt("He so a = ", 10, 8, 0x0A);
            printIntAt(coeff_a, 20, 8, 0x0E);
            printAt("He so b = ", 10, 9, 0x0A);
            printIntAt(coeff_b, 20, 9, 0x0E);
            printAt("Nhap he so c: ", 10, 11, 0x0F);
            break;
        case SHOWING_RESULT:
            // Display equation with actual values
            printAt("Phuong trinh: ", 10, 8, 0x0B);
            printIntAt(coeff_a, 24, 8, 0x0E);
            printAt("x^2 + ", 27, 8, 0x0B);
            printIntAt(coeff_b, 33, 8, 0x0E);
            printAt("x + ", 36, 8, 0x0B);
            printIntAt(coeff_c, 40, 8, 0x0E);
            printAt(" = 0", 43, 8, 0x0B);
            
            QuadraticResult result = solveQuadratic(coeff_a, coeff_b, coeff_c);
            
            if(!result.hasRealRoots) {
                printAt("KET QUA: Phuong trinh vo nghiem", 10, 12, 0x0C);
                printAt("(Discriminant < 0)", 10, 13, 0x0C);
            } else if(result.discriminant == 0) {
                printAt("KET QUA: Nghiem kep", 10, 12, 0x0A);
                printAt("x = ", 10, 13, 0x0A);
                printIntAt(result.x1_int, 14, 13, 0x0E);
                if(result.x1_frac > 0) {
                    printAt(".", 17, 13, 0x0E);
                    if(result.x1_frac < 100) printAt("0", 18, 13, 0x0E);
                    if(result.x1_frac < 10) printAt("0", 19, 13, 0x0E);
                    printIntAt(result.x1_frac, result.x1_frac < 100 ? (result.x1_frac < 10 ? 20 : 19) : 18, 13, 0x0E);
                }
            } else {
                printAt("KET QUA: Co 2 nghiem phan biet", 10, 12, 0x0A);
                printAt("x1 = ", 10, 13, 0x0A);
                printIntAt(result.x1_int, 15, 13, 0x0E);
                if(result.x1_frac > 0) {
                    printAt(".", 18, 13, 0x0E);
                    if(result.x1_frac < 100) printAt("0", 19, 13, 0x0E);
                    if(result.x1_frac < 10) printAt("0", 20, 13, 0x0E);
                    printIntAt(result.x1_frac, result.x1_frac < 100 ? (result.x1_frac < 10 ? 21 : 20) : 19, 13, 0x0E);
                }
                
                printAt("x2 = ", 10, 14, 0x0A);
                printIntAt(result.x2_int, 15, 14, 0x0E);
                if(result.x2_frac > 0) {
                    printAt(".", 18, 14, 0x0E);
                    if(result.x2_frac < 100) printAt("0", 19, 14, 0x0E);
                    if(result.x2_frac < 10) printAt("0", 20, 14, 0x0E);
                    printIntAt(result.x2_frac, result.x2_frac < 100 ? (result.x2_frac < 10 ? 21 : 20) : 19, 14, 0x0E);
                }
            }
            
            printCentered("Nhan 'r' de giai lai, ESC de ve menu", 20, 0x0C);
            break;
    }
    
    if(current_state >= WAITING_A && current_state <= WAITING_C) {
        printCentered("Nhan Enter de xac nhan, ESC de ve menu", 20, 0x07);
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
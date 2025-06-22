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

void printf(char* str)
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;
    static uint8_t x=0,y=0;

    for(int i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                x = 0;
                y++;
                break;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }

        if(x >= 80)
        {
            x = 0;
            y++;
        }

        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}

void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}

// Simple integer square root using Newton's method
int32_t sqrt_int(int32_t x)
{
    if(x <= 1) return x;
    
    int32_t guess = x / 2;
    int32_t prev_guess;
    
    for(int i = 0; i < 10; i++) // limit iterations
    {
        prev_guess = guess;
        guess = (guess + x / guess) / 2;
        if(guess == prev_guess) break;
    }
    return guess;
}

// Convert string to integer
int32_t stringToInt(char* str, int len)
{
    int32_t result = 0;
    int32_t sign = 1;
    int start = 0;
    
    if(str[0] == '-') {
        sign = -1;
        start = 1;
    }
    
    for(int i = start; i < len; i++) {
        if(str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
        }
    }
    return result * sign;
}

// Quadratic equation solver struct
struct QuadraticResult {
    bool hasRealRoots;
    int32_t x1_int;  // integer part of first root
    int32_t x1_frac; // fractional part * 1000
    int32_t x2_int;  // integer part of second root  
    int32_t x2_frac; // fractional part * 1000
    int32_t discriminant;
};

// Solve quadratic equation ax² + bx + c = 0
QuadraticResult solveQuadratic(int32_t a, int32_t b, int32_t c)
{
    QuadraticResult result;
    
    if(a == 0) {
        // Not a quadratic equation
        result.hasRealRoots = false;
        return result;
    }
    
    // Calculate discriminant: b² - 4ac
    result.discriminant = b * b - 4 * a * c;
    
    if(result.discriminant < 0) {
        result.hasRealRoots = false;
        return result;
    }
    
    result.hasRealRoots = true;
    
    // Calculate roots: (-b ± √discriminant) / (2a)
    int32_t sqrt_disc = sqrt_int(result.discriminant);
    
    // To handle fractions, multiply by 1000 for precision
    int32_t numerator1 = (-b + sqrt_disc) * 1000;
    int32_t numerator2 = (-b - sqrt_disc) * 1000;
    int32_t denominator = 2 * a;
    
    // Root 1
    result.x1_int = numerator1 / denominator / 1000;
    result.x1_frac = (numerator1 / denominator) % 1000;
    if(result.x1_frac < 0) result.x1_frac = -result.x1_frac;
    
    // Root 2  
    result.x2_int = numerator2 / denominator / 1000;
    result.x2_frac = (numerator2 / denominator) % 1000;
    if(result.x2_frac < 0) result.x2_frac = -result.x2_frac;
    
    return result;
}

// Global variables for enhanced interface
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

void clearScreen()
{
    uint16_t* VideoMemory = (uint16_t*)0xb8000;
    for(int i = 0; i < 80*25; i++) {
        VideoMemory[i] = 0x0720; // Space with white on black
    }
}

void drawBorder()
{
    uint16_t* VideoMemory = (uint16_t*)0xb8000;
    
    // Top border
    for(int x = 0; x < 80; x++) {
        VideoMemory[x] = 0x0F00 | (x == 0 ? 0xC9 : (x == 79 ? 0xBB : 0xCD));
    }
    
    // Bottom border
    for(int x = 0; x < 80; x++) {
        VideoMemory[24*80 + x] = 0x0F00 | (x == 0 ? 0xC8 : (x == 79 ? 0xBC : 0xCD));
    }
    
    // Side borders
    for(int y = 1; y < 24; y++) {
        VideoMemory[y*80] = 0x0F00 | 0xBA;      // Left
        VideoMemory[y*80 + 79] = 0x0F00 | 0xBA; // Right
    }
}

void printCentered(char* text, int line, uint8_t color = 0x0F)
{
    uint16_t* VideoMemory = (uint16_t*)0xb8000;
    int len = 0;
    
    // Calculate length
    for(int i = 0; text[i] != '\0'; i++) len++;
    
    int start_x = (80 - len) / 2;
    for(int i = 0; i < len; i++) {
        VideoMemory[line*80 + start_x + i] = (color << 8) | text[i];
    }
}

void printAt(char* text, int x, int y, uint8_t color = 0x0F)
{
    uint16_t* VideoMemory = (uint16_t*)0xb8000;
    for(int i = 0; text[i] != '\0'; i++) {
        VideoMemory[y*80 + x + i] = (color << 8) | text[i];
    }
}

void displayMainMenu()
{
    clearScreen();
    drawBorder();
    
    printCentered("HE DIEU HANH TINH TOAN PHUONG TRINH BAC 2", 2, 0x0E);
    printCentered("========================================", 3, 0x0E);
    
    printCentered("MENU CHINH", 6, 0x0B);
    printCentered("----------", 7, 0x0B);
    
    printAt("  [1] Giai Phuong Trinh Bac 2", 25, 10, 0x0A);
    printAt("  [2] Huong Dan Su Dung", 25, 12, 0x0A);  
    printAt("  [3] Thong Tin Phat Trien", 25, 14, 0x0A);
    printAt("  [4] Shutdown", 25, 16, 0x0A);
    
    printCentered("Nhan phim tuong ung de chon chuc nang", 20, 0x07);
    printCentered("Phat trien boi: Nhat + Ngoc + Sang + Minh + Quynh", 22, 0x08);
}

void displayHelpScreen()
{
    clearScreen();
    drawBorder();
    
    printCentered("HUONG DAN SU DUNG", 2, 0x0E);
    printCentered("=================", 3, 0x0E);
    
    printAt("Phuong trinh bac 2 co dang: ax^2 + bx + c = 0", 15, 6, 0x0F);
    printAt("Trong do:", 15, 8, 0x0B);
    printAt("  - a: He so cua x^2 (a khac 0)", 15, 9, 0x0A);
    printAt("  - b: He so cua x", 15, 10, 0x0A);
    printAt("  - c: Hang so tu do", 15, 11, 0x0A);
    
    printAt("Cac phim dieu khien:", 15, 14, 0x0B);
    printAt("  - Enter: Xac nhan nhap", 15, 15, 0x0A);
    printAt("  - Backspace: Xoa ky tu", 15, 16, 0x0A);
    printAt("  - ESC: Quay lai menu chinh", 15, 17, 0x0A);
    printAt("  - R: Reset de giai phuong trinh moi", 15, 18, 0x0A);
    
    printCentered("Nhan ESC de quay lai menu chinh", 22, 0x0C);
}

void displayAboutScreen()
{
    clearScreen();
    drawBorder();
    
    printCentered("THONG TIN PHAT TRIEN", 2, 0x0E);
    printCentered("====================", 3, 0x0E);
    
    printCentered("HE DIEU HANH MYOS - PHIEN BAN 2.0", 6, 0x0B);

    
    
    printAt("Nhom phat trien:", 25, 9, 0x0F);
    printAt("  - Nhat: Kernel & Memory Management", 25, 11, 0x0A);
    printAt("  - Ngoc: Keyboard Driver & Input", 25, 12, 0x0A);
    printAt("  - Sang: Math Library & Algorithms", 25, 13, 0x0A);
    printAt("  - Minh: Interface & Display", 25, 14, 0x0A);
    printAt("  - Quynh: Testing & Documentation", 25, 15, 0x0A);
    
    printAt("Tinh nang:", 25, 17, 0x0F);
    printAt("  - Giai phuong trinh bac 2", 25, 18, 0x0A);
    printAt("  - Giao dien tieng Viet", 25, 19, 0x0A);
    printAt("  - Xu ly so nguyen va phan thap phan", 25, 20, 0x0A);
    
    printCentered("Nhan ESC de quay lai menu chinh", 22, 0x0C);
}
// Print integer with sign
void printInt(int32_t num)
{
    if(num < 0) {
        printf("-");
        num = -num;
    }
    
    char buffer[12];
    int pos = 0;
    
    if(num == 0) {
        printf("0");
        return;
    }
    
    while(num > 0) {
        buffer[pos++] = '0' + (num % 10);
        num /= 10;
    }
    
    for(int i = pos - 1; i >= 0; i--) {
        char temp[2] = {buffer[i], '\0'};
        printf(temp);
    }
}

void printIntAt(int32_t num, int x, int y, uint8_t color = 0x0F)
{
    uint16_t* VideoMemory = (uint16_t*)0xb8000;
    char buffer[12];
    int pos = 0;
    
    if(num < 0) {
        VideoMemory[y*80 + x] = (color << 8) | '-';
        x++;
        num = -num;
    }
    
    if(num == 0) {
        VideoMemory[y*80 + x] = (color << 8) | '0';
        return;
    }
    
    while(num > 0) {
        buffer[pos++] = '0' + (num % 10);
        num /= 10;
    }
    
    for(int i = pos - 1; i >= 0; i--) {
        VideoMemory[y*80 + x++] = (color << 8) | buffer[i];
    }
}

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
    
    // Determine input position based on current state
    int input_x = (current_state == WAITING_A) ? 33 : 
                  (current_state == WAITING_B) ? 24 : 24;
    int input_y = (current_state == WAITING_A) ? 8 : 
                  (current_state == WAITING_B) ? 10 : 11;
    
    // Clear input area (20 characters)
    for(int i = 0; i < 20; i++) {
        VideoMemory[input_y*80 + input_x + i] = 0x0720;
    }
    
    // Display current input
    for(int i = 0; i < input_pos; i++) {
        VideoMemory[input_y*80 + input_x + i] = 0x0F00 | input_buffer[i];
    }
    
    // Show blinking cursor
    VideoMemory[input_y*80 + input_x + input_pos] = 0x7000 | '_';
}

void shutdown()
{
    // Lệnh đặc biệt cho QEMU
    Port16Bit port(0x604);
    port.Write(0x2000);
}


class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void OnKeyDown(char c)
    {
        // Handle menu navigation
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
                    shutdown(); // shutdown voi qemu
                    return;
            }
            return;
        }
        
        // Handle ESC key - go back to main menu
        if(c == 27) { // ESC key
            current_state = MAIN_MENU;
            displayMainMenu();
            return;
        }
        
        // Handle help and about screens
        if(current_state == HELP_SCREEN || current_state == ABOUT_SCREEN) {
            if(c == 27) { // ESC
                current_state = MAIN_MENU;
                displayMainMenu();
            }
            return;
        }
        
        // Handle result screen
        if(current_state == SHOWING_RESULT) {
            if(c == 'r' || c == 'R') {
                // Reset for new equation
                current_state = WAITING_A;
                input_pos = 0;
                coeff_a = coeff_b = coeff_c = 0;
                displayQuadraticInterface();
                displayCurrentInput();
                return;
            }
            return;
        }
        
        // Handle input for coefficients
        if(current_state >= WAITING_A && current_state <= WAITING_C) {
            
            // Handle Enter key
            if(c == '\n' || c == '\r') {
                if(input_pos > 0) {
                    input_buffer[input_pos] = '\0';
                    int32_t value = stringToInt(input_buffer, input_pos);
                    
                    // Validate coefficient a
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
            
            // Handle backspace
            if(c == '\b' || c == 127) {
                if(input_pos > 0) {
                    input_pos--;
                    input_buffer[input_pos] = '\0';
                    displayCurrentInput();
                }
                return;
            }
            
            // Handle numbers and minus sign
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
    // Clear screen and show loading message

    printAt("                                                                     ", 1, 2, 0x0E);
    printAt("                                             @@@  #@#                ", 1, 3, 0x0E);
    printAt("                                          %@@@@@@@@                  ", 1, 4, 0x0E);
    printAt("                                   -@@@@@@@@@@@@@                    ", 1, 5, 0x0E);
    printAt("                                 @@@@+=*@@@@@@@@                     ", 1, 6, 0x0E);
    printAt("                           -++- @  _ :@@@@%=@@@@                     ", 1, 7, 0x0E);
    printAt("                 .@@%@@::######*    +=  _  @=#@@                     ", 1, 8, 0x0E);
    printAt("                +@=@+====@###########@#    @==*@                     ", 1, 9, 0x0E);
    printAt("                @+#=# *==#############@*##+===@@@@@@@= #:            ", 1, 10, 0x0E);
    printAt("                @==+: - +=############@==========%###+@*             ", 1, 11, 0x0E);
    printAt("                @==+- - @==@#######@+======*@@@#@+=%*@@+             ", 1, 12, 0x0E);
    printAt("                =*==% -    +%+==+=====+#@#.   @   *=#+@#@%           ", 1, 13, 0x0E);
    printAt("                 #+==%*    -    +@#     +     =  =*=%+@##            ", 1, 14, 0x0E);
    printAt("                  =%==*+   -     .      +       @+====#%             ", 1, 15, 0x0E);
    printAt("                    *%==*+ %     .      %    +%*====#%               ", 1, 16, 0x0E);
    printAt("                      -%===%+   ..      % .*#====+@.                 ", 1, 17, 0x0E);
    printAt("                        @======+#@@@@@%*+========@                   ", 1, 18, 0x0E);
    printAt("                         %=======================@                   ", 1, 19, 0x0E);
    printAt("                                                                     ", 1, 20, 0x0E);
    printAt("                                                                     ", 1, 21, 0x0E);
    printAt("                      Dang khoi dong he dieu hanh...\n               ", 1, 22, 0x0E);


    // Chờ 2 giây (tương đối)
    for (volatile int i = 0; i < 1000000000; ++i);



    GlobalDescriptorTable gdt;
    
    uint32_t* memupper = (uint32_t*)(((size_t)multiboot_structure) + 8);
    size_t heap = 10*1024*1024;
    MemoryManager memoryManager(heap, (*memupper)*1024 - heap - 10*1024);
    
    TaskManager taskManager;
    InterruptManager interrupts(0x20, &gdt, &taskManager);
    
    DriverManager drvManager;
    
    PrintfKeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts, &kbhandler);
    drvManager.AddDriver(&keyboard);
    
    drvManager.ActivateAll();
    
    // Initialize interface with main menu
    displayMainMenu();
    
    interrupts.Activate();
    
    while(1)
    {
        // Keep the OS running
    }
}
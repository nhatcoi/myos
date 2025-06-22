#include <common/types.h>

using namespace myos::common;

// import simple functions
void printf(char* str);
void printfHex(uint8_t key);
void printInt(int32_t num);
void printIntAt(int32_t num, int x, int y, uint8_t color = 0x0F);
int32_t stringToInt(char* str, int len);

// QuadraticResult struct định nghĩa lại để sử dụng
struct QuadraticResult {
    bool hasRealRoots;
    int32_t x1_int;  
    int32_t x1_frac; 
    int32_t x2_int;   
    int32_t x2_frac; 
    int32_t discriminant;
};

// import functions từ math_functions
QuadraticResult solveQuadratic(int32_t a, int32_t b, int32_t c);

void clearScreen()
{
    uint16_t* VideoMemory = (uint16_t*)0xb8000;
    for(int i = 0; i < 80*25; i++) {
        VideoMemory[i] = 0x0720; // Space with white on black
    }
}

// Hàm vẽ border
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

// Hàm hiển thị text ở giữa màn hình
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

// Hàm hỗ trợ hiển thị vị trí x, y
void printAt(char* text, int x, int y, uint8_t color = 0x0F)
{
    uint16_t* VideoMemory = (uint16_t*)0xb8000;
    for(int i = 0; text[i] != '\0'; i++) {
        VideoMemory[y*80 + x + i] = (color << 8) | text[i];
    }
}

// Hàm hiển thị menu chính
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
    
    printAt("Dinh dang so nhap:", 15, 13, 0x0B);
    printAt("  - So nguyen: 5, -3, 0", 15, 14, 0x0A);
    printAt("  - So thap phan: 1.5, -2.75, 0.333", 15, 15, 0x0A);
    printAt("  - Toi da 3 chu so thap phan", 15, 16, 0x0A);
    
    printAt("Cac phim dieu khien:", 15, 18, 0x0B);
    printAt("  - Enter: Xac nhan nhap", 15, 19, 0x0A);
    printAt("  - Backspace: Xoa ky tu", 15, 20, 0x0A);
    printAt("  - ESC: Quay lai menu chinh", 15, 21, 0x0A);
    printAt("  - R: Reset de giai phuong trinh moi", 15, 22, 0x0A);
    
    printCentered("Nhan ESC de quay lai menu chinh", 23, 0x0C);
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


// logo boot
void displayBootScreen()
{
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
} 
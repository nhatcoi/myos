#include <common/types.h>

using namespace myos::common;

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
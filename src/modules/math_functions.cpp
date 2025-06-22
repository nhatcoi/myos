#include <common/types.h>

using namespace myos::common;

// === FIXED-POINT ARITHMETIC SUPPORT ===
// Sử dụng fixed-point với 3 chữ số thập phân (scale = 1000)
#define DECIMAL_SCALE 1000

// Struct lưu số thập phân
struct DecimalNumber {
    int32_t value;  // Giá trị nhân với DECIMAL_SCALE
    
    DecimalNumber() : value(0) {}
    DecimalNumber(int32_t val) : value(val * DECIMAL_SCALE) {}
    DecimalNumber(int32_t int_part, int32_t frac_part) : 
        value(int_part * DECIMAL_SCALE + frac_part) {}
};

// Các phép toán với số thập phân
DecimalNumber add(DecimalNumber a, DecimalNumber b) {
    DecimalNumber result;
    result.value = a.value + b.value;
    return result;
}

DecimalNumber subtract(DecimalNumber a, DecimalNumber b) {
    DecimalNumber result;
    result.value = a.value - b.value;
    return result;
}

DecimalNumber multiply(DecimalNumber a, DecimalNumber b) {
    DecimalNumber result;
    // Nhân rồi chia lại DECIMAL_SCALE để giữ đúng scale
    result.value = (a.value * b.value) / DECIMAL_SCALE;
    return result;
}

DecimalNumber divide(DecimalNumber a, DecimalNumber b) {
    DecimalNumber result;
    // Nhân với DECIMAL_SCALE trước khi chia để giữ đúng scale
    result.value = (a.value * DECIMAL_SCALE) / b.value;
    return result;
}

// Hàm tính căn bậc hai cho fixed-point (Newton's method)
DecimalNumber sqrt_decimal(DecimalNumber x)
{
    if(x.value <= 0) {
        DecimalNumber result;
        result.value = 0;
        return result;
    }
    
    DecimalNumber guess;
    guess.value = x.value / 2;
    
    for(int i = 0; i < 10; i++) {
        DecimalNumber prev_guess = guess;
        // guess = (guess + x/guess) / 2
        DecimalNumber div_result = divide(x, guess);
        guess = add(guess, div_result);
        guess.value = guess.value / 2;
        
        // Check convergence
        if(guess.value == prev_guess.value) break;
    }
    return guess;
}

// Hàm tính căn bậc hai sử dụng phương pháp Newton (legacy cho integer)
int32_t sqrt_int(int32_t x)
{
    if(x <= 1) return x;
    
    int32_t guess = x / 2;
    int32_t prev_guess;
    
    for(int i = 0; i < 10; i++)
    {
        prev_guess = guess;
        guess = (guess + x / guess) / 2;
        if(guess == prev_guess) break;
    }
    return guess;
}

int strlen(const char* str) {
    int len = 0;
    while(str[len] != '\0') {
        len++;
    }
    return len;
}


// Convert string có thập phân thành DecimalNumber
DecimalNumber stringToDecimal(char* str, int len)
{
    // Nếu len không hợp lệ, tính lại thủ công
    if(len <= 0) {
        len = 0;
        while(str[len] != '\0') len++;
    }

    DecimalNumber result;
    int32_t int_part = 0;
    int32_t frac_part = 0;
    int32_t sign = 1;
    int start = 0;
    bool has_decimal = false;
    int decimal_pos = -1;

    if(str[0] == '-') {
        sign = -1;
        start = 1;
    }

    for(int i = start; i < len; i++) {
        if(str[i] == '.') {
            has_decimal = true;
            decimal_pos = i;
            break;
        }
    }

    int end_int = has_decimal ? decimal_pos : len;
    for(int i = start; i < end_int; i++) {
        if(str[i] >= '0' && str[i] <= '9') {
            int_part = int_part * 10 + (str[i] - '0');
        }
    }

    if(has_decimal) {
        int frac_digits = 0;
        for(int i = decimal_pos + 1; i < len && frac_digits < 3; i++) {
            if(str[i] >= '0' && str[i] <= '9') {
                frac_part = frac_part * 10 + (str[i] - '0');
                frac_digits++;
            }
        }
        while(frac_digits < 3) {
            frac_part *= 10;
            frac_digits++;
        }
    }

    int total = int_part * DECIMAL_SCALE + frac_part;
    result.value = (sign < 0) ? -total : total;
    return result;
}



// Convert string qua int (legacy)
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

// struct lưu fields của ptb2 với decimal support
struct QuadraticResult {
    bool hasRealRoots;
    DecimalNumber x1;     // Nghiệm 1
    DecimalNumber x2;     // Nghiệm 2  
    DecimalNumber discriminant;
    
    // Legacy fields for backward compatibility
    int32_t x1_int;  // phần nguyên của nghiệm 1
    int32_t x1_frac; // phần thập phân của nghiệm 1 (0-999)
    int32_t x2_int;  // phần nguyên của nghiệm 2
    int32_t x2_frac; // phần thập phân của nghiệm 2 (0-999)
};

// Tính ax² + bx + c = 0 với decimal numbers
QuadraticResult solveQuadraticDecimal(DecimalNumber a, DecimalNumber b, DecimalNumber c)
{
    QuadraticResult result;
    
    if(a.value == 0) {
        result.hasRealRoots = false;
        return result;
    }
    
    // Tính discriminant = b² - 4ac
    DecimalNumber b_squared = multiply(b, b);
    DecimalNumber four_a = DecimalNumber(4);
    DecimalNumber four_ac = multiply(multiply(four_a, a), c);
    result.discriminant = subtract(b_squared, four_ac);
    
    if(result.discriminant.value < 0) {
        result.hasRealRoots = false;
        return result;
    }
    
    result.hasRealRoots = true;
    
    // Tính nghiệm: (-b ± √discriminant) / (2a)
    DecimalNumber sqrt_disc = sqrt_decimal(result.discriminant);
    DecimalNumber neg_b;
    neg_b.value = -b.value;
    DecimalNumber two_a = DecimalNumber(2);
    two_a = multiply(two_a, a);
    
    // x1 = (-b + √discriminant) / (2a)
    DecimalNumber numerator1 = add(neg_b, sqrt_disc);
    result.x1 = divide(numerator1, two_a);
    
    // x2 = (-b - √discriminant) / (2a)  
    DecimalNumber numerator2 = subtract(neg_b, sqrt_disc);
    result.x2 = divide(numerator2, two_a);
    
    // Fill legacy fields
    result.x1_int = result.x1.value / DECIMAL_SCALE;
    result.x1_frac = result.x1.value % DECIMAL_SCALE;
    if(result.x1_frac < 0) result.x1_frac = -result.x1_frac;
    
    result.x2_int = result.x2.value / DECIMAL_SCALE;
    result.x2_frac = result.x2.value % DECIMAL_SCALE;
    if(result.x2_frac < 0) result.x2_frac = -result.x2_frac;
    
    return result;
}

// Legacy function cho integer (backward compatibility)
QuadraticResult solveQuadratic(int32_t a, int32_t b, int32_t c)
{
    DecimalNumber dec_a(a);
    DecimalNumber dec_b(b);
    DecimalNumber dec_c(c);
    return solveQuadraticDecimal(dec_a, dec_b, dec_c);
}
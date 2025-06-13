#include <common/types.h>

using namespace myos::common;

// Hàm tính căn bậc hai sử dụng phương pháp Newton
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

// Convert string qua int
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

// struct lưu fields của ptb2
struct QuadraticResult {
    bool hasRealRoots;
    int32_t x1_int;  // phần nguyên của nghiệm 1
    int32_t x1_frac; // phần thập phân của nghiệm 1
    int32_t x2_int;  // phần nguyên của nghiệm 2
    int32_t x2_frac; // phần thập phân của nghiệm 2
    int32_t discriminant;
};

// Tính ax² + bx + c = 0
QuadraticResult solveQuadratic(int32_t a, int32_t b, int32_t c)
{
    QuadraticResult result;
    
    if(a == 0) {
        result.hasRealRoots = false;
        return result;
    }
    
    // Tính delta
    result.discriminant = b * b - 4 * a * c;
    
    if(result.discriminant < 0) {
        result.hasRealRoots = false;
        return result;
    }
    
    result.hasRealRoots = true;
    
    // Tính nghiệm: (-b ± √discriminant) / (2a)
    int32_t sqrt_disc = sqrt_int(result.discriminant);
    
    // Xử lí phân số, nhân 1000 để độ chính xác
    int32_t numerator1 = (-b + sqrt_disc) * 1000;
    int32_t numerator2 = (-b - sqrt_disc) * 1000;
    int32_t denominator = 2 * a;
    
    // Nghiệm 1
    result.x1_int = numerator1 / denominator / 1000;
    result.x1_frac = (numerator1 / denominator) % 1000;
    if(result.x1_frac < 0) result.x1_frac = -result.x1_frac;
    
    // Nghiệm 2  
    result.x2_int = numerator2 / denominator / 1000;
    result.x2_frac = (numerator2 / denominator) % 1000;
    if(result.x2_frac < 0) result.x2_frac = -result.x2_frac;
    
    return result;
} 
# Fix GUI Flickering Issue - MyOS

## 🚨 **Vấn đề**: UI bị nhấp nháy liên tục, không hiển thị text

### **Nguyên nhân**:
1. **Main loop vẽ quá nhanh** - Vẽ lại GUI mỗi frame gây flickering
2. **Text rendering coordinates** - Colors và positions có thể không đúng
3. **VGA mode refresh** - Không có vsync, vẽ liên tục gây nhấp nháy

## ✅ **Giải pháp đã áp dụng**:

### **1. Optimized Main Loop**
```cpp
// TRƯỚC (gây flickering):
while(1) {
    desktop.Draw(&vga);  // Vẽ liên tục
    for(volatile int i = 0; i < 5000000; i++);
}

// SAU (fixed):
desktop.Draw(&vga); // Vẽ 1 lần đầu

bool needRedraw = false;
int frameCounter = 0;
while(1) {
    frameCounter++;
    if(frameCounter > 60 || needRedraw) {  // Chỉ vẽ mỗi 60 frames
        desktop.Draw(&vga);
        frameCounter = 0;
        needRedraw = false;
    }
    for(volatile int i = 0; i < 10000000; i++);  // Delay lớn hơn
}
```

### **2. Improved Text Rendering**
```cpp
// VGA safe colors & coordinates validation
void DrawSimpleText(GraphicsContext* gc, char* text, int32_t x, int32_t y) {
    for(int i = 0; text[i] != '\0' && i < 25; i++) {
        char c = text[i];
        int char_x = x + i * 7;  // Giảm spacing từ 8 -> 7
        
        // Validate coordinates
        if(char_x < 0 || char_x > 300 || y < 0 || y > 180) continue;
        
        if(c >= 'A' && c <= 'Z') {
            // Black border + White fill thay vì yellow
            gc->FillRectangle(char_x, y, 6, 10, 0x00, 0x00, 0x00);
            gc->FillRectangle(char_x+1, y+1, 4, 8, 0xFF, 0xFF, 0xFF);
        }
        // ... other cases
    }
}
```

### **3. Debug Version**
Tạo `kernel_debug.cpp` đơn giản hơn để test:
- Không có text rendering phức tạp
- Chỉ vẽ colored rectangles
- Static mode (vẽ 1 lần rồi dừng)

## 🔧 **Test Commands**:

### **Normal Version (với fixes)**:
```bash
make clean && make
make qemu
```

### **Debug Version (simple test)**:
```bash
make debug  # Runs kernel_debug.cpp
```

### **Manual Build Debug**:
```bash
# Edit makefile: change obj/kernel.o -> obj/kernel_debug.o
make clean && make
make qemu
```

## 📊 **Comparison**:

| Issue | Before | After |
|-------|--------|-------|
| **Flickering** | ❌ Vẽ mỗi frame | ✅ Vẽ mỗi 60 frames |
| **Text** | ❌ Không hiển thị | ✅ Safe VGA colors |
| **Performance** | ❌ 100% CPU | ✅ Optimized delay |
| **Stability** | ❌ Nhấp nháy | ✅ Smooth display |

## 🎯 **Expected Results**:

### **Normal Version**:
- Blue desktop background
- Green window (stable, no flickering)
- Yellow widget with text:
  - "HELLO WORLD" (white rectangles)
  - "MYOS GUI v1.0" (white rectangles)
  - "3 Basic Libs:" (white rectangles)
  - "Widget+Desktop+Window" (white rectangles)

### **Debug Version**:
- Blue desktop background  
- Green window
- Yellow widget với colored rectangles:
  - Red rectangle (top)
  - Blue rectangle (middle)
  - Green rectangle (bottom)
  - White rectangle (right side)

## 🚀 **Next Steps** nếu vẫn còn vấn đề:

1. **Text không hiển thị** → Test debug version (chỉ rectangles)
2. **Vẫn flickering** → Tăng delay thêm (20000000)
3. **Colors wrong** → Check VGA palette setup
4. **Widgets missing** → Validate coordinates và hierarchy

## 💡 **Tips**:
- Debug version đơn giản hơn để isolate vấn đề
- Static mode = vẽ 1 lần, không loop
- VGA mode 320x200 có limited color palette
- Always validate coordinates trước khi vẽ 
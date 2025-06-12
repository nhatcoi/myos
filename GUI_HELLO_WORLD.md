# MyOS GUI Hello World - Kernel cơ bản

## Mô tả
Kernel.cpp được xây dựng dựa trên **3 lib cơ bản** trong src/gui để tạo GUI đơn giản hiển thị "Hello World".

## 3 Lib cơ bản được sử dụng

### 1. **Widget** (`src/gui/widget.cpp`, `include/gui/widget.h`)
- Base class cho tất cả GUI components
- Quản lý position (x,y), size (w,h), colors (r,g,b)
- Event handling cho mouse/keyboard
- CompositeWidget để chứa các widget con

### 2. **Desktop** (`src/gui/desktop.cpp`, `include/gui/desktop.h`)
- Root container chứa tất cả GUI elements
- MouseEventHandler để xử lý mouse events
- Vẽ mouse cursor (white cross)
- Background color: Blue (0x00, 0x00, 0xA8)

### 3. **Window** (`src/gui/window.cpp`, `include/gui/window.h`)
- Container cho widgets với drag & drop functionality
- Có thể di chuyển bằng mouse
- Màu xanh lá: (0x00, 0xA8, 0x00)

## HelloWorldWidget Class
```cpp
class HelloWorldWidget : public Widget
{
public:
    HelloWorldWidget(Widget* parent, int32_t x, int32_t y, int32_t w, int32_t h)
    : Widget(parent, x, y, w, h, 0xFF, 0xFF, 0x00) // Yellow background
    
    virtual void Draw(GraphicsContext* gc)
    {
        Widget::Draw(gc);  // Vẽ background
        DrawSimpleText(gc, "HELLO WORLD", X + 10, Y + 20);
        DrawSimpleText(gc, "MYOS GUI v1.0", X + 10, Y + 40);
        DrawSimpleText(gc, "3 Basic Libs:", X + 10, Y + 60);
        DrawSimpleText(gc, "Widget+Desktop+Window", X + 5, Y + 80);
    }
}
```

## Visual Design
- **VGA Mode**: 320x200 pixels, 8-bit color
- **Desktop Background**: Blue (0x00, 0x00, 0xA8)
- **Main Window**: Green (0x00, 0xA8, 0x00), size 260x140, position (30,30)
- **Hello Widget**: Yellow (0xFF, 0xFF, 0x00), size 240x120, position (10,10) trong window
- **Text Rendering**: Pixel art đơn giản với colored rectangles
  - Letters A-Z: Black border + Yellow fill
  - Numbers 0-9: Blue border + White fill  
  - Special chars: Red dots

## Architecture
```
Desktop (320x200, Blue background)
├── Mouse cursor (white cross)
└── Window (260x140, Green)
    └── HelloWorldWidget (240x120, Yellow)
        ├── "HELLO WORLD"
        ├── "MYOS GUI v1.0"
        ├── "3 Basic Libs:"
        └── "Widget+Desktop+Window"
```

## Build & Run
```bash
make clean && make
make qemu      # Test với QEMU
```

## Features
- ✅ VGA Graphics mode (320x200)
- ✅ Widget hierarchy system
- ✅ Mouse cursor rendering
- ✅ Window drag & drop (mouse support)
- ✅ Custom text rendering with pixel art
- ✅ Event-driven architecture
- ✅ Memory management integration

## GUI Hierarchy
1. **VGA Driver** → Graphics context
2. **Desktop** → Root container + mouse handling
3. **Window** → Draggable container
4. **HelloWorldWidget** → Custom widget hiển thị text

Đây là foundation cơ bản để phát triển GUI phức tạp hơn sau này! 
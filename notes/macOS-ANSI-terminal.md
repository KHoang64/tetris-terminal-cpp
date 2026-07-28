# macOS Terminal Rendering & Input — ANSI Escape Codes + termios

## Overview

The javidx9 Tetris tutorial uses **Windows-specific APIs**:
- `WriteConsoleOutput()` — Win32 screen buffer
- `GetAsyncKeyState()` — Win32 async keyboard polling

On macOS, we replace these with:
1. **ANSI escape codes** — position cursor, clear screen
2. **`termios` raw mode** — non-blocking keyboard input

---

## Part 1: ANSI Escape Codes (Screen Rendering)

### What are ANSI codes?

Escape sequences that instruct the terminal to move the cursor, clear regions, or apply colors. All ANSI codes start with **`\033`** (octal for `ESC` character).

### Key codes used in Tetris:

```cpp
#define CLEAR_SCREEN "\033[2J"      // Clear entire screen
#define CURSOR_HOME "\033[H"        // Move cursor to (0,0)
```

| Code | Effect |
|------|--------|
| `\033[2J` | Erase entire screen |
| `\033[H` | Move cursor to home (0,0) |
| `\033[nA` | Move cursor up n lines |
| `\033[nB` | Move cursor down n lines |
| `\033[nC` | Move cursor right n columns |
| `\033[nD` | Move cursor left n columns |

### Usage in the game loop:

```cpp
// Every frame:
std::cout << CLEAR_SCREEN << CURSOR_HOME;  // Clear + home
// ... draw content ...
std::cout.flush();  // Force output immediately
```

---

## Part 2: termios Raw Mode (Keyboard Input)

### The Problem

By default, your terminal is in **canonical mode**:
- Input is buffered until you press Enter
- Echo is on (you see your keystrokes)
- Blocking — the program waits for Enter

For a game, we need:
- **Non-blocking** — check for keypresses without waiting
- **No echo** — don't print arrow keys to screen
- **Immediate** — react to individual keystrokes

### The Solution: Raw Mode

```cpp
#include <termios.h>
#include <unistd.h>

termios original_termios;

void EnableRawMode()
{
    // Get current terminal settings
    tcgetattr(STDIN_FILENO, &original_termios);
    
    termios raw = original_termios;
    
    // Disable canonical mode (ICANON) and echo (ECHO)
    raw.c_lflag &= ~(ICANON | ECHO);
    
    // Set non-blocking read: return immediately if no data
    raw.c_cc[VMIN] = 0;   // Minimum chars to read (0 = non-blocking)
    raw.c_cc[VTIME] = 0;  // Timeout in deciseconds (0 = no timeout)
    
    // Apply new settings
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void DisableRawMode()
{
    // Restore original settings when exiting
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}
```

### Ensure cleanup on exit:

```cpp
int main()
{
    EnableRawMode();
    atexit(DisableRawMode);  // Auto-restore terminal if Ctrl+C
    
    // ... game loop ...
    
    return 0;
}
```

---

## Part 3: Reading Arrow Keys

### Arrow Key ANSI Sequences

When you press an arrow key in a terminal, it sends:

| Key | Sequence |
|-----|----------|
| Left arrow | `ESC [ D` → `\033[D` or `\x1b[D` |
| Right arrow | `ESC [ C` → `\033[C` or `\x1b[C` |
| Up arrow | `ESC [ A` → `\033[A` |
| Down arrow | `ESC [ B` → `\033[B` |

The sequence is **3 bytes**: `ESC` (27), `[`, then a letter.

### Non-blocking read:

```cpp
int GetKeyInput()
{
    unsigned char c;
    
    // Try to read 1 byte (non-blocking, returns -1 if nothing)
    if (read(STDIN_FILENO, &c, 1) == 1) {
        
        // Check if it's the start of an escape sequence (ESC = 27)
        if (c == 27) {
            unsigned char seq[2];
            
            // Try to read the next 2 bytes of the sequence
            if (read(STDIN_FILENO, &seq[0], 1) == 1 &&
                read(STDIN_FILENO, &seq[1], 1) == 1) {
                
                // Check if it's "[" and then a direction
                if (seq[0] == '[') {
                    if (seq[1] == 'D') return -1;  // Left
                    if (seq[1] == 'C') return 1;   // Right
                    if (seq[1] == 'A') return -2;  // Up (unused yet)
                    if (seq[1] == 'B') return 2;   // Down (unused yet)
                }
            }
        }
    }
    return 0;  // No input
}
```

---

## Part 4: Game Loop Pattern

```cpp
#include <chrono>
#include <thread>

int main()
{
    EnableRawMode();
    atexit(DisableRawMode);
    
    const int FPS = 10;
    const auto frame_time = std::chrono::milliseconds(1000 / FPS);
    auto last_input_time = std::chrono::high_resolution_clock::now();
    
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        
        // 1. Input
        int key = GetKeyInput();
        if (key != 0) {
            // Handle arrow input
            current_piece = (current_piece + key + 7) % 7;
            last_input_time = start;
        }
        
        // 2. Logic (idle timeout)
        auto idle_time = std::chrono::high_resolution_clock::now() - last_input_time;
        if (idle_time > std::chrono::seconds(5)) {
            current_piece = (current_piece + 1) % 7;
            last_input_time = start;
        }
        
        // 3. Render
        std::cout << CLEAR_SCREEN << CURSOR_HOME;
        // ... draw frame ...
        std::cout.flush();
        
        // 4. Timing (maintain FPS)
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        auto sleep_time = frame_time - elapsed;
        if (sleep_time.count() > 0) {
            std::this_thread::sleep_for(sleep_time);
        }
    }
    
    return 0;
}
```

---

## Windows vs macOS: The Key Differences

| Aspect | Windows (Tutorial) | macOS (Our Workaround) |
|--------|-------------------|------------------------|
| **Screen buffer** | `WriteConsoleOutput()` (Win32 API) | `\033[2J` (ANSI) + `std::cout` |
| **Cursor positioning** | `SetConsoleCursorPosition()` | `\033[H` (ANSI) |
| **Keyboard polling** | `GetAsyncKeyState(vk)` (Win32 API) | `read(STDIN_FILENO)` + raw mode |
| **Non-blocking input** | Built-in to Win32 | `termios.c_cc[VMIN]=0` |
| **Terminal mode** | Console buffer object | Raw terminal I/O |

---

## Compilation Notes

Requires includes:
```cpp
#include <iostream>
#include <unistd.h>      // read(), STDIN_FILENO
#include <termios.h>     // tcgetattr(), tcsetattr()
#include <sys/select.h>  // (for future: select() if needed)
#include <chrono>
#include <thread>
```

No extra libraries — all POSIX standard on macOS.

---

## Debugging Tips

**Terminal acting weird after exit?**
```bash
# Manually restore terminal
stty sane
```

**Arrow keys not working?**
- Some terminals map arrow keys differently
- Try: `printf '\033[A'` in the terminal (move cursor up)
- If it works, raw mode setup is correct

**Frame timing too slow/fast?**
- Reduce/increase `FPS` constant
- Or adjust `frame_time = 1000 / FPS` in milliseconds

---

## Related: Next Steps

Once this frame loop is solid:
1. **Step 6 (Rotation math)** — Implement `Rotate(px, py, r)` to handle piece rotations at 0°/90°/180°/270°
2. **Step 7 (Collision)** — Implement `DoesPieceFit()` to check if pieces can move
3. **Step 8** — Integrate user input (already done!) into piece movement
4. **Step 9+** — Falling pieces, line detection, scoring

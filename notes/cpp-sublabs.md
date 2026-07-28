# C++ Sub-Labs: Isolating Terminal Concepts

## Overview

Instead of learning ANSI codes, termios, and arrow key input all at once (like in `main.cpp`), we break them into isolated **lab programs**. Each lab is a standalone executable that focuses on **one concept**.

Benefits:
- Learn each piece independently
- Experiment without breaking the game
- Easier to debug
- Good reference for later

---

## Project Structure

```
tetris-terminal-cpp/
├── src/
│   ├── main.cpp           # Full Tetris game (uses all 3 concepts)
│   ├── lab_ansi.cpp       # Lab 1: ANSI escape codes only
│   ├── lab_termios.cpp    # Lab 2: termios raw mode only
│   └── lab_arrows.cpp     # Lab 3: Arrow key input only
├── Makefile               # Builds tetris + all labs
└── notes/
    └── cpp-sublabs.md     # This file
```

---

## Building

### Build everything:
```bash
make
```

Produces: `tetris`, `lab_ansi`, `lab_termios`, `lab_arrows`

### Build just one:
```bash
make lab_ansi
make lab_termios
make lab_arrows
make tetris
```

### Clean:
```bash
make clean
```

---

## Lab 1: ANSI Escape Codes (`lab_ansi.cpp`)

**What it teaches:** How ANSI codes clear the screen and position the cursor.

**Key concepts:**
- `\033[2J` — clear entire screen
- `\033[H` — home cursor to (0,0)
- `std::cout.flush()` — immediate output

**What it does:**
- Demo 1: Clears screen with text fading
- Demo 2: Draws text at different positions
- Demo 3: Animated counter (10 frames)
- Demo 4: Simple box drawing

**Run it:**
```bash
make lab_ansi && ./lab_ansi
```

Press Enter between demos to step through.

---

## Lab 2: termios Raw Mode (`lab_termios.cpp`)

**What it teaches:** How to enable non-blocking, no-echo keyboard input.

**Key concepts:**
- `ICANON` — canonical mode flag (disable for immediate input)
- `ECHO` — echo flag (disable so you don't see typed characters)
- `c_cc[VMIN]=0` — non-blocking read
- `tcgetattr()` / `tcsetattr()` — get/set terminal settings
- `atexit()` — cleanup on exit

**What it does:**
- Part 1: Shows canonical mode (buffered input, you type + press Enter)
- Part 2: Switches to raw mode, collects 10 keystrokes instantly (no echo)
- Displays the difference clearly

**Run it:**
```bash
make lab_termios && ./lab_termios
```

Follow the on-screen prompts.

---

## Lab 3: Arrow Key Input (`lab_arrows.cpp`)

**What it teaches:** How to decode ANSI arrow key sequences.

**Key concepts:**
- Arrow keys send 3-byte sequences: `ESC [ letter`
- ESC = byte 27
- `[` is always the middle byte
- `D`/`C`/`A`/`B` encode Left/Right/Up/Down
- Parsing sequence byte-by-byte with `read()`

**What it does:**
- Interactive: press arrow keys to move `@` around a 20×10 grid
- Shows position in real-time
- 'q' to quit

**Run it:**
```bash
make lab_arrows && ./lab_arrows
```

Use arrow keys to navigate, 'q' to exit.

---

## How They Connect to main.cpp (Tetris)

| Lab | Concept | Used in main.cpp |
|-----|---------|------------------|
| `lab_ansi` | Screen clearing + rendering | `std::cout << CLEAR_SCREEN << CURSOR_HOME` in game loop |
| `lab_termios` | Non-blocking input setup | `EnableRawMode()` at startup, `DisableRawMode()` on exit |
| `lab_arrows` | Keyboard decoding | `GetKeyInput()` function returns -1/1/-2/2 for directions |

In `main.cpp`, all three are combined:
1. **Render** (ANSI) — clear screen, draw board
2. **Input** (termios + arrows) — read keyboard
3. **Logic** — update game state
4. **Timing** — maintain FPS

---

## Learning Path

**Recommended order:**

1. **Run `lab_ansi`** — understand screen clearing and ANSI escape sequences
2. **Read** the ANSI codes; try modifying them (e.g., change animation speed)
3. **Run `lab_termios`** — see the difference between canonical and raw mode
4. **Read** termios setup; understand each flag
5. **Run `lab_arrows`** — move the cursor around with arrow keys
6. **Read** arrow key parsing; try adding more keys (e.g., WASD)
7. **Study `main.cpp`** — see how all three come together
8. **Modify `main.cpp`** — experiment with game logic

---

## Makefile Tips

### How the Makefile works:

```makefile
tetris: src/main.cpp
	$(CXX) $(CXXFLAGS) -o tetris src/main.cpp
```

This rule says:
- To build `tetris` (target), use `src/main.cpp` (dependency)
- If `src/main.cpp` is newer than `tetris`, rebuild
- Command: `clang++ -std=c++17 -Wall -Wextra -O2 -o tetris src/main.cpp`

### Add more labs:

To add a 4th lab:
```makefile
TARGETS = tetris lab_ansi lab_termios lab_arrows lab_rotation

# Add this rule:
lab_rotation: src/lab_rotation.cpp
	$(CXX) $(CXXFLAGS) -o lab_rotation src/lab_rotation.cpp
```

Then `make` rebuilds everything including `lab_rotation`.

---

## Debugging

**Arrow keys not working in a lab?**
```bash
# Test raw mode is set correctly
make lab_arrows && ./lab_arrows
# Try pressing Ctrl+C if stuck
```

**Terminal acting weird?**
```bash
stty sane
```

**Modify a lab but `make` doesn't rebuild it?**
```bash
make clean && make lab_ansi
```

---

## Next Steps

Once you understand all three labs:

1. **Step 4-5 (main.cpp):** Add playing field rendering
2. **Step 6 (main.cpp):** Add `Rotate()` function (could be a `lab_rotation.cpp` too!)
3. **Step 7 (main.cpp):** Add `DoesPieceFit()` collision detection
4. **Step 8+ (main.cpp):** Integrate all pieces into the game

Each lab is a building block for the full Tetris engine.

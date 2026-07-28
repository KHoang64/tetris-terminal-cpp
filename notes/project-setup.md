# Tetris Terminal C++ — Project Setup

> **Status:** Step 1 in progress
> **Repo:** https://github.com/KHoang64/tetris-terminal-cpp
> **Local:** `~/Projects/tetris-terminal-cpp`
> **Tutorial:** [javidx9 — Tetris in the Command Line (YouTube)](https://www.youtube.com/watch?v=8OK8_tHeCIA)

---

## Project Files

### `src/main.cpp`
Step 1 of the javidx9 Tetris tutorial — macOS port.

Will contain:
- **7 tetromino shapes** stored as `std::wstring` — each piece is a 4×4 grid flattened to 16 chars (`.` = empty, `X` = filled)
- **`Rotate(px, py, r)` helper** — converts a (x, y) coordinate in the 4×4 grid to the correct flat index given a rotation (0°, 90°, 180°, 270°) using the index math the tutorial walks through

Currently: hello world with `std::wstring` / `std::wcout` to confirm wide-string support works on macOS.

---

### `Makefile`
Builds with `clang++` (ships with Xcode Command Line Tools — no extra install needed on Mac).

```makefile
CXX      = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
```

Usage:
```bash
make        # build → ./tetris
make clean  # remove binary
```

---

### `README.md`
Tracks the 12 tutorial steps as checkboxes and documents the two key **macOS workarounds**:

| Windows (tutorial) | macOS replacement |
|--------------------|-------------------|
| `WriteConsoleOutput` — writes directly to the Win32 console screen buffer | **ANSI escape codes** — move cursor + print directly to the terminal |
| `GetAsyncKeyState` — polls key state without blocking | **`termios` raw mode** + non-blocking `read()` from stdin |

Steps checklist:
- [ ] Step 1 — Assets: tetromino shapes as `wstring`
- [ ] Step 2 — Playing field setup
- [ ] Step 3 — Screen buffer (Mac: ANSI escape codes)
- [ ] Step 4 — Game loop skeleton
- [ ] Step 5 — Draw field
- [ ] Step 6 — Rotation math & `Rotate()` helper
- [ ] Step 7 — `DoesPieceFit()` collision detection
- [ ] Step 8 — User input (Mac: `termios` raw mode)
- [ ] Step 9 — Timing & forced piece fall
- [ ] Step 10 — Lock piece, pick next piece, game over
- [ ] Step 11 — Line detection & clearing
- [ ] Step 12 — Scoring & difficulty scaling

---

### `.gitignore`
Excludes macOS and VSCode artifacts:

```
tetris          # compiled binary
.DS_Store
*.dSYM/
.vscode/
*.o
```

---

## Notes

- `std::wstring` / `L"..."` literals are used throughout because the tutorial uses them for the shape strings and screen buffer
- `std::wcout` needs `std::ios::sync_with_stdio(false)` on some terminals — worth noting when we get to the draw step
- The rotation math (Step 6) is the trickiest concept early on — the four equations derive from manually tracing how indices shift in a 4×4 flat array at each 90° turn

# Tetris Terminal C++

Learning to build Tetris in the terminal with C++, following [javidx9's YouTube tutorial](https://www.youtube.com/watch?v=8OK8_tHeCIA) with **macOS workarounds**.

## What this is

The original tutorial targets Windows and uses:
- `WriteConsoleOutput` — Windows-only console screen buffer API
- `GetAsyncKeyState` — Windows-only async keyboard input

This repo adapts the same game logic step by step, replacing those with:
- **ANSI escape codes** — draw directly to the Mac terminal
- **termios + non-blocking stdin** — raw keyboard input on macOS/Linux

## Build & Run

```bash
make
./tetris
```

Requires a C++17-capable compiler (`clang++` ships with Xcode Command Line Tools).

## Project Structure

```
tetris-terminal-cpp/
├── src/
│   └── main.cpp       # Game source
├── Makefile
└── README.md
```

## Learning Log

Steps correspond roughly to sections of the javidx9 tutorial:

- [ ] Step 1 — Assets: tetromino shapes as strings
- [ ] Step 2 — Playing field setup
- [ ] Step 3 — Screen buffer (Mac: ANSI escape codes)
- [ ] Step 4 — Game loop skeleton
- [ ] Step 5 — Draw field
- [ ] Step 6 — Rotation math & `Rotate()` helper
- [ ] Step 7 — `DoesPieceFit()` collision detection
- [ ] Step 8 — User input (Mac: termios raw mode)
- [ ] Step 9 — Timing & forced piece fall
- [ ] Step 10 — Lock piece, pick next piece, game over
- [ ] Step 11 — Line detection & clearing
- [ ] Step 12 — Scoring & difficulty scaling

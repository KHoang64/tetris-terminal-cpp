// Tetris — terminal C++
// Following javidx9's tutorial with macOS workarounds
// Step 1: Assets (tetromino shapes as strings)

#include <string>
#include <vector>

// ─── Tetromino assets ──────────────────────────────────────────────────────
// Each piece is a 4x4 grid flattened to a 16-char string.
// '.' = empty cell, 'X' = filled cell.

std::vector<std::wstring> tetromino(7);

void InitAssets()
{
    // Shape 0 — I (straight line)
    tetromino[0] = L"..X."
                   L"..X."
                   L"..X."
                   L"..X.";

    // Shape 1 — S
    tetromino[1] = L"..X."
                   L".XX."
                   L".X.."
                   L"....";

    // Shape 2 — Z
    tetromino[2] = L".X.."
                   L".XX."
                   L"..X."
                   L"....";

    // Shape 3 — O (square)
    tetromino[3] = L".XX."
                   L".XX."
                   L"...."
                   L"....";

    // Shape 4 — L
    tetromino[4] = L".X.."
                   L".X.."
                   L".XX."
                   L"....";

    // Shape 5 — J
    tetromino[5] = L"..X."
                   L"..X."
                   L".XX."
                   L"....";

    // Shape 6 — T
    tetromino[6] = L".X.."
                   L".XX."
                   L".X.."
                   L"....";
}

// ─── Rotation helper ───────────────────────────────────────────────────────
// Maps (px, py) in a 4x4 grid to the rotated index.
// r = 0 (0°), 1 (90°), 2 (180°), 3 (270°)
int Rotate(int px, int py, int r)
{
    switch (r % 4) {
    case 0: return py * 4 + px;          //   0°
    case 1: return 12 + py - (px * 4);   //  90°
    case 2: return 15 - (py * 4) - px;   // 180°
    case 3: return 3 - py + (px * 4);    // 270°
    }
    return 0;
}

int main()
{
    InitAssets();
    // Nothing drawn yet — we'll add the screen buffer next step.
    return 0;
}

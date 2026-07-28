// ============================================================================
// VERSION 1 (Commented Out) — Hello World with wstrings[]
// ============================================================================
/*
#include <iostream>
#include <string>

int main()
{
    // "wstrings[]" style
    std::wstring lines[3];

    lines[0].append(L"Hello");
    lines[1].append(L" ");
    lines[2].append(L"World!");

    std::wstring message;
    message.append(lines[0]);
    message.append(lines[1]);
    message.append(lines[2]);

    std::wcout << message << L'\n';
    return 0;
}
*/

// ============================================================================
// VERSION 2 — Tetromino Assets + ANSI + Keyboard Controls (Video 3:11)
// macOS terminal rendering with arrow key controls
// Left/Right arrows to cycle pieces, auto-cycle after 5 seconds idle
// ============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>

// ANSI escape codes for macOS terminal
#define CLEAR_SCREEN "\033[2J"      // Clear entire screen
#define CURSOR_HOME "\033[H"        // Move cursor to (0,0)

std::vector<std::wstring> tetromino(7);

// Terminal control structures
termios original_termios;

void EnableRawMode()
{
    tcgetattr(STDIN_FILENO, &original_termios);
    termios raw = original_termios;
    raw.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo
    raw.c_cc[VMIN] = 0;               // Non-blocking read
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void DisableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}

// Returns -1 (left), 1 (right), or 0 (no input)
int GetKeyInput()
{
    unsigned char c;
    if (read(STDIN_FILENO, &c, 1) == 1) {
        if (c == 27) {  // ESC code for arrow keys
            unsigned char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) == 1 &&
                read(STDIN_FILENO, &seq[1], 1) == 1) {
                if (seq[0] == '[') {
                    if (seq[1] == 'D') return -1;  // Left arrow
                    if (seq[1] == 'C') return 1;   // Right arrow
                }
            }
        }
    }
    return 0;
}

void InitAssets()
{
    // I piece — vertical line
    tetromino[0] = L".";
    tetromino[0].append(L".");
    tetromino[0].append(L"X");
    tetromino[0].append(L".");
    tetromino[0].append(L".");
    tetromino[0].append(L".");
    tetromino[0].append(L"X");
    tetromino[0].append(L".");
    tetromino[0].append(L".");
    tetromino[0].append(L".");
    tetromino[0].append(L"X");
    tetromino[0].append(L".");
    tetromino[0].append(L".");
    tetromino[0].append(L".");
    tetromino[0].append(L"X");
    tetromino[0].append(L".");

    // S piece
    tetromino[1] = L".";
    tetromino[1].append(L".");
    tetromino[1].append(L"X");
    tetromino[1].append(L".");
    tetromino[1].append(L".");
    tetromino[1].append(L"X");
    tetromino[1].append(L"X");
    tetromino[1].append(L".");
    tetromino[1].append(L".");
    tetromino[1].append(L"X");
    tetromino[1].append(L".");
    tetromino[1].append(L".");
    tetromino[1].append(L".");
    tetromino[1].append(L".");
    tetromino[1].append(L".");
    tetromino[1].append(L".");

    // Z piece
    tetromino[2] = L".";
    tetromino[2].append(L"X");
    tetromino[2].append(L".");
    tetromino[2].append(L".");
    tetromino[2].append(L".");
    tetromino[2].append(L"X");
    tetromino[2].append(L"X");
    tetromino[2].append(L".");
    tetromino[2].append(L".");
    tetromino[2].append(L".");
    tetromino[2].append(L"X");
    tetromino[2].append(L".");
    tetromino[2].append(L".");
    tetromino[2].append(L".");
    tetromino[2].append(L".");
    tetromino[2].append(L".");

    // O piece — square
    tetromino[3] = L".";
    tetromino[3].append(L"X");
    tetromino[3].append(L"X");
    tetromino[3].append(L".");
    tetromino[3].append(L".");
    tetromino[3].append(L"X");
    tetromino[3].append(L"X");
    tetromino[3].append(L".");
    tetromino[3].append(L".");
    tetromino[3].append(L".");
    tetromino[3].append(L".");
    tetromino[3].append(L".");
    tetromino[3].append(L".");
    tetromino[3].append(L".");
    tetromino[3].append(L".");
    tetromino[3].append(L".");

    // L piece
    tetromino[4] = L".";
    tetromino[4].append(L"X");
    tetromino[4].append(L".");
    tetromino[4].append(L".");
    tetromino[4].append(L".");
    tetromino[4].append(L"X");
    tetromino[4].append(L".");
    tetromino[4].append(L".");
    tetromino[4].append(L".");
    tetromino[4].append(L"X");
    tetromino[4].append(L"X");
    tetromino[4].append(L".");
    tetromino[4].append(L".");
    tetromino[4].append(L".");
    tetromino[4].append(L".");
    tetromino[4].append(L".");

    // J piece
    tetromino[5] = L".";
    tetromino[5].append(L".");
    tetromino[5].append(L"X");
    tetromino[5].append(L".");
    tetromino[5].append(L".");
    tetromino[5].append(L".");
    tetromino[5].append(L"X");
    tetromino[5].append(L".");
    tetromino[5].append(L".");
    tetromino[5].append(L"X");
    tetromino[5].append(L"X");
    tetromino[5].append(L".");
    tetromino[5].append(L".");
    tetromino[5].append(L".");
    tetromino[5].append(L".");
    tetromino[5].append(L".");

    // T piece
    tetromino[6] = L".";
    tetromino[6].append(L"X");
    tetromino[6].append(L".");
    tetromino[6].append(L".");
    tetromino[6].append(L".");
    tetromino[6].append(L"X");
    tetromino[6].append(L"X");
    tetromino[6].append(L".");
    tetromino[6].append(L".");
    tetromino[6].append(L"X");
    tetromino[6].append(L".");
    tetromino[6].append(L".");
    tetromino[6].append(L".");
    tetromino[6].append(L".");
    tetromino[6].append(L".");
    tetromino[6].append(L".");
}

int main()
{
    InitAssets();

    EnableRawMode();
    atexit(DisableRawMode);  // Ensure terminal is restored on exit

    int frame = 0;
    int current_piece = 0;
    const int FPS = 10;  // 10 FPS for smooth updates
    const auto frame_time = std::chrono::milliseconds(1000 / FPS);
    const auto idle_threshold = std::chrono::seconds(5);
    
    auto last_input_time = std::chrono::high_resolution_clock::now();

    // Display all 7 tetromino shapes in a loop with keyboard control
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();

        // Handle keyboard input
        int key = GetKeyInput();
        if (key != 0) {
            current_piece = (current_piece + key + 7) % 7;  // +7 to handle negative wrapping
            last_input_time = start;  // Reset idle timer
        }

        // Auto-cycle if idle for 5 seconds
        auto idle_time = std::chrono::high_resolution_clock::now() - last_input_time;
        if (idle_time > idle_threshold) {
            current_piece = (current_piece + 1) % 7;
            last_input_time = start;  // Reset idle timer
        }

        // Clear screen and home cursor
        std::cout << CLEAR_SCREEN << CURSOR_HOME;

        // Draw frame info
        std::cout << "Frame: " << frame << " | Piece: " << current_piece << "\n\n";

        // Draw current tetromino piece
        const wchar_t* names[] = {L"I", L"S", L"Z", L"O", L"L", L"J", L"T"};
        std::wcout << names[current_piece] << L" piece (4x4):\n";
        for (int y = 0; y < 4; ++y) {
            for (int x = 0; x < 4; ++x) {
                std::wcout << tetromino[current_piece][y * 4 + x];
            }
            std::wcout << L'\n';
        }

        auto idle_secs = std::chrono::duration_cast<std::chrono::seconds>(idle_time).count();
        std::cout << "\nIdle: " << idle_secs << "s / 5s (auto-cycle)\n";
        std::cout << "Use LEFT/RIGHT arrows to navigate\n";
        std::cout << "Press Ctrl+C to exit\n";
        std::cout.flush();

        frame++;

        // Frame timing
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        auto sleep_time = frame_time - elapsed;
        if (sleep_time.count() > 0) {
            std::this_thread::sleep_for(sleep_time);
        }
    }

    return 0;
}

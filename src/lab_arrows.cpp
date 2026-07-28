// ============================================================================
// LAB 3: Arrow Key Input
// Learn how arrow key ANSI sequences are decoded
// Run: make lab_arrows && ./lab_arrows
// ============================================================================

#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <cstdlib>
#include <chrono>
#include <thread>

#define CLEAR_SCREEN "\033[2J"
#define CURSOR_HOME "\033[H"

termios original_termios;

void EnableRawMode()
{
    tcgetattr(STDIN_FILENO, &original_termios);
    termios raw = original_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void DisableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}

// Decode arrow key sequences
// Returns: -1=Left, 1=Right, -2=Up, 2=Down, 0=NoInput, other=Other
int GetKeyInput()
{
    unsigned char c;
    if (read(STDIN_FILENO, &c, 1) == 1) {
        if (c == 27) {  // ESC = start of ANSI sequence
            unsigned char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) == 1 &&
                read(STDIN_FILENO, &seq[1], 1) == 1) {
                if (seq[0] == '[') {
                    if (seq[1] == 'D') return -1;  // Left
                    if (seq[1] == 'C') return 1;   // Right
                    if (seq[1] == 'A') return -2;  // Up
                    if (seq[1] == 'B') return 2;   // Down
                }
            }
        }
        return (int)c;  // Regular character
    }
    return 0;  // No input
}

int main()
{
    std::cout << CLEAR_SCREEN << CURSOR_HOME;
    std::cout << "=== Arrow Key Input Lab ===\n\n";
    std::cout << "ANSI Arrow Key Sequences:\n";
    std::cout << "  Left:  ESC [ D  (\\033[D)\n";
    std::cout << "  Right: ESC [ C  (\\033[C)\n";
    std::cout << "  Up:    ESC [ A  (\\033[A)\n";
    std::cout << "  Down:  ESC [ B  (\\033[B)\n\n";
    
    std::cout << "Each sequence is 3 bytes: 27 ('[' + letter)\n";
    std::cout << "We read them one at a time to decode.\n\n";
    std::cout << "Try pressing arrow keys (or 'q' to quit)...\n";
    std::cout.flush();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    EnableRawMode();
    atexit(DisableRawMode);
    
    int x = 5, y = 8;
    bool running = true;
    
    while (running) {
        std::cout << CLEAR_SCREEN << CURSOR_HOME;
        std::cout << "Position: (" << x << ", " << y << ")\n\n";
        
        // Draw a simple grid
        for (int row = 0; row < 10; ++row) {
            for (int col = 0; col < 20; ++col) {
                if (row == y && col == x) {
                    std::cout << "@";  // Player position
                } else {
                    std::cout << ".";
                }
            }
            std::cout << "\n";
        }
        
        std::cout << "\nPress arrows to move, 'q' to quit\n";
        std::cout.flush();
        
        int key = GetKeyInput();
        if (key == -1 && x > 0) x--;           // Left
        if (key == 1 && x < 19) x++;           // Right
        if (key == -2 && y > 0) y--;           // Up
        if (key == 2 && y < 9) y++;            // Down
        if (key == 'q' || key == 'Q') running = false;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    std::cout << CLEAR_SCREEN << CURSOR_HOME;
    std::cout << "Arrow key lab complete!\n\n";
    std::cout << "You learned:\n";
    std::cout << "  - Arrow keys send 3-byte ANSI sequences\n";
    std::cout << "  - ESC (27) starts the sequence\n";
    std::cout << "  - '[' is always the middle byte\n";
    std::cout << "  - D/C/A/B encode the direction\n";
    std::cout << "  - Non-blocking read() allows immediate response\n";
    
    return 0;
}

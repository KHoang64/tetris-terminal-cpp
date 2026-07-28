// ============================================================================
// LAB 2: termios Raw Mode
// Learn how to enable non-blocking, no-echo keyboard input on macOS
// Run: make lab_termios && ./lab_termios
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
    std::cout << "Enabling raw mode...\n";
    std::cout.flush();
    
    // Save original settings
    tcgetattr(STDIN_FILENO, &original_termios);
    
    termios raw = original_termios;
    
    // Disable canonical mode (ICANON) — allow immediate input
    // Disable echo (ECHO) — don't print typed characters
    raw.c_lflag &= ~(ICANON | ECHO);
    
    // Non-blocking read
    raw.c_cc[VMIN] = 0;   // Return immediately even if no data
    raw.c_cc[VTIME] = 0;  // No timeout
    
    // Apply settings
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void DisableRawMode()
{
    std::cout << "\nDisabling raw mode...\n";
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}

int main()
{
    std::cout << CLEAR_SCREEN << CURSOR_HOME;
    std::cout << "=== termios Raw Mode Lab ===\n\n";
    std::cout << "This lab shows the difference between canonical and raw mode.\n\n";
    
    std::cout << "Part 1: Canonical Mode (Default)\n";
    std::cout << "  - Input is buffered until you press Enter\n";
    std::cout << "  - You see what you type (echo is on)\n";
    std::cout << "  - Type something and press Enter: ";
    std::cout.flush();
    
    std::string input;
    std::getline(std::cin, input);
    std::cout << "You typed: '" << input << "'\n\n";
    
    std::cout << "Part 2: Raw Mode\n";
    std::cout << "  - Input is immediate, no Enter needed\n";
    std::cout << "  - You won't see what you type (echo is off)\n";
    std::cout << "  - Press any 10 characters (no Enter needed)...\n";
    std::cout.flush();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    EnableRawMode();
    atexit(DisableRawMode);
    
    unsigned char chars[10];
    int count = 0;
    
    while (count < 10) {
        unsigned char c;
        if (read(STDIN_FILENO, &c, 1) == 1) {
            chars[count] = c;
            std::cout << ".";  // Show progress without echoing input
            std::cout.flush();
            count++;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    DisableRawMode();
    
    std::cout << "\n\nYou pressed: ";
    for (int i = 0; i < 10; ++i) {
        std::cout << chars[i];
    }
    std::cout << "\n\n";
    
    std::cout << "Key differences demonstrated:\n";
    std::cout << "  - ICANON: removed canonical mode\n";
    std::cout << "  - ECHO: removed echo\n";
    std::cout << "  - c_cc[VMIN]=0: immediate read\n";
    std::cout << "  - c_cc[VTIME]=0: no timeout\n";
    std::cout << "  - atexit(DisableRawMode): restore on exit\n";
    
    return 0;
}

// ============================================================================
// LAB 1: ANSI Escape Codes
// Learn ANSI codes for clearing screen, positioning cursor, basic rendering
// Run: make lab_ansi && ./lab_ansi
// ============================================================================

#include <iostream>
#include <string>
#include <chrono>
#include <thread>

// ANSI escape codes
#define CLEAR_SCREEN "\033[2J"      // Clear entire screen
#define CURSOR_HOME "\033[H"        // Move cursor to (0,0)
#define CURSOR_POS(x, y) "\033[" << (y) << ";" << (x) << "H"  // Move to (x,y)

int main()
{
    std::cout << "=== ANSI Escape Codes Lab ===\n";
    std::cout << "Press enter to see each demo...\n";
    std::cin.get();

    // Demo 1: Clear screen
    std::cout << "Demo 1: Clear Screen (\\033[2J)\n";
    std::cout << "This text will disappear...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << CLEAR_SCREEN << CURSOR_HOME;
    std::cout << "Screen cleared! Now showing at (0,0).\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    std::cin.get();

    // Demo 2: Cursor positioning
    std::cout << CLEAR_SCREEN << CURSOR_HOME;
    std::cout << "Demo 2: Cursor Positioning\n\n";
    std::cout << "Printing at different positions:\n\n";
    std::cout << "Line 5:     " << "This is at y=5\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "Line 7:     " << "This is at y=7\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "Line 10:    " << "This is at y=10\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    std::cin.get();

    // Demo 3: Animated clearing
    std::cout << CLEAR_SCREEN << CURSOR_HOME;
    std::cout << "Demo 3: Animation Loop (10 frames)\n\n";
    for (int i = 0; i < 10; ++i) {
        std::cout << CLEAR_SCREEN << CURSOR_HOME;
        std::cout << "Frame: " << i << "\n";
        std::cout << "This text updates in place.\n";
        std::cout << "No scrolling, just clearing + redrawing.\n";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    std::cout << "\n";
    std::cin.get();

    // Demo 4: Draw a simple box
    std::cout << CLEAR_SCREEN << CURSOR_HOME;
    std::cout << "Demo 4: Simple Box (using newlines)\n\n";
    std::cout << "  +--------+\n";
    std::cout << "  |        |\n";
    std::cout << "  | TETRIS |\n";
    std::cout << "  |        |\n";
    std::cout << "  +--------+\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    std::cout << "\nLab complete. You now understand:\n";
    std::cout << "  - \\033[2J clears the screen\n";
    std::cout << "  - \\033[H moves cursor to (0,0)\n";
    std::cout << "  - Combination allows smooth animation\n";
    std::cout << "  - No library needed, just POSIX terminal\n";

    return 0;
}


#include <clocale>
#include <iostream>
#include <locale>
#include <string>
using namespace std;

wstring tetromino[7];

int main()
{
    std::setlocale(LC_ALL, "");
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale());

    std::string s = "naïve";
    std::wstring w = L"naïve";

    std::cout << s << "\n";
    std::wcout << w << L"\n";

    tetromino[0].append(L"..X...X...X...X."); // I

    std::cout << "Tetris - Run 1\n";
    return 0;
}

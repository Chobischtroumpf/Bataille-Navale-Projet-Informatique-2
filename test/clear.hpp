#include <iostream>
#ifdef _WIN32
#include <windows.h> // Pour Windows
#else
#include <cstdlib>   // Pour Linux/macOS
#endif

void clear_screen() {
#ifdef _WIN32
    std::system("cls");
#else
    // Assume que les autres systèmes d'exploitation sont compatibles avec ANSI escape codes
    std::cout << "\033[2J\033[1;1H";
#endif
}
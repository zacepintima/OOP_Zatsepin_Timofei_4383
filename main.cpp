#include <windows.h>
#include "GameLoop.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    GameLoop loop;
    loop.run();
    return 0;
}
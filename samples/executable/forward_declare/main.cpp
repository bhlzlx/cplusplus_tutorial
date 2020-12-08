#include <cstdlib>
#include <cstdio>
#include "hardware/computer.h"
#include "header.txt"
// #include "hardware/mouse.h"

int main( int _argc, char** _argv ) {
    Mouse* mouse = createMouse();
    Computer* computer = new Computer();
    computer->plugMouse( mouse );
    computer->unplugMouse();
    //
    a = 0;
    //
    return 0;
}
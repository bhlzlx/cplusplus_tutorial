#include "computer.h"
#include "mouse.h"

void Computer::plugMouse( Mouse* _mouse ) {
    if( this->m_mouse ) {
        this->m_mouse->shake();
    }
    this->m_mouse = _mouse;
    m_mouse->flash();
}

void Computer::unplugMouse() {
    if( this->m_mouse ) {
        this->m_mouse->shake();
        this->m_mouse = nullptr;
    }
}

Mouse* createMouse() {
    return new Mouse();
}
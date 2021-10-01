#pragma once
class Mouse;        // 鼠标类，我们会在其它地方定义
class Keyboard;     // 只声明没定义
class Monitor;      // 只声明没定义

class Computer {
private:
    Mouse*          m_mouse;
    Keyboard*       m_keyboard;
    Monitor*        m_monitor;
public:
    void plugMouse( Mouse* _mouse );
    void unplugMouse();
    void plugKeyboard( Keyboard* );
    void unplugKeyboard();
    void plugMonitor( Monitor* );
    void unplugMonitor();
};

Mouse* createMouse();
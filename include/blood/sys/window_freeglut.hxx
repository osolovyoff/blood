#pragma once

#if defined(USE_FREEGLUT)
#include <string>
#include <GL/glut.h>

namespace blood { namespace sys {

class WindowFreeGlut
{
public:
    WindowFreeGlut(int argc, char* argv[]);
    ~WindowFreeGlut();

    void initialize();
    void release();

    bool isCreated() const;

    void show();
    
    void setSize(int width, int height);
    void setTitle(const std::string title);
    void setFullscreen(bool fullscreen = true);

public: // callback functions
    void setDraw(void(*)(void));
    void setKeyDown(void(*)(unsigned char key, int x, int y));
    void setKeyUp(void(*)(unsigned char key, int x, int y));
    void setMouse(void(*)(int button, int state, int x, int y));
    void setMouseWheel(void(*)(int wheel, int direction, int x, int y));
    void setJoystick(void(*)(unsigned int button, int x, int y, int z), int pollInterval);

private:
    std::string m_title;
    int         m_id;
    int         m_w, m_h;
    bool        m_fullscreen;

private:
    void *(m_fn_keyboard)(unsigned char, int, int);
};

}} // end blood::sys

#endif
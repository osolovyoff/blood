#include "blood/sys/window_freeglut.hxx"

#if defined(USE_FREEGLUT) 

#include <GL/freeglut.h>

using namespace blood::sys;

void WindowFreeGlut::mouse()
{
}

void keyboard(unsigned char key, int x, int y)
{
    if (key = 27)
        glutLeaveMainLoop();
}

void draw()
{
    
}

WindowFreeGlut::WindowFreeGlut(int argc, char* argv[])
: m_w(640)
, m_h(480)
, m_id(0)
, m_title("defult")
, m_fullscreen(false)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
}

WindowFreeGlut::~WindowFreeGlut()
{
    if (m_id)
        glutDestroyWindow(m_id);
}

void WindowFreeGlut::initialize()
{
    const bool is_mode = glutGameModeGet(GLUT_GAME_MODE_POSSIBLE);

    if (m_fullscreen && is_mode)
    {
        const int width = glutGet(GLUT_SCREEN_WIDTH);
        const int height = glutGet(GLUT_SCREEN_HEIGHT);

        char mode[24];
        sprintf(mode, "%dx%d:32@60", glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
        glutGameModeString(mode);

        glutEnterGameMode();
    }
    else
    {
        //m_id = glutCreateWindow(m_title.c_str());
    }

    float modelmatrix[16];
    static float ambient[] =
    { 0.0, 0.0, 0.0, 1.0 };
    static float diffuse[] =
    { 1.0, 1.0, 1.0, 1.0 };
    static float position[] =
    { 90.0, 90.0, -150.0, 0.0 };
    static float lmodel_ambient[] =
    { 1.0, 1.0, 1.0, 1.0 };
    
    // initialize spaces
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, 1.0, 2.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, modelmatrix);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_AUTO_NORMAL);
    glFrontFace(GL_CCW);

    glEnable(GL_MAP2_VERTEX_4);
    glClearColor(0.25, 0.25, 0.5, 0.0);
    // initialize 
}

void WindowFreeGlut::show()
{
    glutMainLoop();
}

void WindowFreeGlut::setFullscreen(bool fullscreen /*= true*/ )
{
    m_fullscreen = fullscreen;
}

void WindowFreeGlut::setTitle(const std::string title)
{
    m_title = title;
    //glutSetWindowTitle(m_title.c_str());
}

void WindowFreeGlut::setSize(int width, int height)
{
    glutInitWindowSize(width, height);
}

void blood::sys::WindowFreeGlut::setDraw(void(*fn)(void))
{
    glutDisplayFunc(fn);
}

void blood::sys::WindowFreeGlut::setKeyDown(void(*fn)(unsigned char key, int x, int y))
{
    glutKeyboardFunc(fn);
}

void blood::sys::WindowFreeGlut::setKeyUp(void(*fn)(unsigned char key, int x, int y))
{
    glutKeyboardUpFunc(fn);
}

void blood::sys::WindowFreeGlut::setMouse(void(*fn)(int button, int state, int x, int y))
{
    glutMouseFunc(fn);
}

void blood::sys::WindowFreeGlut::setMouseWheel(void(*fn)(int wheel, int direction, int x, int y))
{
    glutMouseWheelFunc(fn);
}

void blood::sys::WindowFreeGlut::setJoystick(void(*fn)(unsigned int button, int x, int y, int z), int pollInterval)
{
    glutJoystickFunc(fn,pollInterval);
}

#endif // end USE_FREEGLUT
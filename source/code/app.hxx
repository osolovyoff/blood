#pragma once
#include <memory>
#include <blood/gui/window_winapi.hxx>
#include "dxdevice.hxx"

class App;
extern std::unique_ptr<App> app;

class App
{
public:
    static App& get_instance() { return *app.get(); }
    void start();

public:
    std::unique_ptr<blood::gui::WindowWinApi> m_window;
    std::unique_ptr<DXDevice>                 m_device;
};
#include <memory>

#include "app.hxx"
std::unique_ptr<App> app;

int main()
{
    app.reset(new App);
    app->start();
    return 0;
}
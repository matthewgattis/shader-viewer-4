#pragma once

#include <string>
#include <vector>
#include <memory>

class Window;
class Context;

class App
{
public:
    App();
    ~App();

    void run(const std::vector<std::string> &args);

private:
    void init();
};


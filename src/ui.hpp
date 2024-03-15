#pragma once

#include <memory>

class Window;
class Context;

class Ui
{
public:
    Ui(
        const std::shared_ptr<Window>& window,
        const std::shared_ptr<Context>& context);

    ~Ui();
};


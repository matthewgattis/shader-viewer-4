#pragma once

#include <memory>

class Window;
class Context;

class UiContext
{
public:
    UiContext(
        const std::shared_ptr<Window>& window,
        const std::shared_ptr<Context>& context);

    ~UiContext();
};


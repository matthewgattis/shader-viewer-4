#include <string>
#include <vector>
#include <cstdlib>

#include "app.hpp"

#define LOG_MODULE_NAME "main"
#include "log.hpp"

int main(int argc, char **argv)
{
    log_init();

    std::vector<std::string> args(argv, argv + argc);

    LOG_INFO("application start");

    try
    {
        App app(args);
        app.run();
    }
    catch (const std::exception& e)
    {
        LOG_CRITICAL("uncaught exception: {}", e.what());
        return EXIT_FAILURE;
    }

    LOG_INFO("application quit");

    return EXIT_SUCCESS;
}

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include "app.hpp"

#define LOG_MODULE_NAME ("main")
#include "log.hpp"

int main(int argc, char **argv)
{
    std::vector<std::string> args;
    for (int i = 0; i < argc; i++)
        args.push_back(argv[i]);

    LOG_INFO << "application start" << std::endl;

    try
    {
        App app(args);
        app.run();
    }
    catch (const std::exception &e)
    {
        LOG_SEVERE << "uncaught exception: " << e.what() << "" << std::endl;
        return EXIT_FAILURE;
    }

    LOG_INFO << "application quit" << std::endl;

    return EXIT_SUCCESS;
}


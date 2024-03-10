#pragma once
#include <iostream>
#define LOG_INFO (std::clog << "[INFO] <" << LOG_MODULE_NAME << ">: ")
#define LOG_DEBUG (std::clog << "[DEBUG] <" << LOG_MODULE_NAME << ">: ")
#define LOG_WARNING (std::clog << "[WARNING] <" << LOG_MODULE_NAME << ">: ")
#define LOG_ERROR (std::clog << "[ERROR] <" << LOG_MODULE_NAME << ">: ")
#define LOG_SEVERE (std::clog << "[SEVERE] <" << LOG_MODULE_NAME << ">: ")


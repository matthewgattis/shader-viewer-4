#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// Call once at startup (main.cpp) to configure the default logger.
inline void log_init()
{
    auto logger = spdlog::stderr_color_mt("default");
    spdlog::set_default_logger(logger);
    spdlog::set_pattern("[%H:%M:%S.%e] [%^%l%$] [%n] %v");
    spdlog::set_level(spdlog::level::trace);
}

// Per-translation-unit logger: define LOG_MODULE_NAME before including this header.
// Returns (or lazily creates) a named logger that shares the default sink.
#ifndef LOG_MODULE_NAME
#  error "Define LOG_MODULE_NAME before including log.hpp"
#endif

inline spdlog::logger& log_get_logger(const char* name)
{
    auto existing = spdlog::get(name);
    if (existing)
        return *existing;

    // Create a child logger sharing the default logger's sinks and level.
    auto parent = spdlog::default_logger();
    auto child = std::make_shared<spdlog::logger>(name, parent->sinks().begin(), parent->sinks().end());
    child->set_level(parent->level());
    spdlog::register_logger(child);
    return *child;
}

#define LOG_LOGGER (log_get_logger(LOG_MODULE_NAME))

#define LOG_TRACE(...)   LOG_LOGGER.trace(__VA_ARGS__)
#define LOG_DEBUG(...)   LOG_LOGGER.debug(__VA_ARGS__)
#define LOG_INFO(...)    LOG_LOGGER.info(__VA_ARGS__)
#define LOG_WARN(...)    LOG_LOGGER.warn(__VA_ARGS__)
#define LOG_ERROR(...)   LOG_LOGGER.error(__VA_ARGS__)
#define LOG_CRITICAL(...)LOG_LOGGER.critical(__VA_ARGS__)

/**
 * @file logger.cpp
 * @author Feo
 * @date 09/08/2018
 * @brief
 */

#include <cstdarg>

#include <time.h>

#include "main/groot_consts.h"

#include "logger/logger.h"

namespace groot {

    /**
     * stdout_logger
     */

    // virtual
    void stdout_logger::log(level_t level,
                            const char *file,
                            size_t line,
                            const char *function,
                            const char *format,
                            ...) const noexcept
    {
        static std::unordered_map<level_t, std::string_view> levels {
                { level_t::EMERG, "EMERG" },
                { level_t::ALERT, "ALERT" },
                { level_t::CRIT, "CRIT" },
                { level_t::ERROR, "ERROR" },
                { level_t::WARN, "warn" },
                { level_t::NOTICE, "notice" },
                { level_t::INFO, "info" },
                { level_t::DEBUG, "debug" },
        };

        time_t rawtime;
        time(&rawtime);

        struct tm *timeinfo { localtime(&rawtime) };

        char buffer[consts::LOGGER_MAX_MESSAGE_LENGTH];
        char format_buffer[consts::LOGGER_MAX_MESSAGE_LENGTH];

        va_list args;
        va_start(args, format);
        vsnprintf(format_buffer, consts::LOGGER_MAX_MESSAGE_LENGTH, format, args);
        va_end(args);

        std::snprintf(buffer,
                      consts::LOGGER_MAX_MESSAGE_LENGTH,
                      "[%4d-%02d-%02d %02d:%02d:%02d][%s:%lu][%s] %s\n",
                      timeinfo->tm_year + 1900,
                      timeinfo->tm_mon + 1,
                      timeinfo->tm_mday,
                      timeinfo->tm_hour,
                      timeinfo->tm_min,
                      timeinfo->tm_sec,
                      &file[consts::SOURCE_DIR.size() + 1],
                      line,
                      //function,
                      levels[level].data(),
                      format_buffer
                );

        bool fatal = (level == level_t::EMERG ||
                level == level_t::ALERT ||
                level == level_t::CRIT ||
                level == level_t::ERROR);

        std::ostream& stream = fatal ? std::cerr : std::cout;

        stream << buffer;
    }

}
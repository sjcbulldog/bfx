#pragma once

#include <BfxConfig.hpp>
#include <BfxLogTypes.hpp>
#include <cstring>
#include <cstdint>

namespace Bfx {

    class LogSink ;

    class Logger {
    public:
        Logger() {
            memset(this->sink, 0, sizeof(this->sink)) ;
        }
        void log(LogLevel level, uint32_t module, const char* fmt, ...) ;

        void addSink(LogSink* sink) {
            for (uint32_t i = 0; i < BFX_MAX_LOG_SINKS; i++) {
                if (this->sink[i] == nullptr) {
                    this->sink[i] = sink ;
                    break ;
                }
            }
        }

    private:
        LogSink* sink[BFX_MAX_LOG_SINKS] ;
    };
}

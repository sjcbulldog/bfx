#include <BfxLogger.hpp>
#include <BfxLogTypes.hpp>
#include <BfxConfig.hpp>
#include <BfxLogSink.hpp>
#include <stdint.h>
#include <cstdarg>
#include <cstdio>

namespace Bfx {

static char buffer[1024] ;

void Logger::log(LogLevel level, uint32_t module, const char* fmt, ...) {
    va_list args ;
    va_start(args, fmt) ;
    vsnprintf(buffer, sizeof(buffer), fmt, args) ;
    va_end(args) ;

    for (uint32_t i = 0; i < BFX_MAX_LOG_SINKS; i++) {
        if (this->sink[i] != nullptr) {
            this->sink[i]->log(level, module, buffer) ;
        }
    }
}

}

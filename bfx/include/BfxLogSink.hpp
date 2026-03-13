#pragma once

#include <BfxLogTypes.hpp>
#include <cstdint>

namespace Bfx {
    class LogSink {
    public:
        virtual void log(LogLevel level, uint32_t module, const char* msg) = 0 ;
    };
}
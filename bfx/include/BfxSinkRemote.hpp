#pragma once

#include <BfxLogSink.hpp>
#include <BfxLogTypes.hpp>
#include <cstdarg>
#include <cstdio>

namespace Bfx {
    class LogSinkRemove : public LogSink {
    public:
        virtual void log(LogLevel level, uint32_t module, const char* msg) override {
        }
    };
}

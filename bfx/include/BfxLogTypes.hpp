#pragma once

namespace Bfx {
    enum LogLevel {
        LOG_LEVEL_ERROR = 0,
        LOG_LEVEL_WARNING,
        LOG_LEVEL_INFO,
        LOG_LEVEL_DEBUG
    } ;

#define BFX_LOG_MODULE              (0x00000000U)
}

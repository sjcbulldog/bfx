#pragma once

#include <cy_result.h>

namespace Bfx {

class Application ;
class Logger ;

class Child {
public:
    Child() { ; }
    void registerChild(Application* app)  ;

    // This method is called for hw initialization. It is called before any RTOS is initialized.  But if
    // retarget-io is enabled, then the retarget-io initialization is done before this method is called. 
    // So printf can be used for logging in this method.
    virtual cy_rslt_t hwinit() = 0 ;

    // This method is called after the RTOS is initialized. So this can be used to create tasks, timers, queues, etc.
    virtual cy_rslt_t init() = 0 ;

    // Return the logger for the application. This can be used by the child objects to log messages.
    Logger *getLogger() ;

private:
    Application* app ;

} ;
}

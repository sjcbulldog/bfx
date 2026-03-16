#pragma once

#include "mtb_hal.h"
#include <cy_result.h>
#include <FreeRTOS.h>
#include <Task.h>
#include <BfxConfig.hpp>
#include <BfxLogger.hpp>

namespace Bfx {

class Child ;
class Logger ;

class Application {
public:
    static const uint32_t CREATE_FLAG_RETARGET = (1U << 0) ;

public:
    Application(uint32_t flags = 0) ;

    // Initialize the application
    cy_rslt_t hwinit() ;

    // Run the application
    void run(UBaseType_t priority = tskIDLE_PRIORITY + 1, int stsize = configMINIMAL_STACK_SIZE) ;

    // Register a child object
    void registerChild(Child* child) ;

    // Get the logger object
    Logger *getLogger() {
        return this->logger ;
    }

protected:
    virtual void appThread() = 0 ;

    virtual void createLoggerSinks() = 0 ;

    void addLoggerSink(LogSink* sink) {
        if (this->logger != nullptr) {
            this->logger->addSink(sink) ;
        }
    }

private:
    void setup_clib_support(void) ;

    cy_rslt_t setup_retarget_io(void) ;
    
    void crash() ;

    void afterStartRTOS() ;

private:
    // The heap for the shared memory space
    void *shared_heap ;

    // The flags used during application creation
    uint32_t create_flags ;

    // Logger object for logging messages
    Logger* logger ;

    // RTC HAL object - for RTC integration into the C library 
    mtb_hal_rtc_t rtc_obj;    

    // The context for the UART retarget-io HAL object
    cy_stc_scb_uart_context_t DEBUG_UART_context;
    
    // The HAL object for the UART retarget-io
    mtb_hal_uart_t DEBUG_UART_hal_obj;

    // Task handle for the application thread
    TaskHandle_t task_handle ;

    // List of registered child objects
    Child* children[BFX_MAX_CHILD_OBJECTS];

} ;

}

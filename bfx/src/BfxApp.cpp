#include "BfxConfig.hpp"
#include "cy_time.h"
#include "cybsp.h"
#include "heap.h"
#include "cy_retarget_io.h"
#include <BfxApp.hpp>
#include <BfxChild.hpp>
#include <BfxLogger.hpp>
#include <BfxLogSinkPrintf.hpp>
#include <FreeRTOSConfig.h>

#define APP_LPTIMER_INTERRUPT_PRIORITY      (1U)
#define LPTIMER_0_WAIT_TIME_USEC            (62U)

// This is a shared memory pool used by the BFX framework for communications between the cores. 
// The allocator strategy is defined by BFX_SHARED_MEMORY_ALLOCATOR_STRATEGY in BfxConfig.hpp
// This CY_SECTION_SHAREDMEM forces this block to be placed in a shared memory section defined in the linker script.
CY_SECTION_SHAREDMEM static uint8_t shared_memory_pool[BFX_SHARED_MEMORY_SIZE] ;

namespace Bfx {
Application::Application(uint32_t flags) : create_flags(flags), logger(nullptr), task_handle(nullptr) {
    memset(this->children, 0, sizeof(this->children)) ;
    shared_heap = heapInit(shared_memory_pool, sizeof(shared_memory_pool)) ;

}

void Application::crash() {
    /* Disable all interrupts. */
    __disable_irq();

    CY_ASSERT(0);

    /* Infinite loop */
    while(true);
}

cy_rslt_t Application::hwinit() {
    cy_rslt_t res ;

    res = cybsp_init() ;
    if (CY_RSLT_SUCCESS != res) {
        return res ;
    }

    /* Setup CLIB support library. */
    setup_clib_support();

    this->logger = new Logger() ;

    if (create_flags & CREATE_FLAG_RETARGET) {
        res = setup_retarget_io() ;
        if (CY_RSLT_SUCCESS != res) {
            return res ;
        }
    }

    this->createLoggerSinks() ;

    this->logger->log(LOG_LEVEL_DEBUG, BFX_LOG_MODULE, "Calling hwinit of child objects") ;

    for(uint32_t i = 0; i < BFX_MAX_CHILD_OBJECTS; i++) {
        if (this->children[i] != nullptr) {
            res = this->children[i]->hwinit() ;
            if (CY_RSLT_SUCCESS != res) {
                return res ;
            }
        }
    }

    return CY_RSLT_SUCCESS;
}

void Application::registerChild(Child* child) {
    for (uint32_t i = 0; i < BFX_MAX_CHILD_OBJECTS; i++) {
        if (this->children[i] == nullptr) {
            this->children[i] = child ;
            return ;
        }
    }

    printf("Failed to register child object. Max limit reached.\r\n") ;
    crash() ;
}

void Application::run(UBaseType_t priority, int stsize) {
    BaseType_t ret ;

    ret = xTaskCreate([](void* param) {
        Application* app = static_cast<Application*>(param) ;
        app->afterStartRTOS() ;
    }, "appThread", stsize, this, priority, &this->task_handle) ;

    if (ret != pdPASS) {
        printf("Failed to create app thread.\r\n") ;
        crash() ;
    }

    vTaskStartScheduler() ;
}   

void Application::afterStartRTOS() 
{
    for(uint32_t i = 0; i < BFX_MAX_CHILD_OBJECTS; i++) {
        if (this->children[i] != nullptr) {
            cy_rslt_t res = this->children[i]->init() ;
            if (CY_RSLT_SUCCESS != res) {
                printf("Child initialization failed.\r\n") ;
                crash() ;
            }
        }
    }

    this->appThread() ;
}

void Application::setup_clib_support(void)
{
    /* RTC Initialization */
    Cy_RTC_Init(&CYBSP_RTC_config);
    Cy_RTC_SetDateAndTime(&CYBSP_RTC_config);

    /* Initialize the ModusToolbox CLIB support library */
    mtb_clib_support_init(&rtc_obj);
}

cy_rslt_t Application::setup_retarget_io(void)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;

    /* Initialize the SCB UART */
    result = (cy_rslt_t)Cy_SCB_UART_Init(CYBSP_DEBUG_UART_HW,
                                        &CYBSP_DEBUG_UART_config,
                                        &DEBUG_UART_context);

    /* UART initialization failed. Stop program execution. */
    if (CY_RSLT_SUCCESS != result)
    {
        return result ;
    }

    /* Enable the SCB UART */
    Cy_SCB_UART_Enable(CYBSP_DEBUG_UART_HW);

    result = mtb_hal_uart_setup(&DEBUG_UART_hal_obj,
                                &CYBSP_DEBUG_UART_hal_config,
                                &DEBUG_UART_context, NULL);

    /* UART setup failed. Stop program execution. */
    if (CY_RSLT_SUCCESS != result)
    {
        return result ;
    }

    /* Initialize retarget-io to use the debug UART port. */
    result = cy_retarget_io_init(&DEBUG_UART_hal_obj);

    /* retarget-io initialization failed. Stop program execution. */
    if (CY_RSLT_SUCCESS != result)
    {
        return result ;
    }

    printf("Retarget-io initialization successful.\r\n");

     return CY_RSLT_SUCCESS ;

    return CY_RSLT_SUCCESS ;

}

}

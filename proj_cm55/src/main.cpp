#include <MyApp.hpp>

/*******************************************************************************
* Header Files
*******************************************************************************/
#include "cybsp.h"
#include "cy_time.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cyabs_rtos.h"
#include "cyabs_rtos_impl.h"

static void handle_app_error(void)
{
    /* Disable all interrupts. */
    __disable_irq();

    CY_ASSERT(0);

    /* Infinite loop */
    while(true);
}

MyApp app ;
int main(void)
{
    if (CY_RSLT_SUCCESS != app.hwinit()) {
        handle_app_error();
    }

    /* Enable global interrupts */
    __enable_irq();

   app.run() ; 
}

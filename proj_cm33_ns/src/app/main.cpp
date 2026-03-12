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

/*******************************************************************************
 * Macros
 ******************************************************************************/

/* The timeout value in microsecond used to wait for the CM55 core to be booted.
 * Use value 0U for infinite wait till the core is booted successfully.
 */
#define CM55_BOOT_WAIT_TIME_USEC            (10U)

/* App boot address for CM55 project */
#define CM55_APP_BOOT_ADDR                  (CYMEM_CM33_0_m55_nvm_START + CYBSP_MCUBOOT_HEADER_SIZE)

/*******************************************************************************
* Function Name: handle_app_error
********************************************************************************
* Summary:
* User defined error handling function
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void handle_app_error(void)
{
    /* Disable all interrupts. */
    __disable_irq();

    CY_ASSERT(0);

    /* Infinite loop */
    while(true);
}

MyApp app(Bfx::Application::CREATE_FLAG_RETARGET) ;
int main(void)
{
    if (CY_RSLT_SUCCESS != app.hwinit()) {
        handle_app_error();
    }

    /* Enable CM55. */
    /* CM55_APP_BOOT_ADDR must be updated if CM55 memory layout is changed.*/
    Cy_SysEnableCM55(MXCM55, CM55_APP_BOOT_ADDR, CM55_BOOT_WAIT_TIME_USEC);

    /* Enable global interrupts */
    __enable_irq();

    app.run() ;
    
}

/* [] END OF FILE */

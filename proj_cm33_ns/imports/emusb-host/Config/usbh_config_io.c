
/*********************************************************************
*
*       #include Section
*
**********************************************************************
*/
#include "USBH.h"

extern void loggerputs(const char *s) ;

/*********************************************************************
*
*       Defines, configurable
*
*       This section is normally the only section which requires
*       changes on most embedded systems.
*/

/* The standard output methods are enabled by default
 * To disable an output in Debug mode or provide a custom
 * output method, set USBH_DISABLE_STANDARD_OUTPUT=1 in DEFINES
 * variable in the application project Makefile
 */
#define USBH_DISABLE_STANDARD_OUTPUT        (1U)

#ifndef   SHOW_TIME
    #define SHOW_TIME                           (1U)
#endif

#ifndef   SHOW_TASK
    #define SHOW_TASK                           (1U)
    #include "cyabs_rtos.h"
#endif

#if SHOW_TIME
static char * _WriteUnsigned(char * s, U32 v, int NumDigits) {
    unsigned   Base;
    unsigned   Div;
    U32        Digit;
    Digit    = 1;
    Base     = 10;

    /* Count how many digits are required */
    while (((v / Digit) >= Base) || (NumDigits > 1))
    {
        NumDigits--;
        Digit *= Base;
    }

    /* Output digits */
    do
    {
        Div = v / Digit;
        v  -= Div * Digit;
        *s++ = (char)('0' + Div);
        Digit /= Base;
    } while (Digit);
    *s = 0;
    return s;
}
#endif

/*********************************************************************
*
*       _ShowStamp
*
*  Function description
*    Prints a time-stamp and the name of the task from which
*    the function was executed.
*
*/
static void _ShowStamp(void) {
#if SHOW_TIME
    U32    Time;
    char   ac[20];
    char * sBuffer = &ac[0];
    Time           = USBH_OS_GetTime32();
    sBuffer        = _WriteUnsigned(sBuffer, Time / 1000, 0);
    *sBuffer++     = ':';
    sBuffer        = _WriteUnsigned(sBuffer, Time % 1000, 3);
    *sBuffer++     = ' ';
    *sBuffer++     = 0;
    loggerputs(ac);
#endif

#if SHOW_TASK
{
    const char * s = NULL;
    cy_rslt_t result;
    cy_thread_t current_thread_handle;
    result = cy_rtos_thread_get_handle(&current_thread_handle);
    if (CY_RSLT_SUCCESS == result)
    {
        result = cy_rtos_thread_get_name(&current_thread_handle, &s);
        if (CY_RSLT_SUCCESS != result)
        {
            s = "ERROR: Unknown task name";
        }
    }
    else
    {
        s = "ERROR: Unknown task name";
    }

    if (s)
    {
        loggerputs(s);
        loggerputs(" - ");
    }
}
#endif
}

void USBH_Panic(const char * s) {
/* Uncomment to enter critical section.
 * FreeRTOS API functions must not be called from within a critical
 * section. As a retarget-io uses mutex, the printf() can not be used
 * inside critical section.
 */
/*
 * USBH_OS_DisableInterrupt();
 */
#if (USBH_DEBUG > 1)
    loggerputs("*** Fatal error, System halted: ");
    loggerputs(s);
    loggerputs("\r\n");
#else
    (void)s;
#endif
    USBH_HALT;
}

/*********************************************************************
*
*       USBH_Log
*
*  Function description
*    This function is called by the stack in debug builds with log output.
*    In a release build, this function is not be linked in.
*
*  Parameters
*    s - Pointer to a string holding the log message.
*/
void USBH_Log(const char * s) {
/* Uncomment to enter critical section.
 * FreeRTOS API functions must not be called from within a critical
 * section. As a retarget-io uses mutex, the printf() can not be used
 * inside critical section.
 */
/*
 * USBH_OS_DisableInterrupt();
 */
    _ShowStamp();
    loggerputs(s);
    loggerputs("\r\n");
/* Uncomment to exit critical section.
 * FreeRTOS API functions must not be called from within a critical
 * section. As a retarget-io uses mutex, the printf() can not be used
 * inside critical section.
 */
/*
 * USBH_OS_EnableInterrupt();
 */
}

/*********************************************************************
*
*       USBH_Warn
*
*  Function description
*    This function is called by the stack in debug builds with log output.
*    In a release build, this function is not be linked in.
*
*  Parameters
*    s - Pointer to a string holding the warning message.
*/
void USBH_Warn(const char * s) {
/* Uncomment to enter critical section.
 * FreeRTOS API functions must not be called from within a critical
 * section. As a retarget-io uses mutex, the printf() can not be used
 * inside critical section.
 */
/*
 * USBH_OS_DisableInterrupt();
 */
    _ShowStamp();
    loggerputs("*** Warning *** ");
    loggerputs(s);
    loggerputs("\r\n");
/* Uncomment to exit critical section.
 * FreeRTOS API functions must not be called from within a critical
 * section. As a retarget-io uses mutex, the printf() can not be used
 * inside critical section.
 */
/*
 * USBH_OS_EnableInterrupt();
 */
}

/*********************************************************************
*
*       USBH_Puts
*
*  Function description
*    Prints a string without any additional output (no timestamp or newlines).
*
*  Parameters
*    s - Pointer to a string holding the warning message.
*/
void USBH_Puts(const char * s) {
/* Uncomment to enter critical section.
 * FreeRTOS API functions must not be called from within a critical
 * section. As a retarget-io uses mutex, the printf() can not be used
 * inside critical section.
 */
/*
 * USBH_OS_DisableInterrupt();
 */
    loggerputs(s);
/* Uncomment to exit critical section.
 * FreeRTOS API functions must not be called from within a critical
 * section. As a retarget-io uses mutex, the printf() can not be used
 * inside critical section.
 */
/*
 * USBH_OS_EnableInterrupt();
 */
}

/*************************** End of file ****************************/

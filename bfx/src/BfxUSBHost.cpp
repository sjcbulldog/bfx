#include "BfxUsbHidDev.hpp"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include <task.h>
#include <BfxUSBHost.hpp>
#include <BfxLogTypes.hpp>
#include <BfxLogger.hpp>

#include <USBH_HID.h>
#include <stdio.h>

#define USB_MAIN_TASK_MEMORY_REQ  (1024U)
#define USB_ISR_TASK_MEMORY_REQ   (1024U)

static char buffer[128] ;

extern "C" {
void loggerputs(const char *s) 
{
    strncat(buffer, s, sizeof(buffer) - strlen(buffer) - 1) ;
    int len = strlen(buffer) ;
    if (len > 1 && buffer[len - 1] == '\n' && buffer[len - 2] == '\r') {
        buffer[len - 2] = '\0' ;
        Bfx::Logger *logger = Bfx::UsbHost::getInstance()->getLogger() ;
        if (logger) {
            logger->log(Bfx::LogLevel::LOG_LEVEL_DEBUG, BFX_LOG_MODULE, "%s", buffer) ;
        }
        buffer[0] = '\0' ;
    }
}

}

namespace Bfx {

UsbHost* UsbHost::theOne = nullptr ;

UsbHost::UsbHost()
{
    if (theOne == nullptr) {
        theOne = this ;
    }
}

cy_rslt_t UsbHost::hwinit() {
    return CY_RSLT_SUCCESS ;
}

cy_rslt_t UsbHost::init() { 
    BaseType_t ret ;
    
    /* Initialize USBH stack */
    USBH_Init();

    /* Create two tasks which are mandatory for USBH operation */
    ret = xTaskCreate([](void* param) {
            UsbHost* app = static_cast<UsbHost*>(param) ;
            app->usbh_task(nullptr) ;
        }, "usbh_task", USB_MAIN_TASK_MEMORY_REQ, NULL, configMAX_PRIORITIES - 1, &usbh_task_handle);

    if (pdPASS != ret)
    {
        return BFX_USBHOST_TASK_CREATE_FAILED ;
    }

    ret = xTaskCreate([](void* param) {
        UsbHost* app = static_cast<UsbHost*>(param) ;
        app->usbh_isr_task(nullptr) ;
    }  , "usbh_isr_task", USB_ISR_TASK_MEMORY_REQ, NULL, configMAX_PRIORITIES - 2, &usbh_irq_task_handle);

    if (pdPASS != ret)
    {
        return BFX_USBHOST_TASK_CREATE_FAILED ;
    }


    USBH_HID_Init() ;
    USBH_HID_AddNotification(&hook, [](void* usb_context, uint8_t usb_index, USBH_DEVICE_EVENT usb_event) {
            UsbHost *h = static_cast<UsbHost*>(usb_context) ;
            h->device_notify(usb_index, usb_event);
        } ,this) ;    

    return CY_RSLT_SUCCESS ;
}

void UsbHost::usbh_task(void* arg)
{
    CY_UNUSED_PARAMETER(arg);

    while(true)
    {
        USBH_Task();
    }
}

void UsbHost::usbh_isr_task(void* arg)
{
    CY_UNUSED_PARAMETER(arg);

    while(true)
    {
        USBH_ISRTask();
    }
}

void UsbHost::addDevice(uint8_t usb_index) 
{
    this->getLogger()->log(Bfx::LogLevel::LOG_LEVEL_DEBUG, BFX_LOG_MODULE, "USB add device at index %d", usb_index) ;    
    UsbHidDev *dev = new UsbHidDev(*this, usb_index) ;
    cy_rslt_t res = this->addDevice(dev) ;
    if (res != CY_RSLT_SUCCESS) {
        this->getLogger()->log(Bfx::LogLevel::LOG_LEVEL_ERROR, BFX_LOG_MODULE, "Failed to add USB HID device at index %d, error 0x%x", usb_index, res) ; 
        delete dev ;
    }
    else {
        if (dev->open() == CY_RSLT_SUCCESS) {
            dev->logDeviceInfo() ;
        }
    }
}

void UsbHost::device_notify(uint8_t usb_index, USBH_DEVICE_EVENT usb_event) 
{
    if (usb_event == USBH_DEVICE_EVENT_ADD) {
        this->addDevice(usb_index) ;
    }
}

}

#pragma once

#include "BfxApp.hpp"
#include <BfxChild.hpp>
#include <BfxErrors.hpp>
#include <BfxConfig.hpp>
#include <USBH.h>
#include <FreeRTOS.h>
#include <Task.h>


namespace Bfx {

class UsbHidDev ;
class Application ;

class UsbHost : public Child {

    friend void usb_device_notify(void* usb_context, uint8_t usb_index, USBH_DEVICE_EVENT usb_event) ;

public:
    UsbHost() ;

    static UsbHost *getInstance() {
        return theOne ;
    }

    cy_rslt_t addDevice(UsbHidDev *dev) {
        for (unsigned int i = 0; i < BFX_MAX_USB_HID_DEVICES; i++) {
            if (this->hid_devs[i] == nullptr) {
                this->hid_devs[i] = dev ;
                return CY_RSLT_SUCCESS ;
            }
        }
        return BFX_USBHOST_TOO_MANY_HID_DEVICES ;
    }

    cy_rslt_t hwinit() override  ;
    cy_rslt_t init() override ;

private:
    void usbh_task(void *) ;
    void usbh_isr_task(void *) ;
    void device_notify(uint8_t usb_index, USBH_DEVICE_EVENT usb_event) ;
    void addDevice(uint8_t usb_index) ;

private:
    static UsbHost *theOne ;

    TaskHandle_t usbh_task_handle ;
    TaskHandle_t usbh_irq_task_handle ;
    USBH_NOTIFICATION_HOOK hook ;

    UsbHidDev *hid_devs[BFX_MAX_USB_HID_DEVICES] ;
    
} ;
}
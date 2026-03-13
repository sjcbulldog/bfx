#pragma once

#include <cy_result.h>
#include <USBH_HID.h>

namespace Bfx {
class UsbHost ;

class UsbHidDev {
public:
    UsbHidDev(UsbHost &host, uint8_t index);

    cy_rslt_t open() ;
    cy_rslt_t close() ;

    void logDeviceInfo() ;

private:
    static UsbHidDev *s_instance ; // Static instance pointer for callback access

    void hid_notification_callback(void *pContext, uint8_t index, USBH_DEVICE_EVENT event) ;

    void mouse_event(USBH_HID_MOUSE_DATA* pMouseData) ;
    void keyword_event(USBH_HID_KEYBOARD_DATA* pKeyData) ;

private:
    // The USB host that owns this device
    UsbHost &m_host ;

    // The index of the USB device (as assigned by the USB host)
    uint8_t m_index ;

    // The handle for the opened HID device
    USBH_HID_HANDLE m_handle ;

} ;
}

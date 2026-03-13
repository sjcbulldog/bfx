#include <FreeRTOS.h>
#include <BfxUSBHost.hpp>
#include <BfxLogTypes.hpp>
#include <BfxLogger.hpp>
#include <BfxUsbHidDev.hpp>
#include <BfxErrors.hpp>

namespace Bfx {
UsbHidDev *UsbHidDev::s_instance = nullptr ;

UsbHidDev::UsbHidDev(UsbHost &host, uint8_t index) : m_host(host), m_index(index) {
    s_instance = this ;
}

cy_rslt_t UsbHidDev::open() {
    this->m_handle = USBH_HID_Open(this->m_index) ;
    if (this->m_handle == USBH_HID_INVALID_HANDLE) {
        return BFX_USBHOST_CANNOT_OPEN_HID_DEVICE ;
    }

    USBH_HID_SetOnMouseStateChange([](USBH_HID_MOUSE_DATA* pMouseData) -> void {
        s_instance->mouse_event(pMouseData) ;
    }) ;

    USBH_HID_SetOnKeyboardStateChange([](USBH_HID_KEYBOARD_DATA* pKeyData) -> void {
        s_instance->keyword_event(pKeyData) ;
    }) ;

    return CY_RSLT_SUCCESS ;
}

void UsbHidDev::mouse_event(USBH_HID_MOUSE_DATA* pMouseData) {
    if (pMouseData != nullptr) {
        this->m_host.getLogger()->log(Bfx::LogLevel::LOG_LEVEL_INFO, BFX_LOG_MODULE, "Mouse Event: X: %d, Y: %d, Wheel: %d, Buttons: 0x%02X", 
            pMouseData->xChange, pMouseData->yChange, pMouseData->WheelChange, pMouseData->ButtonState) ;
    }
}

void UsbHidDev::keyword_event(USBH_HID_KEYBOARD_DATA* pKeyData) {
    if (pKeyData != nullptr) {
        this->m_host.getLogger()->log(Bfx::LogLevel::LOG_LEVEL_INFO, BFX_LOG_MODULE, "Keyboard Event: Modifier: 0x%02X, KeyCode: 0x%02X", pKeyData->Code, pKeyData->Value) ;
    }
}

cy_rslt_t UsbHidDev::close() {
    if (this->m_handle != USBH_HID_INVALID_HANDLE) {
        USBH_HID_Close(this->m_handle) ;
        this->m_handle = USBH_HID_INVALID_HANDLE ;
    }

    return CY_RSLT_SUCCESS ;
}

void UsbHidDev::logDeviceInfo() {
    USBH_HID_DEVICE_INFO info ;
    if (USBH_HID_GetDeviceInfo(this->m_handle, &info) != USBH_STATUS_SUCCESS) {
        this->m_host.getLogger()->log(Bfx::LogLevel::LOG_LEVEL_ERROR, BFX_LOG_MODULE, "Failed to get HID device info") ;
    }
    else {
        this->m_host.getLogger()->log(Bfx::LogLevel::LOG_LEVEL_INFO, BFX_LOG_MODULE, "HID Device Info: Vendor ID: 0x%04X, Product ID: 0x%04X", info.VendorId, info.ProductId) ;
    }
}

}

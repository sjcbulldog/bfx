#include "MyApp.hpp"

MyApp::MyApp(uint32_t flags) : Bfx::Application(flags) 
{
    this->usbHost.registerChild(this) ;
}

void MyApp::appThread() 
{
    while (1) {
        Cy_SysLib_Delay(1000);
    }
}
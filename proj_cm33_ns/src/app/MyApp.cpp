#include "MyApp.hpp"
#include "BfxLogTypes.hpp"

MyApp::MyApp() : Bfx::Application()
{
    // this->usbHost.registerChild(this) ;
}

void MyApp::appThread() 
{
    while (1) {
        Cy_SysLib_Delay(1000);
        getLogger()->log(Bfx::LOG_LEVEL_INFO, 0, "MyApp - Hello, world!") ;
    }
}

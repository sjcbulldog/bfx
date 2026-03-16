#pragma once

#include <BfxApp.hpp>
#include <BfxUSBHost.hpp>
#include <BfxLogSinkPrintf.hpp>

class MyApp : public Bfx::Application {
public:
    MyApp() ;

    void appThread() override ;

protected:
    void createLoggerSinks() override {
        this->addLoggerSink(new Bfx::LogSinkPrintf()) ;
    }

private:
    Bfx::UsbHost usbHost;
} ;

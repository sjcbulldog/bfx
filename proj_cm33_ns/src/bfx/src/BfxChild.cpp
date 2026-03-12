#include <BfxChild.hpp>
#include <BfxApp.hpp>

namespace Bfx {
void Child::registerChild(Application* app) {
    this->app = app ;
    app->registerChild(this) ;
}

Logger *Child::getLogger() {
    if (app) {
        return app->getLogger() ;
    }
    return nullptr ;
}
}

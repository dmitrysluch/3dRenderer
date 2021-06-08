#pragma once
#include "Kernel.h"

namespace renderer {
class BaseController {
   public:
    BaseController(Kernel *kernel) : kernel_(kernel) {}
   protected:
    Kernel &GetKernel();
   private:
    Kernel *kernel_;
};
} // renderer
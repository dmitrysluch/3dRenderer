#pragma once
#include "Mesh.h"
#include "BasicMaterial.h"
#include "Kernel.h"
namespace renderer {
class Object {
   public:
    

   private:
    Kernel *kernel_;
    shared_ptr<Mesh> mesh_;
    vector<shared_ptr<const BasicMaterial>> material_overrides_;
};
}

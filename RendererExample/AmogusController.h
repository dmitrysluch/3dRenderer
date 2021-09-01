#include <BaseController.h>
#include <Object.h>
#include <Kernel.h>
#include <Keycodes.h>
#include <Library/Observer/Observer.h>

using namespace renderer;
using namespace NSLibrary;

class AmogusController : BaseController {
   public:
    AmogusController(Kernel *kernel, CObservableData<KeyCode> *observable);
    void OnKeyDown(KeyCode code);

   private:
    unique_ptr<CObserverColdStrict<KeyCode>> observer_;
    vector<Object *> amogus_;
    int active_ = 0;
    shared_ptr<Mesh> amogus_mesh_;
    MaterialVec amogus_mat_;
};
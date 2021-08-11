// [CFXS] //
#include "CFXS.hpp"
#include <driverlib/interrupt.h>

namespace CFXS::CPU {

    static bool __noinit s_InterruptsEnabled;

    void EnableInterrupts() {
        s_InterruptsEnabled = true;
        IntMasterEnable();
    }

    void DisableInterrupts() {
        IntMasterDisable();
        s_InterruptsEnabled = false;
    }

    bool InterruptsEnabled() {
        return s_InterruptsEnabled;
    }

} // namespace CFXS::CPU
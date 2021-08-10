#pragma once

#ifdef __cplusplus

namespace CFXS::CPU {

    void EnableInterrupts();
    void DisableInterrupts();
    bool InterruptsEnabled();

} // namespace CFXS::CPU

#endif
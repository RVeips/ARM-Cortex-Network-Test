#pragma once

#define CFXS_println(...)    \
    {                        \
        printf(__VA_ARGS__); \
        printf("\n");        \
    }

#define CFXS_printf(...) printf(__VA_ARGS__);

#ifdef __cplusplus

namespace CFXS::CPU {

    void EnableInterrupts();
    void DisableInterrupts();
    bool InterruptsEnabled();

} // namespace CFXS::CPU

#endif
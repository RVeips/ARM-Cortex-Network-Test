#include "lwiplib.h"

namespace CFXS::CPU {
    uint32_t CLOCK_FREQUENCY = 120000000;
}

extern "C" {
bool e_LWIPLock = false;
void CheckGlobalLWIPLock() {
    if (e_LWIPLock) {
        asm volatile("bkpt #1");
    }
}
void lwIPHostTimerHandler() {
}
}

int main() {
    uint8_t mac[]    = {1, 2, 3, 4, 5, 6};
    uint32_t ip      = 0x02001010;
    uint32_t mask    = 0xFF000000;
    uint32_t gateway = 0;
    lwIPInit(CFXS::CPU::CLOCK_FREQUENCY, mac, ip, mask, gateway, IPADDR_USE_STATIC);

    while (1 < 2) {}
}
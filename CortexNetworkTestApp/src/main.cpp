#include "lwiplib.h"
#include <CFXS/Base/Network/IPv4.hpp>
#include <CFXS/Base/Network/MAC_Address.hpp>

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
    CFXS::MAC_Address mac = {0x08, 0x00, 0x28, 0x5A, 0x92, 0x4F};
    CFXS::IPv4 ip         = {192, 168, 1, 222};
    CFXS::IPv4 mask       = {255, 255, 255, 0};
    CFXS::IPv4 gateway    = {0, 0, 0, 0};
    lwIPInit(CFXS::CPU::CLOCK_FREQUENCY, mac.GetDataPointer(), ip.GetValue(), mask.GetValue(), gateway.GetValue(), IPADDR_USE_STATIC);

    while (1 < 2) {}
}
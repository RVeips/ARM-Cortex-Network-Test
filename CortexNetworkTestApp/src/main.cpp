#include "lwiplib.h"
#include <CFXS/Base/Network/IPv4.hpp>
#include <CFXS/Base/Network/MAC_Address.hpp>
#include <driverlib_includes.hpp>
#include <driverlib/systick.h>

namespace CFXS::CPU {
    extern uint32_t CLOCK_FREQUENCY;
}

namespace CFXS::Time {
    uint64_t ms;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
static constexpr CFXS::MAC_Address mac = {0x08, 0x00, 0x28, 0x5A, 0x92, 0x4F};
static constexpr CFXS::IPv4 ip         = {192, 168, 1, 222};
static constexpr CFXS::IPv4 mask       = {255, 255, 255, 0};
static constexpr CFXS::IPv4 gateway    = {0, 0, 0, 0};
//////////////////////////////////////////////////////////////////////////////////////////////////////////
static constexpr uint32_t Ports[] = {GPIO_PORTN_BASE, GPIO_PORTN_BASE, GPIO_PORTF_BASE, GPIO_PORTF_BASE};
static constexpr uint32_t Pins[]  = {GPIO_PIN_1, GPIO_PIN_0, GPIO_PIN_4, GPIO_PIN_0};
#define LED(id, state) GPIOPinWrite(Ports[id], Pins[id], (state) ? Pins[id] : 0)
#define TOGGLE_LED(id) GPIOPinWrite(Ports[id], Pins[id], !GPIOPinRead(Ports[id], Pins[id]) ? Pins[id] : 0)
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_GPION) || !ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)) {}
    for (int i = 0; i < 4; i++) {
        ROM_GPIOPinTypeGPIOOutput(Ports[i], Pins[i]);
        LED(i, 0);
    }

    IntRegister(INT_EMAC0, []() __interrupt {
        TOGGLE_LED(1);
        lwIPEthernetIntHandler();
    });
    lwIPInit(CFXS::CPU::CLOCK_FREQUENCY, mac.GetDataPointer(), ip.GetValue(), mask.GetValue(), gateway.GetValue(), IPADDR_USE_STATIC);

    SysTickPeriodSet(120000);
    SysTickIntRegister([]() __interrupt {
        CFXS::Time::ms++;
    });
    SysTickEnable();
    SysTickIntEnable();

    raw_pcb *socket = raw_new(0xFF);

    raw_recv(
        socket,
        [](void *arg, struct raw_pcb *pcb, struct pbuf *p, const ip_addr_t *addr) -> uint8_t {
            TOGGLE_LED(3);
            return 0;
        },
        nullptr);
    raw_bind(socket, &ip_addr_any);
    ip_set_option(socket, SOF_BROADCAST);

    while (1 < 2) {
        LED(0, (CFXS::Time::ms / 250) & 1);
    }
}
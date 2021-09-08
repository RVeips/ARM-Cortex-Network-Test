// ---------------------------------------------------------------------
// CFXS ARM Network Test App <https://github.com/RVeips/ARM-Cortex-Network-Test>
// Copyright (C) 2021 | CFXS / Rihards Veips
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>
// ---------------------------------------------------------------------
// [CFXS] //

#include <driverlib/systick.h>

/*
/###########    /###########    \##\   /##/    /###########
############    ###########/     \##\ /##/     ###########/
###             ###               \##/##/      ###
###             ### #######        \/##/       ############
###             ### ######/        /##/#\      \###########
###             ###               /##/\##\              ###
############    ###              /##/  \##\    /###########
###########/    ###             /##/    \##\   ###########/
*/

#include <CFXS/Base/IPv4.hpp>
#include <CFXS/Base/MAC_Address.hpp>
#include <CFXS/IP/NetworkInterface.hpp>
#include <CFXS/IP/TransferBuffer.hpp>
#include <driverlib_includes.hpp>

namespace CFXS::Time {
    uint64_t ms;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
static constexpr CFXS::MAC_Address mac = {0x08, 0x00, 0x28, 0x5A, 0x92, 0x4F};
static constexpr CFXS::IPv4 ip         = {192, 168, 1, 222};
static constexpr CFXS::IPv4 mask       = {255, 255, 255, 0};
static constexpr CFXS::IPv4 gateway    = {255, 255, 255, 255};
//////////////////////////////////////////////////////////////////////////////////////////////////////////
static constexpr uint32_t Ports[] = {GPIO_PORTN_BASE, GPIO_PORTN_BASE, GPIO_PORTF_BASE, GPIO_PORTF_BASE};
static constexpr uint32_t Pins[]  = {GPIO_PIN_1, GPIO_PIN_0, GPIO_PIN_4, GPIO_PIN_0};
#define LED(id, state) GPIOPinWrite(Ports[id], Pins[id], (state) ? Pins[id] : 0)
#define TOGGLE_LED(id) GPIOPinWrite(Ports[id], Pins[id], !GPIOPinRead(Ports[id], Pins[id]) ? Pins[id] : 0)
//////////////////////////////////////////////////////////////////////////////////////////////////////////

extern void lwIPServiceTimers();

extern "C" void InputEvent() {
    TOGGLE_LED(2);
}

extern "C" {

uint8_t s_Heap[64][2048];
bool s_HeapUsage[64] = {false};

void *malloc(size_t size) {
    if (size <= 2048) {
        for (int i = 0; i < 64; i++) {
            if (s_HeapUsage[i] == false) {
                s_HeapUsage[i] = true;
                return s_Heap[i];
            }
        }
    } else {
        CFXS_println("buffer too big");
    }

    return nullptr;
}

void free(void *ptr) {
    if (!ptr)
        return;

    for (int i = 0; i < 64; i++) {
        if (ptr == s_Heap[i]) {
            s_HeapUsage[i] = false;
            return;
        }
    }
}
}

uint32_t s_BPS  = 0;
uint32_t s_Rate = 0;

int main() {
    CFXS_println("[CortexNetworkTestApp] main()");

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_GPION) || !ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)) {}
    for (int i = 0; i < 4; i++) {
        ROM_GPIOPinTypeGPIOOutput(Ports[i], Pins[i]);
        LED(i, 0);
    }

    CFXS::IP::NetworkInterface netif(mac, ip, mask, gateway);
    netif.InitializeHardware();

    IntRegister(INT_EMAC0, []() __interrupt {
        ROM_EMACIntClear(EMAC0_BASE, 0xFFFFFFFF);
        TOGGLE_LED(1);
        //lwIPEthernetIntHandler();
    });

    /*
    lwIPInit(CFXS::CPU::CLOCK_FREQUENCY,
             mac.GetDataPointer(),
             ip.ToNetworkOrder(),
             mask.ToNetworkOrder(),
             gateway.ToNetworkOrder(),
             IPADDR_USE_STATIC);*/

    SysTickPeriodSet(120000);
    SysTickIntRegister([]() __interrupt {
        CFXS::Time::ms++;
        //if ((CFXS::Time::ms % 100) == 0) {
        //lwIPServiceTimers();
        //}
        if ((CFXS::Time::ms % 1000) == 0) {
            s_Rate = s_BPS;
            s_BPS  = 0;
        }
    });
    SysTickEnable();
    SysTickIntEnable();

    CFXS::CPU::EnableInterrupts();

    while (1 < 2) {
        LED(0, (CFXS::Time::ms / 250) & 1);
    }
}
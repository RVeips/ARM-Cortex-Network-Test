// CFXS ARM Network Test App <https://github.com/RVeips/ARM-Cortex-Network-Test>
// Copyright (C) 2021 - CFXS / Rihards Veips
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>
// [CFXS] //
#include <driverlib_includes.hpp>

namespace CFXS::CPU {
    extern const uint32_t CLOCK_FREQUENCY;
} // namespace CFXS::CPU

// Empty handler for Tiva lib ASSERT
extern "C" void __error__(char *pcFilename, uint32_t ui32Line) {
    __asm__("bkpt");
}

/////////////////////////////////////////////////////////////
// Externals
extern const uint32_t __STACK_BASE__;
extern const uint32_t __TEXT_END__;
extern uint32_t __DATA_START__;
extern const uint32_t __DATA_END__;
extern const uint32_t __BSS_START__;
extern const uint32_t __BSS_END__;
extern void (*const __PREINIT_ARRAY_START__[])(void);
extern void (*const __PREINIT_ARRAY_END__[])(void);
extern void (*const __INIT_ARRAY_START__[])(void);
extern void (*const __INIT_ARRAY_END__[])(void);
extern void (*const __FINI_ARRAY_START__[])(void);
extern void (*const __FINI_ARRAY_END__[])(void);
/////////////////////////////////////////////////////////////
// Default startup

__weak __used void __cfxs_entry_point() {
    extern int main();
    main();
    __asm__("bkpt");
}

/////////////////////////////////////////////////////////////
// Default Handlers

static __interrupt void __interrupt_NMI(void) {
    __asm__("bkpt");
}
static __interrupt void __interrupt_HardFault(void) {
    __asm__("bkpt");
}

static __interrupt void __interrupt_DefaultHandler(void) {
    __asm__("bkpt");
}

static __interrupt void __interrupt_Reset() {
    CFXS::CPU::DisableInterrupts();

    // Enable FPU
    HWREG(NVIC_CPAC) = ((HWREG(NVIC_CPAC) & ~(NVIC_CPAC_CP10_M | NVIC_CPAC_CP11_M)) | NVIC_CPAC_CP10_FULL | NVIC_CPAC_CP11_FULL);

    // Configure clock (for fast init)
    SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), CFXS::CPU::CLOCK_FREQUENCY);

    // const init
    auto pui32Src = &__TEXT_END__;
    for (auto pui32Dest = &__DATA_START__; pui32Dest < &__DATA_END__;) {
        *pui32Dest++ = *pui32Src++;
    }

    // zero init
    __asm__("    ldr     r0, =__BSS_START__ \n"
            "    ldr     r1, =__BSS_END__   \n"
            "    mov     r2, #0             \n"
            "zeroinit_loop:                 \n"
            "        cmp     r0, r1         \n"
            "        it      lt             \n"
            "        strlt   r2, [r0], #4   \n"
            "        blt     zeroinit_loop    ");

    // ctor init
    for (auto i = 0; i < __PREINIT_ARRAY_END__ - __PREINIT_ARRAY_START__; i++) {
        __PREINIT_ARRAY_START__[i]();
    }

    for (auto i = 0; i < __INIT_ARRAY_END__ - __INIT_ARRAY_START__; i++) {
        __INIT_ARRAY_START__[i]();
    }

    __cfxs_entry_point();
}

////////////////////////////////////////////////////////////////////////////////////////
// Default Vector Table
__attribute__((section(".vector_table"), used)) void (*const __vector_table[])(void) = {
    (void (*)(void))(&__STACK_BASE__), // Initial stack pointer
    __interrupt_Reset,                 // Reset handler
    __interrupt_NMI,                   // NMI handler
    __interrupt_HardFault,             // Hard fault handler
    __interrupt_DefaultHandler,        // MPU fault handler
    __interrupt_DefaultHandler,        // Bus fault handler
    __interrupt_DefaultHandler,        // Usage fault handler
    nullptr,                           // Reserved
    nullptr,                           // Reserved
    nullptr,                           // Reserved
    nullptr,                           // Reserved
    __interrupt_DefaultHandler,        // SVCall handler
    __interrupt_DefaultHandler,        // Debug monitor handler
    nullptr,                           // Reserved
    __interrupt_DefaultHandler,        // The PendSV handler
    __interrupt_DefaultHandler,        // The SysTick handler
    __interrupt_DefaultHandler,        // GPIO Port A
    __interrupt_DefaultHandler,        // GPIO Port B
    __interrupt_DefaultHandler,        // GPIO Port C
    __interrupt_DefaultHandler,        // GPIO Port D
    __interrupt_DefaultHandler,        // GPIO Port E
    __interrupt_DefaultHandler,        // UART0 Rx and Tx
    __interrupt_DefaultHandler,        // UART1 Rx and Tx
    __interrupt_DefaultHandler,        // SSI0 Rx and Tx
    __interrupt_DefaultHandler,        // I2C0 Master and Slave
    __interrupt_DefaultHandler,        // PWM Fault
    __interrupt_DefaultHandler,        // PWM Generator 0
    __interrupt_DefaultHandler,        // PWM Generator 1
    __interrupt_DefaultHandler,        // PWM Generator 2
    __interrupt_DefaultHandler,        // Quadrature Encoder 0
    __interrupt_DefaultHandler,        // ADC Sequence 0
    __interrupt_DefaultHandler,        // ADC Sequence 1
    __interrupt_DefaultHandler,        // ADC Sequence 2
    __interrupt_DefaultHandler,        // ADC Sequence 3
    __interrupt_DefaultHandler,        // Watchdog timer
    __interrupt_DefaultHandler,        // Timer 0 subtimer A
    __interrupt_DefaultHandler,        // Timer 0 subtimer B
    __interrupt_DefaultHandler,        // Timer 1 subtimer A
    __interrupt_DefaultHandler,        // Timer 1 subtimer B
    __interrupt_DefaultHandler,        // Timer 2 subtimer A
    __interrupt_DefaultHandler,        // Timer 2 subtimer B
    __interrupt_DefaultHandler,        // Analog Comparator 0
    __interrupt_DefaultHandler,        // Analog Comparator 1
    __interrupt_DefaultHandler,        // Analog Comparator 2
    __interrupt_DefaultHandler,        // System Control (PLL, OSC, BO)
    __interrupt_DefaultHandler,        // FLASH Control
    __interrupt_DefaultHandler,        // GPIO Port F
    __interrupt_DefaultHandler,        // GPIO Port G
    __interrupt_DefaultHandler,        // GPIO Port H
    __interrupt_DefaultHandler,        // UART2 Rx and Tx
    __interrupt_DefaultHandler,        // SSI1 Rx and Tx
    __interrupt_DefaultHandler,        // Timer 3 subtimer A
    __interrupt_DefaultHandler,        // Timer 3 subtimer B
    __interrupt_DefaultHandler,        // I2C1 Master and Slave
    __interrupt_DefaultHandler,        // CAN0
    __interrupt_DefaultHandler,        // CAN1
    __interrupt_DefaultHandler,        // Ethernet
    __interrupt_DefaultHandler,        // Hibernate
    __interrupt_DefaultHandler,        // USB0
    __interrupt_DefaultHandler,        // PWM Generator 3
    __interrupt_DefaultHandler,        // uDMA Software Transfer
    __interrupt_DefaultHandler,        // uDMA Error
    __interrupt_DefaultHandler,        // ADC1 Sequence 0
    __interrupt_DefaultHandler,        // ADC1 Sequence 1
    __interrupt_DefaultHandler,        // ADC1 Sequence 2
    __interrupt_DefaultHandler,        // ADC1 Sequence 3
    __interrupt_DefaultHandler,        // External Bus Interface 0
    __interrupt_DefaultHandler,        // GPIO Port J
    __interrupt_DefaultHandler,        // GPIO Port K
    __interrupt_DefaultHandler,        // GPIO Port L
    __interrupt_DefaultHandler,        // SSI2 Rx and Tx
    __interrupt_DefaultHandler,        // SSI3 Rx and Tx
    __interrupt_DefaultHandler,        // UART3 Rx and Tx
    __interrupt_DefaultHandler,        // UART4 Rx and Tx
    __interrupt_DefaultHandler,        // UART5 Rx and Tx
    __interrupt_DefaultHandler,        // UART6 Rx and Tx
    __interrupt_DefaultHandler,        // UART7 Rx and Tx
    __interrupt_DefaultHandler,        // I2C2 Master and Slave
    __interrupt_DefaultHandler,        // I2C3 Master and Slave
    __interrupt_DefaultHandler,        // Timer 4 subtimer A
    __interrupt_DefaultHandler,        // Timer 4 subtimer B
    __interrupt_DefaultHandler,        // Timer 5 subtimer A
    __interrupt_DefaultHandler,        // Timer 5 subtimer B
    __interrupt_DefaultHandler,        // FPU
    nullptr,                           // Reserved
    nullptr,                           // Reserved
    __interrupt_DefaultHandler,        // I2C4 Master and Slave
    __interrupt_DefaultHandler,        // I2C5 Master and Slave
    __interrupt_DefaultHandler,        // GPIO Port M
    __interrupt_DefaultHandler,        // GPIO Port N
    nullptr,                           // Reserved
    __interrupt_DefaultHandler,        // Tamper
    __interrupt_DefaultHandler,        // GPIO Port P (Summary or P0)
    __interrupt_DefaultHandler,        // GPIO Port P1
    __interrupt_DefaultHandler,        // GPIO Port P2
    __interrupt_DefaultHandler,        // GPIO Port P3
    __interrupt_DefaultHandler,        // GPIO Port P4
    __interrupt_DefaultHandler,        // GPIO Port P5
    __interrupt_DefaultHandler,        // GPIO Port P6
    __interrupt_DefaultHandler,        // GPIO Port P7
    __interrupt_DefaultHandler,        // GPIO Port Q (Summary or Q0)
    __interrupt_DefaultHandler,        // GPIO Port Q1
    __interrupt_DefaultHandler,        // GPIO Port Q2
    __interrupt_DefaultHandler,        // GPIO Port Q3
    __interrupt_DefaultHandler,        // GPIO Port Q4
    __interrupt_DefaultHandler,        // GPIO Port Q5
    __interrupt_DefaultHandler,        // GPIO Port Q6
    __interrupt_DefaultHandler,        // GPIO Port Q7
    __interrupt_DefaultHandler,        // GPIO Port R
    __interrupt_DefaultHandler,        // GPIO Port S
    __interrupt_DefaultHandler,        // SHA/MD5 0
    __interrupt_DefaultHandler,        // AES 0
    __interrupt_DefaultHandler,        // DES3DES 0
    __interrupt_DefaultHandler,        // LCD Controller 0
    __interrupt_DefaultHandler,        // Timer 6 subtimer A
    __interrupt_DefaultHandler,        // Timer 6 subtimer B
    __interrupt_DefaultHandler,        // Timer 7 subtimer A
    __interrupt_DefaultHandler,        // Timer 7 subtimer B
    __interrupt_DefaultHandler,        // I2C6 Master and Slave
    __interrupt_DefaultHandler,        // I2C7 Master and Slave
    __interrupt_DefaultHandler,        // HIM Scan Matrix Keyboard 0
    __interrupt_DefaultHandler,        // One Wire 0
    __interrupt_DefaultHandler,        // HIM PS/2 0
    __interrupt_DefaultHandler,        // HIM LED Sequencer 0
    __interrupt_DefaultHandler,        // HIM Consumer IR 0
    __interrupt_DefaultHandler,        // I2C8 Master and Slave
    __interrupt_DefaultHandler,        // I2C9 Master and Slave
    __interrupt_DefaultHandler         // GPIO Port T
};
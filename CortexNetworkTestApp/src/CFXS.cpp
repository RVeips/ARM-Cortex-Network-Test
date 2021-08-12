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
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
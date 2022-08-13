
/* Specify the memory areas */

MEMORY
{
    RAM            (xrw) : ORIGIN = 0x20000000,             LENGTH = 640K - 4K
    RAM_SEGGER_RTT (xrw) : ORIGIN = 0x20000000 + 640K - 4K, LENGTH = 4K

    VECTOR_TABLE   (rx)  : ORIGIN = 0x08000000,             LENGTH = 570
    FLASH          (rx)  : ORIGIN = 0x08000240,             LENGTH = 2048K - 570
}

#include <src/L4-hal/stm32l4/linker_common.ld.inc>
